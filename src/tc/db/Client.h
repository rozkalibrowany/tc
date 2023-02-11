#ifndef C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF
#define C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF

#include <tc/common/Common.h>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <mini/ini.h>
#include <json/json.h>

namespace tc::db::mongo {

using namespace mINI;

class Client : public tc::LogI
{
public:

	Client(std::string &uri);

	result_t load(INIStructure &ini);

	/* Not thread safe */
	result_t get(const std::string &imei, std::string &json_doc);
	result_t insert(const std::string &json_doc);

	result_t create(const std::string &coll_name);
	result_t create(const std::string &coll_name, const std::string &db_name);
	
	result_t update(const std::string &key, const std::string &old, const std::string &val);
	result_t update(const std::string &key, const int64_t old, const int64_t val);

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
