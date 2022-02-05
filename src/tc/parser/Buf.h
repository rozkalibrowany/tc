#ifndef DA60B58C_59CB_4A50_9994_E86447621129
#define DA60B58C_59CB_4A50_9994_E86447621129

#include <tc/common/Common.h>
#include <vector>
#include <iostream>
#include <iterator>

namespace tc::parser {

class Buf {
public:

  using ByteArray = std::vector < uchar >;

  Buf(const size_t size);
  Buf(const uchar *rhs, size_t size);
  Buf(const ByteArray &rhs);
	Buf(ByteArray::iterator &begin, ByteArray::iterator &end);

	~Buf();

	bool operator==(const Buf &rhs) const;
	Buf &operator=(const Buf &rhs);
	Buf &operator=(Buf &&rhs);

  uchar &operator[](uint idx);
	const uchar &operator[](uint idx) const;

  uint16_t toUInt16(int offset);
  int16_t toInt16(int offset);
  int32_t toInt32(int offset);
  int64_t toInt64(int offset);

	const size_t size();
  Buf::ByteArray::iterator begin();
	Buf::ByteArray::iterator end();

  Buf::ByteArray::const_iterator cbegin() const;
	Buf::ByteArray::const_iterator cend() const;

  ByteArray iBuf;
};

} // namespace tc::parser

#endif /* DA60B58C_59CB_4A50_9994_E86447621129 */
