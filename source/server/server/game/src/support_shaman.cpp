#include "stdafx.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "sectree_manager.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "support_shaman.h"
#include "../../common/VnumHelper.h"
#include "packet.h"
#include "item_manager.h"
#include "item.h"
#include "affect.h"
#include "skill.h"

extern int passes_per_sec;
EVENTINFO(SupportShaman_event_info)
{
	CSupportShaman* pSupportShaman;
};

struct FPartyPIDCollector {
	std::vector <DWORD> vecPIDs;
	FPartyPIDCollector()
	{
	}
	void operator () (LPCHARACTER ch)
	{
		vecPIDs.push_back(ch->GetPlayerID());
	}
};

EVENTFUNC(SupportShaman_update_event)
{
	SupportShaman_event_info* info = dynamic_cast<SupportShaman_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("check_speedhack_event> <Factor> nullptr pointer");
		return 0;
	}

	CSupportShaman* pSupportShaman = info->pSupportShaman;

	if (nullptr == pSupportShaman)
		return 0;

	pSupportShaman->Update(0);
	return PASSES_PER_SEC(1) / 4;
}
const float SupportShaman_COUNT_LIMIT = 3;

CSupportShamanActor::CSupportShamanActor(LPCHARACTER owner, DWORD vnum, DWORD options)
{
	m_dwVnum = vnum;
	m_dwVID = 0;
	m_dwOptions = options;
	m_dwLastActionTime = 0;

	m_dwlevel = 1;
	m_dwExp = 0;
	m_dwNextExp = 0;
	m_dwIntSkill = 3;

	m_pkChar = 0;
	m_pkOwner = owner;

	m_originalMoveSpeed = 0;

	m_dwSummonItemVID = 0;
	m_dwSummonItemVnum = 0;
}

CSupportShamanActor::~CSupportShamanActor()
{
	this->Unsummon();

	m_pkOwner = 0;
}

void CSupportShamanActor::SetName()
{
	std::string SupportShamanName = m_pkChar->GetOwner()->GetName();
	SupportShamanName += " - Buffi";

	if (true == IsSummoned())
		m_pkChar->SetName(SupportShamanName);
}

void CSupportShamanActor::SetNextExp(int nextExp)
{
	m_dwNextExp = nextExp;
}

void CSupportShamanActor::SetIntSkill()
{
	if (GetLevel() * 1 >= 90)
		m_dwIntSkill = 90;
	else
		m_dwIntSkill = GetLevel() * 1;

	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr)
		pSummonItem->SetForceAttribute(1, 1, m_dwIntSkill);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanZeka %d", m_dwIntSkill);
}

int CSupportShamanActor::GetIntSkill()
{
	int k = 0;
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr)
		k = pSummonItem->GetAttributeValue(1) + pSummonItem->GetAttributeValue(2);

	return k;
}
void CSupportShamanActor::SetLevel(DWORD level)
{
	m_pkChar->SetLevel(static_cast<char>(level));
	m_dwlevel = level;
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanSeviye %d", m_dwlevel);
	SetNextExp(m_pkChar->SupportShamanGetNextExp());
	SetIntSkill();
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr)
		pSummonItem->SetForceAttribute(0, 1, m_dwlevel);
}
void CSupportShamanActor::SetCostume()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	m_pkChar->SetSupportShamanArmor(41120);
	// if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 8)
		// m_pkChar->SetSupportShamanArmor(11809);
	// if (m_pkChar->GetLevel() >= 9 && m_pkChar->GetLevel() <= 17)
		// m_pkChar->SetSupportShamanArmor(11819);
	// if (m_pkChar->GetLevel() >= 18 && m_pkChar->GetLevel() <= 25)
		// m_pkChar->SetSupportShamanArmor(11829);
	// if (m_pkChar->GetLevel() >= 26 && m_pkChar->GetLevel() <= 33)
		// m_pkChar->SetSupportShamanArmor(11839);
	// if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 41)
		// m_pkChar->SetSupportShamanArmor(11849);
	// if (m_pkChar->GetLevel() >= 42 && m_pkChar->GetLevel() <= 47)
		// m_pkChar->SetSupportShamanArmor(11859);
	// if (m_pkChar->GetLevel() >= 48 && m_pkChar->GetLevel() <= 53)
		// m_pkChar->SetSupportShamanArmor(11869);
	// if (m_pkChar->GetLevel() >= 54 && m_pkChar->GetLevel() <= 60)
		// m_pkChar->SetSupportShamanArmor(11879);
	// if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 69)
		// m_pkChar->SetSupportShamanArmor(11889);
	// if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 89)
		// m_pkChar->SetSupportShamanArmor(11899);
	// if (m_pkChar->GetLevel() >= 90 && m_pkChar->GetLevel() <= 104)
		// m_pkChar->SetSupportShamanArmor(12049);
	// if (m_pkChar->GetLevel() >= 105 && m_pkChar->GetLevel() <= 114)
		// m_pkChar->SetSupportShamanArmor(20759);
	// if (m_pkChar->GetLevel() >= 115 && m_pkChar->GetLevel() <= 119)
		// m_pkChar->SetSupportShamanArmor(20849);
	// if (m_pkChar->GetLevel() == 120)
		// m_pkChar->SetSupportShamanArmor(12089);
}

