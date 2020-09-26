#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "desc.h"
#include "sectree_manager.h"
#include "packet.h"
#include "protocol.h"
#include "log.h"
#include "skill.h"
#include "unique_item.h"
#include "profiler.h"
#include "marriage.h"
#include "item_addon.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"
#include "affect.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt.h"
#include "../../common/VnumHelper.h"
#include "mob_manager.h"
#ifdef ENABLE_EXTENDED_PET_SYSTEM
#include "PetSystem.h"
#endif
#include "war_map.h"
#include "features.h"

CItem::CItem(DWORD dwVnum)
	: m_dwVnum(dwVnum), m_bWindow(0), m_dwID(0), m_bEquipped(false), m_dwVID(0), m_wCell(0), m_dwCount(0), m_lFlag(0), m_dwLastOwnerPID(0),
	m_bExchanging(false), m_pkDestroyEvent(nullptr), m_pkExpireEvent(nullptr), m_pkUniqueExpireEvent(nullptr),
	m_pkTimerBasedOnWearExpireEvent(nullptr), m_pkRealTimeExpireEvent(nullptr),
	m_pkAccessorySocketExpireEvent(nullptr), m_pkOwnershipEvent(nullptr), m_dwOwnershipPID(0), m_bSkipSave(false), m_isLocked(false),
	m_dwMaskVnum(0), m_dwSIGVnum(0)
#ifdef ENABLE_CHANGELOOK_SYSTEM
	, m_dwTransmutation(0)
#endif
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	, m_lSealbind(0), m_pkUnSealTimerExpireEvent(nullptr)
#endif
	, is_basic(false)
{
	memset(&m_alSockets, 0, sizeof(m_alSockets));
	memset(&m_aAttr, 0, sizeof(m_aAttr));
}

CItem::~CItem()
{
	Destroy();
}

void CItem::Initialize()
{
	CEntity::Initialize(ENTITY_ITEM);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	m_dwRealID = 0;
#endif
	m_bWindow = RESERVED_WINDOW;
	m_pOwner = nullptr;
	m_dwID = 0;
	m_bEquipped = false;
	m_dwVID = m_wCell = m_dwCount = m_lFlag = 0;
	m_pProto = nullptr;
	m_bExchanging = false;
	memset(&m_alSockets, 0, sizeof(m_alSockets));
	memset(&m_aAttr, 0, sizeof(m_aAttr));
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	m_lSealbind = 0;
	m_pkUnSealTimerExpireEvent = nullptr;
#endif
#ifdef ENABLE_CHANGELOOK_SYSTEM
	m_dwTransmutation = 0;
#endif

	m_pkDestroyEvent = nullptr;
	m_pkOwnershipEvent = nullptr;
	m_dwOwnershipPID = 0;
	m_pkUniqueExpireEvent = nullptr;
	m_pkTimerBasedOnWearExpireEvent = nullptr;
	m_pkRealTimeExpireEvent = nullptr;

	m_pkAccessorySocketExpireEvent = nullptr;

	m_bSkipSave = false;
	m_dwLastOwnerPID = 0;
	is_basic = false;
}

void CItem::Destroy()
{
	event_cancel(&m_pkDestroyEvent);
	event_cancel(&m_pkOwnershipEvent);
	event_cancel(&m_pkUniqueExpireEvent);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);
	event_cancel(&m_pkRealTimeExpireEvent);
	event_cancel(&m_pkAccessorySocketExpireEvent);
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	event_cancel(&m_pkUnSealTimerExpireEvent);
#endif

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);
}

EVENTFUNC(item_destroy_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("item_destroy_event> <Factor> nullptr pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetOwner())
		sys_err("item_destroy_event: Owner exist. (item %s owner %s)", pkItem->GetName(), pkItem->GetOwner()->GetName());

	pkItem->SetDestroyEvent(nullptr);
	M2_DESTROY_ITEM(pkItem);
	return 0;
}

void CItem::SetDestroyEvent(LPEVENT pkEvent)
{
	m_pkDestroyEvent = pkEvent;
}

void CItem::StartDestroyEvent(int iSec)
{
	if (m_pkDestroyEvent)
		return;

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetDestroyEvent(event_create(item_destroy_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::EncodeInsertPacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	const PIXEL_POSITION & c_pos = GetXYZ();

	struct packet_item_ground_add pack;

	pack.bHeader = HEADER_GC_ITEM_GROUND_ADD;
	pack.x = c_pos.x;
	pack.y = c_pos.y;
	pack.z = c_pos.z;
	pack.dwVnum = GetVnum();
	pack.dwVID = m_dwVID;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];
	pack.count = m_dwCount;

	d->Packet(&pack, sizeof(pack));

	if (m_pkOwnershipEvent != nullptr)
	{
		item_event_info* info = dynamic_cast<item_event_info*>(m_pkOwnershipEvent->info);

		if (info == nullptr)
		{
			sys_err("CItem::EncodeInsertPacket> <Factor> nullptr pointer");
			return;
		}

		TPacketGCItemOwnership p;

		p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
		p.dwVID = m_dwVID;
		strlcpy(p.szName, info->szOwnerName, sizeof(p.szName));

		d->Packet(&p, sizeof(TPacketGCItemOwnership));
	}
}

void CItem::EncodeRemovePacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	struct packet_item_ground_del pack;

	pack.bHeader = HEADER_GC_ITEM_GROUND_DEL;
	pack.dwVID = m_dwVID;

	d->Packet(&pack, sizeof(pack));
	sys_log(2, "Item::EncodeRemovePacket %s to %s", GetName(), ((LPCHARACTER)ent)->GetName());
}

void CItem::SetProto(const TItemTable * table)
{
	assert(table != nullptr);
	m_pProto = table;
	SetFlag(m_pProto->dwFlags);
}

void CItem::UsePacketEncode(LPCHARACTER ch, LPCHARACTER victim, struct packet_item_use* packet)
{
	if (!GetVnum())
		return;

	packet->header = HEADER_GC_ITEM_USE;
	packet->ch_vid = ch->GetVID();
	packet->victim_vid = victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
	packet->vnum = GetVnum();
}

void CItem::RemoveFlag(long bit)
{
	REMOVE_BIT(m_lFlag, bit);
}

void CItem::AddFlag(long bit)
{
	SET_BIT(m_lFlag, bit);
}

void CItem::UpdatePacket()
{
	if (!m_pOwner || !m_pOwner->GetDesc())
		return;

	TPacketGCItemUpdate pack;

	pack.header = HEADER_GC_ITEM_UPDATE;
	pack.Cell = TItemPos(GetWindow(), m_wCell);
	pack.count = m_dwCount;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	pack.transmutation = m_dwTransmutation;
#endif
	pack.is_basic = is_basic;

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];

	thecore_memcpy(pack.aAttr, GetAttributes(), sizeof(pack.aAttr));
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	pack.sealbind = m_lSealbind;
#endif

	sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
	m_pOwner->GetDesc()->Packet(&pack, sizeof(TPacketGCItemUpdate));
}

#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
void CItem::SetSealBind(long sp)
{
	m_lSealbind = sp;
	UpdatePacket();
	Save();
}

EVENTFUNC(unbind_timer_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);
	if (!info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);
	if (!item)
		return 0;

	const time_t current = get_global_time();
	if (current > item->GetSealBindTime())
	{
		item->SetSealBind(0);
		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartUnSealBindTimerExpireEvent()
{
	if (m_pkUnSealTimerExpireEvent)
		return;

	item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	m_pkUnSealTimerExpireEvent = event_create(unbind_timer_expire_event, info, PASSES_PER_SEC(1));
	sys_log(0, "UNSEAL_TIME_EXPIRE: StartUnSealBindTimerExpireEvent");
}

#endif

DWORD CItem::GetCount()
{
	if (m_dwVnum == 1) 
		return MIN(m_dwCount, INT_MAX);
	else
	{
		return MIN(m_dwCount, g_bItemCountLimit);
	}
}

bool CItem::SetCount(DWORD count, bool bSkipUpdatePacket)
{
	if (m_dwVnum == 1)
	{
		m_dwCount = MIN(count, INT_MAX);
	}
	else
	{
		m_dwCount = MIN(count, g_bItemCountLimit);
	}

	if (count == 0 && m_pOwner)
	{
		if (GetSubType() == USE_ABILITY_UP || GetSubType() == USE_POTION || GetVnum() == 70020)
		{
			LPCHARACTER pOwner = GetOwner();
			WORD wCell = GetCell();

			RemoveFromCharacter();

			if (!IsDragonSoul())
			{
				LPITEM pItem = pOwner->FindSpecifyItem(GetVnum());

				if (nullptr != pItem)
				{
					pOwner->ChainQuickslotItem(pItem, QUICKSLOT_TYPE_ITEM, wCell);
				}
				else
				{
					pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, wCell, 1000);
				}
			}

			M2_DESTROY_ITEM(this);
		}
		else
		{
			// @fixpch003
			if (!IsDragonSoul() && !IsSkillBook() && !IsUpgradeItem() && !IsStone() && !IsAttr() && !IsFlower() && !IsChest())
			{
				m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, m_wCell, 1000);
			}
			M2_DESTROY_ITEM(RemoveFromCharacter());
		}

		return false;
	}
	
	UpdatePacket();
	Save();
	return true;
}

#ifdef ENABLE_CHANGELOOK_SYSTEM
DWORD CItem::GetTransmutation() const
{
	if (!features::IsFeatureEnabled(features::CHANGELOOK))
		return 0;
	else
		return m_dwTransmutation;
}

void CItem::SetTransmutation(DWORD dwVnum, bool bLog)
{
	m_dwTransmutation = dwVnum;
	UpdatePacket();
	Save();
}
#endif

