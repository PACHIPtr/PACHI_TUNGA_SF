#include "stdafx.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "sectree_manager.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "New_PetSystem.h"
#include "../../common/VnumHelper.h"
#include "packet.h"
#include "item_manager.h"
#include "item.h"
#include "db.h"
#include "features.h"
extern int passes_per_sec;
EVENTINFO(newpetsystem_event_info)
{
	CNewPetSystem* pPetSystem;
};

EVENTINFO(newpetsystem_event_infoe)
{
	CNewPetSystem* pPetSystem;
};

// PetSystem을 update 해주는 event.
// PetSystem은 CHRACTER_MANAGER에서 기존 FSM으로 update 해주는 기존 chracters와 달리,
// Owner의 STATE를 update 할 때 _UpdateFollowAI 함수로 update 해준다.
// 그런데 owner의 state를 update를 CHRACTER_MANAGER에서 해주기 때문에,
// petsystem을 update하다가 pet을 unsummon하는 부분에서 문제가 생겼다.
// (CHRACTER_MANAGER에서 update 하면 chracter destroy가 pending되어, CPetSystem에서는 dangling 포인터를 가지고 있게 된다.)
// 따라서 PetSystem만 업데이트 해주는 event를 발생시킴.
EVENTFUNC(newpetsystem_update_event)
{
	newpetsystem_event_info* info = dynamic_cast<newpetsystem_event_info*>(event->info);
	if (info == nullptr)
	{
		sys_err("check_speedhack_event> <Factor> nullptr pointer");
		return 0;
	}

	CNewPetSystem* pPetSystem = info->pPetSystem;

	if (nullptr == pPetSystem)
		return 0;

	pPetSystem->Update(0);
	// 0.25초마다 갱신.
	return PASSES_PER_SEC(1) / 4;
}

EVENTFUNC(newpetsystem_expire_event)
{
	newpetsystem_event_infoe* info = dynamic_cast<newpetsystem_event_infoe*>(event->info);
	if (info == nullptr)
	{
		sys_err("check_speedhack_event> <Factor> nullptr pointer");
		return 0;
	}

	CNewPetSystem* pPetSystem = info->pPetSystem;

	if (nullptr == pPetSystem)
		return 0;

	pPetSystem->UpdateTime();
	// 0.25초마다 갱신.
	return PASSES_PER_SEC(1);
}

/// NOTE: 1캐릭터가 몇개의 펫을 가질 수 있는지 제한... 캐릭터마다 개수를 다르게 할거라면 변수로 넣등가... 음..
/// 가질 수 있는 개수와 동시에 소환할 수 있는 개수가 틀릴 수 있는데 이런건 기획 없으니 일단 무시
const float PET_COUNT_LIMIT = 3;

///////////////////////////////////////////////////////////////////////////////////////
//  CPetActor
///////////////////////////////////////////////////////////////////////////////////////

CNewPetActor::CNewPetActor(LPCHARACTER owner, DWORD vnum, DWORD options)
{
	m_dwVnum = vnum;
	m_dwVID = 0;
	m_dwlevel = 1;
	m_dwlevelstep = 0;
	m_dwExpFromMob = 0;
	m_dwExpFromItem = 0;
	m_dwexp = 0;
	m_dwexpitem = 0;
	m_dwOptions = options;
	m_dwLastActionTime = 0;

	m_pkChar = 0;
	m_pkOwner = owner;

	m_originalMoveSpeed = 0;

	m_dwSummonItemVID = 0;
	m_dwSummonItemID = 0;
	m_dwSummonItemVnum = 0;

	m_dwevolution = 0;
	m_dwduration = 0;
	m_dwtduration = 0;
	m_dwAgeDuration = 0;

	m_dwTimePet = 0;
	m_dwslotimm = 0;
	m_dwImmTime = 0;

	m_dwskill[0] = 0;
	m_dwskill[1] = 0;
	m_dwskill[2] = 0;

	for (int s = 0; s < 25; ++s) {
		m_dwpetslotitem[s] = -1;
	}

	//Riferimento allo slot -1 se non disp 0 disp non set > 0 setted
	m_dwskillslot[0] = -1;
	m_dwskillslot[0] = -1;
	m_dwskillslot[0] = -1;

	for (int x = 0; x < 3; ++x) //Inizializzazione bonus del pet
	{
		int btype[3] = { 1, 54, 53 };
		m_dwbonuspet[x][0] = btype[x];
		m_dwbonuspet[x][1] = 0;
	}
	
	m_dwPetType = 0;
}

CNewPetActor::~CNewPetActor()
{
	this->Unsummon();
	m_pkOwner = 0;
}

void CNewPetActor::SetName(const char* name)
{
	//std::string petName = m_pkOwner->GetName();
	std::string petName = "";

	if (0 != m_pkOwner &&
		0 == name &&
		0 != m_pkOwner->GetName())
	{
		petName += "'s Pet";
	}
	else
		petName += name;

	if (true == IsSummoned())
		m_pkChar->SetName(petName);

	m_name = petName;
}

void CNewPetActor::SetItemCube(int pos, int invpos) {
	if (m_dwpetslotitem[pos] != -1) //Controllo se l'item e' gia settato
		return;
	if (pos > 180 || pos < 0)
		return;

	m_dwpetslotitem[pos] = invpos;
}

