#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/parser/packet/PacketRequest.h>

namespace tc::server::tcp {

std::shared_ptr<TelematicsServer> TelematicsSession::tcServer()
{
	std::shared_ptr<TelematicsServer> server = std::dynamic_pointer_cast<TelematicsServer>(this->server());
	return server;
}

void TelematicsSession::onReceived(const void *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session: received buffer[{}]", size);

	return handleDataBuffer((const uchar*) buffer, size, Action::get((const uchar*) buffer, size));
}

void TelematicsSession::handleDataBuffer(const uchar* buffer, size_t size, Action::Type type)
{
	result_t res = RES_OK;
	bool crc_ok = true;

	if (type == Action::payload) {
		res = handlePayload(buffer, size, crc_ok);
	}

	if (type == Action::imei) {
		res = handleImei(buffer, size);
	}

	if (type == Action::command) {
		res = handleCommand(buffer, size);
	}

	if (type == Action::request) {
		res = handleRequest(buffer, size);
	}

	if (type == Action::standby) {
		res = handleStandby(buffer, size);
	}

	auto chunked_data = (crc_ok == false && iBufferIncomplete != nullptr && iBufferIncomplete->size() != size);
	if (chunked_data || type == Action::incomplete_payload) {
		res = handleIncomplete(buffer, size, crc_ok);
	}

	if (res != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle data buffer");
	}
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

	LG_NFO(this->logger(), "Handle imei OK: {}", imei);

	iDevice = std::make_unique<iot::Device>(imei);

	/* if IMEI does not exists - add it */
	/*if (tcServer()->has(imei) == false)
		tcServer()->add(this->id(), imei); */

	res |= send(eOK, true);

	return res;
}

result_t TelematicsSession::handlePayload(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Handle payload [{}]", size);

	result_t res = RES_OK;

	if ((res = iDevice->add(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to add packet, imei[{}]", iDevice->imei());
		return res;
	}

	auto records = (int) iDevice->lastRecords();
	iBufferIncomplete.reset();
	//iBufferIncomplete = nullptr;

	LG_NFO(this->logger(), "Handle payload succesfull. Device imei: {} records: {} ", iDevice->imei(), records);

	send(records);
	return res;
}

result_t TelematicsSession::handleCommand(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle command[{}]", size);

	Imei imei;

	result_t res = parser::Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		return res;
	}

	auto packetCommand = std::make_shared< parser::PacketCommand >();
	res = packetCommand->parse((uchar*) buffer, size, imei.length());
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse command.");
		return res;
	}

	if (tcServer()->has(imei) == false) {
		LG_ERR(this->logger(), "Imei not recognized in server.");
		return res;
	}

	return tcServer()->sendCommand(imei, packetCommand);
}

result_t TelematicsSession::handleIncomplete(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Handle incomplete[{}]", size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || iBufferIncomplete->size() == 0 || size == iBufferIncomplete->size()) {
		send(eInvalid);
		return res;
	}

	iBufferIncomplete->iBuf.insert(iBufferIncomplete->iBuf.end(), buffer, buffer + size);
	if((res = handlePayload((const uchar *)iBufferIncomplete->iBuf.data(), iBufferIncomplete->size(), crc_ok)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle payload");
	}

	return res;
}

result_t TelematicsSession::handleStandby(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle standby[{}]", size);

	return send(eOK);
}

result_t TelematicsSession::handleRequest(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle request[{}]", size);

	auto request = std::make_shared< parser::PacketRequest >();
	result_t res = request->parse((uchar*) buffer, size);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse request.");
		send(eInvalid);
		return res;
	}

	return dispatchRequest(request);
}

result_t TelematicsSession::dispatchRequest(std::shared_ptr< parser::PacketRequest > &request)
{
	using namespace parser;

	result_t res = RES_OK;
	auto type = request->iType;
	auto method = request->iMethod;

	if (type == PacketRequest::Devices && method == PacketRequest::GET) {
		auto json = tcServer()->iDevices.toJson();
		auto hexJson = tc::tohex(json.toStyledString());

		if ((res = send((const uchar*) hexJson.data(), hexJson.size(), true)) != RES_OK) {
			LG_ERR(this->logger(), "Send hex json");
		}
	}

	return res;
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
	return send(static_cast< void *>(&buffer), sizeof(buffer), async);
}

result_t TelematicsSession::send(const void *buffer, size_t size, const bool async)
{
	auto res = RES_NOENT;
	LG_NFO(this->logger(), "Sending: {}", tc::uchar2string((const uchar*) buffer, size));


	if (async == false) {
		auto sent = Send(buffer, size);
		res = sent == size ? RES_OK : RES_CONNERROR;
	} else {
		auto sent = SendAsync(buffer, size);
		res = sent == true ? RES_OK : RES_CONNERROR;
	}

	if (res != RES_OK) {
		LG_ERR(this->logger(), "Incomplete bytes sent");
		return res;
	}

	return res;
}

/* const uchar* data = "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE";
	uchar *out = new uchar[32];
	tc::hex2bin((const char*) data, (char*) out);
	for (int i = 0; i < 32; i++) {
		LG_ERR(this->logger(), "out[{}] = {}", i, out[i]);
	}
	SendAsync((void*) out, 32);
	WORKS TO SEND COMMAND */

} // namespace tc::tcServer::tcp