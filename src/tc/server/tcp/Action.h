#ifndef D1DC27A3_560C_41B9_978D_1501280E6365
#define D1DC27A3_560C_41B9_978D_1501280E6365

#include <tc/common/Common.h>
#include <tc/parser/Buf.h>

namespace tc::server::tcp {

class Action {
public:

	using Imei = std::string;

	enum Type {
		unknown = 0,
		payload,
		imei,
		command,
		incomplete_payload,
		standby
	};

	static Type get(const uchar* buffer, size_t size);

	static bool hasImei(const uchar *cbuf, size_t size);
	static result_t parseImei(const uchar *cbuf, size_t size, Imei &imei);
	static const Imei toImei(const uchar *cbuf, int len);

};

} // namespace tc::server::tcp

#endif /* D1DC27A3_560C_41B9_978D_1501280E6365 */
