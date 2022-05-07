#ifndef D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7
#define D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7

#include <tc/common/Common.h>
#include <tc/parser/packet/Packet.h>

namespace tc::parser {

class PacketCommand : public Packet
{
public:

	static size_t DATA_SIZE;
	static size_t IMEI_MIN_SIZE;

	PacketCommand(const std::string imei = "");

	virtual ~PacketCommand() = default;
  static bool hasCommand(const uchar* cbuf, size_t size);

	result_t parse(const uchar* cbuf, size_t size) override;
	result_t parseImei(const uchar* cbuf, size_t size) override;

	const std::string imei() const override;
	const size_t size() override;
	const uchar* command();

private:
	uchar* iCommand;
	size_t iSize;
};

} // namespace tc::parser


#endif /* D43AB83E_8F58_407E_A5C3_3BD76E9F3EF7 */
