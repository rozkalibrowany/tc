#include <tc/common/Regex.h>

namespace tc {

const std::string regex(const std::regex &reg, const std::string_view str)
{
	return regex(reg, std::string(str));
}

const std::string regex(const std::regex &reg, const std::string str)
{
	std::smatch sm;
	for(std::sregex_iterator it = std::sregex_iterator(
		str.begin(), str.end(), reg);
		it != std::sregex_iterator(); it++) {
			sm = *it;
	}

	return sm.empty() ? std::string() : sm.str(1);
}

} // namespace tc
