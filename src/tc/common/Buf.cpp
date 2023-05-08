#include <tc/common/Buf.h>

namespace tc::common {

Buf::Buf(const size_t size)
 : iBuf(ByteArray(size))
{
	// nothing to do
}

Buf::Buf(const uchar *rhs, size_t size)
 : iBuf(rhs, rhs + size)
{
	// nothing to do
}

Buf::Buf(const ByteArray &rhs)
 : iBuf(ByteArray(rhs.begin(), rhs.end()))
{
	// nothing to do
}

Buf::Buf(Buf::ByteArray::iterator begin, Buf::ByteArray::iterator end)
 : iBuf(ByteArray(begin, end))
{
	// nothing to do
}

Buf::Buf(Buf::ByteArray::const_iterator begin, Buf::ByteArray::const_iterator end)
 : iBuf(ByteArray(begin, end))
{
	// nothing to do
}


Buf::~Buf()
{
	iBuf.clear();
}

Buf &Buf::operator=(const Buf &rhs)
{
	iBuf = rhs.iBuf;
	return *this;
}

bool Buf::operator==(const Buf &rhs) const
{
	return iBuf == rhs.iBuf;
}

Buf &Buf::operator=(Buf &&rhs)
{
	if (this == &rhs) return *this;

	iBuf.clear();
	iBuf.swap(rhs.iBuf);

	rhs.iBuf.clear();
	return *this;
}

uchar &Buf::operator[](uint idx)
{
	return iBuf[idx];
}

const uchar &Buf::operator[](uint idx) const
{
	return iBuf[idx];
}

std::string_view Buf::asHex() const
{
	return size() == 0 ? "" : bytes2hex(iBuf.data(), size());
}

// Allow to remove n characters from end
// helpful with newline terminated buffer
std::string_view Buf::asString() const
{
	if (iBuf.empty())
		return std::string_view();

	std::string_view str(reinterpret_cast<const char *>(iBuf.data()));
	return str;
}

std::string_view Buf::asStringWithoutSuffix(size_t characters) const
{
	if (iBuf.empty())
		return std::string_view();

	std::string_view str(reinterpret_cast<const char *>(iBuf.data()));
	// remove any newlines for pretty output
	if (characters > 0 && characters < size())
		str.remove_suffix(characters);
	return str;
}

void Buf::clear()
{
	iBuf.clear();
}

const uchar *Buf::cdata() const
{
	return iBuf.data();
}

uchar *Buf::data()
{
	return iBuf.data();
}

void Buf::push_back(const uchar val)
{
	iBuf.push_back(val);
}

result_t Buf::insert(iterator begin, iterator end)
{
	if (begin == end) {
		return RES_NOENT;
	}

	iBuf.insert(this->end(), begin, end);
	return RES_OK;
}

result_t Buf::insert(const char* buf, size_t length)
{
	return insert((const uchar*) buf, length);
}

result_t Buf::insert(const uchar* buf, size_t length)
{
	if (buf == nullptr) {
		return RES_INVARG;
	}

	iBuf.insert(end(), buf, buf + length);
	return RES_OK;
}

uint16_t Buf::toUInt16(int offset) const
{
	return ((iBuf[offset] & 0xFF) | ((iBuf[offset + 1] & 0xFF) << 8));
}

int16_t Buf::toInt16(int offset) const
{
	 return (int16_t) ((iBuf[offset] & 0xFF) | ((iBuf[offset + 1] & 0xFF) << 8));
}

int32_t Buf::toInt32(int offset) const
{
	return ((iBuf[offset] & 0xFF) |
                ((iBuf[offset + 1] & 0xFF) << 8)
                | ((iBuf[offset + 2] & 0xFF) << 16)
                | ((iBuf[offset + 3] & 0xFF) << 24));
}

int64_t Buf::toInt64(int offset) const
{
	return (((int64_t) (iBuf[offset + 7] & 0xff) << 56)
                | ((int64_t) (iBuf[offset + 6] & 0xff) << 48)
                | ((int64_t) (iBuf[offset + 5] & 0xff) << 40)
                | ((int64_t) (iBuf[offset + 4] & 0xff) << 32)
                | ((int64_t) (iBuf[offset + 3] & 0xff) << 24)
                | ((int64_t) (iBuf[offset + 2] & 0xff) << 16)
                | ((int64_t) (iBuf[offset + 1] & 0xff) << 8) | (iBuf[offset] & 0xff));
}

Buf::const_iterator Buf::find_nth_it(char delim, unsigned n) const
{
	if (n == 0)
		return cend();

	uint8_t occurences = 0;
	for (ByteArray::const_iterator it = iBuf.begin(); it != iBuf.end(); it++) {
		if (occurences == n)
			return it; // return start point with offset + 1

		if (*it == delim)
			occurences++;
	}

	return cend();
}

int Buf::find_nth(char delim, unsigned n) const
{
	auto it = find_nth_it(delim, n);

	return it == cend() ? 0 : std::distance(cbegin(), it);
}

const bool Buf::empty() const
{
	return iBuf.empty();
}

const size_t Buf::size() const
{
	return iBuf.size();
}

Buf::ByteArray::iterator Buf::begin()
{
	return iBuf.begin();
}

Buf::ByteArray::iterator Buf::end()
{
	return iBuf.end();
}

Buf::ByteArray::const_iterator Buf::cbegin() const
{
	return iBuf.cbegin();
}

Buf::ByteArray::const_iterator Buf::cend() const
{
	return iBuf.cend();
}


} // namespace tc::common