#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "char.h"
#include "char_manager.h"
#include "battle.h"
#include "item.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "vector.h"
#include "packet.h"
#include "pvp.h"
#include "profiler.h"
#include "guild.h"
#include "affect.h"
#include "unique_item.h"
#include "arena.h"
#include "sectree.h"
#include "ani.h"
#include "locale_service.h"
#include "affect_flag.h"
#ifdef ENABLE_ATTENDANCE_EVENT
#include "minigame.h"
#endif

int battle_hit(LPCHARACTER ch, LPCHARACTER victim, int& iRetDam);

bool battle_distance_valid_by_xy(long x, long y, long tx, long ty)
{
	long distance = DISTANCE_APPROX(x - tx, y - ty);

	if (distance > 170)
		return false;

	return true;
}

bool battle_distance_valid(LPCHARACTER ch, LPCHARACTER victim)
{
	return battle_distance_valid_by_xy(ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY());
}

bool timed_event_cancel(LPCHARACTER ch)
{
	if (ch->m_pkTimedEvent)
	{
		event_cancel(&ch->m_pkTimedEvent);
		return true;
	}

	/* RECALL_DELAY
	   차후 전투로 인해 귀환부 딜레이가 취소 되어야 할 경우 주석 해제
	   if (ch->m_pk_RecallEvent)
	   {
	   event_cancel(&ch->m_pkRecallEvent);
	   return true;
	   }
	   END_OF_RECALL_DELAY */

	return false;
}

bool battle_is_attackable(LPCHARACTER ch, LPCHARACTER victim)
{
	// 상대방이 죽었으면 중단한다.
	if (victim->IsDead())
		return false;

	if (victim->IsObserverMode() || victim->IsStun())
		return false;
	if (victim->GetRaceNum() >= 30000 && victim->GetRaceNum() <= 30100)
		return false;

	if (!ch->IsGuardNPC())
	{
		SECTREE* sectree = nullptr;

		sectree = ch->GetSectree();
		if (sectree && sectree->IsAttr(ch->GetX(), ch->GetY(), ATTR_BANPK))
			return false;

		sectree = victim->GetSectree();
		if (sectree && sectree->IsAttr(victim->GetX(), victim->GetY(), ATTR_BANPK))
			return false;
	}

	if (ch->IsStun() || ch->IsDead())
		return false;

	if (ch->IsPC() && victim->IsPC())
	{
		CGuild* g1 = ch->GetGuild();
		CGuild* g2 = victim->GetGuild();

		if (g1 && g2)
		{
			if (g1->UnderWar(g2->GetID()))
				return true;
		}
	}

	if (CArenaManager::instance().CanAttack(ch, victim) == true)
		return true;

#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
	if (victim->GetRaceNum() == 20434 && ch->IsMonster())
		return true;
#endif

	return CPVPManager::instance().CanAttack(ch, victim);
}

