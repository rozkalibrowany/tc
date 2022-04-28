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

	LG_NFO(this->logger(), "Session[{}] received buffer[{}] [{}]", id().string(), size, unsigned_char_to_string((unsigned char*) buffer, size));

	Action action;
	if (action.parse(buffer, size) != RES_OK)
	{
		LG_ERR(this->logger(), "Unable to parse action");
		return;
	}
	/*
	auto buf_ori = (const uchar*) buffer;
	auto len = ((buf_ori[0]) << 6) | ((buf_ori[1]) << 0);

	LG_NFO(this->logger(), "ORIGINAL BUF, len {}", len);
	for (uint i = 0; i < size; i++) {
		LG_NFO(this->logger(), "i[{}]: {:x}", i, std::byte(buf_ori[i]));
	}
	LG_NFO(this->logger(), "MODIFIED BUF");
	auto buf = (const uchar*) buffer + 4;
	for (uint i = 0; i < size; i++) {
		LG_NFO(this->logger(), "i[{}]: {:x}", i, std::byte(buf[i]));
	}
	LG_NFO(this->logger(), "_______________"); */
	return handleDataBuffer(buffer, size, action.type());
}

void TelematicsSession::handleDataBuffer(const void* buffer, size_t size, Action::Type type)
{
	result_t res = RES_OK;
	bool crc_ok = true;

	if (type == Action::payload) {
		res = handlePayload((const uchar*) buffer, size, crc_ok);
	}

	if (type == Action::payload_imei && crc_ok == true) {
			res = handlePayloadImei((const uchar *)buffer, size);
	}

	if (type == Action::command) {
		res = handleCommand((const uchar*) buffer, size);
	}

	auto chunked_data = (crc_ok == false && iBufferIncomplete != nullptr && iBufferIncomplete->size() != size);
	if (chunked_data || type == Action::incomplete_payload) {
		res = handleIncomplete((const uchar*) buffer, size, crc_ok);
	}

	if (res != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle data buffer");
	}
}

result_t TelematicsSession::handlePayloadImei(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session[{}] handle payload imei [{}]", id().string(), size);

	result_t res = RES_OK;
	int response = 0;

	auto packet = std::make_shared< parser::PacketPayload >();
	res |= packet->parseImei(buffer, size);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		send(response);
		return res;
	}

	LG_NFO(this->logger(), "Handle IMEI OK, IMEI: {}. packet addr: {} session: {}", packet->imei(), fmt::ptr(packet.get()), id().string());
	iImei = packet->imei();
	response = 1;

	tcServer()->add(iImei, std::move(packet));

	res |= send(response);
	return res;
}

result_t TelematicsSession::handlePayload(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Session[{}] handle payload [{}]", id().string(), size);

	result_t res = RES_OK;
	int response = 0;

	if (tcServer()->hasQueuedCommands(iImei)) {
		std::shared_ptr<parser::PacketCommand> packet;
		if ((res = tcServer()->get(iImei, packet)) != RES_OK) {
			LG_ERR(this->logger(), "Get command.");
			return res;
		}
		if (send((void*) packet->command(), packet->size()) != RES_OK) {
			LG_ERR(this->logger(), "send command.");
			return res;
		}
		tcServer()->packetsCommand().clear();
		LG_NFO(this->logger(), "Sent command: {}", tc::unsigned_char_to_string(packet->command(), packet->size()));
	}

	if (iImei.empty()) { //  && iServer->has(iImei) == true
		send(response, true);
		return res;
	}

	auto buf = std::make_shared< parser::Buf >(buffer, size);

	res = checkCrc(buf, size, crc_ok);
	if (res != RES_OK) {
		send(response, true);
		return res;
	}

	if (crc_ok == false) {
		LG_WRN(this->logger(), "Incorrect CRC checksum.");
		iBufferIncomplete = buf;
		CppCommon::Thread::Sleep(100);
		return RES_NOENT;
	}

	auto packet = std::make_shared< parser::PacketPayload >();
	packet->setImei(iImei);
	if ((res = packet->parse(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Parse payload packet");
		return res;
	}

	response = (int) packet->size();
	iBufferIncomplete.reset();
	iBufferIncomplete = nullptr;

	LG_NFO(this->logger(), "Handle payload succesfull. Imei: {} total records: {} ", packet->imei(), packet->size());

	tcServer()->add(iImei, std::move(packet));

	send(response);
	return res;
}

result_t TelematicsSession::handleCommand(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session[{}] received command[{}] [{}]", id().string(), size, unsigned_char_to_string((unsigned char*) buffer, size));
	/*auto packetCommand = std::make_shared< parser::PacketCommand >();

	auto res = packetCommand->parse(buffer, size);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Handle command");
		return res;
	}*/
  const std::string cmd = "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE";
	std::vector<char> buf(cmd.size() / 2);
	//std::transform(cmd.begin(), cmd.end(), str_packet.begin(), ::toupper);

	tc::hex2bin((const char*) cmd.data(), buf.data());

	//LG_NFO(this->logger(), "before send command: {}", tc::unsigned_char_to_string(cmd.data(), 32));

	auto res = SendAsync((const void*) buf.data(), 32) == false ? RES_ERROR : RES_OK;

	//tcServer()->add(packetCommand->imei(), packetCommand);

	return res;
}

result_t TelematicsSession::handleIncomplete(const uchar *buffer, size_t size, bool &crc_ok)
{
	LG_NFO(this->logger(), "Session[{}] handle incomplete [{}]", id().string(), size);
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

result_t TelematicsSession::checkCrc(std::shared_ptr< parser::Buf > buf, size_t size, bool &crc_ok)
{
	if (buf == nullptr) {
		return RES_NOENT;
	}

	common::CRC16 crc;
	auto reader = std::make_shared< parser::Reader >(buf, size);
	auto reader_crc = reader->read(4, size - 4);

	parser::Buf subBuf(parser::Buf::ByteArray{buf->begin() + 8, buf->begin() + size - 4});
	auto calc = crc.calc((const uchar*) subBuf.iBuf.data(), subBuf.size());

	crc_ok = calc == (int)reader_crc ? true : false;

	if (crc_ok) {
		LG_NFO(this->logger(), "CRC ok.");
	} else {
		LG_WRN(this->logger(), "CRC incorrect.");
	}

	return RES_OK;
}

result_t TelematicsSession::send(int buffer, const bool async)
{
	LG_NFO(this->logger(), "Sent val: {} to client", buffer);
	return send(static_cast< void *>(&buffer), sizeof(buffer), async);
}

result_t TelematicsSession::send(void *buffer, size_t size, const bool async)
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
		LG_ERR(this->logger(), "Send bytes incomplete");
		return res;
	}

	LG_NFO(this->logger(), "Sent {} bytes to client", size);
	return res;
}

} // namespace tc::server::tcp
