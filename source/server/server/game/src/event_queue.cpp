/*
 *    Filename: queue.c
 * Description: Å¥ Ã³¸®
 *
 *      Author: ±èÇÑÁÖ (aka. ºñ¿±, Cronan), ¼Û¿µÁø (aka. myevan, ºøÀÚ·ç)
 */
#include "stdafx.h"

#include "event_queue.h"

CEventQueue::CEventQueue()
{
}

CEventQueue::~CEventQueue()
{
	Destroy();
}

void CEventQueue::Destroy()
{
	while (!m_pq_queue.empty())
	{
		TQueueElement* pElem = m_pq_queue.top();
		m_pq_queue.pop();

		Delete(pElem);
	}
}

TQueueElement* CEventQueue::Enqueue(LPEVENT pvData, int duration, int pulse)
{
	TQueueElement* pElem = M2_NEW TQueueElement;
	pElem->pvData = pvData;
	pElem->iStartTime = pulse;
	pElem->iKey = duration + pulse;
	pElem->bCancel = FALSE;

	m_pq_queue.push(pElem);
	return pElem;
}

TQueueElement* CEventQueue::Dequeue()
{
	if (m_pq_queue.empty())
		return nullptr;

	TQueueElement* pElem = m_pq_queue.top();
	m_pq_queue.pop();
	return pElem;
}

void CEventQueue::Delete(TQueueElement* pElem)
{
	M2_DELETE(pElem);
}

int CEventQueue::GetTopKey()
{
	if (m_pq_queue.empty())
		return INT_MAX;

	return m_pq_queue.top()->iKey;
}

int CEventQueue::Size()
{
	return m_pq_queue.size();
}