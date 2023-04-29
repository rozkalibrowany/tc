#ifndef C2D35EE8_585E_4CB9_840C_7BD165783793
#define C2D35EE8_585E_4CB9_840C_7BD165783793

#include <tc/parser/omni/records/RecordI.h>
#include <chrono>

namespace tc::parser::omni::records {

class Unlock : public RecordI {
public:
	Unlock(bool has_response) : RecordI(has_response) {}
	Unlock(const Unlock &rhs) = default;
	Unlock(Unlock &&rhs) = default;
	~Unlock() = default;

	result_t parse(const common::Buf &buf) override;
	result_t response(common::Buf& response) override;

	uint8_t result() const;
	int customerID() const;

private:
	int iCustomerID {0};
	uint8_t iResult {0}; // 0 success, 1 failure
};

} // namespace tc::parser::omni::records

#endif /* C2D35EE8_585E_4CB9_840C_7BD165783793 */
