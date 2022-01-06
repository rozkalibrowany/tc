#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <string>

namespace tc {

std::string unsigned_char_to_string(unsigned char* charArray, uint32_t charArrayLength)
{

	if(charArray == nullptr) {
			return "nullptr";
	}

	if(charArrayLength > 65536) {
			return "charArrayLength overflow";
	}

	constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
															'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	std::string s(charArrayLength * 2, ' ');
	for (int i = 0; i < charArrayLength; ++i) {
			s[2 * i] = hexmap[(charArray[i] & 0xF0) >> 4];
			s[2 * i + 1] = hexmap[charArray[i] & 0x0F];
	}
	return s;
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
