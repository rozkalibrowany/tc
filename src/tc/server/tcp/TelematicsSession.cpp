#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/parser/packet/PacketCommand.h>
#include <tc/common/CRC16.h>

namespace tc::server::tcp {

std::shared_ptr<TelematicsServer> TelematicsSession::tcServer()
{
	std::shared_ptr<TelematicsServer> server = std::dynamic_pointer_cast<TelematicsServer>(this->server());
	return server;
}

void TelematicsSession::onReceived(const void *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session: received buffer[{}]: {}", size, tc::uchar2string((const uchar *)buffer, size));

	return handleDataBuffer(buffer, size, Action::get((const uchar*) buffer, size));
}

void TelematicsSession::handleDataBuffer(const void* buffer, size_t size, Action::Type type)
{
	result_t res = RES_OK;
	bool crc_ok = true;

	if (type == Action::payload) {
		res = handlePayload((const uchar*) buffer, size, crc_ok);
	}

	if (type == Action::imei) {
		res = handleImei((const uchar *)buffer, size);
	}

	if (type == Action::command) {
		res = handleCommand((const uchar*) buffer, size);
	}

		if (type == Action::standby) {
		res = handleStandby((const uchar*) buffer, size);
	}

	auto chunked_data = (crc_ok == false && iBufferIncomplete != nullptr && iBufferIncomplete->size() != size);
	if (chunked_data || type == Action::incomplete_payload) {
		res = handleIncomplete((const uchar*) buffer, size, crc_ok);
	}

	if (res != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle data buffer");
	}
}

result_t TelematicsSession::handleImei(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session: handle payload imei [{}]: {}", size, tc::uchar2string(buffer, size));

	result_t res = RES_OK;
	Imei imei;
	int response = 0;

	res |= Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		send(response);
		return res;
	}

	LG_NFO(this->logger(), "Handle IMEI OK, IMEI: {}", imei);
	auto has_uuid = tcServer()->has(this->id());

	if (has_uuid == false)
		tcServer()->add(this->id(), imei);

	res |= send((int) has_uuid, true);

	return res;
}

result_t TelematicsSession::handlePayload(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Session[{}] handle payload [{}]", id().string(), size);

	result_t res = RES_OK;
	int response = 0;
	Imei imei;

	if (tcServer()->has(this->id()) == false) {
		LG_ERR(this->logger(), "Unknown imei.");
		send(response);
		return res;
	}

	if ((res = tcServer()->get(this->id(), imei)) != RES_OK) {
		LG_ERR(this->logger(), "Error getting imei.");
		send(response);
		return res;
	}

	auto buf = std::make_shared< parser::Buf >(buffer, size);

	res = checkCrc(buf, size, crc_ok);
	if (res != RES_OK) {
		send(response);
		return res;
	}

	if (crc_ok == false) {
		LG_WRN(this->logger(), "Incorrect CRC checksum.");
		iBufferIncomplete = buf;
		return RES_NOENT;
	}
	auto packet = std::make_shared<parser::PacketPayload>();
		if ((res = packet->parse((uchar*) buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Parse payload packet");
		return res;
	}

	response = (int) packet->size();
	iBufferIncomplete.reset();
	iBufferIncomplete = nullptr;

	LG_NFO(this->logger(), "Handle payload succesfull. Imei: {} total records: {} ", imei, packet->size());

	tcServer()->add(imei, std::move(packet));

	send(response);

	return res;
}

result_t TelematicsSession::handleCommand(const uchar *buffer, size_t size)
{
		auto val = (((buffer[0]) << 8) | ((buffer[1]) << 0));

		LG_NFO(this->logger(), "Session: static_cast: {}: {} received buffer[{}]: {}", static_cast<int>(buffer[0]), val, size, tc::uchar2string((const uchar *)buffer, size));

	LG_NFO(this->logger(), "Session: received command[{}]: [{}]", size, uchar2string((unsigned char *)buffer, size));
	
	int response = 0;
	Imei imei;

	result_t res = parser::Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		send(response);
		return res;
	}

	auto packetCommand = std::make_shared< parser::PacketCommand >();
	res = packetCommand->parse((uchar*) buffer, size, imei.length());
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Handle command.");
		return res;
	}

	if (tcServer()->has(imei) == false) {
		LG_ERR(this->logger(), "Imei not present.");
		return res;
	}

	return tcServer()->sendCommand(imei, packetCommand);
}

result_t TelematicsSession::handleIncomplete(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Session: handle incomplete [{}]", size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || iBufferIncomplete->size() == 0 || size == iBufferIncomplete->size()) {
		int response = 0;
		send(response);
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
	LG_NFO(this->logger(), "Session: handle standby [{}]", size);

	return send(1);
}

result_t TelematicsSession::checkCrc(std::shared_ptr< parser::Buf > buf, size_t size, bool &crc_ok)
{
	if (buf == nullptr) {
		return RES_NOENT;
	}

	common::CRC16 crc;
	auto reader = std::make_shared< parser::Reader >(buf, size);
	auto reader_crc = reader->read(4, size - 4);

	parser::Buf subBuf(parser::Buf::ByteArray{buf->begin() + 8, buf->begin() + size - 4});
	auto calc = crc.calc((char*) subBuf.iBuf.data(), subBuf.size());

	crc_ok = calc == (int)reader_crc ? true : false;

	if (crc_ok) {
		LG_NFO(this->logger(), "CRC ok.");
	} else {
		LG_WRN(this->logger(), "CRC incorrect.");
	}

	return RES_OK;
}

result_t TelematicsSession::send(const uchar* buffer, size_t size, const bool async) {

	LG_NFO(this->logger(), "Sending uchar[{}]: {}", size, tc::uchar2string(buffer, size));

	auto buf_str = tc::uchar2string(buffer, size);
	auto out = new char[size];
	tc::hex2bin(buf_str.data(), out);

	for (int i = 0; i < size; i++) {
		LG_NFO(this->logger(), "out[{}] = {}", i, out[i]);
	}

		return send((const void *)out, size, async);
}

result_t TelematicsSession::send(int buffer, const bool async)
{
	LG_NFO(this->logger(), "Sent to client: {}", buffer);

	return send(static_cast< void *>(&buffer), sizeof(buffer), async);
}

result_t TelematicsSession::send(const void *buffer, size_t size, const bool async)
{
	auto res = RES_NOENT;

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

} // namespace tc::server::tcp