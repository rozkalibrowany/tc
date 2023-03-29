#ifndef D426066F_F3EE_412F_864E_4D6726173ECE
#define D426066F_F3EE_412F_864E_4D6726173ECE

#include <tc/parser/records/RecordI.h>
#include <tc/common/SysMutex.h>
#include <mutex>
namespace tc::parser::records::gps {

class GPSRecord : public RecordI {
public:
	GPSRecord();
	GPSRecord(const GPSRecord& rhs) = default;
	GPSRecord(GPSRecord&& rhs) = default;

	~GPSRecord() = default;

	GPSRecord &operator=(const GPSRecord &rhs);

	result_t parse(Reader &reader) override;
	result_t parse(Reader &reader, int codec) override;

	bool empty() const override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
	
	std::optional<int> iLongitude;
	std::optional<int> iLatitude;
	std::optional<int> iAltitude;
	std::optional<int> iAngle;
	std::optional<int> iSatellites;
	std::optional<int> iSpeed;
};

} // namespace tc::parser::records::avl


#endif /* D426066F_F3EE_412F_864E_4D6726173ECE */
