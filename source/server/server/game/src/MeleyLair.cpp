#include "stdafx.h"
#include "MeleyLair.h"

#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "db.h"
#include "log.h"
#include "item.h"
#include "char.h"
#include "utils.h"
#include "party.h"
#include "regen.h"
#include "config.h"
#include "packet.h"
#include "motion.h"
#include "item_manager.h"
#include "guild_manager.h"
#include "start_position.h"
#include "locale_service.h"
#include <boost/lexical_cast.hpp>
#include "questmanager.h"

namespace MeleyLair
{
	int32_t stoneSpawnPos[MOBCOUNT_RESPAWN_STONE_STEP2][2] =
	{
		{ 140, 131 },
		{ 130, 122 },
		{ 121, 128 },
		{ 128, 140 }
	};

	int32_t monsterSpawnPos[MOBCOUNT_RESPAWN_COMMON_STEP][2] =
	{
		{ 140, 131 },
		{ 130, 122 },
		{ 121, 128 },
		{ 128, 140 },
		{ 128, 142 },
		{ 128, 131 },
		{ 135, 130 },
		{ 141, 126 },
		{ 128, 122 },
		{ 117, 127 },
		{ 118, 136 },
		{ 126, 142 }
	};

	struct FNotice
	{
		FNotice(const char* psz) : m_psz(psz) {}
		void operator() (CEntity* ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				CHARACTER* pkChar = (CHARACTER*)ent;
				pkChar->ChatPacket(CHAT_TYPE_NOTICE, "%s", m_psz);
			}
		}

		const char* m_psz;
	};

	struct FExitAndGoTo
	{
		FExitAndGoTo() {};
		void operator()(CEntity* ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				CHARACTER* pkChar = (CHARACTER*)ent;
				if (pkChar->IsPC())
				{
					PIXEL_POSITION posSub = CMgr::instance().GetSubXYZ();
					if (!posSub.x)
						pkChar->WarpSet(EMPIRE_START_X(pkChar->GetEmpire()), EMPIRE_START_Y(pkChar->GetEmpire()));
					else
						pkChar->WarpSet(posSub.x, posSub.y);
				}
			}
		}
	};

	EVENTINFO(r_meleystatues_info)
	{
		bool	bFirst;
		CMgrMap* pMap;
	};

	EVENTFUNC(r_meleystatues_event)
	{
		r_meleystatues_info* pEventInfo = dynamic_cast<r_meleystatues_info*>(event->info);
		if (pEventInfo)
		{
			if (pEventInfo->bFirst)
			{
				CMgrMap* pMap = pEventInfo->pMap;
				if (pMap)
					pMap->DungeonResult();
			}
			else
			{
				pEventInfo->bFirst = true;
				return PASSES_PER_SEC((uint32_t)(MeleyLair::TIME_LIMIT_TO_KILL_STATUE));
			}
		}

		return 0;
	}

	EVENTINFO(r_meleylimit_info)
	{
		bool	bWarp;
		CMgrMap* pMap;
	};

	EVENTFUNC(r_meleylimit_event)
	{
		r_meleylimit_info* pEventInfo = dynamic_cast<r_meleylimit_info*>(event->info);
		if (pEventInfo)
		{
			CMgrMap* pMap = pEventInfo->pMap;
			if (pMap)
			{
				if (pEventInfo->bWarp){
					pMap->EndDungeonWarp();
					quest::CQuestManager::instance().SetEventFlag("meley_lair_dungeon_status", 0);
					quest::CQuestManager::instance().SetEventFlag("meley_lair_dungeon_guild", 0);
				}
				else
					pMap->EndDungeon(false, false);
			}
		}

		return 0;
	}

	EVENTINFO(r_meleyspawn_info)
	{
		CMgrMap* pMap;
		bool	bFirst;
		uint8_t	bStep;
		uint32_t	dwTimeReload, dwMobVnum, dwMobCount;
	};

	EVENTFUNC(r_meleyspawn_event)
	{
		r_meleyspawn_info* pEventInfo = dynamic_cast<r_meleyspawn_info*>(event->info);
		if (!pEventInfo)
			return 0;

		CMgrMap* pMap = pEventInfo->pMap;
		if (!pMap)
			return 0;

		if (!pMap->GetMapSectree())
			return 0;

		uint32_t dwTimeReload = pEventInfo->dwTimeReload, dwMobVnum = pEventInfo->dwMobVnum, dwMobCount = pEventInfo->dwMobCount;
		size_t mob_count = SECTREE_MANAGER::instance().GetMonsterCountInMap(pMap->GetMapIndex(), dwMobVnum, true);
		bool bRespawnCommon = dwMobCount > mob_count ? true : false;
		if (bRespawnCommon)
		{
			uint32_t dwDiff = dwMobCount - mob_count;
			for (uint32_t i = 0; i < dwDiff; ++i)
			{
				uint32_t dwRandom = number(3, 10);
				pMap->Spawn(dwMobVnum, monsterSpawnPos[i][0], monsterSpawnPos[i][1] + dwRandom, 0);
			}
		}

		if ((pEventInfo->bStep == 2) || (pEventInfo->bStep == 3))
		{
			size_t stones_count = SECTREE_MANAGER::instance().GetMonsterCountInMap(pMap->GetMapIndex(), (uint32_t)(MeleyLair::MOBVNUM_RESPAWN_STONE_STEP2), true);
			if (stones_count == 0)
			{
				uint32_t dwTime = get_global_time();
				if (dwTime >= pMap->GetLastStoneKilledTime())
				{
					for (uint32_t i = 0; i < MOBCOUNT_RESPAWN_STONE_STEP2; ++i)
					{
						uint32_t dwRandom = number(3, 10);
						pMap->Spawn((uint32_t)(MeleyLair::MOBVNUM_RESPAWN_STONE_STEP2), stoneSpawnPos[i][0], stoneSpawnPos[i][1] + dwRandom, 0);
					}

					if (pEventInfo->bStep == 3)
					{
						if (pMap->GetStatue1Char() && !pMap->GetStatue1Char()->IsAffectFlag(AFF_STATUE3))
							pMap->GetStatue1Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue2Char() && !pMap->GetStatue2Char()->IsAffectFlag(AFF_STATUE3))
							pMap->GetStatue2Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue3Char() && !pMap->GetStatue3Char()->IsAffectFlag(AFF_STATUE3))
							pMap->GetStatue3Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue4Char() && !pMap->GetStatue4Char()->IsAffectFlag(AFF_STATUE3))
							pMap->GetStatue4Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						pMap->SetKillCountStones(0);
					}
					else
					{
						if (pMap->GetStatue1Char() && !pMap->GetStatue1Char()->IsAffectFlag(AFF_STATUE2))
							pMap->GetStatue1Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue2Char() && !pMap->GetStatue2Char()->IsAffectFlag(AFF_STATUE2))
							pMap->GetStatue2Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue3Char() && !pMap->GetStatue3Char()->IsAffectFlag(AFF_STATUE2))
							pMap->GetStatue3Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

						if (pMap->GetStatue4Char() && !pMap->GetStatue4Char()->IsAffectFlag(AFF_STATUE2))
							pMap->GetStatue4Char()->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);
					}
				}
			}

			if ((pEventInfo->bStep == 3) && (pEventInfo->bFirst))
			{
				for (uint32_t i = 0; i < MOBCOUNT_RESPAWN_BOSS_STEP3; ++i)
				{
					uint32_t dwRandom = number(3, 10);
					pMap->Spawn((uint32_t)(MeleyLair::MOBVNUM_RESPAWN_BOSS_STEP3), stoneSpawnPos[i][0], stoneSpawnPos[i][1] + dwRandom, 0);
					pMap->Spawn((uint32_t)(MeleyLair::MOBVNUM_RESPAWN_SUBBOSS_STEP3), stoneSpawnPos[i][0], stoneSpawnPos[i][1] + dwRandom, 0);
					pMap->Spawn((uint32_t)(MeleyLair::MOBVNUM_RESPAWN_SUBBOSS_STEP3), stoneSpawnPos[i][0], stoneSpawnPos[i][1] + dwRandom, 0);
				}

				pEventInfo->bFirst = false;
			}
		}

		return PASSES_PER_SEC(dwTimeReload);
	}

	EVENTINFO(r_meleyeffect_info)
	{
		CMgrMap* pMap;
		uint8_t	bStep;
		uint8_t	bEffectStep;
	};

	EVENTFUNC(r_meleyeffect_event)
	{
		r_meleyeffect_info* pEventInfo = dynamic_cast<r_meleyeffect_info*>(event->info);
		if (!pEventInfo)
			return 0;

		CMgrMap* pMap = pEventInfo->pMap;
		if (!pMap)
			return 0;

		if ((!pMap->GetBossChar()) || (!pMap->GetStatue1Char()) || (!pMap->GetStatue2Char()) || (!pMap->GetStatue3Char()) || (!pMap->GetStatue4Char()))
			return 0;

		uint8_t bStep = pEventInfo->bStep, bEffectStep = pEventInfo->bEffectStep;
		if (bStep == 1)
		{
			if (bEffectStep == 1)
			{
#ifdef LASER_EFFECT_ON_75HP
				time_t timeNow = static_cast<time_t>(get_dword_time());

				if (pMap->GetMapSectree())
				{
					PIXEL_POSITION pos = MeleyLair::CMgr::instance().GetXYZ();
					pos.x = pMap->GetMapSectree()->m_setting.iBaseX + 130 * 100;
					pos.y = pMap->GetMapSectree()->m_setting.iBaseY + 77 * 100;
					pos.z = 0;
					pMap->GetStatue1Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue2Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue3Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue4Char()->SetRotationToXY(pos.x, pos.y);
				}

				pMap->GetStatue1Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue1Char()->GetX(), pMap->GetStatue1Char()->GetY(), 0, timeNow);
				pMap->GetStatue2Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue2Char()->GetX(), pMap->GetStatue2Char()->GetY(), 0, timeNow);
				pMap->GetStatue3Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue3Char()->GetX(), pMap->GetStatue3Char()->GetY(), 0, timeNow);
				pMap->GetStatue4Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue4Char()->GetX(), pMap->GetStatue4Char()->GetY(), 0, timeNow);