int battle_melee_attack(LPCHARACTER ch, LPCHARACTER victim)
{
	if (test_server&&ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	if (!victim || ch == victim)
		return BATTLE_NONE;

	if (test_server && ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	if (!battle_is_attackable(ch, victim))
		return BATTLE_NONE;

	if (test_server && ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	// 거리 체크
	int distance = DISTANCE_APPROX(ch->GetX() - victim->GetX(), ch->GetY() - victim->GetY());

	if (!victim->IsBuilding())
	{
		int max = 300;

		if (false == ch->IsPC())
		{
			// 몬스터의 경우 몬스터 공격 거리를 사용
			max = (int)(ch->GetMobAttackRange() * 1.15f);
		}
		else
		{
			// PC일 경우 상대가 melee 몹일 경우 몹의 공격 거리가 최대 공격 거리
			if (false == victim->IsPC() && BATTLE_TYPE_MELEE == victim->GetMobBattleType())
				max = MAX(300, (int)(victim->GetMobAttackRange() * 1.15f));
		}
		
#ifdef ENABLE_DEFENSE_WAVE_SYSTEM
		if (ch->IsPC() && (victim->GetRaceNum() == 3960 || victim->GetRaceNum() == 3961 || victim->GetRaceNum() == 3962))
			max += 400;
#endif

		if (distance > max)
		{
			if (test_server)
				sys_log(0, "VICTIM_FAR: %s distance: %d max: %d", ch->GetName(), distance, max);

			return BATTLE_NONE;
		}
	}

	if (timed_event_cancel(ch))
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("전투가 시작 되어 취소 되었습니다."));

	if (timed_event_cancel(victim))
		victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("전투가 시작 되어 취소 되었습니다."));

	ch->SetPosition(POS_FIGHTING);
	ch->SetVictim(victim);

	const PIXEL_POSITION& vpos = victim->GetXYZ();
	ch->SetRotationToXY(vpos.x, vpos.y);

	int dam;
	int ret = battle_hit(ch, victim, dam);
	return (ret);
}

// 실제 GET_BATTLE_VICTIM을 nullptr로 만들고 이벤트를 캔슬 시킨다.
void battle_end_ex(LPCHARACTER ch)
{
	if (ch->IsPosition(POS_FIGHTING))
		ch->SetPosition(POS_STANDING);
}

void battle_end(LPCHARACTER ch)
{
	battle_end_ex(ch);
}

// AG = Attack Grade
// AL = Attack Limit
int CalcBattleDamage(int iDam, int iAttackerLev, int iVictimLev)
{
	if (iDam < 3)
		iDam = number(1, 5);

	//return CALCULATE_DAMAGE_LVDELTA(iAttackerLev, iVictimLev, iDam);
	return iDam;
}

int CalcMagicDamageWithValue(int iDam, LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	return CalcBattleDamage(iDam, pkAttacker->GetLevel(), pkVictim->GetLevel());
}

int CalcMagicDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	int iDam = 0;

	if (pkAttacker->IsNPC())
	{
		iDam = CalcMeleeDamage(pkAttacker, pkVictim, false, false);
	}

	iDam += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);

	return CalcMagicDamageWithValue(iDam, pkAttacker, pkVictim);
}

float CalcAttackRating(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, bool bIgnoreTargetRating)
{
	int attacker_dx = pkAttacker->GetPolymorphPoint(POINT_DX);
	int attacker_lv = pkAttacker->GetLevel();

	int victim_dx = pkVictim->GetPolymorphPoint(POINT_DX);
	int victim_lv = pkAttacker->GetLevel();

	int iARSrc = MIN(90, (attacker_dx * 4	+ attacker_lv * 2) / 6);
	int iERSrc = MIN(90, (victim_dx	  * 4	+ victim_lv   * 2) / 6);

	float fAR = ((float)iARSrc + 210.0f) / 300.0f; // fAR = 0.7 ~ 1.0

	if (bIgnoreTargetRating)
		return fAR;

	// ((Edx * 2 + 20) / (Edx + 110)) * 0.3
	float fER = ((float)(iERSrc * 2 + 5) / (iERSrc + 95)) * 3.0f / 10.0f;

	return fAR - fER;
}

int CalcAttBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int iAtk)
{
	// PvP에는 적용하지않음
	if (!pkVictim->IsPC())
		iAtk += pkAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_ATTACK_BONUS);

	// PvP에는 적용하지않음
	if (!pkAttacker->IsPC())
	{
		int iReduceDamagePct = pkVictim->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_TRANSFER_DAMAGE);
		iAtk = iAtk * (100 + iReduceDamagePct) / 100;
	}

	if (pkAttacker->IsNPC() && pkVictim->IsPC())
	{
		iAtk = (iAtk * CHARACTER_MANAGER::instance().GetMobDamageRate(pkAttacker)) / 100;
	}

