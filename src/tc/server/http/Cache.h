#ifndef C56B686D_6161_4ADA_84B8_E655D445D95C
#define C56B686D_6161_4ADA_84B8_E655D445D95C

#include <server/http/https_server.h>
#include <tc/server/iot/Devices.h>
#include <tc/parser/Command.h>
#include <tc/common/Common.h>
#include <queue>

namespace tc::server::http {

using namespace parser;

class Cache : public CppCommon::Singleton<Cache>, public tc::LogI
{
   friend CppCommon::Singleton<Cache>;
public:
	bool hasCommands() const;
	bool hasImei(const Imei imei) const;

	std::shared_ptr< Command > getCommand();

	Json::Value getDevices();

	result_t addCommand(const Imei imei, const string cmd);
	result_t set(const Imei imei, pair< const string, const string > val);

	void onReceived(const void *buffer, size_t size);

private:
	result_t decodeJson(const std::string &data);

	iot::Devices iDevices;
	std::mutex _cache_lock;

	std::queue< std::shared_ptr< Command > > iCommands;
};

} // namespace tc::server::http

#endif /* C56B686D_6161_4ADA_84B8_E655D445D95C */
