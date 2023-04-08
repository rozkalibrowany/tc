#ifndef D8AB2E39_84B7_48C6_9D5F_3015EF5742CF
#define D8AB2E39_84B7_48C6_9D5F_3015EF5742CF

#include <tc/client/tcp/Client.h>
#include <tc/asio/AsioService.h>
#include <tc/server/http/CacheHandler.h>
#include <tc/server/http/CacheSession.h>
#include <tc/server/http/CacheServer.h>
#include <tc/server/http/Request.h>
#include <tc/server/http/LSync.h>
#include <tc/server/http/Updater.h>
#include <tc/db/Client.h>
#include <mini/ini.h>
#include <filesystem>
#include <chrono>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace mINI;
using namespace tc;

namespace defaults {
	constexpr int c_default_threads 								= 2;
	constexpr int c_default_db_sync_interval 				= 30000;
	constexpr int c_default_pool_interval 					= 2000;
	constexpr int c_default_devices_update_interval	= 6000;
	constexpr int c_default_packets_update_interval	= 5000;
};

void sleep_for(uint64_t time) {
	std::this_thread::sleep_for(chrono::milliseconds(time));
}

result_t readServerConfig(INIStructure &ini, LogI &log, int &port, int &threads, int &pool_interval)
{
	if (!ini["server"].has("port")) {
		LG_ERR(log.logger(), "Missing HTTP port number.");
		return RES_NOENT;
	}

	port = std::stoi(ini["server"]["port"]);
	if (!vIsPortNumber(port)) {
		LG_ERR(log.logger(), "Invalid HTTP port number[{}].", port);
		return RES_NOENT;
	}

	threads = ini["server"].has("threads") ? std::stoi(ini["server"]["threads"]) : defaults::c_default_threads;
	pool_interval = ini["server"].has("pool_interval") ? std::stoi(ini["server"]["pool_interval"]) : defaults::c_default_pool_interval;

	return RES_OK;
}

result_t readTelematicsConfig(INIStructure &ini, LogI &log, std::string &addr, int &port, int &devices_update_interval, int &packets_update_interval)
{
	if (!ini["telematics"].has("port")) {
		LG_ERR(log.logger(), "Missing HTTP port number.");
		return RES_NOENT;
	}

	port = std::stoi(ini["telematics"]["port"]);
	if (!vIsPortNumber(port)) {
		LG_ERR(log.logger(), "Invalid TCP port number[{}].", port);
		return RES_NOENT;
	}

	if (!ini["telematics"].has("address")) {
		LG_ERR(log.logger(), "Missing TCP address.");
		return RES_NOENT;
	}

	addr = ini["telematics"]["address"];
	if (!vIsAddress(addr)) {
		LG_ERR(log.logger(), "Invalid TCP address[{}].", addr);
		return RES_NOENT;
	}

	devices_update_interval = ini["telematics"].has("devices_update_interval") ? std::stoi(ini["telematics"]["devices_update_interval"]) : defaults::c_default_devices_update_interval;
	packets_update_interval = ini["telematics"].has("packets_update_interval") ? std::stoi(ini["telematics"]["packets_update_interval"]) : defaults::c_default_packets_update_interval;

	return RES_OK;
}

result_t readDatabaseConfig(INIStructure &ini, LogI &log, std::string &uri, int &sync_interval)
{
	if (!ini["db"].has("uri")) {
		LG_WRN(log.logger(), "Missing DB URI address.");
	} else {
		uri = ini["db"]["uri"];
	}

	sync_interval = ini["db"].has("sync_interval") ? std::stoi(ini["db"]["sync_interval"]) : defaults::c_default_db_sync_interval;
	return RES_OK;
}

