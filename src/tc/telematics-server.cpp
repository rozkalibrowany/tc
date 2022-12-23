#include <spdlog/sinks/stdout_color_sinks.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/asio/AsioService.h>
#include <tc/db/Client.h>
#include <mini/ini.h>
#include <filesystem>

// core dumps may be disallowed by parent of this process; change that


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
	const auto service = std::make_shared< server::tcp::AsioService >();
	if (service->Start() != true) {
		LG_ERR(log.logger(), "Unable to start asio service. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "Asio service running!");

	// Create DB client
	auto &s_uri = ini["db"]["uri"];
	auto db_client = std::make_shared< server::db::Client >(s_uri);
	if (!db_client->load(ini)) {
		LG_ERR(log.logger(), "Unable to parse db client config. Exiting...");
		return 1;
	}

	if (db_client->enabled()) {
		std::vector<std::string> collections = db_client->client()["db"].list_collection_names();
		if (std::find(collections.begin(), collections.end(), db_client->collection("collection_packets")) == collections.end()) {
			db_client->client()["db"].create_collection(db_client->collection("collection_packets"));
		}
		LG_NFO(log.logger(), "DB enabled and client connected. Database: {}, collection: {}", db_client->name(), db_client->collection("collection_packets"));
	}

	// Create a new TCP server
	auto server = std::make_shared< server::tcp::TelematicsServer >(service, db_client, cache, port, addr);
	server->SetupReusePort(true);

	if (server->Start() != true) {
		LG_ERR(log.logger(), "Unable to start TCP server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "TCP server running on port {}", port);


	while (true) {
		CppCommon::Thread::Sleep(15000);
		LG_NFO(log.logger(), "Alive! connected sessions: {} threads: {} IsPolling: {} IsStarted: {}",
		(int) server->connected_sessions(), (int) service->threads(), (int) service->IsPolling(), (int) service->IsStarted());
	}

	// Stop the server
	server->Stop();
	LG_NFO(log.logger(), "Server stopped...");

	// Stop the Asio service
	service->Stop();
	LG_NFO(log.logger(), "Asio service stopped...");


	return 0;
}
