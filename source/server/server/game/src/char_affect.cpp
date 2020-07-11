#include "stdafx.h"

#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "affect.h"
#include "packet.h"
#include "buffer_manager.h"
#include "desc_client.h"
#include "battle.h"
#include "guild.h"
#include "utils.h"
#include "locale_service.h"
#include "arena.h"
#include "horsename_manager.h"
#include "item.h"
#include "DragonSoul.h"
#include "war_map.h"

#define IS_NO_SAVE_AFFECT(type) ((type) == AFFECT_WAR_FLAG || (type) == AFFECT_REVIVE_INVISIBLE || ((type) >= AFFECT_PREMIUM_START && (type) <= AFFECT_PREMIUM_END)) || (type) == AFFECT_MOUNT_BONUS
#define IS_NO_CLEAR_ON_DEATH_AFFECT(type) ((type) == AFFECT_BLOCK_CHAT || ((type) >= 500 && (type) < 600) || (type) == AFFECT_PREMIUM || (type) == AFFECT_PREMIUM2 || (type) == AFFECT_PREMIUM3 || (type) == AFFECT_WIND_SHOES || (type) == AFFECT_DG_ABILITY)
#define IS_NO_UPDATE_PACKET_AFFECT(type) ((type) == AFFECT_MOV_SPEED || (type) == AFFECT_ATT_SPEED || (type) == AFFECT_STR || (type) == AFFECT_DEX || (type) == AFFECT_INT || (type) == AFFECT_CON || (type) == AFFECT_CHINA_FIREWORK || (type) == SKILL_JEONGWI || (type) == SKILL_GEOMKYUNG || (type) == SKILL_CHUNKEON || (type) == SKILL_EUNHYUNG || (type) == SKILL_GYEONGGONG || (type) == SKILL_GWIGEOM || (type) == SKILL_TERROR || (type) == SKILL_JUMAGAP || (type) == SKILL_MANASHILED || (type) == SKILL_HOSIN || (type) == SKILL_REFLECT || (type) == SKILL_KWAESOK || (type) == SKILL_JEUNGRYEOK || (type) == SKILL_GICHEON || (type) == SKILL_JEOKRANG || (type) == SKILL_CHEONGRANG)
#define IS_REAL_TIME_AFFECT(type) ((type) == AFFECT_PREMIUM || (type) == AFFECT_PREMIUM2 || (type) == AFFECT_PREMIUM3)

void SendAffectRemovePacket(LPDESC d, DWORD pid, DWORD type, BYTE point)
{
	TPacketGCAffectRemove ptoc;
	ptoc.bHeader = HEADER_GC_AFFECT_REMOVE;
	ptoc.dwType = type;
	ptoc.bApplyOn = point;
	d->Packet(&ptoc, sizeof(TPacketGCAffectRemove));

	TPacketGDRemoveAffect ptod;
	ptod.dwPID = pid;
	ptod.dwType = type;
	ptod.bApplyOn = point;
	db_clientdesc->DBPacket(HEADER_GD_REMOVE_AFFECT, 0, &ptod, sizeof(ptod));
}

void SendAffectAddPacket(LPDESC d, CAffect* pkAff)
{
	TPacketGCAffectAdd ptoc;
	ptoc.bHeader = HEADER_GC_AFFECT_ADD;
	ptoc.elem.dwType = pkAff->dwType;
	ptoc.elem.bApplyOn = pkAff->bApplyOn;
	ptoc.elem.lApplyValue = pkAff->lApplyValue;
	ptoc.elem.dwFlag = pkAff->dwFlag;
#ifdef ENABLE_VALUE_PACK_SYSTEM
	if (IS_REAL_TIME_AFFECT(pkAff->dwType))
		ptoc.elem.lDuration = pkAff->lDuration - time(0);
	else
#endif
		ptoc.elem.lDuration = pkAff->lDuration;

	ptoc.elem.lSPCost = pkAff->lSPCost;
	d->Packet(&ptoc, sizeof(TPacketGCAffectAdd));
}
////////////////////////////////////////////////////////////////////
// Affect
CAffect* CHARACTER::FindAffect(DWORD dwType, BYTE bApply) const
{
	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAffect = *it++;

		if (!pkAffect)
			return nullptr;

		if (pkAffect->dwType == dwType && (bApply == APPLY_NONE || bApply == pkAffect->bApplyOn))
			return pkAffect;
	}

	return nullptr;
}

