#include <tc/common/Logger.h>

namespace tc {

SpdLogSptr LogI::defaultLogger(const std::string name)
{
	return spdlog::get(name);
}


LogI::LogI(SpdLogSptr rhs, const std::string name)
	: m_log(rhs)
	, m_name(name)
{
	m_log->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%s][%!:%#][%^%l%$] %v");
}

LogI::LogI(Log &rhs, const std::string name)
	: LogI(rhs.logger(), name)
{
}

LogI::LogI(const std::string name)
	: LogI(LogI::defaultLogger(name))
{
	// nothing to do
}

SpdLogSptr &LogI::logger()
{
	return m_log;
}

SpdLogSptr &LogI::getLogger()
{
	return m_log;
}

void LogI::name(const std::string &rhs)
{
	m_name = rhs;
}

const std::string &LogI::name() const
{
	return m_name;
}

} // namespace tc