LPITEM CItem::RemoveFromCharacter()
{
	if (!m_pOwner)
	{
		sys_err("Item::RemoveFromCharacter owner nullptr");
		return (this);
	}

	LPCHARACTER pOwner = m_pOwner;

	if (m_bEquipped)	// 장착되었는가?
	{
		Unequip();
		//pOwner->UpdatePacket();

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
	else
	{
		if (GetWindow() != SAFEBOX && GetWindow() != MALL)
		{
			if (IsDragonSoul())
			{
				if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= DRAGON_SOUL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
			else if (IsSkillBook())
			{
				if (m_wCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= SKILL_BOOK_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
			else if (IsUpgradeItem())
			{
				if (m_wCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= UPGRADE_ITEMS_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
			else if (IsStone())
			{
				if (m_wCell >= STONE_ITEMS_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= STONE_ITEMS_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
			else if (IsChest())
			{
				if (m_wCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= CHEST_ITEMS_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
			else if (IsAttr())
			{
				if (m_wCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= ATTR_ITEMS_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
			else if (IsFlower())
			{
				if (m_wCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= FLOWER_ITEMS_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), nullptr);
			}
#endif
			else
			{
				TItemPos cell(INVENTORY, m_wCell);

				if (false == cell.IsDefaultInventoryPosition() && false == cell.IsBeltInventoryPosition()) // 아니면 소지품에?
					sys_err("CItem::RemoveFromCharacter: Invalid Item Position");
				else
				{
					pOwner->SetItem(cell, nullptr);
				}
			}
		}

		m_pOwner = nullptr;
		m_wCell = 0;

		SetWindow(RESERVED_WINDOW);
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if (!GetRealID())
			Save();
#else
		Save();
#endif
		return (this);
	}
}

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
bool CItem::AddToCharacter(LPCHARACTER ch, TItemPos Cell, bool isHighLight)
#else
bool CItem::AddToCharacter(LPCHARACTER ch, TItemPos Cell)
#endif
{
	assert(GetSectree() == nullptr);
	assert(m_pOwner == nullptr);
	WORD pos = Cell.cell;
	BYTE window_type = Cell.window_type;

	if (INVENTORY == window_type)
	{
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM
		if (m_wCell >= ch->GetExtendInvenMax() && BELT_INVENTORY_SLOT_START > m_wCell)
#else
		if (m_wCell >= INVENTORY_MAX_NUM && BELT_INVENTORY_SLOT_START > m_wCell)
#endif
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	else if (SKILL_BOOK_INVENTORY == window_type)
	{
		if (m_wCell >= SKILL_BOOK_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (UPGRADE_ITEMS_INVENTORY == window_type)
	{
		if (m_wCell >= UPGRADE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (STONE_ITEMS_INVENTORY == window_type)
	{
		if (m_wCell >= STONE_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (CHEST_ITEMS_INVENTORY == window_type)
	{
		if (m_wCell >= CHEST_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (ATTR_ITEMS_INVENTORY == window_type)
	{
		if (m_wCell >= ATTR_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (FLOWER_ITEMS_INVENTORY == window_type)
	{
		if (m_wCell >= FLOWER_ITEMS_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif
	else if (DRAGON_SOUL_INVENTORY == window_type)
	{
		if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}

	if (ch->GetDesc())
		m_dwLastOwnerPID = ch->GetPlayerID();

#ifdef ENABLE_ACCE_SYSTEM
	if ((GetType() == ITEM_COSTUME && GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
		case 2:
		{
			lVal = SASH_GRADE_2_ABS;
		}
		break;
		case 3:
		{
			lVal = SASH_GRADE_3_ABS;
		}
		break;
		case 4:
		{
			lVal = number(SASH_GRADE_4_ABS_MIN, SASH_GRADE_4_ABS_MAX_COMB);
		}
		break;
		default:
		{
			lVal = SASH_GRADE_1_ABS;
		}
		break;
		}

		SetSocket(SASH_ABSORPTION_SOCKET, lVal);
	}
#endif

#ifdef ENABLE_AURA_SYSTEM
	if ((GetType() == ITEM_COSTUME && GetSubType() == COSTUME_AURA) && (GetSocket(AURA_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(AURA_GRADE_VALUE_FIELD);
		if (lVal < 1)
		{
			SetSocket(AURA_GRADE_VALUE_FIELD, 1);
			SetSocket(AURA_ABSORPTION_SOCKET, 1);
		}
		else
			SetSocket(AURA_ABSORPTION_SOCKET, lVal);
	}
#endif

	event_cancel(&m_pkDestroyEvent);

#ifdef ENABLE_ITEM_HIGHLIGHT_SYSTEM
	ch->SetItem(TItemPos(window_type, pos), this, isHighLight);
#else
	ch->SetItem(TItemPos(window_type, pos), this);
#endif

	m_pOwner = ch;

	Save();
	return true;
}

LPITEM CItem::RemoveFromGround()
{
	if (GetSectree())
	{
		SetOwnership(nullptr);

		GetSectree()->RemoveEntity(this);

		ViewCleanup();

		Save();
	}

	return (this);
}

bool CItem::AddToGround(long lMapIndex, const PIXEL_POSITION & pos, bool skipOwnerCheck)
{
	if (0 == lMapIndex)
	{
		sys_err("wrong map index argument: %d", lMapIndex);
		return false;
	}

	if (GetSectree())
	{
		sys_err("sectree already assigned");
		return false;
	}

	if (!skipOwnerCheck && m_pOwner)
	{
		sys_err("owner pointer not nullptr");
		return false;
	}

	LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, pos.x, pos.y);

	if (!tree)
	{
		sys_err("cannot find sectree by %dx%d", pos.x, pos.y);
		return false;
	}

	//tree->Touch();

	SetWindow(GROUND);
	SetXYZ(pos.x, pos.y, pos.z);
	tree->InsertEntity(this);
	UpdateSectree();
	Save();
	return true;
}

bool CItem::DistanceValid(LPCHARACTER ch)
{
	if (!GetSectree())
		return false;

	int iDist = DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY());

	if (iDist > 300)
		return false;

	return true;
}

bool CItem::CanUsedBy(LPCHARACTER ch)
{
	switch (ch->GetJob())
	{
	case JOB_WARRIOR:
		if (GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
			return false;
		break;

	case JOB_ASSASSIN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
			return false;
		break;

	case JOB_SHAMAN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		break;

	case JOB_SURA:
		if (GetAntiFlag() & ITEM_ANTIFLAG_SURA)
			return false;
		break;

	case JOB_WOLFMAN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
			return false;
		break;
	}

	return true;
}

int CItem::FindEquipCell(LPCHARACTER ch, int iCandidateCell)
{
	// 코스츔 아이템(ITEM_COSTUME)은 WearFlag 없어도 됨. (sub type으로 착용위치 구분. 귀찮게 또 wear flag 줄 필요가 있나..)
	// 용혼석(ITEM_DS, ITEM_SPECIAL_DS)도  SUB_TYPE으로 구분. 신규 반지, 벨트는 ITEM_TYPE으로 구분 -_-
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType() && ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType() && ITEM_RING != GetType() && ITEM_BELT != GetType())
		return -1;

	// 용혼석 슬롯을 WEAR로 처리할 수가 없어서(WEAR는 최대 32개까지 가능한데 용혼석을 추가하면 32가 넘는다.)
	// 인벤토리의 특정 위치((INVENTORY_MAX_NUM + WEAR_MAX_NUM)부터 (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1)까지)를
	// 용혼석 슬롯으로 정함.
	// return 할 때에, INVENTORY_MAX_NUM을 뺀 이유는,
	// 본래 WearCell이 INVENTORY_MAX_NUM를 빼고 return 하기 때문.
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}
		else
		{
			for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
			{
				if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
				{
					return iCandidateCell;
				}
			}
			return -1;
		}
	}
	else if (GetType() == ITEM_COSTUME)
	{
		if (GetSubType() == COSTUME_BODY)
			return WEAR_COSTUME_BODY;
		else if (GetSubType() == COSTUME_HAIR)
			return WEAR_COSTUME_HAIR;
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		else if (GetSubType() == COSTUME_WEAPON)
			return WEAR_COSTUME_WEAPON;
#endif
#ifdef ENABLE_ACCE_SYSTEM
		else if (GetSubType() == COSTUME_SASH)
		{
			if (features::IsFeatureEnabled(features::ACCE))
				return WEAR_COSTUME_SASH;
			else
				return -1;
		}
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		else if (GetSubType() == COSTUME_MOUNT && !CWarMapManager::instance().IsWarMap(m_pOwner->GetMapIndex()))
			return WEAR_COSTUME_MOUNT;
#endif
#ifdef ENABLE_AURA_SYSTEM
		else if (GetSubType() == COSTUME_AURA)
		{
			if (features::IsFeatureEnabled(features::AURA))
				return WEAR_COSTUME_AURA;
			else
				return -1;
		}
#endif
	}
	else if (GetType() == ITEM_RING)
	{
		if (ch->GetWear(WEAR_RING1))
			return WEAR_RING2;
		else
			return WEAR_RING1;
	}
	else if (GetType() == ITEM_BELT)
		return WEAR_BELT;
	else if (GetWearFlag() & WEARABLE_BODY)
		return WEAR_BODY;
	else if (GetWearFlag() & WEARABLE_HEAD)
		return WEAR_HEAD;
	else if (GetWearFlag() & WEARABLE_FOOTS)
		return WEAR_FOOTS;
	else if (GetWearFlag() & WEARABLE_WRIST)
		return WEAR_WRIST;
	else if (GetWearFlag() & WEARABLE_WEAPON)
		return WEAR_WEAPON;
	else if (GetWearFlag() & WEARABLE_SHIELD)
		return WEAR_SHIELD;
	else if (GetWearFlag() & WEARABLE_NECK)
		return WEAR_NECK;
	else if (GetWearFlag() & WEARABLE_EAR)
		return WEAR_EAR;
	else if (GetWearFlag() & WEARABLE_ARROW)
		return WEAR_ARROW;
	else if (GetWearFlag() & WEARABLE_UNIQUE)
	{
		if (ch->GetWear(WEAR_UNIQUE1))
			return WEAR_UNIQUE2;
		else
			return WEAR_UNIQUE1;
	}

	// 수집 퀘스트를 위한 아이템이 박히는곳으로 한번 박히면 절대 뺼수 없다.
	else if (GetWearFlag() & WEARABLE_ABILITY)
	{
		if (!ch->GetWear(WEAR_ABILITY1))
		{
			return WEAR_ABILITY1;
		}
		else if (!ch->GetWear(WEAR_ABILITY2))
		{
			return WEAR_ABILITY2;
		}
		else if (!ch->GetWear(WEAR_ABILITY3))
		{
			return WEAR_ABILITY3;
		}
		else if (!ch->GetWear(WEAR_ABILITY4))
		{
			return WEAR_ABILITY4;
		}
		else if (!ch->GetWear(WEAR_ABILITY5))
		{
			return WEAR_ABILITY5;
		}
		else if (!ch->GetWear(WEAR_ABILITY6))
		{
			return WEAR_ABILITY6;
		}
		else if (!ch->GetWear(WEAR_ABILITY7))
		{
			return WEAR_ABILITY7;
		}
		else if (!ch->GetWear(WEAR_ABILITY8))
		{
			return WEAR_ABILITY8;
		}
		else
		{
			return -1;
		}
	}

#ifdef ENABLE_TALISMAN_SYSTEM
	if (IsCharmItem() && (GetWearFlag() & WEARABLE_CHARM))
		return WEAR_CHARM;
#endif

#ifdef ENABLE_EXTENDED_PET_SYSTEM
	if (IsPetItem())
		return WEAR_PET;
#endif

	return -1;
}

void CItem::ModifyPoints(bool bAdd)
{
	int accessoryGrade;

	// 무기와 갑옷만 소켓을 적용시킨다.
	if (false == IsAccessoryForSocket())
	{
#ifdef ENABLE_QUIVER_SYSTEM
		if ((m_pProto->bType == ITEM_WEAPON || m_pProto->bType == ITEM_ARMOR) && m_pProto->bSubType != WEAPON_QUIVER)
#else
		if (m_pProto->bType == ITEM_WEAPON || m_pProto->bType == ITEM_ARMOR)
#endif
		{
#ifdef ENABLE_TIME_WEAPON_POINT_FIX
			for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
			{
				if (LIMIT_REAL_TIME == m_pProto->aLimits[i].bType)
					continue;
			}
#endif

			// 소켓이 속성강화에 사용되는 경우 적용하지 않는다 (ARMOR_WRIST ARMOR_NECK ARMOR_EAR)
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				if (IsRealTimeItem() == true)
					continue;

				DWORD dwVnum;

				if ((dwVnum = GetSocket(i)) <= 3)
					continue;

				TItemTable * p = ITEM_MANAGER::instance().GetTable(dwVnum);

				if (!p)
				{
					sys_err("cannot find table by vnum %u", dwVnum);
					continue;
				}

				if (ITEM_METIN == p->bType)
				{
					//m_pOwner->ApplyPoint(p->alValues[0], bAdd ? p->alValues[1] : -p->alValues[1]);
					for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
					{
						if (p->aApplies[i].bType == APPLY_NONE)
							continue;

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
						if (IsMountItem())
							continue;
#endif

						if (p->aApplies[i].bType == APPLY_SKILL)
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : p->aApplies[i].lValue ^ 0x00800000);
						else
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : -p->aApplies[i].lValue);
					}
				}
			}
		}

		accessoryGrade = 0;
	}
	else
	{
		accessoryGrade = MIN(GetAccessorySocketGrade(), ITEM_ACCESSORY_SOCKET_MAX_NUM);
	}

#ifdef ENABLE_ACCE_SYSTEM
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORBED_SOCKET)))
	{
		TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && (pkItemAbsorbed->bSubType == ARMOR_BODY))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lDefGrade <= 0)) || ((pkItemAbsorbed->alValues[5] > 0) && (lDefGrade < 1)))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;

				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);

				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long)dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}
			else if (pkItemAbsorbed->bType == ITEM_WEAPON)
			{
				long lAttGrade = pkItemAbsorbed->alValues[4] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[3] > pkItemAbsorbed->alValues[4])
					lAttGrade = pkItemAbsorbed->alValues[3] + pkItemAbsorbed->alValues[5];

				double dValue = lAttGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[3] > 0) && (lAttGrade < 1)) || ((pkItemAbsorbed->alValues[4] > 0) && (lAttGrade < 1)))
					lAttGrade += 1;
				else if ((pkItemAbsorbed->alValues[3] > 0) || (pkItemAbsorbed->alValues[4] > 0))
					lAttGrade += 1;

				m_pOwner->ApplyPoint(APPLY_ATT_GRADE_BONUS, bAdd ? lAttGrade : -lAttGrade);

				long lAttMagicGrade = pkItemAbsorbed->alValues[2] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[1] > pkItemAbsorbed->alValues[2])
					lAttMagicGrade = pkItemAbsorbed->alValues[1] + pkItemAbsorbed->alValues[5];

				dValue = lAttMagicGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttMagicGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lAttMagicGrade < 1)) || ((pkItemAbsorbed->alValues[2] > 0) && (lAttMagicGrade < 1)))
					lAttMagicGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[2] > 0))
					lAttMagicGrade += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_ATT_GRADE, bAdd ? lAttMagicGrade : -lAttMagicGrade);
			}
		}
	}