CAffect* CHARACTER::FindAffectByFlag(DWORD dwFlag) const
{
	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAffect = *it++;

		if (!pkAffect)
			return nullptr;
		
		if (pkAffect->dwFlag == dwFlag)
			return pkAffect;
	}

	return nullptr;
}

EVENTFUNC(affect_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("affect_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == nullptr) { // <Factor>
		return 0;
	}

	if (!ch->UpdateAffect())
		return 0;
	else
		return passes_per_sec; // 1초
}

bool CHARACTER::UpdateAffect()
{
#ifdef ENABLE_NONSWORD_SKILL_BUG_FIX
	if (IsAffectFlag(AFF_GWIGUM) && !GetWear(WEAR_WEAPON))
		RemoveAffect(SKILL_GWIGEOM);
	if (IsAffectFlag(AFF_GEOMGYEONG) && !GetWear(WEAR_WEAPON))
		RemoveAffect(SKILL_GEOMKYUNG);
#endif
	// affect_event 에서 처리할 일은 아니지만, 1초짜리 이벤트에서 처리하는 것이
	// 이것 뿐이라 여기서 물약 처리를 한다.
	if (GetPoint(POINT_HP_RECOVERY) > 0)
	{
		if (GetMaxHP() <= GetHP())
		{
			PointChange(POINT_HP_RECOVERY, -GetPoint(POINT_HP_RECOVERY));
		}
		else
		{
			int iVal = MIN(GetPoint(POINT_HP_RECOVERY), GetMaxHP() * 7 / 100);

			PointChange(POINT_HP, iVal);
			PointChange(POINT_HP_RECOVERY, -iVal);
		}
	}

	if (GetPoint(POINT_SP_RECOVERY) > 0)
	{
		if (GetMaxSP() <= GetSP())
			PointChange(POINT_SP_RECOVERY, -GetPoint(POINT_SP_RECOVERY));
		else
		{
			int iVal = MIN(GetPoint(POINT_SP_RECOVERY), GetMaxSP() * 7 / 100);

			PointChange(POINT_SP, iVal);
			PointChange(POINT_SP_RECOVERY, -iVal);
		}
	}

	if (GetPoint(POINT_HP_RECOVER_CONTINUE) > 0)
	{
		PointChange(POINT_HP, GetPoint(POINT_HP_RECOVER_CONTINUE));
	}

	if (GetPoint(POINT_SP_RECOVER_CONTINUE) > 0)
	{
		PointChange(POINT_SP, GetPoint(POINT_SP_RECOVER_CONTINUE));
	}

	AutoRecoveryItemProcess(AFFECT_AUTO_HP_RECOVERY);
	AutoRecoveryItemProcess(AFFECT_AUTO_SP_RECOVERY);

	// 스테미나 회복
	if (GetMaxStamina() > GetStamina())
	{
		int iSec = (get_dword_time() - GetStopTime()) / 3000;
		if (iSec)
			PointChange(POINT_STAMINA, GetMaxStamina() / 1);
	}

	// ProcessAffect는 affect가 없으면 true를 리턴한다.
	if (ProcessAffect())
		if (GetPoint(POINT_HP_RECOVERY) == 0 && GetPoint(POINT_SP_RECOVERY) == 0 && GetStamina() == GetMaxStamina())
		{
			m_pkAffectEvent = nullptr;
			return false;
		}

	return true;
}

