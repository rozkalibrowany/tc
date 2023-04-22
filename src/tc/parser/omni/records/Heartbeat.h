#ifndef BEF1F63B_F323_4957_838D_AD4657A213DF
#define BEF1F63B_F323_4957_838D_AD4657A213DF

#include <tc/parser/omni/records/RecordI.h>

namespace tc::parser::omni::records {

class Heartbeat : public RecordI {
public:
	Heartbeat(const common::Buf& buf) : RecordI(buf) {}

	result_t parse(uint8_t elements = 1) override;
	result_t response(common::Buf& response) override;

	bool locked() const;
	float voltage() const;
	uint8_t signal() const;

private:
	bool iLocked;
	float iVoltage{0.f};
	uint8_t iSignal{0};
};

} // namespace tc::parser::omni::records

#endif /* BEF1F63B_F323_4957_838D_AD4657A213DF */