void CNewPetActor::ItemCubeFeed(int type)
{
	for (int i = 0; i < 25; ++i) {
		if (m_dwpetslotitem[i] != -1) {
			LPITEM itemxp = m_pkOwner->GetInventoryItem(m_dwpetslotitem[i]);
			if (!itemxp)
				return;

			if (itemxp->IsBasicItem())
				return;

			if (itemxp->IsSealed())
				return;

			if (itemxp->GetID() == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetID() || m_pkOwner->GetExchange() || m_pkOwner->GetMyShop() || m_pkOwner->GetShopOwner() || m_pkOwner->IsOpenSafebox() || m_pkOwner->IsCubeOpen() || m_pkOwner->IsCombOpen() || m_pkOwner->IsCubeOpen() || m_pkOwner->isSashOpened(true) || m_pkOwner->isSashOpened(false) || m_pkOwner->isChangeLookOpened() || m_pkOwner->isAuraOpened(true) || m_pkOwner->isAuraOpened(false))
				return;
			if (type == 1)
			{
				if ((itemxp->GetVnum() >= 55401 && itemxp->GetVnum() <= 55410) || (itemxp->GetVnum() >= 55701 && itemxp->GetVnum() <= 55710) || itemxp->GetVnum() == 55001) {
					if (itemxp->GetVnum() == 55001)
					{
						int tmp_dur = m_dwtduration / 2;
						if ((m_dwduration + (tmp_dur + time(0))) > m_dwtduration)
							m_dwduration = m_dwtduration + time(0);
						else
						{
							int old_dur = time(0) - m_dwduration;
							m_dwduration = (tmp_dur + old_dur) + time(0);
						}
					}
					else
					{
						int tmp_dur = m_dwtduration * 3 / 100;
						if ((m_dwduration + (tmp_dur + time(0))) > m_dwtduration)
							m_dwduration = m_dwtduration + time(0);
						else
						{
							int old_dur = time(0) - m_dwduration;
							m_dwduration = (tmp_dur + old_dur) + time(0);
						}
						m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetDuration %d %d", m_dwduration, m_dwtduration);
					}
					ITEM_MANAGER::instance().RemoveItem(itemxp);
				}
			}
			else if (type == 3)
			{
				if (itemxp->GetType() == 1 || itemxp->GetType() == 2)
				{
					if (GetNextExpFromItem() - GetExpI() > 0)
					{
						SetExp(itemxp->GetShopBuyPrice() / 2, 1);
						ITEM_MANAGER::instance().RemoveItem(itemxp);
					}
					else
						m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_NO_MORE_ITEM_EXP"));
				}
			}
		}
	}

	for (int s = 0; s < 25; ++s) {
		m_dwpetslotitem[s] = -1;
	}
}

bool CNewPetActor::ResetSkill(int skill) {
	if (m_dwskillslot[skill] > 0)
	{
		m_dwskillslot[skill] = 0;
		m_dwskill[skill] = 0;
		return true;
	}
	return false;
}

bool CNewPetActor::IncreasePetSkill(int skill) {
	if (GetLevel() < 80 && m_dwevolution < 3)
		return false;
	for (int i = 0; i < 3; ++i) { //Itero gli slot per cercare la skill
		if (m_dwskillslot[i] == skill) {  //Se trova la skill o la aumenta oppure e' gi?max
			if (m_dwskill[i] < 20) {
				m_dwskill[i] += 1;
				m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Beceri duzeyine ulasti:%d"), m_dwskill[i]);
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", i, m_dwskillslot[i], m_dwskill[i]);
				return true;
			}
			else {
				m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Beceri zaten maksimum seviyeye ulasti!"));
				return false;
			}
		}
	}

	for (int i = 0; i < 3; ++i) {
		if (m_dwskillslot[i] == 0) { //Controllo se trovo uno slot vuoto abilitato
			m_dwskillslot[i] = skill;
			m_dwskill[i] = 1;
			m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Evcil hayvaniniz yeni bir beceri ogrendi!"));
			m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", i, m_dwskillslot[i], m_dwskill[i]);
			return true;
		}
	}

	/* Qualora il pet non soddisfi le condizioni precedenti
	   Allora tutti gli slot sono pieni e quind non pu?
	   imparare nuove skill
	*/
	m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Evcil hayvaninizin diger becerileri ogrenmek mumkun degil!"));
	return false;
}

bool CNewPetActor::IncreasePetEvolution() {
	if (m_dwevolution < 3) {
		if ((GetLevel() == 40 && m_dwevolution < 1) || (GetLevel() == 60 && m_dwevolution < 2) || (GetLevel() == 80 && m_dwevolution < 3)) {
			m_dwevolution += 1;
			m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetEvolution %d", m_dwevolution);
			if (m_dwevolution == 3) {
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 2, m_dwskillslot[2], m_dwskill[2]);
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 2, m_dwskillslot[2], m_dwskill[2]);
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 2, m_dwskillslot[2], m_dwskill[2]);
			}
		}
		else
			return false;
	}
	else {
		m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Evcil hayvan maksimum evrime ulasti!"));
		return false;
	}
	return true;
}

void CNewPetActor::IncreasePetBonus() {
	int tmplevel = GetLevel();
	if (m_dwPetType == 0)
	{
		int lvbonus = 0;
		if (tmplevel >= 60)
			lvbonus = 1;
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[0][1] += number(1+lvbonus, 5+lvbonus);
			m_dwbonuspet[2][1] += number(1+lvbonus, 5+lvbonus);
		}
		
		if (tmplevel % 8 == 0)
		{
			m_dwbonuspet[1][1] += number(1+lvbonus, 5+lvbonus);
		}
	}
	else if (m_dwPetType == 1) // sabit
	{
		int lvbonus = 0;
		if (GetAge() >= 60)
			lvbonus = 1;
		if (tmplevel % 5 == 0)
		{
			m_dwbonuspet[0][1] += number(2+lvbonus, 3+lvbonus);
			m_dwbonuspet[2][1] += number(2+lvbonus, 3+lvbonus);
		}
		
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[1][1] += number(2+lvbonus, 3+lvbonus);
		}
	}
	else if (m_dwPetType == 2)
	{
		int lvbonus = 0;
		if (tmplevel >= 60)
			lvbonus = 1;
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[0][1] += number(1+lvbonus, 7+lvbonus);
			m_dwbonuspet[2][1] += number(1+lvbonus, 7+lvbonus);
		}
		
		if (tmplevel % 8 == 0)
		{
			m_dwbonuspet[1][1] += number(1+lvbonus, 7+lvbonus);
		}
	}
	else if (m_dwPetType == 3) // sabit
	{
		int lvbonus = 0;
		if (GetAge() >= 60)
			lvbonus = 1;
		if (tmplevel % 5 == 0)
		{
			m_dwbonuspet[0][1] += number(3+lvbonus, 4+lvbonus);
			m_dwbonuspet[2][1] += number(3+lvbonus, 4+lvbonus);
		}
		
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[1][1] += number(3+lvbonus, 4+lvbonus);
		}
	}
	else if (m_dwPetType == 4)
	{
		int lvbonus = 0;
		if (tmplevel >= 60)
			lvbonus = 1;
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[0][1] += number(1+lvbonus, 9+lvbonus);
			m_dwbonuspet[2][1] += number(1+lvbonus, 9+lvbonus);
		}
		
		if (tmplevel % 8 == 0)
		{
			m_dwbonuspet[1][1] += number(1+lvbonus, 9+lvbonus);
		}
	}
	else if (m_dwPetType == 5) // sabit
	{
		int lvbonus = 0;
		if (GetAge() >= 60)
			lvbonus = 1;
		if (tmplevel % 5 == 0)
		{
			m_dwbonuspet[0][1] += number(5+lvbonus, 6+lvbonus);
			m_dwbonuspet[2][1] += number(5+lvbonus, 6+lvbonus);
		}
		
		if (tmplevel % 6 == 0)
		{
			m_dwbonuspet[1][1] += number(5+lvbonus, 6+lvbonus);
		}
	}
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetBonus %d %d %d", m_dwbonuspet[0][1], m_dwbonuspet[1][1], m_dwbonuspet[2][1]);
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr) {
		for (int b = 0; b < 3; b++) {
			pSummonItem->SetForceAttribute(b, 1, m_dwbonuspet[b][1]);
		}
	}
}

