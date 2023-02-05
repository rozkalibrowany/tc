#ifndef E4EF0587_628B_4A0B_AB56_828E1C63D31C
#define E4EF0587_628B_4A0B_AB56_828E1C63D31C

#include <tc/common/Common.h>
#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>

namespace tc::db::mongo {

class Access : public tc::LogI {
public:
  Access(mongocxx::client& client, std::string dbName, std::string collName);

  result_t insert(std::string jsonDoc);
	result_t insert(const bsoncxx::document::value &val);

private:
  mongocxx::client& iClient;
  std::string iDbName;
  std::string iCollectionName;
  mongocxx::database iDb;
  mongocxx::collection iCollection;
};

} // namespace tc::db::mongo

#endif /* E4EF0587_628B_4A0B_AB56_828E1C63D31C */
