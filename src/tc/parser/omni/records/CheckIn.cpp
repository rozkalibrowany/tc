#include <tc/parser/omni/records/CheckIn.h>

namespace tc::parser::omni::records {

result_t CheckIn::parse(uint8_t elements)
{
	if (RecordI::parse() != RES_OK)
		return RES_NOENT;

	auto offset = iBuf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;
	setOffset(offset);

	auto voltage = (int) read(3);
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