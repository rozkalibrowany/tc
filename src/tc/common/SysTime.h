#ifndef B262A57B_40ED_4E8E_970E_F62EEFE161A6
#define B262A57B_40ED_4E8E_970E_F62EEFE161A6

#include <cstdint>
#include <string>

namespace tc {

class SysTime
{
public:
	SysTime(bool now = false)
	{
		iTimestamp = 978307200000LL;
		if (now)
			set();
	}

	SysTime(int64_t timestamp)
	{
		iTimestamp = timestamp;
	}

	virtual ~SysTime()
	{
	}

	int getYear() const;
	int getMonth() const;
	int getDay() const;
	int getHour() const;
	int getMin() const;
	int getSec() const;
	int getMSec() const;
	std::string getDateTime() const;
	std::string getDateWithoudDelim() const;

	void set();
	void set(int year, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0, int msec = 0);

	SysTime operator-(long long t) const;
	long long operator-(const SysTime &time) const;
	SysTime operator+(long long t) const;
	SysTime &operator-=(long long t);
	SysTime &operator+=(long long t);

	auto operator<=>(const SysTime&) const = default;

	SysTime& timestamp(int64_t value);
	int64_t timestamp() const;

protected:
	long long iTimestamp;
};

} // namespace tc

#endif /* B262A57B_40ED_4E8E_970E_F62EEFE161A6 */
