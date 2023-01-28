#ifndef D6CE9ED9_44F8_4952_9A8A_BD481D8676B8
#define D6CE9ED9_44F8_4952_9A8A_BD481D8676B8

#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <iostream>
#include <memory>

namespace tc::db::mongo {

class Instance {
public:
	~Instance () {};

	static Instance* getInstance() {
		static Instance objInstance;
		return &objInstance;
	}

	/* Create a pool object only once from MongoDB URI */
	void createPool(std::string uri) {
		if (!iClientPool){
			iClientPool = (std::unique_ptr<mongocxx::pool>)
			new mongocxx::pool { mongocxx::uri {uri} };
		}
	}

	/* Acquire a client from the pool */
	mongocxx::pool::entry getClientFromPool() {
		return iClientPool->acquire();
	}

private:
  mongocxx::instance iDbInstance;
  std::unique_ptr<mongocxx::pool> iClientPool;
  Instance () { }
};

} // namespace tc::db::mongo

#endif /* D6CE9ED9_44F8_4952_9A8A_BD481D8676B8 */
