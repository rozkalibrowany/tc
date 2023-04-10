#ifndef B3F010B2_E528_4C0F_B6A0_FB3C7AD71E46
#define B3F010B2_E528_4C0F_B6A0_FB3C7AD71E46

#include <tc/iot/Device.h>
#include <tc/common/Buf.h>
#include <tc/server/tcp/handler/HandlerI.h>

namespace tc::server::tcp {

class TelematicsSession;

class TeltonikaHandler : public HandlerI {
	friend class TelematicsSession;
public:
	enum Response {
		eInvalid = 0,
		eOK = 1
	};

	explicit TeltonikaHandler(const std::shared_ptr< TelematicsSession > &session);
	TeltonikaHandler(const TeltonikaHandler&) = delete;
	TeltonikaHandler(TeltonikaHandler&&) = delete;

	TeltonikaHandler& operator=(const TeltonikaHandler&) = delete;
	TeltonikaHandler& operator=(TeltonikaHandler&&) = delete;

	result_t handle(const uchar* buffer, size_t size) override;

private:
	result_t handleImei(const uchar *buffer, size_t size);
	result_t handlePayload(const uchar *buffer, size_t size);
	result_t handleStandby(const uchar *buffer, size_t size);
	result_t handleIncomplete(const uchar *buffer, size_t size);

	std::shared_ptr<common::Buf> iBufferIncomplete{nullptr};
};

} // namespace tc::server::tcp

#endif /* B3F010B2_E528_4C0F_B6A0_FB3C7AD71E46 */
