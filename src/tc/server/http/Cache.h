#ifndef C2C73546_BB60_4433_A0EF_D1D4FF692471
#define C2C73546_BB60_4433_A0EF_D1D4FF692471

#include <server/http/https_server.h>

namespace tc::server::http {

class Cache : public CppCommon::Singleton<Cache>
{
   friend CppCommon::Singleton<Cache>;

public:
	std::string GetAllCache();

	bool GetCacheValue(std::string_view key, std::string &value);
	void PutCacheValue(std::string_view key, std::string_view value);
	bool DeleteCacheValue(std::string_view key, std::string &value);

private:
    std::mutex _cache_lock;
    std::map<std::string, std::string, std::less<>> _cache;
};

} // namespace tc::server::http

#endif /* C2C73546_BB60_4433_A0EF_D1D4FF692471 */
