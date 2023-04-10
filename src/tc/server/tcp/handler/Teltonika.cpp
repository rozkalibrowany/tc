#include <tc/server/tcp/handler/Teltonika.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/teltonika/Action.h>

namespace tc::server::tcp {

using namespace parser::teltonika;

TeltonikaHandler::TeltonikaHandler(const std::shared_ptr<TelematicsSession> &session)
 : HandlerI(session)
{
	// nothing to do
}

result_t TeltonikaHandler::handle(const uchar* buffer, size_t size)
{
	switch(Action::get(buffer, size)) {
		case Action::imei:
			return handleImei(buffer, size);
		case Action::payload:
			return handlePayload(buffer, size);
		case Action::standby:
			return handleStandby(buffer, size);
		case Action::incomplete_payload:
			return handleIncomplete(buffer, size);
		case Action::unknown:
			LG_WRN(this->logger(), "[{}] Unable to handle data buffer[{}]", iImei, size);
			return RES_NOENT;
	}

	return RES_NOENT;
}

result_t TeltonikaHandler::handleImei(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle imei[{}]", size);

	result_t res = RES_OK;
	Imei imei;

	res |= Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "Parse imei.");
		iSession->send(eInvalid);
		return res;
	}

	LG_NFO(this->logger(), "Parse imei OK, imei[{}]", imei);
	
	iDevice = std::make_unique<iot::Device>(imei, iSession->server()->cacheSize());
	res |= iSession->send(eOK);

	iImei = std::move(imei);
	return res;
}

result_t TeltonikaHandler::handlePayload(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle payload [{}]", iImei, size);

	result_t res = RES_OK;

	auto packet = std::make_shared< parser::teltonika::Payload >();
	if ((res = packet->parse(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "[{}] Parse payload packet", iImei);
		return res;
	}

	if (iSession->server()->dbClient()->enabled())
		res |= iSession->savePacket(packet);

	auto records = packet->records().size();

	{
		std::lock_guard lock(iMutex);
		res = iDevice->add(packet);
		if (res != RES_OK && res != RES_INVCRC) {
			LG_ERR(this->logger(), "[{}] Unable to add packet", iImei);
			iSession->send(eInvalid);
			return res;
		}
	}

	if (res == RES_INVCRC) {
		iBufferIncomplete = std::make_shared< common::Buf >(buffer, size);
		return res;
	}

	LG_DBG(this->logger(), "[{}] Handle payload succesfull. AVL records[{}] ", iImei, records);

	res |= iSession->send(records);
	return res;
}

result_t TeltonikaHandler::handleIncomplete(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle incomplete[{}]", iImei, size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || size >= iBufferIncomplete->size()) {
		iSession->send(eInvalid);
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

result_t TeltonikaHandler::handleStandby(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "Handle standby[{}]", size);

	return iSession->send(eOK);
}

} // namespace tc::server::tcp 