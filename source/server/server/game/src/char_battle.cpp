#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "battle.h"
#include "pvp.h"
#include "skill.h"
#include "start_position.h"
#include "profiler.h"
#include "cmd.h"
#include "dungeon.h"
#include "log.h"
#include "unique_item.h"
#include "priv_manager.h"
#include "db.h"
#include "vector.h"
#include "marriage.h"
#include "arena.h"
#include "regen.h"
#include "exchange.h"
#include "shop_manager.h"
#include "ani.h"
#include "packet.h"
#include "party.h"
#include "affect.h"
#include "guild.h"
#include "guild_manager.h"
#include "questmanager.h"
#include "questlua.h"
#include "BlueDragon.h"
#include "DragonLair.h"
#ifdef ENABLE_VERSION_162_UPDATE
#include "TempleOchao.h"
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "MeleyLair.h"
#endif
#include "war_map.h"
#include "../../common/VnumHelper.h"
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif
#ifdef ENABLE_BUFFI_SYSTEM
#include "support_shaman.h"
#endif
#ifdef ENABLE_BOSS_TRACKING_SYSTEM
#include "boss_tracking.h"
#endif
#include "daily_boss.h"
#include <random>
#include <algorithm>
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

static DWORD __GetPartyExpNP(const DWORD level)
{
	if (!level || level > PLAYER_EXP_TABLE_MAX)
		return 14000;
	return party_exp_distribute_table[level];
}

static int __GetExpLossPerc(const DWORD level)
{
	if (!level || level > PLAYER_EXP_TABLE_MAX)
		return 1;
	return aiExpLossPercents[level];
}

DWORD AdjustExpByLevel(const LPCHARACTER ch, const DWORD exp)
{
	if (PLAYER_EXP_TABLE_MAX < ch->GetLevel())
	{
		double ret = 0.95;
		double factor = 0.1;

		for (ssize_t i = 0; i < ch->GetLevel() - 100; ++i)
		{
			if ((i % 10) == 0)
				factor /= 2.0;

			ret *= 1.0 - factor;
		}

		ret = ret * static_cast<double>(exp);

		if (ret < 1.0)
			return 1;

		return static_cast<DWORD>(ret);
	}

	return exp;
}

bool CHARACTER::CanBeginFight() const
{
	if (!CanMove())
		return false;

	return m_pointsInstant.position == POS_STANDING && !IsDead() && !IsStun();
}

void CHARACTER::BeginFight(LPCHARACTER pkVictim)
{
	SetVictim(pkVictim);
	SetPosition(POS_FIGHTING);
	SetNextStatePulse(1);
}

bool CHARACTER::CanFight() const
{
	return m_pointsInstant.position >= POS_FIGHTING ? true : false;
}

void CHARACTER::CreateFly(BYTE bType, LPCHARACTER pkVictim)
{
	TPacketGCCreateFly packFly;

	packFly.bHeader = HEADER_GC_CREATE_FLY;
	packFly.bType = bType;
	packFly.dwStartVID = GetVID();
	packFly.dwEndVID = pkVictim->GetVID();

	PacketAround(&packFly, sizeof(TPacketGCCreateFly));
}

void CHARACTER::DistributeSP(LPCHARACTER pkKiller, int iMethod)
{
	if (pkKiller->GetSP() >= pkKiller->GetMaxSP())
		return;

	bool bAttacking = (get_dword_time() - GetLastAttackTime()) < 3000;
	bool bMoving = (get_dword_time() - GetLastMoveTime()) < 3000;

	if (iMethod == 1)
	{
		int num = number(0, 3);

		if (!num)
		{
			int iLvDelta = GetLevel() - pkKiller->GetLevel();
			int iAmount = 0;

			if (iLvDelta >= 5)
				iAmount = 10;
			else if (iLvDelta >= 0)
				iAmount = 6;
			else if (iLvDelta >= -3)
				iAmount = 2;

			if (iAmount != 0)
			{
				iAmount += (iAmount * pkKiller->GetPoint(POINT_SP_REGEN)) / 100;

				if (iAmount >= 11)
					CreateFly(FLY_SP_BIG, pkKiller);
				else if (iAmount >= 7)
					CreateFly(FLY_SP_MEDIUM, pkKiller);
				else
					CreateFly(FLY_SP_SMALL, pkKiller);

				pkKiller->PointChange(POINT_SP, iAmount);
			}
		}
	}
	else
	{
		if (pkKiller->GetJob() == JOB_SHAMAN || (pkKiller->GetJob() == JOB_SURA && pkKiller->GetSkillGroup() == 2))
		{
			int iAmount;

			if (bAttacking)
				iAmount = 2 + GetMaxSP() / 100;
			else if (bMoving)
				iAmount = 3 + GetMaxSP() * 2 / 100;
			else
				iAmount = 10 + GetMaxSP() * 3 / 100; // 평상시

			iAmount += (iAmount * pkKiller->GetPoint(POINT_SP_REGEN)) / 100;
			pkKiller->PointChange(POINT_SP, iAmount);
		}
		else
		{
			int iAmount;

			if (bAttacking)
				iAmount = 2 + pkKiller->GetMaxSP() / 200;
			else if (bMoving)
				iAmount = 2 + pkKiller->GetMaxSP() / 100;
			else
			{
				// 평상시
				if (pkKiller->GetHP() < pkKiller->GetMaxHP())
					iAmount = 2 + (pkKiller->GetMaxSP() / 100); // 피 다 안찼을때
				else
					iAmount = 9 + (pkKiller->GetMaxSP() / 100); // 기본
			}

			iAmount += (iAmount * pkKiller->GetPoint(POINT_SP_REGEN)) / 100;
			pkKiller->PointChange(POINT_SP, iAmount);
		}
	}
}

bool CHARACTER::Attack(LPCHARACTER pkVictim, BYTE bType)
{
	if (test_server)
		sys_log(0, "[TEST_SERVER] Attack : %s type %d, MobBattleType %d", GetName(), bType, !GetMobBattleType() ? 0 : GetMobAttackRange());
	//PROF_UNIT puAttack("Attack");
	if (!CanMove())
		return false;
	if (pkVictim == this) {
		sys_log(0, "Attack: Trying to attack self");
		return false;
	}
	
#ifdef ENABLE_ANTISAFEZONE
	SECTREE	*sectree = nullptr;
	SECTREE	*vsectree = nullptr;
	sectree = GetSectree();
	vsectree = pkVictim->GetSectree();

	if (sectree && vsectree)
	{
		if (sectree->IsAttr(GetX(), GetY(), ATTR_BANPK) || vsectree->IsAttr(pkVictim->GetX(), pkVictim->GetY(), ATTR_BANPK)) 
		{
			if (GetDesc()) 
			{
				LogManager::instance().HackLog("ANTISAFEZONE", this);
				GetDesc()->DelayedDisconnect(3);
			}
		}
	}
#endif

#ifdef ENABLE_OBSERVER_DAMAGE_FIX
	if (pkVictim->IsPC()) {
		if (pkVictim->IsObserverMode()) {
			return false;
		}
	}
#endif

	DWORD dwCurrentTime = get_dword_time();

#ifdef ENABLE_ABUSE_SYSTEM
	if (IsPC())
	{
		//if (IsSuspectWaitDmg())
		//	return false;

		m_abuse->ReceiveAttackPacket();
	}
#endif

	// @fixme131
	if (!battle_is_attackable(this, pkVictim))
		return false;

	if (IsPC())
	{
		if (bType == 0 && dwCurrentTime < GetSkipComboAttackByTime())
			return false;
	}
	else
	{
		MonsterChat(MONSTER_CHAT_ATTACK);
	}

	pkVictim->SetSyncOwner(this);

	if (pkVictim->CanBeginFight())
	{
#ifdef ENABLE_ABUSE_SYSTEM
		if (IsPC() && pkVictim->IsNPC())
			GetAbuseController()->InitiatedFight(pkVictim->GetXYZ(), pkVictim->IsAggressive());
#endif
		pkVictim->BeginFight(this);
	}

#ifdef ENABLE_VERSION_162_UPDATE
	if ((pkVictim->IsMonster()) && (pkVictim->GetMobTable().dwVnum == TEMPLE_OCHAO_GUARDIAN) && (pkVictim->GetMapIndex() == TEMPLE_OCHAO_MAP_INDEX))
	{
		TempleOchao::CMgr::instance().GuardianAttacked();
	}
#endif
	int iRet;

	if (bType == 0)
	{
		//
		// 일반 공격
		//
		switch (GetMobBattleType())
		{
		case BATTLE_TYPE_MELEE:
		case BATTLE_TYPE_POWER:
		case BATTLE_TYPE_TANKER:
		case BATTLE_TYPE_SUPER_POWER:
		case BATTLE_TYPE_SUPER_TANKER:
			iRet = battle_melee_attack(this, pkVictim);
			break;

		case BATTLE_TYPE_RANGE:
			FlyTarget(pkVictim->GetVID(), pkVictim->GetX(), pkVictim->GetY(), HEADER_CG_FLY_TARGETING);
			iRet = Shoot(0) ? BATTLE_DAMAGE : BATTLE_NONE;
			break;

		case BATTLE_TYPE_MAGIC:
			FlyTarget(pkVictim->GetVID(), pkVictim->GetX(), pkVictim->GetY(), HEADER_CG_FLY_TARGETING);
			iRet = Shoot(1) ? BATTLE_DAMAGE : BATTLE_NONE;
			break;

		default:
			//sys_err("Unhandled battle type %d", GetMobBattleType());
			iRet = BATTLE_NONE;
			break;
		}
	}
	else
	{
		if (IsPC() == true)
		{
			if (dwCurrentTime - m_dwLastSkillTime > 1500)
			{
				sys_log(1, "HACK: Too long skill using term. Name(%s) PID(%u) delta(%u)",
					GetName(), GetPlayerID(), (dwCurrentTime - m_dwLastSkillTime));
				return false;
			}
		}

		sys_log(1, "Attack call ComputeSkill %d %s", bType, pkVictim ? pkVictim->GetName() : "");
		iRet = ComputeSkill(bType, pkVictim);
	}

	//if (test_server && IsPC())
	//	sys_log(0, "%s Attack %s type %u ret %d", GetName(), pkVictim->GetName(), bType, iRet);
	if (iRet == BATTLE_DAMAGE || iRet == BATTLE_DEAD)
	{
		OnMove(true);
		pkVictim->OnMove();

		// only pc sets victim nullptr. For npc, state machine will reset this.
		if (BATTLE_DEAD == iRet && IsPC())
			SetVictim(nullptr);

		return true;
	}

	return false;
}

void CHARACTER::DeathPenalty(BYTE bTown)
{
	sys_log(1, "DEATH_PERNALY_CHECK(%s) town(%d)", GetName(), bTown);

	Cube_close(this);
#ifdef ENABLE_CHANGELOOK_SYSTEM
	ChangeLookWindow(false, true);
#endif
#ifdef ENABLE_ACCE_SYSTEM
	CloseSash();
#endif
#ifdef ENABLE_AURA_SYSTEM
	CloseAura();
#endif

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif

	if (GetLevel() < 10)
	{
		sys_log(0, "NO_DEATH_PENALTY_LESS_LV10(%s)", GetName());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("용신의 가호로 경험치가 떨어지지 않았습니다."));
		return;
	}

	if (number(0, 2))
	{
		sys_log(0, "NO_DEATH_PENALTY_LUCK(%s)", GetName());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("용신의 가호로 경험치가 떨어지지 않았습니다."));
		return;
	}

	if (IS_SET(m_pointsInstant.instant_flag, INSTANT_FLAG_DEATH_PENALTY))
	{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_DEATH_PENALTY);

		if (!bTown) // 국제 버전에서는 제자리 부활시만 용신의 가호를 사용한다. (마을 복귀시는 경험치 패널티 없음)
		{
			if (FindAffect(AFFECT_NO_DEATH_PENALTY))
			{
				sys_log(0, "NO_DEATH_PENALTY_AFFECT(%s)", GetName());
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("용신의 가호로 경험치가 떨어지지 않았습니다."));
				RemoveAffect(AFFECT_NO_DEATH_PENALTY);
				return;
			}
		}
		// END_OF_NO_DEATH_PENALTY_BUG_FIX

		int iLoss = ((GetNextExp() * __GetExpLossPerc(GetLevel())) / 100);
		iLoss = MIN(800000, iLoss);
		if (bTown)
			iLoss = 0;

		if (IsEquipUniqueItem(UNIQUE_ITEM_TEARDROP_OF_GODNESS))
			iLoss /= 2;

		sys_log(0, "DEATH_PENALTY(%s) EXP_LOSS: %d percent %d%%", GetName(), iLoss, __GetExpLossPerc(GetLevel()));

		PointChange(POINT_EXP, -iLoss, true);
	}
}

bool CHARACTER::IsStun() const
{
	if (IS_SET(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN))
		return true;

	return false;
}

EVENTFUNC(StunEvent)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("StunEvent> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}
	ch->m_pkStunEvent = nullptr;
	ch->Dead();
	return 0;
}

void CHARACTER::Stun()
{
	if (IsStun())
		return;

	if (IsDead())
		return;

	if (!IsPC() && m_pkParty)
	{
		m_pkParty->SendMessage(this, PM_ATTACKED_BY, 0, 0);
	}

	if ((GetRaceNum() == (WORD)(MeleyLair::STATUE_VNUM)) && !IsPC())
		return;

	sys_log(1, "%s: Stun %p", GetName(), this);

	PointChange(POINT_HP_RECOVERY, -GetPoint(POINT_HP_RECOVERY));
	PointChange(POINT_SP_RECOVERY, -GetPoint(POINT_SP_RECOVERY));

	CloseMyShop();

	event_cancel(&m_pkRecoveryEvent); // 회복 이벤트를 죽인다.

	TPacketGCStun pack;
	pack.header = HEADER_GC_STUN;
	pack.vid = m_vid;
	PacketAround(&pack, sizeof(pack));

	SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN);

	if (m_pkStunEvent)
		return;

	char_event_info * info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkStunEvent = event_create(StunEvent, info, PASSES_PER_SEC(3));
}

EVENTINFO(SCharDeadEventInfo)
{
	bool isPC;
	uint32_t dwID;

	SCharDeadEventInfo()
		: isPC(0)
		, dwID(0)
	{
	}
};

