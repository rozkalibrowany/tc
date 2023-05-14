#ifndef C8D88FCE_CF62_456B_81E0_29060D8B13E4
#define C8D88FCE_CF62_456B_81E0_29060D8B13E4

#include <tc/parser/omni/records/RecordI.h>

namespace tc::parser::omni::records {

class Positioning : public RecordI {
public:
	enum LocationStatus
	{
		eEffectiveLocation  = 0x41,
		eInvalidLocation    = 0x5641
	};

	Positioning(bool has_response) : RecordI(has_response) {}
	Positioning(const Positioning &rhs) = default;
	Positioning(Positioning &&rhs) = default;
	~Positioning() = default;

	result_t parse(const common::Buf &buf) override;
	result_t response(common::Buf &response) override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

private:
	result_t setLongitude(const std::string &longitude);
	result_t setLatitude(const std::string &latitude);

	std::string iLatitude;
	std::string iLongitude;
	std::string iUtcTiming;
	std::string iUtcDate;
	uint8_t iSatellites {0};
	float iAccuracy {0.f};
	uint8_t iInstructionAcquisition{0};
	uint8_t iAltitude{0};
	char iAltitudeUnit{'m'};
	LocationStatus iLocationStatus{eInvalidLocation};
};

} // namespace tc::parser::omni::records

#endif /* C8D88FCE_CF62_456B_81E0_29060D8B13E4 */
