#include <tc/server/tcp/asio/AsioService.h>
#include <tc/server/tcp/session/Session.h>
#include <tc/server/tcp/server/Server.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#include "threads/thread.h"
#include <chrono>

// core dumps may be disallowed by parent of this process; change that


int main(int argc, char** argv)
{
    // TCP server port
    int port = 8883;
    spdlog::info("TCP server port: {}", port);

    // Create a new Asio service
    auto service = std::make_shared< tc::server::tcp::AsioService >(2);

    // Start the Asio service
		spdlog::info("Asio service starting...");
		service->Start();
		spdlog::info("Done!");

		// Create a new TCP server
    auto server = std::make_shared< tc::server::tcp::Server >(service, port);

    // Start the server
		spdlog::info("TCP Server starting...");
		server->Start();
		spdlog::info("Done!");


		while (true) {
			CppCommon::Thread::Sleep(1000);
			spdlog::info("Alive! server address: {} connected sessions: {} threads: {} IsPolling: {} IsStarted: {}",
			server->address(), (int) server->connected_sessions(), (int) service->threads(), (int) service->IsPolling(), (int) service->IsStarted());
		}

		// Stop the server
    spdlog::info("Server stopping...");
    server->Stop();
    spdlog::info("Done!");

    // Stop the Asio service
    spdlog::info("Asio service stopping...");
    service->Stop();
    spdlog::info("Done!");

    return 0;
}
