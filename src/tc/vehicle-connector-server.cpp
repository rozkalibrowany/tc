#ifndef D8AB2E39_84B7_48C6_9D5F_3015EF5742CF
#define D8AB2E39_84B7_48C6_9D5F_3015EF5742CF

#include <tc/server/http/Client.h>
#include <tc/asio/AsioService.h>
#include <tc/server/http/Cache.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <mini/ini.h>
#include <filesystem>
#include <chrono>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char** argv)
{
	using namespace tc;
	using namespace mINI;

	auto logger = spdlog::stdout_color_mt("console");

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

	auto http_port = std::stoi(ini["http"]["port"]);
	if (!ini["http"].has("port") || !vIsPortNumber(http_port)) {
		LG_ERR(log.logger(), "Invalid or missing HTTP port number. Exiting...");
		return 1;
	}

	auto tcp_port = std::stoi(ini["tcp"]["port"]);
	if (!ini["tcp"].has("port") || !vIsPortNumber(tcp_port)) {
		LG_ERR(log.logger(), "Invalid or missing TCP port number. Exiting...");
		return 1;
	}

	auto &addr = ini["tcp"]["address"];
	if (!ini["tcp"].has("address") || !vIsAddress(addr)) {
		LG_ERR(log.logger(), "Invalid or missing server address. Exiting...");
		return 1;
	}

	auto interval = std::stoi(ini["tcp"]["interval"]);
	if (!ini["tcp"].has("interval")) {
		LG_ERR(log.logger(), "Invalid or missing interval. Exiting...");
		return 1;
	}

	// Create a new Asio service
	auto service = std::make_shared<tc::asio::AsioService>();
	service->Start();
	LG_NFO(log.logger(), "Asio service running!");

	// Create Cache for data
	auto cache = std::make_shared<server::http::Cache>();

	// Create a new HTTPS server
	auto server = std::make_shared<server::http::HTTPCacheServer>(service, http_port);
	server->setCache(cache);
	// Start the server
	if (!server->Start()) {
		LG_ERR(log.logger(), "Unable to start HTTP server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "HTTP Server running!");

	// Create a new TCP client
	auto client = std::make_shared< server::http::Client >(service, addr, tcp_port);
	client->setCache(cache);

	// Connect the client
	if(!client->ConnectAsync()) {
		LG_ERR(log.logger(), "TCP client connect async");
		return 1;
	}
	LG_NFO(log.logger(), "TCP client running!");

	while (true) {
		if (client->IsConnected() == false) {
			client->ConnectAsync();
			continue;
		}

		if (cache->hasCommands() == true) {
			auto cmd = cache->getCommand();
			client->handle(cmd);
		}
		std::string url = std::string("http://localhost:") + std::to_string(tcp_port) + std::string("/devices");
		CppServer::HTTP::HTTPRequest req("GET", url, "HTTP/1.1");
		server::http::Action action;
		if (action.parse(req) != RES_OK) {
			LG_WRN(log.logger(), "Unable to get devices from Telematics Connector");
			continue;
		}
		if (client->handle(action) != RES_OK) {
			LG_DBG(log.logger(), "Unable to handle action");
			client->DisconnectAsync();
		}

		using milliseconds = std::chrono::milliseconds;
		milliseconds interv = 2000ms;
		std::this_thread::sleep_for(interv);
	}
	// Stop the server

	server->Stop();
	LG_NFO(log.logger(), "Done!");

	// Stop the Asio service
	LG_NFO(log.logger(), "Asio service stopping...");
	service->Stop();
	LG_NFO(log.logger(), "Done!");

	return 0;
}


#endif /* D8AB2E39_84B7_48C6_9D5F_3015EF5742CF */
