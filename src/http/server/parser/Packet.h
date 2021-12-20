#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <iostream>
#include <vector>
#include <reader/Buf.h>
#include <common/Result.h>
#include <records/avl/AVLPacket.h>
#include <reader/Reader.h>

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