void CNewPetActor::SetNextExp(int nextExp)
{
	m_dwExpFromMob = (nextExp / 10) * 9;
	m_dwExpFromItem = nextExp - m_dwExpFromMob;
}

void CNewPetActor::SetLevel(DWORD level)
{
	if (m_pkChar->GetLevel() >= 120)
		return;
	m_pkChar->SetLevel(static_cast<char>(level));
	m_dwlevel = level;
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem != nullptr) { pSummonItem->SetSocket(1, level); }
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetLevel %d", m_dwlevel);
	SetNextExp(m_pkChar->PetGetNextExp());
	m_pkChar->ViewReencode();
}

void CNewPetActor::SetEvolution(int lv) {
	if (lv == 40)
		m_dwevolution = 1;
	else if (lv == 60)
		m_dwevolution = 2;
	else if (lv == 80)
		m_dwevolution = 3;
}

void CNewPetActor::SetExp(DWORD exp, int mode)
{
	if (m_pkChar->GetLevel() >= 120)
		return;
	if (exp < 0)
		exp = MAX(m_dwexp - exp, 0);
	if (mode == 0) {
		if (GetExp() + exp >= GetNextExpFromMob() && GetExpI() >= GetNextExpFromItem())
		{
			if (GetEvolution() == 0 && GetLevel() == 40)
				return;
			else if (GetEvolution() == 1 && GetLevel() == 60)
				return;
			else if (GetEvolution() == 2 && GetLevel() == 80)
				return;
		}
	}
	else if (mode == 1)
	{
		if (GetExpI() + exp >= GetNextExpFromItem() && GetExp() >= GetNextExpFromMob())
		{
			if (GetEvolution() == 0 && GetLevel() == 40)
				return;
			else if (GetEvolution() == 1 && GetLevel() == 60)
				return;
			else if (GetEvolution() == 2 && GetLevel() == 80)
				return;
		}
	}

	if (mode == 0) {
		if (GetExp() + exp >= GetNextExpFromMob()) {
			if (GetExpI() >= GetNextExpFromItem()) {
				SetLevel(GetLevel() + 1);
				//m_pkChar->UpdatePacket();
				m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 1, GetLevel(), 1);
				IncreasePetBonus();
				m_dwlevelstep = 0;
				m_dwexp = 0;
				m_dwexpitem = 0;
				m_pkChar->SetExp(0);
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
				//SetEvolution(GetLevel());
				return;
			}
			else {
				//m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 25, GetLevel(), 1);
				m_dwlevelstep = 4;
				exp = GetNextExpFromMob() - GetExp();
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
			}
		}
		m_dwexp += exp;
		m_pkChar->SetExp(m_dwexp);
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
		if (GetLevelStep() < 4) {
			if (GetExp() >= GetNextExpFromMob() / 4 * 3 && m_dwlevelstep != 3) {
				m_dwlevelstep = 3;
				//m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 25, GetLevel(), 1);
			}
			else if (GetExp() >= GetNextExpFromMob() / 4 * 2 && m_dwlevelstep != 2) {
				m_dwlevelstep = 2;
				//m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 25, GetLevel(), 1);
			}
			else if (GetExp() >= GetNextExpFromMob() / 4 && m_dwlevelstep != 1) {
				m_dwlevelstep = 1;
				//m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 25, GetLevel(), 1);
			}
		}
	}
	else if (mode == 1) {
		if (GetExpI() + exp >= GetNextExpFromItem()) {
			if (GetExp() >= GetNextExpFromMob()) { //Set anche exp da mob e' piena livello e conservo exp restante
				m_dwexpitem = GetExpI() + exp - GetNextExpFromItem(); //Nel caso in cui livelli setta exp in eccesso 0 se ==
				m_dwexp = 0;
				m_pkChar->SetExp(0);
				m_dwlevelstep = 0;
				SetLevel(GetLevel() + 1);
				//m_pkChar->UpdatePacket();
				m_pkChar->SendPetLevelUpEffect(m_pkChar->GetVID(), 1, GetLevel(), 1);
				IncreasePetBonus();
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
				//SetEvolution(GetLevel());
				return;
				if (GetExpI() > GetNextExpFromItem()) { //Controllo che in caso exp avanzata sia superiore al livello successivo venga solo maxata ma non superi il limite
					m_dwexpitem = GetNextExpFromItem(); //setto exp come massima ma non >
					m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
				}
			}
			else {
				exp = GetNextExpFromItem() - GetExpI();
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
			}
		}
		m_dwexpitem += exp;
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
	}
}

