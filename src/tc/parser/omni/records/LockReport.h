#ifndef BA0C59C4_658A_4F5D_A428_D99812D6514B
#define BA0C59C4_658A_4F5D_A428_D99812D6514B

#include <tc/parser/omni/records/RecordI.h>
#include <chrono>

namespace tc::parser::omni::records {

class LockReport : public RecordI {
public:
	LockReport(bool has_response) : RecordI(has_response) {}
	LockReport(const LockReport &rhs) = default;
	LockReport(LockReport &&rhs) = default;
	~LockReport() = default;

	result_t parse(const common::Buf &buf) override;
	result_t response(common::Buf &response) override{};
	
	std::chrono::minutes cycle_time() const;

private:
	std::chrono::minutes iCycleTime;
};

} // namespace tc::parser::teltonika::records

#endif /* BA0C59C4_658A_4F5D_A428_D99812D6514B */
