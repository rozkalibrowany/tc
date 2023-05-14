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

	auto voltage = std::string((const char*)reader.readS(3).data(), 3);
	iVoltage = (std::stoi(voltage) * 1.0 / 100.0);

	return RES_OK;
}

result_t CheckIn::response(common::Buf& response)
{
	return RES_NOENT;
}

float CheckIn::voltage() const
{
	return iVoltage;
}

result_t CheckIn::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["Voltage"] = fmt::format("{:.2f}", iVoltage);

	return RES_OK;
}

result_t CheckIn::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.isMember("Voltage"))
		iVoltage = rhs["Voltage"].asFloat();

	return RES_OK;
}

} // namespace tc::parser::omni::records