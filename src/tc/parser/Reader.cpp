#include <tc/parser/Reader.h>
#include <tc/common/Common.h>
#include <array>
#include <algorithm>

namespace tc::parser {

Reader::Reader(std::unique_ptr< Buf > buf, int offset)
 : iBuf(std::move(buf))
 , iOffset(offset)
{
  // nothing to do
}

/*Reader &Reader::operator=(const Reader &reader)
{
	if (this == &reader) return *this;
	iBuf.reset();
	iBuf = std::move(reader.iBuf);
	iOffset = reader.iOffset;
	return *this;
}*/

uint Reader::readU(int bytes)
{
  Buf subBuf(Buf::ByteArray{iBuf->begin() + iOffset, iBuf->begin() + bytes + iOffset});
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
  Buf subBuf(Buf::ByteArray{iBuf->begin() + iOffset, iBuf->begin() + bytes + iOffset});
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
  Buf subBuf(Buf::ByteArray{iBuf->begin() + iOffset, iBuf->begin() + bytes + iOffset});
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

std::string Reader::readImei(const std::string &str)
{
	if (str.empty()) {
		return std::string();
	}

	std::string s_out;
	for (std::string::size_type i = 1; i < str.size(); i += 2)
	{
		char c = str[i];
		s_out.push_back(c);
	}
	return s_out;
}

void Reader::skip(int bytes)
{
  iOffset += bytes;
}


} // namespace tc::parser