void CSupportShamanActor::SetWeapon()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 4)
		m_pkChar->SetSupportShamanWeapon(7009);
	if (m_pkChar->GetLevel() >= 5 && m_pkChar->GetLevel() <= 9)
		m_pkChar->SetSupportShamanWeapon(7019);
	if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 14)
		m_pkChar->SetSupportShamanWeapon(7029);
	if (m_pkChar->GetLevel() >= 15 && m_pkChar->GetLevel() <= 19)
		m_pkChar->SetSupportShamanWeapon(7039);
	if (m_pkChar->GetLevel() >= 20 && m_pkChar->GetLevel() <= 24)
		m_pkChar->SetSupportShamanWeapon(7049);
	if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 31)
		m_pkChar->SetSupportShamanWeapon(7059);
	if (m_pkChar->GetLevel() >= 32 && m_pkChar->GetLevel() <= 35)
		m_pkChar->SetSupportShamanWeapon(7069);
	if (m_pkChar->GetLevel() >= 36 && m_pkChar->GetLevel() <= 39)
		m_pkChar->SetSupportShamanWeapon(7079);
	if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 44)
		m_pkChar->SetSupportShamanWeapon(7089);
	if (m_pkChar->GetLevel() >= 45 && m_pkChar->GetLevel() <= 49)
		m_pkChar->SetSupportShamanWeapon(7099);
	if (m_pkChar->GetLevel() >= 50 && m_pkChar->GetLevel() <= 54)
		m_pkChar->SetSupportShamanWeapon(7109);
	if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 59)
		m_pkChar->SetSupportShamanWeapon(7119);
	if (m_pkChar->GetLevel() >= 60 && m_pkChar->GetLevel() <= 64)
		m_pkChar->SetSupportShamanWeapon(7129);
	if (m_pkChar->GetLevel() >= 65 && m_pkChar->GetLevel() <= 69)
		m_pkChar->SetSupportShamanWeapon(7149);
	if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 89)
		m_pkChar->SetSupportShamanWeapon(7159);
	if (m_pkChar->GetLevel() >= 90 && m_pkChar->GetLevel() <= 104)
		m_pkChar->SetSupportShamanWeapon(7199);
	if (m_pkChar->GetLevel() >= 105 && m_pkChar->GetLevel() <= 119)
		m_pkChar->SetSupportShamanWeapon(7379);
	if (m_pkChar->GetLevel() == 120)
		m_pkChar->SetSupportShamanWeapon(7309);
}

#ifdef ENABLE_ACCE_SYSTEM
void CSupportShamanActor::SetSash()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 34)
		m_pkChar->SetSupportShamanSash(2001);
	else if (m_pkChar->GetLevel() >= 35 && m_pkChar->GetLevel() <= 69)
		m_pkChar->SetSupportShamanSash(2002);
	else if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 104)
		m_pkChar->SetSupportShamanSash(2003);
	else if (m_pkChar->GetLevel() >= 105 && m_pkChar->GetLevel() <= 120)
		m_pkChar->SetSupportShamanSash(2004);
}
#endif

void CSupportShamanActor::SetHair()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	m_pkChar->SetSupportShamanHair(5030);
}

void CSupportShamanActor::SetExp(DWORD exp)
{
	if (exp < 0)
		exp = MAX(m_dwExp - exp, 0);

	if (GetExp() + exp >= GetNextExp())
	{
		SetLevel(GetLevel() + 1);
		m_pkChar->UpdatePacket();
		m_dwExp = 0;
		m_pkChar->SetExp(0);
		return;
	}
	m_dwExp += exp;
	m_pkChar->SetExp(m_dwExp);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanExp %d %d", m_dwExp, m_pkChar->SupportShamanGetNextExp());
}

bool CSupportShamanActor::Mount()
{
	if (0 == m_pkOwner)
		return false;

	if (true == HasOption(ESupportShamanOption_Mountable))
		m_pkOwner->MountVnum(m_dwVnum);

	return m_pkOwner->GetMountVnum() == m_dwVnum;;
}

void CSupportShamanActor::Unmount()
{
	if (0 == m_pkOwner)
		return;

	if (m_pkOwner->IsHorseRiding())
		m_pkOwner->StopRiding();
}

void CSupportShamanActor::Unsummon()
{
	if (true == this->IsSummoned())
	{
		this->ClearBuff();

		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
		if (pSummonItem != nullptr) {
			pSummonItem->SetForceAttribute(0, 1, m_dwlevel);
			pSummonItem->SetSocket(2, m_dwExp);
			pSummonItem->SetForceAttribute(1, 1, m_dwIntSkill);
		}

		this->SetSummonItem(nullptr);
		if (nullptr != m_pkOwner)
			m_pkOwner->ComputePoints();

		if (nullptr != m_pkChar)
			M2_DESTROY_CHARACTER(m_pkChar);

		m_pkChar = 0;
		m_dwVID = 0;
		m_dwlevel = 1;
		m_dwExp = 0;
		m_dwNextExp = 0;
		m_dwIntSkill = 0;
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanArkaplan");
	}
}

DWORD CSupportShamanActor::Summon(const char* SupportShamanName, LPITEM pSummonItem, bool bSpawnFar)
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

	if (0 != m_pkChar)
	{
		m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y);
		m_dwVID = m_pkChar->GetVID();

		return m_dwVID;
	}

	m_pkChar = CHARACTER_MANAGER::instance().SpawnMob(m_dwVnum, m_pkOwner->GetMapIndex(), x, y, z, false, (int)(m_pkOwner->GetRotation() + 180), false);

	if (0 == m_pkChar)
	{
		sys_err("[CSupportShaman::Summon] Failed to summon the shaman. (vnum: %d)", m_dwVnum);
		return 0;
	}

	m_pkChar->SetSupportShaman();

	m_pkChar->SetOwner(m_pkOwner);
	m_pkChar->SetEmpire(m_pkOwner->GetEmpire());

	m_dwVID = m_pkChar->GetVID();

	m_dwlevel = pSummonItem->GetAttributeValue(0);
	m_dwExp = pSummonItem->GetSocket(2);
	m_dwIntSkill = pSummonItem->GetAttributeValue(1);
	this->SetSummonItem(pSummonItem);

	this->SetCostume();
	this->SetWeapon();
#ifdef ENABLE_ACCE_SYSTEM
	this->SetSash();
