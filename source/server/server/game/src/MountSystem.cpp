#include "stdafx.h"
#include "config.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "sectree_manager.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "MountSystem.h"
#include "../../common/VnumHelper.h"
#include "packet.h"
#include "item_manager.h"
#include "item.h"

EVENTINFO(mountsystem_event_info)
{
	CMountSystem* pMountSystem;
};

EVENTFUNC(mountsystem_update_event)
{
	mountsystem_event_info* info = dynamic_cast<mountsystem_event_info*>(event->info);
	if (info == NULL)
	{
		sys_err("<mountsystem_update_event> <Factor> Null pointer");
		return 0;
	}

	CMountSystem* pMountSystem = info->pMountSystem;

	if (NULL == pMountSystem)
		return 0;

	pMountSystem->Update(0);
	return PASSES_PER_SEC(1) / 4;
}

///////////////////////////////////////////////////////////////////////////////////////
//  CMountActor
///////////////////////////////////////////////////////////////////////////////////////

CMountActor::CMountActor(LPCHARACTER owner, DWORD vnum)
{
	m_dwVnum = vnum;
	m_dwVID = 0;
	m_dwLastActionTime = 0;

	m_pkChar = 0;
	m_pkOwner = owner;

	m_originalMoveSpeed = 0;

	m_dwSummonItemVID = 0;
	m_dwSummonItemVnum = 0;
}

CMountActor::~CMountActor()
{
	this->Unsummon();
	m_pkOwner = 0;
}

void CMountActor::SetName()
{
	char buf[64];
	if (0 != m_pkOwner && 0 != m_pkOwner->GetName())
		snprintf(buf, sizeof(buf), "%s - %s", m_pkOwner->GetName(), m_pkChar->GetMobTable().szLocaleName);
	else
		snprintf(buf, sizeof(buf), "%s", m_pkChar->GetMobTable().szLocaleName);

	if (true == IsSummoned())
		m_pkChar->SetName(buf);

	m_name = buf;
}

bool CMountActor::Mount(LPITEM mountItem)
{
	if (0 == m_pkOwner)
		return false;

	if (!mountItem)
		return false;

	if (m_pkOwner->IsHorseRiding())
		m_pkOwner->StopRiding();

	if (m_pkOwner->GetHorse())
		m_pkOwner->HorseSummon(false);

	Unmount();
	int TungaFixTime;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if ((LIMIT_REAL_TIME == mountItem->GetLimitType(i)) || (LIMIT_REAL_TIME_START_FIRST_USE == mountItem->GetLimitType(i)))
			TungaFixTime = (DWORD)mountItem->GetSocket(0);
		else
			TungaFixTime = INFINITE_AFFECT_DURATION;
	}
	m_pkOwner->AddAffect(AFFECT_MOUNT, POINT_MOUNT, m_dwVnum, AFF_NONE, TungaFixTime - time(0), 0, true);

	/*Double mount bonuss fix
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (mountItem->GetProto()->aApplies[i].bType == APPLY_NONE)
			continue;

		m_pkOwner->AddAffect(AFFECT_MOUNT_BONUS, aApplyInfo[mountItem->GetProto()->aApplies[i].bType].bPointType, mountItem->GetProto()->aApplies[i].lValue, AFF_NONE, TungaFixTime - time(0), 0, false);
	}*/

	return m_pkOwner->GetMountVnum() == m_dwVnum;
}

void CMountActor::Unmount()
{
	if (0 == m_pkOwner)
		return;

	if (!m_pkOwner->GetMountVnum())
		return;

	m_pkOwner->RemoveAffect(AFFECT_MOUNT);
	m_pkOwner->RemoveAffect(AFFECT_MOUNT_BONUS);
	m_pkOwner->MountVnum(0);

	if (m_pkOwner->IsHorseRiding())
		m_pkOwner->StopRiding();

	if (m_pkOwner->GetHorse())
		m_pkOwner->HorseSummon(false);

	m_pkOwner->MountVnum(0);
}

void CMountActor::Unsummon()
{
	if (true == this->IsSummoned())
	{
		this->SetSummonItem(NULL);

		if (NULL != m_pkChar)
			M2_DESTROY_CHARACTER(m_pkChar);

		m_pkChar = 0;
		m_dwVID = 0;
	}
}