#endif

				pEventInfo->bEffectStep = 2;
			}
			else
			{
				pMap->StartDungeonStep(2);
				return 0;
			}
		}
		else if (bStep == 2)
		{
			if (bEffectStep == 1)
			{
#ifdef LASER_EFFECT_ON_75HP
				time_t timeNow = static_cast<time_t>(get_dword_time());

				if (pMap->GetMapSectree())
				{
					PIXEL_POSITION pos = MeleyLair::CMgr::instance().GetXYZ();
					pos.x = pMap->GetMapSectree()->m_setting.iBaseX + 130 * 100;
					pos.y = pMap->GetMapSectree()->m_setting.iBaseY + 77 * 100;
					pos.z = 0;
					pMap->GetStatue1Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue2Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue3Char()->SetRotationToXY(pos.x, pos.y);
					pMap->GetStatue4Char()->SetRotationToXY(pos.x, pos.y);
				}

				pMap->GetStatue1Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue1Char()->GetX(), pMap->GetStatue1Char()->GetY(), 0, timeNow);
				pMap->GetStatue2Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue2Char()->GetX(), pMap->GetStatue2Char()->GetY(), 0, timeNow);
				pMap->GetStatue3Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue3Char()->GetX(), pMap->GetStatue3Char()->GetY(), 0, timeNow);
				pMap->GetStatue4Char()->SendMovePacket(FUNC_MOB_SKILL, 0, pMap->GetStatue4Char()->GetX(), pMap->GetStatue4Char()->GetY(), 0, timeNow);
