#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace tc {

std::string unsigned_char_to_string(const unsigned char* data, uint32_t length)
{
	std::stringstream ss;
	ss << std::hex;

	for (uint32_t i = 0; i < length; ++i) {
		ss << std::setw(2) << std::setfill('0') << static_cast< int >(data[i]);
	}
	return ss.str();
}

int char2int(char input)
{
	if(input >= '0' && input <= '9')
		return input - '0';
	if(input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

void hex2bin(const char* src, char* target)
{
	while(*src && src[1])
	{
		*(target++) = char2int(*src)*16 + char2int(src[1]);
		src += 2;
	}
}

std::string tohex(const std::string &s, bool upper = false)
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

unsigned int hex2int(const unsigned char* data)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << data;
	ss >> x;
	return x;
}

} // namespace tc