EVENTFUNC(dead_event)
{
	const SCharDeadEventInfo* info = dynamic_cast<SCharDeadEventInfo*>(event->info);

	if (info == nullptr)
	{
		sys_err("dead_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = nullptr;

	if (true == info->isPC)
	{
		ch = CHARACTER_MANAGER::instance().FindByPID(info->dwID);
	}
	else
	{
		ch = CHARACTER_MANAGER::instance().Find(info->dwID);
	}

	if (nullptr == ch)
	{
		sys_err("DEAD_EVENT: cannot find char pointer with %s id(%d)", info->isPC ? "PC" : "MOB", info->dwID);
		return 0;
	}

	if ((ch->GetRaceNum() == (WORD)(MeleyLair::STATUE_VNUM)) && !ch->IsPC())
		return 0;

	ch->m_pkDeadEvent = nullptr;

	if (ch->GetDesc())
	{
		ch->GetDesc()->SetPhase(PHASE_GAME);

		ch->SetPosition(POS_STANDING);

		PIXEL_POSITION pos;

		if (ch->GetMapIndex() != 200 && ch->GetMapIndex() != 218)
		{
			if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
				ch->WarpSet(pos.x, pos.y);
			else
			{
				sys_err("cannot find spawn position (name %s)", ch->GetName());
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
			}
		}

		ch->PointChange(POINT_HP, (ch->GetMaxHP() / 2) - ch->GetHP(), true);

		ch->DeathPenalty(0);

		ch->StartRecoveryEvent();

		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
	}
	else
	{
		if (ch->IsMonster() == true)
		{
			if (ch->IsRevive() == false && ch->HasReviverInParty() == true)
			{
				ch->SetPosition(POS_STANDING);
				ch->SetHP(ch->GetMaxHP());

				ch->ViewReencode();

				ch->SetAggressive();
				ch->SetRevive(true);

				return 0;
			}
		}

		M2_DESTROY_CHARACTER(ch);
	}

	return 0;
}

bool CHARACTER::IsDead() const
{
	if (m_pointsInstant.position == POS_DEAD)
		return true;

	return false;
}

inline int GetGoldMultipler()
{
	return 1;
}

void CHARACTER::RewardGold(LPCHARACTER pkAttacker)
{
	int llTotalGold = 0;
	int iGoldPercent = MobRankStats[GetMobRank()].iGoldPercent;

	if (pkAttacker->IsPC())
		iGoldPercent = iGoldPercent * (100 + CPrivManager::instance().GetPriv(pkAttacker, PRIV_GOLD_DROP)) / 100;

	if (pkAttacker->GetPoint(POINT_MALL_GOLDBONUS))
		iGoldPercent = (iGoldPercent * pkAttacker->GetPoint(POINT_MALL_GOLDBONUS)) / 100;

	iGoldPercent = iGoldPercent * CHARACTER_MANAGER::instance().GetMobGoldDropRate(pkAttacker) / 100;
	iGoldPercent += iGoldPercent;

	if (iGoldPercent > 100)
		iGoldPercent = 100;

	int iPercent;

	if (GetMobRank() >= MOB_RANK_BOSS)
		iPercent = ((iGoldPercent * PERCENT_LVDELTA_BOSS(pkAttacker->GetLevel(), GetLevel())) / 100);
	else
		iPercent = ((iGoldPercent * PERCENT_LVDELTA(pkAttacker->GetLevel(), GetLevel())) / 100);

	if (number(1, 100) > iPercent)
		return;

	int iGoldMultipler = GetGoldMultipler();

	if (1 == number(1, 50000))
		iGoldMultipler *= 10;
	else if (1 == number(1, 10000))
		iGoldMultipler *= 5;

	if (pkAttacker->GetPoint(POINT_GOLD_DOUBLE_BONUS))
		if (number(1, 100) <= pkAttacker->GetPoint(POINT_GOLD_DOUBLE_BONUS))
			iGoldMultipler *= 2;
	
	int tmpMobTableMax = GetMobTable().dwGoldMax;
	if (GetMobTable().dwGoldMin > GetMobTable().dwGoldMax)
		tmpMobTableMax = GetMobTable().dwGoldMin + 1000;
		
	int iGold = number(GetMobTable().dwGoldMin, tmpMobTableMax);
	iGold = iGold * CHARACTER_MANAGER::instance().GetMobGoldAmountRate(pkAttacker) / 100;
	iGold *= iGoldMultipler;

#if defined(INC_GOLD_MULTIPLER)
	iGold /= INC_GOLD_MULTIPLER;
#endif

#if defined(GOLD_MULTIPLER)
	iGold *= GOLD_MULTIPLER;
#endif

#ifdef ENABLE_GOLDEN_RAIN_EVENT
	if (quest::CQuestManager::Instance().GetEventFlag("golden_rain_event") == 1)
		iGold += iGold / 20;
#endif

#ifdef ENABLE_VALUE_PACK_SYSTEM
	if (pkAttacker->IsAffectFlag(AFF_PREMIUM))
	{
		int llPremiumGold = iGold / 20;
		iGold += llPremiumGold;
	}
#endif

	int iSplitCount;

	if (iGold >= 3)
		iSplitCount = number(1, 3);
	else if (GetMobRank() >= MOB_RANK_BOSS)
	{
		iSplitCount = number(3, 10);

		if ((iGold / iSplitCount) == 0)
			iSplitCount = 1;
	}
	else
		iSplitCount = 1;

	if (iGold != 0)
	{
		llTotalGold += iGold;

		for (int i = 0; i < iSplitCount; ++i)
		{
			pkAttacker->GiveGold(iGold / iSplitCount);
		}
	}

	DBManager::instance().SendMoneyLog(MONEY_LOG_MONSTER, GetRaceNum(), llTotalGold);
}

void CHARACTER::Reward(bool bItemDrop)
{
	if (GetRaceNum() == 5001) // 왜구는 돈을 무조건 드롭
	{
		PIXEL_POSITION pos;

		if (!SECTREE_MANAGER::instance().GetMovablePosition(GetMapIndex(), GetX(), GetY(), pos))
			return;

		LPITEM item;
		int iGold = number(GetMobTable().dwGoldMin, GetMobTable().dwGoldMax);
		iGold = iGold * CHARACTER_MANAGER::instance().GetMobGoldAmountRate(nullptr) / 100;
		iGold *= GetGoldMultipler();
		int iSplitCount = number(25, 35);

		sys_log(0, "WAEGU Dead gold %d split %d", iGold, iSplitCount);

		for (int i = 1; i <= iSplitCount; ++i)
		{
			if ((item = ITEM_MANAGER::instance().CreateItem(1, iGold / iSplitCount)))
			{
				if (i != 0)
				{
					pos.x = number(-7, 7) * 20;
					pos.y = number(-7, 7) * 20;

					pos.x += GetX();
					pos.y += GetY();
				}

				item->AddToGround(GetMapIndex(), pos);
				item->StartDestroyEvent();
			}
		}
		return;
	}

	//PROF_UNIT puReward("Reward");
	LPCHARACTER pkAttacker = DistributeExp();

	if (!pkAttacker)
		return;

	//PROF_UNIT pu1("r1");
	if (pkAttacker->IsPC())
	{
		if ((GetLevel() - pkAttacker->GetLevel()) >= -10)
		{
#ifdef ENABLE_ALIGNMENT_EVENT
			if (quest::CQuestManager::instance().GetEventFlag("alignment_event"))
			{
				if (pkAttacker->GetRealAlignment() < 0)
				{
					if (pkAttacker->IsEquipUniqueItem(UNIQUE_ITEM_FASTER_ALIGNMENT_UP_BY_KILL))
#ifdef ENABLE_ALIGNMENT_SYSTEM
						pkAttacker->UpdateAlignment(36, false);
					else
						pkAttacker->UpdateAlignment(21, false);
				}
				else
					pkAttacker->UpdateAlignment(8, false);
#else
						pkAttacker->UpdateAlignment(36);
					else
						pkAttacker->UpdateAlignment(21);
				}
				else
					pkAttacker->UpdateAlignment(8);
#endif
			}
			else
#endif
			{
				if (pkAttacker->GetRealAlignment() < 0)
				{
					if (pkAttacker->IsEquipUniqueItem(UNIQUE_ITEM_FASTER_ALIGNMENT_UP_BY_KILL))
#ifdef ENABLE_ALIGNMENT_SYSTEM
						pkAttacker->UpdateAlignment(14, false);
					else
						pkAttacker->UpdateAlignment(7, false);
				}
				else
					pkAttacker->UpdateAlignment(2, false);
#else
						pkAttacker->UpdateAlignment(14);
					else
						pkAttacker->UpdateAlignment(7);
				}
				else
					pkAttacker->UpdateAlignment(2);
#endif
			}
		}

		pkAttacker->SetQuestNPCID(GetVID());

#ifdef ENABLE_ATTENDANCE_EVENT
		if (GetRaceNum() >= 6415 && GetRaceNum() <= 6419)
		{
			RewardAttendance();
		}
#endif

		quest::CQuestManager::instance().Kill(pkAttacker->GetPlayerID(), GetRaceNum());
		CHARACTER_MANAGER::instance().KillLog(GetRaceNum());

		if (!number(0, 9))
		{
			if (pkAttacker->GetPoint(POINT_KILL_HP_RECOVERY))
			{
				int iHP = pkAttacker->GetMaxHP() * pkAttacker->GetPoint(POINT_KILL_HP_RECOVERY) / 100;
				pkAttacker->PointChange(POINT_HP, iHP);
				CreateFly(FLY_HP_SMALL, pkAttacker);
			}

			if (pkAttacker->GetPoint(POINT_KILL_SP_RECOVER))
			{
				int iSP = pkAttacker->GetMaxSP() * pkAttacker->GetPoint(POINT_KILL_SP_RECOVER) / 100;
				pkAttacker->PointChange(POINT_SP, iSP);
				CreateFly(FLY_SP_SMALL, pkAttacker);
			}
		}
	}
	//pu1.Pop();

	if (!bItemDrop)
		return;

	PIXEL_POSITION pos = GetXYZ();

	if (!SECTREE_MANAGER::instance().GetMovablePosition(GetMapIndex(), pos.x, pos.y, pos))
		return;

	//
	// 돈 드롭
	//
	//PROF_UNIT pu2("r2");
	if (test_server)
		sys_log(0, "Drop money : Attacker %s", pkAttacker->GetName());
	RewardGold(pkAttacker);
	//pu2.Pop();

	//
	// 아이템 드롭
	//
	//PROF_UNIT pu3("r3");
	LPITEM item;

	static std::vector<LPITEM> s_vec_item;
	s_vec_item.clear();

	if (ITEM_MANAGER::instance().CreateDropItem(this, pkAttacker, s_vec_item))
	{
		if (s_vec_item.size() == 0);
		else if (s_vec_item.size() == 1)
		{
			item = s_vec_item[0];
			item->AddToGround(GetMapIndex(), pos);

#ifdef ENABLE_DICE_SYSTEM
			if (pkAttacker->GetParty())
			{
				FPartyDropDiceRoll f(item, pkAttacker);
				f.Process(this);
			}
			else
				item->SetOwnership(pkAttacker);
#else
			item->SetOwnership(pkAttacker);
#endif

			item->StartDestroyEvent();

			pos.x = number(-7, 7) * 20;
			pos.y = number(-7, 7) * 20;
			pos.x += GetX();
			pos.y += GetY();

			sys_log(0, "DROP_ITEM: %s %d %d from %s", item->GetName(), pos.x, pos.y, GetName());
		}
		else
		{
			int iItemIdx = s_vec_item.size() - 1;

			std::priority_queue<std::pair<int, LPCHARACTER> > pq;

			int total_dam = 0;

			for (TDamageMap::iterator it = m_map_kDamage.begin(); it != m_map_kDamage.end(); ++it)
			{
				int iDamage = it->second.iTotalDamage;
				if (iDamage > 0)
				{
					LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(it->first);

					if (ch)
					{
						pq.push(std::make_pair(iDamage, ch));
						total_dam += iDamage;
					}
				}
			}

			std::vector<LPCHARACTER> v;

			while (!pq.empty() && pq.top().first * 10 >= total_dam)
			{
				v.push_back(pq.top().second);
				pq.pop();
			}

			if (v.empty())
			{
				// 데미지를 특별히 많이 준 사람이 없으니 소유권 없음
				while (iItemIdx >= 0)
				{
					item = s_vec_item[iItemIdx--];

					if (!item)
					{
						sys_err("item nullptr in vector idx %d", iItemIdx + 1);
						continue;
					}

					item->AddToGround(GetMapIndex(), pos);
					// 10% 이하 데미지 준 사람끼리는 소유권없음
					//item->SetOwnership(pkAttacker);
					item->StartDestroyEvent();

					pos.x = number(-7, 7) * 20;
					pos.y = number(-7, 7) * 20;
					pos.x += GetX();
					pos.y += GetY();

					sys_log(0, "DROP_ITEM: %s %d %d by %s", item->GetName(), pos.x, pos.y, GetName());
				}
			}
			else
			{
				// 데미지 많이 준 사람들 끼리만 소유권 나눠가짐
				std::vector<LPCHARACTER>::iterator it = v.begin();

				while (iItemIdx >= 0)
				{
					item = s_vec_item[iItemIdx--];

					if (!item)
					{
						sys_err("item nullptr in vector idx %d", iItemIdx + 1);
						continue;
					}

					item->AddToGround(GetMapIndex(), pos);

					LPCHARACTER ch = *it;

					if (ch->GetParty())
						ch = ch->GetParty()->GetNextOwnership(ch, GetX(), GetY());

					++it;

					if (it == v.end())
						it = v.begin();

#ifdef ENABLE_DICE_SYSTEM
					if (ch->GetParty())
					{
						FPartyDropDiceRoll f(item, ch);
						f.Process(this);
					}
					else
						item->SetOwnership(ch);
#else
					item->SetOwnership(ch);
#endif

					item->StartDestroyEvent();

					pos.x = number(-7, 7) * 20;
					pos.y = number(-7, 7) * 20;
					pos.x += GetX();
					pos.y += GetY();

					sys_log(0, "DROP_ITEM: %s %d %d by %s", item->GetName(), pos.x, pos.y, GetName());
				}
			}
		}
	}

	m_map_kDamage.clear();
}

#ifdef ENABLE_ATTENDANCE_EVENT
void CHARACTER::RewardAttendance()
{
	for (TDamageMap::iterator it = m_map_kDamage.begin(); it != m_map_kDamage.end(); ++it)
	{
		int iDamage = it->second.iTotalDamage;
		if (iDamage > 0)
		{
			LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(it->first);

			if (ch)
			{
				DWORD dwCount = 0;

				std::string flagNames[5] = {
					"attendance_event.red_dragon_hit_points",
					"attendance_event.jotun_thrym_hit_points",
					"attendance_event.razador_hit_points",
					"attendance_event.nemere_hit_points",
					"attendance_event.beran_hit_points",
				};

				if (!ch->m_hitCount.empty())
				{
					for (DWORD i = 0; i < ch->m_hitCount.size(); i++)
					{
						if (ch->m_hitCount[i].dwVid == GetVID())
						{
							dwCount = ch->m_hitCount[i].dwCount;
							break;
						}
					}
				}

				if (dwCount)
				{
					if (ch->GetLevel() < 30)
					{
						continue;
					}

					if (!ch->FindAffect(AFFECT_EXP_BONUS_EVENT))
					{
						ch->AddAffect(AFFECT_EXP_BONUS_EVENT, POINT_EXP, 20, 0, 1800, 0, false);
					}

					if (ch->FindAffect(AFFECT_ATT_SPEED_SLOW))
					{
						ch->RemoveAffect(AFFECT_ATT_SPEED_SLOW);
					}

					ch->SetQuestFlag(flagNames[6419 - GetRaceNum()], ch->GetQuestFlag(flagNames[6419 - GetRaceNum()]) + dwCount);

					time_t iTime; time(&iTime); tm * pTimeInfo = localtime(&iTime);
					char szFlagname[32 + 1];
					snprintf(szFlagname, sizeof(szFlagname), "attendance.clear_day_%d", pTimeInfo->tm_yday);

					if (ch->GetQuestFlag(szFlagname) == 0)
					{
						ch->SetQuestFlag(szFlagname, 1);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have earned your daily target - collect your reward."));

						TPacketGCAttendanceEvent packEvent;
						packEvent.bHeader = HEADER_GC_ATTENDANCE_EVENT;
						packEvent.bType = 2;
						packEvent.bValue = 1;
						ch->GetDesc()->Packet(&packEvent, sizeof(TPacketGCAttendanceEvent));
					}
				}
			}
		}
	}
}
#endif

struct TItemDropPenalty
{
	int iInventoryPct;		// Range: 1 ~ 1000
	int iInventoryQty;		// Range: --
	int iEquipmentPct;		// Range: 1 ~ 100
	int iEquipmentQty;		// Range: --
};

TItemDropPenalty aItemDropPenalty_kor[13] =
{
	{   0,   0,  0,  0 },	// 선왕
	{   0,   0,  0,  0 },	// 영웅
	{   0,   0,  0,  0 },	// 성자
	{   0,   0,  0,  0 },	// 지인
	{   0,   0,  0,  0 },	// 양민
	{   0,   0,  0,  0 },	// 지인
	{   0,   0,  0,  0 },	// 양민
	{  25,   1,  5,  1 },	// 낭인
	{  50,   2, 10,  1 },	// 악인
	{  75,   4, 15,  1 },	// 마두
	{ 100,   8, 20,  1 },	// 패왕
	{ 125,   10, 25,  1 },	// 마두
	{ 150,   12, 30,  1 },	// 패왕
};

