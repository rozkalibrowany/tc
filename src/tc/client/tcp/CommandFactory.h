#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/common/Common.h>
#include <tc/parser/Buf.h>
#include <tc/parser/packet/Packet.h>
#include <map>

namespace tc::client::tcp {

class CommandFactory : public tc::LogI
{
public:
	static const std::string cmd_unlock;
	static const std::string cmd_lock;

	static const std::string cmd_led_on;
	static const std::string cmd_led_off;

	static const std::string cmd_engine_on;
	static const std::string cmd_engine_off;

	static const std::string cmd_restart;

	static const std::string cmdToString(const std::string &cmd);

	CommandFactory(const std::string& imei);
	virtual ~CommandFactory() = default;

	result_t create(const std::string &cmd, parser::Buf &buf, bool cr = false);

private:
	result_t getPayload(const std::string &cmd, parser::Buf &buf);

	std::string iImei;
};

} // namespace tc::client::tcp

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */
