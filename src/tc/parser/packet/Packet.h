#ifndef E2EB797D_BF96_49F2_8076_6FCE70519A6C
#define E2EB797D_BF96_49F2_8076_6FCE70519A6C

#include <tc/parser/Buf.h>
#include <tc/common/Common.h>
#include <tc/parser/Reader.h>

namespace tc::parser {

class Packet : public tc::LogI
{
public:

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

	virtual bool operator<(const Packet &rhs) const;
	virtual bool operator<=(const Packet &rhs) const;
	virtual bool operator>(const Packet &rhs) const;
	virtual bool operator>=(const Packet &rhs) const;
	virtual bool operator==(const Packet &rhs) const;

	virtual result_t parse(uchar *cbuf, size_t size) = 0;

	virtual int codec() const;
	virtual int totalRecords() const;
	virtual const ID &id() const;

protected:
	ID iID;
	int iCodec;
	int iTotalRecords;
};

} // namespace tc::parser


#endif /* E2EB797D_BF96_49F2_8076_6FCE70519A6C */
