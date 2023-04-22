#ifndef F46B5D06_B73A_4A06_9329_580835BE7C56
#define F46B5D06_B73A_4A06_9329_580835BE7C56

#include <tc/common/Buf.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/omni/Datetime.h>

namespace tc::parser::omni {

class Command : public tc::LogI
{
public:
	static constexpr std::string_view c_header = "*CMDS";
	static constexpr std::string_view c_manufacturer = "OM";

	static result_t getHeader(common::Buf &buf, const Imei &imei, const Datetime &datetime);
	static std::string_view datetime();

	Command(const Imei& imei, uint userID = 0);
	~Command() = default;

	result_t init(action::Server::Instruction instruction, int value);
	
	std::string_view asHex() const;
	uchar *asBin() const;

private:
	common::Buf iBuf;
	Imei iImei;
	uint iUserID;
	Datetime iDateTime{true};
};

} // namespace tc::parser::omni

#endif /* F46B5D06_B73A_4A06_9329_580835BE7C56 */