void CHARACTER::StartAffectEvent()
{
	if (m_pkAffectEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkAffectEvent = event_create(affect_event, info, passes_per_sec);
	sys_log(1, "StartAffectEvent %s %p %p", GetName(), this, get_pointer(m_pkAffectEvent));
}

void CHARACTER::ClearAffect(bool bSave)
{
	TAffectFlag afOld = m_afAffectFlag;
	WORD	wMovSpd = GetPoint(POINT_MOV_SPEED);
	WORD	wAttSpd = GetPoint(POINT_ATT_SPEED);

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAff = *it;

		if (bSave)
		{
			if (IS_NO_CLEAR_ON_DEATH_AFFECT(pkAff->dwType) || IS_NO_SAVE_AFFECT(pkAff->dwType))
			{
				++it;
				continue;
			}
			
#ifdef ENABLE_BLEND_ICON_SYSTEM
			int pkAff_blend[] = {AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9, AFFECT_POTION_10, AFFECT_POTION_11, AFFECT_POTION_12, AFFECT_POTION_13};
		  
			if ((pkAff->dwType == pkAff_blend[0]) || (pkAff->dwType == pkAff_blend[1]) || (pkAff->dwType == pkAff_blend[2]) ||
				(pkAff->dwType == pkAff_blend[3]) || (pkAff->dwType == pkAff_blend[4]) || (pkAff->dwType == pkAff_blend[5]) || (pkAff->dwType == pkAff_blend[6])
				|| (pkAff->dwType == pkAff_blend[7]) || (pkAff->dwType == pkAff_blend[8]) || (pkAff->dwType == pkAff_blend[9]) || (pkAff->dwType == pkAff_blend[10])
				|| (pkAff->dwType == pkAff_blend[11]) || (pkAff->dwType == pkAff_blend[12]))
			{
				++it;
				continue;
			}
#endif
			
#ifdef ENABLE_PERMA_BLEND_SYSTEM
			int new_aff[] = {AFFECT_18385, AFFECT_18386, AFFECT_18387, AFFECT_18388, AFFECT_18389, AFFECT_18390, AFFECT_18391, AFFECT_18392, AFFECT_18393, AFFECT_18394, AFFECT_18395, AFFECT_13131, AFFECT_ATT_SPEED, AFFECT_MOV_SPEED};
		  
			if ((pkAff->dwType == new_aff[0]) || (pkAff->dwType == new_aff[1]) || (pkAff->dwType == new_aff[2]) ||
				(pkAff->dwType == new_aff[3]) || (pkAff->dwType == new_aff[4]) || (pkAff->dwType == new_aff[5]) ||
				(pkAff->dwType == new_aff[6]) || (pkAff->dwType == new_aff[7]) || (pkAff->dwType == new_aff[8]) ||
				(pkAff->dwType == new_aff[9]) || (pkAff->dwType == new_aff[10]) || (pkAff->dwType == new_aff[11]) ||
				(pkAff->dwType == new_aff[12]) || (pkAff->dwType == new_aff[13]))
			{
				++it;
				continue;
			}
#endif

			if (IsPC())
			{
				SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
			}
		}

		ComputeAffect(pkAff, false);

		it = m_list_pkAffect.erase(it);
		CAffect::Release(pkAff);
	}

	if (afOld != m_afAffectFlag || wMovSpd != GetPoint(POINT_MOV_SPEED) || wAttSpd != GetPoint(POINT_ATT_SPEED))
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif

	CheckMaximumPoints();

	if (m_list_pkAffect.empty())
		event_cancel(&m_pkAffectEvent);
}

void CHARACTER::ClearAffect_New(bool bSave)
{
	TAffectFlag afOld = m_afAffectFlag;
	WORD	wMovSpd = GetPoint(POINT_MOV_SPEED);
	WORD	wAttSpd = GetPoint(POINT_ATT_SPEED);

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAff = *it;

		if (bSave)
		{
			if (!IS_NO_UPDATE_PACKET_AFFECT(pkAff->dwType))
			{
				++it;
				continue;
			}

			if (IsPC())
			{
				SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
			}
		}

		ComputeAffect(pkAff, false);

		it = m_list_pkAffect.erase(it);
		CAffect::Release(pkAff);
	}
	
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	RemoveAffect(AFFECT_MOUNT);
	RemoveAffect(AFFECT_MOUNT_BONUS);
