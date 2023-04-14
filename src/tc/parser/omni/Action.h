#ifndef C13D1DD0_9961_48DD_931A_636970E3743B
#define C13D1DD0_9961_48DD_931A_636970E3743B

#include <tc/common/Common.h>

namespace tc::parser::omni {

class Action : tc::LogI {
public:
	enum Type
	{
		eCheckIn          = 0x5130,	// Q0
		eHeartBeat        = 0x4830,	// H0
		eUnlock           = 0x4C30,	// L0
		eLockReport       = 0x4C31,	// L1
		eAlarm            = 0x5730,	// W0
		eUpgradeDetection = 0x5530,	// U0
		eUpgradeData      = 0x5531,	// U1
		eUpgradeResults   = 0x5532,	// U2
		eUnknown,
		eInvalid
	};

	Type get(const std::string &type);
	Type get(const uchar* buffer, size_t size);
};

} // namespace tc::parser::namespace

#endif /* C13D1DD0_9961_48DD_931A_636970E3743B */
