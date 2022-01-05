#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <iostream>
#include <vector>
#include <tc/parser/reader/Buf.h>
#include <tc/parser/common/Result.h>
#include <tc/parser/records/avl/AVLPacket.h>
#include <tc/parser/reader/Reader.h>

namespace parser {

using namespace reader;
using namespace records;

class Packet {
public:

	result_t parse(Buf &buf);
	result_t parse(unsigned char* cbuf, size_t size);

	Reader &reader();
	const Reader &reader() const;

protected:
	bool contains(const Buf &buf, const unsigned char c) const;
	int getIdx(const Buf &buf, const unsigned char c);

	avl::PacketUPtr iData;
};

} // namespace parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