#endif

#ifdef ENABLE_AURA_SYSTEM
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA) && (GetSocket(AURA_ABSORBED_SOCKET)))
	{
		TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(AURA_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && ((pkItemAbsorbed->bSubType == ARMOR_SHIELD) || (pkItemAbsorbed->bSubType == ARMOR_HEAD)))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 1000;
				dValue = (double)dValue + .5;
				lDefGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lDefGrade <= 0)) || ((pkItemAbsorbed->alValues[5] > 0) && (lDefGrade < 1)))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;

				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);

				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 1000;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long)dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}
		}
	}
#endif

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
#ifdef ENABLE_ACCE_SYSTEM
	#ifdef ENABLE_AURA_SYSTEM
		#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
			if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH) && (GetSubType() != COSTUME_AURA) && (GetSubType() != COSTUME_MOUNT))
		#else
			if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH) && (GetSubType() != COSTUME_AURA))
		#endif
	#else
		#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
			if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH) && (GetSubType() != COSTUME_MOUNT))
		#else
			if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH))
		#endif
	#endif
#else
		if (m_pProto->aApplies[i].bType == APPLY_NONE)
#endif
			continue;

// #ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		// if (m_pProto->aApplies[i].bType == APPLY_MOUNT)
			// continue;
// #endif

		BYTE bType = m_pProto->aApplies[i].bType;
		long value = m_pProto->aApplies[i].lValue;
#ifdef ENABLE_ACCE_SYSTEM
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
		{
			TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;

				bType = pkItemAbsorbed->aApplies[i].bType;
				value = pkItemAbsorbed->aApplies[i].lValue;
				if (value < 0)
					continue;

				double dValue = value * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				value = (long)dValue;
				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (value <= 0))
					value += 1;
			}
			else
				continue;
		}
#endif
#ifdef ENABLE_AURA_SYSTEM
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA))
		{
			TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(AURA_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;

				bType = pkItemAbsorbed->aApplies[i].bType;
				value = pkItemAbsorbed->aApplies[i].lValue;
				if (value < 0)
					continue;

				double dValue = value * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 1000;
				dValue = (double)dValue + .5;
				value = (long)dValue;
				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (value <= 0))
					value += 1;
			}
			else
				continue;
		}
#endif
#ifdef ENABLE_MOUNT_CHANGELOOK_SYSTEM
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_MOUNT))
		{
			TItemTable * pkItemTransmutation = ITEM_MANAGER::instance().GetTable(GetTransmutation());
			if (pkItemTransmutation)
			{
				if (pkItemTransmutation->aApplies[i].bType == APPLY_NONE)
					continue;
				
				BYTE bType2 = pkItemTransmutation->aApplies[i].bType;
				long value2 = pkItemTransmutation->aApplies[i].lValue;
				if (value2 < 0)
					continue;
				if (m_pOwner)
					m_pOwner->ApplyPoint(bType2, bAdd ? value2 : -value2);
			}
		}
#endif

		if (bType != APPLY_SKILL && value != 0)
		{
			if (accessoryGrade != 0)
				value += MAX(accessoryGrade, value * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

			if (m_pOwner)
				m_pOwner->ApplyPoint(bType, bAdd ? value : -value);
		}
		else
			m_pOwner->ApplyPoint(bType, bAdd ? value : value ^ 0x00800000);
	}
	// 초승달의 반지, 할로윈 사탕, 행복의 반지, 영원한 사랑의 펜던트의 경우
	// 기존의 하드 코딩으로 강제로 속성을 부여했지만,
	// 그 부분을 제거하고 special item group 테이블에서 속성을 부여하도록 변경하였다.
	// 하지만 하드 코딩되어있을 때 생성된 아이템이 남아있을 수도 있어서 특수처리 해놓는다.
	// 이 아이템들의 경우, 밑에 ITEM_UNIQUE일 때의 처리로 속성이 부여되기 때문에,
	// 아이템에 박혀있는 attribute는 적용하지 않고 넘어간다.
	if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	{
		// Do not anything.
	}
	else
	{
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (GetAttributeType(i))
			{
				const TPlayerItemAttribute& ia = GetAttribute(i);
				long sValue = ia.sValue;
#ifdef ENABLE_ACCE_SYSTEM
				if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
				{
					double dValue = sValue * GetSocket(SASH_ABSORPTION_SOCKET);
					dValue = (double)dValue / 100;
					dValue = (double)dValue + .5;
					sValue = (long)dValue;
					if ((ia.sValue > 0) && (sValue <= 0))
						sValue += 1;
				}
#endif
#ifdef ENABLE_AURA_SYSTEM
				if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA))
				{
					double dValue = sValue * GetSocket(AURA_ABSORPTION_SOCKET);
					dValue = (double)dValue / 1000;
					dValue = (double)dValue + .5;
					sValue = (long)dValue;
					if ((ia.sValue > 0) && (sValue <= 0))
						sValue += 1;
				}
#endif

				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : sValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : -sValue);
			}
		}
	}

	switch (m_pProto->bType)
	{
	case ITEM_PICK:
	case ITEM_ROD:
	{
		if (bAdd)
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, GetVnum());
		}
		else
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
				m_pOwner->SetPart(PART_WEAPON, 0);
