#include <tc/parser/omni/records/Positioning.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t Positioning::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto offset = buf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	Reader reader(buf);

	reader.setOffset(offset);

	auto instruction = reader.readU(1);
	reader.skip(1);

	auto utcDate = std::string((const char *)reader.readS(9));
	reader.skip(1);

	auto locationStatus = (LocationStatus)reader.readU(1);
	reader.skip(1);

	auto latitude = std::string((const char*) reader.readS(9));
	reader.skip(2);

	auto longitude = std::string((const char*) reader.readS(9));
	reader.skip(2);

	auto satellites = std::ceil(reader.readU(3) * 100) / 100;
	reader.skip(1);

	auto utcTime = std::string((const char *)reader.readS(6));
	reader.skip(1);

	auto altitude = reader.readU(2);
	reader.skip(1);

	auto altitudeUnit = reader.read(1);
	reader.skip(1);

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
	// re
	response.insert(c_response.data(), c_response.size());
	response.push_back(',');
	// instruction
	auto instruction = hexAsText(byte2string(action::Server::eAcquisitionPositioning));
	response.insert(instruction.data(), instruction.size());
	// end
	response.push_back('#');

	return RES_OK;
}

} // namespace tc::parser::omni::records