#endif

	if (afOld != m_afAffectFlag || wMovSpd != GetPoint(POINT_MOV_SPEED) || wAttSpd != GetPoint(POINT_ATT_SPEED))
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif

	CheckMaximumPoints();

	if (m_list_pkAffect.empty())
		event_cancel(&m_pkAffectEvent);
}

int CHARACTER::ProcessAffect()
{
	bool	bDiff = false;
	CAffect* pkAff = nullptr;

	//
	// 프리미엄 처리
	//
	for (int i = 0; i <= PREMIUM_MAX_NUM; ++i)
	{
		int aff_idx = i + AFFECT_PREMIUM_START;

		pkAff = FindAffect(aff_idx);

		if (!pkAff)
			continue;

		int remain = GetPremiumRemainSeconds(i);

		if (remain < 0)
		{
			RemoveAffect(aff_idx);
			bDiff = true;
		}
		else
			pkAff->lDuration = remain + 1;
	}

	////////// HAIR_AFFECT
	pkAff = FindAffect(AFFECT_HAIR);
	if (pkAff)
	{
		// IF HAIR_LIMIT_TIME() < CURRENT_TIME()
		if (this->GetQuestFlag("hair.limit_time") < get_global_time())
		{
			// SET HAIR NORMAL
			this->SetPart(PART_HAIR, 0);
			// REMOVE HAIR AFFECT
			RemoveAffect(AFFECT_HAIR);
		}
		else
		{
			// INCREASE AFFECT DURATION
			++(pkAff->lDuration);
		}
	}
	////////// HAIR_AFFECT
	//

	CHorseNameManager::instance().Validate(this);

	TAffectFlag afOld = m_afAffectFlag;
	long lMovSpd = GetPoint(POINT_MOV_SPEED);
	long lAttSpd = GetPoint(POINT_ATT_SPEED);

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		pkAff = *it;

		bool bEnd = false;

		if (pkAff->dwType >= GUILD_SKILL_START && pkAff->dwType <= GUILD_SKILL_END)
		{
			if (!GetGuild() || !GetGuild()->UnderAnyWar())
				bEnd = true;
		}

		if (pkAff->lSPCost > 0)
		{
			if (GetSP() < pkAff->lSPCost)
				bEnd = true;
			else
				PointChange(POINT_SP, -pkAff->lSPCost);
		}

		// AFFECT_DURATION_BUG_FIX
		// 무한 효과 아이템도 시간을 줄인다.
		// 시간을 매우 크게 잡기 때문에 상관 없을 것이라 생각됨.
		if (--pkAff->lDuration <= 0)
		{
			bEnd = true;
		}
		// END_AFFECT_DURATION_BUG_FIX

		if (IS_REAL_TIME_AFFECT(pkAff->dwType))
		{
			if (--pkAff->lDuration <= time(0))
				bEnd = true;
			else
				bEnd = false;
		}

		if (bEnd)
		{
			it = m_list_pkAffect.erase(it);
			ComputeAffect(pkAff, false);
			bDiff = true;
			if (IsPC())
			{
				SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
			}

			CAffect::Release(pkAff);

			continue;
		}

		++it;
	}

	if (bDiff)
	{
		if (afOld != m_afAffectFlag || lMovSpd != GetPoint(POINT_MOV_SPEED) || lAttSpd != GetPoint(POINT_ATT_SPEED))
		{
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
			UpdateAffectFlag();
#else
			UpdatePacket();
#endif
		}

		CheckMaximumPoints();
	}

	if (m_list_pkAffect.empty())
		return true;

	return false;
}

void CHARACTER::SaveAffect()
{
	TPacketGDAddAffect p;

	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAff = *it++;

		if (IS_NO_SAVE_AFFECT(pkAff->dwType))
			continue;

		sys_log(1, "AFFECT_SAVE: %u %u %d %d", pkAff->dwType, pkAff->bApplyOn, pkAff->lApplyValue, pkAff->lDuration);

		p.dwPID = GetPlayerID();
		p.elem.dwType = pkAff->dwType;
		p.elem.bApplyOn = pkAff->bApplyOn;
		p.elem.lApplyValue = pkAff->lApplyValue;
		p.elem.dwFlag = pkAff->dwFlag;
		p.elem.lDuration = pkAff->lDuration;
		p.elem.lSPCost = pkAff->lSPCost;
		db_clientdesc->DBPacket(HEADER_GD_ADD_AFFECT, 0, &p, sizeof(p));
	}
}

