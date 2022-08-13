#ifndef D8DB185A_D164_40D8_BFBF_9E353D4EBBFB
#define D8DB185A_D164_40D8_BFBF_9E353D4EBBFB

#include <server/http/http_request.h>
#include <tc/parser/packet/PacketRequest.h>
#include <tc/common/SysMutex.h>

namespace tc::server::http {

using namespace std;
using namespace parser;
using namespace CppServer::HTTP;
class Action : public ReqType, public tc::LogI {
public:

	Action() = default;

	result_t parse(const CppServer::HTTP::HTTPRequest& request);
	result_t parse(const Type type, const Method method);

	string iID;
	string iReq;
	string iAction;
	Method iMethod {NONE};
	Type iType {Unknown};
	pair< string, string > iQueryParam;
	SysMutex iMutex;

private:
	result_t parse_internal_command(const string &action);

};

} // namespace tc::server::http


#endif /* D8DB185A_D164_40D8_BFBF_9E353D4EBBFB */
