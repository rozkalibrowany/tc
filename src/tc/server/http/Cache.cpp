#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <sstream>
namespace tc::server::http {

std::string Cache::GetAllCache()
{
	std::scoped_lock locker(_cache_lock);
	std::string result;
	result += "[\n";
	for (const auto& item : _cache)
	{
			result += "  {\n";
			result += "    \"key\": \"" + item.first + "\",\n";
			result += "    \"value\": \"" + item.second + "\",\n";
			result += "  },\n";
	}
	result += "]\n";
	return result;
}

void Cache::onReceived(const void *buffer, size_t size)
{
	if (size == 0) {
		LG_ERR(this->logger(), "Buffer is empty");
		return;
	}

	auto str = std::string((const char*) buffer, size);
	auto fromHex = tc::hex2string(str);

	LG_NFO(this->logger(), "fromHex: {}", fromHex);

	if (decodeString(fromHex) != RES_OK) {
		LG_ERR(this->logger(), "Unable to decode string");
		return;
	}

}

bool Cache::GetCacheValue(std::string_view key, std::string& value)
{
std::scoped_lock locker(_cache_lock);
auto it = _cache.find(key);
if (it != _cache.end())
{
		value = it->second;
		return true;
}
else
		return false;
}

void Cache::PutCacheValue(std::string_view key, std::string_view value)
{
	std::scoped_lock locker(_cache_lock);
	auto it = _cache.emplace(key, value);
	if (!it.second)
			it.first->second = value;
}

bool Cache::DeleteCacheValue(std::string_view key, std::string& value)
{
	std::scoped_lock locker(_cache_lock);
	auto it = _cache.find(key);
	if (it != _cache.end())
	{
			value = it->second;
			_cache.erase(it);
			return true;
	}
	else
			return false;
}

result_t Cache::decodeString(const std::string &data)
{
	Json::Value root;
	Json::Reader reader;
	bool parsing_ok = reader.parse(data, root);
	if (!parsing_ok) {
		LG_ERR(this->logger(), "Unable to parse string");
		return RES_INVARG;
	}

	LG_ERR(this->logger(), "Unable to parse string1");
	const Json::Value devices = root["devices"];
	LG_ERR(this->logger(), "Unable to parse string2");

	return iDevices.fromJson(devices, false);
}

} // namespace tc::server::http