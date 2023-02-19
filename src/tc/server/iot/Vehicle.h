#ifndef AC5BB633_C3DF_423C_9202_CD95401C79B9
#define AC5BB633_C3DF_423C_9202_CD95401C79B9

#include <tc/server/iot/Device.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

namespace tc::server::iot {

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;

class Vehicle : public Device
{
public:
	enum Source
	{
		eDatabase = 0,
		eTelematics
	};

	static result_t fromJsonString(const std::string &json_doc, Json::Value &rhs);

	explicit Vehicle(const Imei &imei);
	explicit Vehicle(const Imei &imei, size_t cache);
	explicit Vehicle(const Imei &imei, size_t cache, Source source);

	bool operator==(const Device &rhs) const;
	bool operator!=(const Device &rhs) const;
	Vehicle &operator=(const Vehicle &rhs);

	bool has(const std::shared_ptr<parser::PacketPayload> packet) override;
	result_t add(const uchar *buffer, size_t size) override;
	result_t add(const std::shared_ptr<parser::PacketPayload> packet) override;

	result_t set(const std::string &json_doc);

	const std::string id() const;
	const std::string fleet() const;
	const Source source() const;

	void setID(const std::string &id);
	void setFleet(const std::string &id);

protected:
	result_t fromJsonImpl(const Json::Value &rhs, bool active) override;
	result_t toJsonImpl(Json::Value &rhs, bool active_only = true) const override;

	void updateModified(int64_t timestamp);

	std::string iID {"unknown"};
	std::string iFleet {"unknown"};
	int64_t iModified {timestamp()};
	Source iSource {eTelematics};
};

} // namespace tc::server::iot


#endif /* AC5BB633_C3DF_423C_9202_CD95401C79B9 */
