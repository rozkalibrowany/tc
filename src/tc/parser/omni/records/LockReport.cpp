#include <tc/parser/omni/records/LockReport.h>

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

	auto year = hex2int(reader.readS(4));
	auto month = hex2int(reader.readS(2));
	auto hour = hex2int(reader.readS(2));
	auto day = hex2int(reader.readS(2));
	auto min = hex2int(reader.readS(2));
	auto sec = hex2int(reader.readS(2));

	iTimestamp.set(year, month, day, hour, min, sec);
	reader.skip(1);

	auto cycleTimeSize = buf.find_nth('#') - reader.offset();
	if (cycleTimeSize <= 0)
		return RES_NOENT;

	iCycleTime = std::chrono::minutes(reader.readU(cycleTimeSize));

	return RES_OK;
}

std::chrono::minutes LockReport::cycle_time() const
{
	return iCycleTime;
}

} // namespace tc::parser::teltonika::records