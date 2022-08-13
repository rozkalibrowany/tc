#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>
#include <sstream>

namespace tc::server::http {

bool  Cache::hasImei(const Imei imei) const
{
	return iDevices.has(imei);
}

std::shared_ptr< Command > Cache::getCommand()
{
	if (iCommands.empty())
		return nullptr;

	auto command = iCommands.front();
	iCommands.pop();
	return command;
}

Json::Value Cache::getDevices()
{
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

	auto command = std::make_shared<Command>(imei);
	if ((res = command->create(cmd)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to create command");
		return res;
	}

	iCommands.push(std::move(command));
	return res;
}

result_t Cache::set(const Imei imei, pair< const string, const string > val)
{
	auto &devices = iDevices.devices();
	auto it = devices.find(imei);
	if (it != devices.end()) {
		LG_ERR(this->logger(), "val.first: {}, val.second: {} device: {}", val.first, val.second, fmt::ptr((*it).second));
		if (!val.first.compare("ID") || !val.first.compare("id")) {
			(*it).second->iID = val.second;
		} else if (!val.first.compare("Imei")) {
			(*it).second->iImei = val.second;
		}
	}

	return RES_OK;

}

} // namespace tc::server::http