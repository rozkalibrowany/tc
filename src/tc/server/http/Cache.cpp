#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>
#include <sstream>

namespace tc::server::http {

Json::Value Cache::getDevices()
{
	//std::scoped_lock locker(_cache_lock);
	LG_NFO(this->logger(), "iDevices size: {}", iDevices.size());
	return iDevices.toJson();
}

void Cache::onReceived(const void *buffer, size_t size)
{
	if (size == 0) {
		LG_ERR(this->logger(), "Buffer is empty");
		return;
	}

	auto str = std::string((const char*) buffer, size);
	auto fromHex = tc::hex2string(str);

	if (decodeJson(fromHex) != RES_OK) {
		LG_ERR(this->logger(), "Unable to decode string");
		return;
	}
}

result_t Cache::decodeJson(const std::string &data)
{	
	Json::Value root;
	Json::Reader reader;
	bool parsing_ok = reader.parse(data, root, false);
	if (!parsing_ok) {
		LG_ERR(this->logger(), "Unable to parse json");
		return RES_INVARG;
	}

	return iDevices.fromJson(root);
}

} // namespace tc::server::http