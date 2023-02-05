#include <tc/db/Access.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <libbson-1.0/bson.h>

namespace tc::db::mongo {

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

Access::Access(mongocxx::client& client, std::string dbName, std::string collName, Mode mode)
 : iClient(client)
 , iDbName(dbName)
 , iCollectionName(collName)
 , iMode(mode)
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

result_t Access::find_one(const std::string &id, bsoncxx::document::view &view)
{
	bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = iCollection.find_one(document{} << "Imei" << id << finalize);

	if(maybe_result) {
		view = maybe_result->view();
		return RES_OK;
	}
	return RES_NOENT;
}

} // namespace tc::db::mongo
