#include <tc/db/Thread.h>

namespace tc::db::mongo {

Thread::Thread(mongocxx::client& client, std::string db_name, std::string coll, std::string json)
	: iAccess(client, db_name, coll)
	, iJson(json)
{
	// nothing to do
}

void Thread::operator()() {
	iAccess.insert(iJson);
}

} // namespace tc::db::mongo
