#include <tc/common/SysTime.h>
#include <sys/time.h>
#include <time.h>

namespace tc {

int SysTime::getYear() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_year + 1900;
}

int SysTime::getMonth() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_mon + 1;
}

int SysTime::getDay() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_mday;
}

int SysTime::getHour() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_hour;
}

int SysTime::getMin() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_min;
}

int SysTime::getSec() const
{
	struct tm lt;
	time_t t = iTimestamp / 1000;

	localtime_r(&t, &lt);
	return lt.tm_sec;
}

int SysTime::getMSec() const
{
	return iTimestamp % 1000;
}

std::string SysTime::getDateTime() const
{
	std::string datetime;
	datetime += std::to_string(getYear()) + ":";
	datetime += getMonth() < 10 ? "0" : "";
	datetime += std::to_string(getMonth()) + ":";
	datetime += getDay() < 10 ? "0" : "";
	datetime += std::to_string(getDay()) + "-";
	datetime += getHour() < 10 ? "0" : "";
	datetime += std::to_string(getHour()) + ":";
	datetime += getMin() < 10 ? "0" : "";
	datetime += std::to_string(getMin()) + ":";
	datetime += getSec() < 10 ? "0" : "";
	datetime += std::to_string(getSec());
	return datetime;
}

void SysTime::set()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	iTimestamp = (( long long ) tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void SysTime::set(int year, int month, int day, int hour, int min, int sec, int msec)
{
	struct tm lt;

	lt.tm_year = year - 1900;
	lt.tm_mon = month - 1;
	lt.tm_mday = day;
	lt.tm_hour = hour;
	lt.tm_min = min;
	lt.tm_sec = sec;
	lt.tm_isdst = -1;

	iTimestamp = (( long long ) mktime(&lt) * 1000) + msec;
}

SysTime SysTime::operator-(long long t) const
{
	SysTime time(*this);
	time.iTimestamp -= t;

	return time;
}

long long SysTime::operator-(const SysTime &time) const
{
	return iTimestamp - time.iTimestamp;
}

SysTime SysTime::operator+(long long t) const
{
	SysTime time(*this);
	time.iTimestamp += t;

	return time;
}

SysTime &SysTime::operator-=(long long t)
{
	iTimestamp -= t;

	return *this;
}

SysTime &SysTime::operator+=(long long t)
{
	iTimestamp += t;

	return *this;
}

SysTime& SysTime::timestamp(int64_t value)
{
	iTimestamp = value;
	return *this;
}

int64_t SysTime::timestamp() const
{
	return iTimestamp;
}

} // namespace tc