bool CNewPetActor::Mount()
{
	if (0 == m_pkOwner)
		return false;

	if (true == HasOption(EPetOption_Mountable))
		m_pkOwner->MountVnum(m_dwVnum);

	return m_pkOwner->GetMountVnum() == m_dwVnum;;
}

void CNewPetActor::UpdateTime()
{
	m_dwTimePet += 1;
	if (m_dwTimePet >= 60) {
		m_dwduration -= 1;
		m_dwTimePet = 0;
		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
		if (pSummonItem != nullptr) {
			pSummonItem->SetSocket(2, m_dwduration);
			pSummonItem->SetForceAttribute(4, 1, m_dwtduration);
			pSummonItem->SetForceAttribute(13, 1, GetSkillCount());
		}
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetDuration %d %d", m_dwduration, m_dwtduration);
	}
}

void CNewPetActor::Unmount()
{
	if (0 == m_pkOwner)
		return;

	if (m_pkOwner->IsHorseRiding())
		m_pkOwner->StopRiding();
}

void CNewPetActor::Unsummon()
{
	if (true == this->IsSummoned())
	{
		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
		if (pSummonItem != nullptr) {
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET level = %d, evolution=%d, exp=%d, expi=%d, bonus0=%d, bonus1=%d, bonus2=%d, skill0=%d, skill0lv= %d, skill1=%d, skill1lv= %d, skill2=%d, skill2lv= %d, duration=%d, tduration=%d WHERE id = %lu ", this->GetLevel(), this->m_dwevolution, this->GetExp(), this->GetExpI(), this->m_dwbonuspet[0][1], this->m_dwbonuspet[1][1], this->m_dwbonuspet[2][1], this->m_dwskillslot[0], this->m_dwskill[0], this->m_dwskillslot[1], this->m_dwskill[1], this->m_dwskillslot[2], this->m_dwskill[2], this->m_dwduration, this->m_dwtduration, ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetID()));
			this->ClearBuff();
			for (int b = 0; b < 3; b++) {
				pSummonItem->SetForceAttribute(b, 1, m_dwbonuspet[b][1]);
			}
			pSummonItem->SetForceAttribute(4, 1, m_dwtduration);
			pSummonItem->SetForceAttribute(6, 1, m_dwevolution);
			pSummonItem->SetForceAttribute(7, 1, m_dwskillslot[0]);
			pSummonItem->SetForceAttribute(8, 1, m_dwskill[0]);
			pSummonItem->SetForceAttribute(9, 1, m_dwskillslot[1]);
			pSummonItem->SetForceAttribute(10, 1, m_dwskill[1]);
			pSummonItem->SetForceAttribute(11, 1, m_dwskillslot[2]);
			pSummonItem->SetForceAttribute(12, 1, m_dwskill[2]);
			pSummonItem->SetForceAttribute(13, 1, GetSkillCount());
			pSummonItem->SetSocket(1, m_dwlevel);
			pSummonItem->SetSocket(2, m_dwduration);
			pSummonItem->SetSocket(0, false);
			pSummonItem->Lock(false);
		}
		this->SetSummonItem(nullptr);

		if (nullptr != m_pkOwner)
			m_pkOwner->ComputePoints();

		if (nullptr != m_pkChar)
			M2_DESTROY_CHARACTER(m_pkChar);

		m_pkChar = 0;
		m_dwVID = 0;
		m_dwlevel = 1;
		m_dwlevelstep = 0;
		m_dwExpFromMob = 0;
		m_dwExpFromItem = 0;
		m_dwexp = 0;
		m_dwexpitem = 0;
		m_dwTimePet = 0;
		m_dwImmTime = 0;
		m_dwslotimm = 0;

		for (int s = 0; s < 25; ++s) {
			m_dwpetslotitem[s] = -1;
		}
		ClearBuff();
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetUnsummon");
	}
}

