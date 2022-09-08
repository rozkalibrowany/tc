#include <spdlog/sinks/stdout_color_sinks.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/asio/AsioService.h>

// core dumps may be disallowed by parent of this process; change that

int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");
	logger->set_level(spdlog::level::debug);

	tc::LogI log(logger);
	spdlog::set_default_logger(log.logger());
	// TCP server port
	int port = 8883;

	LG_NFO(log.logger(), "TCP server port: {}", port);

	// Create a new Asio service
	const auto service = std::make_shared< tc::asio::AsioService >();

	// Start the Asio service
	LG_NFO(log.logger(), "Asio service starting...");
	if (service->Start() != true) {
		LG_ERR(log.logger(), "Unable to start asio service. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "Done!");

	// Create a new TCP server
	auto server = std::make_shared< tc::server::tcp::TelematicsServer >(service, port);
	server->SetupReusePort(true);
	// Start the server
	LG_NFO(log.logger(), "TCP Server starting...");
	if (server->Start() != true) {
		LG_ERR(log.logger(), "Unable to start server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "Done!");


	while (true) {
		CppCommon::Thread::Sleep(15000);
		LG_NFO(log.logger(), "Alive! connected sessions: {} threads: {} IsPolling: {} IsStarted: {}",
		(int) server->connected_sessions(), (int) service->threads(), (int) service->IsPolling(), (int) service->IsStarted());
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
