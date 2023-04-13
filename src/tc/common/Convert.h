#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace tc {

inline unsigned int char2hex(const unsigned char c)
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

inline unsigned int hex2int(const unsigned char* c)
{
	unsigned int ret;
	std::stringstream ss;
	ss << std::hex << c;
	ss >> ret;
	return ret;
}

inline unsigned int char2int(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;

    return 0;
}

inline std::string bytes2hex(const uint8_t *in, int size)
{
	std::string out;
	char temp[8];
	for (int i = 0; i < size; ++i) {
		snprintf(temp, sizeof(temp), "%02hhx", in[i]);
		out += temp;
	}
	return out;
}

inline std::string tohex(const std::string &s, bool upper = false)
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

inline std::string string2hex(const std::string &s)
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

inline std::string hex2string(const std::string &s)
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

inline std::string uchar2string(const unsigned char *c, uint32_t length)
{
	std::stringstream ss;
	ss << std::hex;

	for (uint32_t i = 0; i < length; ++i) {
		ss << std::setw(2) << std::setfill('0') << static_cast< unsigned int >(c[i]);
	}
	return ss.str();
}


inline std::string byte2string(int val, int width = 2)
{
	std::stringstream ss;
	ss << std::setw(width) << std::setfill('0') << std::hex << (val);
	std::string ret(ss.str());

	return ret;
}

inline void hex2bin(const char* src, char* target)
{
	while(*src && src[1])
	{
		*(target++) = char2int(*src)*16 + char2int(src[1]);
		src += 2;
	}
}

inline std::string hexStr(const uint8_t *data, int len)
{
	std::stringstream ss;
	ss << std::hex;

	for( int i(0) ; i < len; ++i )
			ss << std::setw(2) << std::setfill('0') << (int)data[i];

	return ss.str();
}


} // namespace tc

#endif /* BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C */
