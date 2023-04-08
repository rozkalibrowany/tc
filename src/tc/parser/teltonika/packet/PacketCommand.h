#ifndef D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7
#define D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7

#include <tc/parser/Packet.h>

namespace tc::parser::teltonika {

class PacketCommand : public Packet
{
public:

	PacketCommand(const std::string imei = "");

	~PacketCommand() = default;
  static bool hasCommand(const uchar* cbuf, size_t size);

	result_t parse(const uchar* cbuf, size_t size, size_t offset = 0) override;

	const size_t size() override;
	const uchar* command();

private:
	uchar* iCommand;
	size_t iSize;
};

} // namespace tc::parser::teltonika


#endif /* D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7 */