#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	if (pkVictim->IsPC())
	{
		LPITEM pkWeapon = pkVictim->GetWear(WEAR_WEAPON);
		if (pkWeapon)
		{
			switch (pkWeapon->GetSubType())
			{
			case WEAPON_SWORD:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_SWORD)) / 100;
				break;

			case WEAPON_TWO_HANDED:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_TWOHANDED)) / 100;
				break;

			case WEAPON_CLAW:
			case WEAPON_DAGGER:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DAGGER)) / 100;
				break;

			case WEAPON_BELL:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_BELL)) / 100;
				break;

			case WEAPON_FAN:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_FAN)) / 100;
				break;

			case WEAPON_BOW:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_BOW)) / 100;
				break;
			}
		}
	}
#endif

	if (pkVictim->IsNPC())
	{
#ifdef ENABLE_ELEMENT_NEW_BONUSES
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_ELEC))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ELEC)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_WIND))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WIND)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_EARTH))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_EARTH)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_DARK))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DARK)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_CZ))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_CZ)) / 100;
#endif

		if (pkVictim->IsRaceFlag(RACE_FLAG_ANIMAL))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ANIMAL)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_UNDEAD))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_UNDEAD)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_DEVIL))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DEVIL)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_HUMAN))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_HUMAN)) / 90;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ORC))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ORC)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_MILGYO))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MILGYO)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_INSECT))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_INSECT)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_FIRE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_FIRE)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_ICE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ICE)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_DESERT))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DESERT)) / 100;
		if (pkVictim->IsRaceFlag(RACE_FLAG_TREE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_TREE)) / 100;
#ifdef ENABLE_MUSHROOM_DUNGEON
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_MUSHROOM))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MUSHROOM)) / 100;
#endif
#ifdef ENABLE_WATER_DUNGEON
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_WATER))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WATER)) / 100;
#endif

		iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MONSTER)) / 90;

#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
		if (pkVictim->GetRaceNum() == 6091)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_RAZADOR)) / 100;
		else if (pkVictim->GetRaceNum() == 6191)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_NEMERE)) / 100;
		else if (pkVictim->GetRaceNum() == 3960 || pkVictim->GetRaceNum() == 3961 || pkVictim->GetRaceNum() == 3962 || pkVictim->GetRaceNum() == 3963 || pkVictim->GetRaceNum() == 3964)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_HYDRA)) / 100;
		else if (pkVictim->GetRaceNum() == 1093)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_LUSIFER)) / 100;
		else if (pkVictim->GetRaceNum() == 2493)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_BERAN_SETAOU)) / 100;
		else if (pkVictim->GetRaceNum() == 2598)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_AZRAEL)) / 100;
		else if (pkVictim->GetRaceNum() == 6118)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MELEY)) / 100;
#endif


		if (pkVictim->IsStone())
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_STONE)) / 100;
#ifdef ENABLE_BONUS_TO_BOSS
		else if (pkVictim->GetMobRank() >= MOB_RANK_BOSS)
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_BOSS)) / 100;
#endif
	}
	else if (pkVictim->IsPC())
	{
		iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_HUMAN)) / 100;

		switch (pkVictim->GetJob())
		{
		case JOB_WARRIOR:
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WARRIOR)) / 100;
			break;

		case JOB_ASSASSIN:
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ASSASSIN)) / 100;
			break;

		case JOB_SURA:
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_SURA)) / 100;
			break;

		case JOB_SHAMAN:
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_SHAMAN)) / 100;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case JOB_WOLFMAN: // TODO: 수인족 ATTBONUS 처리
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WOLFMAN)) / 100;
			break;
#endif
		}
	}

	if (pkAttacker->IsPC() == true)
	{
		iAtk -= (iAtk * pkAttacker->GetPoint(POINT_RESIST_HUMAN)) / 100;

		switch (pkAttacker->GetJob())
		{
		case JOB_WARRIOR:
			iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_WARRIOR)) / 150;
			break;

		case JOB_ASSASSIN:
			iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_ASSASSIN)) / 150;
			break;

		case JOB_SURA:
			iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_SURA)) / 150;
			break;

		case JOB_SHAMAN:
			iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_SHAMAN)) / 150;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case JOB_WOLFMAN: // TODO: 수인족 저항 처리
			iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_WOLFMAN)) / 150;
			break;
