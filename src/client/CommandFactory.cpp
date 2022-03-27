/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes

*/

#ifndef E9477072_6179_498A_92A0_E6BDB3FF0516
#define E9477072_6179_498A_92A0_E6BDB3FF0516

#include <tc/client/CommandFactory.h>
#include <tc/common/CRC16.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace tc::client::tcp {

const std::string CommandFactory::cmd_unlock = "sclockctrl 0";
const std::string CommandFactory::cmd_lock = "sclockctrl 1";

const std::string CommandFactory::cmd_led_on = "scledctrl 1";
const std::string CommandFactory::cmd_led_off = "scledctrl 0";

const std::string CommandFactory::cmd_engine_on = "scenginectrl 1";
const std::string CommandFactory::cmd_engine_off = "scenginectrl 0";

CommandFactory::CommandFactory(const std::string &imei)
	: tc::LogI("console")
	, iImei(imei)
{
		// nothing to do
}

result_t CommandFactory::create(std::map< int, std::vector< uchar > > &cmdMap, const std::string &cmd, bool cr)
{
	result_t res = RES_INVARG;

	auto command = cmd;
	std::transform(command.begin(), command.end(), command.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (command.compare("unlock") == 0) {
		std::vector<uchar> unlock;
		std::string sPacket = "00000000"; // 4 zero-bytes

		std::string data;
		data += "0C"; //0x0C
		data += "01"; //0x1
		data += "05"; //0x05
		data += "000000"; // 3 zero-bytes
		data += getHex(CommandFactory::cmd_unlock.length());
		data += tc::tohex(CommandFactory::cmd_unlock, true);
		data += "01"; //0x1

		common::CRC16 crc;
		auto crcStr = data;
		crcStr.erase(0, 1);
		auto sum = crc.calc(crcStr);
		auto calc = getHex(sum);

		sPacket += "000000"; // 3 zero-bytes
		sPacket += getHex(data.length() / 2);
		sPacket += data;
		sPacket += cr ? "0D0A" : "0000"; // <CR><LF>
		sPacket += calc;

		res |= convertCommand(unlock, sPacket);

		for (const auto c : unlock) {
			std::cout << c;
		}
		std::cout << std::endl;
	}
	return RES_OK;
}

inline std::string CommandFactory::getHex(int val, int width)
{
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << std::hex << (val);
	std::string res(ss.str());

	std::transform(res.begin(), res.end(), res.begin(),
		[](unsigned char c) { return std::toupper(c); });

	return res;
}

result_t CommandFactory::convertCommand(std::vector<uchar> &packet, const std::string &cmd)
{
	std::string stringHex = tc::tohex(cmd, true);

	std::transform(stringHex.begin(), stringHex.end(), back_inserter(packet),
    [](uchar c) { return reinterpret_cast<uchar>(c); });

	return RES_OK;
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



#endif /* E9477072_6179_498A_92A0_E6BDB3FF0516 */
