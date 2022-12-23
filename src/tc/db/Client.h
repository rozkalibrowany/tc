#ifndef D636C7F7_D6B6_414B_B468_0498319874FF
#define D636C7F7_D6B6_414B_B468_0498319874FF

#include <mongocxx/v_noabi/mongocxx/instance.hpp>
#include <tc/common/Common.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mini/ini.h>
#include <list>

namespace tc::server::db {

using namespace mINI;

class Client : public tc::LogI
{
public:

	Client(std::string &uri);
	virtual ~Client() = default;

	bool load(INIStructure &ini);

	mongocxx::client &client();

	const std::string name() const;
	std::string collection(const std::string key) const;
	const bool enabled() const;

private:
	mongocxx::client iClient;
	std::string iName;
	bool iEnabled;
	std::map<std::string, std::string> iCollection;
};

} // namespace tc::server::db

#endif /* D636C7F7_D6B6_414B_B468_0498319874FF */