#endif
	this->SetHair();
	if (m_dwlevel == 0)
	{
		m_dwlevel = 1;
		m_dwExp = 0;
		this->SetLevel(1);
	}
	else
		this->SetLevel(m_dwlevel);

	m_dwNextExp = m_pkChar->SupportShamanGetNextExp();

	this->SetName();

	std::string m_speciality;
	if (m_dwSummonItemVnum == 81001 || m_dwSummonItemVnum == 81003 || m_dwSummonItemVnum == 81004)
		m_speciality = "E.Gücü";
	else if (m_dwSummonItemVnum == 81002 || m_dwSummonItemVnum == 81005)
		m_speciality = "Ýyileþtirme";

	m_pkOwner->ComputePoints();
	m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y, z);
	m_pkChar->UpdatePacket();
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanGuiAc %d", m_dwVnum);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanSeviye %d", m_dwlevel);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanZeka %d", m_dwIntSkill);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanExp %d %d", m_dwExp, m_pkChar->SupportShamanGetNextExp());
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanIcon %d", m_dwSummonItemVnum);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "SupportShamanDerece %s", m_speciality.c_str());
	return m_dwVID;
}
void CSupportShamanActor::RefreshCostume()
{
	SetCostume();
	SetWeapon();
#ifdef ENABLE_ACCE_SYSTEM
	SetSash();
#endif
	SetHair();
	m_pkChar->UpdatePacket();
}
void CSupportShamanActor::UpdatePacketSupportShamanActor()
{
	m_pkChar->UpdatePacket();
}
bool CSupportShamanActor::_UpdatAloneActionAI(float fMinDist, float fMaxDist)
{
	float fDist = number(fMinDist, fMaxDist);
	float r = (float)number(0, 359);
	float dest_x = GetOwner()->GetX() + fDist * cos(r);
	float dest_y = GetOwner()->GetY() + fDist * sin(r);

	m_pkChar->SetNowWalking(true);

	if (!m_pkChar->IsStateMove() && m_pkChar->Goto(dest_x, dest_y))
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	m_dwLastActionTime = get_dword_time();

	return true;
}

bool CSupportShamanActor::_UpdateFollowAI()
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
	float	START_RUN_DISTANCE = 400.0f;

	float	RESPAWN_DISTANCE = 4500.f;
	int		APPROACH = 200;

	bool bDoMoveAlone = true;
	bool bRun = false;

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
		if (fDist >= START_RUN_DISTANCE)
		{
			bRun = true;
		}

		m_pkChar->SetNowWalking(!bRun);

		Follow(APPROACH);

		m_pkChar->SetLastAttacked(currentTime);
		m_dwLastActionTime = currentTime;
	}
	else
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	return true;
}