#endif
		}
	}

	return iAtk;
}

void Item_GetDamage(LPITEM pkItem, int* pdamMin, int* pdamMax)
{
	*pdamMin = 0;
	*pdamMax = 1;

	if (!pkItem)
		return;

	switch (pkItem->GetType())
	{
	case ITEM_ROD:
	case ITEM_PICK:
		return;
	}

	if (pkItem->GetType() != ITEM_WEAPON)
		sys_err("Item_GetDamage - !ITEM_WEAPON vnum=%d, type=%d", pkItem->GetOriginalVnum(), pkItem->GetType());
	
	if (pkItem->GetRarePoints() > 0)
	{
		*pdamMin = pkItem->GetValue(3)+pkItem->GetRarePoints();
		*pdamMax = pkItem->GetValue(4)+pkItem->GetRarePoints();
	}
	else
	{
		*pdamMin = pkItem->GetValue(3);
		*pdamMax = pkItem->GetValue(4);
	}
}

int CalcMeleeDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, bool bIgnoreDefense, bool bIgnoreTargetRating)
{
	LPITEM pWeapon = pkAttacker->GetWear(WEAR_WEAPON);
	bool bPolymorphed = pkAttacker->IsPolymorphed();

	if (pWeapon && !(bPolymorphed && !pkAttacker->IsPolyMaintainStat()))
	{
		if (pWeapon->GetType() != ITEM_WEAPON)
			return 0;

		switch (pWeapon->GetSubType())
		{
		case WEAPON_SWORD:
		case WEAPON_DAGGER:
		case WEAPON_TWO_HANDED:
		case WEAPON_BELL:
		case WEAPON_FAN:
		case WEAPON_MOUNT_SPEAR:
#ifdef ENABLE_WOLFMAN_CHARACTER
		case WEAPON_CLAW:
#endif
			break;

		case WEAPON_BOW:
			sys_err("CalcMeleeDamage should not handle bows (name: %s)", pkAttacker->GetName());
			return 0;

		default:
			return 0;
		}
	}

	int iDam = 0;
	float fAR = CalcAttackRating(pkAttacker, pkVictim, bIgnoreTargetRating);
	int iDamMin = 0, iDamMax = 0;

	// TESTSERVER_SHOW_ATTACKINFO
	int DEBUG_iDamCur = 0;
	int DEBUG_iDamBonus = 0;
	// END_OF_TESTSERVER_SHOW_ATTACKINFO

	if (bPolymorphed && !pkAttacker->IsPolyMaintainStat())
	{
		// MONKEY_ROD_ATTACK_BUG_FIX
		Item_GetDamage(pWeapon, &iDamMin, &iDamMax);
		// END_OF_MONKEY_ROD_ATTACK_BUG_FIX
		DWORD dwMobVnum = pkAttacker->GetPolymorphVnum();
		const CMob* pMob = CMobManager::instance().Get(dwMobVnum);

		if (pMob)
		{
			int iPower = pkAttacker->GetPolymorphPower();
			iDamMin += pMob->m_table.dwDamageRange[0] * iPower / 100;
			iDamMax += pMob->m_table.dwDamageRange[1] * iPower / 100;
		}
	}
	else if (pWeapon)
	{
		// MONKEY_ROD_ATTACK_BUG_FIX
		Item_GetDamage(pWeapon, &iDamMin, &iDamMax);
		// END_OF_MONKEY_ROD_ATTACK_BUG_FIX
	}
	else if (pkAttacker->IsNPC())
	{
		iDamMin = pkAttacker->GetMobDamageMin();
		iDamMax = pkAttacker->GetMobDamageMax();
	}

	iDam = number(iDamMin, iDamMax) * 2;

	// TESTSERVER_SHOW_ATTACKINFO
	DEBUG_iDamCur = iDam;
	// END_OF_TESTSERVER_SHOW_ATTACKINFO
	//
	int iAtk = 0;

	// level must be ignored when multiply by fAR, so subtract it before calculation.
	iAtk = pkAttacker->GetPoint(POINT_ATT_GRADE) + iDam - (pkAttacker->GetLevel() * 2);
	iAtk = (int)(iAtk * fAR);
	iAtk += pkAttacker->GetLevel() * 2; // and add again

	if (pWeapon)
	{
		iAtk += pWeapon->GetValue(5) * 2;

		// 2004.11.12.myevan.TESTSERVER_SHOW_ATTACKINFO
		DEBUG_iDamBonus = pWeapon->GetValue(5) * 2;
		///////////////////////////////////////////////
	}

	iAtk += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS); // party attacker role bonus
	iAtk = (int)(iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
	iAtk = CalcAttBonus(pkAttacker, pkVictim, iAtk);

	int iDef = 0;
	if (!bIgnoreDefense)
	{
		iDef = (pkVictim->GetPoint(POINT_DEF_GRADE) * (100 + pkVictim->GetPoint(POINT_DEF_BONUS)) / 100);
		if (!pkAttacker->IsPC())
			iDef += pkVictim->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_DEFENSE_BONUS);
	}

	if (pkAttacker->IsNPC())
		iAtk = (int)(iAtk * pkAttacker->GetMobDamageMultiply());

	iDam = MAX(0, iAtk - iDef);

	if (test_server)
	{
		int DEBUG_iLV = pkAttacker->GetLevel() * 2;
		int DEBUG_iST = int((pkAttacker->GetPoint(POINT_ATT_GRADE) - DEBUG_iLV) * fAR);
		int DEBUG_iPT = pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);
		int DEBUG_iWP = 0;
		int DEBUG_iPureAtk = 0;
		int DEBUG_iPureDam = 0;
		char szRB[32] = "";
		char szGradeAtkBonus[32] = "";

		DEBUG_iWP = int(DEBUG_iDamCur * fAR);
		DEBUG_iPureAtk = DEBUG_iLV + DEBUG_iST + DEBUG_iWP + DEBUG_iDamBonus;
		DEBUG_iPureDam = iAtk - iDef;

		if (pkAttacker->IsNPC())
		{
			snprintf(szGradeAtkBonus, sizeof(szGradeAtkBonus), "=%d*%.1f", DEBUG_iPureAtk, pkAttacker->GetMobDamageMultiply());
			DEBUG_iPureAtk = int(DEBUG_iPureAtk * pkAttacker->GetMobDamageMultiply());
		}

		if (DEBUG_iDamBonus != 0)
			snprintf(szRB, sizeof(szRB), "+RB(%d)", DEBUG_iDamBonus);

		char szPT[32] = "";

		if (DEBUG_iPT != 0)
			snprintf(szPT, sizeof(szPT), ", PT=%d", DEBUG_iPT);

		char szUnknownAtk[32] = "";

		if (iAtk != DEBUG_iPureAtk)
			snprintf(szUnknownAtk, sizeof(szUnknownAtk), "+?(%d)", iAtk - DEBUG_iPureAtk);

		char szUnknownDam[32] = "";

		if (iDam != DEBUG_iPureDam)
			snprintf(szUnknownDam, sizeof(szUnknownDam), "+?(%d)", iDam - DEBUG_iPureDam);

		char szMeleeAttack[128];

		snprintf(szMeleeAttack, sizeof(szMeleeAttack),
			"%s(%d)-%s(%d)=%d%s, ATK=LV(%d)+ST(%d)+WP(%d)%s%s%s, AR=%.3g%s",
			pkAttacker->GetName(),
			iAtk,
			pkVictim->GetName(),
			iDef,
			iDam,
			szUnknownDam,
			DEBUG_iLV,
			DEBUG_iST,
			DEBUG_iWP,
			szRB,
			szUnknownAtk,
			szGradeAtkBonus,
			fAR,
			szPT);

		pkAttacker->ChatPacket(CHAT_TYPE_TALKING, "%s", szMeleeAttack);
		pkVictim->ChatPacket(CHAT_TYPE_TALKING, "%s", szMeleeAttack);
	}
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	if (pkAttacker->IsMonster() && (pkAttacker->GetMapIndex() >= 3580000 && pkAttacker->GetMapIndex() < 3590000))
	{
		if (pkAttacker->GetLevel() > 85)
			iDam += pkAttacker->GetLevel()*8;
		else
			iDam += pkAttacker->GetLevel()*5;
	}
