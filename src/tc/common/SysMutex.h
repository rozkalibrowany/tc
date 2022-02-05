#ifndef EAE2FBB8_E9F9_4A24_94A7_E4B9A88A84E0
#define EAE2FBB8_E9F9_4A24_94A7_E4B9A88A84E0

#include <tc/common/Common.h>
#include <mutex>

namespace tc {

class SysMutex
{
public:
	SysMutex();
	virtual ~SysMutex();

	result_t lock(int timeout = -1);
	result_t unlock();

protected:
	pthread_mutex_t iMutex;

private:
	SysMutex(const SysMutex &);
	SysMutex &operator=(const SysMutex &);
};
} // namespace tc

#endif /* EAE2FBB8_E9F9_4A24_94A7_E4B9A88A84E0 */
