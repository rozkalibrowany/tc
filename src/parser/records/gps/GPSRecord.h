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


class GPSRecord : public RecordI< GPSRecord > {
public:

	GPSRecord(const GPSData &data);
	GPSRecord();

	virtual ~GPSRecord() = default;

	GPSRecord &operator=(const GPSRecord &rhs);

  result_t parse(const reader::ReaderSPtr &reader) override;
	result_t parse(const reader::ReaderSPtr &reader, int codec) override;

	uint8_t *data();
	const uint8_t *cdata() const;
	std::string toString();

private:
	GPSData iData;
};

} // namespace parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
