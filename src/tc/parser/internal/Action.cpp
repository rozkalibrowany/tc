#include <tc/parser/internal/Action.h>
#include <tc/parser/internal/Request.h>
#include <tc/parser/CommandI.h>

namespace tc::parser::internal {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	if (Request::hasRequest(buffer, size))
		return eData;
	
	if (CommandI::hasCommand(buffer, size))
		return eCommand;

	return eUnknown;

}

} // namespace tc::parser::internal
