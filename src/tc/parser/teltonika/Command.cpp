/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes
*/
#include <tc/parser/teltonika/Command.h>
#include <tc/parser/Packet.h>
#include <tc/common/CRC16.h>

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

Command::Command(const std::string &imei)
	: tc::LogI("console")
	, iImei(imei)
{
	// nothing to do
}

result_t Command::create(const std::string &cmd, bool cr)
{
	result_t res = RES_OK;

	if (!iBuf.empty()) iBuf.clear();

	// 1 zero-byte
	auto val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// imei length
	val = byte2string(iImei.length());
	iBuf.insert(val.data(), val.length());

	// imei
	val = tc::tohex(iImei, cr);
	iBuf.insert(val.data(), val.length());

	// 4 zero-bytes
	val = byte2string(0, 4*2);
	iBuf.insert(val.data(), val.length());

	common::Buf payload;
	if ((res = getPayload(cmd, payload)) != RES_OK) {
		return res;
	}

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
	val = cr ? byte2string(0x0D0A) : byte2string(0);
	iBuf.insert(val.data(), val.length());

	// CRC
	val = byte2string(sum, 3*2);
	iBuf.insert(val.data(), val.length());

	return res;
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
	val = byte2string(TYPE_PACKET_COMMAND);
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


/*
		"00000000000000130C01050000000B73637365746D6F646520320100001F93"
		"00000000000000180C01050000001073637365746C656473776974636820300100004966"; // scsetledswitch off
		"00000000000000130C01050000000B73636C65646374726C20310100003205"; // scledctrl turn ON
		"00000000000000130C01050000000B73636c65646374726c2030010000A204 // scledctrl off
		"00000000000000160C01050000000E7363656E67696E656374726C2031010000A39E" engine on BAD?
		"00000000000000160C01050000000E7363656e67696e656374726c2031010000E39C // engine on
		"00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE" sclockctrl lock off
		"00000000000000140C01050000000C73636C6F636B6374726C203001000042C4
		"00000000000000140C01050000000C73636C6F636B6374726C203101000042AF" sclockctrl lock on

		CRC from C (first from left) until 01
		0C 010500000007676574696E666F 01
*/