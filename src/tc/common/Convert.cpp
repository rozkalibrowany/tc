#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace tc {

int hex_value(unsigned char hex_digit)
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
    int value = hex_values[hex_digit];
    return value;
}

std::string unsigned_char_to_string(const unsigned char* data, uint32_t length)
{
	std::stringstream ss;
	ss << std::hex;

	for (uint32_t i = 0; i < length; ++i) {
		ss << std::setw(2) << std::setfill('0') << static_cast< int >(data[i]);
	}
	return ss.str();
}

const unsigned char *string2unsigned_char(const std::string &s, size_t &size)
{
	std::vector<unsigned char> uchars;
	transform(std::begin(s), std::end(s), std::back_inserter(uchars),
			[](char c) { return static_cast<unsigned char>(c); });
	size = uchars.size();
	return uchars.data();
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


std::string hex2string(const std::string& input)
{
    const auto len = input.length();
    if (len & 1) return "";

    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end(); )
    {
        int hi = hex_value(*it++);
        int lo = hex_value(*it++);
        output.push_back(hi << 4 | lo);
    }
    return output;
}

std::string string2hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
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
