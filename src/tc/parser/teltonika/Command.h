#ifndef EBC1B453_899B_44DB_BCE2_D5BD9C5CF467
#define EBC1B453_899B_44DB_BCE2_D5BD9C5CF467

#include <tc/parser/CommandI.h>

namespace tc::parser::teltonika {

class Command : public CommandI
{
public:
	static std::map <std::string, std::string> sMapping;

	Command(const uchar* cbuf, size_t size);
	Command(const std::string& cmd, const Imei& imei);
	Command(const Command &rhs) = default;
	Command(Command &&rhs) = default;

	~Command() = default;

	result_t parse(const uchar* cbuf, size_t size) override;

protected:
	void create(const std::string &cmd, const std::string& imei, Protocol::Type type = Protocol::eTeltonika) override;
	result_t getPayload(const std::string &cmd, common::Buf &buf);
};

} // namespace tc::parser::teltonika

#endif /* EBC1B453_899B_44DB_BCE2_D5BD9C5CF467 */
