#include <tc/common/Convert.h>

namespace tc {

unsigned int char2hex(const unsigned char c)
{
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    return hex_values[c];
}

unsigned int hex2int(const unsigned char* c)
{
	unsigned int ret;
	std::stringstream ss;
	ss << std::hex << c;
	ss >> ret;
	return ret;
}

unsigned int char2int(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;

    return 0;
}

std::string tohex(const std::string &s, bool upper)
{
	std::ostringstream ret;

	unsigned int c;
	for (std::string::size_type i = 0; i < s.length(); ++i)
	{
		c = (unsigned int)(unsigned char)s[i];
		ret << std::hex << std::setfill('0') <<
			std::setw(2) << (upper ? std::uppercase : std::nouppercase) << c;
	}
	return ret.str();
}

std::string string2hex(const std::string &s)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string ret;
    ret.reserve(s.length() * 2);
    for (unsigned char c : s)
    {
        ret.push_back(hex_digits[c >> 4]);
        ret.push_back(hex_digits[c & 15]);
    }
    return ret;
}

std::string hex2string(const std::string &s)
{
    const auto len = s.length();
    if (len & 1) return "";

    std::string ret;
    ret.reserve(len / 2);
    for (auto it = s.begin(); it != s.end();)
    {
        int hi = char2hex(*it++);
        int lo = char2hex(*it++);
        ret.push_back(hi << 4 | lo);
    }
    return ret;
}

std::string uchar2string(const unsigned char *c, uint32_t length)
{
	std::stringstream ss;
	ss << std::hex;

	for (uint32_t i = 0; i < length; ++i) {
		ss << std::setw(2) << std::setfill('0') << static_cast< unsigned int >(c[i]);
	}
	return ss.str();
}

std::string uchar2string2(const unsigned char *c, uint32_t length)
{
	std::string s(*c, length);

	return s;
}

std::string byte2string(int val, int width)
{
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << std::hex << (val);
	std::string ret(ss.str());

	//std::transform(ret.begin(), ret.end(), ret.begin(),
	//	[](char c) { return std::toupper(c); });

	return ret;
}

void hex2bin(const char* src, char* target)
{
	while(*src && src[1])
	{
		*(target++) = char2int(*src)*16 + char2int(src[1]);
		src += 2;
	}
}

} // namespace tc
