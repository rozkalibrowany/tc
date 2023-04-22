#ifndef DBF2D458_842D_4EC9_B03E_D2CBFBBAA79A
#define DBF2D458_842D_4EC9_B03E_D2CBFBBAA79A

#include <tc/parser/Packet.h>
#include <tc/parser/Reader.h>

namespace tc::parser {

class RequestI : public tc::LogI
{
public:

	enum Type {
		eUnknown = 0,
		ePackets,
		eDevices,
		eDevice
  };

	enum Method {
		eNone = 0,
		eHead,
		eGet,
		ePost
	};

	virtual ~RequestI() = default;

	inline bool contains(const uchar *buf, size_t size, uchar c);

	static Type string2type(const std::string &req);
	static std::string_view type2string(Type type);

	virtual result_t parse(const uchar *cbuf, size_t size) = 0;
	virtual size_t size() const = 0;

	virtual Method method() const;
	virtual Type type() const;

protected:
	Method iMethod{eNone};
	Type iType{eUnknown};
	size_t iSize{0};
};

bool RequestI::contains(const uchar* buf, size_t size, uchar c)
{
	auto end = buf + (unsigned long) size;
	auto pos = std::find(buf, end, c);

	return pos != end;
}

} // namespace tc::parser::internal

#endif /* DBF2D458_842D_4EC9_B03E_D2CBFBBAA79A */
