/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes

*/
#include <tc/parser/Command.h>
#include <tc/parser/packet/Packet.h>
#include <tc/common/CRC16.h>

namespace tc::parser {

const std::string Command::unlock = "sclockctrl 0";
const std::string Command::lock = "sclockctrl 1";

const std::string Command::led_on = "scledctrl 1";
const std::string Command::led_off = "scledctrl 0";

const std::string Command::engine_on = "scenginectrl 1";
const std::string Command::engine_off = "scenginectrl 0";

const std::string Command::restart = "screbootsys";

const std::string Command::toString(const std::string &cmd)
{
	if (cmd.compare("unlock") == 0) {
		return Command::unlock;
	} else if (cmd.compare("lock") == 0) {
		return Command::lock;
	} else if (cmd.compare("engine_on") == 0) {
		return Command::engine_on;
	} else if (cmd.compare("engine_off") == 0) {
		return Command::engine_off;
	} else if (cmd.compare("led_on") == 0) {
		return Command::led_on;
	} else if (cmd.compare("led_off") == 0) {
		return Command::led_off;
	} else if (cmd.compare("restart") == 0) {
		return Command::restart;
	}

	return std::string();
}

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

	parser::Buf payload;
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

std::vector<char> Command::asBin()
{
	std::vector<char> bin(iBuf.size() / 2);
	tc::hex2bin((char*) iBuf.data(), (char*) bin.data());
	return bin;
}

result_t Command::getPayload(const std::string &cmd, parser::Buf &buf)
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
	val = byte2string(Command::toString(cmd).length());
	buf.insert(val.data(), val.length());

	// command
	const auto command = Command::toString(cmd);
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

} // namespace tc::client::tcp


/*
	// CRC

		// "00000000000000130C01050000000B73637365746D6F646520320100001F93"
		//const char* data = "00000000000000180C01050000001073637365746C656473776974636820300100004966"; // scsetledswitch off
		// "00000000000000130C01050000000B73636C65646374726C20310100003205"; // scledctrl turn ON
		//  00000000000000130C01050000000B73636c65646374726c2030010000A204 // scledctrl off
		// "00000000000000160C01050000000E7363656E67696E656374726C2031010000A39E" engine on BAD?
			  00000000000000160C01050000000E7363656e67696e656374726c2031010000E39C // engine on
		// "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE" sclockctrl lock off
				00000000000000140C01050000000C73636C6F636B6374726C203001000042C4

		// "00000000000000140C01050000000C73636C6F636B6374726C203101000042AF" sclockctrl lock on

*/

	/* CRC od C (pierwszego z lewej) do 01
	0C 010500000007676574696E666F 01

	length:

		od 18 (packet length)
		do 01
		"00000000000000140C01050000000773636C6F636B6374726C203001000019a4"
		00000000000000160C010500000010736574706172616D2032333920310100007B78 //setparam 239 1
	*/