#include <tc/parser/omni/records/Unlock.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t Unlock::parse(uint8_t elements)
{
	if (RecordI::parse() != RES_OK)
		return RES_NOENT;

	auto offset = iBuf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	setOffset(offset);

	auto result = read(1);
	skip(1);

	auto customerID = read(4);
	skip(1);

	auto timestamp = read(10);
	skip(1);

	iResult = result;
	iCustomerID = customerID;
	iTimestamp.timestamp(timestamp);

	return RES_OK;
}

result_t Unlock::response(common::Buf& response)
{
	if (Command::getHeader(response, iImei, iDateTime) != RES_OK)
		return RES_NOENT;

	// re
	auto val = tc::tohex(c_response);
	response.insert(val.data(), val.length());
	val = byte2string(0x2c);
	response.insert(val.data(), val.length());

	// instruction
	val = byte2string(action::Server::eUnlock);
	response.insert(val.data(), val.length());

	// end
	val = byte2string(0x23);
	response.insert(val.data(), val.length());

	return RES_OK;
}

SysTime Unlock::timestamp() const
{
	return iTimestamp;
}

uint8_t Unlock::result() const
{
	return iResult;
}

int Unlock::customerID() const
{
	return iCustomerID;
}

} // namespace tc::parser::omni::records