#else
				m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
#endif
		}
	}
	break;

	case ITEM_WEAPON:
	{
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		if (m_pOwner->GetWear(WEAR_COSTUME_WEAPON) != 0 && m_pOwner->GetQuestFlag("game_option.hide_costume_w") == 0)
			break;
#endif
		if (bAdd)
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
			{
#ifdef ENABLE_CHANGELOOK_SYSTEM
				DWORD dwRes = GetTransmutation() != 0 ? GetTransmutation() : GetVnum();
				m_pOwner->SetPart(PART_WEAPON, dwRes);
#else
				m_pOwner->SetPart(PART_WEAPON, GetVnum());
#endif
			}
		}
		else
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
				m_pOwner->SetPart(PART_WEAPON, 0);
#else
				m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
#endif
		}
	}
	break;
	case ITEM_ARMOR:
	{
		// 코스츔 body를 입고있다면 armor는 벗던 입던 상관 없이 비주얼에 영향을 주면 안 됨.
		if (!bAdd)
			m_pOwner->UpdatePacket();
		if (0 != m_pOwner->GetWear(WEAR_COSTUME_BODY) && m_pOwner->GetQuestFlag("game_option.hide_costume") == 0)
			break;

		if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD)
		{
			if (bAdd)
			{
				if (GetProto()->bSubType == ARMOR_BODY)
#ifdef ENABLE_CHANGELOOK_SYSTEM
				{
					DWORD dwRes = GetTransmutation() != 0 ? GetTransmutation() : GetVnum();
					m_pOwner->SetPart(PART_MAIN, dwRes);
				}
#else
					m_pOwner->SetPart(PART_MAIN, GetVnum());
#endif
			}
			else
			{
				if (GetProto()->bSubType == ARMOR_BODY)
					m_pOwner->SetPart(PART_MAIN, m_pOwner->GetOriginalPart(PART_MAIN));
			}
		}
	}
	break;

	// 코스츔 아이템 입었을 때 캐릭터 parts 정보 세팅. 기존 스타일대로 추가함..
	case ITEM_COSTUME:
	{
		DWORD toSetValue = this->GetVnum();
		EParts toSetPart = PART_MAX_NUM;
		if (GetSubType() == COSTUME_BODY)
		{
			if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume") == 1)
				break;

			toSetPart = PART_MAIN;
			if (false == bAdd)
			{
				const CItem* pArmor = m_pOwner->GetWear(WEAR_BODY);
				toSetValue = (nullptr != pArmor) ? pArmor->GetVnum() : m_pOwner->GetOriginalPart(PART_MAIN);
#ifdef ENABLE_CHANGELOOK_SYSTEM
				if (pArmor)
					toSetValue = pArmor->GetTransmutation() != 0 ? pArmor->GetTransmutation() : pArmor->GetVnum();
#endif
			}
#ifdef ENABLE_CHANGELOOK_SYSTEM
			else
				toSetValue = GetTransmutation() != 0 ? GetTransmutation() : GetVnum();
#endif
		}
		else if (GetSubType() == COSTUME_HAIR)
		{
			if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume_h") == 1)
				break;
			toSetPart = PART_HAIR;
			if (!bAdd)
				toSetValue = 0;
			else
			{
#ifdef ENABLE_CHANGELOOK_SYSTEM
				DWORD dwTransmutation = GetTransmutation();
				if (dwTransmutation != 0)
				{
					TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(dwTransmutation);
					toSetValue = (pItemTable != nullptr) ? pItemTable->alValues[3] : GetValue(3);
				}
				else
					toSetValue = GetValue(3);
#else
				toSetValue = GetValue(3);
#endif
			}
		}
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		else if (GetSubType() == COSTUME_WEAPON)
		{
			if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume_w") == 1)
				break;

			toSetPart = PART_WEAPON;
			if (!bAdd)
			{
				const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
				toSetValue = (nullptr != pWeapon) ? pWeapon->GetVnum() : m_pOwner->GetPart(PART_WEAPON);
#ifdef ENABLE_CHANGELOOK_SYSTEM
				if (pWeapon)
					toSetValue = pWeapon->GetTransmutation() != 0 ? pWeapon->GetTransmutation() : pWeapon->GetVnum();
#endif
			}
#ifdef ENABLE_CHANGELOOK_SYSTEM
			else
				toSetValue = GetTransmutation() != 0 ? GetTransmutation() : GetVnum();
#endif
		}
#endif
#ifdef ENABLE_ACCE_SYSTEM
		else if (GetSubType() == COSTUME_SASH)
		{
			if (features::IsFeatureEnabled(features::ACCE))
			{
				toSetValue -= 85000;
				if (GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_EFFECT_FROM_ABS)
					toSetValue += 2000;

				toSetValue = (bAdd == true) ? toSetValue : 0;
				toSetPart = PART_SASH;
			}
		}
#endif
#ifdef ENABLE_AURA_SYSTEM
		else if (GetSubType() == COSTUME_AURA)
		{
			if (features::IsFeatureEnabled(features::AURA))
			{
				toSetValue = (bAdd == true) ? toSetValue : 0;
				toSetPart = PART_AURA;
			}
		}
#endif
		if (PART_MAX_NUM != toSetPart)
		{
			m_pOwner->SetPart((BYTE)toSetPart, toSetValue);
			m_pOwner->UpdatePacket();
		}
	}
	break;
	case ITEM_UNIQUE:
	{
		if (0 != GetSIGVnum())
		{
			const CSpecialItemGroup* pItemGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(GetSIGVnum());
			if (nullptr == pItemGroup)
				break;
			DWORD dwAttrVnum = pItemGroup->GetAttrVnum(GetVnum());
			const CSpecialAttrGroup * pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(dwAttrVnum);
			if (nullptr == pAttrGroup)
				break;
			for (auto it = pAttrGroup->m_vecAttrs.begin(); it != pAttrGroup->m_vecAttrs.end(); it++)
			{
				m_pOwner->ApplyPoint(it->apply_type, bAdd ? it->apply_value : -it->apply_value);
			}
		}
	}
	break;
	}

#ifdef ENABLE_COSTUME_SET_SYSTEM
	if (features::IsFeatureEnabled(features::COSTUME_SET))
	{
		if (m_pOwner->IsHalloweenSetCount() >= 2 && m_pOwner->IsHalloweenSetCount() < 4)
		{
			if (m_pOwner->IsHalloweenSetStatus() == 0)
			{
				m_pOwner->ApplyPoint(APPLY_EXP_DOUBLE_BONUS, 20);
				m_pOwner->ApplyPoint(APPLY_ATTBONUS_MONSTER, 10);
				m_pOwner->UpdateHalloweenSetStatus(1);
			}
		}
		else if (m_pOwner->IsHalloweenSetCount() >= 4)
		{
			if (m_pOwner->IsHalloweenSetStatus() == 0)
			{
				m_pOwner->ApplyPoint(APPLY_EXP_DOUBLE_BONUS, 30);
				m_pOwner->ApplyPoint(APPLY_ATTBONUS_MONSTER, 15);
				m_pOwner->UpdateHalloweenSetStatus(2);
			}
		}
		else if (m_pOwner->IsHalloweenSetCount() < 2)
		{
			if (m_pOwner->IsHalloweenSetStatus() == 1)
			{
				m_pOwner->ApplyPoint(APPLY_EXP_DOUBLE_BONUS, -20);
				m_pOwner->ApplyPoint(APPLY_ATTBONUS_MONSTER, -10);
				m_pOwner->UpdateHalloweenSetStatus(0);
			}
			else if (m_pOwner->IsHalloweenSetStatus() == 2)
			{
				m_pOwner->ApplyPoint(APPLY_EXP_DOUBLE_BONUS, -30);
				m_pOwner->ApplyPoint(APPLY_ATTBONUS_MONSTER, -15);
				m_pOwner->UpdateHalloweenSetStatus(0);
			}
		}
	}
#endif
}

bool CItem::IsEquipable() const
{
	switch (this->GetType())
	{
	case ITEM_COSTUME:
	case ITEM_ARMOR:
	case ITEM_WEAPON:
	case ITEM_ROD:
	case ITEM_PICK:
	case ITEM_UNIQUE:
	case ITEM_DS:
	case ITEM_SPECIAL_DS:
	case ITEM_RING:
	case ITEM_BELT:
		return true;
	}

	return false;
}

