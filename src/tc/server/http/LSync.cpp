#include <tc/server/http/LSync.h>

namespace tc::server::http {

void LSync::execute(std::shared_ptr<CacheHandler> cache, std::shared_ptr<Client> client, int64_t interval)
{
	while (true) {
		std::chrono::milliseconds _interval(interval);
		std::this_thread::sleep_for(_interval);

		auto &devices = cache->devices();
		for (auto &dev : devices) {
			if (!client->hasImei(dev.first)) {
				client->insert(dev.second->toJson().toStyledString());
				continue;
			}
			std::string json;
			if (client->get(dev.first, json) != RES_OK)
				continue;

			if (client->replace(json, dev.second->toJson().toStyledString()) != RES_OK) {
				LG_WRN(this->logger(), "Unable to replace document[{}]", dev.first);
				continue;
			}
		}
	}
}

} // namespace tc::server::http