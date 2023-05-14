#include <tc/parser/omni/records/LockReport.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t LockReport::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto offset = buf.find_nth(',', 3); // data after third ','
	if (!offset)
		return RES_NOENT;

	Reader reader(buf);
	reader.setOffset(offset);

	auto year = hex2int(reader.readS(4).data());
	auto month = hex2int(reader.readS(2).data());
	auto hour = hex2int(reader.readS(2).data());
	auto day = hex2int(reader.readS(2).data());
	auto min = hex2int(reader.readS(2).data());
	auto sec = hex2int(reader.readS(2).data());

	iTimestamp.set(year, month, day, hour, min, sec);
	reader.skip(1);

	auto cycleTimeSize = buf.find_nth('#') - reader.offset();
	if (cycleTimeSize <= 0)
		return RES_NOENT;

	iCycleTime = std::chrono::minutes(reader.readU(cycleTimeSize));

	return RES_OK;
}

result_t LockReport::response(common::Buf& response)
{
	// re
	response.insert(c_response.data(), c_response.size());
	response.push_back(',');
	// instruction
	auto instruction = hexAsText(byte2string(action::Locker::eLockReport));
	response.insert(instruction.data(), instruction.size());
	// end
	response.push_back('#');
	// newline
	response.push_back('\n');

	return RES_OK;
}

std::chrono::minutes LockReport::cycle_time() const
{
	return iCycleTime;
}

result_t LockReport::toJsonImpl(Json::Value &rhs, bool root) const
{
	rhs["CycleTime"] = iCycleTime.count();

	return RES_OK;
}

result_t LockReport::fromJsonImpl(const Json::Value &rhs, bool root)
{
	if (rhs.isMember("CycleTime"))
		iCycleTime = std::chrono::minutes(rhs["CycleTime"].asUInt());

	return RES_OK;
}

} // namespace tc::parser::teltonika::records