// return false on error state
bool CItem::EquipTo(LPCHARACTER ch, UINT bWearCell)
{
	if (!ch)
	{
		sys_err("EquipTo: nil character");
		return false;
	}

	// 용혼석 슬롯 index는 WEAR_MAX_NUM 보다 큼.
	if (IsDragonSoul())
	{
		if (bWearCell < WEAR_MAX_NUM || bWearCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
		{
			sys_err("EquipTo: invalid dragon soul cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetSubType(), bWearCell - WEAR_MAX_NUM);
			return false;
		}
	}
	else
	{
		if (bWearCell >= WEAR_MAX_NUM)
		{
			sys_err("EquipTo: invalid wear cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetWearFlag(), bWearCell);
			return false;
		}
	}

	if (ch->GetWear(bWearCell))
	{
		sys_err("EquipTo: item already exist (this: #%d %s cell: %d %s)", GetOriginalVnum(), GetName(), bWearCell, ch->GetWear(bWearCell)->GetName());
		return false;
	}

	if (GetOwner())
		RemoveFromCharacter();

	ch->SetWear(bWearCell, this); // 여기서 패킷 나감

	m_pOwner = ch;
	m_bEquipped = true;
	m_wCell = INVENTORY_MAX_NUM + bWearCell;

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i) {
		if (m_pOwner->GetWear(i)) {
			//SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);

			//28/03/14 - Think - Fix immune bug!
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->GetRealImmuneFlag());
			//End of Sersemlik  Bug Fix -
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	if (IsDragonSoul())
	{
		DSManager::instance().ActivateDragonSoul(this);
	}
	else
	{
		ModifyPoints(true);
		StartUniqueExpireEvent();
		if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
			StartTimerBasedOnWearExpireEvent();

		// ACCESSORY_REFINE
		StartAccessorySocketExpireEvent();
		// END_OF_ACCESSORY_REFINE
	}

#ifdef ENABLE_EXTENDED_PET_SYSTEM
	if (IsPetItem())
	{
		CPetSystem* petSystem = ch->GetPetSystem();
		if (petSystem)
			petSystem->Summon(GetValue(0), this, 0, false);
	}
#endif

	ch->BuffOnAttr_AddBuffsFromItem(this);

	m_pOwner->ComputeBattlePoints();

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (IsMountItem())
		ch->MountSummon(this);
#endif

	m_pOwner->UpdatePacket();

	Save();

	return (true);
}

bool CItem::Unequip()
{
	if (!m_pOwner || GetCell() < INVENTORY_MAX_NUM)
	{
		// ITEM_OWNER_INVALID_PTR_BUG
		sys_err("%s %u m_pOwner %p, GetCell %d",
			GetName(), GetID(), get_pointer(m_pOwner), GetCell());
		// END_OF_ITEM_OWNER_INVALID_PTR_BUG
		return false;
	}

	if (this != m_pOwner->GetWear(GetCell() - INVENTORY_MAX_NUM))
	{
		sys_err("m_pOwner->GetWear() != this");
		return false;
	}

#ifdef ENABLE_EXTENDED_PET_SYSTEM
	if (IsPetItem())
	{
		CPetSystem* petSystem = m_pOwner->GetPetSystem();
		if (petSystem)
			petSystem->Unsummon(GetValue(0));
	}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (IsMountItem())
		m_pOwner->MountUnsummon(this);
#endif

	//신규 말 아이템 제거시 처리
	if (IsRideItem())
		ClearMountAttributeAndAffect();

	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
	}
	else
	{
		ModifyPoints(false);
	}

	StopUniqueExpireEvent();

	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		StopTimerBasedOnWearExpireEvent();

	// ACCESSORY_REFINE
	StopAccessorySocketExpireEvent();
	// END_OF_ACCESSORY_REFINE

	m_pOwner->BuffOnAttr_RemoveBuffsFromItem(this);

	m_pOwner->SetWear(GetCell() - INVENTORY_MAX_NUM, nullptr);

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i) {
		if (m_pOwner->GetWear(i)) {
			//SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);

			//28/03/14 - Think - Fix immune bug!
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->GetRealImmuneFlag());
			//End of fix immune bug
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	m_pOwner->ComputeBattlePoints();

	m_pOwner->UpdatePacket();

	m_pOwner = nullptr;
	m_wCell = 0;
	m_bEquipped = false;

	return true;
}

long CItem::GetValue(DWORD idx)
{
	assert(idx < ITEM_VALUES_MAX_NUM);
	return GetProto()->alValues[idx];
}

void CItem::SetExchanging(bool bOn)
{
	m_bExchanging = bOn;
}

void CItem::Save()
{
	if (m_bSkipSave)
		return;

	ITEM_MANAGER::instance().DelayedSave(this);
}

bool CItem::CreateSocket(BYTE bSlot, BYTE bGold)
{
	assert(bSlot < ITEM_SOCKET_MAX_NUM);

	if (m_alSockets[bSlot] != 0)
	{
		sys_err("Item::CreateSocket : socket already exist %s %d", GetName(), bSlot);
		return false;
	}

	if (bGold)
		m_alSockets[bSlot] = 2;
	else
		m_alSockets[bSlot] = 1;

	UpdatePacket();

	Save();
	return true;
}

void CItem::SetSockets(const long* c_al)
{
	thecore_memcpy(m_alSockets, c_al, sizeof(m_alSockets));
	Save();
}

void CItem::SetSocket(int i, long v, bool bLog)
{
	assert(i < ITEM_SOCKET_MAX_NUM);
	m_alSockets[i] = v;
	UpdatePacket();
	Save();
	if (bLog)
		LogManager::instance().ItemLog(i, v, 0, GetID(), "SET_SOCKET", "", "", GetOriginalVnum());
}

int CItem::GetGold()
{
	if (IS_SET(GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD))
	{
		if (GetProto()->dwGold == 0)
			return GetCount();
		else
			return GetCount() / GetProto()->dwGold;
	}
	else
		return GetProto()->dwGold;
}

int CItem::GetShopBuyPrice()
{
	return GetProto()->dwShopBuyPrice;
}

bool CItem::IsOwnership(LPCHARACTER ch)
{
	if (!m_pkOwnershipEvent)
		return true;

	return m_dwOwnershipPID == ch->GetPlayerID() ? true : false;
}

EVENTFUNC(ownership_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("ownership_event> <Factor> nullptr pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	pkItem->SetOwnershipEvent(nullptr);

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = pkItem->GetVID();
	p.szName[0] = '\0';

	pkItem->PacketAround(&p, sizeof(p));
	return 0;
}

void CItem::SetOwnershipEvent(LPEVENT pkEvent)
{
	m_pkOwnershipEvent = pkEvent;
}

void CItem::SetOwnership(LPCHARACTER ch, int iSec)
{
	if (!ch)
	{
		if (m_pkOwnershipEvent)
		{
			event_cancel(&m_pkOwnershipEvent);
			m_dwOwnershipPID = 0;

			TPacketGCItemOwnership p;

			p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
			p.dwVID = m_dwVID;
			p.szName[0] = '\0';

			PacketAround(&p, sizeof(p));
		}
		return;
	}

	if (m_pkOwnershipEvent)
		return;

	if (iSec <= 10)
	{
		iSec = 30;
	}

	m_dwOwnershipPID = ch->GetPlayerID();

	item_event_info* info = AllocEventInfo<item_event_info>();
	strlcpy(info->szOwnerName, ch->GetName(), sizeof(info->szOwnerName));
	info->item = this;

	SetOwnershipEvent(event_create(ownership_event, info, PASSES_PER_SEC(iSec)));

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = m_dwVID;
	strlcpy(p.szName, ch->GetName(), sizeof(p.szName));

	PacketAround(&p, sizeof(p));
}

int CItem::GetSocketCount()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		if (GetSocket(i) == 0)
			return i;
	}
	return ITEM_SOCKET_MAX_NUM;
}

bool CItem::AddSocket()
{
	int count = GetSocketCount();
	if (count == ITEM_SOCKET_MAX_NUM)
		return false;
	m_alSockets[count] = 1;
	return true;
}

void CItem::AlterToSocketItem(int iSocketCount)
{
	if (iSocketCount >= ITEM_SOCKET_MAX_NUM)
	{
		sys_log(0, "Invalid Socket Count %d, set to maximum", ITEM_SOCKET_MAX_NUM);
		iSocketCount = ITEM_SOCKET_MAX_NUM;
	}

	for (int i = 0; i < iSocketCount; ++i)
		SetSocket(i, 1);
}

void CItem::AlterToMagicItem()
{
	int idx = GetAttributeSetIndex();

	if (idx < 0)
		return;

	int iSecondPct;
	int iThirdPct;

	switch (GetType())
	{
	case ITEM_WEAPON:
		iSecondPct = 20;
		iThirdPct = 5;
		break;

	case ITEM_UNIQUE:
	{
#ifdef ENABLE_TALISMAN_SYSTEM
		if (GetSubType() == USE_CHARM)
		{
			iSecondPct = 100;
			iThirdPct = 100;
		}
		else
		{
			iSecondPct = 0;
			iThirdPct = 0;
		}
#endif
	}
	break;

	case ITEM_ARMOR:
	case ITEM_COSTUME:
		if (GetSubType() == ARMOR_BODY)
		{
			iSecondPct = 10;
			iThirdPct = 2;
		}
		else
		{
			iSecondPct = 10;
			iThirdPct = 1;
		}
		break;

	default:
		return;
	}

	// 100% 확률로 좋은 속성 하나
	PutAttribute(aiItemMagicAttributePercentHigh);

	if (number(1, 100) <= iSecondPct)
		PutAttribute(aiItemMagicAttributePercentLow);

	if (number(1, 100) <= iThirdPct)
		PutAttribute(aiItemMagicAttributePercentLow);
}

DWORD CItem::GetRefineFromVnum()
{
	return ITEM_MANAGER::instance().GetRefineFromVnum(GetVnum());
}

int CItem::GetRefineLevel()
{
	const char* name = GetBaseName();
	char* p = const_cast<char*>(strrchr(name, '+'));

	if (!p)
		return 0;

	int	rtn = 0;
	str_to_number(rtn, p + 1);

	const char* locale_name = GetName();
	p = const_cast<char*>(strrchr(locale_name, '+'));

	if (p)
	{
		int	locale_rtn = 0;
		str_to_number(locale_rtn, p + 1);
		if (locale_rtn != rtn)
		{
			sys_err("refine_level_based_on_NAME(%d) is not equal to refine_level_based_on_LOCALE_NAME(%d).", rtn, locale_rtn);
		}
	}

	return rtn;
}

bool CItem::IsPolymorphItem()
{
	return GetType() == ITEM_POLYMORPH;
}

EVENTFUNC(unique_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("unique_expire_event> <Factor> nullptr pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->IsCharmItem())
		return 0;

	if (pkItem->GetValue(2) == 0)
	{
		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= 1)
		{
			sys_log(0, "UNIQUE_ITEM: expire %s %u", pkItem->GetName(), pkItem->GetID());
			pkItem->SetUniqueExpireEvent(nullptr);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			pkItem->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - 1);
			return PASSES_PER_SEC(60);
		}
	}
	else
	{
		time_t cur = get_global_time();

		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= cur)
		{
			pkItem->SetUniqueExpireEvent(nullptr);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			// 게임 내에 시간제 아이템들이 빠릿빠릿하게 사라지지 않는 버그가 있어
			// 수정
			// by rtsummit
			if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur < 600)
				return PASSES_PER_SEC(pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur);
			else
				return PASSES_PER_SEC(600);
		}
	}
}