EVENTINFO(load_affect_login_event_info)
{
	DWORD pid;
	DWORD count;
	char* data;

	load_affect_login_event_info()
		: pid(0)
		, count(0)
		, data(0)
	{
	}
};

EVENTFUNC(load_affect_login_event)
{
	load_affect_login_event_info* info = dynamic_cast<load_affect_login_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("load_affect_login_event_info> <Factor> nullptr pointer");
		return 0;
	}

	DWORD dwPID = info->pid;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);

	if (!ch)
	{
		M2_DELETE_ARRAY(info->data);
		return 0;
	}

	LPDESC d = ch->GetDesc();

	if (!d)
	{
		M2_DELETE_ARRAY(info->data);
		return 0;
	}

	if (d->IsPhase(PHASE_HANDSHAKE) ||
		d->IsPhase(PHASE_LOGIN) ||
		d->IsPhase(PHASE_SELECT) ||
		d->IsPhase(PHASE_DEAD) ||
		d->IsPhase(PHASE_LOADING))
	{
		return PASSES_PER_SEC(1);
	}
	else if (d->IsPhase(PHASE_CLOSE))
	{
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
	else if (d->IsPhase(PHASE_GAME))
	{
		sys_log(1, "Affect Load by Event");
		ch->LoadAffect(info->count, (TPacketAffectElement*)info->data);
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
	else
	{
		sys_err("input_db.cpp:quest_login_event INVALID PHASE pid %d", ch->GetPlayerID());
		M2_DELETE_ARRAY(info->data);
		return 0;
	}
}

void CHARACTER::LoadAffect(DWORD dwCount, TPacketAffectElement * pElements)
{
	m_bIsLoadedAffect = false;

	if (!GetDesc()->IsPhase(PHASE_GAME))
	{
		if (test_server)
			sys_log(0, "LOAD_AFFECT: Creating Event", GetName(), dwCount);

		load_affect_login_event_info* info = AllocEventInfo<load_affect_login_event_info>();

		info->pid = GetPlayerID();
		info->count = dwCount;
		info->data = M2_NEW char[sizeof(TPacketAffectElement) * dwCount];
		thecore_memcpy(info->data, pElements, sizeof(TPacketAffectElement) * dwCount);

		event_create(load_affect_login_event, info, PASSES_PER_SEC(1));

		return;
	}

	ClearAffect(true);

	if (test_server)
		sys_log(0, "LOAD_AFFECT: %s count %d", GetName(), dwCount);

	TAffectFlag afOld = m_afAffectFlag;

	long lMovSpd = GetPoint(POINT_MOV_SPEED);
	long lAttSpd = GetPoint(POINT_ATT_SPEED);

	for (DWORD i = 0; i < dwCount; ++i, ++pElements)
	{
		// 무영진은 로드하지않는다.
		if (pElements->dwType == SKILL_MUYEONG)
			continue;

		if (AFFECT_AUTO_HP_RECOVERY == pElements->dwType || AFFECT_AUTO_SP_RECOVERY == pElements->dwType)
		{
			LPITEM item = FindItemByID(pElements->dwFlag);

			if (nullptr == item)
				continue;

			item->Lock(true);
		}

		if (pElements->bApplyOn >= POINT_MAX_NUM)
		{
			sys_err("invalid affect data %s ApplyOn %u ApplyValue %d",
				GetName(), pElements->bApplyOn, pElements->lApplyValue);
			continue;
		}

		if (test_server)
		{
			sys_log(0, "Load Affect : Affect %s %d %d", GetName(), pElements->dwType, pElements->bApplyOn);
		}

		CAffect* pkAff = CAffect::Acquire();
		m_list_pkAffect.push_back(pkAff);

		pkAff->dwType = pElements->dwType;
		pkAff->bApplyOn = pElements->bApplyOn;
		pkAff->lApplyValue = pElements->lApplyValue;
		pkAff->dwFlag = pElements->dwFlag;
		pkAff->lDuration = pElements->lDuration;
		pkAff->lSPCost = pElements->lSPCost;

		SendAffectAddPacket(GetDesc(), pkAff);

		ComputeAffect(pkAff, true);
	}

	if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
	{
		RemoveGoodAffect();
	}

	if (afOld != m_afAffectFlag || lMovSpd != GetPoint(POINT_MOV_SPEED) || lAttSpd != GetPoint(POINT_ATT_SPEED))
	{
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif
	}

	StartAffectEvent();
#ifdef ENABLE_AFK_MODE_SYSTEM
	if (IsAffectFlag(AFF_AFK))
		RemoveAffect(AFFECT_AFK);
#endif
	m_bIsLoadedAffect = true;
	
	ComputePoints(); // @fixme156

	// 용혼석 셋팅 로드 및 초기화
	DragonSoul_Initialize();

	// @fixme
	if (!IsDead())
	{
		PointChange(POINT_HP, GetMaxHP() - GetHP());
		PointChange(POINT_SP, GetMaxSP() - GetSP());
	}
}

bool CHARACTER::AddAffect(DWORD dwType, BYTE bApplyOn, long lApplyValue, DWORD dwFlag, long lDuration, long lSPCost, bool bOverride, bool IsCube)
{
	// CHAT_BLOCK
	if (dwType == AFFECT_BLOCK_CHAT && lDuration > 1)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("운영자 제제로 채팅이 금지 되었습니다."));
	}
	// END_OF_CHAT_BLOCK

	if (lDuration == 0)
	{
		sys_err("Character::AddAffect lDuration == 0 type %d", lDuration, dwType);
		lDuration = 1;
	}
