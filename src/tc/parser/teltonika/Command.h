#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/common/Buf.h>

namespace tc::parser::teltonika {

class Command : public tc::LogI
{
public:
	static std::map <std::string, std::string> sMapping;

	Command(const std::string& imei);
	~Command() = default;

	result_t create(const std::string &cmd, bool cr = false);

	common::Buf iBuf;
	std::string iImei;

private:
	result_t getPayload(const std::string &cmd, common::Buf &buf);
};

} // namespace tc::parser::teltonika

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */