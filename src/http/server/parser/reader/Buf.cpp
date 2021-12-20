#include <reader/Buf.h>
#include <array>

namespace parser::reader {

Buf::Buf(const size_t size)
 : iBuf(ByteArray(size))
 , iPos(0)
{
	// nothing to do
}

Buf::Buf(const unsigned char *rhs, size_t size)
 : iBuf(rhs, rhs + size)
{
	// nothing to do
}

Buf::Buf(const char *rhs, size_t size)
 : Buf(reinterpret_cast< const unsigned char* >(rhs), size)
{
  // nothing to do
}
Buf::Buf(const std::string &rhs)
 : Buf(rhs.c_str(), rhs.size())
{
  // nothing to do
}

Buf::Buf(const ByteArray &rhs)
 : iBuf(ByteArray(rhs.begin(), rhs.end()))
{
	// nothing to do
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

unsigned char &Buf::operator[](uint idx)
{
	return iBuf[idx];
}

const unsigned char &Buf::operator[](uint idx) const
{
	return iBuf[idx];
}

uint16_t Buf::toUInt16(int offset)
{
	std::cout << "readValueUnsigned 4" << std::endl;
	return (((int)iBuf[offset] & 0xFF) | (((int)iBuf[offset + 1] & 0xFF) << 8));
}

int16_t Buf::toInt16(int offset)
{
	 return (int16_t) (iBuf[offset] & 0xFF | (iBuf[offset + 1] & 0xFF) << 8);
}

int32_t Buf::toInt32(int offset)
{
	return (iBuf[offset] & 0xFF) |
                ((iBuf[offset + 1] & 0xFF) << 8)
                | ((iBuf[offset + 2] & 0xFF) << 16)
                | ((iBuf[offset + 3] & 0xFF) << 24);
}

int64_t Buf::toInt64(int offset)
{
	return (((int64_t) (iBuf[offset + 7] & 0xff) << 56)
                | ((int64_t) (iBuf[offset + 6] & 0xff) << 48)
                | ((int64_t) (iBuf[offset + 5] & 0xff) << 40)
                | ((int64_t) (iBuf[offset + 4] & 0xff) << 32)
                | ((int64_t) (iBuf[offset + 3] & 0xff) << 24)
                | ((int64_t) (iBuf[offset + 2] & 0xff) << 16)
                | ((int64_t) (iBuf[offset + 1] & 0xff) << 8) | (iBuf[offset] & 0xff));
}

const size_t Buf::size()
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


} // namespace parser::reader