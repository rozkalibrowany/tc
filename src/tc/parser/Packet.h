#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <vector>
#include <tc/parser/reader/Buf.h>
#include <tc/common/Result.h>
#include <tc/parser/records/avl/AVLPacket.h>
#include <tc/parser/reader/Reader.h>

namespace tc::parser {

using namespace reader;
using namespace records;
using namespace avl;

class Packet
{
public:

	static int PACKET_INIT_MAX_SIZE;
	struct imei_t {
		size_t iLength {0};
		unsigned char* iData {nullptr};
	};

	result_t parse(Buf &buf);
	result_t parse(unsigned char* cbuf, size_t size);

	int codec() const;
	int size() const;

	ReaderSPtr &reader();
	const ReaderSPtr &creader() const;
	AVLRecordsUPtr records();

protected:
	result_t parseImei(Buf &buf);
	bool contains(const Buf &buf, const unsigned char c) const;
	int getIdx(const Buf &buf, const unsigned char c);

private:
	imei_t iImei;
	int iCodec{0};
	int iRecordsTotal {0};
	ReaderSPtr iReader {nullptr};
	AVLRecordsUPtr iAVLRecordsUPtr {nullptr};
};

} // namespace tc::parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
