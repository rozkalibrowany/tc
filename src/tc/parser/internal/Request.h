#ifndef E0CCFC07_4DC4_466F_BFF6_501414B18F09
#define E0CCFC07_4DC4_466F_BFF6_501414B18F09

#include <tc/parser/RequestI.h>
#include <tc/parser/Reader.h>

namespace tc::parser::internal {

class Request : public RequestI
{
public:
	static constexpr size_t REQ_MIN_SIZE = 11;

	bool hasRequest(const uchar *buf, size_t size);

	result_t parse(const uchar *cbuf, size_t size) override;
	size_t size() const override;
};

} // namespace tc::parser::internal

#endif /* E0CCFC07_4DC4_466F_BFF6_501414B18F09 */
