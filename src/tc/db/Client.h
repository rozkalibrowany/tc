#ifndef C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF
#define C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF

#include <tc/common/Common.h>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <optional>
#include <mini/ini.h>
#include <json/json.h>

namespace tc::db::mongo {

using namespace mINI;

class Client : public tc::LogI
{
public:
	enum CollType
	{
		eDevices = 0,
		ePackets
	};

	struct Collection{
		std::string iName;
		SysTime iTime;
		CollType iType;

		Collection(const std::string &name, CollType type, bool now)
			: iName(name)
			, iTime(now)
			, iType(type) {}

		Collection(const std::string &name, CollType type, int64_t timestamp)
			: iName(name)
			, iTime(timestamp)
			, iType(type) {}

		Collection(const std::string &name, CollType type)
			: Collection(name, type, true) {}

		Collection(CollType type = ePackets)
			: Collection(type == ePackets ? std::string{"Packets"} : std::string{"Devices"}, type, true) {}

		~Collection() {}

		operator std::string() const {
			if (iType == ePackets)
				return fmt::format("{}_{}_{}_{}", iName, iTime.getDay(), iTime.getMonth(), iTime.getYear());
			return iName;
		}
	};

	Client(CollType type);

	result_t load(INIStructure &ini);

	void init(const std::string &uri);
	void synchronizeTime(int64_t timestamp);

	/* Not thread safe */
	result_t get(const std::string &imei, std::string &json_doc);
	std::optional< mongocxx::cursor > getCursor();

	result_t insert(const std::string &json_doc);

	result_t create(const std::string &coll_name);
	result_t create(const std::string &coll_name, const std::string &db_name);

	result_t update(const std::string &key, const std::string &old, const std::string &val);
	result_t update(const std::string &key, const int64_t old, const int64_t val);

	result_t replace(const std::string &json_old, const std::string &json_new);

	result_t drop(const std::string &collection);
	void setCollection(const std::string &coll);

	bool has(const std::string &coll_name);
	bool has(const std::string &coll_name, const std::string &db_name);
	bool hasImei(const Imei &imei);

	Collection collection();
	std::string collection_str();
	const std::string name() const;
	const bool enabled() const;

private:
	bool iEnabled {false};
	std::string iName;
	Collection iCollection;
};

} // namespace tc::db::mongo

#endif /* C6AD8F6C_7B7E_49E7_B0FB_E6CDD1C06BAF */
