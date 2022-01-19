#include <tc/server/tcp/session/Session.h>
#include <tc/server/tcp/server/Server.h>
#include <tc/common/Convert.h>

namespace tc::server::tcp {

Session::Session(const std::shared_ptr<TCPServer> &server)
 : TCPSession(server)
 , common::LogI("console")
 , iTimestamp(SysTime(true))
 , iServer(std::move(std::dynamic_pointer_cast<Server>(server)))
{
	// nothing to do
}

void Session::onReceived(const void* buffer, size_t size)
{
	SPDLOG_LOGGER_INFO(this->logger(), "Session[{}] got buffer, size[{}]", ids(), (int) size);

	result_t res = RES_OK;
	auto action = getAction(size);
	switch(action) {
		case A_PARSE_DATA:
		case A_PARSE_IMEI:
			res = handleDataBuffer(buffer, size);
			break;
		case A_NO_ACTION:
			res = RES_NOENT;
			break;
		default:
			break;
	}

	if (res != RES_OK) {
		SPDLOG_LOGGER_ERROR(this->logger(), "Handle received data error. Session[{}], data size[{}]", ids(), (int) size);
		iServer->sessionPackets().erase(id());
		this->Disconnect();
		return;
	}

	handleResponse(action, true);
}

result_t Session::handleDataBuffer(const void *buffer, size_t size)
{
	result_t res = RES_OK;

	auto it = iServer->sessionPackets().find(id());
	if (it == iServer->sessionPackets().end()) {
		SPDLOG_LOGGER_ERROR(this->logger(), "Error getting packet. Session[{}], data size[{}]", ids(), (int) size);
		return res;
	}
	auto &packet = iServer->sessionPackets().at(id());

	if ((res = packet.parse((unsigned char *)buffer, size)) != RES_OK) {
		SPDLOG_LOGGER_ERROR(this->logger(), "Error parsing packet. Session[{}], data size[{}]", ids(), (int) size);
		return res;
	}

	if (packet.imei().empty() == false && packet.size() > 0) {
		iServer->add(packet.imei(), packet);
		iServer->sessionPackets().erase(id());
	}

	return res;
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
	id_str_out.append(id_str.substr(0, id_str.find("-")));
	return id_str_out;
}

void Session::handleResponse(Action action, bool async)
{
	if (action == A_NO_ACTION) {
		return;
	}

	int response = (int) R_REFUSE;

	if (action == A_PARSE_DATA) {
		auto it = iServer->sessionPackets().find(id());
		if (it != iServer->sessionPackets().end()) {
			//auto &packet = iServer->sessionPackets().at(id());
			response = (int) it->second.size();
		}
	} else {
		response = (int) R_ACCEPT;
	}

	if (!async) {
		Send(static_cast<void *>(&response), sizeof(response));
		return;
	}

	SendAsync(static_cast<void *>(&response), sizeof(response));
}

} // namespace tc::server::tcp