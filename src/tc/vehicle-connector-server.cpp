#include <tc/server/http/Client.h>
#include <tc/asio/AsioService.h>
#include <tc/server/http/Cache.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <tc/common/Common.h>

#include <map>
#include <mutex>


int main(int argc, char** argv)
{
	using PacketRequest = tc::parser::PacketRequest;
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


	LG_NFO(log.logger(), "HTTPS server port: {}", 8443);

	// Create a new Asio service
	auto service = std::make_shared<tc::asio::AsioService>();

	// Start the Asio service
	LG_NFO(log.logger(), "Asio service starting...");
	service->Start();
	//std::cout << "Done!" << std::endl;

	// Create and prepare a new SSL server context
	auto context = std::make_shared<CppServer::Asio::SSLContext>(asio::ssl::context::tlsv12);
	context->set_password_callback([](size_t max_length, asio::ssl::context::password_purpose purpose) -> std::string { return "qwerty"; });
	context->use_certificate_chain_file("/etc/ssl/certs/snakeoil.pem");
	context->use_private_key_file("/etc/ssl/certs/snakeoil.pem", asio::ssl::context::pem);
	//context->use_tmp_dh_file("../tools/certificates/dh4096.pem");

	// Create Cache for data
	auto cache = std::make_shared<tc::server::http::Cache>();

	// Create a new HTTPS server
	auto server = std::make_shared<tc::server::http::HTTPSCacheServer>(service, context, port);
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

		/*if (cache->hasCommands() == true) {
			auto buf = cache->getCommand();
			client->handle(buf);
		}*/

		tc::server::http::Action action;
		if (action.parse(PacketRequest::Devices, PacketRequest::GET) != tc::RES_OK) {
			LG_WRN(log.logger(), "Unable to get devices from Telematics Connector");
			continue;
		}
		if (client->handle(action) != tc::RES_OK) {
			LG_DBG(log.logger(), "Unable to handle action");
			client->DisconnectAsync();
		}

		LG_NFO(log.logger(), "Cached: {}", cache->getDevices().toStyledString());

		CppCommon::Thread::Sleep(5000);
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