DWORD CMountActor::Summon(LPITEM pSummonItem, bool bSpawnFar)
{
	long x = m_pkOwner->GetX();
	long y = m_pkOwner->GetY();
	long z = m_pkOwner->GetZ();

	if (true == bSpawnFar)
	{
		x += (number(0, 1) * 2 - 1) * number(2000, 2500);
		y += (number(0, 1) * 2 - 1) * number(2000, 2500);
	}
	else
	{
		x += number(-100, 100);
		y += number(-100, 100);
	}

	if (m_pkOwner->GetMapIndex() == 113) // In ox you can't summon your mount.
		return false;

	if (0 != m_pkChar)
	{
		m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y);
		m_dwVID = m_pkChar->GetVID();

		return m_dwVID;
	}

	m_pkChar = CHARACTER_MANAGER::instance().SpawnMob(m_dwVnum, m_pkOwner->GetMapIndex(), x, y, z, false, (int)(m_pkOwner->GetRotation() + 180), false);

	if (0 == m_pkChar)
	{
		sys_err("[CMountActor::Summon] Failed to summon the mount. (vnum: %d)", m_dwVnum);
		return 0;
	}

	m_pkChar->SetMount();

	m_pkChar->SetEmpire(m_pkOwner->GetEmpire());

	m_dwVID = m_pkChar->GetVID();

	this->SetName();

	this->SetSummonItem(pSummonItem);

	//m_pkOwner->ComputePoints(); mount lag fix

	m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y, z);

	return m_dwVID;
}

bool CMountActor::_UpdateFollowAI()
{
	if (0 == m_pkChar->m_pkMobData)
	{
		return false;
	}

	if (0 == m_originalMoveSpeed)
	{
		const CMob* mobData = CMobManager::Instance().Get(m_dwVnum);

		if (0 != mobData)
			m_originalMoveSpeed = mobData->m_table.sMovingSpeed;
	}
	float	START_FOLLOW_DISTANCE = 300.0f;

	float	RESPAWN_DISTANCE = 4500.f;
	int		APPROACH = 200;

	DWORD currentTime = get_dword_time();

	long ownerX = m_pkOwner->GetX();		long ownerY = m_pkOwner->GetY();
	long charX = m_pkChar->GetX();			long charY = m_pkChar->GetY();

	float fDist = DISTANCE_APPROX(charX - ownerX, charY - ownerY);

	if (fDist >= RESPAWN_DISTANCE)
	{
		float fOwnerRot = m_pkOwner->GetRotation() * 3.141592f / 180.f;
		float fx = -APPROACH * cos(fOwnerRot);
		float fy = -APPROACH * sin(fOwnerRot);
		if (m_pkChar->Show(m_pkOwner->GetMapIndex(), ownerX + fx, ownerY + fy))
		{
			return true;
		}
	}

	if (fDist >= START_FOLLOW_DISTANCE)
	{
		m_pkChar->SetNowWalking(false);

		Follow(APPROACH);

		m_pkChar->SetLastAttacked(currentTime);
		m_dwLastActionTime = currentTime;
	}
	else
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	return true;
}

bool CMountActor::Update(DWORD deltaTime)
{
	bool bResult = true;

	if ((this->GetSummonItemVID() != 0 && (NULL == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID()) || ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetOwner() != this->GetOwner())))
	{
		this->Unsummon();
		return true;
	}

	if (this->IsSummoned())
		bResult = bResult && this->_UpdateFollowAI();

	return bResult;
}

bool CMountActor::Follow(float fMinDistance)
{
	if (!m_pkOwner || !m_pkChar)
		return false;

	float fOwnerX = m_pkOwner->GetX();
	float fOwnerY = m_pkOwner->GetY();

	float fPetX = m_pkChar->GetX();
	float fPetY = m_pkChar->GetY();

	float fDist = DISTANCE_SQRT(fOwnerX - fPetX, fOwnerY - fPetY);
	if (fDist <= fMinDistance)
		return false;

	m_pkChar->SetRotationToXY(fOwnerX, fOwnerY);

	float fx, fy;

	float fDistToGo = fDist - fMinDistance;
	GetDeltaByDegree(m_pkChar->GetRotation(), fDistToGo, &fx, &fy);

	if (!m_pkChar->Goto((int)(fPetX + fx + 0.5f), (int)(fPetY + fy + 0.5f)))
		return false;

	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0, 0);

	return true;
}

