#ifndef BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C
#define BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

namespace tc {

unsigned int char2hex(const unsigned char c);
unsigned int hex2int(const unsigned char *c);
unsigned int char2int(char c);

std::string tohex(const std::string &s, bool upper = false);
std::string string2hex(const std::string &s);
std::string hex2string(const std::string &s);
std::string byte2string(int val, int width = 2);
std::string uchar2string(const unsigned char *c, uint32_t length);
std::string uchar2string2(const unsigned char *c, uint32_t length);

void hex2bin(const char* src, char* target);

} // namespace tc

#endif /* BFF772B8_549C_4D5D_AFDC_BCC7C72ACE9C */