bool CSupportShamanActor::Update(DWORD deltaTime)
{
	bool bResult = true;

	if (m_pkOwner->IsDead() || (IsSummoned() && m_pkChar->IsDead()) || nullptr == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID()) || ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetOwner() != this->GetOwner())
	{
		this->Unsummon();
		return true;
	}
	if (get_dword_time() - this->GetLastExpUpdate() >= 3000)
	{
		this->UpdateItemExp();
		m_pkChar->SetSupportShamanInt(this->GetIntSkill());
		this->SetLastExpUpdate(get_dword_time());
	}

	this->RefreshCostume();
	this->UseSkill();

	if (this->IsSummoned() && HasOption(ESupportShamanOption_Followable))
		bResult = bResult && this->_UpdateFollowAI();

	return bResult;
}
void CSupportShamanActor::UseSkill()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (!pSummonItem)
		return;
	if (pSummonItem->GetVnum() == 81001 || pSummonItem->GetVnum() == 81003 || pSummonItem->GetVnum() == 81004)
	{
		if (m_pkChar->GetOwner() && (m_pkChar->GetOwner()->IsAffectFlag(AFF_HOSIN) == false && get_dword_time() - this->GetLastSkillTime() >= 3000))
		{
			if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 3)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 1);
			if (m_pkChar->GetLevel() >= 4 && m_pkChar->GetLevel() <= 6)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 2);
			if (m_pkChar->GetLevel() >= 7 && m_pkChar->GetLevel() <= 9)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 3);
			if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 12)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 4);
			if (m_pkChar->GetLevel() >= 13 && m_pkChar->GetLevel() <= 15)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 5);
			if (m_pkChar->GetLevel() >= 16 && m_pkChar->GetLevel() <= 18)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 6);
			if (m_pkChar->GetLevel() >= 19 && m_pkChar->GetLevel() <= 21)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 7);
			if (m_pkChar->GetLevel() >= 22 && m_pkChar->GetLevel() <= 24)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 8);
			if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 27)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 9);
			if (m_pkChar->GetLevel() >= 28 && m_pkChar->GetLevel() <= 30)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 10);
			if (m_pkChar->GetLevel() >= 31 && m_pkChar->GetLevel() <= 33)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 11);
			if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 36)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 12);
			if (m_pkChar->GetLevel() >= 37 && m_pkChar->GetLevel() <= 39)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 13);
			if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 42)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 14);
			if (m_pkChar->GetLevel() >= 43 && m_pkChar->GetLevel() <= 45)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 15);
			if (m_pkChar->GetLevel() >= 46 && m_pkChar->GetLevel() <= 48)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 16);
			if (m_pkChar->GetLevel() >= 49 && m_pkChar->GetLevel() <= 51)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 17);
			if (m_pkChar->GetLevel() >= 52 && m_pkChar->GetLevel() <= 54)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 18);
			if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 57)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 19);
			if (m_pkChar->GetLevel() >= 58 && m_pkChar->GetLevel() <= 60)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 20);
			if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 63)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 21);
			if (m_pkChar->GetLevel() >= 64 && m_pkChar->GetLevel() <= 66)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 22);
			if (m_pkChar->GetLevel() >= 67 && m_pkChar->GetLevel() <= 69)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 23);
			if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 72)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 24);
			if (m_pkChar->GetLevel() >= 73 && m_pkChar->GetLevel() <= 75)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 25);
			if (m_pkChar->GetLevel() >= 76 && m_pkChar->GetLevel() <= 78)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 26);
			if (m_pkChar->GetLevel() >= 79 && m_pkChar->GetLevel() <= 81)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 27);
			if (m_pkChar->GetLevel() >= 82 && m_pkChar->GetLevel() <= 84)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 28);
			if (m_pkChar->GetLevel() >= 85 && m_pkChar->GetLevel() <= 87)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 29);
			if (m_pkChar->GetLevel() >= 88 && m_pkChar->GetLevel() <= 90)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 30);
			if (m_pkChar->GetLevel() >= 91 && m_pkChar->GetLevel() <= 93)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 31);
			if (m_pkChar->GetLevel() >= 94 && m_pkChar->GetLevel() <= 96)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 32);
			if (m_pkChar->GetLevel() >= 97 && m_pkChar->GetLevel() <= 99)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 33);
			if (m_pkChar->GetLevel() >= 100 && m_pkChar->GetLevel() <= 102)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 34);
			if (m_pkChar->GetLevel() >= 103 && m_pkChar->GetLevel() <= 105)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 35);
			if (m_pkChar->GetLevel() >= 106 && m_pkChar->GetLevel() <= 108)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 36);
			if (m_pkChar->GetLevel() >= 109 && m_pkChar->GetLevel() <= 110)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 37);
			if (m_pkChar->GetLevel() >= 111 && m_pkChar->GetLevel() <= 113)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 38);
			if (m_pkChar->GetLevel() >= 114 && m_pkChar->GetLevel() <= 116)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 39);
			if (m_pkChar->GetLevel() >= 117 && m_pkChar->GetLevel() <= 118)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 40);
			if (m_pkChar->GetLevel() >= 119 && m_pkChar->GetLevel() <= 120)
				m_pkChar->ComputeSkill(610, m_pkChar->GetOwner(), 41);
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportShamanSkillPacket(SKILL_HOSIN);
		}
		else if ((m_pkChar->GetOwner() && m_pkChar->GetOwner()->IsAffectFlag(AFF_BOHO)) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 3)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 1);
			if (m_pkChar->GetLevel() >= 4 && m_pkChar->GetLevel() <= 6)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 2);
			if (m_pkChar->GetLevel() >= 7 && m_pkChar->GetLevel() <= 9)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 3);
			if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 12)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 4);
			if (m_pkChar->GetLevel() >= 13 && m_pkChar->GetLevel() <= 15)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 5);
			if (m_pkChar->GetLevel() >= 16 && m_pkChar->GetLevel() <= 18)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 6);
			if (m_pkChar->GetLevel() >= 19 && m_pkChar->GetLevel() <= 21)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 7);
			if (m_pkChar->GetLevel() >= 22 && m_pkChar->GetLevel() <= 24)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 8);
			if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 27)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 9);
			if (m_pkChar->GetLevel() >= 28 && m_pkChar->GetLevel() <= 30)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 10);
			if (m_pkChar->GetLevel() >= 31 && m_pkChar->GetLevel() <= 33)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 11);
			if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 36)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 12);
			if (m_pkChar->GetLevel() >= 37 && m_pkChar->GetLevel() <= 39)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 13);
			if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 42)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 14);
			if (m_pkChar->GetLevel() >= 43 && m_pkChar->GetLevel() <= 45)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 15);
			if (m_pkChar->GetLevel() >= 46 && m_pkChar->GetLevel() <= 48)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 16);
			if (m_pkChar->GetLevel() >= 49 && m_pkChar->GetLevel() <= 51)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 17);
			if (m_pkChar->GetLevel() >= 52 && m_pkChar->GetLevel() <= 54)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 18);
			if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 57)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 19);
			if (m_pkChar->GetLevel() >= 58 && m_pkChar->GetLevel() <= 60)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 20);
			if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 63)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 21);
			if (m_pkChar->GetLevel() >= 64 && m_pkChar->GetLevel() <= 66)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 22);
			if (m_pkChar->GetLevel() >= 67 && m_pkChar->GetLevel() <= 69)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 23);
			if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 72)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 24);
			if (m_pkChar->GetLevel() >= 73 && m_pkChar->GetLevel() <= 75)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 25);
			if (m_pkChar->GetLevel() >= 76 && m_pkChar->GetLevel() <= 78)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 26);
			if (m_pkChar->GetLevel() >= 79 && m_pkChar->GetLevel() <= 81)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 27);
			if (m_pkChar->GetLevel() >= 82 && m_pkChar->GetLevel() <= 84)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 28);
			if (m_pkChar->GetLevel() >= 85 && m_pkChar->GetLevel() <= 87)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 29);
			if (m_pkChar->GetLevel() >= 88 && m_pkChar->GetLevel() <= 90)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 30);
			if (m_pkChar->GetLevel() >= 91 && m_pkChar->GetLevel() <= 93)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 31);
			if (m_pkChar->GetLevel() >= 94 && m_pkChar->GetLevel() <= 96)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 32);
			if (m_pkChar->GetLevel() >= 97 && m_pkChar->GetLevel() <= 99)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 33);
			if (m_pkChar->GetLevel() >= 100 && m_pkChar->GetLevel() <= 102)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 34);
			if (m_pkChar->GetLevel() >= 103 && m_pkChar->GetLevel() <= 105)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 35);
			if (m_pkChar->GetLevel() >= 106 && m_pkChar->GetLevel() <= 108)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 36);
			if (m_pkChar->GetLevel() >= 109 && m_pkChar->GetLevel() <= 110)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 37);
			if (m_pkChar->GetLevel() >= 111 && m_pkChar->GetLevel() <= 113)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 38);
			if (m_pkChar->GetLevel() >= 114 && m_pkChar->GetLevel() <= 116)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 39);
			if (m_pkChar->GetLevel() >= 117 && m_pkChar->GetLevel() <= 118)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 40);
			if (m_pkChar->GetLevel() >= 119 && m_pkChar->GetLevel() <= 120)
				m_pkChar->ComputeSkill(611, m_pkChar->GetOwner(), 41);
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportShamanSkillPacket(SKILL_REFLECT);
		}
		else if ((m_pkChar->GetOwner() && m_pkChar->GetOwner()->IsAffectFlag(AFF_GICHEON)) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 3)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 1);
			if (m_pkChar->GetLevel() >= 4 && m_pkChar->GetLevel() <= 6)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 2);
			if (m_pkChar->GetLevel() >= 7 && m_pkChar->GetLevel() <= 9)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 3);
			if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 12)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 4);
			if (m_pkChar->GetLevel() >= 13 && m_pkChar->GetLevel() <= 15)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 5);
			if (m_pkChar->GetLevel() >= 16 && m_pkChar->GetLevel() <= 18)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 6);
			if (m_pkChar->GetLevel() >= 19 && m_pkChar->GetLevel() <= 21)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 7);
			if (m_pkChar->GetLevel() >= 22 && m_pkChar->GetLevel() <= 24)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 8);
			if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 27)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 9);
			if (m_pkChar->GetLevel() >= 28 && m_pkChar->GetLevel() <= 30)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 10);
			if (m_pkChar->GetLevel() >= 31 && m_pkChar->GetLevel() <= 33)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 11);
			if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 36)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 12);
			if (m_pkChar->GetLevel() >= 37 && m_pkChar->GetLevel() <= 39)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 13);
			if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 42)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 14);
			if (m_pkChar->GetLevel() >= 43 && m_pkChar->GetLevel() <= 45)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 15);
			if (m_pkChar->GetLevel() >= 46 && m_pkChar->GetLevel() <= 48)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 16);
			if (m_pkChar->GetLevel() >= 49 && m_pkChar->GetLevel() <= 51)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 17);
			if (m_pkChar->GetLevel() >= 52 && m_pkChar->GetLevel() <= 54)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 18);
			if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 57)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 19);
			if (m_pkChar->GetLevel() >= 58 && m_pkChar->GetLevel() <= 60)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 20);
			if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 63)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 21);
			if (m_pkChar->GetLevel() >= 64 && m_pkChar->GetLevel() <= 66)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 22);
			if (m_pkChar->GetLevel() >= 67 && m_pkChar->GetLevel() <= 69)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 23);
			if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 72)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 24);
			if (m_pkChar->GetLevel() >= 73 && m_pkChar->GetLevel() <= 75)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 25);
			if (m_pkChar->GetLevel() >= 76 && m_pkChar->GetLevel() <= 78)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 26);
			if (m_pkChar->GetLevel() >= 79 && m_pkChar->GetLevel() <= 81)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 27);
			if (m_pkChar->GetLevel() >= 82 && m_pkChar->GetLevel() <= 84)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 28);
			if (m_pkChar->GetLevel() >= 85 && m_pkChar->GetLevel() <= 87)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 29);
			if (m_pkChar->GetLevel() >= 88 && m_pkChar->GetLevel() <= 90)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 30);
			if (m_pkChar->GetLevel() >= 91 && m_pkChar->GetLevel() <= 93)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 31);
			if (m_pkChar->GetLevel() >= 94 && m_pkChar->GetLevel() <= 96)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 32);
			if (m_pkChar->GetLevel() >= 97 && m_pkChar->GetLevel() <= 99)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 33);
			if (m_pkChar->GetLevel() >= 100 && m_pkChar->GetLevel() <= 102)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 34);
			if (m_pkChar->GetLevel() >= 103 && m_pkChar->GetLevel() <= 105)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 35);
			if (m_pkChar->GetLevel() >= 106 && m_pkChar->GetLevel() <= 108)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 36);
			if (m_pkChar->GetLevel() >= 109 && m_pkChar->GetLevel() <= 110)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 37);
			if (m_pkChar->GetLevel() >= 111 && m_pkChar->GetLevel() <= 113)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 38);
			if (m_pkChar->GetLevel() >= 114 && m_pkChar->GetLevel() <= 116)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 39);
			if (m_pkChar->GetLevel() >= 117 && m_pkChar->GetLevel() <= 118)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 40);
			if (m_pkChar->GetLevel() >= 119 && m_pkChar->GetLevel() <= 120)
				m_pkChar->ComputeSkill(612, m_pkChar->GetOwner(), 41);
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportShamanSkillPacket(SKILL_GICHEON);
		}
	}
	else
	{
		if ((m_pkChar->GetOwner() && m_pkChar->GetOwner()->IsAffectFlag(AFF_KWAESOK)) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 3)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 1);
			if (m_pkChar->GetLevel() >= 4 && m_pkChar->GetLevel() <= 6)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 2);
			if (m_pkChar->GetLevel() >= 7 && m_pkChar->GetLevel() <= 9)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 3);
			if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 12)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 4);
			if (m_pkChar->GetLevel() >= 13 && m_pkChar->GetLevel() <= 15)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 5);
			if (m_pkChar->GetLevel() >= 16 && m_pkChar->GetLevel() <= 18)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 6);
			if (m_pkChar->GetLevel() >= 19 && m_pkChar->GetLevel() <= 21)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 7);
			if (m_pkChar->GetLevel() >= 22 && m_pkChar->GetLevel() <= 24)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 8);
			if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 27)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 9);
			if (m_pkChar->GetLevel() >= 28 && m_pkChar->GetLevel() <= 30)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 10);
			if (m_pkChar->GetLevel() >= 31 && m_pkChar->GetLevel() <= 33)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 11);
			if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 36)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 12);
			if (m_pkChar->GetLevel() >= 37 && m_pkChar->GetLevel() <= 39)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 13);
			if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 42)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 14);
			if (m_pkChar->GetLevel() >= 43 && m_pkChar->GetLevel() <= 45)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 15);
			if (m_pkChar->GetLevel() >= 46 && m_pkChar->GetLevel() <= 48)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 16);
			if (m_pkChar->GetLevel() >= 49 && m_pkChar->GetLevel() <= 51)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 17);
			if (m_pkChar->GetLevel() >= 52 && m_pkChar->GetLevel() <= 54)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 18);
			if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 57)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 19);
			if (m_pkChar->GetLevel() >= 58 && m_pkChar->GetLevel() <= 60)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 20);
			if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 63)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 21);
			if (m_pkChar->GetLevel() >= 64 && m_pkChar->GetLevel() <= 66)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 22);
			if (m_pkChar->GetLevel() >= 67 && m_pkChar->GetLevel() <= 69)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 23);
			if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 72)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 24);
			if (m_pkChar->GetLevel() >= 73 && m_pkChar->GetLevel() <= 75)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 25);
			if (m_pkChar->GetLevel() >= 76 && m_pkChar->GetLevel() <= 78)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 26);
			if (m_pkChar->GetLevel() >= 79 && m_pkChar->GetLevel() <= 81)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 27);
			if (m_pkChar->GetLevel() >= 82 && m_pkChar->GetLevel() <= 84)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 28);
			if (m_pkChar->GetLevel() >= 85 && m_pkChar->GetLevel() <= 87)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 29);
			if (m_pkChar->GetLevel() >= 88 && m_pkChar->GetLevel() <= 90)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 30);
			if (m_pkChar->GetLevel() >= 91 && m_pkChar->GetLevel() <= 93)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 31);
			if (m_pkChar->GetLevel() >= 94 && m_pkChar->GetLevel() <= 96)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 32);
			if (m_pkChar->GetLevel() >= 97 && m_pkChar->GetLevel() <= 99)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 33);
			if (m_pkChar->GetLevel() >= 100 && m_pkChar->GetLevel() <= 102)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 34);
			if (m_pkChar->GetLevel() >= 103 && m_pkChar->GetLevel() <= 105)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 35);
			if (m_pkChar->GetLevel() >= 106 && m_pkChar->GetLevel() <= 108)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 36);
			if (m_pkChar->GetLevel() >= 109 && m_pkChar->GetLevel() <= 110)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 37);
			if (m_pkChar->GetLevel() >= 111 && m_pkChar->GetLevel() <= 113)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 38);
			if (m_pkChar->GetLevel() >= 114 && m_pkChar->GetLevel() <= 116)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 39);
			if (m_pkChar->GetLevel() >= 117 && m_pkChar->GetLevel() <= 118)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 40);
			if (m_pkChar->GetLevel() >= 119 && m_pkChar->GetLevel() <= 120)
				m_pkChar->ComputeSkill(613, m_pkChar->GetOwner(), 41);
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportShamanSkillPacket(SKILL_KWAESOK);
		}
		else if ((m_pkChar->GetOwner() && m_pkChar->GetOwner()->IsAffectFlag(AFF_JEUNGRYEOK)) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			if (m_pkChar->GetLevel() >= 1 && m_pkChar->GetLevel() <= 3)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 1);
			if (m_pkChar->GetLevel() >= 4 && m_pkChar->GetLevel() <= 6)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 2);
			if (m_pkChar->GetLevel() >= 7 && m_pkChar->GetLevel() <= 9)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 3);
			if (m_pkChar->GetLevel() >= 10 && m_pkChar->GetLevel() <= 12)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 4);
			if (m_pkChar->GetLevel() >= 13 && m_pkChar->GetLevel() <= 15)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 5);
			if (m_pkChar->GetLevel() >= 16 && m_pkChar->GetLevel() <= 18)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 6);
			if (m_pkChar->GetLevel() >= 19 && m_pkChar->GetLevel() <= 21)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 7);
			if (m_pkChar->GetLevel() >= 22 && m_pkChar->GetLevel() <= 24)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 8);
			if (m_pkChar->GetLevel() >= 25 && m_pkChar->GetLevel() <= 27)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 9);
			if (m_pkChar->GetLevel() >= 28 && m_pkChar->GetLevel() <= 30)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 10);
			if (m_pkChar->GetLevel() >= 31 && m_pkChar->GetLevel() <= 33)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 11);
			if (m_pkChar->GetLevel() >= 34 && m_pkChar->GetLevel() <= 36)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 12);
			if (m_pkChar->GetLevel() >= 37 && m_pkChar->GetLevel() <= 39)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 13);
			if (m_pkChar->GetLevel() >= 40 && m_pkChar->GetLevel() <= 42)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 14);
			if (m_pkChar->GetLevel() >= 43 && m_pkChar->GetLevel() <= 45)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 15);
			if (m_pkChar->GetLevel() >= 46 && m_pkChar->GetLevel() <= 48)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 16);
			if (m_pkChar->GetLevel() >= 49 && m_pkChar->GetLevel() <= 51)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 17);
			if (m_pkChar->GetLevel() >= 52 && m_pkChar->GetLevel() <= 54)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 18);
			if (m_pkChar->GetLevel() >= 55 && m_pkChar->GetLevel() <= 57)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 19);
			if (m_pkChar->GetLevel() >= 58 && m_pkChar->GetLevel() <= 60)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 20);
			if (m_pkChar->GetLevel() >= 61 && m_pkChar->GetLevel() <= 63)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 21);
			if (m_pkChar->GetLevel() >= 64 && m_pkChar->GetLevel() <= 66)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 22);
			if (m_pkChar->GetLevel() >= 67 && m_pkChar->GetLevel() <= 69)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 23);
			if (m_pkChar->GetLevel() >= 70 && m_pkChar->GetLevel() <= 72)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 24);
			if (m_pkChar->GetLevel() >= 73 && m_pkChar->GetLevel() <= 75)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 25);
			if (m_pkChar->GetLevel() >= 76 && m_pkChar->GetLevel() <= 78)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 26);
			if (m_pkChar->GetLevel() >= 79 && m_pkChar->GetLevel() <= 81)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 27);
			if (m_pkChar->GetLevel() >= 82 && m_pkChar->GetLevel() <= 84)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 28);
			if (m_pkChar->GetLevel() >= 85 && m_pkChar->GetLevel() <= 87)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 29);
			if (m_pkChar->GetLevel() >= 88 && m_pkChar->GetLevel() <= 90)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 30);
			if (m_pkChar->GetLevel() >= 91 && m_pkChar->GetLevel() <= 93)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 31);
			if (m_pkChar->GetLevel() >= 94 && m_pkChar->GetLevel() <= 96)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 32);
			if (m_pkChar->GetLevel() >= 97 && m_pkChar->GetLevel() <= 99)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 33);
			if (m_pkChar->GetLevel() >= 100 && m_pkChar->GetLevel() <= 102)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 34);
			if (m_pkChar->GetLevel() >= 103 && m_pkChar->GetLevel() <= 105)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 35);
			if (m_pkChar->GetLevel() >= 106 && m_pkChar->GetLevel() <= 108)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 36);
			if (m_pkChar->GetLevel() >= 109 && m_pkChar->GetLevel() <= 110)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 37);
			if (m_pkChar->GetLevel() >= 111 && m_pkChar->GetLevel() <= 113)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 38);
			if (m_pkChar->GetLevel() >= 114 && m_pkChar->GetLevel() <= 116)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 39);
			if (m_pkChar->GetLevel() >= 117 && m_pkChar->GetLevel() <= 118)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 40);
			if (m_pkChar->GetLevel() >= 119 && m_pkChar->GetLevel() <= 120)
				m_pkChar->ComputeSkill(614, m_pkChar->GetOwner(), 41);
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportShamanSkillPacket(SKILL_JEUNGRYEOK);
		}
	}
}

