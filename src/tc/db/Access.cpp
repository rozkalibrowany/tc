#include <tc/db/Access.h>
#include <libbson-1.0/bson.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <mongocxx/exception/logic_error.hpp>

namespace tc::db::mongo {

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;

Access::Access(mongocxx::client& client, std::string dbName, std::string collName, Mode mode)
 : iClient(client)
 , iDbName(dbName)
 , iCollectionName(collName)
 , iMode(mode)
{
	iDb = iClient[dbName];
	iCollection = iDb[collName];
}

/**
 * It converts the JSON string to a BSON document, and then inserts it into the database
 * @param jsonDoc The JSON document to be inserted into the database.
 * @return The result of the insert operation.
 */
result_t Access::insert(const std::string &jsonDoc) {
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

/**
 * It finds a document in the database, and returns it as a JSON string
 * @param imei The IMEI of the device to be searched.
 * @param json_doc The JSON document to be inserted into the database.
 * @return A JSON document.
 */
result_t Access::find_one(const std::string &imei, std::string &json_doc)
{
	try {
		auto doc = document{} << "Imei" << imei << finalize;
		auto value = iCollection.find_one(doc.view());
		auto doc_value = bsoncxx::to_json(value.value());
		json_doc = doc_value;
	} catch(const mongocxx::query_exception &e) {
		LG_ERR(this->logger(), "Error on find data in DB, {}", e.what());
		return RES_NOENT;
	}	catch(const bsoncxx::exception &e) {
		LG_ERR(this->logger(), "Error converting JSON, {}", e.what());
		return RES_NOENT;
  }

	return RES_OK;
}

/**
 * It updates the value of the key in the database
 * @param key the key to update
 * @param old the old value of the key
 * @param val the value to be updated
 * @return RES_OK
 */
result_t Access::update(const std::string &key, const int64_t old, const int64_t val)
{
	try {
		iCollection.update_one(document{} << key << old << finalize, document{} << "" << open_document <<
			"i" << val << close_document << finalize);
	} catch(const mongocxx::logic_error &e) {
		LG_ERR(this->logger(), "Error on update data in DB, {}", e.what());
		return RES_NOENT;
	}

	return RES_OK;
}

/**
 * It updates the value of the key in the database
 * @param key the key to update
 * @param old the old value of the key
 * @param val the value to be updated
 * @return RES_OK
 */
result_t Access::update(const std::string &key, const std::string &old, const std::string &val)
{
	try {
		iCollection.update_one(document{} << key << old << finalize, document{} << "" << open_document <<
			"i" << val << close_document << finalize);
	} catch(const mongocxx::logic_error &e) {
		LG_ERR(this->logger(), "Error on update data in DB, {}", e.what());
		return RES_NOENT;
	}
	
	return RES_OK;
}

} // namespace tc::db::mongo