#endif

	return CalcBattleDamage(iDam, pkAttacker->GetLevel(), pkVictim->GetLevel());
}

int CalcArrowDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, LPITEM pkBow, LPITEM pkArrow, bool bIgnoreDefense)
{
	if (!pkBow || pkBow->GetType() != ITEM_WEAPON || pkBow->GetSubType() != WEAPON_BOW)
		return 0;

	if (!pkArrow)
		return 0;

	int iDist = (int)(DISTANCE_SQRT(pkAttacker->GetX() - pkVictim->GetX(), pkAttacker->GetY() - pkVictim->GetY()));
	int iGap = (iDist / 100) - 5 - pkAttacker->GetPoint(POINT_BOW_DISTANCE);
	int iPercent = 100 - (iGap * 5);

#ifdef ENABLE_QUIVER_SYSTEM
	if (pkArrow->GetSubType() == WEAPON_QUIVER)
		iPercent = 100;
#endif

	if (iPercent <= 0)
		return 0;
	else if (iPercent > 100)
		iPercent = 100;

	int iDam = 0;
	float fAR = CalcAttackRating(pkAttacker, pkVictim, false);
	iDam = number(pkBow->GetValue(3), pkBow->GetValue(4)) * 2 + pkArrow->GetValue(3);

	int iAtk;
	iAtk = pkAttacker->GetPoint(POINT_ATT_GRADE) + iDam - (pkAttacker->GetLevel() * 2);
	iAtk = (int)(iAtk * fAR);
	iAtk += pkAttacker->GetLevel() * 2;

	iAtk += pkBow->GetValue(5) * 2;
	iAtk += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);
	iAtk = (int)(iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
	iAtk = CalcAttBonus(pkAttacker, pkVictim, iAtk);

	int iDef = 0;
	if (!bIgnoreDefense)
		iDef = (pkVictim->GetPoint(POINT_DEF_GRADE) * (100 + pkAttacker->GetPoint(POINT_DEF_BONUS)) / 100);

	if (pkAttacker->IsNPC())
		iAtk = (int)(iAtk * pkAttacker->GetMobDamageMultiply());

	iDam = MAX(0, iAtk - iDef);

	int iPureDam = iDam;
	iPureDam = (iPureDam * iPercent) / 100;
	if (test_server)
	{
		pkAttacker->ChatPacket(CHAT_TYPE_INFO, "ARROW %s -> %s, DAM %d DIST %d GAP %d %% %d", pkAttacker->GetName(), pkVictim->GetName(), iPureDam, iDist, iGap, iPercent);
	}

	return iPureDam;
}

