#ifndef D8AB2E39_84B7_48C6_9D5F_3015EF5742CF
#define D8AB2E39_84B7_48C6_9D5F_3015EF5742CF

#include <tc/client/tcp/Client.h>
#include <tc/asio/AsioService.h>
#include <tc/server/http/Cache.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <tc/server/http/Request.h>
#include <mini/ini.h>
#include <filesystem>
#include <chrono>
#include <spdlog/sinks/stdout_color_sinks.h>

void sleep_for(uint64_t time) {
	std::this_thread::sleep_for(chrono::milliseconds(time));
}

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
		LG_ERR(log.logger(), "Unable to read INI config file.");
		return 1;
	}

	auto http_port = std::stoi(ini["http"]["port"]);
	if (!ini["http"].has("port") || !vIsPortNumber(http_port)) {
		LG_ERR(log.logger(), "Invalid or missing HTTP port number.");
		return 1;
	}

	auto tcp_port = std::stoi(ini["tcp"]["port"]);
	if (!ini["tcp"].has("port") || !vIsPortNumber(tcp_port)) {
		LG_ERR(log.logger(), "Invalid or missing TCP port number.");
		return 1;
	}

	auto &addr = ini["tcp"]["address"];
	if (!ini["tcp"].has("address") || !vIsAddress(addr)) {
		LG_ERR(log.logger(), "Invalid or missing server address.");
		return 1;
	}

	auto interval = std::stoi(ini["tcp"]["interval"]);
	if (!ini["tcp"].has("interval")) {
		LG_ERR(log.logger(), "Invalid or missing interval.");
		return 1;
	}

	// Create Cache for data
	Signal<Imei, std::string> signal_cmd;
	auto cache = std::make_shared<server::http::Cache>(signal_cmd);

	// Create a new Asio service
	auto service = std::make_shared<tc::asio::AsioService>();
	service->Start();
	LG_NFO(log.logger(), "Asio service started!");

	// Create and connect signal from tcp client
	Signal<const void *, size_t> signal;
 	signal.connect([&](const void * buf, size_t size) {
    cache->onReceived(buf, size);
  });

	// Create a new TCP client
	auto client = std::make_shared< client::tcp::Client >(signal, service, addr, tcp_port);

	// connect cache signal
	signal_cmd.connect([&](Imei imei, std::string cmd) {
    client->send(imei, cmd);
  });

	// Create a new HTTPS server
	auto server = std::make_shared<server::http::HTTPCacheServer>(service, http_port, cache);
	if (!server->Start()) {
		LG_ERR(log.logger(), "Unable to start HTTP server.");
		return 1;
	}
	LG_NFO(log.logger(), "HTTP Server started!");


	// Connect client to telematics server
	if(!client->ConnectAsync()) {
		LG_ERR(log.logger(), "TCP client connect error.");
		return 1;
	}
	LG_NFO(log.logger(), "TCP client connected to [{}][{}].", addr, tcp_port);

	while (true) {
		if (client->IsConnected() == false) {
			LG_NFO(log.logger(), "Retrying connection to telematics server...");
			client->ConnectAsync();
			sleep_for(interval);
			continue;
		}

		CppServer::HTTP::HTTPRequest req("GET", "/devices", "HTTP/1.1");
		server::http::Request request(req);
		parser::Buf buf;
		if (request.toInternal(buf) != RES_OK) {
			LG_ERR(log.logger(), "Unable to convert to internal request");
			continue;
		}

		if (client->send(buf) != RES_OK)	{
			LG_ERR(log.logger(), "Unable to send buffer");
			client->DisconnectAsync();
		}


		LG_NFO(log.logger(), "Cached: {}", cache->getDevices());
		sleep_for(interval);
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