DWORD CNewPetActor::Summon(const char* petName, LPITEM pSummonItem, bool bSpawnFar)
{
	if (!features::IsFeatureEnabled(features::PET))
	{
		m_pkOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return 0;		
	}
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
	
	m_pkChar = CHARACTER_MANAGER::instance().SpawnMob(
		m_dwVnum,
		m_pkOwner->GetMapIndex(),
		x, y, z,
		false, (int)(m_pkOwner->GetRotation() + 180), false);

	if (0 == m_pkChar)
	{
		sys_err("[CPetSystem::Summon] Failed to summon the pet. (vnum: %d)", m_dwVnum);
		return 0;
	}

	m_pkChar->SetNewPet();

	//	m_pkOwner->DetailLog();
	//	m_pkChar->DetailLog();

		//펫의 국가를 주인의 국가로 설정함.
	m_pkChar->SetEmpire(m_pkOwner->GetEmpire());

	m_dwVID = m_pkChar->GetVID();

	char szQuery1[1024];
	snprintf(szQuery1, sizeof(szQuery1), "SELECT name,level,exp,expi,bonus0,bonus1,bonus2,skill0,skill0lv,skill1,skill1lv,skill2,skill2lv,duration,tduration,evolution,ageduration,pet_type FROM new_petsystem WHERE id = %u ", pSummonItem->GetID());
	std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
	if (pmsg2->Get()->uiNumRows > 0) {
		MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
		this->SetName(row[0]);
		this->SetLevel(atoi(row[1]));
		this->m_dwexp = atoi(row[2]);
		this->m_dwexpitem = atoi(row[3]);
		this->m_dwbonuspet[0][1] = atoi(row[4]);
		this->m_dwbonuspet[1][1] = atoi(row[5]);
		this->m_dwbonuspet[2][1] = atoi(row[6]);
		this->m_dwskillslot[0] = atoi(row[7]);
		this->m_dwskill[0] = atoi(row[8]);
		this->m_dwskillslot[1] = atoi(row[9]);
		this->m_dwskill[1] = atoi(row[10]);
		this->m_dwskillslot[2] = atoi(row[11]);
		this->m_dwskill[2] = atoi(row[12]);
		this->m_dwduration = atoi(row[13]);
		this->m_dwtduration = atoi(row[14]);
		this->m_dwevolution = atoi(row[15]);
		this->m_dwAgeDuration = atoi(row[16]);
		this->m_dwPetType = atoi(row[17]);
	}
	else
		this->SetName(petName);

	// SetSummonItem(pSummonItem)를 부른 후에 ComputePoints를 부르면 버프 적용됨.
	this->SetSummonItem(pSummonItem);

	//this->SetNextExp(m_pkChar->PetGetNextExp());
	m_pkOwner->ComputePoints();
	m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y, z);

	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetIcon %d", m_dwSummonItemVnum);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetEvolution %d", m_dwevolution);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetName %s", m_name.c_str());
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetLevel %d", m_dwlevel);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetDuration %d %d", m_dwduration, m_dwtduration);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetAgeDuration %d", m_dwAgeDuration);
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetBonus %d %d %d", m_dwbonuspet[0][1], m_dwbonuspet[1][1], m_dwbonuspet[2][1]);
	if (GetLevel() >= 80 && m_dwevolution == 3) {
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 0, m_dwskillslot[0], m_dwskill[0]);
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 1, m_dwskillslot[1], m_dwskill[1]);
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 2, m_dwskillslot[2], m_dwskill[2]);
	}
	else {
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 0, -1, m_dwskill[0]);
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 1, -1, m_dwskill[1]);
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetSkill %d %d %d", 2, -1, m_dwskill[2]);
	}
	m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "PetExp %d %d %d", m_dwexp, m_dwexpitem, m_pkChar->PetGetNextExp());
	this->GiveBuff();

	for (int b = 0; b < 3; b++) {
		pSummonItem->SetForceAttribute(b, 1, m_dwbonuspet[b][1]);
	}
	pSummonItem->SetForceAttribute(4, 1, m_dwtduration);
	pSummonItem->SetForceAttribute(6, 1, m_dwevolution);
	pSummonItem->SetForceAttribute(7, 1, m_dwskillslot[0]);
	pSummonItem->SetForceAttribute(8, 1, m_dwskill[0]);
	pSummonItem->SetForceAttribute(9, 1, m_dwskillslot[1]);
	pSummonItem->SetForceAttribute(10, 1, m_dwskill[1]);
	pSummonItem->SetForceAttribute(11, 1, m_dwskillslot[2]);
	pSummonItem->SetForceAttribute(12, 1, m_dwskill[2]);
	pSummonItem->SetSocket(3, m_dwAgeDuration);
	pSummonItem->SetSocket(2, m_dwduration);
	pSummonItem->SetSocket(1, m_dwlevel);
	pSummonItem->SetForceAttribute(14, 1, m_dwPetType);
	//m_pkOwner->AddAffect( AFFECT_AUTO_SP_RECOVERY, POINT_NONE, 4, pSummonItem->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);
	pSummonItem->SetSocket(0, true);
	pSummonItem->Lock(true);
	return m_dwVID;
}

bool CNewPetActor::_UpdatAloneActionAI(float fMinDist, float fMaxDist)
{
	float fDist = number(fMinDist, fMaxDist);
	float r = (float)number(0, 359);
	float dest_x = GetOwner()->GetX() + fDist * cos(r);
	float dest_y = GetOwner()->GetY() + fDist * sin(r);

	//m_pkChar->SetRotation(number(0, 359));        // 방향은 랜덤으로 설정

	//GetDeltaByDegree(m_pkChar->GetRotation(), fDist, &fx, &fy);

	// 느슨한 못감 속성 체크; 최종 위치와 중간 위치가 갈수없다면 가지 않는다.
	//if (!(SECTREE_MANAGER::instance().IsMovablePosition(m_pkChar->GetMapIndex(), m_pkChar->GetX() + (int) fx, m_pkChar->GetY() + (int) fy)
	//			&& SECTREE_MANAGER::instance().IsMovablePosition(m_pkChar->GetMapIndex(), m_pkChar->GetX() + (int) fx/2, m_pkChar->GetY() + (int) fy/2)))
	//	return true;

	m_pkChar->SetNowWalking(true);

	//if (m_pkChar->Goto(m_pkChar->GetX() + (int) fx, m_pkChar->GetY() + (int) fy))
	//	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	if (!m_pkChar->IsStateMove() && m_pkChar->Goto(dest_x, dest_y))
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	m_dwLastActionTime = get_dword_time();

	return true;
}

// char_state.cpp StateHorse함수 그냥 C&P -_-;
bool CNewPetActor::_UpdateFollowAI()
{
	if (0 == m_pkChar->m_pkMobData)
	{
		//sys_err("[CPetActor::_UpdateFollowAI] m_pkChar->m_pkMobData is nullptr");
		return false;
	}

	if (!m_pkOwner)
		return false;

	// NOTE: 캐릭터(펫)의 원래 이동 속도를 알아야 하는데, 해당 값(m_pkChar->m_pkMobData->m_table.sMovingSpeed)을 직접적으로 접근해서 알아낼 수도 있지만
	// m_pkChar->m_pkMobData 값이 invalid한 경우가 자주 발생함. 현재 시간관계상 원인은 다음에 파악하고 일단은 m_pkChar->m_pkMobData 값을 아예 사용하지 않도록 함.
	// 여기서 매번 검사하는 이유는 최초 초기화 할 때 정상 값을 제대로 못얻어오는 경우도 있음.. -_-;; ㅠㅠㅠㅠㅠㅠㅠㅠㅠ
	if (0 == m_originalMoveSpeed)
	{
		const CMob* mobData = CMobManager::Instance().Get(m_dwVnum);

		if (0 != mobData)
			m_originalMoveSpeed = mobData->m_table.sMovingSpeed;
	}
	float	START_FOLLOW_DISTANCE = 300.0f;		// 이 거리 이상 떨어지면 쫓아가기 시작함
	float	START_RUN_DISTANCE = 900.0f;		// 이 거리 이상 떨어지면 뛰어서 쫓아감.

	float	RESPAWN_DISTANCE = 4500.f;			// 이 거리 이상 멀어지면 주인 옆으로 소환함.
	int		APPROACH = 290;						// 접근 거리

	//bool bDoMoveAlone = true;					// 캐릭터와 가까이 있을 때 혼자 여기저기 움직일건지 여부 -_-;
	bool bRun = false;							// 뛰어야 하나?

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

		m_pkChar->SetNowWalking(!bRun);		// NOTE: 함수 이름보고 멈추는건줄 알았는데 SetNowWalking(false) 하면 뛰는거임.. -_-;

		Follow(APPROACH);

		m_pkChar->SetLastAttacked(currentTime);
		m_dwLastActionTime = currentTime;
	}
	//else
	//{
	//	if (fabs(m_pkChar->GetRotation() - GetDegreeFromPositionXY(charX, charY, ownerX, ownerX)) > 10.f || fabs(m_pkChar->GetRotation() - GetDegreeFromPositionXY(charX, charY, ownerX, ownerX)) < 350.f)
	//	{
	//		m_pkChar->Follow(m_pkOwner, APPROACH);
	//		m_pkChar->SetLastAttacked(currentTime);
	//		m_dwLastActionTime = currentTime;
	//	}
	//}
	// Follow 중이지만 주인과 일정 거리 이내로 가까워졌다면 멈춤
	else
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	//else if (currentTime - m_dwLastActionTime > number(5000, 12000))
	//{
	//	this->_UpdatAloneActionAI(START_FOLLOW_DISTANCE / 2, START_FOLLOW_DISTANCE);
	//}

	return true;
}

