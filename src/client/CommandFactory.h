#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/common/Common.h>
#include <map>

namespace tc::client::tcp {

class CommandFactory : public tc::LogI
{
public:
	enum Type
	{
		Unlock = 0,
		Lock
	};

	static const std::string cmd_unlock;
	static const std::string cmd_lock;

	static const std::string cmd_led_on;
	static const std::string cmd_led_off;

	static const std::string cmd_engine_on;
	static const std::string cmd_engine_off;

	CommandFactory(const std::string& imei);
	virtual ~CommandFactory() = default;

	result_t create(std::map< int, std::vector< uchar > > &cmdMap, const std::string &cmd, bool cr = false);

private:
	inline std::string getHex(int val, int width = 2);
	inline result_t convertCommand(std::vector<uchar> &packet, const std::string &cmd);

	Type iType;
	std::string iImei;
};

} // namespace tc::client::tcp

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */
