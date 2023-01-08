#ifndef D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7
#define D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7

#include <tc/parser/packet/Packet.h>

namespace tc::parser::packet {

class Command : public Packet
{
public:
	static std::map <std::string, std::string> sMapping;

	Command() = default;
	Command(const std::string imei);

	static bool hasCommand(const uchar* cbuf, size_t size);

	result_t parse(uchar* cbuf, size_t size, size_t offset = 0) override;
	result_t create(const std::string &cmd, bool cr = false);

	size_t size() const override;
	virtual const uchar *command();

	virtual uchar *data();
	virtual const uchar *cdata() const;

protected:
	result_t getPayload(const std::string &cmd, parser::Buf &buf);

	Buf iBuf;
};

} // namespace tc::parser::packet


#endif /* D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7 */

