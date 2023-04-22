#include <tc/parser/omni/records/LockReport.h>

namespace tc::parser::omni::records {

result_t LockReport::parse(uint8_t elements)
{
	auto offset = iBuf.find_nth(',', 3); // data after third ','
	if (!offset)
		return RES_NOENT;

	setOffset(offset);

	auto year = hex2int(readS(4));
	auto month = hex2int(readS(2));
	auto hour = hex2int(readS(2));
	auto day = hex2int(readS(2));
	auto min = hex2int(readS(2));
	auto sec = hex2int(readS(2));

	iTimestamp.set(year, month, day, hour, min, sec);
	skip(1);

	auto cycleTimeSize = iBuf.find_nth('#') - iOffset;
	if (cycleTimeSize <= 0)
		return RES_NOENT;

	iCycleTime = std::chrono::minutes(readU(cycleTimeSize));

	return RES_OK;
}

SysTime LockReport::timestamp() const
{
	return iTimestamp;
}

std::chrono::minutes LockReport::cycle_time() const
{
	return iCycleTime;
}

} // namespace tc::parser::teltonika::records