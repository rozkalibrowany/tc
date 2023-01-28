#ifndef C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF
#define C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF

#include <tc/common/Common.h>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <mini/ini.h>
#include <list>

namespace tc::db::mongo {

using namespace mINI;

class Client : public tc::LogI
{
public:

	Client(std::string &uri);

	result_t load(INIStructure &ini);

	result_t insert(const std::string &json_doc);
	result_t insert(const std::string &json_doc, const std::string &coll);

	std::string collection(const std::string key) const;
	const bool enabled() const;

private:
	bool iEnabled;
	std::string iName;
	std::map<std::string, std::string> iCollection;
};

} // namespace tc::db::mongo

#endif /* C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF */
