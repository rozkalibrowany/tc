#include <stdio.h>
#include <Packet.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace parser;

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
    if (value == -1) throw std::invalid_argument("invalid hex digit");
    return value;
}

std::string hex_to_string(const std::string& input)
{
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

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

int main(int argc, char** argv) {

  std::string str("00000000000004af8e0d0000017dd52caa70000c9743141f1ca322004d00cd1500000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000313801640001e2b2000000010153000e4e325a42483138353143303036380000017dd5281690000c9743141f1ca322004d00cd1500000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000313801640001e2b2000000010153000e4e325a42483138353143303036380000017dd52382b0000c9743141f1ca322004d00cd1500000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000313801640001e2b2000000010153000e4e325a42483138353143303036380000017dd51eeed0000c9743141f1ca322004d00cd1600000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000313801640001e2b2000000010153000e4e325a42483138353143303036380000017dd51a5af0000c9743141f1ca322004d00cd1400000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd515c710000c9743141f1ca322004d00cd1300000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd5113330000c9743141f1ca322004d00cd1400000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd50c9f50000c9743141f1ca322004d00cd1300000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd5080b70000c9743141f1ca322004d00cd1400000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd5037790000c9743141f1ca322004d00cd1400000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd4fdfd38000c9743881f1c9f4b0038009b1400000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd4f96958000c9743881f1c9f4b0038009b1100000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380000017dd4f4d578000c973b541f1ca45f003d00df1300000000000a0006007164015500015801015d0001601c0163000000000300100001e2b201620000321401640001e2b2000000010153000e4e325a42483138353143303036380d00");
  std::string fromHex = hex_to_string(str);


  unsigned char v[fromHex.length() + 1];
  std::copy(fromHex.data(), fromHex.data() + fromHex.length() + 1, v);

  Packet packet;
  packet.parse(v, (size_t) fromHex.length());


  return 0;

}
