#ifndef C2C73546_BB60_4433_A0EF_D1D4FF692471
#define C2C73546_BB60_4433_A0EF_D1D4FF692471

#include <server/http/https_server.h>
#include <tc/server/iot/Devices.h>
#include <tc/common/Common.h>
#include <queue>

namespace tc::server::http {

class Cache : public CppCommon::Singleton<Cache>, public tc::LogI
{
   friend CppCommon::Singleton<Cache>;
public:
	/*bool hasCommands() const;
	std::shared_ptr< parser::Buf > getCommand()*/

	Json::Value getDevices();
	result_t addCommand(const Imei imei, const std::string cmd);

	void onReceived(const void *buffer, size_t size);


private:
	result_t decodeJson(const std::string &data);

	iot::Devices iDevices;
	std::mutex _cache_lock;

//std::vector< parser::Buf > iCommands;
};

} // namespace tc::server::http

#endif /* C2C73546_BB60_4433_A0EF_D1D4FF692471 */
