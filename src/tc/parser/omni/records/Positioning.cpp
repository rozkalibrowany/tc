#include <tc/parser/omni/records/Positioning.h>
#include <tc/parser/omni/Command.h>

namespace tc::parser::omni::records {

result_t Positioning::parse(const common::Buf &buf)
{
	if (buf.empty())
		return RES_NOENT;

	auto valid_mode_offset = buf.find_nth('#');
	LG_ERR(this->logger(), "Invalid positioning: {}", static_cast<char>(buf[valid_mode_offset - 1]));
	if (static_cast<char>(buf[valid_mode_offset - 1]) == (char)'N') {
		LG_ERR(this->logger(), "Invalid positioning data");
		return RES_NOENT;
	}

	auto offset = buf.find_nth(',', 5); // data after fifth ','
	if (!offset)
		return RES_NOENT;

	Reader reader(buf);
	reader.setOffset(offset);

	auto instruction = reader.readU(1);
	reader.skip(1);

	auto utcDate = std::string((const char *)reader.readS(9).data());
	reader.skip(1);

	auto locationStatus = (LocationStatus)reader.readU(1);
	reader.skip(1);

	auto latitude = std::string((const char*) reader.readS(9).data());
	reader.skip(2);

	auto longitude = std::string((const char*) reader.readS(9).data());
	reader.skip(2);

	auto satellites = std::ceil(reader.readU(3) * 100) / 100;
	reader.skip(1);

	auto utcTime = std::string((const char *)reader.readS(6).data());
	reader.skip(1);

	auto altitude = reader.readU(2);
	reader.skip(1);

	auto altitudeUnit = reader.read(1);
	reader.skip(1);

	iInstructionAcquisition = instruction;
	iUtcDate = utcDate;
	iLocationStatus = locationStatus;
	iLatitude = setLatitude(latitude);
	iLongitude = setLongitude(latitude);
	LG_NFO(this->logger(), "Latitude: {} Longitude: {}", iLatitude, iLongitude);
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
	// newline
	response.push_back('\n');

	return RES_OK;
}

result_t Positioning::toJsonImpl(Json::Value &rhs, bool root) const
{
	Json::Value gps;
	gps["Latitude"] = iLatitude;
	gps["Longitude"] = iLongitude;
	gps["Altitude"] = iAltitude;
	gps["Satellites"] = iSatellites;
	gps["Utc"] = iUtcDate;

	rhs["GPS"] = gps;
	return RES_OK;
}

result_t Positioning::fromJsonImpl(const Json::Value &rhs, bool root)
{
	const auto& gps = rhs["GPS"];
	if (gps.size() == 0) {
		LG_WRN(this->logger(), "GPS record object empty");
		return RES_INVARG;
	}

	if (gps.isMember("Longitude"))
		iLongitude = gps["Longitude"].asString();

	if (gps.isMember("Latitude"))
		iLatitude = gps["Latitude"].asString();
	
	if (gps.isMember("Altitude"))
		iAltitude = gps["Altitude"].asUInt();
	
	if (gps.isMember("Satellites"))
		iSatellites = gps["Satellites"].asUInt();
	
	if (gps.isMember("Utc"))
		iUtcDate = gps["Utc"].asString();

	return RES_OK;

}

result_t Positioning::setLongitude(const std::string &longitude)
{
	if (longitude.find('.') == std::string::npos || longitude.size() < 9)
		return RES_NOENT;

	// Longitude conversion algorithm: lng=ddd+mm.mmmm/60, source: Omni protocol
	auto ddd = std::stoi(longitude.substr(0, 2));
	auto mm = std::stof(longitude.substr(3, longitude.size() - 1));
	iLongitude = std::to_string((float) ddd + mm / 60.0);

	return RES_OK;
}

result_t Positioning::setLatitude(const std::string &latitude)
{
	if (latitude.find('.') == std::string::npos || latitude.size() < 9)
		return RES_NOENT;

	// Latitude conversion algorithm: the lat=dd+mm.mmm/60, source: Omni protocol
	auto dd = std::stoi(latitude.substr(0, 1));
	auto mm = std::stof(latitude.substr(2, latitude.size() - 1));
	iLatitude = std::to_string((float) dd + mm / 60.0);

	return RES_OK;
}

} // namespace tc::parser::omni::records