void CSupportShamanActor::UpdateItemExp()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr)
	{
		pSummonItem->SetSocket(2, m_dwExp);
		pSummonItem->SetForceAttribute(0, 1, m_dwlevel);
		pSummonItem->SetForceAttribute(1, 1, m_dwIntSkill);
	}
}

bool CSupportShamanActor::Follow(float fMinDistance)
{
	if (!m_pkOwner || !m_pkChar)
		return false;

	float fOwnerX = m_pkOwner->GetX();
	float fOwnerY = m_pkOwner->GetY();

	float fSupportShamanX = m_pkChar->GetX();
	float fSupportShamanY = m_pkChar->GetY();

	float fDist = DISTANCE_SQRT(fOwnerX - fSupportShamanX, fOwnerY - fSupportShamanY);
	if (fDist <= fMinDistance)
		return false;

	m_pkChar->SetRotationToXY(fOwnerX, fOwnerY);

	float fx, fy;

	float fDistToGo = fDist - fMinDistance;
	GetDeltaByDegree(m_pkChar->GetRotation(), fDistToGo, &fx, &fy);

	if (!m_pkChar->Goto((int)(fSupportShamanX + fx + 0.5f), (int)(fSupportShamanY + fy + 0.5f)))
		return false;

	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0, 0);

	return true;
}