bool CNewPetActor::Update(DWORD deltaTime)
{
	bool bResult = true;

	// 펫 주인이 죽었거나, 소환된 펫의 상태가 이상하다면 펫을 없앰. (NOTE: 가끔가다 이런 저런 이유로 소환된 펫이 DEAD 상태에 빠지는 경우가 있음-_-;)
	// 펫을 소환한 아이템이 없거나, 내가 가진 상태가 아니라면 펫을 없앰.
	if (IsSummoned()) {
		if (m_pkOwner->IsImmortal() && Pet_Skill_Table[16][2 + m_dwskill[m_dwslotimm]] <= (get_global_time() - m_dwImmTime) * 10) {
			//m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "%d - %d  diff %d  Skilltable %d", get_global_time(), m_dwImmTime, (get_global_time() - m_dwImmTime) * 10, Pet_Skill_Table[16][2 + m_dwskill[m_dwslotimm]]);
			m_dwImmTime = 0;
			m_pkOwner->SetImmortal(0);
		}
	}
	if (m_pkOwner->IsDead() || (IsSummoned() && m_pkChar->IsDead()) || (IsSummoned() && (m_pkOwner->GetExchange() || m_pkOwner->GetShop() || m_pkOwner->GetMyShop() || m_pkOwner->GetShopOwner() || m_pkOwner->IsOpenSafebox() || m_pkOwner->IsCubeOpen() || m_dwduration <= 0 || m_dwduration <= get_global_time()))
		|| nullptr == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())
		|| ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetOwner() != this->GetOwner()
		)
	{
		this->Unsummon();
		return true;
	}

	if (this->IsSummoned() && HasOption(EPetOption_Followable))
		bResult = bResult && this->_UpdateFollowAI();

	return bResult;
}

//NOTE : 주의!!! MinDistance를 크게 잡으면 그 변위만큼의 변화동안은 follow하지 않는다,
bool CNewPetActor::Follow(float fMinDistance)
{
	// 가려는 위치를 바라봐야 한다.
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

void CNewPetActor::SetSummonItem(LPITEM pItem)
{
	if (nullptr == pItem)
	{
		m_dwSummonItemVID = 0;
		m_dwSummonItemID = 0;
		m_dwSummonItemVnum = 0;
		return;
	}

	m_dwSummonItemVID = pItem->GetVID();
	m_dwSummonItemID = pItem->GetID();
	m_dwSummonItemVnum = pItem->GetVnum();
}

void CNewPetActor::GiveBuff()
{
	//Inizializzo i bonus del NewPetSystem //hp sp e def
	// 559 Affect NewPet
	int cbonus[3] = { m_pkOwner->GetMaxHP(),  m_pkOwner->GetPoint(POINT_DEF_GRADE), m_pkOwner->GetPoint(POINT_ATT_GRADE) };
	for (int i = 0; i < 3; ++i) {
		m_pkOwner->AddAffect(AFFECT_NEW_PET, aApplyInfo[m_dwbonuspet[i][0]].bPointType, (int)(cbonus[i] * m_dwbonuspet[i][1] / 1000), 0, 60 * 60 * 24 * 365, 0, false);
	}

	//Inizializzo le skill del pet inattive  No 10-17-18 No 0 no -1
	//Condizione lv > 81 evo 3 Solo Skill Passive
	if (GetLevel() > 81 && m_dwevolution == 3) {
		for (int s = 0; s < 3; s++) {
			switch (m_dwskillslot[s]) {
				/*
					Pet_Skill_Table[m_dwskillslot[s] - 1][0]; //Mi ritorna il type della skill
					Pet_Skill_Table[m_dwskillslot[s] - 1][1]; // Mi ritorna attiva/passiva della skill
					Pet_Skill_Table[m_dwskillslot[s] - 1][2]; // Mi ritorna il cd della skill
					Pet_Skill_Table[m_dwskillslot[s] - 1][2 + m_dwskill[s]]; //Mi ritorna l'apply della skill
				*/
			case 1: //Resistenza Guerriero 78 Punti
			case 2: //Resistenza Sura 80
			case 3: //Resistenza Ninja 79
			case 4: //Resistenza Shamani 81
			case 5: //Resistenza Lycan 93
			case 6: //Valore Attacco 53 Punti
			case 7: //Antimagia 97
			case 8: //Velocit?Magia %
			case 9: //Perforazione 16 Punti
			case 11: //Sotto Hp Avv  Punti
			case 12: //Sotto MP Avv Punti
			case 13: // Blocco corp 27 Punti
			case 14: // Riflessione 28 Punti
			case 15: // Drop Yang 44 Punti
			case 16: // Portata Arco 172 Punti
			case 19: // metin avcisi
			case 20: // canavar avcisi
				m_pkOwner->AddAffect(AFFECT_NEW_PET, aApplyInfo[Pet_Skill_Table[m_dwskillslot[s] - 1][0]].bPointType, Pet_Skill_Table[m_dwskillslot[s] - 1][2 + m_dwskill[s]], 0, 60 * 60 * 24 * 365, 0, false);
				break;
			default:
				return;
			}
		}
	}
}

void CNewPetActor::ClearBuff()
{
	m_pkOwner->RemoveAffect(AFFECT_NEW_PET);
	return;
}

void CNewPetActor::DoPetSkill(int skillslot) {
	if (GetLevel() < 82 || m_dwevolution < 3)
		return;
	switch (m_dwskillslot[skillslot]) {
	case 10:
	{
		if (get_global_time() - m_pkOwner->GetNewPetSkillCD(0) <= 480) {
			m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "Yetenegini yeniden kullanmak icin !%ds kaldi", 480 - (get_global_time() - m_pkOwner->GetNewPetSkillCD(0)));
			return;
		}
		if (m_pkOwner->GetHPPct() > 20) {
			m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "Sadece bu beceriyi kullanabilirsiniz <= 20%!");
			return;
		}
		m_pkOwner->SetNewPetSkillCD(0, get_global_time());
		int riphp = MIN(m_pkOwner->GetHP() + (int)Pet_Skill_Table[9][2 + m_dwskill[skillslot]], m_pkOwner->GetMaxHP());
		m_pkOwner->PointChange(POINT_HP, riphp);
		m_pkOwner->EffectPacket(SE_HPUP_RED);
	}
	break;

	case 17:
	{
		if (get_global_time() - m_pkOwner->GetNewPetSkillCD(1) <= 600) {
			m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "Yetenegini yeniden kullanmak icin !%ds kaldi", 600 - (get_global_time() - m_pkOwner->GetNewPetSkillCD(1)));
			return;
		}
		m_pkOwner->SetNewPetSkillCD(1, get_global_time());
		m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "Olumsuz olmak var!");
		m_pkOwner->SetImmortal(1);
		m_dwslotimm = skillslot;
		m_dwImmTime = get_global_time();
	}
	break;
	case 18:
	{
		if (get_global_time() - m_pkOwner->GetNewPetSkillCD(2) <= 480) {
			m_pkOwner->ChatPacket(CHAT_TYPE_INFO, "Yetenegini yeniden kullanmak icin !%ds kaldi", 480 - (get_global_time() - m_pkOwner->GetNewPetSkillCD(2)));
			return;
		}
		m_pkOwner->SetNewPetSkillCD(2, get_global_time());
		m_pkOwner->RemoveBadAffect();
	}
	break;

	default:
		return;
	}
}

