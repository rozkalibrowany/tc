#ifndef EA4A817F_A54B_417D_8A01_900A95EFF3FE
#define EA4A817F_A54B_417D_8A01_900A95EFF3FE

#include <tc/db/Access.h>
#include <tc/common/Logger.h>

namespace tc::db::mongo {

class Thread : public tc::LogI {
public:
	Thread(mongocxx::client& client, std::string db_name, std::string coll_name, const std::string &data);

	void operator()();
	void operator()(const std::string &key, const int64_t old, const int64_t val);
	void operator()(const std::string &key, const std::string &old, const std::string &val);
	void operator()(const std::string &id, bsoncxx::document::view &val, bool &ok);

private:
	Access::Mode iMode{Access::Write};
	Access iAccess;
	std::string iData;
};

} // namespace tc::db::mongo

#endif /* EA4A817F_A54B_417D_8A01_900A95EFF3FE */
