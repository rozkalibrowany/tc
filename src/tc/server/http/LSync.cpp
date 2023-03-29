#include <tc/server/http/LSync.h>

namespace tc::server::http {

void LSync::execute(std::shared_ptr<CacheHandler> cache, std::shared_ptr<Client> client, int64_t interval)
{
	while (true) {
		std::chrono::milliseconds _interval(interval);
		std::this_thread::sleep_for(_interval);

		auto &vehicles = cache->vehicles();
		for (auto &veh : vehicles) {
			if (!client->hasImei(veh.first)) {
				client->insert(veh.second->toJson().toStyledString());
				continue;
			}
			std::string json;
			if (client->get(veh.first, json) != RES_OK)
				continue;

			if (client->replace(json, veh.second->toJson().toStyledString()) != RES_OK) {
				LG_WRN(this->logger(), "Unable to replace document[{}]", veh.first);
				continue;
			}
		}
	}
}

} // namespace tc::server::http