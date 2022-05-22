#ifndef DB3FB3EA_312B_4E59_A539_314B2EF90A10
#define DB3FB3EA_312B_4E59_A539_314B2EF90A10

#include <tc/parser/Packet.h>

namespace tc::server::data::iot {

using namespace parser;

class PacketEntries;
using PacketEntriesSPtr = std::shared_ptr<PacketEntries>;

class PacketEntries
{
public:
	PacketEntries(const Parser::imei_t &imei);
	PacketEntries(const std::string &imei, size_t max_size = 5000);
	~PacketEntries() = default;

	result_t add(Packet &packet);
	result_t get(const Packet &id, Packet &packet) const;
	result_t get(const Packet::PacketID &id, Packet &packet) const;
	result_t remove(const Packet &id);
	result_t remove(const Packet::PacketID &id);
	result_t contains(const Packet &id) const;
	result_t contains(const Packet::PacketID &id) const;

	const size_t size() const;
};

} // namespace tc::iot::device

#endif /* DB3FB3EA_312B_4E59_A539_314B2EF90A10 */
