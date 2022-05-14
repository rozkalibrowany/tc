#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <tc/parser/records/avl/AVLRecord.h>
#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>
#include <tc/parser/Buf.h>

namespace tc::parser {

using namespace records;
using namespace avl;

class PacketPayload : public Packet
{
public:
	static size_t DATA_MIN_SIZE;
	static size_t IMEI_MIN_SIZE;

	static bool hasPayload(const uchar* buf, size_t size);
	static bool hasPayloadImei(const uchar* buf, size_t size);
	static bool contains(const uchar* buf, size_t size, uchar c);
	static int getIdx(const uchar* cbuf, size_t size, const uchar c);

	result_t parse(uchar* cbuf, size_t size) override;

	const std::string imei() const override;
	const size_t size() override;
	std::shared_ptr< Reader > reader();

	AVLRecords &records();

private:
	std::shared_ptr< Reader > iReader;
	size_t iRecords;
	AVLRecords iAVLRecords;
};

} // namespace tc::parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
