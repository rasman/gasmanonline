
#ifndef SEMAPHOREHOLDER_H
#define SEMAPHOREHOLDER_H

#include <QSemaphore>

/////////////////////////////////////////////////////////////////////////////
// Recursion blocking class -- Acquires a semaphore on creation and
// releases it on destruction (if it is available). Use by creating
// SemaphoreHolder on the stack and checking member IsAcquired()
//
class SemaphoreHolder
{
public:
	SemaphoreHolder(QSemaphore *ps)
		: m_pSem(ps)
		, m_acquired( ps->tryAcquire() )
	{}
	~SemaphoreHolder() { if (m_acquired) m_pSem->release(); }
	inline bool IsAcquired() { return m_acquired; }
private:
	QSemaphore *m_pSem;
	bool m_acquired;
};

#endif