#include <tc/server/tcp/asio/AsioService.h>
#include <tc/server/tcp/session/Session.h>
#include <tc/server/tcp/server/Server.h>
#include <tc/common/Logger.h>

#include <chrono>

// core dumps may be disallowed by parent of this process; change that

using namespace tc;

int main(int argc, char** argv)
{
	auto logger = spdlog::stdout_color_mt("console");

	common::LogI log(logger);
	SPDLOG_LOGGER_INFO(log.logger(), "TCP server port");

	spdlog::set_default_logger(log.logger());

	// TCP server port
	int port = 8883;

	SPDLOG_LOGGER_INFO(log.logger(), "TCP server port: {}", port);

	// Create a new Asio service
	auto service = std::make_shared< tc::server::tcp::AsioService >(1);

	// Start the Asio service
	SPDLOG_LOGGER_INFO(log.logger(), "Asio service starting...");
	if (service->Start() != true) {
		SPDLOG_LOGGER_ERROR(log.logger(), "Unable to start asio service. Exiting...");
		return 1;
	}
	SPDLOG_LOGGER_INFO(log.logger(), "Done!");

	// Create a new TCP server
	auto server = std::make_shared< tc::server::tcp::Server >(service, port);

	// Start the server
	SPDLOG_LOGGER_INFO(log.logger(), "TCP Server starting...");
	if (server->Start() != true) {
		SPDLOG_LOGGER_ERROR(log.logger(), "Unable to start server. Exiting...");
		return 1;
	}
	SPDLOG_LOGGER_INFO(log.logger(), "Done!");


	while (true) {
		CppCommon::Thread::Sleep(1000);
		////spdlog::info("Alive! server address: {} connected sessions: {} threads: {} IsPolling: {} IsStarted: {}",
		//server->address(), (int) server->connected_sessions(), (int) service->threads(), (int) service->IsPolling(), (int) service->IsStarted());

		server->printSessions();
	}

	// Stop the server
	SPDLOG_LOGGER_INFO(log.logger(), "Server stopping...");
	server->Stop();
	SPDLOG_LOGGER_INFO(log.logger(), "Done!");

	// Stop the Asio service
	SPDLOG_LOGGER_INFO(log.logger(), "Asio service stopping...");
	service->Stop();
	SPDLOG_LOGGER_INFO(log.logger(), "Done!");

	return 0;
}
