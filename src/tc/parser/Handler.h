#ifndef BF982547_2C16_4935_B8F9_3937C7ECCCA2
#define BF982547_2C16_4935_B8F9_3937C7ECCCA2

#include <tc/common/Common.h>

namespace tc::parser {

class Handler {

public:
	enum Protocol {
		eUnknown = 0,
		eTeltonika,
		eOmni
	};

	result_t parse(const uchar *buffer, size_t size);
	Protocol protocol() const;

protected:
	Protocol iProtocol{eUnknown};
};

} // namespace tc::parser

#endif /* BF982547_2C16_4935_B8F9_3937C7ECCCA2 */
