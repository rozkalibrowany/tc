#ifndef C56B686D_6161_4ADA_84B8_E655D445D95C
#define C56B686D_6161_4ADA_84B8_E655D445D95C

#include <server/http/https_server.h>
#include <tc/server/iot/Devices.h>
#include <tc/server/iot/Vehicle.h>
#include <tc/parser/Command.h>
#include <tc/server/http/Action.h>
#include <tc/client/tcp/Client.h>
#include <queue>

namespace tc::server::http {

using namespace parser;

class CacheHandler : public CppCommon::Singleton<CacheHandler>, public tc::LogI
{
	friend CppCommon::Singleton<CacheHandler>;
public:
	CacheHandler(std::shared_ptr<client::tcp::Client> client);

	bool hasImei(const Imei imei) const;

	result_t getDevice(const Request &request, CppServer::HTTP::HTTPResponse &response);
	result_t getDevices(CppServer::HTTP::HTTPResponse &response, bool active_only = true);
	result_t getDevices(std::string &devices, bool active_only = true);
	result_t getPackets(CppServer::HTTP::HTTPResponse &response);
	result_t getPacket(const std::shared_ptr< iot::Vehicle > vehicle, Json::Value &rhs);

	result_t handleAction(Request &request, CppServer::HTTP::HTTPResponse &response);
	void onReceived(const void *buffer, size_t size);

	iot::Devices<iot::Vehicle> &vehicles();
	Signal<Imei> &signal();

private:
	result_t findImei(Request &request);
	result_t addCommand(const Request &request, CppServer::HTTP::HTTPResponse &response);
	result_t set(Request &request, CppServer::HTTP::HTTPResponse &response);
	result_t decodeJson(const std::string &data);

	Json::Value lastLocation;

	iot::Devices<iot::Vehicle> iVehicles;
	std::shared_ptr<client::tcp::Client> iClient;
	Signal<Imei> iSignal;
	std::mutex iMutex;
};

} // namespace tc::server::http

#endif /* C56B686D_6161_4ADA_84B8_E655D445D95C */
