#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/parser/Buf.h>

namespace tc::parser {

class Command : public tc::LogI
{
public:
	static std::map <std::string, std::string> sMapping;

	Command(const std::string& imei);
	~Command() = default;

	result_t create(const std::string &cmd, bool cr = false);

	Buf iBuf;
	std::string iImei;

private:
	result_t getPayload(const std::string &cmd, parser::Buf &buf);
};

} // namespace tc::parser

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */
