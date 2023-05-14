#ifndef B1855F8A_B229_42C5_9F3B_01E62F4EA697
#define B1855F8A_B229_42C5_9F3B_01E62F4EA697

#include <tc/common/Common.h>
#include <tc/iot/Device.h>
#include <tc/parser/Protocol.h>
namespace tc::server::tcp {

class TelematicsSession;

class HandlerI : public parser::Protocol, public parser::JsonI {
	friend class TelematicsSession;
public:
	using parser::Protocol::Protocol;

	HandlerI(const std::shared_ptr< TelematicsSession > &session, Protocol protocol = eUnknown)
	 : Protocol(protocol)
	 , iSession(session) {}

	virtual ~HandlerI() = default;

	HandlerI(const HandlerI&) = delete;
	HandlerI(HandlerI&&) = delete;

	virtual result_t handle(const uchar *buffer, size_t size) = 0;
	virtual const Imei imei() const { return iImei; }

protected:
	result_t toJsonImpl(Json::Value &rhs, bool root) const override
	{
		if(!iDevice) return RES_NOENT;
		return iDevice->toJson(rhs, root);
	}

	std::mutex iMutex;
	Imei iImei{"unknown"};
	std::shared_ptr<TelematicsSession> iSession;
	std::unique_ptr<iot::Device> iDevice{nullptr};
};

} // namespace tc::server::tcp

#endif /* B1855F8A_B229_42C5_9F3B_01E62F4EA697 */
