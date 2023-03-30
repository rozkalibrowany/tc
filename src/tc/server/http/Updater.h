#ifndef F07AC2B9_04FB_491A_9EB7_24092B2989EF
#define F07AC2B9_04FB_491A_9EB7_24092B2989EF

#include <tc/server/http/Request.h>
#include <tc/client/tcp/Client.h>
#include <thread>
#include <functional>
#include <mutex>

namespace tc::server::http {

class Updater : public tc::LogI
{
public:
	Updater(Request::Method method, Request::Type type);
	void execute(std::shared_ptr<client::tcp::Client> client, int64_t interval);

private:
	Request iRequest;
	std::mutex iMutex;
};

} // namespace tc::server::http

#endif /* F07AC2B9_04FB_491A_9EB7_24092B2989EF */
