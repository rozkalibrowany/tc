#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/packet/PacketPayload.h>
#include <tc/common/CRC16.h>

namespace tc::server::tcp {

std::shared_ptr<TelematicsServer> TelematicsSession::tcServer()
{
	std::shared_ptr<TelematicsServer> server = std::dynamic_pointer_cast<TelematicsServer>(this->server());
	return server;
}

void TelematicsSession::onReceived(const void *buffer, size_t size)
{
	LG_NFO(this->logger(), "Session[{}] received buffer[{}]", id().string(), unsigned_char_to_string((const uchar*) buffer, size));

	Action action;
	if (action.parse(buffer, size) != RES_OK)
	{
		LG_ERR(this->logger(), "Unable to parse action");
		return;
	}

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

	LG_NFO(this->logger(), "bytes_pending(): {}", (int) this->bytes_pending());

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

	if (iImei.empty()) { //  && iServer->has(iImei) == true
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
	return RES_NOIMPL;
	/*auto packetCommand = std::make_shared< PacketCommand >();

	res = packetCommand->parse(buffer, size);

	return res;*/
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
