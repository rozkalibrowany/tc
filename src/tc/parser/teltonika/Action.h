#ifndef D1DC27A3_560C_41B9_978D_1501280E6365
#define D1DC27A3_560C_41B9_978D_1501280E6365

#include <tc/common/Common.h>
#include <tc/common/Buf.h>

namespace tc::parser::teltonika {

class Action {
public:

	enum Type {
		eUnknown = 0,
		eImei,
		ePayload,
		eStandby,
		eIncompletePayload
	};

	static Type get(const uchar* buffer, size_t size);
};

} // namespace tc::parser::teltonika

#endif /* D1DC27A3_560C_41B9_978D_1501280E6365 */
