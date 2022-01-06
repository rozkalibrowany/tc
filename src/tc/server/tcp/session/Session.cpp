#include <tc/server/tcp/session/Session.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <tc/common/Convert.h>

namespace tc::server::tcp {

int64_t Session::timestamp() const
{
	return iTimestamp;
}

void Session::onConnected()
{
	spdlog::info("TCP session with Id {} connected!", id().string());
}

void Session::onDisconnected()
{
	spdlog::info("TCP session with Id {} disconnected!", id().string());
}

void Session::onReceived(const void* buffer, size_t size)
{
	spdlog::info("TCP session got buffer with size[ {} ]", (int)size);
	spdlog::info("Buffer: {}", unsigned_char_to_string((unsigned char *)buffer, (uint32_t) size));

	int accept_hex = 1;
	Send(static_cast<void *>(&accept_hex), sizeof(accept_hex));
	return;

	if (size <= 5)
	{
		int accept_hex = 1;
		Send(static_cast<void *>(&accept_hex), sizeof(accept_hex));
		return;
	}

	auto packet = std::make_shared<parser::Packet>();
	if (packet->parse((unsigned char *)buffer, size) != RES_OK) {
		spdlog::warn("Unable to parse data buffer");
		return;
	}

	spdlog::info("Parsing OK", packet->codec());

	// check if IMEI is in database/config file
	// send response 0x1 if accept, 0x0 if denied
	spdlog::info("TCP packet size {}", (int) packet->size());

	if (packet->size() == 0) {
		int accept_hex = 1;
		Send(static_cast<void *>(&accept_hex), sizeof(accept_hex));
	} else {
		int accept_hex = packet->size();
		Send(static_cast<void *>(&accept_hex), sizeof(accept_hex));
	}

	iPacket = std::move(packet);
}

void Session::onError(int error, const std::string& category, const std::string& message)
{
	spdlog::info("TCP session with Id {} disconnected!", id().string());
}

} // namespace tc::server::tcp