void NormalAttackAffect(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	// 독 공격은 특이하므로 특수 처리
	if (pkAttacker->GetPoint(POINT_POISON_PCT) && !pkVictim->IsAffectFlag(AFF_POISON) && !pkVictim->IsAffectFlag(AFF_BLEEDING))
	{
		if (number(1, 100) <= pkAttacker->GetPoint(POINT_POISON_PCT))
			pkVictim->AttackedByPoison(pkAttacker);
	}
#ifdef ENABLE_WOLFMAN_CHARACTER
	// BLEED
	if (pkAttacker->GetPoint(POINT_BLEEDING_PCT) && !pkVictim->IsAffectFlag(AFF_BLEEDING) && !pkVictim->IsAffectFlag(AFF_POISON))
	{
		if (number(1, 100) <= pkAttacker->GetPoint(POINT_BLEEDING_PCT))
			pkVictim->AttackedByBleeding(pkAttacker);
	}
#endif
	int iStunDuration = 2;
	if (pkAttacker->IsPC() && !pkVictim->IsPC())
		iStunDuration = 4;

	AttackAffect(pkAttacker, pkVictim, POINT_STUN_PCT, IMMUNE_STUN, AFFECT_STUN, POINT_NONE, 0, AFF_STUN, iStunDuration, "STUN");
	AttackAffect(pkAttacker, pkVictim, POINT_SLOW_PCT, IMMUNE_SLOW, AFFECT_SLOW, POINT_MOV_SPEED, -30, AFF_SLOW, 20, "SLOW");
}

