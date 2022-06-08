#ifndef ACC1C2F6_6ABB_4C3C_A014_1FC0AE8B93CD
#define ACC1C2F6_6ABB_4C3C_A014_1FC0AE8B93CD

#include <tc/server/http/Action.h>
#include <tc/parser/packet/PacketRequest.h>
#include <tc/common/Common.h>

namespace tc::server::http {

class RequestFactory : public tc::LogI
{
public:

	virtual ~RequestFactory() = default;

	result_t create(const Action &action, parser::Buf &buf, bool cr = false);
};

} // namespace tc::server::http


#endif /* ACC1C2F6_6ABB_4C3C_A014_1FC0AE8B93CD */
