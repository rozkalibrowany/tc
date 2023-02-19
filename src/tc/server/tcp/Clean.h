#ifndef DAD6A397_B86A_4AF9_B2A6_DEC27EFC00C8
#define DAD6A397_B86A_4AF9_B2A6_DEC27EFC00C8

#include <tc/db/Client.h>
#include <thread>
#include <functional>

namespace tc::server::tcp {

using namespace db::mongo;

class Clean : public tc::LogI
{
public:
	void execute(std::shared_ptr<Client> client, uint64_t interval, uint32_t days_lifetime);
};

} // namespace tc::server::http

#endif /* DAD6A397_B86A_4AF9_B2A6_DEC27EFC00C8 */
