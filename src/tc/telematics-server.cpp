#include <tc/server/tcp/TelematicsServer.h>
#include <tc/server/tcp/AsioService.h>
#include <tc/common/Common.h>

// core dumps may be disallowed by parent of this process; change that

int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");

	tc::LogI log(logger);
	spdlog::set_default_logger(log.logger());

	// TCP server port
	int port = 8883;

	LG_NFO(log.logger(), "TCP server port: {}", port);

	// Create a new Asio service
	const auto service = std::make_shared< tc::server::tcp::AsioService >();

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
	server->SetupNoDelay(true);
	// Start the server
	LG_NFO(log.logger(), "TCP Server starting...");
	if (server->Start() != true) {
		LG_ERR(log.logger(), "Unable to start server. Exiting...");
		return 1;
	}
	LG_NFO(log.logger(), "Done!");


	while (true) {
		//sleep(1);
		CppCommon::Thread::Sleep(500);
		////spdlog::info("Alive! server address: {} connected sessions: {} threads: {} IsPolling: {} IsStarted: {}",
		//server->address(), (int) server->connected_sessions(), (int) service->threads(), (int) service->IsPolling(), (int) service->IsStarted());
		LG_NFO(log.logger(), "sessionsSize: {} payloadPackets size {} service threads: {}", server->sessionsSize(), (int) server->payloadPackets().size(), service->threads());
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
