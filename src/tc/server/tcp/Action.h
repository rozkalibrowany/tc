#ifndef D1DC27A3_560C_41B9_978D_1501280E6365
#define D1DC27A3_560C_41B9_978D_1501280E6365

#include <tc/common/Common.h>
#include <tc/parser/Buf.h>

namespace tc::server::tcp {

using namespace parser;

class Action {
public:

	enum Type {
		unknown = 0,
		payload,
		payload_imei,
		command
	};

	Action(Type type = Type::unknown);
	~Action() = default;

	const Type &type() const;

	result_t parse(const uchar* buffer, size_t size);

private:
	Type iType;
};

} // namespace tc::server::tcp

#endif /* D1DC27A3_560C_41B9_978D_1501280E6365 */
