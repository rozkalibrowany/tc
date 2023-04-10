#include <tc/parser/internal/Action.h>

namespace tc::parser::internal {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	return Type::unknown;
}

} // namespace tc::parser::internal
