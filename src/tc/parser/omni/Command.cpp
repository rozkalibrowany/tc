/*

0x00000000	Data size	0x0C	Command quantity	0x05	Command size	Command	<CR><LF>	Command quantity	CRC
4 bytes	4 bytes	1 byte	1 byte	1 byte	4 bytes	X bytes	0D0A	1 byte	4 bytes

*/

#include <tc/parser/omni/Command.h>
#include <tc/common/MagicEnum.h>
#include <tc/parser/Reader.h>

namespace tc::parser::omni {

std::map <std::string, std::pair<Server::Event, int>> Command::sMapping = {
	{"unlock", std::make_pair(Server::eUnlock, 0)},
	{"bike_search", std::make_pair(Server::eBikeSearch, 0)},
	{"restart", std::make_pair(Server::eRestart, -1)},
	{"shutdown", std::make_pair(Server::eShutdown, -1)}
};

result_t Command::getHeader(common::Buf &buf, const Imei &imei, const std::string &datetime)
{
	if (imei.empty())
		return RES_NOENT;

	// 2 bytes 0xFF
	buf.push_back('\xff');
	buf.push_back('\xff');

	// header
	buf.insert(c_header.data(), c_header.size());
	buf.push_back(',');

	// manufacturer
	buf.insert(c_manufacturer.data(), c_manufacturer.size());
	buf.push_back(',');

	// imei
	buf.insert(imei.data(), imei.size());
	buf.push_back(',');

	// datetime
	buf.insert(datetime.data(), datetime.size());
	buf.push_back(',');

	return RES_OK;
}

Command::Command(const Imei& imei)
 : CommandI(imei)
{
	// nothing to do
}

Command::Command(const uchar* cbuf, size_t size)
{
	parse(cbuf, size);
}

Command::Command(const std::string& cmd, const Imei &imei, uint userID)
	: CommandI(imei)
	, iUserID(userID)
{
	create(cmd, imei);
}

Command &Command::operator=(const Command &rhs)
{
	iBuf = rhs.iBuf;
	iImei = rhs.iImei;
	iUserID = rhs.iUserID;
	iDatetime = rhs.iDatetime;
	return *this;
}

result_t Command::parse(const uchar* cbuf, size_t size)
{
	result_t res = RES_OK;
	std::string str_input = std::string((const char*) cbuf, size);
	LG_NFO(this->logger(), "parse: {} hexAsText: {} str_input: {}", uchar2string(cbuf, size), tc::hexAsText(cbuf, size), str_input);

	common::Buf buf((uchar *)cbuf, size);
	Reader reader(buf);

	if (int protocol; (protocol = reader.read(1, 1)) != (int) Protocol::eOmni) 
		throw std::invalid_argument(fmt::format("Invalid protocol [{}]", protocol)); 

	if (int id; (id = reader.read(1, 2)) != (uchar) CommandI::c_id)
		throw std::invalid_argument(fmt::format("Invalid command [{}]", id)); 

	auto imei_len = reader.readU(1, 3);
	LG_NFO(this->logger(), "imei_len: {}", imei_len);
	iImei = std::string((const char*) reader.readS(imei_len, 4).data());
	auto str = std::string((const char*) cbuf + 4 + imei_len, size - 4 - imei_len);
	auto hex = tohex(str);

	iBuf = common::Buf((const uchar*) hex.data(), hex.size());

	LG_NFO(this->logger(), "command: {}", tc::hexAsText(iBuf.data(), iBuf.size()));
	return res;
}

common::Buf& Command::buf()
{
	return iBuf;
}

result_t Command::create(const common::Buf &buf, const Imei &imei, const std::string &datetime)
{
	common::Buf header;
	if (auto res = Command::getHeader(header, imei, datetime); res != RES_OK) {
		LG_ERR(this->logger(), "Unable to generate command");
		return res;
	}

	iBuf.reserve(buf.size() + header.size());
	iBuf.insert(header.cdata(), header.size());
	iBuf.insert(buf.cdata(), buf.size());

	return RES_OK;
}

void Command::create(const std::string& cmd, const Imei &imei, Protocol::Type type)
{
	auto el = sMapping.find(cmd);
	if (el == sMapping.end())
		throw std::invalid_argument(fmt::format("Invalid command [{}]", cmd));
	
	common::Buf command;
	// 1 zero-byte
	auto val = byte2string(0);
	command.insert(val.data(), val.length());

	// protocol type
	val = byte2string(type);
	command.insert(val.data(), val.length());

	// packet type
	val = byte2string((int) CommandI::c_id);
	command.insert(val.data(), val.length());

	// imei length
	val = byte2string(imei.length());
	command.insert(val.data(), val.length());

	// imei
	val = tc::tohex(imei);
	command.insert(val.data(), val.length());


	/*if (getHeader(command, imei, iDatetime.getDateWithoudDelim()) != RES_OK)
		throw std::invalid_argument(fmt::format("Invalid arguments [{}]", cmd));
	*/
	// instruction
	auto instruction = hexAsText(byte2string((el->second.first)));
	command.insert(instruction.data(), instruction.size());
	command.push_back(',');

	switch(el->second.first) {
		case action::Server::eUnlock:
			// value
			val = byte2string(el->second.second, 1);
			command.insert(val.data(), val.length());
			command.push_back(',');

			// user id
			val = byte2string(iUserID, 4);
			command.insert(val.data(), val.length());
			command.push_back(',');
			// timestamp
			val = std::to_string(iDatetime.timestamp());
			command.insert(val.data(), val.size());
			break;
		case action::Server::eBikeSearch:
		case action::Server::eLocationTracing:
		case action::Server::eAlarm:
			// value
			val = byte2string(el->second.second);
			command.insert(val.data(), val.length());
			command.push_back(',');
			break;
		case action::Server::eAcquisitionPositioning:
		case action::Server::eLockInformation:
		case action::Server::eAcquireLockFirmware:
			throw std::invalid_argument(fmt::format("Unsupported instruction [{}]", enum_name(el->second.first)));
			break;
		case action::Server::eRestart:
			break;
		case action::Server::eShutdown:
			break;
		case action::Server::eUnknown:
			throw std::invalid_argument(fmt::format("Unsupported instruction [{}]", enum_name(el->second.first)));
			break;
	}
	// end
	command.push_back('#');
	iBuf = std::move(command);
}

} // namespace tc::parser::omni
