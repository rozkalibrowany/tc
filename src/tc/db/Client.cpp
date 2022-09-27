#include <tc/db/Client.h>

namespace tc::server::db {

Client::Client(std::string &uri)
 : iClient(mongocxx::uri{uri})
{
	// nothing to do
}

bool Client::load(INIStructure &ini)
{
	if (!ini["db"].has("name"))
		return false;
	iName = ini["db"]["name"];

	if (ini["db"].has("collection_packets"))
		iCollection.insert(std::make_pair("collection_packets", ini["db"]["collection_packets"]));

	if (ini["db"].has("collection_devices"))
		iCollection.insert(std::make_pair("collection_devices", ini["db"]["collection_devices"]));

	if (iCollection.empty())
		return false;

	if (!ini["db"].has("enabled"))
		return false;
	iEnabled = ini["db"]["enabled"] == "true";

	return true;
}

mongocxx::client &Client::client()
{
	return iClient;
}

const std::string Client::name() const
{
	return iName;
}

std::string Client::collection(const std::string key) const
{
	return iCollection.at(key);
}

const bool Client::enabled() const
{
	return iEnabled;
}

} // namespace tc::server::db