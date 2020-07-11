#include "stdafx.h"
#include "config.h"

#include "minigame.h"

#include "../../common/length.h"
#include "../../common/tables.h"
#include "p2p.h"
#include "locale_service.h"
#include "char.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "questmanager.h"
#include "questlua.h"
#include "start_position.h"
#include "char_manager.h"
#include "item_manager.h"
#include "sectree_manager.h"
#include "regen.h"
#include "log.h"
#include "db.h"
#include "target.h"
#include "party.h"

void CMiniGame::ShadowWarriorMonsterAttack(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	if (pkAttacker == nullptr || pkVictim == nullptr)
		return;

	if (!pkAttacker->GetDesc())
		return;

	if (pkVictim->GetRaceNum() >= 6420 && pkVictim->GetRaceNum() <= 6421)
	{
		bool bContain = false;
		DWORD dwCount = 1;

		if (!pkAttacker->m_hitCount.empty())
		{
			for (DWORD i = 0; i < pkAttacker->m_hitCount.size(); i++)
			{
				if (pkAttacker->m_hitCount[i].dwVid == pkVictim->GetVID())
				{
					bContain = true;
					pkAttacker->m_hitCount[i].dwCount += 1;
					dwCount = pkAttacker->m_hitCount[i].dwCount;
					break;
				}
			}
		}

		if (!bContain)
		{
			pkAttacker->m_hitCount.push_back(THitCountInfo(pkVictim->GetVID(), dwCount));
		}

		TPacketGCHitCountInfo packHitCount;
		packHitCount.bHeader = HEADER_GC_HIT_COUNT_INFO;
		packHitCount.dwVid = pkVictim->GetVID();
		packHitCount.dwCount = dwCount;
		pkAttacker->GetDesc()->Packet(&packHitCount, sizeof(TPacketGCHitCountInfo));
		pkAttacker->PointChange(POINT_YEAR_ROUND, 1, false);
	}
}