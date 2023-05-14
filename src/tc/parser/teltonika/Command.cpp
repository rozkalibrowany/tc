/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes
*/
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/Packet.h>
#include <tc/common/CRC16.h>
#include <tc/parser/Protocol.h>

namespace tc::parser::teltonika {

 std::map <std::string, std::string> Command::sMapping = {
	{"unlock", "sclockctrl 0"},
	{"lock", "sclockctrl 1"},
	{"engine_on", "scenginectrl 1"},
	{"engine_off", "scenginectrl 0"},
	{"led_on", "scledctrl 1"},
	{"led_off", "scledctrl 0"},
	{"led_sw_on", "scsetledswitch 1"},
	{"led_sw_off", "scsetledswitch 0"},
	{"mode_normal", "scsetmode 0"},
	{"mode_eco", "scsetmode 1"},
	{"mode_sport", "scsetmode 2"},
	{"buzz_ctrl_off", "scbuzzerctrl 0 1"},
	{"buzz_ctrl_on", "scbuzzerctrl 1 1"},
	{"alarm_sw_on", "scsetalarmswitch 1"},
	{"alarm_sw_off", "scsetalarmswitch 0"},
	{"restart", "screbootsys"},
	{"get_mode", "scgetmode"}};

Command::Command(const uchar* cbuf, size_t size)
{
	parse(cbuf, size);
}

Command::Command(const std::string& cmd, const std::string& imei)
{
	try {
		create(cmd, imei);
	} catch (const std::invalid_argument &e) {
		LG_NFO(this->logger(), "Caught exception [{}]", e.what());
	}
}

result_t Command::parse(const uchar* cbuf, size_t size)
{
	result_t res = RES_OK;
	LG_NFO(this->logger(), "parse: {}", uchar2string(cbuf, size));

	common::Buf buf((uchar *)cbuf, size);
	Reader reader(buf);

	if (int protocol; (protocol = reader.read(1, 1)) != (int) Protocol::eTeltonika) 
		throw std::invalid_argument(fmt::format("Invalid protocol [{}]", protocol)); 

	if (int id; (id = reader.read(1, 2)) != (uchar) CommandI::c_id)
		throw std::invalid_argument(fmt::format("Invalid command [{}]", id)); 

	auto imei_len = reader.readU(1, 3);
	LG_NFO(this->logger(), "imei_len: {}", imei_len);
	iImei = std::string((const char*) reader.readS(imei_len, 4).data());
	iBuf = common::Buf(cbuf + 4 + imei_len, size - 4 - imei_len);

	LG_NFO(this->logger(), "command: {} offset: {}", tc::uchar2string(iBuf.data(), iBuf.size()), reader.offset());

	return res;
}

void Command::create(const std::string &cmd, const std::string& imei, Protocol::Type type)
{
	LG_NFO(this->logger(), "cmd[{}] imei[{}]", cmd, imei);

	iBuf.clear();
	try {
		CommandI::create(cmd, imei, type);
	} catch (const std::invalid_argument &e) {
		LG_NFO(this->logger(), "Caught exception [{}]", e.what());
		return;
	}

	iBuf.reserve(27 + imei.size() + cmd.size());

	if (cmd.empty() || imei.empty())
		throw std::invalid_argument("Invalid arguments"); 

	// 1 zero-byte
	auto val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// protocol type
	val = byte2string(Protocol::eTeltonika);
	iBuf.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) CommandI::c_id);
	iBuf.insert(val.data(), val.length());

	// imei length
	val = byte2string(imei.length());
	iBuf.insert(val.data(), val.length());

	// imei
	val = tc::tohex(imei);
	iBuf.insert(val.data(), val.length());

	// 4 zero-bytes
	val = byte2string(0, 4*2);
	iBuf.insert(val.data(), val.length());

	common::Buf payload;
	if (getPayload(cmd, payload) != RES_OK)
		throw std::invalid_argument(fmt::format("Unknown command [{}]", cmd)); 

	// CRC
	common::CRC16 crc;
	std::string payload_hex((const char*) payload.cdata(), payload.size());

	auto payload_str = tc::hex2string(payload_hex);
	auto sum = crc.calc(payload_str.data(), payload_str.size());

	// 3 zero-bytes
	val = byte2string(0, 3*2);
	iBuf.insert(val.data(), val.length());

	// payload length
	val = byte2string(payload.size() / 2);
	iBuf.insert(val.data(), val.length());

	// insert payload
	iBuf.insert(payload.begin(), payload.end());

	// command end symbol
	val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// CRC
	val = byte2string(sum, 3*2);
	iBuf.insert(val.data(), val.length());

	iImei = imei;
}

result_t Command::getPayload(const std::string &cmd, common::Buf &buf)
{
	// codec
	auto val = byte2string(CODEC_12);
	buf.insert(val.data(), val.length());

	// 0x1
	val= byte2string(1);
	buf.insert(val.data(), val.length());

	// type command
	val = byte2string(c_id);
	buf.insert(val.data(), val.length());

	// 3 zero-bytes
	val = byte2string(0, 3*2);
	buf.insert(val.data(), val.length());

	// command length
	val = byte2string(Command::sMapping.at(cmd).length());
	buf.insert(val.data(), val.length());

	// command
	const auto command = Command::sMapping.at(cmd);
	if (command.empty() == true) {
		return RES_INVARG;
	}
	val = tc::tohex(command);
	buf.insert(val.data(), val.length());

	// 0x1
	val = byte2string(1);
	buf.insert(val.data(), val.size());

	return RES_OK;
}

} // namespace tc::parser::teltonika