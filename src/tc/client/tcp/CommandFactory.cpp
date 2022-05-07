/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes

*/
#include <tc/client/tcp/CommandFactory.h>
#include <tc/common/CRC16.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <bitset>

namespace tc::client::tcp {

const std::string CommandFactory::cmd_unlock = "sclockctrl 0";
const std::string CommandFactory::cmd_lock = "sclockctrl 1";

const std::string CommandFactory::cmd_led_on = "scledctrl 1";
const std::string CommandFactory::cmd_led_off = "scledctrl 0";

const std::string CommandFactory::cmd_engine_on = "scenginectrl 1";
const std::string CommandFactory::cmd_engine_off = "scenginectrl 0";

const std::string CommandFactory::cmdToString(const std::string &cmd)
{
	if (cmd.compare("unlock") == 0) {
		return CommandFactory::cmd_unlock;
	} else if (cmd.compare("lock") == 0) {
		return CommandFactory::cmd_lock;
	} else if (cmd.compare("engine_on") == 0) {
		return CommandFactory::cmd_engine_on;
	} else if (cmd.compare("engine_off") == 0) {
		return CommandFactory::cmd_engine_off;
	} else if (cmd.compare("led_on") == 0) {
		return CommandFactory::cmd_led_on;
	} else if (cmd.compare("led_off") == 0) {
		return CommandFactory::cmd_led_off;
	}

	return std::string();
}

CommandFactory::CommandFactory(const std::string &imei)
	: tc::LogI("console")
	, iImei(imei)
{
		// nothing to do
}

result_t CommandFactory::create(const std::string &cmd, parser::Buf &buf, bool cr)
{
	result_t res = RES_OK;

	// imei length
	auto val = byte2string(iImei.length(), 4);
	buf.insert((const uchar*) val.data(), val.length());

	// imei
	buf.insert((const uchar*) iImei.data(), iImei.length());

	// 4 zero-bytes
	val = byte2string(0, 4*2);
	buf.insert((const uchar*) val.data(), val.length());

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
	buf.insert((const uchar*) val.data(), val.length());

	// payload length
	val = byte2string(payload.size() / 2);
	buf.insert((const uchar*) val.data(), val.length());

	// insert payload
	buf.insert(payload.begin(), payload.end());

	// command end symbol
	val = cr ? byte2string(0x0D0A) : byte2string(0);
	buf.insert((const uchar*) val.data(), val.length());

	// CRC
	val = byte2string(sum, 3*2);
	buf.insert((const uchar*) val.data(), val.length());

	return res;
}

result_t CommandFactory::getPayload(const std::string &cmd, parser::Buf &buf)
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
	val = byte2string(CommandFactory::cmdToString(cmd).length());
	buf.insert(val.data(), val.length());

	// command
	const auto command = CommandFactory::cmdToString(cmd);
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
	common::CRC16 crc;
	std::string tocalc = "0C01050000000C73636C6F636B6374726C203001";
	auto strhex = hex2string(tocalc);

	for (auto &s : _sessions) {
		const char* data = "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE";

		// "00000000000000130C01050000000B73637365746D6F646520320100001F93"
		//const char* data = "00000000000000180C01050000001073637365746C656473776974636820300100004966"; // scsetledswitch off
		// "00000000000000130C01050000000B73636C65646374726C20310100003205"; // scledctrl turn ON
		//  00000000000000130C01050000000B73636c65646374726c2030010000A204 // scledctrl off
		// "00000000000000160C01050000000E7363656E67696E656374726C2031010000A39E" engine on BAD?
			  00000000000000160C01050000000E7363656e67696e656374726c2031010000E39C // engine on
		// "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE" sclockctrl lock off
				00000000000000140C01050000000C73636C6F636B6374726C203001000042C4

		// "00000000000000140C01050000000C73636C6F636B6374726C203101000042AF" sclockctrl lock on



		char *out = new char[32];
		tc::hexToBin(data, out);

		// auto str = hex_to_string_t(data);
		//"00000000000000140C01050000000773636C6F636B6374726C203001000019a4";
		//  auto out = ToHex(str, false);

		// auto out = string_to_hex_t(str);
		//  auto out = hex_to_string_t(str);
		// SPDLOG_LOGGER_INFO(this->logger(), "{}: {}", "ON SEND CMD", str);
		s.second->Send((void*) out, 32);
	}


	}*/

	/* CRC od C (pierwszego z lewej) do 01
	0C 010500000007676574696E666F 01

	length:

		od 18 (packet length)
		do 01
		"00000000000000140C01050000000773636C6F636B6374726C203001000019a4"
		00000000000000160C010500000010736574706172616D2032333920310100007B78 //setparam 239 1
	*/