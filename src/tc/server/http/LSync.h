#ifndef E8D63998_FFC8_43E7_8694_6D9FC7A3CB19
#define E8D63998_FFC8_43E7_8694_6D9FC7A3CB19

#include <tc/server/http/CacheHandler.h>
#include <tc/db/Client.h>
#include <thread>
#include <functional>

namespace tc::server::http {

using namespace db::mongo;

class LSync : public tc::LogI
{
public:
	void execute(std::shared_ptr<CacheHandler> cache, std::shared_ptr<Client> client, int64_t interval);
};

} // namespace tc::server::http

#endif /* E8D63998_FFC8_43E7_8694_6D9FC7A3CB19 */
