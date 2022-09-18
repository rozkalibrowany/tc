#ifndef D8AB2E39_84B7_48C6_9D5F_3015EF5742CF
#define D8AB2E39_84B7_48C6_9D5F_3015EF5742CF
#include <tc/server/http/Client.h>
#include <tc/asio/AsioService.h>
#include <tc/server/http/Cache.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tc/parser/ReqType.h>


int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");

	tc::LogI log(logger);
	spdlog::set_default_logger(log.logger());

	// HTTPS server port
	int port = 8443;

	// TCP server port
	int tcp_port = 8883;

	if (argc > 1)
			port = std::atoi(argv[1]);
	std::string address = "127.0.0.1";


	LG_NFO(log.logger(), "HTTP server port: {}", 8443);

	// Create a new Asio service
	auto service = std::make_shared<tc::asio::AsioService>();

	// Start the Asio service
	LG_NFO(log.logger(), "Asio service starting...");
	service->Start();
	//std::cout << "Done!" << std::endl;

	// Create Cache for data
	auto cache = std::make_shared<tc::server::http::Cache>();

	// Create a new HTTPS server
	auto server = std::make_shared<tc::server::http::HTTPCacheServer>(service, port);
	server->setCache(cache);
	// Start the server
	LG_NFO(log.logger(), "Server starting...");
	server->Start();
	LG_NFO(log.logger(), "Done!");

	// Create a new TCP client
	auto client = std::make_shared< tc::server::http::Client >(service, address, tcp_port);
	client->setCache(cache);

	// Connect the client
	LG_NFO(log.logger(), "Client connecting...");
	if(!client->ConnectAsync()) {
		LG_ERR(log.logger(), "Connect async");
		return 1;
	}

	while (true) {
		if (client->IsConnected() == false) {
			client->ConnectAsync();
			continue;
		}

		if (cache->hasCommands() == true) {
			auto cmd = cache->getCommand();
			client->handle(cmd);
		}

		tc::server::http::Action action;
		if (action.parse( tc::parser::PacketRequest::Devices, tc::parser::PacketRequest::GET) != tc::RES_OK) {
			LG_WRN(log.logger(), "Unable to get devices from Telematics Connector");
			continue;
		}
		if (client->handle(action) != tc::RES_OK) {
			LG_DBG(log.logger(), "Unable to handle action");
			client->DisconnectAsync();
		}

		LG_NFO(log.logger(), "Cached: {}", cache->getDevices().toStyledString());

		CppCommon::Thread::Sleep(1500);
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
