#ifndef F2B3EF93_9529_48E8_9906_FDF0C5C0AABD
#define F2B3EF93_9529_48E8_9906_FDF0C5C0AABD

#include <tc/parser/Packet.h>
#include <tc/parser/Json.h>

namespace tc::parser::omni {

#define OMNI_HEADER 0x2a // '*'

class Payload : public Packet, public JsonI
{
	static constexpr size_t IMEI_SIZE = 15;

public:
	static bool valid(const uchar* buf, size_t size);
	static size_t index_of_nth(const std::string_view &str, char delim, unsigned n);
	static result_t parseImei(const std::string_view &buf, Imei &imei);

	result_t parse(const std::string_view &buffer);
	result_t parse(const uchar* cbuf, size_t size, size_t offset = 0) override;

	const size_t size() override;

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override;
	result_t fromJsonImpl(const Json::Value &rhs, bool root) override;

private:
	result_t parseHeader(const std::string_view &str);
};

} // namespace tc::parser::omni

#endif /* F2B3EF93_9529_48E8_9906_FDF0C5C0AABD */