#if defined(ENABLE_IGNORE_LOWER_BUFFS)
	switch (dwType)
	{
	case SKILL_HOSIN:
	case SKILL_REFLECT:
	case SKILL_GICHEON:
	case SKILL_JEONGEOP:
	case SKILL_KWAESOK:
	case SKILL_JEUNGRYEOK:
#if defined(ENABLE_WOLFMAN_CHARACTER)
	case SKILL_CHEONGRANG:
#endif
	{
		const CAffect* pkAffect = FindAffect(dwType);
		if (!pkAffect)
			break;

		if (lApplyValue < pkAffect->lApplyValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<AddAffect> has blocked receiving skill (%s) because power is (%ld%%) more small then current one (%ld%%)."), CSkillManager::instance().Get(dwType)->szName, lApplyValue, pkAffect->lApplyValue);
			return false;
		}
	}
	break;
	default:
		break;
	}
#endif
	CAffect* pkAff = NULL;

	if (IsCube)
		pkAff = FindAffect(dwType, bApplyOn);
	else
		pkAff = FindAffect(dwType);

	if (dwFlag == AFF_STUN)
	{
		if (m_posDest.x != GetX() || m_posDest.y != GetY())
		{
			m_posDest.x = m_posStart.x = GetX();
			m_posDest.y = m_posStart.y = GetY();
			battle_end(this);

			SyncPacket();
		}
	}

	// 이미 있는 효과를 덮어 쓰는 처리
	if (pkAff && bOverride)
	{
		ComputeAffect(pkAff, false); // 일단 효과를 삭제하고

		if (GetDesc())
			SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
	}
	else
	{
		//
		// 새 에펙를 추가
		//
		// NOTE: 따라서 같은 type 으로도 여러 에펙트를 붙을 수 있다.
		//
		pkAff = CAffect::Acquire();
		m_list_pkAffect.push_back(pkAff);
	}

	sys_log(1, "AddAffect %s type %d apply %d %d flag %u duration %d", GetName(), dwType, bApplyOn, lApplyValue, dwFlag, lDuration);
	sys_log(0, "AddAffect %s type %d apply %d %d flag %u duration %d", GetName(), dwType, bApplyOn, lApplyValue, dwFlag, lDuration);

	pkAff->dwType = dwType;
	pkAff->bApplyOn = bApplyOn;
	pkAff->lApplyValue = lApplyValue;
	pkAff->dwFlag = dwFlag;
	pkAff->lDuration = lDuration;
	pkAff->lSPCost = lSPCost;

	WORD wMovSpd = GetPoint(POINT_MOV_SPEED);
	WORD wAttSpd = GetPoint(POINT_ATT_SPEED);

	ComputeAffect(pkAff, true);

	if (pkAff->dwFlag || wMovSpd != GetPoint(POINT_MOV_SPEED) || wAttSpd != GetPoint(POINT_ATT_SPEED))
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif

	StartAffectEvent();

	if (IsPC())
	{
		SendAffectAddPacket(GetDesc(), pkAff);

		if (IS_NO_SAVE_AFFECT(pkAff->dwType))
			return true;

		TPacketGDAddAffect p;
		p.dwPID = GetPlayerID();
		p.elem.dwType = pkAff->dwType;
		p.elem.bApplyOn = pkAff->bApplyOn;
		p.elem.lApplyValue = pkAff->lApplyValue;
		p.elem.dwFlag = pkAff->dwFlag;
		p.elem.lDuration = pkAff->lDuration;
		p.elem.lSPCost = pkAff->lSPCost;
		db_clientdesc->DBPacket(HEADER_GD_ADD_AFFECT, 0, &p, sizeof(p));
	}

	return true;
}

