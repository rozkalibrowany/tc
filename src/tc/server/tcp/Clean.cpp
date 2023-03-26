#include <tc/server/tcp/Clean.h>
#include <tc/db/Instance.h>

namespace tc::server::tcp {

void Clean::execute(std::shared_ptr<Client> client, uint64_t interval, uint32_t days_lifetime)
{
	while (true) {
		std::chrono::milliseconds _interval(interval);
		std::this_thread::sleep_for(_interval);

		auto entry = Instance::getInstance()->getClientFromPool();
		if (!entry)
			continue;

		auto db = (*entry)->database(client->name());
		for (auto& coll : db.list_collection_names()) {
			int year = 0, month = 0, day = 0;
			if (sscanf(coll.c_str(),"Packets_%d_%d_%d", &day, &month, &year) != 3)
				continue;

			SysTime collection_datetime, collection_oldest;
			collection_datetime.set(year, month, day);

			if ((SysTime(true).timestamp() - collection_datetime.timestamp()) > (days_lifetime * 1000 * 60 * 60 * 24))
				client->drop(coll);
		}
	}
}

} // namespace tc::server::http