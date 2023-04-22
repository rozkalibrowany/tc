#ifndef BA91C251_20B5_408F_9FA1_C2597744FBCC
#define BA91C251_20B5_408F_9FA1_C2597744FBCC

#include <tc/common/Common.h>
#include <fmt/format.h>

namespace tc::parser::omni {

class Datetime : public SysTime
{
public:
	Datetime(bool now = false) : SysTime(now) {}

	result_t parse(const std::string& datetime) {
		if (datetime.empty())
			return RES_NOENT;

		auto year = std::stoi(datetime.substr(0, 1));
		auto month = std::stoi(datetime.substr(2, 3));
		auto day = std::stoi(datetime.substr(4, 5));
		auto hour = std::stoi(datetime.substr(6, 7));
		auto minute = std::stoi(datetime.substr(8, 9));
		auto sec = std::stoi(datetime.substr(10, 11));

		set(year, month, day, hour, minute, sec);

		return RES_OK;
	}

	std::string_view datetime() const {
		return fmt::format("{}{}{}{}{}{}", getYear(), getMonth(),
		getDay(), getHour(), getMin(), getSec());
	}
};

} // namespace tc::parser::omni


#endif /* BA91C251_20B5_408F_9FA1_C2597744FBCC */
