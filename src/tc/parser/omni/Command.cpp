/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes
*/
#include <tc/parser/omni/Command.h>
#include <magic_enum.hpp>

namespace tc::parser::omni {

result_t Command::getHeader(common::Buf &buf, const Imei &imei, const std::string &datetime)
{
	if (imei.empty())
		return RES_NOENT;

	// 1 bytes FF
	auto val = byte2string(0xFF, 1);
	buf.insert(val.data(), val.length());

	// 1 bytes FF
	val = byte2string(0xFF, 1);
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
	val = tc::tohex(datetime);
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

Command &Command::operator=(const Command &rhs)
{
	iBuf = rhs.iBuf;
	iImei = rhs.iImei;
	iUserID = rhs.iUserID;
	iDatetime = rhs.iDatetime;
	return *this;
}

common::Buf& Command::buf()
{
	return iBuf;
}

result_t Command::init(const common::Buf &buf, const Imei &imei, const std::string &datetime)
{
	common::Buf header;
	if (auto res = Command::getHeader(header, imei, datetime); res != RES_OK) {
		LG_ERR(this->logger(), "Unable to generate command");
		return res;
	}

	iBuf.iBuf.reserve(buf.size() + header.size());
	iBuf.insert(header.cdata(), header.size());
	iBuf.insert(buf.cdata(), buf.size());

	return RES_OK;
}

result_t Command::init(action::Server::Event event, int value)
{
	result_t res = RES_OK;

	common::Buf command;
	if (getHeader(command, iImei, iDatetime) != RES_OK) {
		return RES_INVARG;
	}

	// instruction
	auto val = byte2string(event);
	command.insert(val.data(), val.length());
	command.push_back(',');

	switch(event) {
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
			val = tc::tohex(iDatetime);
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
