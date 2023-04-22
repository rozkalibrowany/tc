#ifndef DA60B58C_59CB_4A50_9994_E86447621129
#define DA60B58C_59CB_4A50_9994_E86447621129

#include <vector>
#include <iterator>
#include <tc/common/Common.h>

namespace tc::common {

class Buf final {
public:

  using ByteArray = std::vector < uchar >;
	using iterator = Buf::ByteArray::iterator;
	using const_iterator = Buf::ByteArray::const_iterator;

	Buf(const size_t size = 0);
	Buf(const uchar *rhs, size_t size);
  Buf(const ByteArray &rhs);
	Buf(ByteArray::iterator begin, ByteArray::iterator end);
	Buf(ByteArray::const_iterator begin, ByteArray::const_iterator end);

	Buf(Buf &&rhs) = default;
	Buf(const Buf &rhs) = default;

	~Buf();

	bool operator==(const Buf &rhs) const;
	Buf &operator=(const Buf &rhs);
	Buf &operator=(Buf &&rhs);
  uchar &operator[](uint idx);
	const uchar &operator[](uint idx) const;

	std::string_view asHex() const;

	uint16_t toUInt16(int offset) const;
	int16_t toInt16(int offset) const;
	int32_t toInt32(int offset) const;
  int64_t toInt64(int offset) const;

	const_iterator find_nth_it(char delim, unsigned n = 1) const;
	int find_nth(char delim, unsigned n = 1) const;

	const bool empty() const;
	const size_t size() const;

	void clear();

	const uchar *cdata() const;
	uchar *data();

	void push_back(const uchar val);
	result_t insert(iterator begin, iterator end);
	result_t insert(const uchar *buf, size_t length);
	result_t insert(const char* buf, size_t length);

  iterator begin();
	iterator end();

  const_iterator cbegin() const;
	const_iterator cend() const;

  ByteArray iBuf;
};

} // namespace tc::common

#endif /* DA60B58C_59CB_4A50_9994_E86447621129 */
