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
	if (!ini["db"].has("name"))
		return RES_NOENT;

	auto name = ini["db"]["name"];
	auto enabled = ini["db"]["enabled"] == "true";

	if (ini["db"].has("collection_packets"))
		iCollection.insert(std::make_pair("collection_packets", ini["db"]["collection_packets"]));

	if (ini["db"].has("collection_devices"))
		iCollection.insert(std::make_pair("collection_devices", ini["db"]["collection_devices"]));

	if (iCollection.empty())
		return RES_NOENT;

	if (!ini["db"].has("enabled"))
		return RES_NOENT;

	iName = name;
	iEnabled = enabled;

	return RES_OK;
}

result_t Client::insert(const std::string &json_doc)
{
	return insert(json_doc, iCollection.at("collection_packets"));
}

result_t Client::insert(const std::string &json_doc, const std::string &coll)
{
	auto entry = Instance::getInstance()->getClientFromPool();

	std::thread thread{db::mongo::Thread(*entry, iName, coll, json_doc)};
	thread.join();

	return RES_OK;
}

std::string Client::collection(const std::string key) const
{
	return iCollection.at(key);
}

const bool Client::enabled() const
{
	return iEnabled;
}

} // namespace tc::db::::mongo