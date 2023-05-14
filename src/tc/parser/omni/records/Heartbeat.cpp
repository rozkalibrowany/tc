#include <tc/parser/omni/records/Heartbeat.h>
#include <cstdlib>

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

	auto voltage = std::string((const char*)reader.readS(3).data(), 3);
	reader.skip(1);

	auto signal = atoi((const char*) reader.readS(3).data());
	reader.skip(1);

	iLocked = locked;
	iVoltage = (std::stoi(voltage) * 1.0 / 100.0);
	iSignal = signal;

	return RES_OK;
}

result_t Heartbeat::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["Locked"] = iLocked;
	rhs["Voltage"] = fmt::format("{:.2f}", iVoltage);
	rhs["Signal"] = iSignal;

	return RES_OK;
}

result_t Heartbeat::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.isMember("Locked"))
		iLocked = rhs["Locked"].asBool();

	if (rhs.isMember("Voltage"))
		iVoltage = rhs["Voltage"].asFloat();

	if (rhs.isMember("Signal"))
		iSignal = rhs["Signal"].asUInt();

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