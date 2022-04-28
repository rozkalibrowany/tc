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

const std::string CommandFactory::getCommandType(const std::string &cmd)
{
	if (cmd.compare("unlock") == 0) {
		return CommandFactory::cmd_unlock;
	} else if (cmd.compare("lock") == 0) {
		return CommandFactory::cmd_lock;
	} else if (cmd.compare("engine_on") == 0) {
		return CommandFactory::cmd_engine_on;
	} else if (cmd.compare("engine_off") == 0) {
		return CommandFactory::cmd_engine_off;
	}
	return std::string();
}

CommandFactory::CommandFactory(const std::string &imei)
	: tc::LogI("console")
	, iImei(imei)
{
		// nothing to do
}

/*std::string CommandFactory::create(const std::string &cmd, bool cr)
{
	auto command = cmd;
	std::transform(command.begin(), command.end(), command.begin(),
		[](unsigned char c) { return std::tolower(c); });

	std::string sPacket = getStringHex(iImei.length(), 4); // imei length
	sPacket += iImei; // add imei
	sPacket += "00000000"; // 4 zero-bytes
	sPacket += iImei.length() % 2 != 0 ? "0" : "";

	std::string data;
	data += "0C"; //0x0C
	data += "01"; //0x1
	data += "05"; //0x05
	data += "000000"; // 3 zero-bytes
	data += getStringHex(CommandFactory::getCommandType(cmd).length());
	data += tc::tohex(CommandFactory::getCommandType(cmd), true);
	data += "01"; //0x1


	//common::CRC16 crc;
	//auto sum = crc.calc((const uchar *) data.data(), data.size());
	//auto calc = getStringHex(sum);
	auto calc = "D2AE";

	//LG_NFO(this->logger(), "sum: {} calc: {}", sum, calc);

	sPacket += "000000"; // 3 zero-bytes
	sPacket += getStringHex(data.length() / 2);
	sPacket += data;
	sPacket += cr ? "0D0A" : "0000"; // <CR><LF>
	sPacket += calc;

	return sPacket;
}*/

result_t CommandFactory::create(const std::string &cmd, parser::Buf &buf)
{
	//parser::Buf::ByteArray::iterator it = buf.begin();
	//std::fill(it, it + 2, 1); // first two bytes are zeros

	// imei length
	auto imei_size = getStringHex(iImei.length(), 4);
	buf.insert((const uchar*)imei_size.data(), imei_size.length());
	// imei
	buf.insert((const uchar*) iImei.data(), iImei.length());

	// 4 zero-bytes
	auto zeros = getStringHex(0, 4);
	buf.insert((const uchar*) zeros.data(), zeros.length());

	return RES_OK;
}

inline std::string CommandFactory::getStringHex(int val, int width)
{
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << std::hex << (val);
	std::string res(ss.str());

	std::transform(res.begin(), res.end(), res.begin(),
		[](char c) { return std::toupper(c); });

	return res;
}


} // namespace tc::client::tcp


/*

	for (auto &s : _sessions) {
		const char* data = "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE";
		// "00000000000000130C01050000000B73637365746D6F646520320100001F93"
		//const char* data = "00000000000000180C01050000001073637365746C656473776974636820300100004966"; // scsetledswitch off
		// "00000000000000130C01050000000B73636C65646374726C20310100003205"; // scledctrl turn ON
		// "00000000000000160C01050000000E7363656E67696E656374726C2031010000A39E" engine on
		// "00000000000000140C01050000000C73636C6F636B6374726C2030010000D2AE" sclockctrl lock off
				00000000000000140C01050000000C73636C6F636B6374726C203001000042C4

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


	}*/

	/* CRC od C (pierwszego z lewej) do 01
	0C 010500000007676574696E666F 01

	length:

		od 18 (packet length)
		do 01
		"00000000000000140C01050000000773636C6F636B6374726C203001000019a4"
		00000000000000160C010500000010736574706172616D2032333920310100007B78 //setparam 239 1
	*/