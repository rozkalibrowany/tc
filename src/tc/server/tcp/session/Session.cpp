#include <tc/server/tcp/session/Session.h>
#include <tc/server/tcp/server/Server.h>
#include <tc/common/Convert.h>

namespace tc::server::tcp {

Session::Session(const std::shared_ptr<TCPServer> &server)
 : TCPSession(server)
 , common::LogI("console")
 , iServer(std::dynamic_pointer_cast<Server>(server))
 , iPacket(std::make_unique< parser::Packet >(true))
// , iLog(log)
{
	// nothing to do
}

parser::PacketUPtr Session::data()
{
	return std::move(iPacket);
}

bool Session::emptyData() const
{
	if (iPacket == nullptr || iPacket->size() == 0) {
		return true;
	}
	return false;
}

void Session::onConnected()
{
	SPDLOG_LOGGER_INFO(this->logger(), "TCP session with Id {} connected!", ids());
}

void Session::onDisconnected()
{
	SPDLOG_LOGGER_INFO(this->logger(), "TCP session with Id {} disconnected!", ids());
}

void Session::onReceived(const void* buffer, size_t size)
{
	SPDLOG_LOGGER_INFO(this->logger(), "Session got buffer with size[ {} ]", (int)size);

	result_t res = RES_OK;
	auto action = getAction(size);
	switch(action) {
		case A_PARSE_DATA:
		case A_PARSE_IMEI:
			res = iPacket->parse((unsigned char *)buffer, size);
			break;
		case A_NO_ACTION:
			res = RES_NOENT;
			break;
		default:
			break;
	}

	if (res != RES_OK) {
		SPDLOG_LOGGER_ERROR(this->logger(), "Handle received data error. Session[{}], data size[{}]", ids(), (int) size);
		return;
	}

	sendResponse(action);
}

void Session::onError(int error, const std::string& category, const std::string& message)
{
	SPDLOG_LOGGER_ERROR(this->logger(), "TCP session with Id {} ERROR!", ids());
}

Session::Action Session::getAction(size_t size)
{
	if (size >= Packet::PACKET_DATA_MIN_SIZE) {
		return Action::A_PARSE_DATA;
	}

	if (size >= Packet::PACKET_IMEI_MIN_SIZE) {
		return Action::A_PARSE_IMEI;
	}

	if (size > (size_t) 0) {
		return Action::A_DEFAULT;
	}

	return Action::A_NO_ACTION;
}

std::string Session::ids() const
{
	auto id_str = id().string();
	auto id_str_out = std::string("...-");
	id_str_out.append(id_str.substr(id_str.find_first_of("-"), 0));
	return id_str_out;
}

void Session::sendResponse(Action action, bool async)
{
	if (action == A_NO_ACTION || iPacket == nullptr) {
		return;
	}

	int response = (int) R_REFUSE;
	switch(action) {
		case A_PARSE_DATA:
			response = (int) iPacket->size();
			break;
		default:
			response = (int)R_ACCEPT;
			break;
	}

	if (!async) {
		Send(static_cast<void *>(&response), sizeof(response));
		return;
	}

	SendAsync(static_cast<void *>(&response), sizeof(response));
}

} // namespace tc::server::tcp