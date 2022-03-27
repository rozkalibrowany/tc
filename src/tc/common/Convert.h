#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <sstream>
#include <iostream>

namespace tc {

	int hex_value(unsigned char hex_digit);
	std::string tohex(const std::string &s, bool upper = false);
	std::string unsigned_char_to_string(const unsigned char *data, uint32_t length);
	const unsigned char *string2unsigned_char(const std::string &s, size_t &size);

	std::string string2hex(const std::string &input);
	std::string hex2string(const std::string &input);
	unsigned int hex2int(const unsigned char *data);
	void hex2bin(const char *src, char *target);
	int char2int(char input);

} // namespace tc

#endif /* BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C */
