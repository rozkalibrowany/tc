#include <reader/Reader.h>
#include <common/Result.h>
#include <array>
#include <algorithm>

namespace parser::reader {

Reader::Reader(Buf &buf, int offset)
 : iBuf(buf)
 , iOffset(offset)
{
  // nothing to do
}

Reader::Reader(Buf &buf)
 : Reader(buf, 0)
{
	// nothing to do
}

Reader &Reader::operator=(const Reader &reader)
{
	iBuf = reader.iBuf;
  iOffset = reader.iOffset;
	return *this;
}

uint Reader::readU(int bytes)
{
  Buf subBuf(Buf::ByteArray{iBuf.begin() + iOffset, iBuf.begin() + bytes + iOffset});
  iOffset += bytes;
  std::reverse(subBuf.begin(), subBuf.end());

  if (bytes == 1)
    return subBuf[0] & 0xFF;

  if (bytes == 2)
    return subBuf.toUInt16(0);

  return 0;
}

int64_t Reader::readL(int bytes)
{
  Buf subBuf(Buf::ByteArray{iBuf.begin() + iOffset, iBuf.begin() + bytes + iOffset});
  iOffset += bytes;
  std::reverse(subBuf.begin(), subBuf.end());

  if (bytes == 1)
      return subBuf[0] & 0xFF;

  if (bytes == 2)
    return subBuf.toUInt16(0);

  return 0;
}

int Reader::read(int bytes)
{
  Buf subBuf(Buf::ByteArray{iBuf.begin() + iOffset, iBuf.begin() + bytes + iOffset});
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

Buf Reader::copy(int bytes)
{
  Buf subBuf(Buf::ByteArray{iBuf.begin() + iOffset, iBuf.begin() + bytes + iOffset});
  return subBuf;
}

void Reader::skip(int bytes)
{
  iOffset += bytes;
}


} // namespace parser::reader