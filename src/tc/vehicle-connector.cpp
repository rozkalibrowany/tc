#include <tc/asio/AsioService.h>
#include <tc/server/http/Cache.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <tc/client/tcp/TelematicsClient.h>
#include <tc/common/Common.h>

#include <map>
#include <mutex>


int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");

	tc::LogI log(logger);
	spdlog::set_default_logger(log.logger());

	// HTTPS server port
	int port = 8443;
	int port_tcp = 8883;
	if (argc > 1)
		port = std::atoi(argv[1]);
	// HTTPS server content path
	/*std::string www = "../www/api";
	if (argc > 2)
			www = argv[2];*/


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
	context->use_certificate_chain_file("../tools/certificates/server.pem");
	context->use_private_key_file("../tools/certificates/server.pem", asio::ssl::context::pem);
	context->use_tmp_dh_file("../tools/certificates/dh4096.pem");


	std::string address = "127.0.0.1";
	// Create a new HTTPS server
	auto server = std::make_shared<tc::server::http::HTTPSCacheServer>(service, context, port);
	auto client = std::make_shared< tc::client::tcp::TelematicsClient >(service, address, port_tcp);
	//server->AddStaticContent(www, "/api");

	// Start the server
	LG_NFO(log.logger(), "Server starting...");
	server->Start();
	LG_NFO(log.logger(), "Done!");

	LG_NFO(log.logger(), "Client connecting...");
	if(!client->ConnectAsync()) {
		LG_ERR(log.logger(), "Connect async");
		return 1;
	}

	// Perform text input
	std::string line;
	while (getline(std::cin, line))
	{
			if (line.empty())
					break;

			// Restart the server
			if (line == "!")
			{
					LG_NFO(log.logger(), "Server restarting...");

					//std::cout << "";
					server->Restart();
					//std::cout << "Done!" << std::endl;
					continue;
			}
	}

	// Stop the server
	LG_NFO(log.logger(), "Server stopping...");
	server->Stop();
	LG_NFO(log.logger(), "Done!");

	// Stop the Asio service
	LG_NFO(log.logger(), "Asio service stopping...");
	service->Stop();
	LG_NFO(log.logger(), "Done!");

	return 0;
}