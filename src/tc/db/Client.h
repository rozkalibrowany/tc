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
	result_t create(const std::string &coll_name);
	result_t create(const std::string &coll_name, const std::string &db_name);
	result_t get(const std::string &id, bsoncxx::document::view &doc);

	bool has(const std::string &coll_name);
	bool has(const std::string &coll_name, const std::string &db_name);

	const std::string collection() const;
	const std::string name() const;
	const bool enabled() const;

private:
	bool iEnabled;
	std::string iName;
	std::string iCollection;
};

} // namespace tc::db::mongo

#endif /* C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF */
