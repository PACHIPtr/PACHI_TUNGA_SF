#include "stdafx.h"
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "locale_service.h"
#include <stdlib.h>
#include <sstream>
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "char_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "mob_manager.h"
#include "desc_client.h"
#include "group_text_parse_tree.h"
#include <cctype>
#include "p2p.h"
#include "entity.h"
#include "sectree_manager.h"
#include "regen.h"
#include "boss_tracking.h"

CBossTracking::CBossTracking()
{
	m_bossTrackingVector.resize(6);
}

CBossTracking::~CBossTracking()
{
}

DWORD CBossTracking::GetDeadTime(BYTE channel, DWORD dwMobVnum)
{
	TBossTracking& m_memory = m_bossTrackingVector[channel];
	if (m_memory.mob_vnum == dwMobVnum)
		return m_memory.dead_time;
	return 0;
}

DWORD CBossTracking::GetRegenTime(BYTE channel, DWORD dwMobVnum)
{
	TBossTracking& m_memory = m_bossTrackingVector[channel];
	if (m_memory.mob_vnum == dwMobVnum)
		return m_memory.regen_time;
	return 0;
}

void CBossTracking::SetDeadTime(BYTE channel, DWORD dwMobVnum, DWORD dwTime)
{
	TBossTracking& m_memory = m_bossTrackingVector[channel];
	m_memory.mob_vnum = dwMobVnum;
	m_memory.dead_time = dwTime;
}

void CBossTracking::SetRegenTime(BYTE channel, DWORD dwMobVnum, DWORD dwTime)
{
	TBossTracking& m_memory = m_bossTrackingVector[channel];
	m_memory.mob_vnum = dwMobVnum;
	m_memory.regen_time = dwTime;
}

bool CBossTracking::IsTrackingMob(DWORD dwMobVnum)
{
	switch (dwMobVnum)
	{
	case 691:
	case 791:
	case 1304:
	case 2492:
	case 2494:
	case 2495:
	case 2091:
	case 3972:
	case 2191:
	case 2306:
	case 1901:
	case 3090:
	case 3091:
	case 3290:
	case 3291:
	case 3590:
	case 3591:
	case 3490:
	case 3491:
	case 3690:
	case 3691:
	case 3890:
	case 3891:
	case 3390:
	case 3391:
	case 3595:
	case 3596:
	case 3790:
	case 3791:
	case 3190:
	case 3191:
	case 6407:
	case 2291:
	case 1192:
	case 2206:
		return true;
	}

	return false;
}

void CBossTracking::SendP2PPacket(DWORD dwMobVnum)
{
	TPacketGGBossTracking p;
	p.header = HEADER_GG_BOSS_TRACKING;
	p.dead_time = CBossTracking::instance().GetDeadTime(g_bChannel, dwMobVnum);
	p.regen_time = CBossTracking::instance().GetRegenTime(g_bChannel, dwMobVnum);
	p.channel = g_bChannel;
	p.mob_vnum = dwMobVnum;
	P2P_MANAGER::instance().Send(&p, sizeof(p));
}

void CBossTracking::SendClientPacket(LPCHARACTER pkChr, BYTE channel, DWORD dwMobVnum)
{
	if (!pkChr)
		return;

	if (!pkChr->GetDesc())
		return;

	if (CBossTracking::instance().IsTrackingMob(dwMobVnum) == false)
		return;

	if (channel < 1 || channel > 6)
		return;

	TPacketGCBossTracking p;
	p.header = HEADER_GC_BOSS_TRACKING;
	p.dead_time = CBossTracking::instance().GetDeadTime(channel, dwMobVnum);
	p.regen_time = CBossTracking::instance().GetRegenTime(channel, dwMobVnum);
	p.channel = channel;
	p.mob_vnum = dwMobVnum;
	pkChr->GetDesc()->Packet(&p, sizeof(p));
}