// 시간 후불제
// timer를 시작할 때에 시간 차감하는 것이 아니라,
// timer가 발화할 때에 timer가 동작한 시간 만큼 시간 차감을 한다.
EVENTFUNC(timer_based_on_wear_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("expire_event <Factor> nullptr pointer");
		return 0;
	}

	LPITEM pkItem = info->item;
	int remain_time = pkItem->GetSocket(ITEM_SOCKET_REMAIN_SEC) - processing_time / passes_per_sec;
	if (remain_time <= 0)
	{
		sys_log(0, "ITEM EXPIRED : expired %s %u", pkItem->GetName(), pkItem->GetID());
		pkItem->SetTimerBasedOnWearExpireEvent(nullptr);
		pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, 0);

		// 일단 timer based on wear 용혼석은 시간 다 되었다고 없애지 않는다.
		if (pkItem->IsDragonSoul())
		{
			DSManager::instance().DeactivateDragonSoul(pkItem);
		}
		else
		{
			ITEM_MANAGER::instance().RemoveItem(pkItem, "TIMER_BASED_ON_WEAR_EXPIRE");
		}
		return 0;
	}
	pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	return PASSES_PER_SEC(MIN(60, remain_time));
}

void CItem::SetUniqueExpireEvent(LPEVENT pkEvent)
{
	m_pkUniqueExpireEvent = pkEvent;
}

void CItem::SetTimerBasedOnWearExpireEvent(LPEVENT pkEvent)
{
	m_pkTimerBasedOnWearExpireEvent = pkEvent;
}

EVENTFUNC(real_time_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);

	if (nullptr == info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (nullptr == item)
		return 0;

	const time_t current = get_global_time();

	if (current > item->GetSocket(0))
	{
		switch (item->GetVnum())
		{
			if (item->IsNewMountItem())
			{
				if (item->GetSocket(2) != 0)
					item->ClearMountAttributeAndAffect();
			}
			break;
		}

		if (item->GetType() == ITEM_WEAPON)//kostum suresi bitme fix
		{
			if (item->GetOwner() && item->GetOwner()->GetWear(WEAR_COSTUME_WEAPON) != nullptr)
				item->GetOwner()->UnequipItem(item->GetOwner()->GetWear(WEAR_COSTUME_WEAPON));
		}

		ITEM_MANAGER::instance().RemoveItem(item, "REAL_TIME_EXPIRE");

		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartRealTimeExpireEvent()
{
	if (m_pkRealTimeExpireEvent)
		return;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType || LIMIT_REAL_TIME_START_FIRST_USE == GetProto()->aLimits[i].bType)
		{
			item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
			info->item_vid = GetVID();

			m_pkRealTimeExpireEvent = event_create(real_time_expire_event, info, PASSES_PER_SEC(1));

			sys_log(0, "REAL_TIME_EXPIRE: StartRealTimeExpireEvent");

			return;
		}
	}
}

bool CItem::IsRealTimeItem()
{
	if (!GetProto())
		return false;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return true;
	}
	return false;
}

void CItem::StartUniqueExpireEvent()
{
	if (GetType() != ITEM_UNIQUE)
		return;

	if (m_pkUniqueExpireEvent)
		return;

	//기간제 아이템일 경우 시간제 아이템은 동작하지 않는다
	if (IsRealTimeItem())
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(false);

	int iSec = GetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME);

	if (iSec == 0)
		iSec = 60;
	else
		iSec = MIN(iSec, 60);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, 0);

	item_event_info * info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetUniqueExpireEvent(event_create(unique_expire_event, info, PASSES_PER_SEC(iSec)));
}

// 시간 후불제
// timer_based_on_wear_expire_event 설명 참조
void CItem::StartTimerBasedOnWearExpireEvent()
{
	if (m_pkTimerBasedOnWearExpireEvent)
		return;

	//기간제 아이템일 경우 시간제 아이템은 동작하지 않는다
	if (IsRealTimeItem())
		return;

	if (-1 == GetProto()->cLimitTimerBasedOnWearIndex)
		return;

	int iSec = GetSocket(0);

	// 남은 시간을 분단위로 끊기 위해...
	if (0 != iSec)
	{
		iSec %= 60;
		if (0 == iSec)
			iSec = 60;
	}

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetTimerBasedOnWearExpireEvent(event_create(timer_based_on_wear_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopUniqueExpireEvent()
{
	if (!m_pkUniqueExpireEvent)
		return;

	if (GetValue(2) != 0) // 게임시간제 이외의 아이템은 UniqueExpireEvent를 중단할 수 없다.
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(true);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, event_time(m_pkUniqueExpireEvent) / passes_per_sec);
	event_cancel(&m_pkUniqueExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::StopTimerBasedOnWearExpireEvent()
{
	if (!m_pkTimerBasedOnWearExpireEvent)
		return;

	int remain_time = GetSocket(ITEM_SOCKET_REMAIN_SEC) - event_processing_time(m_pkTimerBasedOnWearExpireEvent) / passes_per_sec;

	SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::ApplyAddon(int iAddonType)
{
	CItemAddonManager::instance().ApplyAddonTo(iAddonType, this);
}

int CItem::GetSpecialGroup() const
{
	return ITEM_MANAGER::instance().GetSpecialGroupFromItem(GetVnum());
}

//
// 악세서리 소켓 처리.
//
bool CItem::IsAccessoryForSocket()
{
	return (m_pProto->bType == ITEM_ARMOR && (m_pProto->bSubType == ARMOR_WRIST || m_pProto->bSubType == ARMOR_NECK || m_pProto->bSubType == ARMOR_EAR)) ||
		(m_pProto->bType == ITEM_BELT);				// 2013년 2월 새로 추가된 '벨트' 아이템의 경우 기획팀에서 악세서리 소켓 시스템을 그대로 이용하자고 함.
}

#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
void CItem::SetAccessorySocketGrade(int iGrade, bool is_perma) 
{ 
	SetSocket(0, MINMAX(0, iGrade, GetAccessorySocketMaxGrade())); 

	int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

	if (is_perma)
		SetAccessorySocketDownGradeTime(0);
	else
		SetAccessorySocketDownGradeTime(iDownTime);
}
#else
void CItem::SetAccessorySocketGrade(int iGrade)
{
	SetSocket(0, MINMAX(0, iGrade, GetAccessorySocketMaxGrade()));

	int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

	//if (test_server)
	//	iDownTime /= 60;

	SetAccessorySocketDownGradeTime(iDownTime);
}
#endif

void CItem::SetAccessorySocketMaxGrade(int iMaxGrade)
{
	SetSocket(1, MINMAX(0, iMaxGrade, ITEM_ACCESSORY_SOCKET_MAX_NUM));
}

void CItem::SetAccessorySocketDownGradeTime(DWORD time)
{
#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
	if (GetSocket(2) != 31)
#endif
		SetSocket(2, time);

	if (test_server && GetOwner())
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s에서 소켓 빠질때까지 남은 시간 %d"), GetName(), time);
}

EVENTFUNC(accessory_socket_expire_event)
{
	item_vid_event_info* info = dynamic_cast<item_vid_event_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("accessory_socket_expire_event> <Factor> nullptr pointer");
		return 0;
	}

	LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (item->GetAccessorySocketDownGradeTime() <= 1)
	{
	degrade:
		item->SetAccessorySocketExpireEvent(nullptr);
		item->AccessorySocketDegrade();
		return 0;
	}
	else
	{
		int iTime = item->GetAccessorySocketDownGradeTime() - 60;

		if (iTime <= 1)
			goto degrade;

		item->SetAccessorySocketDownGradeTime(iTime);

		if (iTime > 60)
			return PASSES_PER_SEC(60);
		else
			return PASSES_PER_SEC(iTime);
	}
}

void CItem::StartAccessorySocketExpireEvent()
{
	if (!IsAccessoryForSocket())
		return;

	if (m_pkAccessorySocketExpireEvent)
		return;

	if (GetAccessorySocketMaxGrade() == 0)
		return;

	if (GetAccessorySocketGrade() == 0)
		return;

	int iSec = GetAccessorySocketDownGradeTime();
	SetAccessorySocketExpireEvent(nullptr);

	if (iSec <= 1)
		iSec = 5;
	else
		iSec = MIN(iSec, 60);

	item_vid_event_info * info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	SetAccessorySocketExpireEvent(event_create(accessory_socket_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopAccessorySocketExpireEvent()
{
	if (!m_pkAccessorySocketExpireEvent)
		return;

	if (!IsAccessoryForSocket())
		return;

	int new_time = GetAccessorySocketDownGradeTime() - (60 - event_time(m_pkAccessorySocketExpireEvent) / passes_per_sec);

	event_cancel(&m_pkAccessorySocketExpireEvent);

	if (new_time <= 1)
	{
		AccessorySocketDegrade();
	}
	else
	{
		SetAccessorySocketDownGradeTime(new_time);
	}
}

bool CItem::IsRideItem()
{
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_RIDE == GetSubType())
		return true;
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == GetSubType())
		return true;
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (ITEM_COSTUME == GetType() && COSTUME_MOUNT == GetSubType())
		return true;
#endif
	return false;
}

// bool CItem::IsMountItem()
// {
	// if (GetType() == ITEM_UNIQUE && GetSubType() == USE_MOUNT)
		// return true;

	// if (GetVnum() == 71199)
		// return false;

	// if (GetVnum() >= 71182 and GetVnum() <= 71227)
		// return true;

	// return false;
// }

#ifdef ENABLE_TALISMAN_SYSTEM
bool CItem::IsCharmItem()
{
	if (GetType() == ITEM_UNIQUE && GetSubType() == USE_CHARM)
		return true;

	return false;
}
#endif

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
bool CItem::IsMountItem()
{
	if (GetType() == ITEM_COSTUME && GetSubType() == COSTUME_MOUNT)
		return true;
	return false;
}
#endif

bool CItem::IsRamadanRing()
{
	if (GetVnum() == UNIQUE_ITEM_RAMADAN_RING)
		return true;
	return false;
}

void CItem::ClearMountAttributeAndAffect()
{
	LPCHARACTER ch = GetOwner();

	ch->RemoveAffect(AFFECT_MOUNT);
	ch->RemoveAffect(AFFECT_MOUNT_BONUS);

	ch->MountVnum(0);

	ch->PointChange(POINT_ST, 0);
	ch->PointChange(POINT_DX, 0);
	ch->PointChange(POINT_HT, 0);
	ch->PointChange(POINT_IQ, 0);
}

// fixme
// 이거 지금은 안쓴데... 근데 혹시나 싶어서 남겨둠.
// by rtsummit
bool CItem::IsNewMountItem()
{
	switch (GetVnum())
	{
	case 76000: case 76001: case 76002: case 76003:
	case 76004: case 76005: case 76006: case 76007:
	case 76008: case 76009: case 76010: case 76011:
	case 76012: case 76013: case 76014:
		return true;
	}
	return false;
}

void CItem::SetAccessorySocketExpireEvent(LPEVENT pkEvent)
{
	m_pkAccessorySocketExpireEvent = pkEvent;
}

void CItem::AccessorySocketDegrade()
{
	if (GetAccessorySocketGrade() > 0)
	{
		LPCHARACTER ch = GetOwner();

#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
		if (GetSocket(2) == 31)
			return;
#endif

		if (ch)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s에 박혀있던 보석이 사라집니다."), GetName());
		}

		ModifyPoints(false);
		SetAccessorySocketGrade(GetAccessorySocketGrade() - 1);
		ModifyPoints(true);

		int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

		if (test_server)
			iDownTime /= 60;

#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
		if (GetSocket(2) != 31)
#endif
			SetAccessorySocketDownGradeTime(iDownTime);

		if (iDownTime)
			StartAccessorySocketExpireEvent();
	}
}

// ring에 item을 박을 수 있는지 여부를 체크해서 리턴
static bool CanPutIntoRing(LPITEM ring, LPITEM item)
{
	return false;
}

bool CItem::CanPutInto(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if (item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

	else if (item->GetType() != ITEM_ARMOR)
		return false;

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
	};
	const static JewelAccessoryInfo infos[] = {
#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
		{ 50641, 14220, 16220, 17220 }, 
		{ 50642, 24500, 19500, 18500 },
		{ 50642, 14500, 16500, 17500 }, 
		{ 50643, 14520, 16520, 17520 }, 
		{ 50643, 24520, 19520, 18520 },
		{ 50644, 14540, 16540, 17540 }, 
		{ 50644, 24540, 19540, 18540 },
		{ 50645, 14560, 16560, 17560 }, 
		{ 50645, 24560, 19560, 18560 },
		{ 50646, 24570, 19570, 18570 },
		{ 50646, 14570, 16570, 17570 }, 
		{ 50647, 14230, 16230, 17230 }, 
#else
		{ 50634, 14420, 16220, 17220 },
		{ 50635, 14500, 16500, 17500 },
		{ 50636, 14520, 16520, 17520 },
		{ 50637, 14540, 16540, 17540 },
		{ 50638, 14560, 16560, 17560 },
		{ 50639, 14570, 16570, 17570 },
#endif
	};

	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (unsigned int i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch (item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
	if (item->GetSubType() == ARMOR_WRIST)
		vnum -= 14000;
	else if (item->GetSubType() == ARMOR_NECK)
		vnum -= 16000;
	else if (item->GetSubType() == ARMOR_EAR)
		vnum -= 17000;
	else
		return false;

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16230 && item->GetVnum() <= 16239)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}

	return 50623 + type == GetVnum();
}

#ifdef ENABLE_PERMA_ACCESSORY_SYSTEM
bool CItem::CanPutIntoNew(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if(item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

	else if (item->GetType() != ITEM_ARMOR)
		return false;

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
	};
	const static JewelAccessoryInfo infos[] = { 
		{ 50634, 14220, 16220, 17220 }, 
		{ 50635, 14500, 16500, 17500 }, 
		{ 50636, 14520, 16520, 17520 }, 
		{ 50637, 14540, 16540, 17540 }, 
		{ 50638, 14560, 16560, 17560 }, 
		{ 50639, 14570, 16570, 17570 }, 
		{ 50640, 14230, 16230, 17230 }, 
		{ 50635, 24500, 19500, 18500 },
		{ 50636, 24520, 19520, 18520 },
		{ 50637, 24540, 19540, 18540 },
		{ 50638, 24560, 19560, 18560 },
		{ 50639, 24570, 19570, 18570 },
	};
	
	
	
	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (unsigned int i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch(item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
	if (item->GetSubType() == ARMOR_WRIST)
		vnum -= 14000;
	else if (item->GetSubType() == ARMOR_NECK)
		vnum -= 16000;
	else if (item->GetSubType() == ARMOR_EAR)
		vnum -= 17000;
	else
		return false;

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16230 && item->GetVnum() <= 16239)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}

	return 50623 + type == GetVnum();
}
#endif

bool CItem::CheckItemUseLevel(int nLevel)
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			if (this->m_pProto->aLimits[i].lValue > nLevel) return false;
			else return true;
		}
	}
	return true;
}

long CItem::FindApplyValue(BYTE bApplyType)
{
	if (m_pProto == nullptr)
		return 0;

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == bApplyType)
			return m_pProto->aApplies[i].lValue;
	}

	return 0;
}

