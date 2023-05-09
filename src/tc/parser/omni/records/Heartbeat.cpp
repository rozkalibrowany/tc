#include <tc/parser/omni/records/Heartbeat.h>

namespace tc::parser::omni::records {

result_t Heartbeat::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto offset = buf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;
	Reader reader(buf);
	reader.setOffset(offset);

	auto locked = (int) reader.read(1);
	reader.skip(1);

	auto voltage = (int) reader.read(3);
	reader.skip(1);

	auto signal = (int8_t) reader.read(2);
	reader.skip(1);

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

result_t Heartbeat::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["Locked"] = iLocked;
	rhs["Voltage"] = iVoltage;
	rhs["Signal"] = iSignal;

	return RES_OK;
}

result_t Heartbeat::fromJsonImpl(const Json::Value &rhs, bool root)
{
	return RES_NOIMPL;
}

} // namespace tc::parser::omni::records