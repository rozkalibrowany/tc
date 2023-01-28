#ifndef EA4A817F_A54B_417D_8A01_900A95EFF3FE
#define EA4A817F_A54B_417D_8A01_900A95EFF3FE

#include <tc/db/Access.h>

namespace tc::db::mongo {

class Thread {
public:
	Thread(mongocxx::client &client, std::string db_name, std::string coll_name, std::string json_doc);

	void operator()();
	
private:
  Access iAccess;
  std::string iJson;
};

} // namespace tc::db::mongo

#endif /* EA4A817F_A54B_417D_8A01_900A95EFF3FE */