void CSupportShamanActor::SetSummonItem(LPITEM pItem)
{
	if (nullptr == pItem)
	{
		m_dwSummonItemVID = 0;
		m_dwSummonItemVnum = 0;
		return;
	}

	pItem->SetSocket(1, TRUE);
	m_dwSummonItemVID = pItem->GetVID();
	m_dwSummonItemVnum = pItem->GetVnum();
}

void CSupportShamanActor::GiveBuff()
{
	if (m_dwVnum == 34077)
	{
		if (nullptr == m_pkOwner->GetDungeon())
		{
			return;
		}
	}
	LPITEM item = ITEM_MANAGER::instance().FindByVID(m_dwSummonItemVID);
	if (nullptr != item)
		item->ModifyPoints(true);
	return;
}

void CSupportShamanActor::ClearBuff()
{
	if (nullptr == m_pkOwner)
		return;
	TItemTable * item_proto = ITEM_MANAGER::instance().GetTable(m_dwSummonItemVnum);
	if (nullptr == item_proto)
		return;
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; i++)
	{
		if (item_proto->aApplies[i].bType == APPLY_NONE)
			continue;
		m_pkOwner->ApplyPoint(item_proto->aApplies[i].bType, -item_proto->aApplies[i].lValue);
	}

	return;
}

