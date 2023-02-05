#ifndef AC5BB633_C3DF_423C_9202_CD95401C79B9
#define AC5BB633_C3DF_423C_9202_CD95401C79B9

#include <tc/server/iot/Device.h>
#include <bsoncxx/builder/stream/document.hpp>

namespace tc::server::iot {

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

class Vehicle : public Device
{
	public:
		Vehicle(const Imei &imei);
		Vehicle(const Imei &imei, size_t cache);

		bool operator==(const Device &rhs) const;
		bool operator!=(const Device &rhs) const;
		Vehicle &operator=(const Vehicle &rhs);

		bool has(const std::shared_ptr<parser::PacketPayload> packet) override;
		result_t add(const uchar *buffer, size_t size) override;
		result_t add(const std::shared_ptr<parser::PacketPayload> packet) override;

		result_t updateDeviceInfo(const bsoncxx::document::view &view);

		const std::string id() const;
		const std::string fleet() const;

		void setID(const std::string &id);
		void setFleet(const std::string &id);

	protected:
		result_t fromJsonImpl(const Json::Value &rhs, bool root) override;
		result_t toJsonImpl(Json::Value &rhs, bool root) const override;

		std::string iID {"unknown"};
		std::string iFleet {"unknown"};
};

} // namespace tc::server::iot


#endif /* AC5BB633_C3DF_423C_9202_CD95401C79B9 */
