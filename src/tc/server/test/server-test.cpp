#include <tc/server/tcp/asio/AsioService.h>
#include <tc/server/tcp/session/Session.h>
#include <server/asio/tcp_server.h>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <bitset>


constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

using namespace tc::server::tcp::session;

class ChatServer : public CppServer::Asio::TCPServer
{
public:
    using CppServer::Asio::TCPServer::TCPServer;

protected:
    std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(const std::shared_ptr<CppServer::Asio::TCPServer>& server) override
    {
        return std::make_shared<Session>(server);
    }

protected:
    void onError(int error, const std::string& category, const std::string& message) override
    {
        std::cout << "Chat TCP server caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
    }
};

int main(int argc, char** argv)
{
    // TCP server port
    int port = 8883;
    if (argc > 1)
        port = std::atoi(argv[1]);
    port = 8883;

    std::cout << "TCP server port: " << port << std::endl;

    std::cout << std::endl;

    // Create a new Asio service
    auto service = std::make_shared<AsioService>();

    // Start the Asio service
    std::cout << "Asio service starting...";
    service->Start();
    std::cout << "Done!" << std::endl;

    // Create a new TCP chat server
    auto server = std::make_shared<ChatServer>(service, port);

    // Start the server
    std::cout << "Server starting...";
    server->Start();
    std::cout << "Done!" << std::endl;

    std::cout << "Press Enter to stop the server or '!' to restart the server..." << std::endl;

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line.empty())
            break;

        // Restart the server
        if (line == "!")
        {
            std::cout << "Server restarting...";
            server->Restart();
            std::cout << "Done!" << std::endl;
            continue;
        }

        // Multicast admin message to all sessions
        line = "(admin) " + line;
        server->Multicast(line);
    }

    // Stop the server
    std::cout << "Server stopping...";
    server->Stop();
    std::cout << "Done!" << std::endl;

    // Stop the Asio service
    std::cout << "Asio service stopping...";
    service->Stop();
    std::cout << "Done!" << std::endl;

    return 0;
}
