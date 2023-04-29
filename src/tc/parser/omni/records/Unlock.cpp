#include <tc/parser/omni/records/Unlock.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t Unlock::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto offset = buf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	Reader reader(buf);
	reader.setOffset(offset);

	auto result = reader.read(1);
	reader.skip(1);

	auto customerID = reader.read(4);
	reader.skip(1);

	auto timestamp = reader.read(10);
	reader.skip(1);

	iResult = result;
	iCustomerID = customerID;
	iTimestamp.timestamp(timestamp);

	return RES_OK;
}

result_t Unlock::response(common::Buf& response)
{
	// re
	auto val = tc::tohex(c_response);
	response.insert(val.data(), val.length());
	val = tc::tohex(std::string(","));
	response.insert(val.data(), val.length());

	// instruction
	val = byte2string(action::Server::eUnlock);
	response.insert(val.data(), val.length());

	// end
	val = tc::tohex(std::string("#"));
	response.insert(val.data(), val.length());

	return RES_OK;
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