void CHARACTER::ItemDropPenalty(LPCHARACTER pkKiller)
{
	// 개인상점을 연 상태에서는 아이템을 드롭하지 않는다.
	if (GetMyShop())
		return;

	if (GetLevel() < 50)
		return;

	struct TItemDropPenalty* table = &aItemDropPenalty_kor[0];

	if (GetLevel() < 10)
		return;

	int iAlignIndex;

	if (GetRealAlignment() >= 120000)
		iAlignIndex = 0;
	else if (GetRealAlignment() >= 80000)
		iAlignIndex = 1;
	else if (GetRealAlignment() >= 40000)
		iAlignIndex = 2;
	else if (GetRealAlignment() >= 10000)
		iAlignIndex = 3;
	else if (GetRealAlignment() >= 0)
		iAlignIndex = 4;
	else if (GetRealAlignment() > -40000)
		iAlignIndex = 5;
	else if (GetRealAlignment() > -80000)
		iAlignIndex = 6;
	else if (GetRealAlignment() > -120000)
		iAlignIndex = 7;
	else
		iAlignIndex = 8;

	std::vector<std::pair<LPITEM, int> > vec_item;
	LPITEM pkItem;
	int	i;
	bool isDropAllEquipments = false;

	TItemDropPenalty & r = table[iAlignIndex];
	sys_log(0, "%s align %d inven_pct %d equip_pct %d", GetName(), iAlignIndex, r.iInventoryPct, r.iEquipmentPct);

	bool bDropInventory = r.iInventoryPct >= number(1, 1000);
	bool bDropEquipment = r.iEquipmentPct >= number(1, 100);
	bool bDropAntiDropUniqueItem = false;

	if ((bDropInventory || bDropEquipment) && IsEquipUniqueItem(UNIQUE_ITEM_SKIP_ITEM_DROP_PENALTY))
	{
		bDropInventory = false;
		bDropEquipment = false;
		bDropAntiDropUniqueItem = true;
	}

	if (bDropInventory) // Drop Inventory
	{
		std::vector<BYTE> vec_bSlots;

#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		for (i = 0; i < GetExtendInvenMax(); ++i)
#else
		for (i = 0; i < INVENTORY_MAX_NUM; ++i)
#endif
			if (GetInventoryItem(i))
				vec_bSlots.push_back(i);

		if (!vec_bSlots.empty())
		{
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(vec_bSlots.begin(), vec_bSlots.end(), g);
			int iQty = MIN (vec_bSlots.size(), r.iInventoryQty);

			if (iQty)
			{
				iQty = number (1, iQty);
			}

			for (i = 0; i < iQty; ++i)
			{
				pkItem = GetInventoryItem(vec_bSlots[i]);

				if (IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_PKDROP))
					continue;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
				if (pkItem->IsSealed())
					continue;
#endif

				if (pkItem->IsBasicItem())
					continue;

				SyncQuickslot(QUICKSLOT_TYPE_ITEM, vec_bSlots[i], 1000);
				vec_item.push_back(std::make_pair(pkItem->RemoveFromCharacter(), INVENTORY));
			}
		}
		else if (iAlignIndex == 8)
			isDropAllEquipments = true;
	}

	if (bDropEquipment) // Drop Equipment
	{
		std::vector<BYTE> vec_bSlots;

		for (i = 0; i < WEAR_MAX_NUM; ++i)
			if (GetWear(i))
				vec_bSlots.push_back(i);

		if (!vec_bSlots.empty())
		{
			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(vec_bSlots.begin(), vec_bSlots.end(), g);

			int iQty = MIN(vec_bSlots.size(), r.iInventoryQty);

			if (iQty)
			{
				iQty = number(1, iQty);
			}

			for (i = 0; i < iQty; ++i)
			{
				pkItem = GetWear(vec_bSlots[i]);

				if (IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_PKDROP))
					continue;

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
				if (pkItem->IsSealed())
					continue;
#endif

				if (pkItem->IsBasicItem())
					continue;
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, vec_bSlots[i], 1000);
				vec_item.push_back(std::make_pair(pkItem->RemoveFromCharacter(), EQUIPMENT));
			}
		}
	}

	if (bDropAntiDropUniqueItem)
	{
		LPITEM pkItem;

		pkItem = GetWear(WEAR_UNIQUE1);

		if (pkItem && pkItem->GetVnum() == UNIQUE_ITEM_SKIP_ITEM_DROP_PENALTY)
		{
			SyncQuickslot(QUICKSLOT_TYPE_ITEM, WEAR_UNIQUE1, 1000);
			vec_item.push_back(std::make_pair(pkItem->RemoveFromCharacter(), EQUIPMENT));
		}

		pkItem = GetWear(WEAR_UNIQUE2);

		if (pkItem && pkItem->GetVnum() == UNIQUE_ITEM_SKIP_ITEM_DROP_PENALTY)
		{
			SyncQuickslot(QUICKSLOT_TYPE_ITEM, WEAR_UNIQUE2, 1000);
			vec_item.push_back(std::make_pair(pkItem->RemoveFromCharacter(), EQUIPMENT));
		}
	}

	{
		PIXEL_POSITION pos;
		pos.x = GetX();
		pos.y = GetY();

		unsigned int i;

		for (i = 0; i < vec_item.size(); ++i)
		{
			LPITEM item = vec_item[i].first;
			int window = vec_item[i].second;

			item->AddToGround(GetMapIndex(), pos);
			item->StartDestroyEvent();

			sys_log(0, "DROP_ITEM_PK: %s %d %d from %s", item->GetName(), pos.x, pos.y, GetName());
			LogManager::instance().ItemLog(this, item, "DEAD_DROP", (window == INVENTORY) ? "INVENTORY" : ((window == EQUIPMENT) ? "EQUIPMENT" : ""));

			pos.x = GetX() + number(-7, 7) * 20;
			pos.y = GetY() + number(-7, 7) * 20;
		}
	}
}

class FPartyAlignmentCompute
{
public:
	FPartyAlignmentCompute(int iAmount, int x, int y)
	{
		m_iAmount = iAmount;
		m_iCount = 0;
		m_iStep = 0;
		m_iKillerX = x;
		m_iKillerY = y;
	}

	void operator () (LPCHARACTER pkChr)
	{
		if (DISTANCE_APPROX(pkChr->GetX() - m_iKillerX, pkChr->GetY() - m_iKillerY) < PARTY_DEFAULT_RANGE)
		{
			if (m_iStep == 0)
			{
				++m_iCount;
			}
			else
			{
#ifdef ENABLE_ALIGNMENT_SYSTEM
				pkChr->UpdateAlignment(m_iAmount / m_iCount, false);
#else
				pkChr->UpdateAlignment(m_iAmount / m_iCount);
#endif
			}
		}
	}

	int m_iAmount;
	int m_iCount;
	int m_iStep;

	int m_iKillerX;
	int m_iKillerY;
};

void CHARACTER::Dead(LPCHARACTER pkKiller, bool bImmediateDead)
{
	if (IsDead())
		return;
	
#ifdef ENABLE_ABUSE_SYSTEM
	if (IsPC())
		GetAbuseController()->MarkDead();
#endif

	{
		if (IsHorseRiding())
		{
			StopRiding();
		}
		else if (GetMountVnum())
		{
			RemoveAffect(AFFECT_MOUNT);
			RemoveAffect(AFFECT_MOUNT_BONUS);
			m_dwMountVnum = 0;
			UnEquipSpecialRideUniqueItem();
			UpdatePacket();
		}
	}

	if (!pkKiller && m_dwKillerPID)
		pkKiller = CHARACTER_MANAGER::instance().FindByPID(m_dwKillerPID);

	m_dwKillerPID = 0; // 반드시 초기화 해야함 DO NOT DELETE THIS LINE UNLESS YOU ARE 1000000% SURE

	bool isAgreedPVP = false;
	bool isUnderGuildWar = false;
	bool isDuel = false;

	if (pkKiller && pkKiller->IsPC())
	{
		if (pkKiller->m_pkChrTarget == this)
			pkKiller->SetTarget(nullptr);

		if (!IsPC() && pkKiller->GetDungeon())
			pkKiller->GetDungeon()->IncKillCount(pkKiller, this);

		isAgreedPVP = CPVPManager::instance().Dead(this, pkKiller->GetPlayerID());
		isDuel = CArenaManager::instance().OnDead(pkKiller, this);

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
		CCombatZoneManager::instance().OnDead(pkKiller, this);
#endif

		if (IsPC())
		{
			pkKiller->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().Kill(pkKiller->GetPlayerID(), quest::QUEST_NO_NPC);
			CGuildManager::instance().Kill(pkKiller, this);
		}
		
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		if ((IsMonster() || IsStone()) && pkKiller->IsPC() && (pkKiller->GetMapIndex() >= 3580000 && pkKiller->GetMapIndex() < 3590000))
		{
			CZodiacManager::instance().DeadMob(this, pkKiller->GetMapIndex());
		}
#endif
	}

#ifdef ENABLE_QUEST_DIE_EVENT
	if (IsPC())
	{
		if (pkKiller)
			SetQuestNPCID(pkKiller->GetVID());
		// quest::CQuestManager::instance().Die(GetPlayerID(), quest::QUEST_NO_NPC);
		quest::CQuestManager::instance().Die(GetPlayerID(), (pkKiller) ? pkKiller->GetRaceNum() : quest::QUEST_NO_NPC);
	}
#endif

#ifdef ENABLE_BOSS_TRACKING_SYSTEM
	if (IsMonster() && CBossTracking::instance().IsTrackingMob(GetMobTable().dwVnum) == true)
	{
		if (pkKiller)
		{
			LPREGEN pkRegen = GetRegen();
			if (pkRegen != nullptr)
			{
				if (pkRegen->time != 0)
				{
					CBossTracking::instance().SetDeadTime(g_bChannel, GetMobTable().dwVnum, get_global_time());
					CBossTracking::instance().SetRegenTime(g_bChannel, GetMobTable().dwVnum, pkRegen->time);
					CBossTracking::instance().SendP2PPacket(GetMobTable().dwVnum);
				}
			}
		}
	}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		if (IsPC() && (pkKiller && pkKiller->IsMonster()) && (GetMapIndex() >= 3580000 && GetMapIndex() < 3590000))
		{
			IncDeadCount();
			CZodiacManager::instance().DeadPC(this->GetMapIndex());
		}
#endif

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		if (GetRaceNum() == 20434)
		{
			if (pkKiller->IsMonster())
			{
				pkKiller->GetDungeon()->SetMast(nullptr);
			}
		}
#endif

	if (pkKiller && pkKiller->IsPC() &&
		!isAgreedPVP &&
		!isUnderGuildWar &&
		IsPC() &&
		!isDuel)
	{
		if (GetGMLevel() == GM_PLAYER || test_server)
		{
			ItemDropPenalty(pkKiller);
		}
	}

	if (pkKiller)
	{
		if (CDailyBoss::instance().IsBossMap(pkKiller->GetMapIndex()) == true)
		{
			if (CDailyBoss::instance().BossVnum() == GetRaceNum())
				CDailyBoss::instance().OnKill(pkKiller);
		}
	}

	SetPosition(POS_DEAD);
	ClearAffect(true);

	if (pkKiller)
	{
		if (!pkKiller->IsPC())
		{
			sys_log(1, "DEAD: %s %p WITH PENALTY", GetName(), this);
			SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_DEATH_PENALTY);
		}
		else
		{
			sys_log(1, "DEAD_BY_PC: %s %p KILLER %s %p", GetName(), this, pkKiller->GetName(), get_pointer(pkKiller));
			REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_DEATH_PENALTY);

			if (GetEmpire() != pkKiller->GetEmpire())
			{
				int iEP = MIN(GetPoint(POINT_EMPIRE_POINT), pkKiller->GetPoint(POINT_EMPIRE_POINT));

				PointChange(POINT_EMPIRE_POINT, -(iEP / 10));
				pkKiller->PointChange(POINT_EMPIRE_POINT, iEP / 5);
			}
			else
			{
				if (!isAgreedPVP && !isUnderGuildWar && !IsKillerMode() && GetAlignment() >= 0 && !isDuel && pkKiller->GetMapIndex() != 200 && pkKiller->GetMapIndex() != 218)
				{
					int iNoPenaltyProb = 0;

					if (pkKiller->GetAlignment() >= 0)	// 1/3 percent down
						iNoPenaltyProb = 33;
					else				// 4/5 percent down
						iNoPenaltyProb = 20;

					if (number(1, 100) < iNoPenaltyProb)
						pkKiller->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("용신의 보호로 아이템이 떨어지지 않았습니다."));
					else
					{
						if (pkKiller->GetParty())
						{
							FPartyAlignmentCompute f(-20000, pkKiller->GetX(), pkKiller->GetY());
							pkKiller->GetParty()->ForEachOnlineMember(f);

							if (f.m_iCount == 0)
#ifdef ENABLE_ALIGNMENT_SYSTEM
								pkKiller->UpdateAlignment(-20000, false);
#else
								pkKiller->UpdateAlignment(-20000);
#endif
							else
							{
								sys_log(0, "ALIGNMENT PARTY count %d amount %d", f.m_iCount, f.m_iAmount);

								f.m_iStep = 1;
								pkKiller->GetParty()->ForEachOnlineMember(f);
							}
						}
						else
#ifdef ENABLE_ALIGNMENT_SYSTEM
							pkKiller->UpdateAlignment(-20000, false);
#else
							pkKiller->UpdateAlignment(-20000);
#endif
					}
				}
			}
		}
	}
	else
	{
		sys_log(1, "DEAD: %s %p", GetName(), this);
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_DEATH_PENALTY);
	}

	ClearSync();

	//sys_log(1, "stun cancel %s[%d]", GetName(), (DWORD)GetVID());
	event_cancel(&m_pkStunEvent); // 기절 이벤트는 죽인다.

	if (IsPC())
	{
		m_dwLastDeadTime = get_dword_time();
		SetKillerMode(false);
		GetDesc()->SetPhase(PHASE_DEAD);
	}
	else
	{
		// 가드에게 공격받은 몬스터는 보상이 없어야 한다.
		if (!IS_SET(m_pointsInstant.instant_flag, INSTANT_FLAG_NO_REWARD))
		{
			if (!(pkKiller && pkKiller->IsPC() && pkKiller->GetGuild() && pkKiller->GetGuild()->UnderAnyWar(GUILD_WAR_TYPE_FIELD)))
			{
				// 부활하는 몬스터는 보상을 주지 않는다.
				if (GetMobTable().dwResurrectionVnum)
				{
					// DUNGEON_MONSTER_REBIRTH_BUG_FIX
					LPCHARACTER chResurrect = CHARACTER_MANAGER::instance().SpawnMob(GetMobTable().dwResurrectionVnum, GetMapIndex(), GetX(), GetY(), GetZ(), true, (int)GetRotation());
					if (GetDungeon() && chResurrect)
					{
						chResurrect->SetDungeon(GetDungeon());
					}
					
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
					if (GetZodiac() && chResurrect)
					{
						chResurrect->SetZodiac(GetZodiac());
					}
#endif
					// END_OF_DUNGEON_MONSTER_REBIRTH_BUG_FIX

					Reward(false);
				}
				else if (IsRevive() == true)
				{
					Reward(false);
				}
				else
				{
					Reward(true); // Drops gold, item, etc..
				}
			}
			else
			{
				if (pkKiller->m_dwUnderGuildWarInfoMessageTime < get_dword_time())
				{
					pkKiller->m_dwUnderGuildWarInfoMessageTime = get_dword_time() + 60000;
					pkKiller->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드전중에는 사냥에 따른 이익이 없습니다."));
				}
			}
		}
	}

	// BOSS_KILL_LOG
	if (GetMobRank() >= MOB_RANK_BOSS && pkKiller && pkKiller->IsPC())
	{
		char buf[51];
		snprintf(buf, sizeof(buf), "%d %ld", g_bChannel, pkKiller->GetMapIndex());
#ifdef ENABLE_USELESS_LOGS
		if (IsStone())
			LogManager::instance().CharLog(pkKiller, GetRaceNum(), "STONE_KILL", buf);
		else
			LogManager::instance().CharLog(pkKiller, GetRaceNum(), "BOSS_KILL", buf);
#endif
	}
	// END_OF_BOSS_KILL_LOG

	TPacketGCDead pack;
	pack.header = HEADER_GC_DEAD;
	pack.vid = m_vid;
	PacketAround(&pack, sizeof(pack));

	REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN);

	// 플레이어 캐릭터이면
	if (GetDesc() != nullptr) {
		//
		// 클라이언트에 에펙트 패킷을 다시 보낸다.
		//
		auto it = m_list_pkAffect.begin();

		while (it != m_list_pkAffect.end())
			SendAffectAddPacket(GetDesc(), *it++);
	}

	//
	// Dead 이벤트 생성,
	//
	// Dead 이벤트에서는 몬스터의 경우 몇초 후에 Destroy 되도록 해주며,
	// PC의 경우 3분 있다가 마을에서 나오도록 해 준다. 3분 내에는 유저로부터
	// 마을에서 시작할 건지, 여기서 시작할 건지 결정을 받는다.
	if (isDuel == false)
	{
		if (m_pkDeadEvent)
		{
			sys_log(1, "DEAD_EVENT_CANCEL: %s %p %p", GetName(), this, get_pointer(m_pkDeadEvent));
			event_cancel(&m_pkDeadEvent);
		}

		if (IsStone())
			ClearStone();

		if (GetDungeon())
		{
			GetDungeon()->DeadCharacter(this);
		}

		SCharDeadEventInfo* pEventInfo = AllocEventInfo<SCharDeadEventInfo>();

		if (IsPC())
		{
			pEventInfo->isPC = true;
			pEventInfo->dwID = this->GetPlayerID();

			m_pkDeadEvent = event_create(dead_event, pEventInfo, PASSES_PER_SEC(180));
		}
		else
		{
			pEventInfo->isPC = false;
			pEventInfo->dwID = this->GetVID();

			if (IsRevive() == false && HasReviverInParty() == true)
			{
				m_pkDeadEvent = event_create(dead_event, pEventInfo, bImmediateDead ? 1 : PASSES_PER_SEC(5));
			}
			else
			{
				m_pkDeadEvent = event_create(dead_event, pEventInfo, bImmediateDead ? 1 : PASSES_PER_SEC(3));
			}
		}

		sys_log(1, "DEAD_EVENT_CREATE: %s %p %p", GetName(), this, get_pointer(m_pkDeadEvent));
	}

	if (m_pkExchange != nullptr)
		m_pkExchange->Cancel();

	if (IsCubeOpen() == true)
		Cube_close(this);

