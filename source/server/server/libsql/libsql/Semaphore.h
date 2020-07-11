#pragma once
#include <semaphore.h>

class CSemaphore
{
private:
	sem_t* m_hSem;

public:
	CSemaphore();
	~CSemaphore();

	int Initialize();
	void Clear();
	void Destroy();
	int Wait();
	int Release(int count = 1);
};