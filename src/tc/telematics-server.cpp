#include <tc/server/tcp/TelematicsServer.h>
#include <tc/asio/AsioService.h>
#include <tc/server/tcp/Clean.h>
#include <tc/db/Client.h>
#include <mini/ini.h>
#include <filesystem>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace tc;
using namespace mINI;

namespace defaults {
	constexpr int c_cache									 = 100;
	constexpr int c_default_clean_interval = 90000;
	constexpr int c_packets_days_lifetime  = 3;
};

result_t readTelematicsConfig(INIStructure &ini, LogI &log, std::string &addr, int &port, int &cache)
{
	if (!ini["server"].has("port")) {
		LG_ERR(log.logger(), "Missing HTTP port number.");
		return RES_NOENT;
	}		

	port = std::stoi(ini["server"]["port"]);
	if (!vIsPortNumber(port)) {
		LG_ERR(log.logger(), "Invalid TCP port number[{}].", port);
		return RES_NOENT;
	}

	if (!ini["server"].has("address")) {
		LG_ERR(log.logger(), "Missing TCP address.");
		return RES_NOENT;
	}

	addr = ini["server"]["address"];
	if (!vIsAddress(addr)) {
		LG_ERR(log.logger(), "Invalid TCP address[{}].", addr);
		return RES_NOENT;
	}

	cache = ini["server"].has("cache") ? std::stoi(ini["server"]["cache"]) : defaults::c_cache;

	return RES_OK;
}

result_t readDatabaseConfig(INIStructure &ini, LogI &log, std::string &uri, int &clean_interval, int &packets_days_lifetime)
{
	if (!ini["db"].has("uri")) {
		LG_WRN(log.logger(), "Missing DB URI address.");
	} else {
		uri = ini["db"]["uri"];
	}

	clean_interval = ini["db"].has("clean_interval") ? std::stoi(ini["db"]["clean_interval"]) : defaults::c_default_clean_interval;
	packets_days_lifetime = ini["db"].has("packets_days_lifetime") ? std::stoi(ini["db"]["packets_days_lifetime"]) : defaults::c_packets_days_lifetime;

	return RES_OK;
}

int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");
	logger->set_level(spdlog::level::debug);

	LogI log(logger);
	spdlog::set_default_logger(log.logger());

	if(!std::filesystem::exists(argv[1])) {
		LG_ERR(log.logger(), "Config file not exists. Exit...");
		return 1;
	}

	INIFile file(argv[1]);
	INIStructure ini;

	if(!file.read(ini)) {
		LG_ERR(log.logger(), "Unable to read INI config file. Exiting...");
		return 1;
	}

	std::string addr;
	int tcp_port, cache;
	if (readTelematicsConfig(ini, log, addr, tcp_port, cache) != RES_OK) {
		return 1;
	}

	std::string uri;
	int clean_interval, packets_days_lifetime;
	if (readDatabaseConfig(ini, log, uri, clean_interval, packets_days_lifetime) != RES_OK) {
		return 1;
	}

	// Create a new Asio service
	const auto service = std::make_shared< server::tcp::AsioService >(10);
	if (service->Start() != true) {
		LG_ERR(log.logger(), "Unable to start asio service. Exiting...");
		return 1;
	}

	// Create DB client
	auto &s_uri = ini["db"]["uri"];
	auto db_client = std::make_shared< db::mongo::Client >(s_uri, db::mongo::Client::ePackets);
	if (db_client->load(ini) != RES_OK) {
		LG_ERR(log.logger(), "Unable to parse db client config. Exiting...");
		return 1;
	}

	if (db_client->enabled())
		LG_NFO(log.logger(), "DB connected. Name: {}, collection: {}, uri: {}", db_client->name(), (std::string) db_client->collection(), s_uri);

	// Create a new TCP server
	auto server = std::make_shared< server::tcp::TelematicsServer >(service, db_client, cache, tcp_port, addr);
	if (server->Start() != true) {
		LG_ERR(log.logger(), "Unable to start TCP server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "TCP server running on port {}", tcp_port);

	// Sync devices into DB
	server::tcp::Clean clean;
	std::thread thread(&server::tcp::Clean::execute, &clean, db_client, clean_interval, packets_days_lifetime);
	thread.detach();

	while (true) {
		using milliseconds = std::chrono::milliseconds;
		milliseconds interv = 15000ms;
		std::this_thread::sleep_for(interv);
		LG_NFO(log.logger(), "Alive! connected sessions: {} threads: {} polling: {} started: {}",
		 server->connected_sessions(), service->threads(), service->IsPolling(), service->IsStarted());

		if (db_client->enabled()) {
			db_client->synchronizeTime(SysTime(true).timestamp());
			if(!db_client->has(db_client->collection())) {
				db_client->create(db_client->collection());
			}
		}
	}

	// Stop the server
	server->Stop();
	LG_NFO(log.logger(), "Server stopped...");

	// Stop the Asio service
	service->Stop();
	LG_NFO(log.logger(), "Asio service stopped...");


	return 0;
}
