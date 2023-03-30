#include <tc/server/http/Updater.h>

namespace tc::server::http {

Updater::Updater(Request::Method method, Request::Type type)
 : tc::LogI("console")
 , iRequest(method, type)
{
	// nothing to do
}

void Updater::execute(std::shared_ptr< tc::client::tcp::Client > client, int64_t interval)
{
	std::chrono::milliseconds _interval(interval);

	while (!client->IsConnected()) {
		std::this_thread::sleep_for(_interval);
		continue;
	}
	
	parser::Buf buf;
	if (iRequest.toInternal(buf) != RES_OK) {
		LG_ERR(this->logger(), "Unable to convert to internal request");
	}

	while (true) {
		std::this_thread::sleep_for(_interval);

		if (client->send(buf) != RES_OK)	{
			LG_ERR(this->logger(), "Unable to send buffer");
		}		
	}
}

} // namespace tc::server::http