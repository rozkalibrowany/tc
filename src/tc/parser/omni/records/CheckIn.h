#ifndef F03787EA_5A84_4C2C_B054_D3E514170612
#define F03787EA_5A84_4C2C_B054_D3E514170612

#include <tc/parser/omni/records/RecordI.h>

namespace tc::parser::omni::records {

class CheckIn : public RecordI {
public:
	CheckIn(const common::Buf& buf) : RecordI(buf) {}

	result_t parse(uint8_t elements = 1) override;
	result_t response(common::Buf& response) override;

	float voltage() const;

private:
	float iVoltage{0.f};
};

} // namespace tc::parser::omni::records

#endif /* F03787EA_5A84_4C2C_B054_D3E514170612 */