CSupportShaman::CSupportShaman(LPCHARACTER owner)
{
	m_pkOwner = owner;
	m_dwUpdatePeriod = 400;

	m_dwLastUpdateTime = 0;
}

CSupportShaman::~CSupportShaman()
{
	Destroy();
}

void CSupportShaman::Destroy()
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;

		if (0 != SupportShamanActor)
		{
			delete SupportShamanActor;
		}
	}
	event_cancel(&m_pkSupportShamanUpdateEvent);
	m_SupportShamanActorMap.clear();
}

bool CSupportShaman::Update(DWORD deltaTime)
{
	bool bResult = true;

	DWORD currentTime = get_dword_time();

	if (m_dwUpdatePeriod > currentTime - m_dwLastUpdateTime)
		return true;

	std::vector <CSupportShamanActor*> v_garbageActor;

	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;

		if (0 != SupportShamanActor && SupportShamanActor->IsSummoned())
		{
			LPCHARACTER pSupportShaman = SupportShamanActor->GetCharacter();

			if (nullptr == CHARACTER_MANAGER::instance().Find(pSupportShaman->GetVID()))
			{
				v_garbageActor.push_back(SupportShamanActor);
			}
			else
			{
				bResult = bResult && SupportShamanActor->Update(deltaTime);
			}
		}
	}
	for (std::vector<CSupportShamanActor*>::iterator it = v_garbageActor.begin(); it != v_garbageActor.end(); it++)
		DeleteSupportShaman(*it);

	m_dwLastUpdateTime = currentTime;

	return bResult;
}