#ifdef ENABLE_CHANGELOOK_SYSTEM
	if (IsPC())
		ChangeLookWindow(false, true);
#endif

#ifdef ENABLE_ACCE_SYSTEM
	if (IsPC())
		CloseSash();
#endif

#ifdef ENABLE_ITEM_COMBINATION_SYSTEM
	if (IsCombOpen() == true)
		Comb_close(this);
#endif

#ifdef ENABLE_AURA_SYSTEM
	if (IsPC())
		CloseAura();
#endif

	CShopManager::instance().StopShopping(this);
	CloseMyShop();
	CloseSafebox();
#ifdef ENABLE_VERSION_162_UPDATE
	if (IsMonster())
	{
		if ((GetMobTable().dwVnum == TEMPLE_OCHAO_GUARDIAN) && (pkKiller && pkKiller->GetMapIndex() == TEMPLE_OCHAO_MAP_INDEX))
			TempleOchao::CMgr::instance().OnGuardianKilled(pkKiller->GetX(), pkKiller->GetY(), pkKiller->GetZ());
	}
#endif
	if (true == IsMonster() && 2493 == GetMobTable().dwVnum)
	{
		if (nullptr != pkKiller && nullptr != pkKiller->GetGuild())
		{
			CDragonLairManager::instance().OnDragonDead(this, pkKiller->GetGuild()->GetID());
		}
		else
		{
			sys_err("DragonLair: Dragon killed by nobody");
		}
	}
#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if ((IsStone()) || (IsMonster()))
	{
		if (pkKiller)
		{
			if (((GetMobTable().dwVnum == (DWORD)(MeleyLair::MOBVNUM_RESPAWN_STONE_STEP2)) || (GetMobTable().dwVnum == (DWORD)(MeleyLair::MOBVNUM_RESPAWN_BOSS_STEP3))) && (MeleyLair::CMgr::instance().IsMeleyMap(pkKiller->GetMapIndex())))
				MeleyLair::CMgr::instance().OnKill(GetMobTable().dwVnum, pkKiller->GetGuild());
			else if (MeleyLair::CMgr::instance().IsMeleyMap(pkKiller->GetMapIndex()))
				MeleyLair::CMgr::instance().OnKillCommon(this, pkKiller, pkKiller->GetGuild());
		}
	}
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (IsStone())
	{
		if (GetSectree())
		{
			GetSectree()->RemoveEntity(this);
			ViewCleanup();
		}
	}

	if (IsZodiacBoss())
	{
		if (m_pkZodiacSkill1)
			event_cancel(&m_pkZodiacSkill1);

		if (m_pkZodiacSkill2)
			event_cancel(&m_pkZodiacSkill2);

		if (m_pkZodiacSkill3)
			event_cancel(&m_pkZodiacSkill3);

		if (m_pkZodiacSkill4)
			event_cancel(&m_pkZodiacSkill4);

		if (m_pkZodiacSkill5)
			event_cancel(&m_pkZodiacSkill5);

		if (m_pkZodiacSkill6)
			event_cancel(&m_pkZodiacSkill6);

		if (m_pkZodiacSkill7)
			event_cancel(&m_pkZodiacSkill7);

		if (m_pkZodiacSkill8)
			event_cancel(&m_pkZodiacSkill8);

		if (m_pkZodiacSkill9)
			event_cancel(&m_pkZodiacSkill9);

		if (m_pkZodiacSkill10)
			event_cancel(&m_pkZodiacSkill10);

		if (m_pkZodiacSkill11)
			event_cancel(&m_pkZodiacSkill11);
	}
#endif
}

struct FuncSetLastAttacked
{
	FuncSetLastAttacked(DWORD dwTime) : m_dwTime(dwTime)
	{
	}

	void operator () (LPCHARACTER ch)
	{
		ch->SetLastAttacked(m_dwTime);
	}

	DWORD m_dwTime;
};

void CHARACTER::SetLastAttacked(DWORD dwTime)
{
	if (m_pkMobInst != nullptr)
		return;
	
	m_pkMobInst->m_dwLastAttackedTime = dwTime;
	m_pkMobInst->m_posLastAttacked = GetXYZ();
}

void CHARACTER::SendDamagePacket(LPCHARACTER pAttacker, int Damage, BYTE DamageFlag)
{
	if (IsPC() == true || (pAttacker->IsPC() == true && pAttacker->GetTarget() == this))
	{
		TPacketGCDamageInfo damageInfo;
		memset(&damageInfo, 0, sizeof(TPacketGCDamageInfo));

		damageInfo.header = HEADER_GC_DAMAGE_INFO;
		damageInfo.dwVID = (DWORD)GetVID();
		damageInfo.flag = DamageFlag;
		damageInfo.damage = Damage;

		if (GetDesc() != nullptr)
		{
			GetDesc()->Packet(&damageInfo, sizeof(TPacketGCDamageInfo));
		}

		if (pAttacker->GetDesc() != nullptr)
		{
			pAttacker->GetDesc()->Packet(&damageInfo, sizeof(TPacketGCDamageInfo));
		}
		/*
		   if (GetArenaObserverMode() == false && GetArena() != nullptr)
		   {
		   GetArena()->SendPacketToObserver(&damageInfo, sizeof(TPacketGCDamageInfo));
		   }
		 */
	}
}

//
// CHARACTER::Damage 메소드는 this가 데미지를 입게 한다.
//
// Arguments
//    pAttacker		: 공격자
//    dam		: 데미지
//    EDamageType	: 어떤 형식의 공격인가?
//
// Return value
//    true		: dead
//    false		: not dead yet
//
void CHARACTER::Damage_PirateGoldDrop()
{
	if (GetRaceNum() == 5001)
	{
		bool bDropMoney = false;

		int32_t iPercent = 0; // @fixme136
		if (GetMaxHP() >= 0)
			iPercent = (GetHP() * 100) / GetMaxHP();

		if (iPercent <= 10 && GetMaxSP() < 5)
		{
			SetMaxSP(5);
			bDropMoney = true;
		}
		else if (iPercent <= 20 && GetMaxSP() < 4)
		{
			SetMaxSP(4);
			bDropMoney = true;
		}
		else if (iPercent <= 40 && GetMaxSP() < 3)
		{
			SetMaxSP(3);
			bDropMoney = true;
		}
		else if (iPercent <= 60 && GetMaxSP() < 2)
		{
			SetMaxSP(2);
			bDropMoney = true;
		}
		else if (iPercent <= 80 && GetMaxSP() < 1)
		{
			SetMaxSP(1);
			bDropMoney = true;
		}

		if (bDropMoney)
		{
			uint32_t dwGold = 1000;
			int32_t iSplitCount = number(10, 13);

			sys_log(0, "WAEGU DropGoldOnHit %d times", GetMaxSP());

			for (int32_t i = 1; i <= iSplitCount; ++i)
			{
				PIXEL_POSITION pos;
				CItem * item;

				if ((item = ITEM_MANAGER::instance().CreateItem(1, dwGold / iSplitCount)))
				{
					if (i != 0)
					{
						pos.x = (number(-14, 14) + number(-14, 14)) * 20;
						pos.y = (number(-14, 14) + number(-14, 14)) * 20;

						pos.x += GetX();
						pos.y += GetY();
					}

					item->AddToGround(GetMapIndex(), pos);
					item->StartDestroyEvent();
				}
			}
		}
	}
}

