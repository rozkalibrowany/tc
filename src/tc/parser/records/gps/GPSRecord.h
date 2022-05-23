#ifndef D426066F_F3EE_412F_864E_4D6726173ECE
#define D426066F_F3EE_412F_864E_4D6726173ECE

#include <tc/parser/records/RecordI.h>

namespace tc::parser::records::gps {

class GPSRecord : public RecordI {
public:

	struct Data {
		Data &operator=(const Data &rhs) = default;

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

	GPSRecord(const Data &data);
	GPSRecord();
	virtual ~GPSRecord() = default;

	GPSRecord &operator=(const GPSRecord &rhs) = default;

	void clear() override;
	bool empty() const override;
	Data &data();
	const Data &cdata() const;

  result_t parse(const std::shared_ptr< Reader > &reader) override;
	result_t parse(const std::shared_ptr< Reader > &reader, int codec) override;

	std::string toString() override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;

private:
	Data iData;
};

} // namespace tc::parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
