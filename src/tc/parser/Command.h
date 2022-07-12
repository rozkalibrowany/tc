#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/parser/Buf.h>

namespace tc::parser {

class Command : public tc::LogI
{
public:
	static const std::string unlock;
	static const std::string lock;
	static const std::string led_on;
	static const std::string led_off;
	static const std::string engine_on;
	static const std::string engine_off;
	static const std::string restart;

	static const std::string cmdToString(const std::string &cmd);

	Command(const std::string& imei);
	virtual ~Command() = default;

	result_t create(const std::string &cmd, bool cr = false);
	std::vector<char> asBin();

private:
	result_t getPayload(const std::string &cmd, parser::Buf &buf);

	Buf iBuf;
	std::string iImei;
};

} // namespace tc::parser

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */
