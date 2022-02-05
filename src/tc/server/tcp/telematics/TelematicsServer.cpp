#include <tc/server/tcp/telematics/TelematicsServer.h>

namespace tc::server::tcp {

TelematicsServer::TelematicsServer(const std::shared_ptr< Service >& service, int port)
 : CppServer::Asio::TCPServer(service, port, CppServer::Asio::InternetProtocol::IPv4)
 , tc::LogI("console")
{
 // nothing to do
}

int TelematicsServer::sessionsSize()
{
	LG_NFO(this->logger(), "________PACKET_MAP____________");
	for (auto &s : iPayloadPackets) {
		LG_NFO(this->logger(), "packet.imei: {}  packetSet.size: {}", s.first, (int) s.second.size());
	}
	LG_NFO(this->logger(), "____________________________\n");

	return _sessions.size();
}

void TelematicsServer::sendCmd() {

	for (auto &s : _sessions) {
		const char* data = "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE";
		// "00000000000000130C01050000000B73637365746D6F646520320100001F93"
		//const char* data = "00000000000000180C01050000001073637365746C656473776974636820300100004966"; // scsetledswitch off
		// "00000000000000130C01050000000B73636C65646374726C20310100003205"; // scledctrl turn ON
		// "00000000000000160C01050000000E7363656E67696E656374726C2031010000A39E" engine on
		// "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE" sclockctrl lock off
		// "00000000000000140C01050000000C73636C6F636B6374726C203101000042AF" sclockctrl lock on
		char *out = new char[32];
		tc::hex2bin(data, out);

		// auto str = hex_to_string_t(data);
		//"00000000000000140C01050000000773636C6F636B6374726C203001000019a4";
		//  auto out = ToHex(str, false);

		// auto out = string_to_hex_t(str);
		//  auto out = hex_to_string_t(str);
		// SPDLOG_LOGGER_INFO(this->logger(), "{}: {}", "ON SEND CMD", str);
		s.second->Send((void*) out, 32);
	}
}

TelematicsServer::PayloadPackets &TelematicsServer::payloadPackets()
{
	return iPayloadPackets;
}

std::shared_ptr< TCPSession > TelematicsServer::CreateSession(const std::shared_ptr< TCPServer > &server)
{
	return std::make_shared< TelematicsSession >(server);
}

/*void TelematicsServer::onConnected(std::shared_ptr< TelematicsSession > &session)
{

	parser::Packet packet(true);
	iSessionPacketMap.insert(std::make_pair(session->id(), std::move(packet)));
	LG_DBG(this->logger(), "TCP session connected, ID: {}", session->ids());

	for (auto &s : this->_sessions) {
		auto _session = std::dynamic_pointer_cast< TelematicsSession >(s.second);
		auto its = iSessionPacketMap.find(_session->id());
		if (its == iSessionPacketMap.end()) {
			continue;
		}
	}
}*/

result_t TelematicsServer::get(const Imei &imei, std::shared_ptr< PacketPayload > &packet)
{
	LockGuard g(iMutex);
	auto it = iPayloadPackets.find(imei);
	if (it == iPayloadPackets.end()) {
		return RES_NOENT;
	}

	packet = iPayloadPackets.at(imei).back();
	return RES_OK;
}

result_t TelematicsServer::add(const Imei imei, const std::shared_ptr< PacketPayload > &packet)
{
	LockGuard g(iMutex);
	if (iPayloadPackets.find(imei) != iPayloadPackets.end()) {
		auto &packets = iPayloadPackets.at(imei);
		packets.emplace_back(std::move(packet));
	} else {
		Packets packets;
		packets.emplace_back(std::move(packet));
		iPayloadPackets.insert(std::make_pair(imei, std::move(packets)));
	}

	return RES_OK;
}

void TelematicsServer::onConnected(std::shared_ptr< TCPSession > &session)
{
	LG_DBG(this->logger(), "TCP session connected, UUID: {} session: {}", session->id().string(), fmt::ptr(session.get()));
}

/*void TelematicsServer::onDisconnected(std::shared_ptr< TelematicsSession > &session)
{
	LG_NFO(this->logger(), "TCP session disconnected, ID: {} session {}", session->ids(), fmt::ptr(session.get()));
}*/


void TelematicsServer::onDisconnected(std::shared_ptr<TCPSession> &session)
{
	LG_NFO(this->logger(), "TCP session disconnected, ID: {} session {}", session->id().string(), fmt::ptr(session.get()));


	if (session != nullptr && session->IsConnected() == false) {
		auto dis = session->Disconnect();
		if (dis) session.reset();
	}
	//auto s = std::dynamic_pointer_cast< TelematicsSession >(session);
	//return onDisconnected(s);
}
void TelematicsServer::onError(int error, const std::string& category, const std::string& message)
{
	LG_ERR(this->logger(), "Telematics Server caught an error[{}][{}]: {}", error, category, message);
}

} // namespace tc::server::tcp