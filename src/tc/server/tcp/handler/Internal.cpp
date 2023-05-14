#include <tc/server/tcp/handler/Internal.h>
#include <tc/server/tcp/TelematicsSession.h>
#include <tc/server/tcp/TelematicsServer.h>
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/omni/Command.h>
#include <tc/parser/internal/Action.h>
#include <tc/parser/internal/Request.h>
#include <tc/common/MagicEnum.h>

namespace tc::server::tcp {

using namespace parser::internal;

result_t InternalHandler::handle(const uchar* buffer, size_t size)
{
	common::Buf buf(buffer, size);

	// get action (command, request)
	auto action = parser::internal::Action::get(buffer, size);
	if (action == parser::internal::Action::eUnknown)
		return RES_NOENT;

	if (action == parser::internal::Action::eCommand)
		return handleCommand(buffer, size);

	if (action == parser::internal::Action::eData)
		return handleRequest(buffer, size);

	return RES_OK;
}

result_t InternalHandler::handleCommand(const uchar *buffer, size_t size)
{
	switch(parser::CommandI::getProtocol(buffer, size)) {
		case Protocol::eTeltonika:
			return handleTeltonikaCommand(buffer, size);
		case Protocol::eOmni:
			return handleOmniCommand(buffer, size);
		case Protocol::eInternal:
		case Protocol::eUnknown:
			return RES_NOENT;
	}
	return RES_NOENT;
}

result_t InternalHandler::handleRequest(const uchar *buffer, size_t size)
{
	Request request;
	if (result_t res; (res = request.parse(buffer, size)) != RES_OK) {
		LG_ERR(this->logger(), "Unable to parse internal request");
		return res;
	}

	if (result_t res; (res = iSession->tServer()->handleRequest(request, iSession->id())) != RES_OK) {
		LG_ERR(this->logger(), "Unable to handle internal request[{}]", enum_name(request.type()));
		return res;
	}

	LG_NFO(this->logger(), "[{}] Handle request [{}]", enum_name(iType), enum_name(request.type()));
	return RES_OK;
}

result_t InternalHandler::handleTeltonikaCommand(const uchar *buffer, size_t size)
{
	try {
		auto command = std::make_shared<teltonika::Command>(buffer, size);
		if (result_t res; (res = iSession->tServer()->sendCommand(dynamic_pointer_cast<CommandI>(command))) != RES_OK)
			return res;
	} catch (const std::invalid_argument& e) {
		LG_ERR(this->logger(), "Catched exeption, {}", e.what());
		return RES_NOENT;
	}

	LG_NFO(this->logger(), "[{}] Handle command", enum_name(iType));
	return RES_OK;
}

result_t InternalHandler::handleOmniCommand(const uchar *buffer, size_t size)
{
	try {
		auto command = std::make_shared<omni::Command>(buffer, size);
		if (result_t res; (res = iSession->tServer()->sendCommand(dynamic_pointer_cast<CommandI>(command))) != RES_OK)
			return res;
	} catch (const std::invalid_argument& e) {
		LG_ERR(this->logger(), "Catched exeption, {}", e.what());
		return RES_NOENT;
	}

	LG_NFO(this->logger(), "[{}] Handle command", enum_name(iType));
	return RES_OK;
}

} // namespace tc::server::tcp