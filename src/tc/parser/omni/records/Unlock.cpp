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

std::string hexify(unsigned int n)
{
  std::string res;

  do
  {
    res += "0123456789ABCDEF"[n % 16];
    n >>= 4;
  } while(n);

  return std::string(res.rbegin(), res.rend());
}

result_t Unlock::response(common::Buf& response)
{
	// re
	response.insert(c_response.data(), c_response.size());
	response.push_back(',');
	// instruction
	auto instruction = hexAsText(byte2string(action::Server::eUnlock));
	response.insert(instruction.data(), instruction.size());
	// end
	response.push_back('#');

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