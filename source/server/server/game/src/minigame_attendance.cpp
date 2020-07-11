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

CMiniGame::CMiniGame()
{
	//Initialize();
}

CMiniGame::~CMiniGame()
{
	//Destroy();
}

bool CMiniGame::Initialize()
{
	return true;
}

void CMiniGame::Destroy()
{
	return;
}

void CMiniGame::InitializeAttendance(int iEnable)
{
	if (iEnable)
	{
		SpawnEventNPC(30131);
		StartAttendanceRegen();
	}
	else
	{
		CharacterVectorInteractor i;
		CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30131, i);

		for (CharacterVectorInteractor::iterator it = i.begin(); it != i.end(); it++)
		{
			M2_DESTROY_CHARACTER(*it);
		}
	}
}

void CMiniGame::InitializeEasterEvent(int iEnable)
{
	if (iEnable)
	{
		SpawnEventNPC(30129);
		StartEasterStoneRegen();
	}
	else
	{
		CharacterVectorInteractor i;
		CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30129, i);

		for (CharacterVectorInteractor::iterator it = i.begin(); it != i.end(); it++)
		{
			M2_DESTROY_CHARACTER(*it);
		}
	}
}

void CMiniGame::SpawnEventNPC(DWORD dwVnum)
{
	CharacterVectorInteractor i;
	bool isNpcA1 = false;
	bool isNpcB1 = false;
	bool isNpcC1 = false;
	BYTE bVnumC = 0;

	// { red_x, red_y, yellow_x, yellow_y, blue_x, blue_y }
	DWORD spawnPos[3][6] = {
		{ 59000, 47100, 51700, 74200, 30600, 82800 }, // 30129
		{ 60800, 61700, 59600, 61000, 35700, 74300 }, // 30131
#ifdef ENABLE_MINI_GAME_CATCH_KING
		{ 60800, 62300, 59600, 61400, 35000, 73800 }, // 20506
#endif
	};

	switch (dwVnum)
	{
	case 30129:
		bVnumC = 0;
		break;

	case 30131:
		bVnumC = 1;
		break;
#ifdef ENABLE_MINI_GAME_CATCH_KING
	case 20506:
		bVnumC = 2;
		break;
#endif
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(dwVnum, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end())
		{
			LPCHARACTER tch = *(it++);

			if (tch->GetMapIndex() == 1)
			{
				isNpcA1 = true;
			}
			else if (tch->GetMapIndex() == 21)
			{
				isNpcB1 = true;
			}
			else if (tch->GetMapIndex() == 41)
			{
				isNpcC1 = true;
			}
		}
	}

	if (map_allow_find(1) && !isNpcA1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(1))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, 1, pkMap->m_setting.iBaseX + spawnPos[bVnumC][0], pkMap->m_setting.iBaseY + spawnPos[bVnumC][1], 0, false, 90, true);
		}
	}
	if (map_allow_find(21) && !isNpcB1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(21))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, 21, pkMap->m_setting.iBaseX + spawnPos[bVnumC][2], pkMap->m_setting.iBaseY + spawnPos[bVnumC][3], 0, false, 110, true);
		}
	}
	if (map_allow_find(41) && !isNpcC1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(41))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, 41, pkMap->m_setting.iBaseX + spawnPos[bVnumC][4], pkMap->m_setting.iBaseY + spawnPos[bVnumC][5], 0, false, 140, true);
		}
	}
}

void CMiniGame::StartAttendanceRegen()
{
	const char* c_apszRegenNames[4] = {
		"quest/game_api/attendance/nemere_regen.txt",
		"quest/game_api/attendance/razador_regen.txt",
		"quest/game_api/attendance/red_dragon_regen.txt",
		"quest/game_api/attendance/beran_setaou_regen.txt"
	};

	for (int i = 0; i < sizeof(c_apszRegenNames) / sizeof(c_apszRegenNames[0]); i++)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(61 + i))
		{
			char szFileName[256 + 1];
			snprintf(szFileName, sizeof(szFileName), "%s/%s", LocaleService_GetBasePath().c_str(), c_apszRegenNames[i]);
			regen_load(szFileName, 61 + i, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
		}
	}
}

