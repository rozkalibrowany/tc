#ifndef F46B5D06_B73A_4A06_9329_580835BE7C56
#define F46B5D06_B73A_4A06_9329_580835BE7C56

#include <tc/common/Buf.h>
#include <tc/parser/omni/Action.h>
#include <tc/parser/CommandI.h>

namespace tc::parser::omni {

using namespace action;

class Command : public CommandI
{
public:
	static std::map <std::string, std::pair<Server::Event, int>> sMapping;

	static constexpr std::string_view c_header = "*CMDS";
	static constexpr std::string_view c_manufacturer = "OM";

	static result_t getHeader(common::Buf &buf, const Imei &imei, const std::string &datetime);

	Command(const Imei& imei);
	Command(const uchar* cbuf, size_t size);
	Command(const std::string& cmd, const Imei& imei, uint userID = 0);
	~Command() = default;

	Command &operator=(const Command &rhs);
	
	result_t create(const common::Buf &buf, const Imei &imei, const std::string &datetime);
	result_t parse(const uchar* cbuf, size_t size) override;

	std::string asHex() const;
	common::Buf& buf();

protected:
	void create(const std::string& cmd, const Imei &imei, Protocol::Type type = Protocol::eOmni) override;

	uint iUserID {0};
	SysTime iDatetime {true};
};

inline std::string Command::asHex() const
{
	return tc::bytes2hex((const uint8_t *)iBuf.cdata(), iBuf.size());
}

} // namespace tc::parser::omni

#endif /* F46B5D06_B73A_4A06_9329_580835BE7C56 */