void CHARACTER::RefreshAffect()
{
	auto it = m_list_pkAffect.begin();

	while (it != m_list_pkAffect.end())
	{
		CAffect* pkAff = *it++;
		ComputeAffect(pkAff, true);
	}
}

void CHARACTER::ComputeAffect(CAffect * pkAff, bool bAdd)
{
	if (bAdd && pkAff->dwType >= GUILD_SKILL_START && pkAff->dwType <= GUILD_SKILL_END)
	{
		if (!GetGuild())
			return;

		if (!GetGuild()->UnderAnyWar())
			return;
	}

	if (pkAff->dwFlag)
	{
		if (!bAdd)
			m_afAffectFlag.Reset(pkAff->dwFlag);
		else
			m_afAffectFlag.Set(pkAff->dwFlag);
	}

	if (bAdd)
		PointChange(pkAff->bApplyOn, pkAff->lApplyValue);
	else
		PointChange(pkAff->bApplyOn, -pkAff->lApplyValue);

	if (pkAff->dwType == SKILL_MUYEONG)
	{
		if (bAdd)
			StartMuyeongEvent();
		else
			StopMuyeongEvent();
	}
	
#ifdef ENABLE_GYEONGGONG_SKILL
	if (pkAff->dwType == SKILL_GYEONGGONG)
	{
		if (bAdd)
			StartGyeongGongEvent();
		else
			StopGyeongGongEvent();
	}
#endif
}

bool CHARACTER::RemoveAffect(CAffect * pkAff)
{
	if (!pkAff)
		return false;

	// AFFECT_BUF_FIX
	m_list_pkAffect.remove(pkAff);
	// END_OF_AFFECT_BUF_FIX

	ComputeAffect(pkAff, false);

	if (AFFECT_REVIVE_INVISIBLE != pkAff->dwType) {
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		ComputePoints();
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif
	} else {
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
		UpdateAffectFlag();
#else
		UpdatePacket();
#endif
	}
	
	CheckMaximumPoints();

	if (test_server)
		sys_log(0, "AFFECT_REMOVE: %s (flag %u apply: %u)", GetName(), pkAff->dwFlag, pkAff->bApplyOn);

	if (IsPC())
	{
		SendAffectRemovePacket(GetDesc(), GetPlayerID(), pkAff->dwType, pkAff->bApplyOn);
	}

	CAffect::Release(pkAff);
	return true;
}

bool CHARACTER::RemoveAffect(DWORD dwType)
{
	// CHAT_BLOCK
	if (dwType == AFFECT_BLOCK_CHAT)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("채팅 금지가 풀렸습니다."));
	}
	// END_OF_CHAT_BLOCK

	bool flag = false;

	CAffect* pkAff;

	while ((pkAff = FindAffect(dwType)))
	{
		RemoveAffect(pkAff);
		flag = true;
	}

	return flag;
}

