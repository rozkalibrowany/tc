#include <tc/db/Access.h>

namespace tc::db::mongo {

Access::Access(mongocxx::client& client, std::string dbName, std::string collName)
 : iClient(client)
 , iDbName(dbName)
 , iCollectionName(collName)
{
	iDb = iClient[dbName];
	iCollection = iDb[collName];
}

result_t Access::insert(std::string jsonDoc) {
	try {
		// Convert JSON data to document
		auto doc_value = bsoncxx::from_json(jsonDoc);
		auto result = iCollection.insert_one(std::move(doc_value));
	} catch(const mongocxx::bulk_write_exception &e) {
		LG_ERR(this->logger(), "Error on insert data to DB, {}", e.what());
		return RES_NOENT;
	}	catch(const bsoncxx::exception &e) {
		LG_ERR(this->logger(), "Error converting JSON, {}", e.what());
		return RES_NOENT;
  }

	return RES_OK;
}

result_t Access::insert(const bsoncxx::document::value &val)
{
	try {
		auto result = iCollection.insert_one(val.view());
	} catch(const mongocxx::bulk_write_exception &e) {
		LG_ERR(this->logger(), "Error on insert data to DB, {}", e.what());
		return RES_NOENT;
	}

	return RES_OK;
}

} // namespace tc::db::mongo