int CNewPetActor::GetSkillCount() {
	int skillCount = 0;
	for (int i = 0; i < 3; i++) {
		if (m_dwskillslot[i] >= 0)
			++skillCount;
	}
	return skillCount;
}

///////////////////////////////////////////////////////////////////////////////////////
//  CPetSystem
///////////////////////////////////////////////////////////////////////////////////////

CNewPetSystem::CNewPetSystem(LPCHARACTER owner)
{
	//	assert(0 != owner && "[CPetSystem::CPetSystem] Invalid owner");

	m_pkOwner = owner;
	m_dwUpdatePeriod = 400;

	m_dwLastUpdateTime = 0;
}

CNewPetSystem::~CNewPetSystem()
{
	Destroy();
}

bool  CNewPetSystem::IncreasePetSkill(int skill) {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->IncreasePetSkill(skill);
			}
		}
	}
	return false;
}

bool CNewPetSystem::ResetSkill(int skill) {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->ResetSkill(skill);
			}
		}
	}
	return false;
}

bool  CNewPetSystem::IncreasePetEvolution() {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->IncreasePetEvolution();
			}
		}
	}
	return false;
}

void CNewPetSystem::Destroy()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			delete petActor;
		}
	}
	event_cancel(&m_pkNewPetSystemUpdateEvent);
	event_cancel(&m_pkNewPetSystemExpireEvent);
	m_petActorMap.clear();
}

void CNewPetSystem::UpdateTime()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;

		if (0 != petActor && petActor->IsSummoned())
		{
			petActor->UpdateTime();
		}
	}
}

/// 펫 시스템 업데이트. 등록된 펫들의 AI 처리 등을 함.
bool CNewPetSystem::Update(DWORD deltaTime)
{
	bool bResult = true;

	DWORD currentTime = get_dword_time();

	// CHARACTER_MANAGER에서 캐릭터류 Update할 때 매개변수로 주는 (Pulse라고 되어있는)값이 이전 프레임과의 시간차이인줄 알았는데
	// 전혀 다른 값이라서-_-; 여기에 입력으로 들어오는 deltaTime은 의미가 없음ㅠㅠ

	if (m_dwUpdatePeriod > currentTime - m_dwLastUpdateTime)
		return true;

	std::vector <CNewPetActor*> v_garbageActor;

	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;

		if (0 != petActor && petActor->IsSummoned())
		{
			LPCHARACTER pPet = petActor->GetCharacter();

			if (nullptr == CHARACTER_MANAGER::instance().Find(pPet->GetVID()))
			{
				v_garbageActor.push_back(petActor);
			}
			else
			{
				bResult = bResult && petActor->Update(deltaTime);
			}
		}
	}
	for (std::vector<CNewPetActor*>::iterator it = v_garbageActor.begin(); it != v_garbageActor.end(); it++)
		DeletePet(*it);

	m_dwLastUpdateTime = currentTime;

	return bResult;
}

/// 관리 목록에서 펫을 지움
void CNewPetSystem::DeletePet(DWORD mobVnum)
{
	TNewPetActorMap::iterator iter = m_petActorMap.find(mobVnum);

	if (m_petActorMap.end() == iter)
	{
		sys_err("[CPetSystem::DeletePet] Can't find pet on my list (VNUM: %d)", mobVnum);
		return;
	}

	CNewPetActor* petActor = iter->second;

	if (0 == petActor)
		sys_err("[CPetSystem::DeletePet] nullptr Pointer (petActor)");
	else
		delete petActor;

	m_petActorMap.erase(iter);
}

