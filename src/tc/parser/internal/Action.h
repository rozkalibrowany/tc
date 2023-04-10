#ifndef F829D04E_A464_46C2_9788_54419DCBF97F
#define F829D04E_A464_46C2_9788_54419DCBF97F

#include <tc/common/Common.h>

namespace tc::parser::internal {

class Action {
public:

	enum Type {
		unknown = 0,
		command,
		request,
	};

	static Type get(const uchar* buffer, size_t size);
};

} // namespace tc::parser::internal

#endif /* F829D04E_A464_46C2_9788_54419DCBF97F */
