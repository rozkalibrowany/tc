#ifndef D426066F_F3EE_412F_864E_4D6726173ECE
#define D426066F_F3EE_412F_864E_4D6726173ECE

#include <records/RecordI.h>
#include <chrono>

namespace parser::records::gps {


struct GPSData {
	GPSData &operator=(const GPSData &rhs) {
		iLongitude = rhs.iLongitude;
		iLatitude = rhs.iLatitude;
		iAltitude = rhs.iAltitude;
		iAngle = rhs.iAngle;
		iSatellites = rhs.iSatellites;
		iSpeed = rhs.iSpeed;
		return *this;
	}

	uint32_t iLongitude {0U};
	uint32_t iLatitude {0U};
	int iAltitude {0};
	int iAngle {0};
	int iSatellites {0};
	int iSpeed {0};
};


class GPSRecord : public RecordI {
public:

	GPSRecord(const GPSData &data) : iData(data) {}
	GPSRecord() : GPSRecord(GPSData{}) {}

	GPSRecord &operator=(const GPSRecord &rhs) {
		iData = rhs.iData;
		return *this;
	}

	virtual ~GPSRecord() = default;

	//virtual result_t getParser(std::shared_ptr<GPSRecord> &parser);

	std::string toString() override {
	/*	return std::format(R"(
			********** Record GPS **********
			longitude: {}
			latitude: {}
			altitude: {}
			angle: {}
			satelites: {}
			km/h: {}
		)", std::to_string(iData.iLongitude), std::to_string(iData.iLatitude), std::to_string(iData.iAltitude),
				std::to_string(iData.iAngle), std::to_string(iData.iSatellites), std::to_string(iData.iSpeed)); */
		return std::string();
	}
	GPSData iData;
};


} // namespace parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
