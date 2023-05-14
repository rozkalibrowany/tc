#include <tc/parser/Reader.h>
#include <array>
#include <algorithm>

namespace tc::parser {

Reader::Reader(const common::Buf &buf, int offset)
 : iBuf(buf)
 , iOffset(offset)
{
	// nothing to do
}

const common::Buf& Reader::buf() const
{
	return iBuf;
}

int Reader::offset() const
{
	return iOffset;
}

size_t Reader::size() const
{
	return iBuf.size();
}

uint Reader::readU(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
	common::Buf subBuf(common::Buf::ByteArray{iBuf.begin() + offs, iBuf.begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;
	std::reverse(subBuf.begin(), subBuf.end());

	if (bytes == 1)
		return subBuf[0] & 0xFF;

	if (bytes == 2)
		return subBuf.toUInt16(0);

	return 0;
}

long Reader::readL(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
	common::Buf subBuf(common::Buf::ByteArray{iBuf.begin() + offs, iBuf.begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;
	return std::stol (tc::uchar2string(subBuf.cdata(), 8), nullptr, 16);
}

int Reader::read(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
	common::Buf subBuf(common::Buf::ByteArray{iBuf.begin() + offs, iBuf.begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;
	std::reverse(subBuf.begin(), subBuf.end());

	if (bytes == 1)
		return (int) subBuf[0];

	if (bytes == 2)
		return (int) subBuf.toInt16(0);

	if (bytes == 4)
		return (int) subBuf.toInt32(0);

	if (bytes == 8)
		return static_cast<int>(subBuf.toInt64(0));

	return 0;
}

common::Buf Reader::readS(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
	common::Buf subBuf(common::Buf::ByteArray{iBuf.begin() + offs, iBuf.begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;

	return subBuf;
}

void Reader::skip(int bytes)
{
	iOffset += bytes;
}

void Reader::setOffset(int offset)
{
	iOffset = offset;
}

} // namespace tc::parser