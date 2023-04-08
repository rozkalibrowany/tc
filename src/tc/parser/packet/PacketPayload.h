#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <tc/parser/records/avl/AVLRecord.h>
#include <tc/parser/packet/Packet.h>
#include <tc/parser/Reader.h>
#include <tc/common/Buf.h>

namespace tc::parser {

using namespace records;
using namespace avl;

class PacketPayload : public Packet, public JsonI
{
public:
	static size_t DATA_MIN_SIZE;
	static size_t IMEI_MIN_SIZE;

	static bool hasPayload(const uchar* buf, size_t size);
	static bool hasPayloadImei(const uchar* buf, size_t size);
	static bool contains(const uchar* buf, size_t size, uchar c);
	static int getIdx(const uchar* cbuf, size_t size, const uchar c);

	result_t parse(const uchar* cbuf, size_t size, size_t offset = 0) override;
	const size_t size() override;
	AVLRecords &records();

	Json::Value toJsonValue();

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

private:
	size_t iRecordsSize;
	AVLRecords iAVLRecords;
};

} // namespace tc::parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
