#include "stdafx.h"
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "p2p.h"
#include "boss_manager.h"

const CBossManager::TBossTable* CBossManager::GetTable(DWORD dwVnum)
{
	TBossDataMap::iterator itor = m_BossTableDataMap.find(dwVnum);

	if (itor == m_BossTableDataMap.end())
		return NULL;

	return itor->second;
}

void CBossManager::Dead(DWORD dwBossVnum, DWORD regen_time, DWORD dead_time)
{
	const TBossTable* p = GetTable(dwBossVnum);

	if (p)
	{
		sys_log(0, "Already Inserted List %d (Boss Table)", dwBossVnum);
		return;
	}

	TBossTable* pBossData = new TBossTable;
	pBossData->regen_time = regen_time;
	pBossData->dead_time = dead_time;
	m_BossTableDataMap.insert(TBossDataMap::value_type(dwBossVnum, pBossData));
	// ...
	// we write the notice part...
}

DWORD CBossManager::GetRegenTime(DWORD dwBossVnum)
{
	const CBossManager::TBossTable* c_pTable = GetTable(dwBossVnum);
	if (!c_pTable)
		return 0;

	return c_pTable->regen_time;
}

DWORD CBossManager::GetDeadTime(DWORD dwBossVnum)
{
	const CBossManager::TBossTable* c_pTable = GetTable(dwBossVnum);
	if (!c_pTable)
		return 0;

	return c_pTable->dead_time;
}

const bool CBossManager::IsRegisteredBoss(DWORD vnum)
{
	switch (vnum)
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
	default:
		return false;
	}

	return false;
}

void CBossManager::SendBossInformation(LPCHARACTER pkChr, DWORD dwBossVnum)
{
	if (!pkChr)
		return;

	if (!pkChr->GetDesc())
	{
		sys_err("Player dont have desc !...");
		return;
	}

	if (!IsRegisteredBoss(dwBossVnum))
		return;

	TPacketGCBossData pack;
	pack.header = HEADER_GC_BOSS_DATA;
	pack.regen_time = GetRegenTime(dwBossVnum);
	pack.dead_time = GetDeadTime(dwBossVnum);
	pack.channel = g_bChannel;
	pack.boss_vnum = dwBossVnum;
	pkChr->GetDesc()->Packet(&pack, sizeof(pack));

	// we start p2p part
	TPacketGGBossData pack_p2p;
	pack_p2p.header = HEADER_GG_BOSS_DATA;
	pack_p2p.boss_vnum = dwBossVnum;
	strlcpy(pack_p2p.szName, pkChr->GetName(), sizeof(pack_p2p.szName));
	P2P_MANAGER::instance().Send(&pack_p2p, sizeof(pack_p2p));
}

void CBossManager::Update()
{
	for (TBossDataMap::iterator itor = m_BossTableDataMap.begin(); itor != m_BossTableDataMap.end(); ++itor)
	{
		TBossTable* pTable = itor->second;
		if (pTable)
		{
			DWORD birthTime = pTable->dead_time + pTable->regen_time;
			if (get_global_time() > birthTime)
				m_BossTableDataMap.erase(itor);
		}
	}
}

void CBossManager::Destroy()
{
	for (TBossDataMap::iterator itor = m_BossTableDataMap.begin(); itor != m_BossTableDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_BossTableDataMap.clear();
}

CBossManager::CBossManager()
{
}

CBossManager::~CBossManager()
{
	Destroy();
}