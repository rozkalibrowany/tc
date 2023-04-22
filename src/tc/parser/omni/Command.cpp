/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes
*/
#include <tc/parser/omni/Command.h>
#include <magic_enum.hpp>

namespace tc::parser::omni {

result_t Command::getHeader(common::Buf &buf, const Imei &imei, const Datetime &datetime)
{
	if (imei.empty())
		return RES_NOENT;

	// 1 bytes FF
	auto val = byte2string(0xFF, 2);
	buf.insert(val.data(), val.length());

	// 1 bytes FF
	val = byte2string(0xFF, 2);
	buf.insert(val.data(), val.length());
	
	// header
	val = tc::tohex(c_header);
	buf.insert(val.data(), val.length());
	val = byte2string(0x2c);
	buf.insert(val.data(), val.length());

	// manufacturer
	val = tc::tohex(c_manufacturer);
	buf.insert(val.data(), val.length());
	val = byte2string(0x2c);
	buf.insert(val.data(), val.length());

	// imei
	val = tc::tohex(imei);
	buf.insert(val.data(), val.length());
	val = byte2string(0x2c);
	buf.insert(val.data(), val.length());

	// datetime
	val = tc::tohex(datetime.datetime());
	buf.insert(val.data(), val.length());
	val = byte2string(0x2c);
	buf.insert(val.data(), val.length());

	return RES_OK;
}

Command::Command(const Imei &imei, uint userID)
	: tc::LogI("console")
	, iImei(imei)
	, iUserID(userID)
{
	// nothing to do
}

result_t Command::init(action::Server::Instruction instruction, int value)
{
	result_t res = RES_OK;

	common::Buf command;
	if (getHeader(command, iImei, iDateTime) != RES_OK) {
		return RES_INVARG;
	}

	// instruction
	auto val = byte2string(instruction);
	command.insert(val.data(), val.length());
	command.push_back(',');

	switch(instruction) {
		case action::Server::eUnlock:
			// value
			val = byte2string(value);
			command.insert(val.data(), val.length());
			command.push_back(',');
			// user id
			val = byte2string(iUserID);
			command.insert(val.data(), val.length());
			command.push_back(',');
			// timestamp
			val = byte2string(iDateTime.timestamp());
			command.insert(val.data(), val.length());
			command.push_back(',');
			break;
		case action::Server::eBikeSearch:
			// value
			val = byte2string(value);
			command.insert(val.data(), val.length());
			command.push_back(',');
			break;
		case action::Server::eLocationTracing:
			// value
			val = byte2string(value);
			command.insert(val.data(), val.length());
			command.push_back(',');
			break;
		case action::Server::eAlarm:
			// value
			val = byte2string(value);
			command.insert(val.data(), val.length());
			command.push_back(',');
			break;
		case action::Server::eAcquisitionPositioning:
		case action::Server::eLockInformation:
		case action::Server::eAcquireLockFirmware:
		case action::Server::eUnknown:
			break;
		}
	// end
	command.push_back('#');

	return res;
}

} // namespace tc::parser::omni
