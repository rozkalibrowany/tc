#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <sstream>
#include <iostream>

namespace tc {

std::string tohex(const std::string &s, bool upper = false);
std::string unsigned_char_to_string(const unsigned char* data, uint32_t length);

unsigned int hex2int(const unsigned char *data);
int char2int(char input);
void hex2bin(const char *src, char *target);

} // namespace tc

#endif /* BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C */
