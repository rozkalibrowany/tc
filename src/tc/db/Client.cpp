#include <tc/db/Client.h>
#include <tc/db/Instance.h>
#include <tc/db/Thread.h>

namespace tc::db::mongo {

Client::Client(std::string &uri)
{
	Instance::getInstance()->createPool(uri);
}

/**
 * It loads the database configuration from the INI file
 * @param ini The INIStructure object that contains the parsed INI file.
 * @return RES_OK
 */
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

/* Note: not thread safe! */
result_t Client::get(const std::string &imei, std::string &json_doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	Access access(*entry, iName, iCollection, Access::Read);

	return access.find_one(imei, json_doc);
}

/**
 * The function inserts a JSON document into the database
 * @param json_doc The JSON document to be inserted.
 * @return RES_OK
 */
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

/**
 * It gets a client from the pool, creates a collection, and returns.
 * @param coll_name The name of the collection to be created.
 * @param db_name The name of the database to create the collection in.
 * @return RES_OK
 */
result_t Client::create(const std::string &coll_name, const std::string &db_name)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	auto &client = *entry;

	client[db_name].create_collection(coll_name);

	return RES_OK;
}

/**
 * It updates the value of a key in the database.
 * @param key The key to update
 * @param old The old value of the key.
 * @param val The value to be updated
 * @return RES_OK
 */
result_t Client::update(const std::string &key, const std::string &old, const std::string &val)
{
	auto entry = Instance::getInstance()->getClientFromPool();

	std::thread thread(db::mongo::Thread{*entry, iName, iCollection, std::string{}}, key, old, val);
	thread.join();

	return RES_OK;
}

/**
 * It updates the value of a key in the database.
 * @param key The key to update
 * @param old The old value of the key.
 * @param val The value to be updated
 * @return RES_OK
 */
result_t Client::update(const std::string &key, const int64_t old, const int64_t val)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	std::thread thread(db::mongo::Thread{*entry, iName, iCollection, std::string{}}, key, old, val);
	thread.join();

	return RES_OK;
}

bool Client::has(const std::string &coll_name)
{
	return has(coll_name, iName);
}

/**
 * It checks if a collection exists in a database
 * @param coll_name The name of the collection to check for.
 * @param db_name The name of the database to use.
 * @return A boolean value.
 */
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