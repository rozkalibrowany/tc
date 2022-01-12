#ifndef CD123D7B_6390_454A_9DB3_5EAE34FB9315
#define CD123D7B_6390_454A_9DB3_5EAE34FB9315

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace tc::common {

using SpdLogSptr = std::shared_ptr<spdlog::logger>;
class Log;

class LogI
{
public:
	static SpdLogSptr defaultLogger(const std::string name = "console");

	LogI(const std::string name = "console");
	LogI(SpdLogSptr rhs, const std::string name = "console");
	LogI(Log &rhs, const std::string name = "console");

	virtual ~LogI() = default;

	virtual SpdLogSptr &logger();
	virtual SpdLogSptr &getLogger();

	void log(const std::string &msg) const;

	void name(const std::string &rhs);
	const std::string &name() const;

private:
	SpdLogSptr m_log;
	std::string m_name;
};

class Log : public LogI
{
public:
	virtual ~Log() = default;
};

} // namespace tc::common

#endif /* CD123D7B_6390_454A_9DB3_5EAE34FB9315 */