void CItem::CopySocketTo(LPITEM pItem)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		pItem->m_alSockets[i] = m_alSockets[i];
	}
}

int CItem::GetAccessorySocketGrade()
{
	return MINMAX(0, GetSocket(0), GetAccessorySocketMaxGrade());
}

int CItem::GetAccessorySocketMaxGrade()
{
	return MINMAX(0, GetSocket(1), ITEM_ACCESSORY_SOCKET_MAX_NUM);
}

int CItem::GetAccessorySocketDownGradeTime()
{
	return MINMAX(0, GetSocket(2), aiAccessorySocketDegradeTime[GetAccessorySocketGrade()]);
}

void CItem::AttrLog()
{
	const char* pszIP = nullptr;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (m_alSockets[i])
		{
			LogManager::instance().ItemLog(i, m_alSockets[i], 0, GetID(), "INFO_SOCKET", "", pszIP ? pszIP : "", GetOriginalVnum());
		}
	}

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		int	type = m_aAttr[i].bType;
		int value = m_aAttr[i].sValue;

		if (type)
			LogManager::instance().ItemLog(i, type, value, GetID(), "INFO_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum());
	}
}

int CItem::GetLevelLimit()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			return this->m_pProto->aLimits[i].lValue;
		}
	}
	return 0;
}

bool CItem::OnAfterCreatedItem()
{
	// 아이템을 한 번이라도 사용했다면, 그 이후엔 사용 중이지 않아도 시간이 차감되는 방식
	if (-1 != this->GetProto()->cLimitRealTimeFirstUseIndex)
	{
		// Socket1에 아이템의 사용 횟수가 기록되어 있으니, 한 번이라도 사용한 아이템은 타이머를 시작한다.
		if (0 != GetSocket(1))
		{
			StartRealTimeExpireEvent();
		}
	}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	if (GetSealBindTime() > 0)
		StartUnSealBindTimerExpireEvent();
#endif

	return true;
}

bool CItem::IsDragonSoul()
{
	return GetType() == ITEM_DS;
}

int CItem::GiveMoreTime_Per(float fPercent)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		DWORD given_time = fPercent * duration / 100u;
		if (remain_sec == duration)
			return false;
		if ((given_time + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, given_time + remain_sec);
			return given_time;
		}
	}
	// 우선 용혼석에 관해서만 하도록 한다.
	else
		return 0;
}

int CItem::GiveMoreTime_Fix(DWORD dwTime)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		if (remain_sec == duration)
			return false;
		if ((dwTime + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, dwTime + remain_sec);
			return dwTime;
		}
	}
	// 우선 용혼석에 관해서만 하도록 한다.
	else
		return 0;
}

int	CItem::GetDuration()
{
	if (!GetProto())
		return -1;

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return GetProto()->aLimits[i].lValue;
	}

	if (GetProto()->cLimitTimerBasedOnWearIndex >= 0)
	{
		BYTE cLTBOWI = GetProto()->cLimitTimerBasedOnWearIndex;
		return GetProto()->aLimits[cLTBOWI].lValue;
	}
	return -1;
}

bool CItem::IsSameSpecialGroup(const LPITEM item) const
{
	// 서로 VNUM이 같다면 같은 그룹인 것으로 간주
	if (this->GetVnum() == item->GetVnum())
		return true;

	if (GetSpecialGroup() && (item->GetSpecialGroup() == GetSpecialGroup()))
		return true;

	return false;
}

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
bool CItem::IsSkillBook()
{
	std::vector<int> skillBookVector = ITEM_MANAGER::instance().GetSkillBookItemTableVector();
	if (skillBookVector.empty())
	{
		if (GetType() == ITEM_SKILLBOOK)
			return true;
		
		return false;
	}
	
	if (GetType() == ITEM_SKILLBOOK)
		return true;
	
	return std::find(skillBookVector.begin(), skillBookVector.end(), GetVnum()) != skillBookVector.end();
}

bool CItem::IsStone()
{
	std::vector<int> stoneVector = ITEM_MANAGER::instance().GetStoneItemTableVector();
	if (stoneVector.empty())
	{
		if (GetType() == ITEM_METIN)
			return true;

		return false;
	}
	
	if (GetType() == ITEM_METIN)
		return true;
	
	return std::find(stoneVector.begin(), stoneVector.end(), GetVnum()) != stoneVector.end();
}

bool CItem::IsUpgradeItem()
{
	std::vector<int> upgradeVector = ITEM_MANAGER::instance().GetUpgradeItemTableVector();
	if (upgradeVector.empty())
	{
		if (GetType() == ITEM_MATERIAL)
			return true;

		return false;
	}
	
	return std::find(upgradeVector.begin(), upgradeVector.end(), GetVnum()) != upgradeVector.end();
}

bool CItem::IsAttr()
{
	std::vector<int> attrVector = ITEM_MANAGER::instance().GetAttrItemTableVector();
	if (attrVector.empty())
	{
		if (GetVnum() == 71184)
			return true;

		return false;
	}
	
	return std::find(attrVector.begin(), attrVector.end(), GetVnum()) != attrVector.end();
}

