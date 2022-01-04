#ifndef D426066F_F3EE_412F_864E_4D6726173ECE
#define D426066F_F3EE_412F_864E_4D6726173ECE

#include <records/RecordI.h>
#include <chrono>

namespace parser::records::gps {


struct GPSData {
	GPSData &operator=(const GPSData &rhs) = default;

	uint32_t iLongitude {0U};
	uint32_t iLatitude {0U};
	int iAltitude {0};
	int iAngle {0};
	int iSatellites {0};
	int iSpeed {0};
};


class GPSRecord : public RecordI {
public:

	GPSRecord(const GPSData &data);
	GPSRecord();

	virtual ~GPSRecord() = default;

	GPSRecord &operator=(const GPSRecord &rhs) = default;

	GPSData &data();
	const GPSData &cdata() const;

  result_t parse(const reader::ReaderSPtr &reader) override;
	result_t parse(const reader::ReaderSPtr &reader, int codec) override;

	std::string toString() override;

private:
	GPSData iData;
};

} // namespace parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
