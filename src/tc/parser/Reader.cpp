#include <tc/parser/Reader.h>
#include <iostream>
#include <array>
#include <algorithm>

namespace tc::parser {

Reader::Reader(std::shared_ptr<Buf> buf, int offset)
 : iBuf(std::move(buf))
 , iOffset(offset)
{
  // nothing to do
}

std::shared_ptr<Buf> Reader::buf()
{
	return iBuf;
}

int Reader::offset() const
{
	return iOffset;
}

uint Reader::readU(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
  Buf subBuf(Buf::ByteArray{iBuf->begin() + offs, iBuf->begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;
  std::reverse(subBuf.begin(), subBuf.end());

  if (bytes == 1)
    return subBuf[0] & 0xFF;

  if (bytes == 2)
    return subBuf.toUInt16(0);

  return 0;
}

int64_t Reader::readL(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
  Buf subBuf(Buf::ByteArray{iBuf->begin() + offs, iBuf->begin() + bytes + offs});
	if (offset == 0)
		iOffset += bytes;
  std::reverse(subBuf.begin(), subBuf.end());

  if (bytes == 1)
		return subBuf[0] & 0xFF;

  if (bytes == 2)
		return subBuf.toUInt16(0);

  return 0;
}

int Reader::read(int bytes, int offset)
{
	auto offs = offset == 0 ? iOffset : offset;
	Buf subBuf(Buf::ByteArray{iBuf->begin() + offs, iBuf->begin() + bytes + offs});
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

void Reader::skip(int bytes)
{
  iOffset += bytes;
}


} // namespace tc::parser