bool CItem::IsFlower()
{
	std::vector<int> flowerVector = ITEM_MANAGER::instance().GetFlowerItemTableVector();
	if (flowerVector.empty())
	{
		switch (GetVnum())
		{
		case 50701:
		case 50702:
		case 50703:
		case 50704:
		case 50705:
		case 50706:
		case 50707:
		case 50708:
		case 50721:
		case 50722:
		case 50723:
		case 50724:
		case 50725:
		case 50726:
		case 50727:
		case 50728:
			return true;
		}

		return false;
	}
	
	return std::find(flowerVector.begin(), flowerVector.end(), GetVnum()) != flowerVector.end();
}

bool CItem::IsChest()
{
	std::vector<int> chestVector = ITEM_MANAGER::instance().GetChestItemTableVector();
	if (chestVector.empty())
	{
		if (GetType() == ITEM_GIFTBOX)
			return true;
		
		if (GetType() == ITEM_GACHA)
			return true;

		return false;
	}
	
	if (GetType() == ITEM_GIFTBOX)
		return true;
	
	if (GetType() == ITEM_GACHA)
		return true;
	
	return std::find(chestVector.begin(), chestVector.end(), GetVnum()) != chestVector.end();
}
#endif

//28/03/14 - Think - Fix immune bug
DWORD CItem::GetRealImmuneFlag()
{
	DWORD dwImmuneFlag = m_pProto->dwImmuneFlag;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		if (GetAttributeType(i))
		{
			const TPlayerItemAttribute& ia = GetAttribute(i);

			if (ia.bType == APPLY_IMMUNE_STUN && !IS_SET(dwImmuneFlag, IMMUNE_STUN))
				SET_BIT(dwImmuneFlag, IMMUNE_STUN);
			else if (ia.bType == APPLY_IMMUNE_FALL && !IS_SET(dwImmuneFlag, IMMUNE_FALL))
				SET_BIT(dwImmuneFlag, IMMUNE_FALL);
			else if (ia.bType == APPLY_IMMUNE_SLOW && !IS_SET(dwImmuneFlag, IMMUNE_SLOW))
				SET_BIT(dwImmuneFlag, IMMUNE_SLOW);
		}
	}

	return dwImmuneFlag;
}
//End of fix immune bug

std::string CItem::GetHyperlink()
{
	char itemlink[256];
	std::string attrString = "";
	bool hasAttribute = false;

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		//Reuse buffer from start every time, we are storing at attrString
		snprintf(itemlink, sizeof(itemlink), ":%x:%d",
			GetAttributeType(i), GetAttributeValue(i));

		attrString += itemlink;
		hasAttribute = true;
	}

	std::string color = hasAttribute ? "cffffc700" : "cfff1e6c0";
	snprintf(itemlink, sizeof(itemlink), "|%s|Hitem:%x:%lx:%ld:%ld:%ld:%ld:%d:%d:%s|h[%s]|h|r",
		color.c_str(),
		GetVnum(), GetFlag(),
		GetSocket(0), GetSocket(1), GetSocket(2), GetSocket(3), GetTransmutation(), GetCount(), attrString.c_str(),
		GetName());

	return itemlink;
}

const char* CItem::GetName()
{
	static char szItemName[128];
	memset(szItemName, 0, sizeof(szItemName));
	if (GetProto())
	{
		int len = 0;
		switch (GetType())
		{
		case ITEM_POLYMORPH:
		{
			const DWORD dwMobVnum = GetSocket(0);
			const CMob* pMob = CMobManager::instance().Get(dwMobVnum);
			if (pMob)
				len = snprintf(szItemName, sizeof(szItemName), "%s", pMob->m_table.szLocaleName);

			break;
		}
		case ITEM_SKILLBOOK:
		case ITEM_SKILLFORGET:
		{
			const DWORD dwSkillVnum = (GetVnum() == ITEM_SKILLBOOK_VNUM || GetVnum() == ITEM_SKILLFORGET_VNUM) ? GetSocket(0) : 0;
			const CSkillProto * pSkill = (dwSkillVnum != 0) ? CSkillManager::instance().Get(dwSkillVnum) : nullptr;
			if (pSkill)
				len = snprintf(szItemName, sizeof(szItemName), "%s", pSkill->szName);

			break;
		}
		}
		len += snprintf(szItemName + len, sizeof(szItemName) - len, (len > 0) ? " %s" : "%s", GetProto()->szLocaleName);
	}

	return szItemName;
}

bool CItem::IsQuestHair()
{
	switch (GetVnum())
	{
	case 74001:
	case 74002:
	case 74003:
	case 74004:
	case 74005:
	case 74006:
	case 74007:
	case 74008:
	case 74009:
	case 74010:
	case 74011:
	case 74012:
	case 74251:
	case 74252:
	case 74253:
	case 74254:
	case 74255:
	case 74256:
	case 74257:
	case 74258:
	case 74259:
	case 74260:
	case 74261:
	case 74262:
	case 74501:
	case 74502:
	case 74503:
	case 74504:
	case 74505:
	case 74506:
	case 74507:
	case 74508:
	case 74509:
	case 74510:
	case 74511:
	case 74512:
	case 74521:
	case 74522:
	case 74523:
	case 74524:
	case 74525:
	case 74526:
	case 74527:
	case 74528:
	case 74529:
	case 74530:
	case 74531:
	case 74532:
	case 74533:
	case 74534:
	case 74535:
	case 74536:
	case 74537:
	case 74538:
	case 74539:
	case 74540:
	case 74541:
	case 74542:
	case 74543:
	case 74544:
	case 74545:
	case 74546:
	case 74547:
	case 74548:
	case 74549:
	case 74550:
	case 74551:
	case 74552:
	case 74553:
	case 74554:
	case 74555:
	case 74556:
	case 74557:
	case 74558:
	case 74559:
	case 74751:
	case 74752:
	case 74753:
	case 74754:
	case 74755:
	case 74756:
	case 74757:
	case 74758:
	case 74759:
	case 74760:
	case 74761:
	case 74762:
	case 75001:
	case 75002:
	case 75003:
	case 75004:
	case 75005:
	case 75006:
	case 75007:
	case 75008:
	case 75009:
	case 75010:
	case 75011:
	case 75012:
	case 75201:
	case 75202:
	case 75203:
	case 75204:
	case 75205:
	case 75206:
	case 75207:
	case 75208:
	case 75209:
	case 75210:
	case 75211:
	case 75212:
	case 75401:
	case 75402:
	case 75403:
	case 75404:
	case 75405:
	case 75406:
	case 75407:
	case 75408:
	case 75409:
	case 75410:
	case 75411:
	case 75412:
	case 75601:
	case 75602:
	case 75603:
	case 75604:
	case 75605:
	case 75606:
	case 75607:
	case 75608:
	case 75609:
	case 75610:
	case 75611:
	case 75612:
		return true;
	}

	return false;
}

void CItem::SetBasic(bool b)
{
	is_basic = b;
	UpdatePacket();
}

#ifdef ENABLE_EXTENDED_PET_SYSTEM
bool CItem::IsPetItem()
{
	if (GetType() == ITEM_UNIQUE && GetSubType() == USE_PET)
		return true;

	return false;
}
#endif

bool CItem::IsAutoPotionHP()
{
	switch (GetVnum())
	{
	case 39037:
	case 39038:
	case 39039:
	case 72723:
	case 72724:
	case 72725:
	case 72726:
	case 76021:
	case 76022:
		return true;
	}

	return false;
}

bool CItem::IsAutoPotionSP()
{
	switch (GetVnum())
	{
	case 39040:
	case 39041:
	case 39042:
	case 72727:
	case 72728:
	case 72729:
	case 72730:
	case 76004:
	case 76005:
		return true;
	}

	return false;
}

#ifdef ENABLE_COSTUME_SET_SYSTEM
bool CItem::IsSetHalloweenCostume()
{
	switch (GetVnum())
	{
	case 41670:
	case 41671:
	case 41672:
	case 41673:
	case 41674:
	case 41675:
	case 41676:
	case 41677:
		return true;
	}

	return false;
}

bool CItem::IsSetHalloweenHair()
{
	switch (GetVnum())
	{
	case 45336:
	case 45337:
	case 45338:
	case 45339:
	case 45340:
	case 45341:
	case 45342:
	case 45343:
		return true;
	}

	return false;
}

bool CItem::IsSetHalloweenWeapon()
{
	switch (GetVnum())
	{
	case 40101:
	case 40102:
	case 40103:
	case 40104:
	case 40105:
	case 40106:
	case 40107:
	case 40108:
	case 40109:
	case 40110:
	case 40111:
	case 40112:
	case 40113:
	case 40114:
		return true;
	}

	return false;
}

bool CItem::IsSetHalloweenMount()
{
	switch (GetVnum())
	{
	case 71235:
	case 71236:
		return true;
	}

	return false;
}
#endif

bool CItem::IsPotion()
{
	switch (GetVnum())
	{
		case 27001:
		case 27002:
		case 27003:
		case 27004:
		case 27005:
		case 27006:
		case 27007:
		case 27008:
		case 27051:
		case 27052:
		case 27116:
		case 27117:
		case 27118:
		case 27119:
		case 27120:
		case 27121:
		case 27201:
		case 27202:
		case 27203:
		case 27204:
		case 27205:
		case 27206:
			return true;
	}
	
	return false;
}

#ifdef ENABLE_EFFECT_STONE_SYSTEM
DWORD CItem::GetAddedEffectStone()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		int effect_stones[] = {87001,87002,87003,87004,87005,87006};
		
		for (int stoneval = 0; stoneval < _countof(effect_stones); stoneval++)
		{
			if (GetSocket(i) == effect_stones[stoneval])
				return GetSocket(i);
		}
	}
	
	return 0;
}

DWORD CItem::GetAddedEffectStoneWeapon()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		int effect_stones[] = {87101,87102,87103,87104};
		
		for (int stoneval = 0; stoneval < _countof(effect_stones); stoneval++)
		{
			if (GetSocket(i) == effect_stones[stoneval])
				return GetSocket(i);
		}
	}
	
	return 0;
}
#endif