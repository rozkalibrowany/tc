#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <string>
#include <sstream>
#include <iomanip>

namespace tc {

std::string unsigned_char_to_string(unsigned char* data, uint32_t length)
{
	std::stringstream ss;
	ss << std::hex;

	for (uint32_t i = 0; i < length; ++i) {
		ss << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
	}
	return ss.str();
}

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

std::string hex_to_string(const std::string& input)
{
	const auto len = input.length();
	if (len & 1)
		return "invarg";

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

} // namespace tc

#endif /* BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C */
