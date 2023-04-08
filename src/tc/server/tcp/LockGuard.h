#ifndef B20D0C37_9479_48B7_B04F_8F773C91C6B2
#define B20D0C37_9479_48B7_B04F_8F773C91C6B2

#include <tc/common/SysMutex.h>

namespace tc {

class SysMutex;

class LockGuard
{
public:
	LockGuard(SysMutex &mutex, bool aquire = true)
		: iMutex(mutex)
	{
		if (aquire == true) {
			iMutex.lock();
		}
	}

	~LockGuard()
	{
		iMutex.unlock();
	}

private:
	SysMutex &iMutex;

	LockGuard &operator=(const LockGuard &);
	LockGuard(const LockGuard &);
};

} // namespace tc

#endif /* B20D0C37_9479_48B7_B04F_8F773C91C6B2 */
