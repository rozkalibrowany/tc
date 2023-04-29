#ifndef F46B5D06_B73A_4A06_9329_580835BE7C56
#define F46B5D06_B73A_4A06_9329_580835BE7C56

#include <tc/common/Buf.h>
#include <tc/parser/omni/Action.h>

namespace tc::parser::omni {

class Command : public tc::LogI
{
public:
	static constexpr std::string_view c_header = "*CMDS";
	static constexpr std::string_view c_manufacturer = "OM";

	static result_t getHeader(common::Buf &buf, const Imei &imei, const std::string &datetime);
	static std::string_view datetime();

	Command(const Imei& imei, uint userID = 0);
	~Command() = default;

	Command &operator=(const Command &rhs);

	result_t init(action::Server::Event event, int value);
	result_t init(const common::Buf &buf, const Imei &imei, const std::string &datetime);

	std::string asHex() const;
	common::Buf& buf();

private:
	common::Buf iBuf;
	Imei iImei;
	uint iUserID;
	std::string iDatetime;
};

inline std::string Command::asHex() const
{
	return tc::bytes2hex((const uint8_t *)iBuf.cdata(), iBuf.size());
}

} // namespace tc::parser::omni

#endif /* F46B5D06_B73A_4A06_9329_580835BE7C56 */
