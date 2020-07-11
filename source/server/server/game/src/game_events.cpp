#include "stdafx.h"
#include "game_events.h"
#include "char.h"
#include "packet.h"
#include "desc.h"
#include "quest.h"
#include "questmanager.h"
#include "utils.h"

CGameEventsManager::CGameEventsManager()
{
	for (int i = 0; i < EVENT_MAX_NUM; ++i)
	{
		m_pkActivateEvents[i] = false;
		m_dwEventEndTime[i] = 0;
	}
}

CGameEventsManager::~CGameEventsManager()
{
}

void CGameEventsManager::SendEventCharacter(LPCHARACTER ch)
{
	if (!ch)
		return;

	for (int i = 0; i < EVENT_MAX_NUM; ++i)
	{
		TPacketGCEventInfo pPacket;
		pPacket.bHeader = HEADER_GC_EVENT_INFO;
		pPacket.event_id = i;
		pPacket.isActivate = IsActivateEvent(i);
		pPacket.event_time = GetEventTime(i);
		ch->GetDesc()->Packet(&pPacket, sizeof(TPacketGCEventInfo));
	}
}

void CGameEventsManager::SetActivateEvent(BYTE bEventID, bool isActivate)
{
	if (bEventID > EVENT_MAX_NUM)
		return;

	m_pkActivateEvents[bEventID] = isActivate;
}

void CGameEventsManager::SetEventTime(BYTE bEventID, DWORD eventTime)
{
	if (bEventID > EVENT_MAX_NUM)
		return;
	
	m_dwEventEndTime[bEventID] = get_global_time()+eventTime;
}

bool CGameEventsManager::IsActivateEvent(BYTE bEventID)
{
	return m_pkActivateEvents[bEventID];
}

DWORD CGameEventsManager::GetEventTime(BYTE bEventID)
{
	return m_dwEventEndTime[bEventID];
}