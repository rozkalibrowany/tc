#ifndef BF982547_2C16_4935_B8F9_3937C7ECCCA2
#define BF982547_2C16_4935_B8F9_3937C7ECCCA2

#include <tc/common/Common.h>

namespace tc::parser {

class Protocol {

public:
	enum Type {
		eUnknown = 0,
		eTeltonika,
		eOmni
	};

	result_t parse(const uchar *buffer, size_t size);
	Type type() const;

protected:
	Type iType{eUnknown};
};

} // namespace tc::parser

#endif /* BF982547_2C16_4935_B8F9_3937C7ECCCA2 */
