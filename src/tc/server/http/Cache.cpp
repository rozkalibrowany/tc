#include <tc/server/http/Cache.h>
#include <json/json.h>
#include <tc/parser/Util.h>

namespace tc::server::http {

bool  Cache::hasImei(const Imei imei) const
{
	return iDevices.has(imei);
}

result_t Cache::handleAction(const Action &action, CppServer::HTTP::HTTPResponse &response)
{
	switch(action.get()->type()) {
		case Request::eDevices:
			return getDevices(response);

		case Request::eDevice:
			if (action.get()->method() == Request::eGet) {
				return RES_NOIMPL;
			}
			if (action.get()->command().compare("set")) {
				return set(action.get()->id(), action.get()->iQueryParam);
			} else {
				return addCommand(action.get()->id(), action.get()->command());
			}

		case Request::ePackets:
		default:
			return RES_NOIMPL;
		}

		return RES_NOENT;
}

std::shared_ptr< Command > Cache::getCommand()
{
	if (iCommands.empty())
		return nullptr;

	auto command = iCommands.front();
	iCommands.pop();
	return command;
}

result_t Cache::getDevices(CppServer::HTTP::HTTPResponse &response)
{
	response.SetBody(iDevices.toJson().toStyledString());
	return RES_OK;
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
		if (!val.first.compare("ID") || !val.first.compare("id")) {
			(*it).second->iID = val.second;
		} else {
			return RES_INVARG;
		}
	}

	return RES_OK;

}

} // namespace tc::server::http