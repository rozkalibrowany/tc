#include <tc/server/tcp/TelematicsServer.h>
#include <tc/asio/AsioService.h>
#include <tc/db/Client.h>
#include <mini/ini.h>
#include <filesystem>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
	using namespace tc;
	using namespace mINI;

	auto logger = spdlog::stdout_color_mt("console");
	logger->set_level(spdlog::level::debug);

	LogI log(logger);
	spdlog::set_default_logger(log.logger());

	if(!std::filesystem::exists(argv[1])) {
		LG_ERR(log.logger(), "Config file not exists. Exiting...");
		return 1;
	}

	INIFile file(argv[1]);
	INIStructure ini;

	if(!file.read(ini)) {
		LG_ERR(log.logger(), "Unable to read INI config file. Exiting...");
		return 1;
	}

	auto port = std::stoi(ini["server"]["port"]);
	if (!ini["server"].has("port") || !vIsPortNumber(port)) {
		LG_ERR(log.logger(), "Invalid or missing port number. Exiting...");
		return 1;
	}

	auto &addr = ini["server"]["address"];
	if (!ini["server"].has("address") || !vIsAddress(addr)) {
		LG_ERR(log.logger(), "Invalid or missing server address. Exiting...");
		return 1;
	}

	if (!ini["session"].has("cache")) {
		LG_ERR(log.logger(), "Missing session cache size. Exiting...");
		return 1;
	}
	size_t cache = static_cast<size_t>(std::stoi(ini["session"]["cache"]));

	// Create a new Asio service
	const auto service = std::make_shared< server::tcp::AsioService >(10);
	if (service->Start() != true) {
		LG_ERR(log.logger(), "Unable to start asio service. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "Asio service running!");

	// Create DB client
	auto &s_uri = ini["db"]["uri"];
	auto db_client = std::make_shared< db::mongo::Client >(s_uri);
	if (db_client->load(ini) != RES_OK) {
		LG_ERR(log.logger(), "Unable to parse db client config. Exiting...");
		return 1;
	}

	if (db_client->enabled()) {
		if(!db_client->has(db_client->collection())) {
			db_client->create(db_client->collection());
		}
		LG_NFO(log.logger(), "DB connected. Name: {}, collection: {}, uri: {}", db_client->name(), db_client->collection(), s_uri);
	}

	// Create a new TCP server
	auto server = std::make_shared< server::tcp::TelematicsServer >(service, db_client, cache, port, addr);
	if (server->Start() != true) {
		LG_ERR(log.logger(), "Unable to start TCP server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "TCP server running on port {}", port);


	while (true) {
		using milliseconds = std::chrono::milliseconds;
		milliseconds interv = 15000ms;
		std::this_thread::sleep_for(interv);
		LG_NFO(log.logger(), "Alive! connected sessions: {} threads: {} polling: {} started: {}",
		 server->connected_sessions(), service->threads(), service->IsPolling(), service->IsStarted());
	}

	// Stop the server
	server->Stop();
	LG_NFO(log.logger(), "Server stopped...");

	// Stop the Asio service
	service->Stop();
	LG_NFO(log.logger(), "Asio service stopped...");


	return 0;
}
