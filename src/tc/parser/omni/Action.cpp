#include <tc/parser/omni/Action.h>
#include <cstdlib>

namespace tc::parser::omni {

using namespace parser;

Action::Type Action::get(const uchar* buffer, size_t size)
{
	// casting to string_view
	const std::string_view str_buffer(reinterpret_cast<char const *>(buffer));
	// getting index of instruction segment
	auto index = index_of_nth(str_buffer, ',', 4);
	if (index + 2 > size) // index + size of instruction
		return eInvalid;
	// getting instruction
	std::string type((const char *)buffer + index, 2);

	return get(type);
}

Action::Type Action::get(const std::string &type)
{
	if (type.empty())
		return eInvalid;

	unsigned val = strtoul(tohex(type).c_str(), 0, 16);
	switch (val) {
	case eCheckIn:
		return eCheckIn;
	case eHeartBeat:
		return eHeartBeat;
	case eUnlock:
		return eUnlock;
	case eLockReport:
		return eLockReport;
	case eAlarm:
		return eAlarm;
	case eUpgradeDetection:
		return eUpgradeDetection;
	case eUpgradeData:
		return eUpgradeData;
	case eUpgradeResults:
		return eUpgradeResults;
	default:
		return eUnknown;
	}
}

size_t Action::index_of_nth(const std::string_view &str, char delim, unsigned n)
{
	if (n == 0)
		return std::string_view::npos;

	size_t pos, from = 0;
	unsigned i=0;
	while (i < n) {
		pos = str.find(delim, from);
		if (pos == std::string_view::npos)
			break;
		from = pos + 1;
		++i;
	}
	return ++pos; // return with +1 offset
}

} // namespace tc::parser::omni