void CMountActor::SetSummonItem(LPITEM pItem)
{
	if (NULL == pItem)
	{
		m_dwSummonItemVID = 0;
		m_dwSummonItemVnum = 0;
		return;
	}

	m_dwSummonItemVID = pItem->GetVID();
	m_dwSummonItemVnum = pItem->GetVnum();
}

///////////////////////////////////////////////////////////////////////////////////////
//  CMountSystem
///////////////////////////////////////////////////////////////////////////////////////

CMountSystem::CMountSystem(LPCHARACTER owner)
{
	m_pkOwner = owner;
	m_dwUpdatePeriod = 400;

	m_dwLastUpdateTime = 0;
}

CMountSystem::~CMountSystem()
{
	Destroy();
}

void CMountSystem::Destroy()
{
	for (auto iter = m_mountActorMap.begin(); iter != m_mountActorMap.end(); ++iter)
	{
		CMountActor* mountActor = iter->second;

		if (0 != mountActor)
		{
			delete mountActor;
		}
	}
	event_cancel(&m_pkMountSystemUpdateEvent);
	m_mountActorMap.clear();
}

bool CMountSystem::Update(DWORD deltaTime)
{
	bool bResult = true;

	DWORD currentTime = get_dword_time();

	if (m_dwUpdatePeriod > currentTime - m_dwLastUpdateTime)
		return true;

	std::vector <CMountActor*> v_garbageActor;

	for (auto iter = m_mountActorMap.begin(); iter != m_mountActorMap.end(); ++iter)
	{
		CMountActor* mountActor = iter->second;

		if (0 != mountActor && mountActor->IsSummoned())
		{
			LPCHARACTER pMount = mountActor->GetCharacter();

			if (NULL == CHARACTER_MANAGER::instance().Find(pMount->GetVID()))
				v_garbageActor.push_back(mountActor);
			else
				bResult = bResult && mountActor->Update(deltaTime);
		}
	}
	for (auto it = v_garbageActor.begin(); it != v_garbageActor.end(); it++)
		DeleteMount(*it);

	m_dwLastUpdateTime = currentTime;

	return bResult;
}

void CMountSystem::DeleteMount(DWORD mobVnum)
{
	auto iter = m_mountActorMap.find(mobVnum);

	if (m_mountActorMap.end() == iter)
	{
		sys_err("[CMountSystem::DeleteMount] Can't find mount on my list (VNUM: %d)", mobVnum);
		return;
	}

	CMountActor* mountActor = iter->second;

	if (0 == mountActor)
		sys_err("[CMountSystem::DeleteMount] Null Pointer (mountActor)");
	else
		delete mountActor;

	m_mountActorMap.erase(iter);
}

void CMountSystem::DeleteMount(CMountActor* mountActor)
{
	for (auto iter = m_mountActorMap.begin(); iter != m_mountActorMap.end(); ++iter)
	{
		if (iter->second == mountActor)
		{
			delete mountActor;
			m_mountActorMap.erase(iter);

			return;
		}
	}

	sys_err("[CMountSystem::DeleteMount] Can't find mountActor(0x%x) on my list(size: %d) ", mountActor, m_mountActorMap.size());
}

void CMountSystem::Unsummon(DWORD vnum, bool bDeleteFromList)
{
	//if (m_pkOwner->IncreaseMountCounter() >= 5)
	//{
	//	m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_TRANSLATE_CHAT_TYPE_PACKETS_FROM_SOURCE_TO_GLOBAL__TRANSLATE_LIST_110"));
	//	return;
	//}

	CMountActor* actor = this->GetByVnum(vnum);

	if (0 == actor)
	{
		sys_err("[CMountSystem::Unsummon(%d)] Null Pointer (actor)", vnum);
		return;
	}
	actor->Unsummon();

	if (true == bDeleteFromList)
		this->DeleteMount(actor);

	bool bActive = false;
	for (auto it = m_mountActorMap.begin(); it != m_mountActorMap.end(); it++)
	{
		bActive |= it->second->IsSummoned();
	}
	if (false == bActive)
	{
		event_cancel(&m_pkMountSystemUpdateEvent);
		m_pkMountSystemUpdateEvent = NULL;
	}
}

