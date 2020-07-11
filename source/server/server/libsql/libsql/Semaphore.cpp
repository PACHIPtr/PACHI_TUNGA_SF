#include "stdafx.h"
#include "Semaphore.h"

CSemaphore::CSemaphore() : m_hSem(NULL)
{
	Initialize();
}

CSemaphore::~CSemaphore()
{
	Destroy();
}

int CSemaphore::Initialize()
{
	Clear();

	m_hSem = new sem_t;

	if (sem_init(m_hSem, 0, 0) == -1)
	{
		perror("sem_init");
		return false;
	}
	return true;
}

void CSemaphore::Destroy()
{
	Clear();
}

void CSemaphore::Clear()
{
	if (m_hSem)
	{
		sem_destroy(m_hSem);
		delete m_hSem;
	}
	m_hSem = NULL;
}

int CSemaphore::Wait()
{
	if (!m_hSem)
	{
		return true;
	}

	int re = sem_wait(m_hSem);

	if (re == -1)
	{
		if (errno == EINTR)
		{
			return Wait();
		}

		perror("sem_wait");
		return false;
	}
	return true;
}

int CSemaphore::Release(int count)
{
	if (!m_hSem)
	{
		return false;
	}

	for (int i = 0; i < count; ++i)
	{
		sem_post(m_hSem);
	}

	return true;
}