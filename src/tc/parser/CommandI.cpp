#include <tc/parser/CommandI.h>
#include <tc/common/MagicEnum.h>

namespace tc::parser {

CommandI::CommandI(const common::Buf &buf, const Imei& imei)
 : iBuf(buf)
 , iImei(imei)
{
	// nothing to do
}

CommandI::CommandI(const uchar* cbuf, size_t size)
 : CommandI(common::Buf{cbuf, size}, "unknown")
{
	// nothing to do
}

CommandI::CommandI(const Imei& imei)
 : iImei(imei)
{
	// nothing to do
}

CommandI::CommandI()
 : CommandI("unknown")
{
	// nothing to do
}

void CommandI::create(const std::string &cmd, const std::string& imei, Protocol::Type type)
{
	iBuf.clear();

	if (cmd.empty() || imei.empty())
		throw std::invalid_argument("Invalid arguments"); 

	// 1 zero-byte
	auto val = byte2string(0);
	iBuf.insert(val.data(), val.length());

	// protocol type
	val = byte2string(type);
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
}

bool CommandI::hasCommand(const uchar* cbuf, size_t size)
{
	if (size < 2)
		return false;
	// 0x0 | protocol | type (command, request)
	return cbuf[2] == static_cast<uchar>(CommandI::c_id) ? true : false;
}

Protocol::Type CommandI::getProtocol(const uchar* cbuf, size_t size)
{
	if (size < 1)
		return Protocol::eUnknown;
	return enum_cast<Protocol::Type>(cbuf[1]).value();
}

const size_t CommandI::size() const
{
	return iBuf.size();
}

common::Buf CommandI::buf() const
{
	return iBuf;
}

Imei CommandI::imei() const
{
	return iImei;
}

} // namespace tc::parser