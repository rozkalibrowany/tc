#include <tc/db/Client.h>
#include <tc/db/Instance.h>
#include <tc/db/Thread.h>
#include <optional>

namespace tc::db::mongo {

Client::Client(CollType type)
	: iCollection(type)
{
	// nothing to do
}

void Client::init(const std::string &uri)
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
	setCollection(collection);

	return RES_OK;
}

void Client::synchronizeTime(int64_t timestamp)
{
	iCollection.iTime.timestamp(timestamp);
}

/** Note: not thread safe!
 * It gets a client from the pool, creates an access object, and then calls the find_one function.
 * @param imei The IMEI of the device to be retrieved.
 * @param json_doc The JSON document to be inserted.
 * @return The result of the find_one operation.
 */
result_t Client::get(const std::string &imei, std::string &json_doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return RES_NOENT;

	Access access(**entry, iName, iCollection, Access::Read);

	return access.find_one(imei, json_doc);
}

/** Note: not thread safe!
 * It gets a client from the pool, creates an access object, and returns the cursor
 * @return A cursor object.
 */
std::optional< mongocxx::cursor > Client::getCursor()
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return {};

	Access access(**entry, iName, iCollection, Access::Read);

	return access.cursor();
}

/**
 * The function inserts a JSON document into the database
 * @param json_doc The JSON document to be inserted.
 * @return RES_OK
 */
result_t Client::insert(const std::string &json_doc)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return RES_NOENT;

	std::thread thread(db::mongo::Thread{**entry, iName, iCollection, json_doc});
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
	if (!entry)
		return RES_NOENT;
	
	auto &client = **entry;

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
	if (!entry)
		return RES_NOENT;
	
	std::thread thread(db::mongo::Thread{**entry, iName, iCollection, std::string{}}, key, old, val);
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
	if (!entry)
		return RES_NOENT;
	
	std::thread thread(db::mongo::Thread{**entry, iName, iCollection, std::string{}}, key, old, val);
	thread.join();

	return RES_OK;
}

result_t Client::replace(const std::string &json_old, const std::string &json_new)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return RES_NOENT;
	
	Access access(**entry, iName, iCollection, Access::Read);

	return access.replace(json_old, json_new);
}

result_t Client::drop(const std::string &collection)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return RES_NOENT;

	Access access(**entry, iName, collection, Access::Write);
	access.drop(collection);
	return RES_OK;
}

void Client::setCollection(const std::string &coll)
{
	iCollection.iName = coll;
}

bool Client::hasImei(const Imei &imei)
{
	auto entry = Instance::getInstance()->getClientFromPool();
	if (!entry)
		return false;
	
	Access access(**entry, iName, iCollection, Access::Read);

	return access.has(imei);
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
	auto &client = **entry;
	auto collections = client[db_name].list_collection_names();

	return std::find(collections.begin(), collections.end(), coll_name) != collections.end();
}

Client::Collection Client::collection()
{
	return iCollection;
}

std::string Client::collection_str()
{
	return (std::string) iCollection;
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