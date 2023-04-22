#include <tc/parser/omni/records/Heartbeat.h>

namespace tc::parser::omni::records {

result_t Heartbeat::parse(uint8_t elements)
{
	if (RecordI::parse() != RES_OK)
		return RES_NOENT;

	auto offset = iBuf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;
	setOffset(offset);

	auto locked = (int) read(1);
	skip(1);

	auto voltage = (int) read(3);
	skip(1);

	auto signal = (int8_t) read(2);
	skip(1);

	iLocked = locked;
	iVoltage = std::ceil(voltage * 100) / 100;
	iSignal = signal;

	return RES_OK;
}

result_t Heartbeat::response(common::Buf& response)
{
	return RES_NOIMPL;
}

bool Heartbeat::locked() const
{
	return iLocked;
}

float Heartbeat::voltage() const
{
	return iVoltage;
}

uint8_t Heartbeat::signal() const
{
	return iSignal;
}

} // namespace tc::parser::omni::records