/// 관리 목록에서 펫을 지움
void CNewPetSystem::DeletePet(CNewPetActor * petActor)
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		if (iter->second == petActor)
		{
			delete petActor;
			m_petActorMap.erase(iter);

			return;
		}
	}

	sys_err("[CPetSystem::DeletePet] Can't find petActor(0x%x) on my list(size: %d) ", petActor, m_petActorMap.size());
}

void CNewPetSystem::Unsummon(DWORD vnum, bool bDeleteFromList)
{
	CNewPetActor* actor = this->GetByVnum(vnum);

	if (0 == actor)
	{
		sys_err("[CPetSystem::GetByVnum(%d)] nullptr Pointer (petActor)", vnum);
		return;
	}
	actor->Unsummon();

	if (true == bDeleteFromList)
		this->DeletePet(actor);

	bool bActive = false;
	for (TNewPetActorMap::iterator it = m_petActorMap.begin(); it != m_petActorMap.end(); it++)
	{
		bActive |= it->second->IsSummoned();
	}
	if (false == bActive)
	{
		event_cancel(&m_pkNewPetSystemUpdateEvent);
		event_cancel(&m_pkNewPetSystemExpireEvent);
		m_pkNewPetSystemUpdateEvent = nullptr;
		m_pkNewPetSystemExpireEvent = nullptr;
	}
}

DWORD CNewPetSystem::GetNewPetITemID()
{
	DWORD itemid = 0;
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				itemid = petActor->GetSummonItemID();
				break;
			}
		}
	}
	return itemid;
}

bool CNewPetSystem::IsActivePet()
{
	bool state = false;
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				state = true;
				break;
			}
		}
	}
	return state;
}

int CNewPetSystem::GetLevelStep()
{
	int step = 4;
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				step = petActor->GetLevelStep();
				break;
			}
		}
	}
	return step;
}

void CNewPetSystem::SetExp(int iExp, int mode)
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				petActor->SetExp(iExp, mode);
				break;
			}
		}
	}
}

int CNewPetSystem::GetEvolution()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->GetEvolution();
			}
		}
	}
	return -1;
}

int CNewPetSystem::GetLevel()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->GetLevel();
			}
		}
	}
	return -1;
}

int CNewPetSystem::GetExp()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->GetExp();
			}
		}
	}
	return 0;
}

int CNewPetSystem::GetAge()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned())
			{
				int tmpage = (time(0) + petActor->GetAge()) / 86400;
				return tmpage;
			}
		}
	}
	return -1;
}

int CNewPetSystem::GetPetType()
{
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned())
			{
				return petActor->GetPetType();
			}
		}
	}
	return -1;
}

void CNewPetSystem::SetItemCube(int pos, int invpos) {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->SetItemCube(pos, invpos);
			}
		}
	}
}

void CNewPetSystem::ItemCubeFeed(int type) {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->ItemCubeFeed(type);
			}
		}
	}
}

void CNewPetSystem::DoPetSkill(int skillslot) {
	for (TNewPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;
		if (petActor != 0)
		{
			if (petActor->IsSummoned()) {
				return petActor->DoPetSkill(skillslot);
			}
		}
	}
}

CNewPetActor* CNewPetSystem::Summon(DWORD mobVnum, LPITEM pSummonItem, const char* petName, bool bSpawnFar, DWORD options)
{
	CNewPetActor* petActor = this->GetByVnum(mobVnum);

	// 등록된 펫이 아니라면 새로 생성 후 관리 목록에 등록함.
	if (0 == petActor)
	{
		petActor = M2_NEW CNewPetActor(m_pkOwner, mobVnum, options);
		m_petActorMap.insert(std::make_pair(mobVnum, petActor));
	}

	petActor->Summon(petName, pSummonItem, bSpawnFar);

	if (nullptr == m_pkNewPetSystemUpdateEvent)
	{
		newpetsystem_event_info* info = AllocEventInfo<newpetsystem_event_info>();

		info->pPetSystem = this;

		m_pkNewPetSystemUpdateEvent = event_create(newpetsystem_update_event, info, PASSES_PER_SEC(1) / 4);	// 0.25초
	}

	if (nullptr == m_pkNewPetSystemExpireEvent)
	{
		newpetsystem_event_infoe* infoe = AllocEventInfo<newpetsystem_event_infoe>();

		infoe->pPetSystem = this;

		m_pkNewPetSystemExpireEvent = event_create(newpetsystem_expire_event, infoe, PASSES_PER_SEC(1));	// 1 volata per sec
	}

	return petActor;
}

CNewPetActor* CNewPetSystem::GetByVID(DWORD vid) const
{
	CNewPetActor* petActor = 0;

	bool bFound = false;

	for (TNewPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		petActor = iter->second;

		if (0 == petActor)
		{
			sys_err("[CPetSystem::GetByVID(%d)] nullptr Pointer (petActor)", vid);
			continue;
		}

		bFound = petActor->GetVID() == vid;

		if (true == bFound)
			break;
	}

	return bFound ? petActor : 0;
}

/// 등록 된 펫 중에서 주어진 몹 VNUM을 가진 액터를 반환하는 함수.
CNewPetActor* CNewPetSystem::GetByVnum(DWORD vnum) const
{
	CNewPetActor* petActor = 0;

	TNewPetActorMap::const_iterator iter = m_petActorMap.find(vnum);

	if (m_petActorMap.end() != iter)
		petActor = iter->second;

	return petActor;
}

size_t CNewPetSystem::CountSummoned() const
{
	size_t count = 0;

	for (TNewPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			if (petActor->IsSummoned())
				++count;
		}
	}

	return count;
}

void CNewPetSystem::RefreshBuff()
{
	for (TNewPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CNewPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			if (petActor->IsSummoned())
			{
				petActor->ClearBuff();
				petActor->GiveBuff();
			}
		}
	}
}