#ifndef D426066F_F3EE_412F_864E_4D6726173ECE
#define D426066F_F3EE_412F_864E_4D6726173ECE

#include <tc/parser/records/RecordI.h>

namespace tc::parser::records::gps {


struct GPSData {
	GPSData &operator=(const GPSData &rhs) = default;
	bool empty() const {
		return !iLongitude && !iLatitude && !iAltitude && !iAngle && !iSatellites;
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

	GPSRecord(const GPSData &data);
	GPSRecord();
	virtual ~GPSRecord() = default;

	GPSRecord &operator=(const GPSRecord &rhs) = default;

	void clear() override;
	bool empty() const override;
	GPSData &data();
	const GPSData &cdata() const;

  result_t parse(const std::shared_ptr< Reader > &reader) override;
	result_t parse(const std::shared_ptr< Reader > &reader, int codec) override;

	std::string toString() override;

private:
	GPSData iData;
};

} // namespace tc::parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
