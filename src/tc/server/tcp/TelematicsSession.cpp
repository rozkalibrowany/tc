#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/parser/packet/PacketRequest.h>
#include <iomanip>

namespace tc::server::tcp {

std::shared_ptr<TelematicsServer> TelematicsSession::telematicsServer()
{
	std::shared_ptr<TelematicsServer> server = std::dynamic_pointer_cast<TelematicsServer>(this->server());
	return server;
}

bool TelematicsSession::hasImei(const Imei imei) const
{
	if (iDevice == nullptr) {
		return false;
	}

	return imei.compare(iDevice->iImei) == 0 ? true : false;
}

Action::Type TelematicsSession::type() const
{
	return iType;
}

void TelematicsSession::onReceived(const void *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session: received buffer[{}]", size);

	return handleDataBuffer((const uchar*) buffer, size, Action::get((const uchar*) buffer, size));
}

void TelematicsSession::handleDataBuffer(const uchar* buffer, size_t size, Action::Type type)
{
	result_t res = RES_OK;

	if (type == Action::payload) {
		res = handlePayload(buffer, size);
	}

	if (type == Action::imei) {
		res = handleImei(buffer, size);
	}

	if (type == Action::command) {
		res = telematicsServer()->handleCommand(buffer, size);
	}

	if (type == Action::request) {
		res = telematicsServer()->handleRequest(buffer, size, id());
	}

	if (type == Action::standby) {
		res = handleStandby(buffer, size);
	}

	if (type == Action::unknown && iBufferIncomplete != nullptr) {
		res = handleIncomplete(buffer, size);
	}

	if (res != RES_OK) {
		LG_DBG(this->logger(), "Unable to handle data buffer[{}]", size);
	}

	iType = type;
}

result_t TelematicsSession::handleImei(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle imei[{}]", size);

	result_t res = RES_OK;
	Imei imei;

	res |= Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		send(eInvalid);
		return res;
	}

	LG_DBG(this->logger(), "Parse imei OK: {}", imei);

	iDevice = std::make_unique<iot::Device>(imei);
	res |= send(eOK);

	return res;
}

result_t TelematicsSession::handlePayload(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle payload [{}]", size);

	result_t res = RES_OK;

	res = iDevice->add(buffer, size);
	if (res != RES_OK && res != RES_INVCRC) {
		LG_ERR(this->logger(), "Unable to add packet, imei[{}]", iDevice->iImei);
		send(eInvalid);
		return res;
	}

	if (res == RES_INVCRC) {
		iBufferIncomplete = std::make_shared< parser::Buf >(buffer, size);
		return res;
	}

	auto records = (int) iDevice->lastRecords();
	LG_DBG(this->logger(), "Handle payload succesfull. Imei[{}] records[{}] ", iDevice->iImei, records);

	res |= send(records);
	return res;
}

result_t TelematicsSession::handleIncomplete(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle incomplete[{}]", size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || size >= iBufferIncomplete->size()) {
		send(eInvalid);
		return res;
	}

	iBufferIncomplete->iBuf.insert(iBufferIncomplete->iBuf.end(), buffer, buffer + size);
	if((res = handlePayload((const uchar *)iBufferIncomplete->iBuf.data(), iBufferIncomplete->size())) != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle payload");
	}

	iBufferIncomplete.reset();
	iBufferIncomplete = nullptr;
	return res;
}

result_t TelematicsSession::handleStandby(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle standby[{}]", size);

	return send(eOK);
}

result_t TelematicsSession::send(const uchar* buffer, size_t size, const bool async)
{
	auto buf_str = tc::uchar2string(buffer, size);
	auto out = new char[size];
	tc::hex2bin(buf_str.data(), out);

	return send((const void *)out, size, async);
}

result_t TelematicsSession::send(int buffer, const bool async)
{
	return send(static_cast<void *>(&buffer), sizeof(int), async);
}

result_t TelematicsSession::send(const void *buffer, size_t size, const bool async)
{
	auto res = RES_NOENT;
	LG_DBG(this->logger(), "Sending: {} size: {}", tc::uchar2string((const uchar*) buffer, size), size);


	if (async == false) {
		auto sent = Send(buffer, size);
		res = sent == size ? RES_OK : RES_CONNERROR;
	} else {
		auto sent = SendAsync(buffer, size);
		res = sent == true ? RES_OK : RES_CONNERROR;
	}

	return res;
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