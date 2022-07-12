#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>
#include <sstream>

namespace tc::server::http {

/*std::shared_ptr< parser::Buf > Cache::getCommand()
{
	return std::make_shared<parser::Buf>(iCommands.back());
}*/

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

bool Cache::hasCommands() const
{
	return !iCommands.empty();
}

result_t Cache::addCommand(const Imei imei, const std::string cmd)
{
	result_t res = RES_OK;
	auto str_command = tc::parser::Command::toString(cmd);
	auto command = std::make_shared<parser::Command>(imei);

	if ((res = command->create(str_command)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to create command");
		return res;
	}

	iCommands.push(std::move(command));
	return res;
}

} // namespace tc::server::http