bool CHARACTER::Damage(LPCHARACTER pAttacker, int dam, EDamageType type) // returns true if dead
{
	if (!pAttacker)
		return false;

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (IsImmortal())
		return false;
#endif
#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if (pAttacker)
	{
		if ((GetRaceNum() == (WORD)(MeleyLair::STATUE_VNUM)) && (MeleyLair::CMgr::instance().IsMeleyMap(pAttacker->GetMapIndex())) && (!MeleyLair::CMgr::instance().Damage(this, pAttacker->GetGuild())))
		{
			SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
			return false;
		}
		else if ((GetRaceNum() == (WORD)(MeleyLair::BOSS_VNUM)) && (MeleyLair::CMgr::instance().IsMeleyMap(pAttacker->GetMapIndex())))
		{
			SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
			return false;
		}

		else if ((GetRaceNum() == (WORD)(MeleyLair::STATUE_VNUM)))
		{
			if (GetHP() - dam <= 0)
			{
				SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
				return false;
			}
		}
	}
#endif

#ifdef ENABLE_OBSERVER_DAMAGE_FIX
	if (IsPC()) {
		if (IsObserverMode()) {
			return false;
		}
	}
#endif

#ifdef ENABLE_CAPTCHA_SYSTEM
	if (pAttacker)
	{
		if(!IsPC())
		{
			#ifdef ENABLE_AUTO_HUNTING_SYSTEM
			if (nullptr == pAttacker->FindAffect(AFFECT_AUTO_HUNT, APPLY_NONE))
			#endif
			{
				if(get_global_time() > int(pAttacker->GetQuestFlag("captcha.sure")))
				{
								 
					int sans;
					if (pAttacker->GetLevel() >= 75 && pAttacker->GetLevel() < 90)
					{
						sans = number(3600, 3600);
					}
					else
					{
						sans = number(3600, 3600);
					}
				 
					if(int(pAttacker->GetQuestFlag("captcha.durum")) == 0)
					{
						pAttacker->SetQuestFlag("captcha.durum", 1);
						pAttacker->SetQuestFlag("captcha.sure", get_global_time() + (sans));
						int sayi1 = number(1,9);
						int sayi2 = number(1,9);
						int sayi3 = number(1,9);
						int sayi4 = number(1,9);
						int sayi5 = number(1,9);
						long yenicaptcha = (sayi1*10000)+(sayi2*1000)+(sayi3*100)+(sayi4*10)+sayi5;
						pAttacker->ChatPacket(CHAT_TYPE_COMMAND, "captcha %d %d %d %d %d", sayi1, sayi2, sayi3, sayi4, sayi5);
						pAttacker->SetCaptcha(yenicaptcha);
						return true;
					}
				}
				if(int(pAttacker->GetQuestFlag("captcha.durum")) == 1)
				{
					return true;
				}
			}
		}
	}
#endif

	if (DAMAGE_TYPE_MAGIC == type)
	{
		dam = (int)((float)dam * (100 + (pAttacker->GetPoint(POINT_MAGIC_ATT_BONUS_PER) + pAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100.f + 0.5f);
	}
	
	Damage_PirateGoldDrop();

	// 평타가 아닐 때는 공포 처리
	if (type != DAMAGE_TYPE_NORMAL && type != DAMAGE_TYPE_NORMAL_RANGE)
	{
		if (IsAffectFlag(AFF_TERROR))
		{
			int pct = GetSkillPower(SKILL_TERROR) / 400;

			if (number(1, 100) <= pct)
				return false;
		}
	}

	int iCurHP = GetHP();
	int iCurSP = GetSP();

	bool IsCritical = false;
	bool IsPenetrate = false;
	bool IsDeathBlow = false;

#ifdef ENABLE_NEMERE_ADDONS
	if (pAttacker && !IsPC())
	{
		if ((IsMonster() || IsNPC() || IsStone()) && pAttacker->IsPC())
		{
			if (GetRaceNum() == 6151)
			{
				quest::CQuestManager& q = quest::CQuestManager::instance();
				LPDUNGEON pDungeon = q.GetCurrentDungeon();
				if (pDungeon)
				{
					uint32_t dwDungeonLevel = pDungeon->GetFlag("level");

					if (dwDungeonLevel == 4)
					{
						if (!pAttacker->IsAffectFlag(AFF_HOSIN))
						{
							SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
							return false;
						}
					}
					else if (dwDungeonLevel == 7)
					{
						if (!pAttacker->IsAffectFlag(AFF_GICHEON))
						{
							SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
							return false;
						}
					}
				}
			}
			if (GetRaceNum() == 8058)
			{
				if (pAttacker->GetJob() != JOB_SHAMAN)
				{
					SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
					return false;
				}
			}
			if (GetRaceNum() == 20399)
			{
				if (pAttacker->GetJob() == JOB_WARRIOR || pAttacker->GetJob() == JOB_SURA || pAttacker->GetJob() == JOB_WOLFMAN)
				{
					SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
					return false;
				}
			}
		}
	}
#endif

	//PROF_UNIT puAttr("Attr");

	//
	// 마법형 스킬과, 레인지형 스킬은(궁자객) 크리티컬과, 관통공격 계산을 한다.
	// 원래는 하지 않아야 하는데 Nerf(다운밸런스)패치를 할 수 없어서 크리티컬과
	// 관통공격의 원래 값을 쓰지 않고, /2 이상하여 적용한다.
	//
	// 무사 이야기가 많아서 밀리 스킬도 추가
	//
	// 20091109 : 무사가 결과적으로 엄청나게 강해진 것으로 결론남, 독일 기준 무사 비율 70% 육박
	//
	if (type == DAMAGE_TYPE_MELEE || type == DAMAGE_TYPE_RANGE || type == DAMAGE_TYPE_MAGIC)
	{
		if (pAttacker)
		{
			// 크리티컬
			int iCriticalPct = pAttacker->GetPoint(POINT_CRITICAL_PCT);

			if (!IsPC())
				iCriticalPct += pAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_CRITICAL_BONUS);

			if (iCriticalPct)
			{
				if (iCriticalPct >= 10) // 10보다 크면 5% + (4마다 1%씩 증가), 따라서 수치가 50이면 20%
					iCriticalPct = 5 + (iCriticalPct - 10) / 4;
				else // 10보다 작으면 단순히 반으로 깎음, 10 = 5%
					iCriticalPct /= 2;

				//크리티컬 저항 값 적용.
				iCriticalPct -= GetPoint(POINT_RESIST_CRITICAL);

				if (number(1, 100) <= iCriticalPct)
				{
#ifdef ENABLE_CRITICAL_PCT_RENEWAL
					IsCritical = true;
					dam += dam / 2;
					EffectPacket(SE_CRITICAL);
#else
					IsCritical = true;
					dam *= 2;
					EffectPacket(SE_CRITICAL);
#endif

					if (IsAffectFlag(AFF_MANASHIELD))
					{
						RemoveAffect(AFF_MANASHIELD);
					}
				}
			}

			// 관통공격
			int iPenetratePct = pAttacker->GetPoint(POINT_PENETRATE_PCT);

			if (!IsPC())
				iPenetratePct += pAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_PENETRATE_BONUS);

			if (iPenetratePct)
			{
				{
					CSkillProto* pkSk = CSkillManager::instance().Get(SKILL_RESIST_PENETRATE);

					if (nullptr != pkSk)
					{
						pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_RESIST_PENETRATE) / 100.0f);

						iPenetratePct -= static_cast<int>(pkSk->kPointPoly.Eval());
					}
				}

				if (iPenetratePct >= 10)
				{
					// 10보다 크면 5% + (4마다 1%씩 증가), 따라서 수치가 50이면 20%
					iPenetratePct = 5 + (iPenetratePct - 10) / 4;
				}
				else
				{
					// 10보다 작으면 단순히 반으로 깎음, 10 = 5%
					iPenetratePct /= 2;
				}

				//관통타격 저항 값 적용.
				iPenetratePct -= GetPoint(POINT_RESIST_PENETRATE);

				if (number(1, 100) <= iPenetratePct)
				{
					IsPenetrate = true;

					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("관통 추가 데미지 %d"), GetPoint(POINT_DEF_GRADE) * (100 + GetPoint(POINT_DEF_BONUS)) / 100);

					dam += GetPoint(POINT_DEF_GRADE) * (100 + GetPoint(POINT_DEF_BONUS)) / 100;
					EffectPacket(SE_PENETRATE); // delici vurus efekt

					if (IsAffectFlag(AFF_MANASHIELD))
					{
						RemoveAffect(AFF_MANASHIELD);
					}
				}
			}
		}
	}
	//
	// 콤보 공격, 활 공격, 즉 평타 일 때만 속성값들을 계산을 한다.
	//
	else if (type == DAMAGE_TYPE_NORMAL || type == DAMAGE_TYPE_NORMAL_RANGE)
	{
		if (type == DAMAGE_TYPE_NORMAL)
		{
			// 근접 평타일 경우 막을 수 있음
			if (GetPoint(POINT_BLOCK) && number(1, 100) <= GetPoint(POINT_BLOCK))
			{
				if (test_server)
				{
					pAttacker->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 블럭! (%d%%)"), GetName(), GetPoint(POINT_BLOCK));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 블럭! (%d%%)"), GetName(), GetPoint(POINT_BLOCK));
				}

				SendDamagePacket(pAttacker, 0, DAMAGE_BLOCK);
				return false;
			}
		}
		else if (type == DAMAGE_TYPE_NORMAL_RANGE)
		{
			// 원거리 평타의 경우 피할 수 있음
			if (GetPoint(POINT_DODGE) && number(1, 100) <= GetPoint(POINT_DODGE))
			{
				if (test_server)
				{
					pAttacker->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 회피! (%d%%)"), GetName(), GetPoint(POINT_DODGE));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 회피! (%d%%)"), GetName(), GetPoint(POINT_DODGE));
				}

				SendDamagePacket(pAttacker, 0, DAMAGE_DODGE);
				return false;
			}
		}

		if (IsAffectFlag(AFF_JEONGWIHON))
			dam = (int)(dam * (100 + GetSkillPower(SKILL_JEONGWI) * 25 / 100) / 100);

		if (IsAffectFlag(AFF_TERROR))
			dam = (int)(dam * (95 - GetSkillPower(SKILL_TERROR) / 5) / 100);

		if (IsAffectFlag(AFF_HOSIN))
			dam = dam * (100 - GetPoint(POINT_RESIST_NORMAL_DAMAGE)) / 100;

		//
		// 공격자 속성 적용
		//
		if (pAttacker)
		{
			if (type == DAMAGE_TYPE_NORMAL)
			{
				// 반사
				if (GetPoint(POINT_REFLECT_MELEE))
				{
					int reflectDamage = dam * GetPoint(POINT_REFLECT_MELEE) / 100;

					// NOTE: 공격자가 IMMUNE_REFLECT 속성을 갖고있다면 반사를 안 하는 게
					// 아니라 1/3 데미지로 고정해서 들어가도록 기획에서 요청.
					if (pAttacker->IsImmune(IMMUNE_REFLECT))
						reflectDamage = int(reflectDamage / 3.0f + 0.5f);

					pAttacker->Damage(this, reflectDamage, DAMAGE_TYPE_SPECIAL);
				}
			}

			// 크리티컬
			int iCriticalPct = pAttacker->GetPoint(POINT_CRITICAL_PCT);

			if (!IsPC())
				iCriticalPct += pAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_CRITICAL_BONUS);

			if (iCriticalPct)
			{
				//크리티컬 저항 값 적용.
				iCriticalPct -= GetPoint(POINT_RESIST_CRITICAL);

				if (number(1, 100) <= iCriticalPct)
				{
					IsCritical = true;
					dam *= 2;
					EffectPacket(SE_CRITICAL);
				}
			}

			// 관통공격
			int iPenetratePct = pAttacker->GetPoint(POINT_PENETRATE_PCT);

			if (!IsPC())
				iPenetratePct += pAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_PENETRATE_BONUS);

			{
				CSkillProto* pkSk = CSkillManager::instance().Get(SKILL_RESIST_PENETRATE);

				if (nullptr != pkSk)
				{
					pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_RESIST_PENETRATE) / 100.0f);

					iPenetratePct -= static_cast<int>(pkSk->kPointPoly.Eval());
				}
			}

			if (iPenetratePct)
			{
				//관통타격 저항 값 적용.
				iPenetratePct -= GetPoint(POINT_RESIST_PENETRATE);

				if (number(1, 100) <= iPenetratePct)
				{
					IsPenetrate = true;

					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("관통 추가 데미지 %d"), GetPoint(POINT_DEF_GRADE) * (100 + GetPoint(POINT_DEF_BONUS)) / 100);
					dam += GetPoint(POINT_DEF_GRADE) * (100 + GetPoint(POINT_DEF_BONUS)) / 100;
					EffectPacket(SE_PENETRATE); // delici vurus efekt
				}
			}

			if (pAttacker->GetPoint(POINT_STEAL_HP))
			{
				int pct = 1;

				if (number(1, 10) <= pct)
				{
					int iHP = MIN(dam, MAX(0, iCurHP)) * pAttacker->GetPoint(POINT_STEAL_HP) / 100;

					if (iHP > 0 && GetHP() >= iHP)
					{
						CreateFly(FLY_HP_SMALL, pAttacker);
						pAttacker->PointChange(POINT_HP, iHP);
#ifdef ENABLE_EVENT_SYSTEM
						if (CMobVnumHelper::IsShadowWarriorMob(GetRaceNum()) == false || CMobVnumHelper::IsBaashidoWarriorMob(GetRaceNum()) == false || CMobVnumHelper::IsBossHuntersMob(GetRaceNum()) == false)
#endif
							PointChange(POINT_HP, -iHP);
					}
				}
			}

			// SP 스틸
			if (pAttacker->GetPoint(POINT_STEAL_SP))
			{
				int pct = 1;

				if (number(1, 10) <= pct)
				{
					int iCur;

					if (IsPC())
						iCur = iCurSP;
					else
						iCur = iCurHP;

					int iSP = MIN(dam, MAX(0, iCur)) * pAttacker->GetPoint(POINT_STEAL_SP) / 100;

					if (iSP > 0 && iCur >= iSP)
					{
						CreateFly(FLY_SP_SMALL, pAttacker);
						pAttacker->PointChange(POINT_SP, iSP);

						if (IsPC())
							PointChange(POINT_SP, -iSP);
					}
				}
			}

			// 돈 스틸
			if (pAttacker->GetPoint(POINT_STEAL_GOLD))
			{
				if (number(1, 100) <= pAttacker->GetPoint(POINT_STEAL_GOLD))
				{
					int iAmount = number(1, GetLevel());
					pAttacker->PointChange(POINT_GOLD, iAmount);
					DBManager::instance().SendMoneyLog(MONEY_LOG_MISC, 1, iAmount);
				}
			}

			// 칠 때마다 HP회복
			int iAbsoHP_ptr = pAttacker->GetPoint(POINT_HIT_HP_RECOVERY);
			if (iAbsoHP_ptr)
			{
				if (number(1, 100) <= iAbsoHP_ptr)
				{
					int iHPAbso = MIN(dam, GetHP()) * pAttacker->GetPoint(POINT_HIT_HP_RECOVERY) / 100;
					if ((pAttacker->GetHP() > 0) && (pAttacker->GetHP() + iHPAbso < pAttacker->GetMaxHP()) && (GetHP() > 0) && (iHPAbso > 0))
					{
						CreateFly(FLY_HP_SMALL, pAttacker);
						pAttacker->PointChange(POINT_HP, iHPAbso);
					}
				}
			}

			// 칠 때마다 SP회복
			int iAbsoSP_ptr = pAttacker->GetPoint(POINT_HIT_SP_RECOVERY);
			if (iAbsoSP_ptr)
			{
				if (number(1, 100) <= iAbsoSP_ptr)
				{
					int iSPAbso = MIN(dam, GetSP()) * pAttacker->GetPoint(POINT_HIT_SP_RECOVERY) / 100;
					if ((pAttacker->GetSP() > 0) && (pAttacker->GetSP() + iSPAbso < pAttacker->GetMaxSP()) && (GetSP() > 0) && (iSPAbso > 0))
					{
						CreateFly(FLY_SP_SMALL, pAttacker);
						pAttacker->PointChange(POINT_SP, iSPAbso);
					}
				}
			}

			// 상대방의 마나를 없앤다.
			if (pAttacker->GetPoint(POINT_MANA_BURN_PCT))
			{
				if (number(1, 100) <= pAttacker->GetPoint(POINT_MANA_BURN_PCT))
					PointChange(POINT_SP, -50);
			}
		}
	}

	//
	// 평타 또는 스킬로 인한 보너스 피해/방어 계산
	//
	switch (type)
	{
	case DAMAGE_TYPE_NORMAL:
	case DAMAGE_TYPE_NORMAL_RANGE:
		if (pAttacker)
			if (pAttacker->GetPoint(POINT_NORMAL_HIT_DAMAGE_BONUS))
				dam = dam * (100 + pAttacker->GetPoint(POINT_NORMAL_HIT_DAMAGE_BONUS)) / 100;

		dam = dam * (100 - MIN(99, GetPoint(POINT_NORMAL_HIT_DEFEND_BONUS))) / 100;
		break;

	case DAMAGE_TYPE_MELEE:
	case DAMAGE_TYPE_RANGE:
	case DAMAGE_TYPE_FIRE:
	case DAMAGE_TYPE_ICE:
	case DAMAGE_TYPE_ELEC:
	case DAMAGE_TYPE_MAGIC:
		if (pAttacker)
			if (pAttacker->GetPoint(POINT_SKILL_DAMAGE_BONUS))
				dam = dam * (100 + pAttacker->GetPoint(POINT_SKILL_DAMAGE_BONUS)) / 100;

		dam = dam * (100 - MIN(99, GetPoint(POINT_SKILL_DEFEND_BONUS))) / 100;
		break;

	default:
		break;
	}

	//
	// 마나쉴드(흑신수호)
	//
	if (IsAffectFlag(AFF_MANASHIELD))
	{
		// POINT_MANASHIELD 는 작아질수록 좋다
		int iDamageSPPart = dam / 3;
		int iDamageToSP = iDamageSPPart * GetPoint(POINT_MANASHIELD) / 100;
		int iSP = GetSP();

		// SP가 있으면 무조건 데미지 절반 감소
		if (iDamageToSP <= iSP)
		{
			PointChange(POINT_SP, -iDamageToSP);
			dam -= iDamageSPPart;
		}
		else
		{
			// 정신력이 모자라서 피가 더 깍여야할떄
			PointChange(POINT_SP, -GetSP());
			dam -= iSP * 100 / MAX(GetPoint(POINT_MANASHIELD), 1);
		}
	}

	//
	// 전체 방어력 상승 (몰 아이템)
	//
	if (GetPoint(POINT_MALL_DEFBONUS) > 0)
	{
		int dec_dam = MIN(200, dam * GetPoint(POINT_MALL_DEFBONUS) / 100);
		dam -= dec_dam;
	}

	if (pAttacker)
	{
		//
		// 전체 공격력 상승 (몰 아이템)
		//
		if (pAttacker->GetPoint(POINT_MALL_ATTBONUS) > 0)
		{
			int add_dam = MIN(300, dam * pAttacker->GetLimitPoint(POINT_MALL_ATTBONUS) / 100);
			dam += add_dam;
		}

		if (pAttacker->IsPC())
		{
			int iEmpire = pAttacker->GetEmpire();
			long lMapIndex = pAttacker->GetMapIndex();
			int iMapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(lMapIndex);

			// 다른 제국 사람인 경우 데미지 10% 감소
			if (iEmpire && iMapEmpire && iEmpire != iMapEmpire)
			{
				dam = dam * 9 / 10;
			}

			if (!IsPC() && GetMonsterDrainSPPoint())
			{
				int iDrain = GetMonsterDrainSPPoint();

				if (iDrain <= pAttacker->GetSP())
					pAttacker->PointChange(POINT_SP, -iDrain);
				else
				{
					int iSP = pAttacker->GetSP();
					pAttacker->PointChange(POINT_SP, -iSP);
				}
			}
		}
		else if (pAttacker->IsGuardNPC())
		{
			SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_NO_REWARD);
			Stun();
			return true;
		}
		if (pAttacker->IsMonster())
		{
			int resistType = 0;
			// Lighting resistance
			if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_ELEC)) {
				resistType = POINT_RESIST_ELEC;
			}
			// Fire resistance
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_FIRE)) {
				resistType = POINT_RESIST_FIRE;
			}
			// Ice resistance
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_ICE)) {
				resistType = POINT_RESIST_ICE;
			}
			// Wind resistance
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_WIND)) {
				resistType = POINT_RESIST_WIND;
			}
			// Earth resistance
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_EARTH)) {
				resistType = POINT_RESIST_EARTH;
			}
			// Dark resistance
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_DARK)) {
				resistType = POINT_RESIST_DARK;
			}
#ifdef ENABLE_MUSHROOM_DUNGEON
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_MUSHROOM)) {
				resistType = POINT_RESIST_MUSHROOM;
			}
#endif
#ifdef ENABLE_WATER_DUNGEON
			else if (pAttacker->IsRaceFlag(RACE_FLAG_ATT_WATER)) {
				resistType = POINT_RESIST_WATER;
			}
