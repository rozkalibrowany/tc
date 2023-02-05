#ifndef C56B686D_6161_4ADA_84B8_E655D445D95C
#define C56B686D_6161_4ADA_84B8_E655D445D95C

#include <server/http/https_server.h>
#include <tc/server/iot/Devices.h>
#include <tc/server/iot/Vehicle.h>
#include <tc/parser/Command.h>
#include <tc/server/http/Action.h>
#include <tc/common/Signal.h>
#include <queue>

namespace tc::server::http {

using namespace parser;

class Cache : public CppCommon::Singleton<Cache>, public tc::LogI
{
   friend CppCommon::Singleton<Cache>;
public:
	Cache(Signal<Imei, std::string> &signal);

	bool hasImei(const Imei imei) const;

	result_t getDevice(const Imei &imei, CppServer::HTTP::HTTPResponse &response);
	result_t getDevices(CppServer::HTTP::HTTPResponse &response);
	std::string getDevices();

	result_t handleAction(const Action &action, CppServer::HTTP::HTTPResponse &response);
	void onReceived(const void *buffer, size_t size);

private:
	result_t addCommand(const Imei imei, const string cmd, CppServer::HTTP::HTTPResponse &response);
	result_t set(std::shared_ptr< Request > request, CppServer::HTTP::HTTPResponse &response);
	result_t decodeJson(const std::string &data);

	Signal<Imei, std::string> &iSignal;

	iot::Devices<iot::Vehicle> iDevices;
	std::mutex _cache_lock;
};

} // namespace tc::server::http

#endif /* C56B686D_6161_4ADA_84B8_E655D445D95C */
