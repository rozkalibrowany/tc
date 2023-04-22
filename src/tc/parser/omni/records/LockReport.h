#ifndef BA0C59C4_658A_4F5D_A428_D99812D6514B
#define BA0C59C4_658A_4F5D_A428_D99812D6514B

#include <tc/parser/omni/records/RecordI.h>
#include <chrono>

namespace tc::parser::omni::records {

class LockReport : public RecordI {
public:
	result_t parse(uint8_t elements = 3) override;

	SysTime timestamp() const;
	std::chrono::minutes cycle_time() const;

private:
	SysTime iTimestamp{true};
	std::chrono::minutes iCycleTime;
};

} // namespace tc::parser::teltonika::records

#endif /* BA0C59C4_658A_4F5D_A428_D99812D6514B */
