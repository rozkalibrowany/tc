#include <tc/parser/omni/records/Positioning.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t Positioning::parse(uint8_t elements)
{
	if (RecordI::parse() != RES_OK)
		return RES_NOENT;

	auto offset = iBuf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	setOffset(offset);

	auto instruction = readU(1);
	skip(1);

	auto utcDate = std::string((const char *)readS(9));
	skip(1);

	auto locationStatus = (LocationStatus)readU(1);
	skip(1);

	auto latitude = std::string((const char*) readS(9));
	skip(2);

	auto longitude = std::string((const char*) readS(9));
	skip(2);

	auto satellites = std::ceil(readU(3) * 100) / 100;
	skip(1);

	auto utcTime = std::string((const char *)readS(6));
	skip(1);

	auto altitude = readU(2);
	skip(1);

	auto altitudeUnit = read(1);
	skip(1);

	iInstructionAcquisition = instruction;
	iUtcDate = utcDate;
	iLocationStatus = locationStatus;
	iLatitude = latitude;
	iLongitude = longitude;
	iSatellites = satellites;
	iUtcTiming = utcTime;
	iAltitude = altitude;
	iAltitudeUnit = altitudeUnit;

	return RES_OK;
}

result_t Positioning::response(common::Buf& response)
{
	if (Command::getHeader(response, iImei, iDateTime) != RES_OK)
		return RES_NOENT;

	// re
	auto val = tc::tohex(c_response);
	response.insert(val.data(), val.length());
	val = byte2string(0x2c);
	response.insert(val.data(), val.length());

	// instruction
	val = byte2string(action::Server::eAcquisitionPositioning);
	response.insert(val.data(), val.length());

	// end
	val = byte2string(0x23);
	response.insert(val.data(), val.length());

	return RES_OK;
}

} // namespace tc::parser::omni::records