void CMiniGame::StartEasterStoneRegen()
{
	DWORD stoneMaps[19] = {
		1, 3, 21, 23, 41, 43, 61, 62, 63, 64, 65, 67, 68, 69, 70,
		218, 219, 220, 301 // indexurile mapelor beta
	};

	char szFileName10[256 + 1];
	char szFileName20[256 + 1];
	char szFileName30[256 + 1];
	char szFileName40[256 + 1];
	char szFileName50[256 + 1];
	char szFileName60[256 + 1];
	char szFileName70[256 + 1];
	char szFileName80[256 + 1];
	char szFileName90[256 + 1];
	char szFileName95[256 + 1];

	snprintf(szFileName10, sizeof(szFileName10), "%s/quest/game_api/attendance/easter/stone_level_10.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName20, sizeof(szFileName20), "%s/quest/game_api/attendance/easter/stone_level_20.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName30, sizeof(szFileName30), "%s/quest/game_api/attendance/easter/stone_level_30.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName40, sizeof(szFileName40), "%s/quest/game_api/attendance/easter/stone_level_40.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName50, sizeof(szFileName50), "%s/quest/game_api/attendance/easter/stone_level_50.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName60, sizeof(szFileName60), "%s/quest/game_api/attendance/easter/stone_level_60.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName70, sizeof(szFileName70), "%s/quest/game_api/attendance/easter/stone_level_70.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName80, sizeof(szFileName80), "%s/quest/game_api/attendance/easter/stone_level_80.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName90, sizeof(szFileName90), "%s/quest/game_api/attendance/easter/stone_level_90.txt", LocaleService_GetBasePath().c_str());
	snprintf(szFileName95, sizeof(szFileName95), "%s/quest/game_api/attendance/easter/stone_level_95.txt", LocaleService_GetBasePath().c_str());

	for (int i = 0; i < sizeof(stoneMaps) / sizeof(stoneMaps[0]); i++)
	{
		if (map_allow_find(stoneMaps[i]))
		{
			if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(stoneMaps[i]))
			{
				if (stoneMaps[i] == 1 || stoneMaps[i] == 21 || stoneMaps[i] == 41)
				{
					regen_load(szFileName10, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName20, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 3 || stoneMaps[i] == 23 || stoneMaps[i] == 43)
				{
					regen_load(szFileName20, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName30, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 64)
				{
					regen_load(szFileName30, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName40, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName50, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 63 || stoneMaps[i] == 65)
				{
					regen_load(szFileName40, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName50, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 61)
				{
					regen_load(szFileName50, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName60, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 67 || stoneMaps[i] == 62 || stoneMaps[i] == 69)
				{
					regen_load(szFileName60, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName70, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				else if (stoneMaps[i] == 68 || stoneMaps[i] == 70)
				{
					regen_load(szFileName70, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName80, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
				// Acest else if de la sfarsit e pentru mapele beta
				else if (stoneMaps[i] == 218 || stoneMaps[i] == 219 || stoneMaps[i] == 220 || stoneMaps[i] == 301)
				{
					regen_load(szFileName90, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
					regen_load(szFileName95, stoneMaps[i], pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY);
				}
			}
		}
	}
}

void CMiniGame::AttendanceMonsterAttack(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	if (pkAttacker == nullptr || pkVictim == nullptr)
		return;

	if (!pkAttacker->GetDesc())
		return;

	if (pkAttacker->GetRaceNum() >= 6415 && pkAttacker->GetRaceNum() <= 6419)
	{
		if (pkVictim->IsPC())
		{
			if (!pkVictim->FindAffect(AFFECT_ATT_SPEED_SLOW))
			{
				pkVictim->AddAffect(AFFECT_ATT_SPEED_SLOW, POINT_ATT_SPEED, -20, 0, 60, 0, false);
			}
		}
	}

	if (pkVictim->GetRaceNum() >= 6415 && pkVictim->GetRaceNum() <= 6419)
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
	}
}

void CMiniGame::AttendanceEventInfo(LPCHARACTER pChar)
{
	if (pChar == nullptr)
		return;

	if (!pChar->GetDesc())
		return;

	time_t iTime; time(&iTime); tm * pTimeInfo = localtime(&iTime);
	char szClearName[32 + 1], szRewardName[32 + 1];
	snprintf(szClearName, sizeof(szClearName), "attendance.clear_day_%d", pTimeInfo->tm_yday);
	snprintf(szRewardName, sizeof(szRewardName), "attendance.reward_day_%d", pTimeInfo->tm_yday);
	bool bIsEnable = quest::CQuestManager::instance().GetEventFlag("enable_attendance_event");
	BYTE bCurrendDay = pTimeInfo->tm_yday - quest::CQuestManager::instance().GetEventFlag("attendance_start_day");

	TPacketGCAttendanceEventInfo packEvent;
	packEvent.bHeader = HEADER_GC_ATTENDANCE_EVENT_INFO;
	packEvent.wSize = sizeof(packEvent) + (sizeof(TRewardItem) * attendanceRewardVec.size());
	packEvent.bIsEnable = bIsEnable;
	packEvent.bDay = bCurrendDay;
	packEvent.bClear = pChar->GetQuestFlag(szClearName);
	packEvent.bReward = pChar->GetQuestFlag(szRewardName);

	pChar->GetDesc()->BufferedPacket(&packEvent, sizeof(packEvent));
	pChar->GetDesc()->Packet(&attendanceRewardVec[0], sizeof(TRewardItem) * attendanceRewardVec.size());
}

void CMiniGame::AttendanceEventRequestReward(LPCHARACTER pChar)
{
	if (pChar == nullptr)
		return;

	if (!pChar->GetDesc())
		return;

	time_t iTime; time(&iTime); tm * pTimeInfo = localtime(&iTime);
	char szClearName[32 + 1], szRewardName[32 + 1];
	snprintf(szClearName, sizeof(szClearName), "attendance.clear_day_%d", pTimeInfo->tm_yday);
	snprintf(szRewardName, sizeof(szRewardName), "attendance.reward_day_%d", pTimeInfo->tm_yday);

	BYTE bCurrendDay = pTimeInfo->tm_yday - quest::CQuestManager::instance().GetEventFlag("attendance_start_day");

	if (!quest::CQuestManager::instance().GetEventFlag("enable_attendance_event"))
	{
		pChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This event is now enabled."));
		return;
	}

	if (pChar->GetQuestFlag(szClearName) == 0)
	{
		pChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need to earn this reward before you get it."));
		return;
	}

	if (pChar->GetQuestFlag(szRewardName))
	{
		pChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You already get your reward for today."));
		return;
	}

	if (pChar->GetLevel() < 30)
	{
		pChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need at least level 30 for this event."));
		return;
	}

	DWORD rewardVnum = 0, rewardCount = 0;
	if (!attendanceRewardVec.empty())
	{
		for (DWORD i = 0; i < attendanceRewardVec.size(); i++)
		{
			if (attendanceRewardVec[i].bDay == (pTimeInfo->tm_yday - quest::CQuestManager::instance().GetEventFlag("attendance_start_day")) + 1)
			{
				rewardVnum = attendanceRewardVec[i].dwVnum;
				rewardCount = attendanceRewardVec[i].dwCount;
				break;
			}
		}
	}

	pChar->SetQuestFlag(szRewardName, 1);

	if (rewardVnum && rewardCount)
	{
		pChar->AutoGiveItem(rewardVnum, rewardCount);

		TPacketGCAttendanceEvent packEvent;
		packEvent.bHeader = HEADER_GC_ATTENDANCE_EVENT;
		packEvent.bType = 3;
		packEvent.bValue = 1;
		pChar->GetDesc()->Packet(&packEvent, sizeof(TPacketGCAttendanceEvent));
	}
}

bool CMiniGame::ReadRewardItemFile(const char* c_pszFileName)
{
	FILE* fp = fopen(c_pszFileName, "r");
	if (!fp)
		return false;

	DWORD bDay;
	DWORD dwVnum;
	DWORD dwCount;

	while (fscanf(fp, "%u\t%u\t%u", &bDay, &dwVnum, &dwCount) != EOF)
	{
		if (!ITEM_MANAGER::instance().GetTable(dwVnum))
		{
			sys_err("No such item (Vnum: %lu)", dwVnum);
			return false;
		}

		TRewardItem tempItem;
		tempItem.bDay = bDay;
		tempItem.dwVnum = dwVnum;
		tempItem.dwCount = dwCount;

		attendanceRewardVec.push_back(tempItem);
	}

	fclose(fp);
	return true;
}