bool CHARACTER::IsAffectFlag(DWORD dwAff) const
{
	return m_afAffectFlag.IsSet(dwAff);
}

void CHARACTER::RemoveGoodAffect()
{
	RemoveAffect(AFFECT_MOV_SPEED);
	RemoveAffect(AFFECT_ATT_SPEED);
	RemoveAffect(AFFECT_STR);
	RemoveAffect(AFFECT_DEX);
	RemoveAffect(AFFECT_INT);
	RemoveAffect(AFFECT_CON);
	RemoveAffect(AFFECT_CHINA_FIREWORK);

	RemoveAffect(SKILL_JEONGWI);
	RemoveAffect(SKILL_GEOMKYUNG);
	RemoveAffect(SKILL_CHUNKEON);
	RemoveAffect(SKILL_EUNHYUNG);
	RemoveAffect(SKILL_GYEONGGONG);
	RemoveAffect(SKILL_GWIGEOM);
	RemoveAffect(SKILL_TERROR);
	RemoveAffect(SKILL_JUMAGAP);
	RemoveAffect(SKILL_MANASHILED);
	RemoveAffect(SKILL_HOSIN);
	RemoveAffect(SKILL_REFLECT);
	RemoveAffect(SKILL_KWAESOK);
	RemoveAffect(SKILL_JEUNGRYEOK);
	RemoveAffect(SKILL_GICHEON);
#ifdef ENABLE_WOLFMAN_CHARACTER
	RemoveAffect(SKILL_JEOKRANG);
	RemoveAffect(SKILL_CHEONGRANG);
#endif
}

bool CHARACTER::IsGoodAffect(BYTE bAffectType) const
{
	switch (bAffectType)
	{
	case (AFFECT_MOV_SPEED):
	case (AFFECT_ATT_SPEED):
	case (AFFECT_STR):
	case (AFFECT_DEX):
	case (AFFECT_INT):
	case (AFFECT_CON):
	case (AFFECT_CHINA_FIREWORK):

	case (SKILL_JEONGWI):
	case (SKILL_GEOMKYUNG):
	case (SKILL_CHUNKEON):
	case (SKILL_EUNHYUNG):
	case (SKILL_GYEONGGONG):
	case (SKILL_GWIGEOM):
	case (SKILL_TERROR):
	case (SKILL_JUMAGAP):
	case (SKILL_MANASHILED):
	case (SKILL_HOSIN):
	case (SKILL_REFLECT):
	case (SKILL_KWAESOK):
	case (SKILL_JEUNGRYEOK):
	case (SKILL_GICHEON):
#ifdef ENABLE_WOLFMAN_CHARACTER
	case (SKILL_JEOKRANG):
	case (SKILL_CHEONGRANG):
#endif
		return true;
	}
	return false;
}

void CHARACTER::RemoveBadAffect()
{
	sys_log(0, "RemoveBadAffect %s", GetName());

	RemovePoison();
	RemoveFire();
#ifdef ENABLE_WOLFMAN_CHARACTER
	RemoveBleeding();
#endif
	RemoveAffect(AFFECT_STUN);
	RemoveAffect(AFFECT_SLOW);
	RemoveAffect(SKILL_TUSOK);
}

void CHARACTER::SetAffectStack(CAffect * pkAff, uint8_t value)
{
	if (!GetAffectStack(pkAff))
	{
		m_map_affectStack.insert(std::make_pair(pkAff->dwType, value));
		return;
	}

	m_map_affectStack[pkAff->dwType] = value;
}

uint8_t CHARACTER::GetAffectStack(CAffect * pkAff)
{
	auto stackIt = m_map_affectStack.find(pkAff->dwType);
	if (stackIt == m_map_affectStack.end())
		return 0;

	return m_map_affectStack[pkAff->dwType];
}

void CHARACTER::ClearAffectStack(CAffect * pkAff) {
	if (!GetAffectStack(pkAff))
		return;

	m_map_affectStack.erase(pkAff->dwType);
}