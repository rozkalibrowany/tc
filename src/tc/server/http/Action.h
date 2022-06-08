#ifndef D8DB185A_D164_40D8_BFBF_9E353D4EBBFB
#define D8DB185A_D164_40D8_BFBF_9E353D4EBBFB

#include <server/http/http_request.h>
#include <tc/parser/packet/PacketRequest.h>

namespace tc::server::http {

using namespace parser;

class Action {
public:

	using Method = PacketRequest::Method;
	using Type = PacketRequest::Type;

	result_t parse(const CppServer::HTTP::HTTPRequest& request);
	result_t parse(const Type type, const Method method);

	std::string iReq;
	Method iMethod {PacketRequest::NONE};

private:
};

} // namespace tc::server::http


#endif /* D8DB185A_D164_40D8_BFBF_9E353D4EBBFB */
