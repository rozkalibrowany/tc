#include <tc/server/http/Cache.h>

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

} // namespace tc::server::http