#ifndef AF7F8597_2B7B_448F_A38D_4054C93EFC41
#define AF7F8597_2B7B_448F_A38D_4054C93EFC41

#include <tc/parser/Reader.h>
#include <tc/common/Common.h>
#include <tc/parser/Json.h>

namespace tc::parser::omni::records {

class RecordI : public tc::LogI, public JsonI {
public:
	static constexpr std::string_view c_response = "Re";

	RecordI() = default;
	RecordI(bool mandatory_ack) : tc::LogI("console"), iMandatoryAck(mandatory_ack) {}
	RecordI(const RecordI &rhs) = default;
	RecordI(RecordI &&rhs) = default;
	virtual ~RecordI() = default;

	virtual result_t response(common::Buf &response) { return RES_NOIMPL; };
	virtual result_t parse(const common::Buf &buf) { return RES_NOIMPL; };

	virtual bool mandatory_ack() const {
		return iMandatoryAck;
	}

protected:
	SysTime iTimestamp{true};
	bool iMandatoryAck{false};
};

} // namespace tc::parser::omni::records

#endif /* AF7F8597_2B7B_448F_A38D_4054C93EFC41 */
