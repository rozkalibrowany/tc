#include <tc/server/tcp/handler/Teltonika.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/teltonika/Action.h>
#include <magic_enum.hpp>

namespace tc::server::tcp {

using namespace parser::teltonika;
using namespace magic_enum;

result_t TeltonikaHandler::handle(const uchar* buffer, size_t size)
{
	switch(Action::get(buffer, size)) {
		case Action::eImei:
			return handleImei(buffer, size);
		case Action::ePayload:
			return handlePayload(buffer, size);
		case Action::eStandby:
			return handleStandby(buffer, size);
		case Action::eIncompletePayload:
			return handleIncomplete(buffer, size);
		case Action::eUnknown:
			LG_WRN(this->logger(), "[{}][{}] Unable to handle data buffer[{}]", enum_name(iType), iImei, size);
			return RES_NOENT;
	}

	return RES_NOENT;
}

result_t TeltonikaHandler::handleImei(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle imei[{}]", enum_name(iType), size);

	result_t res = RES_OK;
	Imei imei;

	res |= Packet::parseImei(buffer, size, imei);
	if (res != RES_OK) {
		LG_ERR(this->logger(), "[{}] Parse imei.", enum_name(iType));
		iSession->send(eInvalid);
		return res;
	}
	LG_NFO(this->logger(), "[{}] Parse imei OK, imei[{}]", enum_name(iType), imei);

	iDevice = std::make_unique<iot::Device>(imei, iType, iSession->cacheSize());
	res |= iSession->send(eOK);

	iImei = std::move(imei);
	return res;
}

result_t TeltonikaHandler::handlePayload(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}][{}] Handle payload [{}]", enum_name(iType), iImei, size);

	result_t res = RES_OK;

	auto packet = std::make_shared< parser::teltonika::Payload >();
	if ((res = packet->parse(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "[{}][{}] Parse payload packet", enum_name(iType), iImei);
		return res;
	}

	if (iSession->tServer()->dbClient()->enabled())
		res |= iSession->savePacket(packet);

	auto records_size = packet->records().size();

	res = iDevice->add(packet);
	if (res != RES_OK && res != RES_INVCRC) {
		LG_ERR(this->logger(), "[{}][{}] Unable to add packet", enum_name(iType), iImei);
		iSession->send(eInvalid);
		return res;
	}

	if (res == RES_INVCRC) {
		iBufferIncomplete = std::make_shared< common::Buf >(buffer, size);
		return res;
	}

	LG_DBG(this->logger(), "[{}][{}] Handle payload OK. Records[{}] ", enum_name(iType), iImei, records_size);

	res |= iSession->send(records_size);
	return res;
}

result_t TeltonikaHandler::handleIncomplete(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}][{}] Handle incomplete[{}]", enum_name(iType), iImei, size);
	result_t res = RES_OK;

	if(iBufferIncomplete == nullptr || size >= iBufferIncomplete->size()) {
		iSession->send(eInvalid);
		return res;
	}

	iBufferIncomplete->iBuf.insert(iBufferIncomplete->iBuf.end(), buffer, buffer + size);
	if((res = handlePayload((const uchar *)iBufferIncomplete->iBuf.data(), iBufferIncomplete->size())) != RES_OK) {
		LG_ERR(this->logger(), "[{}][{}] Unable to handle payload", enum_name(iType), iImei);
	}

	iBufferIncomplete.reset();
	iBufferIncomplete = nullptr;
	return res;
}

result_t TeltonikaHandler::handleStandby(const uchar *buffer, size_t size)
{
	LG_NFO(this->logger(), "[{}] Handle standby[{}]", enum_name(iType), size);

	return iSession->send(eOK);
}

} // namespace tc::server::tcp