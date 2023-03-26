#ifndef D6CE9ED9_44F8_4952_9A8A_BD481D8676B8
#define D6CE9ED9_44F8_4952_9A8A_BD481D8676B8

#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <iostream>
#include <memory>

namespace tc::db::mongo {

using namespace mongocxx;

class Instance {
public:
	~Instance(){};

	static Instance *getInstance()
	{
		static Instance objInstance;
		return &objInstance;
	}

	/* Create a pool object only once from MongoDB URI */
	void createPool(std::string uri)
	{
		if (!iClientPool)
		{
			iClientPool = (std::unique_ptr<mongocxx::pool>)new mongocxx::pool{mongocxx::uri{uri}};
		}
	}

	/* Acquire a client from the pool */
	stdx::optional<pool::entry> getClientFromPool()
	{
		return iClientPool->try_acquire();
	}

private:
	mongocxx::instance iDbInstance;
	std::unique_ptr<pool> iClientPool;
	Instance() {}
};

} // namespace tc::db::mongo

#endif /* D6CE9ED9_44F8_4952_9A8A_BD481D8676B8 */