#endif

			if (resistType && GetPoint(resistType) > 0) {
				int result = (int)GetPoint(resistType);
				if (result > MAX_DEFENSE_PCT) {
					result = MAX_DEFENSE_PCT;
				}

				dam -= dam * result / 100;
			}
		}
	}
	//puAttr.Pop();

	if (!GetSectree() || GetSectree()->IsAttr(GetX(), GetY(), ATTR_BANPK))
		return false;

	if (!IsPC())
	{
		if (m_pkParty && m_pkParty->GetLeader())
			m_pkParty->GetLeader()->SetLastAttacked(get_dword_time());
		else
			SetLastAttacked(get_dword_time());

		// 몬스터 대사 : 맞을 때
		MonsterChat(MONSTER_CHAT_ATTACKED);
	}

	if (IsStun())
	{
		Dead(pAttacker);
		return true;
	}

	if (IsDead())
		return true;

	// 독 공격으로 죽지 않도록 함.
	if (type == DAMAGE_TYPE_POISON)
	{
		if (GetHP() - dam <= 0)
		{
			dam = GetHP() - 1;
		}
	}

	// ------------------------
	// 독일 프리미엄 모드
	// -----------------------
	if (pAttacker && pAttacker->IsPC())
	{
		int iDmgPct = CHARACTER_MANAGER::instance().GetUserDamageRate(pAttacker);
		dam = dam * iDmgPct / 100;
	}

	// STONE SKIN : 피해 반으로 감소
	if (IsMonster() && IsStoneSkinner())
	{
		if (GetHPPct() < GetMobTable().bStoneSkinPoint)
			dam /= 2;
	}

	//PROF_UNIT puRest1("Rest1");
	if (pAttacker)
	{
		// DEATH BLOW : 확률 적으로 4배 피해 (!? 현재 이벤트나 공성전용 몬스터만 사용함)
#ifdef ENABLE_MONSTER_DEATHBLOW_RENEWAL
		if (pAttacker->IsMonster() && pAttacker->IsDeathBlower())
		{
			if (pAttacker->IsDeathBlow())
			{
				int32_t rate = 2*MINMAX(1, 51 - pAttacker->GetHPPct(), 50);
				if (GetPoint(POINT_BLOCK) > 0 && number(1, 100) <= GetPoint(POINT_BLOCK)) {
					rate -= MINMAX(1, GetPoint(POINT_BLOCK), 75);
				}
				
				IsDeathBlow = true;
				dam *= static_cast<int32_t>(1.5 + 1.5 * rate/100.0);
			}
		}
#else
		if (pAttacker->IsMonster() && pAttacker->IsDeathBlower())
		{
			if (pAttacker->IsDeathBlow())
			{
				if (number(1, 4) == GetJob())
				{
					IsDeathBlow = true;
					dam = dam * 4;
				}
			}
		}
#endif

		dam = BlueDragon_Damage(this, pAttacker, dam);

		BYTE damageFlag = 0;

		if (type == DAMAGE_TYPE_POISON)
			damageFlag = DAMAGE_POISON;
		else if (type == DAMAGE_TYPE_BLEEDING)
			damageFlag = DAMAGE_BLEEDING;
		else if (type == DAMAGE_TYPE_FIRE)
			damageFlag = DAMAGE_FIRE;
		else
			damageFlag = DAMAGE_NORMAL;

		if (IsCritical == true)
			damageFlag |= DAMAGE_CRITICAL;

		if (IsPenetrate == true)
			damageFlag |= DAMAGE_PENETRATE;

		//최종 데미지 보정
		float damMul = this->GetDamMul();
		float tempDam = dam;
#ifdef ENABLE_ATTENDANCE_EVENT
		dam = (GetRaceNum() >= 6415 && GetRaceNum() <= 6419) ? 1 : tempDam * damMul + 0.5f;
#else
		dam = tempDam * damMul + 0.5f;
#endif

#ifdef ENABLE_EVENT_SYSTEM
		if (CMobVnumHelper::IsShadowWarriorMob(GetRaceNum()) == true)
		{
			dam = 1;
			if (pAttacker)
				pAttacker->PointChange(POINT_YEAR_ROUND, dam);
		}

		if (CMobVnumHelper::IsBaashidoWarriorMob(GetRaceNum()) == true)
			dam = 1;
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		if ((IsMonster() || IsStone()) && (pAttacker && pAttacker->IsPC()))
		{
			if (IsZodiacStone())
			{
				dam = 1;

				if (number(1, 30) == 15)
				{
					LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(pAttacker->GetMapIndex());
					if (pZodiac)
					{
						SpawnZodiacStone(pZodiac);
					}
				}
			}
			else if (GetRaceNum() == 20464)
			{
				dam = 1;
				KanoneDamage();
			}
			else if (GetRaceNum() >= 2900 && GetRaceNum() <= 2908)
			{
				if (number(0, 160) <= 7)
				{
					LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(pAttacker->GetMapIndex());
					if (pZodiac)
					{
						SpawnZodiacGroup(pZodiac);
					}
				}
			}
		}
#endif

		if (pAttacker)
			SendDamagePacket(pAttacker, dam, damageFlag);

		if (test_server)
		{
			int iTmpPercent = 0;
			if (GetMaxHP() >= 0)
				iTmpPercent = (GetHP() * 100) / GetMaxHP();
			if (pAttacker)
			{
				pAttacker->ChatPacket(CHAT_TYPE_INFO, "-> %s, DAM %d HP %d(%d%%) %s%s",
					GetName(),
					dam,
					GetHP(),
					iTmpPercent,
					IsCritical ? "crit " : "",
					IsPenetrate ? "pene " : "",
					IsDeathBlow ? "deathblow " : "");
			}

			ChatPacket(CHAT_TYPE_PARTY, "<- %s, DAM %d HP %d(%d%%) %s%s",
				pAttacker ? pAttacker->GetName() : 0,
				dam,
				GetHP(),
				iTmpPercent,
				IsCritical ? "crit " : "",
				IsPenetrate ? "pene " : "",
				IsDeathBlow ? "deathblow " : "");
		}

		if (m_bDetailLog)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s[%d]가 공격 위치: %d %d"), pAttacker->GetName(), (DWORD)pAttacker->GetVID(), pAttacker->GetX(), pAttacker->GetY());
		}
	}

	//
	// !!!!!!!!! 실제 HP를 줄이는 부분 !!!!!!!!!
	//
	if (!cannot_dead)
	{
		if (GetHP() - dam <= 0) // @fixme137
			dam = GetHP();

		PointChange(POINT_HP, -dam, false);
	}

	//puRest1.Pop();

	//PROF_UNIT puRest2("Rest2");
	if (pAttacker && dam > 0 && IsNPC())
	{
		//PROF_UNIT puRest20("Rest20");
		TDamageMap::iterator it = m_map_kDamage.find(pAttacker->GetVID());

		if (it == m_map_kDamage.end())
		{
			m_map_kDamage.insert(TDamageMap::value_type(pAttacker->GetVID(), TBattleInfo(dam, 0)));
			it = m_map_kDamage.find(pAttacker->GetVID());
		}
		else
		{
			it->second.iTotalDamage += dam;
		}
		//puRest20.Pop();

		//PROF_UNIT puRest21("Rest21");
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		if (GetRaceNum() != 20434)
			StartRecoveryEvent();
#else
		StartRecoveryEvent();
#endif
		//puRest21.Pop();

		//PROF_UNIT puRest22("Rest22");
#ifdef ENABLE_POISON_UNAGGR
		if (type != DAMAGE_TYPE_POISON)
			UpdateAggrPointEx(pAttacker, type, dam, it->second);
#else
		UpdateAggrPointEx(pAttacker, type, dam, it->second);
#endif
		//puRest22.Pop();
	}
	//puRest2.Pop();

	//PROF_UNIT puRest3("Rest3");
	if (GetHP() <= 0)
	{
#ifdef ENABLE_STUN_RENEWAL
		if (IsPC() || IsStone())
			Stun();
		else
			Dead();
#else
		Stun();
#endif

		if (pAttacker && !pAttacker->IsNPC())
			m_dwKillerPID = pAttacker->GetPlayerID();
		else
			m_dwKillerPID = 0;
	}

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
	if (GetRaceNum() == 20434)
		if (GetDungeon())
			GetDungeon()->UpdateMastHP();
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (pAttacker && pAttacker->GetRaceNum() == 20464)
	{
		EffectPacket(SE_DEAPO_BOOM);
		pAttacker->Dead();
	}
#endif

	return false;
}

void CHARACTER::DistributeHP(LPCHARACTER pkKiller)
{
	if (pkKiller->GetDungeon()) // 던젼내에선 만두가나오지않는다
		return;
}

#define NEW_GET_LVDELTA(me, victim) aiPercentByDeltaLev[MINMAX(0, (victim + 15) - me, MAX_EXP_DELTA_OF_LEV - 1)]
typedef long double rate_t;
static void GiveExp(LPCHARACTER from, LPCHARACTER to, int iExp)
{
	if (test_server && iExp < 0)
	{
		to->ChatPacket(CHAT_TYPE_INFO, "exp(%d) overflow", iExp);
		return;
	}
	// decrease/increase exp based on player<>mob level
	rate_t lvFactor = static_cast<rate_t>(NEW_GET_LVDELTA(to->GetLevel(), from->GetLevel())) / 100.0L;
	iExp *= lvFactor;
	// start calculating rate exp bonus
	int iBaseExp = iExp;
	rate_t rateFactor = 100;

	rateFactor += CPrivManager::instance().GetPriv(to, PRIV_EXP_PCT);
	if (to->IsEquipUniqueItem(UNIQUE_ITEM_LARBOR_MEDAL))
		rateFactor += 20;
	if (to->GetMapIndex() >= 660000 && to->GetMapIndex() < 670000)
		rateFactor += 20;
	if (to->GetPoint(POINT_EXP_DOUBLE_BONUS))
		if (number(1, 100) <= to->GetPoint(POINT_EXP_DOUBLE_BONUS))
			rateFactor += 30;
	if (to->IsEquipUniqueItem(UNIQUE_ITEM_DOUBLE_EXP))
		rateFactor += 50;
	if (to->IsEquipUniqueItem(UNIQUE_ITEM_DOUBLE_EXP_PLUS))
		rateFactor += 200;

	switch (to->GetMountVnum())
	{
	case 20110:
	case 20111:
	case 20112:
	case 20113:
		if (to->IsEquipUniqueItem(71115) || to->IsEquipUniqueItem(71117) || to->IsEquipUniqueItem(71119) ||
			to->IsEquipUniqueItem(71121))
		{
			rateFactor += 10;
		}
		break;

	case 20114:
	case 20120:
	case 20121:
	case 20122:
	case 20123:
	case 20124:
	case 20125:
		rateFactor += 30;
		break;
	}
	if (to->GetPremiumRemainSeconds(PREMIUM_EXP) > 0)
		rateFactor += 50;
	if (to->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_EXP))
		rateFactor += 50;
#ifdef ENABLE_VALUE_PACK_SYSTEM
	if (to->IsAffectFlag(AFF_PREMIUM))
		rateFactor += 50;
#endif
	rateFactor += to->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_EXP_BONUS);
	rateFactor += to->GetPoint(POINT_RAMADAN_CANDY_BONUS_EXP);
	rateFactor += to->GetPoint(POINT_MALL_EXPBONUS);
	rateFactor += to->GetPoint(POINT_EXP);
	// useless (never used except for china intoxication) = always 100
	rateFactor = rateFactor * static_cast<rate_t>(CHARACTER_MANAGER::instance().GetMobExpRate(to)) / 100.0L;
	// apply calculated rate bonus
	iExp *= (rateFactor / 100.0L);
	if (test_server)
		to->ChatPacket(CHAT_TYPE_INFO, "base_exp(%d) * rate(%Lf) = exp(%d)", iBaseExp, rateFactor / 100.0L, iExp);
	// you can get at maximum only 10% of the total required exp at once (so, you need to kill at least 10 mobs to level up) (useless)
	iExp = MIN(to->GetNextExp() / 10, iExp);
	// it recalculate the given exp if the player level is greater than the exp_table size (useless)
	iExp = AdjustExpByLevel(to, iExp);

	if (test_server)
		to->ChatPacket(CHAT_TYPE_INFO, "exp+minGNE+adjust(%d)", iExp);

#ifdef EXP_MULTIPLER
	iExp *= EXP_MULTIPLER;
#endif

	// set
	if (quest::CQuestManager::instance().GetEventFlag("exp_bonus_log") && iBaseExp > 0)
		to->ChatPacket(CHAT_TYPE_INFO, "exp bonus %d%%", (iExp - iBaseExp) * 100 / iBaseExp);

	if (!to->IsEquipUniqueItem(ITEM_ANTIEXP_RING))
	{
		to->PointChange(POINT_EXP, iExp, true);
		from->CreateFly(FLY_EXP, to);
	}

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (to->GetNewPetSystem()) {
		if (to->GetNewPetSystem()->IsActivePet() && to->GetNewPetSystem()->GetLevelStep() < 4) {
			int tmpexp = iExp * 9 / 20;
			if (to->GetQuestFlag("yuzuk_80051.petfullexp") == 1)
				tmpexp = 0;
			iExp = iExp - tmpexp;
			to->GetNewPetSystem()->SetExp(tmpexp, 0);
		}
	}
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	if (to->GetSupportShaman())
	{
		if (to->GetSupportShaman()->IsActiveSupportShaman() && to->GetSupportShaman()->GetLevel() < 120)
		{
			int tmpexp = iExp * 9 / 20;
			iExp = iExp - tmpexp;
			to->GetSupportShaman()->SetExp(tmpexp);
		}
	}
#endif
	// marriage
	{
		LPCHARACTER you = to->GetMarryPartner();
		// 부부가 서로 파티중이면 금슬이 오른다
		if (you)
		{
			// sometimes, this overflows
			DWORD dwUpdatePoint = (2000.0L / to->GetLevel() / to->GetLevel() / 3) * iExp;
			dwUpdatePoint *= 3;

			marriage::TMarriage * pMarriage = marriage::CManager::instance().Get(to->GetPlayerID());

			// DIVORCE_NULL_BUG_FIX
			if (pMarriage && pMarriage->IsNear())
				pMarriage->Update(dwUpdatePoint);
			// END_OF_DIVORCE_NULL_BUG_FIX
		}
	}
}

namespace NPartyExpDistribute
{
	struct FPartyTotaler
	{
		int		total;
		int		member_count;
		int		x, y;

		FPartyTotaler(LPCHARACTER center)
			: total(0), member_count(0), x(center->GetX()), y(center->GetY())
		{};

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
			{
				total += __GetPartyExpNP(ch->GetLevel());
				++member_count;
			}
		}
	};

	struct FPartyDistributor
	{
		int		total;
		LPCHARACTER	c;
		int		x, y;
		DWORD		_iExp;
		int		m_iMode;
		int		m_iMemberCount;

		FPartyDistributor(LPCHARACTER center, int member_count, int total, DWORD iExp, int iMode)
			: total(total), c(center), x(center->GetX()), y(center->GetY()), _iExp(iExp), m_iMode(iMode), m_iMemberCount(member_count)
		{
			if (m_iMemberCount == 0)
				m_iMemberCount = 1;
		};

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
			{
				DWORD iExp2 = 0;

				switch (m_iMode)
				{
				case PARTY_EXP_DISTRIBUTION_NON_PARITY:
					iExp2 = (DWORD)(_iExp * (float)__GetPartyExpNP(ch->GetLevel()) / total);
					break;

				case PARTY_EXP_DISTRIBUTION_PARITY:
					iExp2 = _iExp / m_iMemberCount;
					break;

				default:
					sys_err("Unknown party exp distribution mode %d", m_iMode);
					return;
				}

				GiveExp(c, ch, iExp2);
			}
		}
	};
}

typedef struct SDamageInfo
{
	int iDam;
	LPCHARACTER pAttacker;
	LPPARTY pParty;

	void Clear()
	{
		pAttacker = nullptr;
		pParty = nullptr;
	}

	inline void Distribute(LPCHARACTER ch, int iExp)
	{
		if (pAttacker)
			GiveExp(ch, pAttacker, iExp);
		else if (pParty)
		{
			NPartyExpDistribute::FPartyTotaler f(ch);
			pParty->ForEachOnlineMember(f);

			if (pParty->IsPositionNearLeader(ch))
				iExp = iExp * (100 + pParty->GetExpBonusPercent()) / 100;

			if (test_server)
			{
				if (quest::CQuestManager::instance().GetEventFlag("exp_bonus_log") && pParty->GetExpBonusPercent())
					pParty->ChatPacketToAllMember(CHAT_TYPE_INFO, "exp party bonus %d%%", pParty->GetExpBonusPercent());
			}

			// 경험치 몰아주기 (파티가 획득한 경험치를 5% 빼서 먼저 줌)
			if (pParty->GetExpCentralizeCharacter())
			{
				LPCHARACTER tch = pParty->GetExpCentralizeCharacter();

				if (DISTANCE_APPROX(ch->GetX() - tch->GetX(), ch->GetY() - tch->GetY()) <= PARTY_DEFAULT_RANGE)
				{
					int iExpCenteralize = (int)(iExp * 0.05f);
					iExp -= iExpCenteralize;

					GiveExp(ch, pParty->GetExpCentralizeCharacter(), iExpCenteralize);
				}
			}

			NPartyExpDistribute::FPartyDistributor fDist(ch, f.member_count, f.total, iExp, pParty->GetExpDistributionMode());
			pParty->ForEachOnlineMember(fDist);
		}
	}
} TDamageInfo;

