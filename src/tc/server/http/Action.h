#ifndef D8DB185A_D164_40D8_BFBF_9E353D4EBBFB
#define D8DB185A_D164_40D8_BFBF_9E353D4EBBFB

#include <tc/server/http/Request.h>
#include <tc/common/Common.h>
namespace tc::server::http {

class Action : public Types, public tc::LogI {
public:

	Action() = default;

	result_t parse(const CppServer::HTTP::HTTPRequest& request);

	std::shared_ptr< Request > get() const;

private:
	bool hasQuery(const Request &request);

	result_t parseDevice(const Request &request);
	result_t parseDevices(const Request &request);
	result_t parseQuery(const Request &request);
	result_t parseDeviceId(const Request &request);
	result_t parseCommand(const Request &request);

	result_t handleGet(const Request &request);
	result_t handlePost(const Request &request);

	Request iRequest;
};

} // namespace tc::server::http


#endif /* D8DB185A_D164_40D8_BFBF_9E353D4EBBFB */
