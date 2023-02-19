#include <tc/db/Thread.h>

namespace tc::db::mongo {

#include <tc/db/Thread.h>

Thread::Thread(mongocxx::client& client, std::string db_name, std::string coll, const std::string &data)
	: iAccess(client, db_name, coll)
	, iData(data)
{
	// nothing to do
}
void Thread::operator()()
{
	if (result_t res; (res = iAccess.insert(iData)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to write data.");
	}
}

void Thread::operator()(const std::string &key, const int64_t old, const int64_t val)
{
	if (result_t res; (res = iAccess.update(key, old, val)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to update data.");
	}
}

void Thread::operator()(const std::string &key, const std::string &old, const std::string &val)
{
	if (result_t res; (res = iAccess.update(key, old, val)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to update data.");
	}
}

void Thread::operator()(const std::string &id, bsoncxx::document::view &val, bool &ok)
{
	std::string json_doc;
	if (result_t res; (res = iAccess.find_one(id, json_doc)) != RES_OK) {
		LG_WRN(this->logger(), "Unable to find: [{}]", id);
		ok = false;
		return;
	}
	ok = true;
}

} // namespace tc::db::mongo