void CMountSystem::Summon(DWORD mobVnum, LPITEM pSummonItem, bool bSpawnFar)
{
	//if (m_pkOwner->IncreaseMountCounter() >= 5)
	//{
	//	m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_TRANSLATE_CHAT_TYPE_PACKETS_FROM_SOURCE_TO_GLOBAL__TRANSLATE_LIST_110"));
	//	return;
	//}

	CMountActor* mountActor = this->GetByVnum(mobVnum);

	if (0 == mountActor)
	{
		mountActor = M2_NEW CMountActor(m_pkOwner, mobVnum);
		m_mountActorMap.insert(std::make_pair(mobVnum, mountActor));
	}

	DWORD mountVID = mountActor->Summon(pSummonItem, bSpawnFar);

	if (!mountVID)
		sys_err("[CMountSystem::Summon(%d)] Null Pointer (mountVID)", pSummonItem->GetID());
	
	if (NULL == m_pkMountSystemUpdateEvent)
	{
		mountsystem_event_info* info = AllocEventInfo<mountsystem_event_info>();

		info->pMountSystem = this;

		m_pkMountSystemUpdateEvent = event_create(mountsystem_update_event, info, PASSES_PER_SEC(1) / 4);
	}

	//return mountActor;
}

void CMountSystem::Mount(DWORD mobVnum, LPITEM mountItem)
{
	CMountActor* mountActor = this->GetByVnum(mobVnum);

	if (!mountActor)
	{
		sys_err("[CMountSystem::Mount] Null Pointer (mountActor)");
		return;
	}

	if (!mountItem)
		return;

	//check timer
	// if (m_pkOwner->IncreaseMountCounter() >= 5)
	// {
		// m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "<Mount> Asteapta 5 secunde pentru a face aceasta actiune");
		// return;
	// }

	this->Unsummon(mobVnum, false);
	mountActor->Mount(mountItem);
}

void CMountSystem::Unmount(DWORD mobVnum)
{
	CMountActor* mountActor = this->GetByVnum(mobVnum);

	if (!mountActor)
	{
		sys_err("[CMountSystem::Mount] Null Pointer (mountActor)");
		return;
	}

	//check timer
	// if (m_pkOwner->IncreaseMountCounter() >= 5)
	// {
		// m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "<Mount> Asteapta 5 secunde pentru a face aceasta actiune");
		// return;
	// }

	if (LPITEM pSummonItem = m_pkOwner->GetWear(WEAR_COSTUME_MOUNT))
	{
		this->Summon(mobVnum, pSummonItem, false);
	}

	mountActor->Unmount();
}

CMountActor* CMountSystem::GetByVID(DWORD vid) const
{
	CMountActor* mountActor = 0;

	bool bFound = false;

	for (auto iter = m_mountActorMap.begin(); iter != m_mountActorMap.end(); ++iter)
	{
		mountActor = iter->second;

		if (0 == mountActor)
		{
			sys_err("[CMountSystem::GetByVID(%d)] Null Pointer (mountActor)", vid);
			continue;
		}

		bFound = mountActor->GetVID() == vid;

		if (true == bFound)
			break;
	}

	return bFound ? mountActor : 0;
}

CMountActor* CMountSystem::GetByVnum(DWORD vnum) const
{
	CMountActor* mountActor = 0;

	auto iter = m_mountActorMap.find(vnum);

	if (m_mountActorMap.end() != iter)
		mountActor = iter->second;

	return mountActor;
}

size_t CMountSystem::CountSummoned() const
{
	size_t count = 0;

	for (auto iter = m_mountActorMap.begin(); iter != m_mountActorMap.end(); ++iter)
	{
		CMountActor* mountActor = iter->second;

		if (0 != mountActor)
		{
			if (mountActor->IsSummoned())
				++count;
		}
	}

	return count;
}