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

	Positioning(const common::Buf& buf) : RecordI(buf) {}

	result_t parse(uint8_t elements = 1) override;
	result_t response(common::Buf& response) override;

private:
	std::string iLatitude;
	std::string iLongitude;
	std::string iUtcTiming;
	std::string iUtcDate;
	uint8_t iSatellites {0};
	float iAccuracy {0.f};
	uint8_t iInstructionAcquisition{0};
	LocationStatus iLocationStatus{eInvalidLocation};
	uint8_t iAltitude{0};
	char iAltitudeUnit{'m'};
};

} // namespace tc::parser::omni::records

#endif /* C8D88FCE_CF62_456B_81E0_29060D8B13E4 */
