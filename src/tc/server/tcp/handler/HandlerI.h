#ifndef B1855F8A_B229_42C5_9F3B_01E62F4EA697
#define B1855F8A_B229_42C5_9F3B_01E62F4EA697

#include <tc/common/Common.h>
#include <tc/iot/Device.h>

namespace tc::server::tcp {

class TelematicsSession;

class HandlerI : public tc::LogI {
	friend class TelematicsSession;
public:

	explicit HandlerI(const std::shared_ptr< TelematicsSession > &session)
	 : iSession(session) {}

	virtual ~HandlerI() = default;

	HandlerI(const HandlerI&) = delete;
	HandlerI(HandlerI&&) = delete;

	virtual result_t handle(const uchar *buffer, size_t size) = 0;
	virtual const Imei imei() const { return iImei; }

protected:
	std::mutex iMutex;
	Imei iImei{"unknown"};
	std::shared_ptr<TelematicsSession> iSession;
	std::unique_ptr<iot::Device> iDevice{nullptr};
};

} // namespace tc::server::tcp

#endif /* B1855F8A_B229_42C5_9F3B_01E62F4EA697 */
