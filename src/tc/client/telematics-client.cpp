#include <tc/common/Common.h>
#include <client/tcp/AsioService.h>
#include <client/tcp/TelematicsClient.h>
#include <client/tcp/CommandFactory.h>
#include <args-parser/all.hpp>
#include <vector>
// core dumps may be disallowed by parent of this process; change that

int main(int argc, char** argv)
{
	using namespace tc;
	using namespace Args;

	std::string command, imei, address;
	std::string address_full = "127.0.0.1:8883";
	int port;

	try {
		CmdLine cmd(argc, argv);

		cmd.addArgWithFlagAndName('a', "address", true, false, "TCP Server address",
															"", "127.0.0.1:8883", "addr:port")
				.addArgWithFlagAndName('i', "imei", true, true, "IoT device IMEI",
																"", "", "arg")
				.addArgWithFlagAndName('c', "cmd", true, true, "IoT command",
																"", "", "lock|unlock|engine_on|engine_off")
				.addHelp(true, argv[0], "Telematics Connector TCP Command Client.");

		cmd.parse();
		port = std::stoi(address_full.substr(address_full.find(":") + 1));

		if (cmd.isDefined("-a"))
			address_full = cmd.value("-a");

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

	if (std::count(address_full.begin(), address_full.end(), '.') != 3) {
		LG_ERR(log.logger(), "Invalid address.");
		return 1;
	}

	address = address_full.substr(0, address_full.find(":", 0));

	client::tcp::CommandFactory factory(imei);
	std::string str_packet = factory.create(command);
	std::vector<char> buf(str_packet.size() / 2);

	std::transform(str_packet.begin(), str_packet.end(), str_packet.begin(), ::toupper);

	hex2bin((const char*) str_packet.data(), buf.data());


	//std::copy(buf.begin(), buf.end(), std::back_inserter(ubuf));

	// Create a new Asio service
	auto service = std::make_shared<client::tcp::AsioService>();

	// Start the Asio service
	LG_NFO(log.logger(), "Asio service starting...");
	if(!service->Start()) {
		LG_ERR(log.logger(), "service start");
		return 1;
	}
	LG_NFO(log.logger(), "Done!");

	// Create a new TCP chat client
	auto client = std::make_shared< client::tcp::TelematicsClient >(service, address, port);

	// Connect the client
	LG_NFO(log.logger(), "Client connecting...");
	if(!client->ConnectAsync()) {
		LG_ERR(log.logger(), "Connect async");
		return 1;
	}
	LG_NFO(log.logger(), "Done!");

	while (true) {
		if (client->IsConnected() == false) {
			client->ConnectAsync();
		}
		if (client->SendAsync(buf.data(), buf.size())) {
			break;
		}
		CppCommon::Thread::Sleep(2000);
	}

	// Disconnect the client
	LG_NFO(log.logger(), "Client disconnecting...");
	client->disconnectAndStop();
	LG_NFO(log.logger(), "Done!");

	// Stop the Asio service
	LG_NFO(log.logger(), "Asio service stopping...");
	service->Stop();
	LG_NFO(log.logger(), "Done!");

	return 0;
}