LPCHARACTER CHARACTER::DistributeExp()
{
	int iExpToDistribute = GetExp();

	if (iExpToDistribute <= 0)
		return nullptr;

	int	iTotalDam = 0;
	LPCHARACTER pkChrMostAttacked = nullptr;
	int iMostDam = 0;

	typedef std::vector<TDamageInfo> TDamageInfoTable;
	TDamageInfoTable damage_info_table;
	std::map<LPPARTY, TDamageInfo> map_party_damage;

	damage_info_table.reserve(m_map_kDamage.size());

	TDamageMap::iterator it = m_map_kDamage.begin();

	// 일단 주위에 없는 사람을 걸러 낸다. (50m)
	while (it != m_map_kDamage.end())
	{
		const VID& c_VID = it->first;
		int iDam = it->second.iTotalDamage;

		++it;

#ifdef ENABLE_PARTY_EXP_FIX
		LPCHARACTER pAttacker = CHARACTER_MANAGER::instance().Find(c_VID);

		// NPC가 때리기도 하나? -.-;
		if(!pAttacker || !pAttacker->IsPC())
			continue;

		// Block exp award based on distance
		// Groups have DOUBLE the exp distance to prevent the "exploit" where the party leader gets full exp bonus and some party members don't get exp.
		int dist = DISTANCE_APPROX(GetX()-pAttacker->GetX(), GetY()-pAttacker->GetY());
		if(dist > 10000 || (dist > 5000 && !pAttacker->GetParty()))
			continue;
#else
		LPCHARACTER pAttacker = CHARACTER_MANAGER::instance().Find(c_VID);

		// NPC가 때리기도 하나? -.-;
		if (!pAttacker || pAttacker->IsNPC() || DISTANCE_APPROX(GetX() - pAttacker->GetX(), GetY() - pAttacker->GetY()) > 5000)
			continue;
#endif

		iTotalDam += iDam;
		if (!pkChrMostAttacked || iDam > iMostDam)
		{
			pkChrMostAttacked = pAttacker;
			iMostDam = iDam;
		}

		if (pAttacker->GetParty())
		{
			std::map<LPPARTY, TDamageInfo>::iterator it = map_party_damage.find(pAttacker->GetParty());
			if (it == map_party_damage.end())
			{
				TDamageInfo di;
				di.iDam = iDam;
				di.pAttacker = nullptr;
				di.pParty = pAttacker->GetParty();
				map_party_damage.insert(std::make_pair(di.pParty, di));
			}
			else
			{
				it->second.iDam += iDam;
			}
		}
		else
		{
			TDamageInfo di;

			di.iDam = iDam;
			di.pAttacker = pAttacker;
			di.pParty = nullptr;

			//sys_log(0, "__ pq_damage %s %d", pAttacker->GetName(), iDam);
			//pq_damage.push(di);
			damage_info_table.push_back(di);
		}
	}

	for (std::map<LPPARTY, TDamageInfo>::iterator it = map_party_damage.begin(); it != map_party_damage.end(); ++it)
	{
		damage_info_table.push_back(it->second);
		//sys_log(0, "__ pq_damage_party [%u] %d", it->second.pParty->GetLeaderPID(), it->second.iDam);
	}

	SetExp(0);
	//m_map_kDamage.clear();

	if (iTotalDam == 0)	// 데미지 준게 0이면 리턴
		return nullptr;

	if (m_pkChrStone)	// 돌이 있을 경우 경험치의 반을 돌에게 넘긴다.
	{
		//sys_log(0, "__ Give half to Stone : %d", iExpToDistribute>>1);
		int iExp = iExpToDistribute >> 1;
		m_pkChrStone->SetExp(m_pkChrStone->GetExp() + iExp);
		iExpToDistribute -= iExp;
	}

	sys_log(1, "%s total exp: %d, damage_info_table.size() == %d, TotalDam %d",
		GetName(), iExpToDistribute, damage_info_table.size(), iTotalDam);
	//sys_log(1, "%s total exp: %d, pq_damage.size() == %d, TotalDam %d",
	//GetName(), iExpToDistribute, pq_damage.size(), iTotalDam);

	if (damage_info_table.empty())
		return nullptr;

	// 제일 데미지를 많이 준 사람이 HP 회복을 한다.
	DistributeHP(pkChrMostAttacked);	// 만두 시스템

	{
		// 제일 데미지를 많이 준 사람이나 파티가 총 경험치의 20% + 자기가 때린만큼의 경험치를 먹는다.
		TDamageInfoTable::iterator di = damage_info_table.begin();
		{
			TDamageInfoTable::iterator it;

			for (it = damage_info_table.begin(); it != damage_info_table.end(); ++it)
			{
				if (it->iDam > di->iDam)
					di = it;
			}
		}

		int	iExp = iExpToDistribute / 5;
		iExpToDistribute -= iExp;

		float fPercent = (float)di->iDam / iTotalDam;

		if (fPercent > 1.0f)
		{
			sys_err("DistributeExp percent over 1.0 (fPercent %f name %s)", fPercent, di->pAttacker->GetName());
			fPercent = 1.0f;
		}

		iExp += (int)(iExpToDistribute * fPercent);

		//sys_log(0, "%s given exp percent %.1f + 20 dam %d", GetName(), fPercent * 100.0f, di.iDam);

		di->Distribute(this, iExp);

		// 100% 다 먹었으면 리턴한다.
		if (fPercent == 1.0f)
			return pkChrMostAttacked;

		di->Clear();
	}

	{
		// 남은 80%의 경험치를 분배한다.
		TDamageInfoTable::iterator it;

		for (it = damage_info_table.begin(); it != damage_info_table.end(); ++it)
		{
			TDamageInfo& di = *it;

			float fPercent = (float)di.iDam / iTotalDam;

			if (fPercent > 1.0f)
			{
				sys_err("DistributeExp percent over 1.0 (fPercent %f name %s)", fPercent, di.pAttacker->GetName());
				fPercent = 1.0f;
			}

			//sys_log(0, "%s given exp percent %.1f dam %d", GetName(), fPercent * 100.0f, di.iDam);
			di.Distribute(this, (int)(iExpToDistribute * fPercent));
		}
	}

	return pkChrMostAttacked;
}

// 화살 개수를 리턴해 줌
int CHARACTER::GetArrowAndBow(LPITEM * ppkBow, LPITEM * ppkArrow, int iArrowCount)
{
	LPITEM pkBow;
	if (!(pkBow = GetWear(WEAR_WEAPON)) || pkBow->GetProto()->bSubType != WEAPON_BOW)
	{
		return 0;
	}

	LPITEM pkArrow;
#ifdef ENABLE_QUIVER_SYSTEM
	if (!(pkArrow = GetWear(WEAR_ARROW)) || pkArrow->GetType() != ITEM_WEAPON || (pkArrow->GetSubType() != WEAPON_ARROW && pkArrow->GetSubType() != WEAPON_QUIVER))
#else
	if (!(pkArrow = GetWear(WEAR_ARROW)) || pkArrow->GetType() != ITEM_WEAPON || pkArrow->GetSubType() != WEAPON_ARROW)
#endif
	{
		return 0;
	}

#ifdef ENABLE_QUIVER_SYSTEM
	if (pkArrow->GetSubType() == WEAPON_QUIVER)
		iArrowCount = MIN(iArrowCount, pkArrow->GetSocket(0) - time(0));
	else
		iArrowCount = MIN(iArrowCount, pkArrow->GetCount());
#else
	iArrowCount = MIN(iArrowCount, pkArrow->GetCount());
#endif

	* ppkBow = pkBow;
	*ppkArrow = pkArrow;
	return iArrowCount;
}

void CHARACTER::UseArrow(LPITEM pkArrow, DWORD dwArrowCount)
{
	int iCount = pkArrow->GetCount();
#ifdef ENABLE_QUIVER_SYSTEM
	if (pkArrow->GetSubType() == WEAPON_QUIVER)
		return;
#endif
	DWORD dwVnum = pkArrow->GetVnum();
	iCount = iCount - MIN(iCount, dwArrowCount);
	pkArrow->SetCount(iCount);
	if (iCount == 0)
	{
		LPITEM pkNewArrow = FindSpecifyItem(dwVnum);
		sys_log(0, "UseArrow : FindSpecifyItem %u %p", dwVnum, get_pointer(pkNewArrow));
		if (pkNewArrow)
			EquipItem(pkNewArrow);
	}
}

class CFuncShoot
{
public:
	LPCHARACTER	m_me;
	BYTE		m_bType;
	bool		m_bSucceed;

	CFuncShoot(LPCHARACTER ch, BYTE bType) : m_me(ch), m_bType(bType), m_bSucceed(FALSE)
	{
	}

	void operator () (DWORD dwTargetVID)
	{
		if (m_bType > 1)
		{
			if (g_bSkillDisable)
				return;

			m_me->m_SkillUseInfo[m_bType].SetMainTargetVID(dwTargetVID);
			/*if (m_bType == SKILL_BIPABU || m_bType == SKILL_KWANKYEOK)
			  m_me->m_SkillUseInfo[m_bType].ResetHitCount();*/
		}

		LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(dwTargetVID);

		if (!pkVictim)
			return;

		// 공격 불가
		if (!battle_is_attackable(m_me, pkVictim))
			return;

		if (m_me->IsNPC())
		{
			if (DISTANCE_APPROX(m_me->GetX() - pkVictim->GetX(), m_me->GetY() - pkVictim->GetY()) > 5000)
				return;
		}

		LPITEM pkBow, pkArrow;
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		int reduce_resist_magic = 0;
#endif
		switch (m_bType)
		{
		case 0: // 일반활
		{
			int iDam = 0;

			if (m_me->IsPC())
			{
				if (m_me->GetJob() != JOB_ASSASSIN)
					return;

				if (0 == m_me->GetArrowAndBow(&pkBow, &pkArrow))
					return;

				if (m_me->GetSkillGroup() != 0)
					if (!m_me->IsNPC() && m_me->GetSkillGroup() != 2)
					{
						if (m_me->GetSP() < 5)
							return;

						m_me->PointChange(POINT_SP, -5);
					}

				iDam = CalcArrowDamage(m_me, pkVictim, pkBow, pkArrow);
				m_me->UseArrow(pkArrow, 1);
			}
			else
				iDam = CalcMeleeDamage(m_me, pkVictim);

			NormalAttackAffect(m_me, pkVictim);

			// 데미지 계산
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_BOW)) / 100;

			//sys_log(0, "%s arrow %s dam %d", m_me->GetName(), pkVictim->GetName(), iDam);

			m_me->OnMove(true);
			pkVictim->OnMove();

			if (pkVictim->CanBeginFight())
				pkVictim->BeginFight(m_me);

			pkVictim->Damage(m_me, iDam, DAMAGE_TYPE_NORMAL_RANGE);
			// 타격치 계산부 끝
		}
		break;

		case 1: // 일반 마법
		{
			int iDam;

			if (m_me->IsPC())
				return;

			iDam = CalcMagicDamage(m_me, pkVictim);

			NormalAttackAffect(m_me, pkVictim);
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
			reduce_resist_magic = pkVictim->GetPoint(POINT_RESIST_MAGIC);
			if (m_me->GetPoint(POINT_ANTI_RESIST_MAGIC) > 0)
			{
				// Fix Magic Resistance : If player have more than 100% magic resistance, will be modified to 100%;
				int fix_magic_resistance = (pkVictim->GetPoint(POINT_RESIST_MAGIC) > 100) ? (int)(100) : (int)(pkVictim->GetPoint(POINT_RESIST_MAGIC));
				// End Fix;

				reduce_resist_magic = fix_magic_resistance - m_me->GetPoint(POINT_ANTI_RESIST_MAGIC);
				if (reduce_resist_magic < 1)
					reduce_resist_magic = 0;
			}
			iDam = iDam * (100 - reduce_resist_magic) / 100;
#else
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_MAGIC)) / 100;
#endif
			//sys_log(0, "%s arrow %s dam %d", m_me->GetName(), pkVictim->GetName(), iDam);

			m_me->OnMove(true);
			pkVictim->OnMove();

			if (pkVictim->CanBeginFight())
				pkVictim->BeginFight(m_me);

			pkVictim->Damage(m_me, iDam, DAMAGE_TYPE_MAGIC);
			// 타격치 계산부 끝
		}
		break;

		case SKILL_YEONSA:	// 연사
		{
			//int iUseArrow = 2 + (m_me->GetSkillPower(SKILL_YEONSA) *6/100);
			int iUseArrow = 1;

			// 토탈만 계산하는경우
			{
				if (iUseArrow == m_me->GetArrowAndBow(&pkBow, &pkArrow, iUseArrow))
				{
					m_me->OnMove(true);
					pkVictim->OnMove();

					if (pkVictim->CanBeginFight())
						pkVictim->BeginFight(m_me);

					m_me->ComputeSkill(m_bType, pkVictim);
					m_me->UseArrow(pkArrow, iUseArrow);

					if (pkVictim->IsDead())
						break;
				}
				else
					break;
			}
		}
		break;

		case SKILL_KWANKYEOK:
		{
			int iUseArrow = 1;

			if (iUseArrow == m_me->GetArrowAndBow(&pkBow, &pkArrow, iUseArrow))
			{
				m_me->OnMove(true);
				pkVictim->OnMove();

				if (pkVictim->CanBeginFight())
					pkVictim->BeginFight(m_me);

				sys_log(0, "%s kwankeyok %s", m_me->GetName(), pkVictim->GetName());
				m_me->ComputeSkill(m_bType, pkVictim);
				m_me->UseArrow(pkArrow, iUseArrow);
			}
		}
		break;

		case SKILL_GIGUNG:
		{
			int iUseArrow = 1;
			if (iUseArrow == m_me->GetArrowAndBow(&pkBow, &pkArrow, iUseArrow))
			{
				m_me->OnMove(true);
				pkVictim->OnMove();

				if (pkVictim->CanBeginFight())
					pkVictim->BeginFight(m_me);

				sys_log(0, "%s gigung %s", m_me->GetName(), pkVictim->GetName());
				m_me->ComputeSkill(m_bType, pkVictim);
				m_me->UseArrow(pkArrow, iUseArrow);
			}
		}

		break;
		case SKILL_HWAJO:
		{
			int iUseArrow = 1;
			if (iUseArrow == m_me->GetArrowAndBow(&pkBow, &pkArrow, iUseArrow))
			{
				m_me->OnMove(true);
				pkVictim->OnMove();

				if (pkVictim->CanBeginFight())
					pkVictim->BeginFight(m_me);

				sys_log(0, "%s hwajo %s", m_me->GetName(), pkVictim->GetName());
				m_me->ComputeSkill(m_bType, pkVictim);
				m_me->UseArrow(pkArrow, iUseArrow);
			}
		}

		break;

		case SKILL_HORSE_WILDATTACK_RANGE:
		{
			int iUseArrow = 1;
			if (iUseArrow == m_me->GetArrowAndBow(&pkBow, &pkArrow, iUseArrow))
			{
				m_me->OnMove(true);
				pkVictim->OnMove();

				if (pkVictim->CanBeginFight())
					pkVictim->BeginFight(m_me);

				sys_log(0, "%s horse_wildattack %s", m_me->GetName(), pkVictim->GetName());
				m_me->ComputeSkill(m_bType, pkVictim);
				m_me->UseArrow(pkArrow, iUseArrow);
			}
		}

		break;

		case SKILL_MARYUNG:
			//case SKILL_GUMHWAN:
		case SKILL_TUSOK:
		case SKILL_BIPABU:
		case SKILL_NOEJEON:
		case SKILL_GEOMPUNG:
		case SKILL_SANGONG:
		case SKILL_MAHWAN:
		case SKILL_PABEOB:
		case SKILL_PAERYONG:
			//case SKILL_CURSE:
		{
			m_me->OnMove(true);
			pkVictim->OnMove();

			if (pkVictim->CanBeginFight())
				pkVictim->BeginFight(m_me);

			sys_log(0, "%s - Skill %d -> %s", m_me->GetName(), m_bType, pkVictim->GetName());
			m_me->ComputeSkill(m_bType, pkVictim);
		}
		break;

		case SKILL_CHAIN:
		{
			m_me->OnMove(true);
			pkVictim->OnMove();

			if (pkVictim->CanBeginFight())
				pkVictim->BeginFight(m_me);

			sys_log(0, "%s - Skill %d -> %s", m_me->GetName(), m_bType, pkVictim->GetName());
			m_me->ComputeSkill(m_bType, pkVictim);

			// TODO 여러명에게 슉 슉 슉 하기
		}
		break;

		case SKILL_YONGBI:
		{
			m_me->OnMove(true);
		}
		break;

		/*case SKILL_BUDONG:
		  {
		  m_me->OnMove(true);
		  pkVictim->OnMove();

		  DWORD * pdw;
		  DWORD dwEI = AllocEventInfo(sizeof(DWORD) * 2, &pdw);
		  pdw[0] = m_me->GetVID();
		  pdw[1] = pkVictim->GetVID();

		  event_create(budong_event_func, dwEI, PASSES_PER_SEC(1));
		  }
		  break;*/

		default:
			sys_err("CFuncShoot: I don't know this type [%d] of range attack.", (int)m_bType);
			break;
		}

		m_bSucceed = TRUE;
	}
};

