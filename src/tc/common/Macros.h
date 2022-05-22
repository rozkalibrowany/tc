#ifndef B4202F9B_1518_419B_999E_8DDCD7047F24
#define B4202F9B_1518_419B_999E_8DDCD7047F24

#include <tc/common/Logger.h>

namespace tc {

#define LG_NFO(LOGGER, ...) SPDLOG_LOGGER_INFO(LOGGER, __VA_ARGS__)
#define LG_DBG(LOGGER, ...) SPDLOG_LOGGER_DEBUG(LOGGER, __VA_ARGS__)
#define LG_WRN(LOGGER, ...) SPDLOG_LOGGER_WARN(LOGGER, __VA_ARGS__)
#define LG_ERR(LOGGER, ...) SPDLOG_LOGGER_ERROR(LOGGER, __VA_ARGS__)
#define LG_CRT(LOGGER, ...) SPDLOG_LOGGER_CRITICAL(LOGGER, __VA_ARGS__)

} // namespace tc

#endif /* B4202F9B_1518_419B_999E_8DDCD7047F24 */