#endif

				pEventInfo->bEffectStep = 2;
			}
			else
			{
				pMap->StartDungeonStep(3);
				return 0;
			}
		}

		return PASSES_PER_SEC(5);
	}

	CMgrMap::CMgrMap(int32_t lMapIndex, uint32_t dwGuildID)
	{
		e_pEndEvent = nullptr;
		e_pWarpEvent = nullptr;
		e_SpawnEvent = nullptr;
		e_SEffectEvent = nullptr;
		e_DestroyStatues = nullptr;
		v_Partecipants.clear();
		v_Already.clear();
		v_Rewards.clear();
		map_index = lMapIndex;
		guild_id = dwGuildID;
		dungeon_step = 0;
		reward = 0;
		pkSectreeMap = SECTREE_MANAGER::instance().GetMap(map_index);
		pkMainNPC = nullptr, pkGate = nullptr, pkBoss = nullptr, pkStatue1 = nullptr, pkStatue2 = nullptr, pkStatue3 = nullptr, pkStatue4 = nullptr;
		Start();
	}

	CMgrMap::~CMgrMap()
	{
		if (e_pEndEvent != nullptr)
			event_cancel(&e_pEndEvent);

		if (e_pWarpEvent != nullptr)
			event_cancel(&e_pWarpEvent);

		if (e_SpawnEvent != nullptr)
			event_cancel(&e_SpawnEvent);

		if (e_SEffectEvent != nullptr)
			event_cancel(&e_SEffectEvent);

		if (e_DestroyStatues != nullptr)
			event_cancel(&e_DestroyStatues);
	}

	void CMgrMap::Destroy()
	{
		if (e_pEndEvent != nullptr)
			event_cancel(&e_pEndEvent);

		if (e_pWarpEvent != nullptr)
			event_cancel(&e_pWarpEvent);

		if (e_SpawnEvent != nullptr)
			event_cancel(&e_SpawnEvent);

		if (e_SEffectEvent != nullptr)
			event_cancel(&e_SEffectEvent);

		if (e_DestroyStatues != nullptr)
			event_cancel(&e_DestroyStatues);

		e_pEndEvent = nullptr;
		e_pWarpEvent = nullptr;
		e_SpawnEvent = nullptr;
		e_SEffectEvent = nullptr;
		e_DestroyStatues = nullptr;
		v_Partecipants.clear();
		v_Already.clear();
		v_Rewards.clear();
		map_index = 0;
		guild_id = 0;
		dungeon_step = 0;
		reward = 0;
		pkSectreeMap = nullptr;
		pkMainNPC = nullptr, pkGate = nullptr, pkBoss = nullptr, pkStatue1 = nullptr, pkStatue2 = nullptr, pkStatue3 = nullptr, pkStatue4 = nullptr;
	}

	uint32_t CMgrMap::GetPartecipantsCount() const
	{
		return v_Partecipants.size();
	}

	void CMgrMap::Partecipant(bool bInsert, uint32_t dwPlayerID)
	{
		if (bInsert)
		{
			bool bCheck = std::find(v_Partecipants.begin(), v_Partecipants.end(), dwPlayerID) != v_Partecipants.end();
			if (!bCheck)
				v_Partecipants.push_back(dwPlayerID);
		}
		else
			v_Partecipants.erase(std::remove(v_Partecipants.begin(), v_Partecipants.end(), dwPlayerID), v_Partecipants.end());
	}

	bool CMgrMap::IsPartecipant(uint32_t dwPlayerID)
	{
		if (v_Partecipants.empty())
			return false;

		bool bCheck = std::find(v_Partecipants.begin(), v_Partecipants.end(), dwPlayerID) != v_Partecipants.end();
		return bCheck;
	}

	CHARACTER* CMgrMap::Spawn(uint32_t dwVnum, int32_t iX, int32_t iY, int32_t iDir, bool bSpawnMotion)
	{
		if (!pkSectreeMap)
			return nullptr;

		CHARACTER* pkMob = CHARACTER_MANAGER::instance().SpawnMob(dwVnum, GetMapIndex(), pkSectreeMap->m_setting.iBaseX + iX * 100, pkSectreeMap->m_setting.iBaseY + iY * 100, 0, bSpawnMotion, iDir == 0 ? -1 : (iDir - 1) * 45);
		if (pkMob)
			sys_log(0, "<MeleyLair> Spawn: %s (map index: %d).", pkMob->GetName(), GetMapIndex());

		return pkMob;
	}

	void CMgrMap::SetDungeonStep(uint8_t bStep)
	{
		if (bStep == dungeon_step)
			return;

		dungeon_step = bStep;

		if (e_SpawnEvent != nullptr)
			event_cancel(&e_SpawnEvent);

		e_SpawnEvent = nullptr;

		if (bStep == 1)
		{
			SetDungeonTimeStart(get_global_time());
			SetKillCountStones(0);
			r_meleyspawn_info* pEventInfo = AllocEventInfo<r_meleyspawn_info>();
			pEventInfo->pMap = this;
			pEventInfo->bFirst = false;
			pEventInfo->bStep = bStep;
			pEventInfo->dwTimeReload = (uint32_t)(TIME_RESPAWN_COMMON_STEP1);
			pEventInfo->dwMobVnum = (uint32_t)(MOBVNUM_RESPAWN_COMMON_STEP1);
			pEventInfo->dwMobCount = (uint32_t)(MOBCOUNT_RESPAWN_COMMON_STEP);
			e_SpawnEvent = event_create(r_meleyspawn_event, pEventInfo, PASSES_PER_SEC(1));
		}
		else if (bStep == 2)
		{
			if ((!pkBoss) || (!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4))
				EndDungeon(false, true);

			if (e_SEffectEvent != nullptr)
				event_cancel(&e_SEffectEvent);

			e_SEffectEvent = nullptr;

			r_meleyeffect_info * pEventInfo = AllocEventInfo<r_meleyeffect_info>();
			pEventInfo->pMap = this;
			pEventInfo->bStep = 1;
			pEventInfo->bEffectStep = 1;
			e_SEffectEvent = event_create(r_meleyeffect_event, pEventInfo, PASSES_PER_SEC(5));
		}
		else if (bStep == 3)
		{
			if ((!pkBoss) || (!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4))
				EndDungeon(false, true);

			if (e_SEffectEvent != nullptr)
				event_cancel(&e_SEffectEvent);

			e_SEffectEvent = nullptr;

			r_meleyeffect_info * pEventInfo = AllocEventInfo<r_meleyeffect_info>();
			pEventInfo->pMap = this;
			pEventInfo->bStep = 2;
			pEventInfo->bEffectStep = 1;
			e_SEffectEvent = event_create(r_meleyeffect_event, pEventInfo, PASSES_PER_SEC(5));
		}
		else if (bStep == 4)
		{
			if ((!pkBoss) || (!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4))
				EndDungeon(false, true);

			if (e_SEffectEvent != nullptr)
				event_cancel(&e_SEffectEvent);

			if (e_DestroyStatues != nullptr)
				event_cancel(&e_DestroyStatues);

			e_SEffectEvent = nullptr;
			e_DestroyStatues = nullptr;

			v_Already.clear();

			//pkStatue1->SetArmada();
			//pkStatue2->SetArmada();
			//pkStatue3->SetArmada();
			//pkStatue4->SetArmada();

			char szBuf[512];
#ifdef __MULTI_LANGUAGE_SYSTEM__
			snprintf(szBuf, sizeof(szBuf), LC_TEXT(ENGLISH, "You have %d second(s) to destroy the statues, hurry up!"), TIME_LIMIT_TO_KILL_STATUE);
#else
			snprintf(szBuf, sizeof(szBuf), LC_TEXT("You have %d second(s) to destroy the statues, hurry up!"), TIME_LIMIT_TO_KILL_STATUE);
#endif
			FNotice f(szBuf);
			GetMapSectree()->for_each(f);

			r_meleystatues_info * pEventInfo = AllocEventInfo<r_meleystatues_info>();
			pEventInfo->bFirst = false;
			pEventInfo->pMap = this;
			e_DestroyStatues = event_create(r_meleystatues_event, pEventInfo, PASSES_PER_SEC(3));
		}
	}

	void CMgrMap::StartDungeonStep(uint8_t bStep)
	{
		if (e_SpawnEvent != nullptr)
			event_cancel(&e_SpawnEvent);

		if (e_SEffectEvent != nullptr)
			event_cancel(&e_SEffectEvent);

		e_SpawnEvent = nullptr;
		e_SEffectEvent = nullptr;

		if (bStep == 2)
		{
			SetLastStoneKilledTime(0);
			r_meleyspawn_info* pEventInfo = AllocEventInfo<r_meleyspawn_info>();
			pEventInfo->pMap = this;
			pEventInfo->bFirst = false;
			pEventInfo->bStep = bStep;
			pEventInfo->dwTimeReload = (uint32_t)(TIME_RESPAWN_COMMON_STEP2);
			pEventInfo->dwMobVnum = (uint32_t)(MOBVNUM_RESPAWN_COMMON_STEP2);
			pEventInfo->dwMobCount = (uint32_t)(MOBCOUNT_RESPAWN_COMMON_STEP);
			e_SpawnEvent = event_create(r_meleyspawn_event, pEventInfo, PASSES_PER_SEC(1));
		}
		else if (bStep == 3)
		{
			SetLastStoneKilledTime(0);
			SetKillCountStones(0);
			SetKillCountBosses(0);
			r_meleyspawn_info* pEventInfo = AllocEventInfo<r_meleyspawn_info>();
			pEventInfo->pMap = this;
			pEventInfo->bFirst = true;
			pEventInfo->bStep = bStep;
			pEventInfo->dwTimeReload = (uint32_t)(TIME_RESPAWN_COMMON_STEP3);
			pEventInfo->dwMobVnum = (uint32_t)(MOBVNUM_RESPAWN_COMMON_STEP3);
			pEventInfo->dwMobCount = (uint32_t)(MOBCOUNT_RESPAWN_COMMON_STEP);
			e_SpawnEvent = event_create(r_meleyspawn_event, pEventInfo, PASSES_PER_SEC(1));
		}
	}

	void CMgrMap::Start()
	{
		if (!pkSectreeMap)
			EndDungeon(false, true);
		else
			pkMainNPC = Spawn((uint32_t)(NPC_VNUM), 135, 179, 8), pkGate = Spawn((uint32_t)(GATE_VNUM), 129, 175, 5), pkBoss = Spawn((uint32_t)(BOSS_VNUM), 130, 77, 1), pkStatue1 = Spawn((uint32_t)(STATUE_VNUM), 123, 137, 5, true), pkStatue2 = Spawn((uint32_t)(STATUE_VNUM), 123, 124, 5, true), pkStatue3 = Spawn((uint32_t)(STATUE_VNUM), 136, 123, 5, true), pkStatue4 = Spawn((uint32_t)(STATUE_VNUM), 137, 137, 5, true);
	}

	void CMgrMap::StartDungeon(CHARACTER * pkChar)
	{
		if (!IsPartecipant(pkChar->GetPlayerID()))
			return;
		else if (e_pEndEvent != nullptr)
		{
			EndDungeon(false, true);
			return;
		}
		else if ((!pkMainNPC) || (!pkGate) || (!pkBoss) || (!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4))
		{
			EndDungeon(false, true);
			return;
		}

		CGuild* pkGuild = CGuildManager::instance().FindGuild(GetGuildID());
		if (!pkGuild)
			return;

		char szBuf1[512], szBuf2[512];
#ifdef __MULTI_LANGUAGE_SYSTEM__
		snprintf(szBuf1, sizeof(szBuf1), LC_TEXT(ENGLISH, "<Guild> The fight against %s just start."), pkBoss->GetName());
		snprintf(szBuf2, sizeof(szBuf2), LC_TEXT(ENGLISH, "<Guild> The shelter's doors are now opened. The time limit is %d hour(s)."), (int32_t)(TIME_LIMIT_DUNGEON / 3600));
#else
		snprintf(szBuf1, sizeof(szBuf1), LC_TEXT("<Guild> The fight against %s just start."), pkBoss->GetName());
		snprintf(szBuf2, sizeof(szBuf2), LC_TEXT("<Guild> The shelter's doors are now opened. The time limit is %d hour(s)."), (int32_t)(TIME_LIMIT_DUNGEON / 3600));
#endif
		pkGuild->Chat(szBuf1);
		pkGuild->Chat(szBuf2);

		pkChar->SetQuestNPCID(0);
		pkGate->Dead();
		pkGate = nullptr;

		SetDungeonStep(1);
		if (e_pEndEvent != nullptr)
			event_cancel(&e_pEndEvent);

		r_meleylimit_info * pEventInfo = AllocEventInfo<r_meleylimit_info>();
		pEventInfo->pMap = this;
		pEventInfo->bWarp = false;
		e_pEndEvent = event_create(r_meleylimit_event, pEventInfo, PASSES_PER_SEC(TIME_LIMIT_DUNGEON));
	}

	void CMgrMap::EndDungeon(bool bSuccess, bool bGiveBack)
	{
		int32_t iWarpTime = 0, iReturn = 0;
		CGuild* pkGuild = CGuildManager::instance().FindGuild(GetGuildID());
		if (pkGuild)
		{
			char szBuf1[512], szBuf2[512];
			if (bGiveBack)
			{
				if (bSuccess)
				{
					std::string time;
					uint32_t dwNecessaryTime = get_global_time() - GetDungeonTimeStart();
					LogManager::instance().MeleyLog(GetGuildID(), GetPartecipantsCount(), dwNecessaryTime);
					int32_t iM = (dwNecessaryTime / 60) % 60, iS = dwNecessaryTime % 60;
					if (iM > 0)
#ifdef __MULTI_LANGUAGE_SYSTEM__
						time.append(boost::lexical_cast<std::string>(iM) + LC_TEXT(ENGLISH, " minute(s)"));
#else
						time.append(boost::lexical_cast<std::string>(iM) + LC_TEXT(" minute(s)"));
#endif

					if (iS > 0)
					{
						if (iM > 0)
							time.append(", ");

#ifdef __MULTI_LANGUAGE_SYSTEM__
						time.append(boost::lexical_cast<std::string>(iS) + LC_TEXT(ENGLISH, " second(s)"));
#else
						time.append(boost::lexical_cast<std::string>(iS) + LC_TEXT(" second(s)"));
#endif
					}

					uint32_t dwCoolDownTime = get_global_time() + COOLDOWN_DUNGEON + 5;
					pkGuild->SetDungeonCooldown(dwCoolDownTime);

					iWarpTime = 60;
					iReturn = (int32_t)(LADDER_POINTS_COST);
#ifdef __MULTI_LANGUAGE_SYSTEM__
					snprintf(szBuf1, sizeof(szBuf1), LC_TEXT(ENGLISH, "<Guild> Compliments, you defeated Meley, the Queen of the Dragons."));
					snprintf(szBuf2, sizeof(szBuf2), LC_TEXT(ENGLISH, "<Guild> Necessary time: %s."), time.c_str());
#else
					snprintf(szBuf1, sizeof(szBuf1), "%s", LC_TEXT("<Guild> Compliments, you defeat Meley."));
					snprintf(szBuf2, sizeof(szBuf2), LC_TEXT("<Guild> Necessary time: %s."), time.c_str());
#endif
				}
				else
				{
					iWarpTime = 10;
					iReturn = (int32_t)(LADDER_POINTS_COST);
#ifdef __MULTI_LANGUAGE_SYSTEM__
					snprintf(szBuf1, sizeof(szBuf1), LC_TEXT(ENGLISH, "<Guild> The dungeon can't be started, please contact our staff."));
					snprintf(szBuf2, sizeof(szBuf2), LC_TEXT(ENGLISH, "<Guild> %d ladder points will be returned."), iReturn);
#else
					snprintf(szBuf1, sizeof(szBuf1), "%s", LC_TEXT("<Guild> The dungeon can't be started, please contact our staff."));
					snprintf(szBuf2, sizeof(szBuf2), LC_TEXT("<Guild> %d ladder points will be returned."), iReturn);
#endif
				}
			}
			else
			{
				uint32_t dwCoolDownTime = get_global_time() + COOLDOWN_DUNGEON + 5;
				pkGuild->SetDungeonCooldown(dwCoolDownTime);

				iWarpTime = 10;
				iReturn = (int32_t)(LADDER_POINTS_RETURN);
#ifdef __MULTI_LANGUAGE_SYSTEM__
				snprintf(szBuf1, sizeof(szBuf1), LC_TEXT(ENGLISH, "<Guild> The time limit to fight %s has ended."), pkBoss->GetName());
				snprintf(szBuf2, sizeof(szBuf2), ".");
#else
				snprintf(szBuf1, sizeof(szBuf1), LC_TEXT("<Guild> The time limit to fight %s has ended."), pkBoss->GetName());
				snprintf(szBuf2, sizeof(szBuf2), ".");
#endif
			}

			pkGuild->Chat(szBuf1);
			if (strlen(szBuf2) > 1)
				pkGuild->Chat(szBuf2);

			if (pkSectreeMap)
			{
				char szBuf[512];
#ifdef __MULTI_LANGUAGE_SYSTEM__
				if (iWarpTime == 60)
					snprintf(szBuf, sizeof(szBuf), LC_TEXT(ENGLISH, "<Guild> For your safety, you will be teleported out of the refuge in %d second(s)."), iWarpTime);
				else
					snprintf(szBuf, sizeof(szBuf), LC_TEXT(ENGLISH, "<Guild> All the partecipants will be teleported out in %d seconds."), iWarpTime);
#else
				if (iWarpTime == 60)
					snprintf(szBuf, sizeof(szBuf), LC_TEXT("<Guild> For your safety, you will be teleported out of the refuge in %d second(s)."), iWarpTime);
				else
					snprintf(szBuf, sizeof(szBuf), LC_TEXT("<Guild> All the partecipants will be teleported out in %d seconds."), iWarpTime);
#endif

				pkGuild->Chat(szBuf);
			}

			if (iReturn)
			{
				int32_t dwCost = iReturn;
				pkGuild->ChangeLadderPoint(+dwCost);
				pkGuild->SetLadderPoint(pkGuild->GetLadderPoint() + dwCost);
			}

			pkGuild->RequestDungeon(0, 0);
		}

		if (!iWarpTime)
			EndDungeonWarp();
		else
		{
			if (e_pEndEvent != nullptr)
				event_cancel(&e_pEndEvent);

			e_pEndEvent = nullptr;

			if (e_pWarpEvent != nullptr)
				event_cancel(&e_pWarpEvent);

			r_meleylimit_info * pEventInfo = AllocEventInfo<r_meleylimit_info>();
			pEventInfo->pMap = this;
			pEventInfo->bWarp = true;
			e_pWarpEvent = event_create(r_meleylimit_event, pEventInfo, PASSES_PER_SEC(iWarpTime));
		}
	}

	void CMgrMap::EndDungeonWarp()
	{
		if (pkSectreeMap)
		{
			FExitAndGoTo f;
			pkSectreeMap->for_each(f);
		}

		CMgr::instance().Remove(GetGuildID());
		SECTREE_MANAGER::instance().DestroyPrivateMap(GetMapIndex());
		Destroy();
		M2_DELETE(this);
	}

	bool CMgrMap::Damage(CHARACTER * pkStatue)
	{
		uint8_t bStep = GetDungeonStep();
		if ((!bStep) || ((pkStatue != pkStatue1) && (pkStatue != pkStatue2) && (pkStatue != pkStatue3) && (pkStatue != pkStatue4)))
			return false;
		else if ((pkStatue->IsAffectFlag(AFF_STATUE1)) || pkStatue->IsAffectFlag(AFF_STATUE2) || pkStatue->IsAffectFlag(AFF_STATUE3) || pkStatue->IsAffectFlag(AFF_STATUE4))
			return false;

		int32_t iHPCalc = 0;
		if (bStep == 1)
			iHPCalc = pkStatue->GetMaxHP() * 75 / 100;
		else if (bStep == 2)
			iHPCalc = pkStatue->GetMaxHP() * 50 / 100;
		else if (bStep == 3)
			iHPCalc = pkStatue->GetMaxHP() * 1 / 100;

		if (pkStatue->GetHP() <= iHPCalc)
		{
			int32_t iCalc = iHPCalc - pkStatue->GetHP();
			pkStatue->PointChange(POINT_HP, iCalc);
			if (bStep == 1)
			{
				if (!pkStatue->IsAffectFlag(AFF_STATUE1))
					pkStatue->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE1, 3600, 0, true);

				bool bNextStep = ((pkStatue1->IsAffectFlag(AFF_STATUE1)) && (pkStatue2->IsAffectFlag(AFF_STATUE1)) && (pkStatue3->IsAffectFlag(AFF_STATUE1)) && (pkStatue4->IsAffectFlag(AFF_STATUE1))) ? true : false;
				if (bNextStep)
					SetDungeonStep(2);

				return false;
			}
			else if (bStep == 2)
			{
				if (!pkStatue->IsAffectFlag(AFF_STATUE2))
					pkStatue->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE2, 3600, 0, true);

				bool bNextStep = ((pkStatue1->IsAffectFlag(AFF_STATUE2)) && (pkStatue2->IsAffectFlag(AFF_STATUE2)) && (pkStatue3->IsAffectFlag(AFF_STATUE2)) && (pkStatue4->IsAffectFlag(AFF_STATUE2))) ? true : false;
				if (bNextStep)
					SetDungeonStep(3);

				return false;
			}
			else if (bStep == 3)
			{
				if (!pkStatue->IsAffectFlag(AFF_STATUE3))
					pkStatue->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE3, 3600, 0, true);

				bool bNextStep = ((pkStatue1->IsAffectFlag(AFF_STATUE3)) && (pkStatue2->IsAffectFlag(AFF_STATUE3)) && (pkStatue3->IsAffectFlag(AFF_STATUE3)) && (pkStatue4->IsAffectFlag(AFF_STATUE3))) ? true : false;
				if (bNextStep)
					SetDungeonStep(4);

				return false;
			}
		}

		return true;
	}

	void CMgrMap::OnKill(uint32_t dwVnum)
	{
		uint8_t bStep = GetDungeonStep();
		if ((!bStep) || ((!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4)))
			return;

		if (((bStep == 2) || (bStep == 3)) && (dwVnum == (uint32_t)(MOBVNUM_RESPAWN_STONE_STEP2)))
		{
			uint32_t dwLimit = (uint32_t)(MOBCOUNT_RESPAWN_STONE_STEP2)-1;
			if (GetKillCountStones() >= dwLimit)
			{
				if (bStep == 2)
					SetKillCountStones(0);
				else
					SetKillCountStones(GetKillCountStones() + 1);

				uint32_t dwRandomMin = number(2, 4);
				uint32_t dwLastKilledTime = get_global_time() + (60 * dwRandomMin);
				SetLastStoneKilledTime(dwLastKilledTime);

				bool bDoAff = true;
				if (bStep == 3)
				{
					uint32_t dwLimit2 = (uint32_t)(MOBCOUNT_RESPAWN_BOSS_STEP3);
					if (GetKillCountBosses() >= dwLimit2)
						SetKillCountStones(0);
					else
						bDoAff = false;
				}

				if (bDoAff)
				{
					if ((pkStatue1->IsAffectFlag(AFF_STATUE1)) || ((bStep == 3) && (pkStatue1->IsAffectFlag(AFF_STATUE2))))
						pkStatue1->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue2->IsAffectFlag(AFF_STATUE1)) || ((bStep == 3) && (pkStatue2->IsAffectFlag(AFF_STATUE2))))
						pkStatue2->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue3->IsAffectFlag(AFF_STATUE1)) || ((bStep == 3) && (pkStatue3->IsAffectFlag(AFF_STATUE2))))
						pkStatue3->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue4->IsAffectFlag(AFF_STATUE1)) || ((bStep == 3) && (pkStatue4->IsAffectFlag(AFF_STATUE2))))
						pkStatue4->RemoveAffect(AFFECT_STATUE);
				}
			}
			else
				SetKillCountStones(GetKillCountStones() + 1);
		}
		else if ((bStep == 3) && (dwVnum == (uint32_t)(MOBVNUM_RESPAWN_BOSS_STEP3)))
		{
			uint32_t dwLimit = (uint32_t)(MOBCOUNT_RESPAWN_BOSS_STEP3)-1;
			if (GetKillCountBosses() >= dwLimit)
			{
				SetKillCountBosses(GetKillCountBosses() + 1);

				bool bDoAff = true;
				uint32_t dwLimit2 = (uint32_t)(MOBCOUNT_RESPAWN_STONE_STEP2);
				if (GetKillCountStones() >= dwLimit2)
					SetKillCountStones(0);
				else
					bDoAff = false;

				if (bDoAff)
				{
					if ((pkStatue1->IsAffectFlag(AFF_STATUE1)) || (pkStatue1->IsAffectFlag(AFF_STATUE2)))
						pkStatue1->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue2->IsAffectFlag(AFF_STATUE1)) || (pkStatue2->IsAffectFlag(AFF_STATUE2)))
						pkStatue2->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue3->IsAffectFlag(AFF_STATUE1)) || (pkStatue3->IsAffectFlag(AFF_STATUE2)))
						pkStatue3->RemoveAffect(AFFECT_STATUE);

					if ((pkStatue4->IsAffectFlag(AFF_STATUE1)) || (pkStatue4->IsAffectFlag(AFF_STATUE2)))
						pkStatue4->RemoveAffect(AFFECT_STATUE);
				}
			}
			else
				SetKillCountBosses(GetKillCountBosses() + 1);
		}
	}

	void CMgrMap::OnKillStatue(CItem * pkItem, CHARACTER * pkChar, CHARACTER * pkStatue)
	{
		uint8_t bStep = GetDungeonStep();
		if ((!pkItem) || (!pkChar) || (!pkStatue) || (bStep != 4) || (!pkBoss) || ((!pkStatue1) || (!pkStatue2) || (!pkStatue3) || (!pkStatue4)))
			return;

		bool bNextStep = (((pkStatue1->IsAffectFlag(AFF_STATUE3)) || (pkStatue1->IsAffectFlag(AFF_STATUE4))) && ((pkStatue2->IsAffectFlag(AFF_STATUE3)) || (pkStatue2->IsAffectFlag(AFF_STATUE4))) && ((pkStatue3->IsAffectFlag(AFF_STATUE3)) || (pkStatue3->IsAffectFlag(AFF_STATUE4))) && ((pkStatue4->IsAffectFlag(AFF_STATUE3)) || (pkStatue4->IsAffectFlag(AFF_STATUE4)))) ? true : false;
		if (!bNextStep)
			return;

		if (pkStatue->IsAffectFlag(AFF_STATUE4))
			return;

		uint32_t dwPlayerID = pkChar->GetPlayerID();

		ITEM_MANAGER::instance().RemoveItem(pkItem);
		pkStatue->RemoveAffect(AFFECT_STATUE);
		pkStatue->AddAffect(AFFECT_STATUE, POINT_NONE, 0, AFF_STATUE4, 3600, 0, true);
		bool bNextStepPass = ((pkStatue1->IsAffectFlag(AFF_STATUE4)) && (pkStatue2->IsAffectFlag(AFF_STATUE4)) && (pkStatue3->IsAffectFlag(AFF_STATUE4)) && (pkStatue4->IsAffectFlag(AFF_STATUE4))) ? true : false;
		if (bNextStepPass)
			DungeonResult();
	}

	void CMgrMap::DungeonResult()
	{
		if (e_DestroyStatues != nullptr)
			event_cancel(&e_DestroyStatues);

		e_DestroyStatues = nullptr;
		bool bNextStep = ((pkStatue1->IsAffectFlag(AFF_STATUE4)) && (pkStatue2->IsAffectFlag(AFF_STATUE4)) && (pkStatue3->IsAffectFlag(AFF_STATUE4)) && (pkStatue4->IsAffectFlag(AFF_STATUE4))) ? true : false;
		if (!bNextStep)
		{
			EndDungeon(false, false);
		}
		else
		{
			v_Rewards.clear();
			SetRewardTime(get_global_time() + 10);
			pkStatue1->Dead();
			pkStatue1 = nullptr;
			pkStatue2->Dead();
			pkStatue2 = nullptr;
			pkStatue3->Dead();
			pkStatue3 = nullptr;
			pkStatue4->Dead();
			pkStatue4 = nullptr;
			pkBoss->Dead();
			pkBoss = nullptr;

			Spawn(CHEST_VNUM, 130, 130, 1);

			EndDungeon(true, true);
		}
	}

	bool CMgrMap::CheckRewarder(uint32_t dwPlayerID)
	{
		bool bCheck = std::find(v_Rewards.begin(), v_Rewards.end(), dwPlayerID) != v_Rewards.end();
		return bCheck;
	}

	void CMgrMap::GiveReward(CHARACTER * pkChar, uint8_t bReward)
	{
		uint32_t dwPlayerID = pkChar->GetPlayerID();
		if (!IsPartecipant(dwPlayerID))
			return;
		else if (CheckRewarder(dwPlayerID))
			return;

		v_Rewards.push_back(dwPlayerID);
		uint32_t dwVnumReward = bReward == 1 ? REWARD_ITEMCHEST_VNUM_1 : REWARD_ITEMCHEST_VNUM_2;
		pkChar->AutoGiveItem(dwVnumReward, 1);
	}

	void CMgr::Initialize()
	{
		m_RegGuilds.clear();
		SetXYZ(0, 0, 0);
		SetSubXYZ(0, 0, 0);
	}

	void CMgr::Destroy()
	{
		auto iter = m_RegGuilds.begin();
		for (; iter != m_RegGuilds.end(); ++iter)
		{
			CMgrMap* pMap = iter->second;
			SECTREE_MANAGER::instance().DestroyPrivateMap(pMap->GetMapIndex());
			pMap->Destroy();
			M2_DELETE(pMap);
		}

		m_RegGuilds.clear();
		SetXYZ(0, 0, 0);
		SetSubXYZ(0, 0, 0);
	}

	bool CMgr::isRegistered(CGuild * pkGuild, int32_t & iCH)
	{
		iCH = 0;
		if (pkGuild)
		{
			int32_t lMinIndex = MAP_INDEX * 10000, lMaxIndex = MAP_INDEX * 10000 + 10000;
			if ((pkGuild->GetDungeonMapIndex() >= lMinIndex) && (pkGuild->GetDungeonMapIndex() <= lMaxIndex))
			{
				iCH = pkGuild->GetDungeonCH();
				return true;
			}
		}

		return false;
	}

	void CMgr::Register(CHARACTER * pkChar, int32_t & iRes1, int32_t & iRes2)
	{
		iRes1 = 0;
		iRes2 = 0;
		if (!pkChar)
			return;

		CGuild* pkGuild = pkChar->GetGuild();
		if (!pkGuild)
			return;
		else if (pkGuild->GetMasterPID() != pkChar->GetPlayerID())
			return;

		int32_t iCH;
		if (isRegistered(pkGuild, iCH))
		{
			iRes1 = 1;
			iRes2 = iCH;
			return;
		}
		else if (pkGuild->GetDungeonCH())
		{
			iRes1 = 2;
			iRes2 = 0;
			return;
		}
		else if (pkGuild->GetLevel() < MIN_LVL)
		{
			iRes1 = 3;
			iRes2 = (int32_t)(MIN_LVL);
			return;
		}
		else if (pkGuild->GetLadderPoint() < LADDER_POINTS_COST)
		{
			iRes1 = 4;
			iRes2 = (int32_t)(LADDER_POINTS_COST);
			return;
		}

		uint32_t dwTimeNow = get_global_time();
		if ((pkGuild->GetDungeonCooldown() > dwTimeNow) && (!pkGuild->GetDungeonCH()))
		{
			int32_t iDif = pkGuild->GetDungeonCooldown() - dwTimeNow;
			iRes1 = 7;
			iRes2 = iDif;
			return;
		}

		int32_t lNormalMapIndex = (int32_t)(MAP_INDEX);
		int32_t lMapIndex = SECTREE_MANAGER::instance().CreatePrivateMap(lNormalMapIndex);
		if (!lMapIndex)
		{
			iRes1 = 5;
			return;
		}

		int32_t dwCost = (int32_t)(LADDER_POINTS_COST);
		pkGuild->ChangeLadderPoint(-dwCost);
		pkGuild->SetLadderPoint(pkGuild->GetLadderPoint() - dwCost);

		PIXEL_POSITION pos = GetXYZ(), posSub = GetSubXYZ();
		if (!pos.x)
		{
			LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((int32_t)(MeleyLair::MAP_INDEX));
			if (pkSectreeMap)
				SetXYZ(pkSectreeMap->m_setting.iBaseX + 130 * 100, pkSectreeMap->m_setting.iBaseY + 130 * 100, 0);
		}

		if (!posSub.x)
		{
			LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((int32_t)(MeleyLair::SUBMAP_INDEX));
			if (pkSectreeMap)
				SetSubXYZ(pkSectreeMap->m_setting.iBaseX + 87 * 100, pkSectreeMap->m_setting.iBaseY + 853 * 100, 0);
		}

		pkGuild->RequestDungeon(g_bChannel, lMapIndex);
		pkGuild->SetDungeonCooldown(0);
		m_RegGuilds.insert(std::make_pair(pkGuild->GetID(), M2_NEW CMgrMap(lMapIndex, pkGuild->GetID())));

		iRes1 = 6;
		iRes2 = (int32_t)(LADDER_POINTS_RETURN);
	}

	bool CMgr::Enter(CGuild * pkGuild, CHARACTER * pkChar, int32_t & iLimit)
	{
		iLimit = 0;
		if ((pkGuild) && (pkChar))
		{
			uint32_t dwGuildID = pkGuild->GetID();
			if (m_RegGuilds.find(dwGuildID) == m_RegGuilds.end())
			{
				uint8_t bCH = pkGuild->GetDungeonCH();
				if ((bCH) && (g_bChannel != bCH))
				{
					iLimit = pkGuild->GetDungeonCH();
					return false;
				}
				else if (g_bChannel != pkGuild->GetDungeonCH())
					return false;

				int32_t lNormalMapIndex = (int32_t)(MAP_INDEX);
				int32_t lMapIndex = SECTREE_MANAGER::instance().CreatePrivateMap(lNormalMapIndex);
				if (!lMapIndex)
				{
					int32_t dwCost = (int32_t)(LADDER_POINTS_COST);
					pkGuild->ChangeLadderPoint(+dwCost);
					pkGuild->SetLadderPoint(pkGuild->GetLadderPoint() + dwCost);
					return false;
				}

				PIXEL_POSITION pos = GetXYZ(), posSub = GetSubXYZ();
				if (!pos.x)
				{
					LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((int32_t)(MeleyLair::MAP_INDEX));
					if (pkSectreeMap)
						SetXYZ(pkSectreeMap->m_setting.iBaseX + 130 * 100, pkSectreeMap->m_setting.iBaseY + 130 * 100, 0);
				}

				if (!posSub.x)
				{
					LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap((int32_t)(MeleyLair::SUBMAP_INDEX));
					if (pkSectreeMap)
						SetSubXYZ(pkSectreeMap->m_setting.iBaseX + 87 * 100, pkSectreeMap->m_setting.iBaseY + 853 * 100, 0);
				}

				pkGuild->RequestDungeon(g_bChannel, lMapIndex);
				m_RegGuilds.insert(std::make_pair(dwGuildID, M2_NEW CMgrMap(lMapIndex, dwGuildID)));
			}

			CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
			if (!pMap)
				return false;
			else if (pMap->GetPartecipantsCount() >= PARTECIPANTS_LIMIT)
			{
				iLimit = 1;
				return true;
			}
			else if (pMap->GetDungeonStep() >= 4)
			{
				iLimit = 3;
				return true;
			}
			else if (pkChar->GetPKMode() != PK_MODE_GUILD)
			{
				iLimit = 4;
				return true;
			}
			else if (pkChar->GetLevel() <= 75)
			{
				iLimit = 6;
				return true;
			}
			else if (quest::CQuestManager::instance().GetEventFlag("meley_lair_dungeon_status") == 1 && quest::CQuestManager::instance().GetEventFlag("meley_lair_dungeon_guild") != pkChar->GetGuild()->GetID())
			{
				if (quest::CQuestManager::instance().GetEventFlag("meley_lair_dungeon_guild") != 0)
				{
					iLimit = 7;
					return true;
				}
			}
			bool canPass = false;
			if (pkChar->CountSpecifyItem(71201) > 0)
				canPass = true;
			else if (pkChar->CountSpecifyItem(72342) > 0)
				canPass = true;
			
			if (canPass == false)
			{
				iLimit = 6;
				return true;
			}

			PIXEL_POSITION mPos;
			if (!SECTREE_MANAGER::instance().GetRecallPositionByEmpire((int32_t)(MAP_INDEX), 0, mPos))
			{
				iLimit = 2;
				return true;
			}

			quest::CQuestManager::instance().SetEventFlag("meley_lair_dungeon_status", 1);
			quest::CQuestManager::instance().SetEventFlag("meley_lair_dungeon_guild", pkChar->GetGuild()->GetID());
			pMap->Partecipant(true, pkChar->GetPlayerID());
			pkChar->SaveExitLocation();
			pkChar->WarpSet(mPos.x, mPos.y, pMap->GetMapIndex());
			return true;
		}

		return false;
	}

	void CMgr::Leave(CGuild * pkGuild, CHARACTER * pkChar, bool bSendOut)
	{
		if (!pkChar)
			return;

		if (pkGuild)
		{
			uint32_t dwGuildID = pkGuild->GetID();
			if (m_RegGuilds.find(dwGuildID) != m_RegGuilds.end())
			{
				CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
				if (pMap)
				{
					if (!bSendOut)
					{
						if (pkChar->GetMapIndex() == pMap->GetMapIndex())
							pMap->Partecipant(false, pkChar->GetPlayerID());
					}
					else if (!pMap->IsPartecipant(pkChar->GetPlayerID()))
						WarpOut(pkChar);
				}
				else
					WarpOut(pkChar);
			}
			else
				WarpOut(pkChar);
		}
		else if (bSendOut)
			WarpOut(pkChar);
	}

	void CMgr::LeaveRequest(CGuild * pkGuild, CHARACTER * pkChar)
	{
		if (!pkChar)
			return;

		if (pkGuild)
		{
			uint32_t dwGuildID = pkGuild->GetID();
			if (m_RegGuilds.find(dwGuildID) != m_RegGuilds.end())
			{
				CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
				if (pMap)
				{
					if (pMap->IsPartecipant(pkChar->GetPlayerID()))
						pMap->Partecipant(false, pkChar->GetPlayerID());

					WarpOut(pkChar);
				}
				else
					WarpOut(pkChar);
			}
			else
				WarpOut(pkChar);
		}
		else
			WarpOut(pkChar);
	}

	bool CMgr::IsMeleyMap(int32_t lMapIndex)
	{
		int32_t lMinIndex = (int32_t)(MAP_INDEX) * 10000, lMaxIndex = (int32_t)(MAP_INDEX) * 10000 + 10000;
		if (((lMapIndex >= lMinIndex) && (lMapIndex <= lMaxIndex)) || (lMapIndex == (int32_t)(MAP_INDEX)))
			return true;

		return false;
	}

	void CMgr::Check(CGuild * pkGuild, CHARACTER * pkChar)
	{
		if (!pkChar)
			return;

		if (pkGuild)
		{
			uint32_t dwGuildID = pkGuild->GetID();
			if (m_RegGuilds.find(dwGuildID) != m_RegGuilds.end())
			{
				CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
				if (pMap)
				{
					if (!pMap->IsPartecipant(pkChar->GetPlayerID()))
						WarpOut(pkChar);
				}
				else
					WarpOut(pkChar);
			}
			else
				WarpOut(pkChar);
		}
		else
			WarpOut(pkChar);
	}

	void CMgr::WarpOut(CHARACTER * pkChar)
	{
		if (!pkChar)
			return;

		PIXEL_POSITION posSub = GetSubXYZ();
		if (!posSub.x)
			pkChar->WarpSet(EMPIRE_START_X(pkChar->GetEmpire()), EMPIRE_START_Y(pkChar->GetEmpire()));
		else
			pkChar->WarpSet(posSub.x, posSub.y);
	}

	void CMgr::Start(CHARACTER * pkChar, CGuild * pkGuild)
	{
		if ((!pkChar) || (!pkGuild))
			return;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (pMap)
			pMap->StartDungeon(pkChar);
	}

	bool CMgr::Damage(CHARACTER * pkStatue, CGuild * pkGuild)
	{
		if ((!pkStatue) || (!pkGuild))
			return false;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return false;

		return pMap->Damage(pkStatue);
	}

	void CMgr::Remove(uint32_t dwGuildID)
	{
		auto iter = m_RegGuilds.find(dwGuildID), iterEnd = m_RegGuilds.end();
		if (iter != iterEnd)
			m_RegGuilds.erase(iter, iterEnd);
	}

	void CMgr::OnKill(uint32_t dwVnum, CGuild * pkGuild)
	{
		if ((!dwVnum) || (!pkGuild))
			return;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return;

		pMap->OnKill(dwVnum);
	}

	void CMgr::OnKillStatue(CItem * pkItem, CHARACTER * pkChar, CHARACTER * pkStatue, CGuild * pkGuild)
	{
		if ((!pkItem) || (!pkChar) || (!pkStatue) || (!pkGuild))
			return;

		if (pkItem->GetOriginalVnum() != (uint32_t)(SEAL_VNUM_KILL_STATUE))
			return;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return;
		else if (!pMap->IsPartecipant(pkChar->GetPlayerID()))
			return;

		pMap->OnKillStatue(pkItem, pkChar, pkStatue);
	}

	void CMgr::OnKillCommon(CHARACTER * pkMonster, CHARACTER * pkChar, CGuild * pkGuild)
	{
		if ((!pkMonster) || (!pkChar) || (!pkGuild))
			return;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return;
		else if (!pMap->IsPartecipant(pkChar->GetPlayerID()))
			return;
		else if (pMap->GetDungeonStep() < 3)
			return;

		int32_t iChance = number(1, 100);
		if (iChance > 70)
		{
			CItem* pkItem = ITEM_MANAGER::instance().CreateItem(SEAL_VNUM_KILL_STATUE);
			if (!pkItem)
				return;

			PIXEL_POSITION mPos;
			mPos.x = pkMonster->GetX();
			mPos.y = pkMonster->GetY();

			pkItem->AddToGround(pMap->GetMapIndex(), mPos);
			pkItem->StartDestroyEvent();
			pkItem->SetOwnership(pkChar, 60);
		}
	}

	bool CMgr::CanGetReward(CHARACTER * pkChar, CGuild * pkGuild)
	{
		if ((!pkChar) || (!pkGuild))
			return false;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return false;
		else if (!pMap->IsPartecipant(pkChar->GetPlayerID()))
			return false;
		else if (pMap->GetDungeonStep() != 4)
			return false;

		uint32_t dwTime = get_global_time();
		if (pMap->GetRewardTime() > dwTime)
			return false;
		else if (pMap->CheckRewarder(pkChar->GetPlayerID()))
			return false;

		return true;
	}

	void CMgr::Reward(CHARACTER * pkChar, CGuild * pkGuild, uint8_t bReward)
	{
		if ((!pkChar) || (!pkGuild))
			return;

		CMgrMap * pMap = m_RegGuilds.find(pkGuild->GetID())->second;
		if (!pMap)
			return;

		pMap->GiveReward(pkChar, bReward);
	}

	void CMgr::OpenRanking(CHARACTER * pkChar)
	{
		if (!pkChar)
			return;

		std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT * FROM log.log_meley_dungeon%s ORDER BY time ASC, partecipants ASC, date ASC LIMIT 5;", get_table_postfix()));
		if (pMsg->Get()->uiNumRows == 0)
		{
			pkChar->ChatPacket(CHAT_TYPE_COMMAND, "meley_open");
			return;
		}
		else
		{
			pkChar->ChatPacket(CHAT_TYPE_COMMAND, "meley_open");
			int32_t iLine = 0;
			MYSQL_ROW mRow;
			while (nullptr != (mRow = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				int32_t iCur = 0;
				uint32_t dwGuildID = 0, dwPartecipants = 0, dwTime = 0;
				str_to_number(dwGuildID, mRow[iCur++]);
				str_to_number(dwPartecipants, mRow[iCur++]);
				str_to_number(dwTime, mRow[iCur]);
				CGuild* pkGuild = CGuildManager::instance().FindGuild(dwGuildID);
				if (!pkGuild)
					continue;

				pkChar->ChatPacket(CHAT_TYPE_COMMAND, "meley_rank #%d#%s#%d#%d#", iLine, pkGuild->GetName(), dwPartecipants, dwTime);
				iLine++;
			}
		}
	}

	void CMgr::MemberRemoved(CHARACTER * pkChar, CGuild * pkGuild)
	{
		if ((!pkChar) || (!pkGuild))
			return;

		if (!MeleyLair::CMgr::instance().IsMeleyMap(pkChar->GetMapIndex()))
			return;

		uint32_t dwGuildID = pkGuild->GetID();
		if (m_RegGuilds.find(dwGuildID) != m_RegGuilds.end())
		{
			CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
			if (pMap)
			{
				if (pMap->IsPartecipant(pkChar->GetPlayerID()))
					pMap->Partecipant(false, pkChar->GetPlayerID());

				WarpOut(pkChar);
			}
		}
	}

	void CMgr::GuildRemoved(CGuild * pkGuild)
	{
		if (!pkGuild)
			return;

		uint32_t dwGuildID = pkGuild->GetID();
		if (m_RegGuilds.find(dwGuildID) != m_RegGuilds.end())
		{
			CMgrMap* pMap = m_RegGuilds.find(dwGuildID)->second;
			if (pMap)
				pMap->EndDungeonWarp();
		}
	}
};
#endif