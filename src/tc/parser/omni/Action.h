#ifndef C13D1DD0_9961_48DD_931A_636970E3743B
#define C13D1DD0_9961_48DD_931A_636970E3743B

#include <tc/common/Buf.h>

namespace tc::parser::omni::action {

class Locker : tc::LogI {
public:
	enum Instruction
	{
		eCheckIn                      = 0x5130,	// Q0
		eHeartBeat                    = 0x4830,	// H0
		eUnlock                       = 0x4C30,	// L0
		eLockReport                   = 0x4C31,	// L1
		eAlarm                        = 0x5730,	// W0
		eUpgradeDetection             = 0x5530,	// U0
		eUpgradeData                  = 0x5531,	// U1
		eUpgradeResults               = 0x5532,	// U2
		eUnknown
	};

	static Instruction get(const common::Buf &buf);
};

class Server : tc::LogI {
public:

	enum Instruction
	{
		eUnlock                       = 0x4C30,	// L0
		eAcquisitionPositioning       = 0x4430,	// D0
		eLocationTracing              = 0x4431,	// D1
		eLockInformation              = 0x5335,	// S5
		eBikeSearch                   = 0x5338,	// S8
		eAcquireLockFirmware          = 0x4730,	// G0
		eAlarm                        = 0x5730,	// U2
		eUnknown
	};


	static Instruction get(const common::Buf &buf);
};

} // namespace tc::parser::namespace

#endif /* C13D1DD0_9961_48DD_931A_636970E3743B */
