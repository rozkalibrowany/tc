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

result_t Client::insert(const std::string &json_doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();

	std::thread thread{db::mongo::Thread(*entry, iName, iCollection, json_doc)};
	thread.join();

	return RES_OK;
}

result_t Client::create(const std::string &coll_name)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	auto &client = *entry;

	client["db"].create_collection(coll_name);
	
	return RES_OK;
}

bool Client::has(const std::string &coll_name)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	auto &client = *entry;
	auto collections = client["db"].list_collection_names();

	return std::find(collections.begin(), collections.end(), coll_name) != collections.end();
}

const std::string Client::collection() const
{
	return iCollection;
}

const bool Client::enabled() const
{
	return iEnabled;
}

} // namespace tc::db::::mongo