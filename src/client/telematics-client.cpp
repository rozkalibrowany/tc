#include <tc/common/Common.h>
#include <tc/client/AsioService.h>
#include <tc/client/TelematicsClient.h>
#include <tc/client/CommandFactory.h>
#include <args-parser/all.hpp>

// core dumps may be disallowed by parent of this process; change that

using namespace tc;
using namespace Args;

int main(int argc, char** argv)
{
	std::string command, imei;
	std::string address = "127.0.0.1:8883";
	int port;

	try {
		CmdLine cmd(argc, argv);

		cmd.addArgWithFlagAndName('a', "address", true, false, "TCP Server address",
															"", "127.0.0.1:8883", "addr:port")
				.addArgWithFlagAndName('i', "imei", true, true, "IoT device IMEI",
																"", "", "arg")
				.addArgWithFlagAndName('c', "cmd", true, true, "IoT command",
																"", "", "lock|unlock")
				.addHelp(true, argv[0], "Telematics Connector TCP Command Client.");

		cmd.parse();
		port = std::stoi(address.substr(address.find(":") + 1));

		if (cmd.isDefined("-a"))
			address = cmd.value("-a");

		if (cmd.isDefined("-i"))
			imei = cmd.value("-i");

		if (cmd.isDefined("-c"))
			command = cmd.value("-c");
	} catch( const HelpHasBeenPrintedException & )
  {
    return 0;
  } catch( const BaseException & x )
  {
    outStream() << x.desc() << "\n";
    return 1;
  }

	auto logger = spdlog::stdout_color_mt("console");
	tc::LogI log(logger);
	spdlog::set_default_logger(log.logger());

	if (port > 65535 || port < 0) {
		LG_ERR(log.logger(), "Invalid port.");
		return 1;
	}

	if (std::count(address.begin(), address.end(), '.') != 3) {
		LG_ERR(log.logger(), "Invalid address.");
		return 1;
	}

	LG_NFO(log.logger(), "TCP server address", address);

	client::tcp::CommandFactory factory(imei);
	std::map<int, std::vector < uchar >> cmdMap;

	if (factory.create(cmdMap, command) != RES_OK) {
		LG_ERR(log.logger(), "Packet creation failed.");
		return 1;
	}

	// Create a new Asio service
	auto service = std::make_shared< client::tcp::AsioService >();

	// Start the Asio service
	LG_NFO(log.logger(), "Asio service starting...");
	service->Start();
	LG_NFO(log.logger(), "Done!");

	// Create a new TCP chat client
	auto client = std::make_shared< client::tcp::TelematicsClient >(service, address, port);

	// Connect the client
	LG_NFO(log.logger(), "Client connecting...");
	//client->ConnectAsync();
	LG_NFO(log.logger(), "Done!");


	while (true) {
		client->IsConnected() ? client->ReconnectAsync() : client->ConnectAsync();
		CppCommon::Thread::Sleep(4000);
		break;
	}

	// Disconnect the client
	LG_NFO(log.logger(), "Client disconnecting...");
	//client->disconnectAndStop();
	LG_NFO(log.logger(), "Done!");

	// Stop the Asio service
	LG_NFO(log.logger(), "Asio service stopping...");
	service->Stop();
	LG_NFO(log.logger(), "Done!");

	return 0;
}
