#ifndef CF85BD92_7E22_45A2_ABC3_BF3EC33AE7C6
#define CF85BD92_7E22_45A2_ABC3_BF3EC33AE7C6

#include <tc/common/Macros.h>
#include <tc/common/Convert.h>
#include <tc/common/Result.h>
#include <tc/common/SysTime.h>
#include <tc/common/Codec.h>
#include <tc/common/Regex.h>
#include <tc/common/Types.h>
#include <string>
#include <cstddef>
#include <vector>
#include <map>

#ifdef unix
#	undef unix
#endif

using uchar = unsigned char;
using Imei = std::string;
using timestamp = int64_t;

static const std::function< bool(int) > vIsPortNumber = [](int port)
{
	return (port >= 0 && port <= 65535);
};

static const std::function<bool(const std::string &)> vIsAddress = [](const std::string &addr)
{
	return std::count(addr.begin(), addr.end(), '.') == 3;
};

using namespace std;

#endif /* CF85BD92_7E22_45A2_ABC3_BF3EC33AE7C6 */