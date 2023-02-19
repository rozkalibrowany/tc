#include <tc/server/tcp/Clean.h>

namespace tc::server::tcp {

void Clean::execute(std::shared_ptr<Client> client, uint64_t interval, uint32_t days_lifetime)
{
	while (true) {
		std::chrono::milliseconds _interval(interval);
		std::this_thread::sleep_for(_interval);

		SysTime now(true);
		if (int64_t timestamp; (timestamp = now.timestamp() - client->collection().iTime.timestamp()) > days_lifetime * 1000 * 60 * 60 * 24)
		{ // period x day
			Client::Collection old(client->collection().iName, Client::ePackets, timestamp);
			client->drop(old);
		}
	}
}

} // namespace tc::server::http