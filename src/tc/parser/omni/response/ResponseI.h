#ifndef AF72C035_7428_4AA2_B682_806207F23C4A
#define AF72C035_7428_4AA2_B682_806207F23C4A

#include <tc/common/Buf.h>
#include <tc/parser/Reader.h>

namespace tc::parser::omni::records {

class ResponseI : public Reader, public tc::LogI {
public:
	static constexpr std::string_view c_response = "Re";

	ResponseI(common::Buf &buf) : Reader(buf), tc::LogI("console") {}
	ResponseI(const ResponseI &rhs) = default;
	ResponseI(ResponseI &&rhs) = default;
	virtual ~ResponseI() = default;

	virtual result_t parse(uint8_t elements) = 0;
	//virtual std::string_view response() const = 0;
};

} // namespace tc::parser::omni::records

#endif /* AF72C035_7428_4AA2_B682_806207F23C4A */
