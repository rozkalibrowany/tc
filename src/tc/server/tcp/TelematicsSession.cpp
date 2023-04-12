#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/server/tcp/handler/Teltonika.h>
#include <tc/server/tcp/handler/Omni.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <fmt/format.h>
#include <iomanip>


namespace tc::server::tcp {

const Imei TelematicsSession::imei() const
{
	return iHandler == nullptr ? "unknown" : iHandler->imei();
}

std::shared_ptr<TelematicsServer> TelematicsSession::server()
{
	return std::dynamic_pointer_cast<TelematicsServer>(this->server());
}

void TelematicsSession::onReceived(const void *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Received buffer[{}]", imei(), size);

	if (iProtocol.type() == Protocol::eUnknown && iProtocol.parse((const uchar*) buffer, size) != RES_OK) {
		LG_WRN(this->logger(), "[{}] Unknown protocol", imei(), size);
		return;
	}

	LG_NFO(this->logger(), "[{}] Protocol: {}", imei(), (int) iProtocol.type());

	if (iHandler == nullptr && createHandler(iProtocol) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable creating handler", imei());
		return;
	}

	return handlePayload(buffer, size);
}

void TelematicsSession::handlePayload(const void *buffer, size_t size)
{
	if (iHandler == nullptr)
		return;

	iHandler->handle((const uchar*) buffer, size);
}

result_t TelematicsSession::createHandler(Protocol protocol)
{
	switch (protocol.type()) {
		case Protocol::eTeltonika:
			iHandler = std::make_unique<TeltonikaHandler>(std::dynamic_pointer_cast<TelematicsSession>(this->shared_from_this()));
			break;
		case Protocol::eOmni:
			iHandler = std::make_unique<OmniHandler>(std::dynamic_pointer_cast<TelematicsSession>(this->shared_from_this()));
			break;
		case Protocol::eUnknown:
			return RES_NOENT;
		}
		return RES_OK;
}

result_t TelematicsSession::savePacket(std::shared_ptr<parser::teltonika::Payload> &packet)
{
	if (iDevice == nullptr || iDevice->imei().empty())
		return RES_NOENT;

	Json::Value val;

	auto timestamp = packet->timestamp().timestamp.timestamp();
	auto systime = SysTime(timestamp);

	val["imei"] = iDevice->imei();
	val["timestamp"] = timestamp;
	val["datetime"] = systime.getDateTime();

	if (result_t res; (res = packet->toJson(val, true)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Packet to json.", imei());
		return res;
	}

	auto dbClient = server()->dbClient();

	if(result_t res; (res = dbClient->insert(val.toStyledString())) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Error inserting data.", imei());
		return res;
	}

	return RES_OK;
}

result_t TelematicsSession::send(const uchar* buffer, size_t size, const bool async)
{
	auto buf_str = tc::uchar2string(buffer, size);
	auto out = new char[size];
	tc::hex2bin(buf_str.data(), out);

	auto res =  send((const void *)out, size, async);
	delete out;
	return res;
}

result_t TelematicsSession::send(int buffer, const bool async)
{
	return send(static_cast<void *>(&buffer), sizeof(int), async);
}

result_t TelematicsSession::send(const void *buffer, size_t size, const bool async)
{
	auto res = RES_NOENT;
	LG_DBG(this->logger(), "[{}] Sending: {} size: {}", imei(), tc::uchar2string((const uchar*) buffer, size), size);

	if (async == false) {
		auto sent = Send(buffer, size);
		res = sent == size ? RES_OK :
			RES_CONNERROR;
	} else {
		auto sent = SendAsync(buffer, size);
		res = sent == true ? RES_OK : RES_CONNERROR;
	}

	return res;
}

result_t TelematicsSession::lastPacketJson(Json::Value &rhs) {
	if (iDevice == nullptr || iDevice->packets().empty()) {
		return RES_NOENT;
	}
	if (iDevice->packets().back()->toJson(rhs) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to serialize last packet into json", imei());
		return RES_ERROR;
	}
 	rhs["Imei"] = imei();

	return RES_OK;
}

result_t TelematicsSession::toJsonImpl(Json::Value &rhs, bool root) const
{
	if (iDevice == nullptr) {
		return RES_NOENT;
	}

	rhs["Session"] = id().string();
	return iDevice->toJson(rhs, root);
}

} // namespace tc::tcServer::tcp