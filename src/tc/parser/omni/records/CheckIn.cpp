#include <tc/parser/omni/records/CheckIn.h>

namespace tc::parser::omni::records {

result_t CheckIn::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto offset = buf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	Reader reader(buf);
	reader.setOffset(offset);

	auto voltage = (int) reader.read(3);
	iVoltage = std::ceil(voltage * 100) / 100;

	return RES_OK;
}

result_t CheckIn::response(common::Buf& response)
{
	return RES_NOIMPL;
}

float CheckIn::voltage() const
{
	return iVoltage;
}

} // namespace tc::parser::omni::records