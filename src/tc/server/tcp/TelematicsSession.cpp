#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/parser/packet/PacketRequest.h>
#include <tc/db/Thread.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
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
	LG_NFO(this->logger(), "[{}] Session: received buffer[{}]", iImei, size);

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
		LG_DBG(this->logger(), "[{}] Unable to handle data buffer[{}]", iImei, size);
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

	LG_NFO(this->logger(), "Parse imei OK: {}", imei);

	iDevice = std::make_unique<iot::Device>(telematicsServer()->cacheSize(), imei);
	res |= send(eOK);

	iImei = std::move(imei);
	return res;
}

result_t TelematicsSession::handlePayload(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle payload [{}]", iImei, size);

	result_t res = RES_OK;

	auto packet = std::make_shared< parser::PacketPayload >();
	if ((res = packet->parse(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Parse payload packet", iImei);
		return res;
	}

	res = iDevice->add(packet);
	if (res != RES_OK && res != RES_INVCRC) {
		LG_ERR(this->logger(), "[{}] Unable to add packet", iImei);
		send(eInvalid);
		return res;
	}

	std::scoped_lock lock(iMutex);
	if (telematicsServer()->dbClient()->enabled())
		res |= savePacket(packet);

	if (res == RES_INVCRC) {
		iBufferIncomplete = std::make_shared< parser::Buf >(buffer, size);
		return res;
	}

	auto records = (int) iDevice->lastRecords();
	LG_DBG(this->logger(), "[{}] Handle payload succesfull. Records[{}] ", iImei, records);

	res |= send(records);
	return res;
}

result_t TelematicsSession::handleIncomplete(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle incomplete[{}]", iImei, size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || size >= iBufferIncomplete->size()) {
		send(eInvalid);
		return res;
	}

	iBufferIncomplete->iBuf.insert(iBufferIncomplete->iBuf.end(), buffer, buffer + size);
	if((res = handlePayload((const uchar *)iBufferIncomplete->iBuf.data(), iBufferIncomplete->size())) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Unable to handle payload", iImei);
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

result_t TelematicsSession::savePacket(std::shared_ptr<parser::PacketPayload> &packet)
{
	Json::Value val;

	auto timestamp = packet->timestamp().timestamp.timestamp();
	auto systime = SysTime(timestamp);

	val["imei"] = iDevice->iImei;
	val["timestamp"] = timestamp;
	val["datetime"] = systime.getDateTime();

	if (result_t res; (res = packet->toJson(val, true)) != RES_OK) {
			LG_ERR(this->logger(), "[{}] Packet to json.", iImei);
			return res;
		}

	auto dbClient = telematicsServer()->dbClient();

	if(result_t res; (res = dbClient->insert(val.toStyledString())) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Error inserting data.", iImei);
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
	LG_DBG(this->logger(), "[{}] Sending: {} size: {}", iImei, tc::uchar2string((const uchar*) buffer, size), size);


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