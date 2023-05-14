#ifndef AA54C7CA_F575_4981_B868_1E85BAF10FBB
#define AA54C7CA_F575_4981_B868_1E85BAF10FBB

#include <tc/common/Buf.h>
#include <tc/common/Logger.h>
#include <tc/parser/Protocol.h>

namespace tc::parser {

class CommandI : public tc::LogI
{
public:
	static constexpr uchar c_id = 0x5;
	static bool hasCommand(const uchar* cbuf, size_t size);
	static Protocol::Type getProtocol(const uchar* cbuf, size_t size);

	CommandI();
	CommandI(const Imei& imei);
	CommandI(const uchar* cbuf, size_t size);
	CommandI(const common::Buf &buf, const Imei& imei);
	virtual ~CommandI() = default;

	virtual result_t parse(const uchar* cbuf, size_t size) = 0;
	virtual const size_t size() const;
	virtual common::Buf buf() const;
	virtual Imei imei() const;

protected:
	virtual void create(const std::string &cmd, const std::string& imei, Protocol::Type type = Protocol::eUnknown);

	common::Buf iBuf;
	Imei iImei;
};

} // namespace tc::parser


#endif /* AA54C7CA_F575_4981_B868_1E85BAF10FBB */