bool CHARACTER::Shoot(BYTE bType)
{
	sys_log(1, "Shoot %s type %u flyTargets.size %zu", GetName(), bType, m_vec_dwFlyTargets.size());

	if (!CanMove())
	{
		return false;
	}

	CFuncShoot f(this, bType);

	if (m_dwFlyTargetID != 0)
	{
		f(m_dwFlyTargetID);
		m_dwFlyTargetID = 0;
	}

	f = std::for_each(m_vec_dwFlyTargets.begin(), m_vec_dwFlyTargets.end(), f);
	m_vec_dwFlyTargets.clear();

	return f.m_bSucceed;
}

void CHARACTER::FlyTarget(DWORD dwTargetVID, long x, long y, BYTE bHeader)
{
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(dwTargetVID);
	TPacketGCFlyTargeting pack;

	//pack.bHeader	= HEADER_GC_FLY_TARGETING;
	pack.bHeader = (bHeader == HEADER_CG_FLY_TARGETING) ? HEADER_GC_FLY_TARGETING : HEADER_GC_ADD_FLY_TARGETING;
	pack.dwShooterVID = GetVID();

	if (pkVictim)
	{
		pack.dwTargetVID = pkVictim->GetVID();
		pack.x = pkVictim->GetX();
		pack.y = pkVictim->GetY();

		if (bHeader == HEADER_CG_FLY_TARGETING)
			m_dwFlyTargetID = dwTargetVID;
		else
			m_vec_dwFlyTargets.push_back(dwTargetVID);
	}
	else
	{
		pack.dwTargetVID = 0;
		pack.x = x;
		pack.y = y;
	}

	sys_log(1, "FlyTarget %s vid %d x %d y %d", GetName(), pack.dwTargetVID, pack.x, pack.y);
	PacketAround(&pack, sizeof(pack), this);
}

LPCHARACTER CHARACTER::GetNearestVictim(LPCHARACTER pkChr)
{
	if (nullptr == pkChr)
		pkChr = this;

	float fMinDist = 99999.0f;
	LPCHARACTER pkVictim = nullptr;

	TDamageMap::iterator it = m_map_kDamage.begin();

	// 일단 주위에 없는 사람을 걸러 낸다.
	while (it != m_map_kDamage.end())
	{
		const VID& c_VID = it->first;
		++it;

		LPCHARACTER pAttacker = CHARACTER_MANAGER::instance().Find(c_VID);

		if (!pAttacker)
			continue;

		if (pAttacker->IsAffectFlag(AFF_EUNHYUNG) ||
			pAttacker->IsAffectFlag(AFF_INVISIBILITY) ||
			pAttacker->IsAffectFlag(AFF_REVIVE_INVISIBLE))
			continue;

		float fDist = DISTANCE_APPROX(pAttacker->GetX() - pkChr->GetX(), pAttacker->GetY() - pkChr->GetY());

		if (fDist < fMinDist)
		{
			pkVictim = pAttacker;
			fMinDist = fDist;
		}
	}

	return pkVictim;
}

void CHARACTER::SetVictim(LPCHARACTER pkVictim)
{
	if (!pkVictim)
	{
		if (0 != (DWORD)m_kVIDVictim)
			MonsterLog("공격 대상을 해제");

		m_kVIDVictim.Reset();
		battle_end(this);
	}
	else
	{
		m_kVIDVictim = pkVictim->GetVID();
		m_dwLastVictimSetTime = get_dword_time();
	}
}

LPCHARACTER CHARACTER::GetVictim() const
{
	return CHARACTER_MANAGER::instance().Find(m_kVIDVictim);
}

LPCHARACTER CHARACTER::GetProtege() const // 보호해야 할 대상을 리턴
{
	if (m_pkChrStone)
		return m_pkChrStone;

	if (m_pkParty)
		return m_pkParty->GetLeader();

	return nullptr;
}

#ifdef ENABLE_ALIGNMENT_SYSTEM
long long CHARACTER::GetAlignment() const
#else
int CHARACTER::GetAlignment() const
#endif
{
	return m_iAlignment;
}

#ifdef ENABLE_ALIGNMENT_SYSTEM
long long CHARACTER::GetRealAlignment() const
#else
int CHARACTER::GetRealAlignment() const
#endif
{
	return m_iRealAlignment;
}

void CHARACTER::ShowAlignment(bool bShow)
{
	if (bShow)
	{
		if (m_iAlignment != m_iRealAlignment)
		{
			m_iAlignment = m_iRealAlignment;
			UpdatePacket();
		}
	}
	else
	{
		if (m_iAlignment != 0)
		{
			m_iAlignment = 0;
			UpdatePacket();
		}
	}
}

#ifdef ENABLE_ALIGNMENT_SYSTEM
void CHARACTER::UpdateAlignment(long long iAmount, bool manuelUpdate)
#else
void CHARACTER::UpdateAlignment(int iAmount)
#endif
{
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(GetMapIndex()))
		return;
#endif

	bool bShow = false;

	if (m_iAlignment == m_iRealAlignment)
		bShow = true;

#ifdef ENABLE_ALIGNMENT_SYSTEM
	long long i = m_iAlignment / 10;
#else
	int i = m_iAlignment / 10;
#endif

#ifdef ENABLE_ALIGNMENT_SYSTEM
	int iOldAlignment = m_iRealAlignment;
	m_iRealAlignment = MINMAX(-200000, m_iRealAlignment + iAmount, 10000000);
	OnAlignUpdate(iOldAlignment, manuelUpdate);
#else
	m_iRealAlignment = MINMAX(-200000, m_iRealAlignment + iAmount, 200000);
#endif

	if (bShow)
	{
		m_iAlignment = m_iRealAlignment;

		if (i != m_iAlignment / 10)
			UpdatePacket();
	}
}

void CHARACTER::SetKillerMode(bool isOn)
{
	if ((isOn ? ADD_CHARACTER_STATE_KILLER : 0) == IS_SET(m_bAddChrState, ADD_CHARACTER_STATE_KILLER))
		return;

	if (isOn)
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_KILLER);
	else
		REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_KILLER);

	m_iKillerModePulse = thecore_pulse();
	UpdatePacket();
	sys_log(0, "SetKillerMode Update %s[%d]", GetName(), GetPlayerID());
}

bool CHARACTER::IsKillerMode() const
{
	return IS_SET(m_bAddChrState, ADD_CHARACTER_STATE_KILLER);
}

void CHARACTER::UpdateKillerMode()
{
	if (!IsKillerMode())
		return;

	if (thecore_pulse() - m_iKillerModePulse >= PASSES_PER_SEC(30))
		SetKillerMode(false);
}

void CHARACTER::SetPKMode(BYTE bPKMode)
{
	if (bPKMode >= PK_MODE_MAX_NUM)
		return;

	if (m_bPKMode == bPKMode)
		return;

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if ((MeleyLair::CMgr::instance().IsMeleyMap(GetMapIndex())) && (bPKMode != PK_MODE_GUILD))
		bPKMode = PK_MODE_GUILD;
#endif

	if (bPKMode == PK_MODE_GUILD && !GetGuild())
		bPKMode = PK_MODE_FREE;

	m_bPKMode = bPKMode;
	UpdatePacket();

	sys_log(0, "PK_MODE: %s %d", GetName(), m_bPKMode);
}

BYTE CHARACTER::GetPKMode() const
{
	return m_bPKMode;
}

struct FuncForgetMyAttacker
{
	LPCHARACTER m_ch;
	FuncForgetMyAttacker(LPCHARACTER ch)
	{
		m_ch = ch;
	}
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
				return;
			if (ch->m_kVIDVictim == m_ch->GetVID())
				ch->SetVictim(nullptr);
		}
	}
};

struct FuncAggregateMonster
{
	LPCHARACTER m_ch;
	FuncAggregateMonster(LPCHARACTER ch)
	{
		m_ch = ch;
	}
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
				return;
			if (!ch->IsMonster())
				return;
			if (ch->GetVictim())
				return;

			if (number(1, 100) <= 100) // 임시로 50% 확률로 적을 끌어온다
				if (DISTANCE_APPROX(ch->GetX() - m_ch->GetX(), ch->GetY() - m_ch->GetY()) < 5000)
					if (ch->CanBeginFight())
						ch->BeginFight(m_ch);
		}
	}
};

struct FuncAttractRanger
{
	LPCHARACTER m_ch;
	FuncAttractRanger(LPCHARACTER ch)
	{
		m_ch = ch;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
				return;
			if (!ch->IsMonster())
				return;
			if (ch->GetVictim() && ch->GetVictim() != m_ch)
				return;
			if (ch->GetMobAttackRange() > 150)
			{
				int iNewRange = 150;//(int)(ch->GetMobAttackRange() * 0.2);
				if (iNewRange < 150)
					iNewRange = 150;

				ch->AddAffect(AFFECT_BOW_DISTANCE, POINT_BOW_DISTANCE, iNewRange - ch->GetMobAttackRange(), AFF_NONE, 3 * 60, 0, false);
			}
		}
	}
};

struct FuncPullMonster
{
	LPCHARACTER m_ch;
	int m_iLength;
	FuncPullMonster(LPCHARACTER ch, int iLength = 300)
	{
		m_ch = ch;
		m_iLength = iLength;
	}

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
				return;
			if (!ch->IsMonster())
				return;
			//if (ch->GetVictim() && ch->GetVictim() != m_ch)
			//return;
			float fDist = DISTANCE_APPROX(m_ch->GetX() - ch->GetX(), m_ch->GetY() - ch->GetY());
			if (fDist > 3000 || fDist < 100)
				return;

			float fNewDist = fDist - m_iLength;
			if (fNewDist < 100)
				fNewDist = 100;

			float degree = GetDegreeFromPositionXY(ch->GetX(), ch->GetY(), m_ch->GetX(), m_ch->GetY());
			float fx;
			float fy;

			GetDeltaByDegree(degree, fDist - fNewDist, &fx, &fy);
			long tx = (long)(ch->GetX() + fx);
			long ty = (long)(ch->GetY() + fy);

			ch->Sync(tx, ty);
			ch->Goto(tx, ty);
			ch->CalculateMoveDuration();

			ch->SyncPacket();
		}
	}
};

void CHARACTER::ForgetMyAttacker()
{
	LPSECTREE pSec = GetSectree();
	if (pSec)
	{
		FuncForgetMyAttacker f(this);
		pSec->ForEachAround(f);
	}
	ReviveInvisible(5);
}

void CHARACTER::AggregateMonster()
{
	LPSECTREE pSec = GetSectree();
	if (pSec)
	{
		FuncAggregateMonster f(this);
		pSec->ForEachAround(f);
	}
}

void CHARACTER::AttractRanger()
{
	LPSECTREE pSec = GetSectree();
	if (pSec)
	{
		FuncAttractRanger f(this);
		pSec->ForEachAround(f);
	}
}

void CHARACTER::PullMonster()
{
	LPSECTREE pSec = GetSectree();
	if (pSec)
	{
		FuncPullMonster f(this);
		pSec->ForEachAround(f);
	}
}

void CHARACTER::UpdateAggrPointEx(LPCHARACTER pAttacker, EDamageType type, int dam, CHARACTER::TBattleInfo & info)
{
	// 특정 공격타입에 따라 더 올라간다
	switch (type)
	{
	case DAMAGE_TYPE_NORMAL_RANGE:
		dam = (int)(dam * 1.2f);
		break;

	case DAMAGE_TYPE_RANGE:
		dam = (int)(dam * 1.5f);
		break;

	case DAMAGE_TYPE_MAGIC:
		dam = (int)(dam * 1.2f);
		break;

	default:
		break;
	}

	// 공격자가 현재 대상인 경우 보너스를 준다.
	if (pAttacker == GetVictim())
		dam = (int)(dam * 1.2f);

	info.iAggro += dam;

	if (info.iAggro < 0)
		info.iAggro = 0;

	//sys_log(0, "UpdateAggrPointEx for %s by %s dam %d total %d", GetName(), pAttacker->GetName(), dam, total);
	if (GetParty() && dam > 0 && type != DAMAGE_TYPE_SPECIAL)
	{
		LPPARTY pParty = GetParty();

		// 리더인 경우 영향력이 좀더 강하다
		int iPartyAggroDist = dam;

		if (pParty->GetLeaderPID() == GetVID())
			iPartyAggroDist /= 2;
		else
			iPartyAggroDist /= 3;

		pParty->SendMessage(this, PM_AGGRO_INCREASE, iPartyAggroDist, pAttacker->GetVID());
	}

#ifdef ENABLE_POISON_UNAGGR
	if (type != DAMAGE_TYPE_POISON)
		ChangeVictimByAggro(info.iAggro, pAttacker);
#else
	ChangeVictimByAggro(info.iAggro, pAttacker);
#endif
}

void CHARACTER::UpdateAggrPoint(LPCHARACTER pAttacker, EDamageType type, int dam)
{
	if (IsDead() || IsStun())
		return;

	TDamageMap::iterator it = m_map_kDamage.find(pAttacker->GetVID());

	if (it == m_map_kDamage.end())
	{
		m_map_kDamage.insert(TDamageMap::value_type(pAttacker->GetVID(), TBattleInfo(0, dam)));
		it = m_map_kDamage.find(pAttacker->GetVID());
	}

	UpdateAggrPointEx(pAttacker, type, dam, it->second);
}

void CHARACTER::ChangeVictimByAggro(int iNewAggro, LPCHARACTER pNewVictim)
{
	if (get_dword_time() - m_dwLastVictimSetTime < 3000) // 3초는 기다려야한다
		return;

	if (pNewVictim == GetVictim())
	{
		if (m_iMaxAggro < iNewAggro)
		{
			m_iMaxAggro = iNewAggro;
			return;
		}

		// Aggro가 감소한 경우
		TDamageMap::iterator it;
		TDamageMap::iterator itFind = m_map_kDamage.end();

		for (it = m_map_kDamage.begin(); it != m_map_kDamage.end(); ++it)
		{
			if (it->second.iAggro > iNewAggro)
			{
				LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(it->first);

				if (ch && !ch->IsDead() && DISTANCE_APPROX(ch->GetX() - GetX(), ch->GetY() - GetY()) < 5000)
				{
					itFind = it;
					iNewAggro = it->second.iAggro;
				}
			}
		}

		if (itFind != m_map_kDamage.end())
		{
			m_iMaxAggro = iNewAggro;
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			if (!IsDefanceWaweMastAttackMob(GetRaceNum()))
				SetVictim(CHARACTER_MANAGER::instance().Find(itFind->first));
#else
			SetVictim(CHARACTER_MANAGER::instance().Find(itFind->first));
#endif
			m_dwStateDuration = 1;
		}
	}
	else
	{
		if (m_iMaxAggro < iNewAggro)
		{
			m_iMaxAggro = iNewAggro;
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
			if (!IsDefanceWaweMastAttackMob(GetRaceNum()))
				SetVictim(pNewVictim);
#else
			SetVictim(pNewVictim);
#endif
			m_dwStateDuration = 1;
		}
	}
}