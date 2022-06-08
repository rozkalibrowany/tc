#ifndef C2C73546_BB60_4433_A0EF_D1D4FF692471
#define C2C73546_BB60_4433_A0EF_D1D4FF692471

#include <server/http/https_server.h>
#include <tc/server/iot/Devices.h>
#include <tc/common/Common.h>

namespace tc::server::http {

class Cache : public CppCommon::Singleton<Cache>, public tc::LogI
{
   friend CppCommon::Singleton<Cache>;

public:

	void onReceived(const void *buffer, size_t size);

	std::string GetAllCache();

	bool GetCacheValue(std::string_view key, std::string &value);
	void PutCacheValue(std::string_view key, std::string_view value);
	bool DeleteCacheValue(std::string_view key, std::string &value);

private:
	result_t decodeString(const std::string &data);

	iot::Devices iDevices;
	std::mutex _cache_lock;
	std::map<std::string, std::string, std::less<>> _cache;
};

} // namespace tc::server::http

#endif /* C2C73546_BB60_4433_A0EF_D1D4FF692471 */
