#ifndef F03787EA_5A84_4C2C_B054_D3E514170612
#define F03787EA_5A84_4C2C_B054_D3E514170612

#include <tc/parser/omni/records/RecordI.h>

namespace tc::parser::omni::records {

class CheckIn : public RecordI {
public:

	CheckIn(bool has_response) : RecordI(has_response) {}
	CheckIn(const CheckIn &rhs) = default;
	CheckIn(CheckIn &&rhs) = default;
	~CheckIn() = default;

	result_t parse(const common::Buf &buf) override;
	result_t response(common::Buf& response) override;

	float voltage() const;

private:
	float iVoltage{0.f};
};

} // namespace tc::parser::omni::records

#endif /* F03787EA_5A84_4C2C_B054_D3E514170612 */
