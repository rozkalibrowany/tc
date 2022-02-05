#include <tc/common/SysMutex.h>

namespace tc {

SysMutex::SysMutex()
{
	pthread_mutex_init(&iMutex, NULL);
}

SysMutex::~SysMutex()
{
	pthread_mutex_destroy(&iMutex);
}

result_t SysMutex::lock(int timeout)
{
	if (timeout < 0) {
		pthread_mutex_lock(&iMutex);
		return RES_OK;
	}

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += (timeout % 1000) * 1000000;
	ts.tv_sec += timeout / 1000 + ts.tv_nsec / 1000000000;
	ts.tv_nsec %= 1000000000;

	if (pthread_mutex_timedlock(&iMutex, &ts) == ETIMEDOUT)
		return RES_TIMEOUT;

	return RES_OK;
}

result_t SysMutex::unlock()
{
	pthread_mutex_unlock(&iMutex);

	return RES_OK;
}

} // namespace tc