int main(int argc, char** argv)
{
	using namespace tc::server::http;

	auto logger = spdlog::stdout_color_mt("console");

	LogI log(logger);
	spdlog::set_default_logger(log.logger());

	if(!std::filesystem::exists(argv[1])) {
		LG_ERR(log.logger(), "Config file not exists. Exit...");
		return 1;
	}

	INIFile file(argv[1]);
	INIStructure ini;

	if(!file.read(ini)) {
		LG_ERR(log.logger(), "Unable to read INI config file.");
		return 1;
	}

	int http_port, threads, pool_interval;
	if (readServerConfig(ini, log, http_port, threads, pool_interval) != RES_OK) {
		return 1;
	}

	std::string addr;
	int tcp_port, devices_update_interval, packets_update_interval;
	if (readTelematicsConfig(ini, log, addr, tcp_port, devices_update_interval, packets_update_interval) != RES_OK) {
		return 1;
	}

	std::string uri;
	int sync_interval;
	if (readDatabaseConfig(ini, log, uri, sync_interval) != RES_OK) {
		return 1;
	}

	// Create DB client
	auto db_client = std::make_shared<db::mongo::Client>(db::mongo::Client::eDevices);
	if (db_client->load(ini) != RES_OK) {
		LG_ERR(log.logger(), "Unable to parse db client config. Exiting...");
		return 1;
	}

	if (db_client->enabled()) {
		db_client->init(uri);
		if(!db_client->has(db_client->collection())) db_client->create(db_client->collection());
		LG_NFO(log.logger(), "DB connected. Name: {}, collection: {}, uri: {}", db_client->name(), (std::string) db_client->collection(), uri);
	}

	// Prepare signals
	Signal<Imei, std::string> signal_cmd;
	Signal<Imei> signal_modified;

	// Create Cache for data
	auto cache_handler = std::make_shared<CacheHandler>(signal_cmd, signal_modified);

	// Create a new Asio service
	auto service = std::make_shared<tc::asio::AsioService>(threads);
	if (!service->Start())
		return 1;

	// Connect client signal for syncing device info
	Signal<const void *, size_t> signal;
	signal.connect([&](const void * buf, size_t size) {
		cache_handler->onReceived(buf, size);
	});

	// Create a new TCP client
	auto client = std::make_shared< client::tcp::Client >(signal, service, addr, tcp_port);

	while (true) {
		if (client->IsConnected()) {
			break;
		}
		LG_NFO(log.logger(), "Retrying connection to telematics server...");
		client->ConnectAsync();
		sleep_for(pool_interval);
	}

	// connect cache signal
	signal_cmd.connect([&](Imei imei, std::string cmd) {
    client->send(imei, cmd);
  });

	// Create a new HTTPS server
	auto server = std::make_shared<HTTPCacheServer>(service, db_client, cache_handler, http_port);
	if (!server->Start()) {
		LG_ERR(log.logger(), "Unable to start HTTP server.");
		return 1;
	}
	LG_NFO(log.logger(), "HTTP Server started!");

	signal_modified.connect([&](Imei imei) {
		server->onModified(imei);
	});

	// sync devices from DB
	server->syncDevices();

	// Sync devices into DB
	server::http::LSync sync;
	std::thread lsync_thread(&LSync::execute, &sync, cache_handler, db_client, sync_interval);
	lsync_thread.detach();

	// Sync packets from telematics server
	server::http::Updater packets_updater(Request::eGet, Request::ePackets);
	std::thread packets_updater_thread(&Updater::execute, &packets_updater, client, packets_update_interval);
	packets_updater_thread.detach();

	// Sync devices from telematics server
	server::http::Updater device_updater(Request::eGet, Request::eDevices);
	std::thread devices_updater_thread(&Updater::execute, &device_updater, client, devices_update_interval);
	devices_updater_thread.detach();

	while (true) {
		LG_NFO(log.logger(), "HTTP server, sessions[{}] bytes sent[{}] received[{}]", server->connected_sessions(), server->bytes_sent(), server->bytes_received());

		sleep_for(pool_interval);
	}

	// Join thread
	lsync_thread.join();
	devices_updater_thread.join();
	packets_updater_thread.join();

	// Stop the server
	LG_NFO(log.logger(), "Server stopping...");
	server->Stop();

	// Stop the Asio service
	LG_NFO(log.logger(), "Asio service stopping...");
	service->Stop();

	return 0;
}

#endif /* D8AB2E39_84B7_48C6_9D5F_3015EF5742CF */