void CSupportShaman::DeleteSupportShaman(DWORD mobVnum)
{
	TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.find(mobVnum);

	if (m_SupportShamanActorMap.end() == iter)
	{
		sys_err("[CSupportShaman::DeleteSupportShaman] Can't find shaman on my list (VNUM: %d)", mobVnum);
		return;
	}

	CSupportShamanActor* SupportShamanActor = iter->second;

	if (0 == SupportShamanActor)
		sys_err("[CSupportShaman::DeleteSupportShaman] nullptr Pointer (SupportShamanActor)");
	else
		delete SupportShamanActor;

	m_SupportShamanActorMap.erase(iter);
}

void CSupportShaman::DeleteSupportShaman(CSupportShamanActor * SupportShamanActor)
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		if (iter->second == SupportShamanActor)
		{
			delete SupportShamanActor;
			m_SupportShamanActorMap.erase(iter);

			return;
		}
	}

	sys_err("[CSupportShaman::DeleteSupportShaman] Can't find SupportShamanActor(0x%x) on my list(size: %d) ", SupportShamanActor, m_SupportShamanActorMap.size());
}

void CSupportShaman::Unsummon(DWORD vnum, bool bDeleteFromList)
{
	CSupportShamanActor* actor = this->GetByVnum(vnum);

	if (0 == actor)
	{
		sys_err("[CSupportShaman::GetByVnum(%d)] nullptr Pointer (SupportShamanActor)", vnum);
		return;
	}
	actor->Unsummon();

	if (true == bDeleteFromList)
		this->DeleteSupportShaman(actor);

	bool bActive = false;
	for (TSupportShamanActorMap::iterator it = m_SupportShamanActorMap.begin(); it != m_SupportShamanActorMap.end(); it++)
	{
		bActive |= it->second->IsSummoned();
	}
	if (false == bActive)
	{
		event_cancel(&m_pkSupportShamanUpdateEvent);
		m_pkSupportShamanUpdateEvent = nullptr;
	}
}

void CSupportShaman::SetExp(int iExp)
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;
		if (SupportShamanActor != 0)
		{
			if (SupportShamanActor->IsSummoned()) {
				SupportShamanActor->SetExp(iExp);
				break;
			}
		}
	}
}

int CSupportShaman::GetLevel()
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;
		if (SupportShamanActor != 0)
		{
			if (SupportShamanActor->IsSummoned()) {
				return SupportShamanActor->GetLevel();
			}
		}
	}
	return -1;
}

int CSupportShaman::GetExp()
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;
		if (SupportShamanActor != 0)
		{
			if (SupportShamanActor->IsSummoned()) {
				return SupportShamanActor->GetExp();
			}
		}
	}
	return 0;
}
bool CSupportShaman::IsActiveSupportShaman()
{
	bool state = false;
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;
		if (SupportShamanActor != 0)
		{
			if (SupportShamanActor->IsSummoned()) {
				state = true;
				break;
			}
		}
	}
	return state;
}

CSupportShamanActor* CSupportShaman::GetActiveSupportShaman()
{
	for (TSupportShamanActorMap::iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;
		if (SupportShamanActor != 0)
		{
			if (SupportShamanActor->IsSummoned()) {
				return SupportShamanActor;
			}
		}
	}

	return nullptr;
}

CSupportShamanActor* CSupportShaman::Summon(DWORD mobVnum, LPITEM pSummonItem, const char* SupportShamanName, bool bSpawnFar, DWORD options)
{
	CSupportShamanActor* SupportShamanActor = this->GetByVnum(mobVnum);

	if (0 == SupportShamanActor)
	{
		SupportShamanActor = M2_NEW CSupportShamanActor(m_pkOwner, mobVnum, options);
		m_SupportShamanActorMap.insert(std::make_pair(mobVnum, SupportShamanActor));
	}

	DWORD SupportShamanVID = SupportShamanActor->Summon(SupportShamanName, pSummonItem, bSpawnFar);

	if (nullptr == m_pkSupportShamanUpdateEvent)
	{
		SupportShaman_event_info* info = AllocEventInfo<SupportShaman_event_info>();

		info->pSupportShaman = this;

		m_pkSupportShamanUpdateEvent = event_create(SupportShaman_update_event, info, PASSES_PER_SEC(1) / 4);	// 0.25ÃÊ
	}

	return SupportShamanActor;
}

CSupportShamanActor* CSupportShaman::GetByVID(DWORD vid) const
{
	CSupportShamanActor* SupportShamanActor = 0;

	bool bFound = false;

	for (TSupportShamanActorMap::const_iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		SupportShamanActor = iter->second;

		if (0 == SupportShamanActor)
		{
			sys_err("[CSupportShaman::GetByVID(%d)] nullptr Pointer (SupportShamanActor)", vid);
			continue;
		}

		bFound = SupportShamanActor->GetVID() == vid;

		if (true == bFound)
			break;
	}

	return bFound ? SupportShamanActor : 0;
}

CSupportShamanActor* CSupportShaman::GetByVnum(DWORD vnum) const
{
	CSupportShamanActor* SupportShamanActor = 0;

	TSupportShamanActorMap::const_iterator iter = m_SupportShamanActorMap.find(vnum);

	if (m_SupportShamanActorMap.end() != iter)
		SupportShamanActor = iter->second;

	return SupportShamanActor;
}

size_t CSupportShaman::CountSummoned() const
{
	size_t count = 0;

	for (TSupportShamanActorMap::const_iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;

		if (0 != SupportShamanActor)
		{
			if (SupportShamanActor->IsSummoned())
				++count;
		}
	}

	return count;
}

void CSupportShaman::RefreshBuff()
{
	for (TSupportShamanActorMap::const_iterator iter = m_SupportShamanActorMap.begin(); iter != m_SupportShamanActorMap.end(); ++iter)
	{
		CSupportShamanActor* SupportShamanActor = iter->second;

		if (0 != SupportShamanActor)
		{
			if (SupportShamanActor->IsSummoned())
			{
				SupportShamanActor->GiveBuff();
			}
		}
	}
}