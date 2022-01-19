#ifndef E1382A91_4A19_490A_9DB0_1B2CEB94E659
#define E1382A91_4A19_490A_9DB0_1B2CEB94E659

#include <tc/parser/reader/Buf.h>
#include <tc/common/Result.h>
#include <tc/common/SysTime.h>
#include <tc/common/Logger.h>
#include <tc/parser/records/avl/AVLRecord.h>
#include <tc/parser/reader/Reader.h>
#include <string>

namespace tc::parser {

using namespace reader;
using namespace records;
using namespace avl;

class Packet;
using PacketSPtr = std::shared_ptr<Packet>;

class Packet : common::LogI
{
public:
	static size_t PACKET_DATA_MIN_SIZE;
	static size_t PACKET_IMEI_MIN_SIZE;

	struct ID {
		ID &operator=(const ID &rhs) = default;
		auto operator<=>(const ID &rhs) const = default;

		ID(bool now) : timestamp(SysTime(now)) {}
		ID() : ID(true) {}

		SysTime timestamp;
	};

	Packet(bool time_now = true);
	Packet(const ID &id);
	virtual ~Packet() = default;

	Packet &operator=(const Packet &rhs);
	bool operator<(const Packet &rhs) const;
	bool operator<=(const Packet &rhs) const;
	bool operator>(const Packet &rhs) const;
	bool operator>=(const Packet &rhs) const;
	bool operator==(const Packet &rhs) const;

	result_t parse(Buf &buf);
	result_t parse(unsigned char* cbuf, size_t size);

	ReaderSPtr &reader();

	int codec() const;
	int size() const;

	ID &id();
	std::string imei() const;
	AVLRecords &records();
	SysTime timestamp() const;

private:
	result_t parseImei(Buf &buf);
	bool contains(const Buf &buf, const unsigned char c) const;
	int getIdx(const Buf &buf, const unsigned char c);

	int iCodec{0};
	int iRecordsTotal {0};
	std::string iImei;
	ID iID;
	ReaderSPtr iReader {nullptr};
	AVLRecords iAVLRecords;
};

} // namespace tc::parser

#endif /* E1382A91_4A19_490A_9DB0_1B2CEB94E659 */
