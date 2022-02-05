#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <tc/parser/records/avl/AVLRecord.h>
#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>
#include <tc/parser/Buf.h>
#include <tc/common/Common.h>

namespace tc::parser {

using namespace records;
using namespace avl;

class PacketPayload : public Packet
{
public:
	static size_t DATA_MIN_SIZE;
	static size_t IMEI_MIN_SIZE;

	PacketPayload(bool time_now = true);
	PacketPayload(const ID &id);

	virtual ~PacketPayload() = default;

	static bool hasPayload(const uchar* buf, size_t size);
	static bool hasPayloadImei(const uchar* buf, size_t size);
	static bool contains(const uchar* cbuf, size_t size, uchar c);
	static int getIdx(uchar* cbuf, size_t size, const uchar c);

	result_t parse(uchar* cbuf, size_t size) override;
	result_t parseImei(uchar* cbuf, size_t size);

	std::shared_ptr< Reader > &reader();

	std::string imei() const;
	AVLRecords &records();

private:
	std::shared_ptr< Reader > iReader;
	std::string iImei;
	AVLRecords iAVLRecords;
};

} // namespace tc::parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