int battle_hit(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int& iRetDam)
{
	//PROF_UNIT puHit("Hit");
	if (test_server)
		sys_log(0, "battle_hit : [%s] attack to [%s] : dam :%d type :%d", pkAttacker->GetName(), pkVictim->GetName(), iRetDam);

	int iDam = CalcMeleeDamage(pkAttacker, pkVictim);

	if (iDam <= 0)
		return (BATTLE_DAMAGE);

	NormalAttackAffect(pkAttacker, pkVictim);

	// 데미지 계산
	//iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST)) / 100;
	LPITEM pkWeapon = pkAttacker->GetWear(WEAR_WEAPON);

	if (pkWeapon)
		switch (pkWeapon->GetSubType())
		{
		case WEAPON_SWORD:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_SWORD)) / 100;
			break;

		case WEAPON_TWO_HANDED:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_TWOHAND)) / 100;
			break;

		case WEAPON_DAGGER:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_DAGGER)) / 100;
			break;

		case WEAPON_BELL:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_BELL)) / 100;
			break;

		case WEAPON_FAN:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_FAN)) / 100;
			break;

		case WEAPON_BOW:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_BOW)) / 100;
			break;
#ifdef ENABLE_WOLFMAN_CHARACTER
		case WEAPON_CLAW:
			iDam = iDam * (100 - pkVictim->GetPoint(POINT_RESIST_DAGGER)) / 100;
			break;
#endif
		}

	//최종적인 데미지 보정. (2011년 2월 현재 대왕거미에게만 적용.)
	float attMul = pkAttacker->GetAttMul();
	float tempIDam = iDam;
	iDam = attMul * tempIDam + 0.5f;
	iRetDam = iDam;

	//PROF_UNIT puDam("Dam");

	CMiniGame::instance().ShadowWarriorMonsterAttack(pkAttacker, pkVictim);

#ifdef ENABLE_ATTENDANCE_EVENT
	EDamageType damageType = (pkVictim->GetRaceNum() >= 6415 && pkVictim->GetRaceNum() <= 6419) ? DAMAGE_TYPE_SPECIAL : DAMAGE_TYPE_NORMAL;

	CMiniGame::instance().AttendanceMonsterAttack(pkAttacker, pkVictim);

#ifdef ENABLE_ITEM_SOUL_SYSTEM
	if (!pkAttacker->IsPolymorphed() && !pkVictim->IsPC())
		iDam *= pkAttacker->GetSoulDamVal(true);
#endif

	if (pkVictim->Damage(pkAttacker, iDam, damageType))
		return (BATTLE_DEAD);
#else
	if (pkVictim->Damage(pkAttacker, iDam, DAMAGE_TYPE_NORMAL))
		return (BATTLE_DEAD);
#endif

	return (BATTLE_DAMAGE);
}