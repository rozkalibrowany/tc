#include <tc/db/Client.h>
#include <tc/db/Instance.h>
#include <tc/db/Thread.h>

namespace tc::db::mongo {

Client::Client(std::string &uri)
{
	Instance::getInstance()->createPool(uri);
}

result_t Client::load(INIStructure &ini)
{
	auto ok = ini["db"].has("name") && ini["db"].has("collection");
	if (!ok)
		return RES_NOENT;

	auto name = ini["db"]["name"];
	auto enabled = ini["db"]["enabled"] == "true";
	auto collection = ini["db"]["collection"];

	if (!ini["db"].has("enabled"))
		enabled = false;

	iName = name;
	iEnabled = enabled;
	iCollection = collection;

	return RES_OK;
}

result_t Client::get(const std::string &id, bsoncxx::document::view &doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	bool ok = false;

	auto thr = std::make_shared<db::mongo::Thread>(*entry, iName, iCollection, std::string{});
	std::thread thread((*thr.get()), id, std::ref(doc), std::ref(ok));
	thread.join();

	return (ok == true ? RES_OK : RES_ERROR);
}

result_t Client::insert(const std::string &json_doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();

	std::thread thread(db::mongo::Thread{*entry, iName, iCollection, json_doc});
	thread.join();

	return RES_OK;
}

result_t Client::create(const std::string &coll_name)
{
	return create(coll_name, iName);
}

result_t Client::create(const std::string &coll_name, const std::string &db_name)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	auto &client = *entry;

	client[db_name].create_collection(coll_name);

	return RES_OK;
}

bool Client::has(const std::string &coll_name)
{
	return has(coll_name, iName);
}

bool Client::has(const std::string &coll_name, const std::string &db_name)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	auto &client = *entry;
	auto collections = client[db_name].list_collection_names();

	return std::find(collections.begin(), collections.end(), coll_name) != collections.end();
}

const std::string Client::collection() const
{
	return iCollection;
}

const std::string Client::name() const
{
	return iName;
}

const bool Client::enabled() const
{
	return iEnabled;
}

} // namespace tc::db::::mongo