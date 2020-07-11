#ifndef	__HEADER_NEWPET_SYSTEM__
#define	__HEADER_NEWPET_SYSTEM__

#include "../../common/service.h"

class CHARACTER;

// TODO: 펫으로서의 능력치? 라던가 친밀도, 배고픔 기타등등... 수치
struct SNewPetAbility
{
};

/**
*/
class CNewPetActor //: public CHARACTER
{
public:
	enum ENewPetOptions
	{
		EPetOption_Followable = 1 << 0,
		EPetOption_Mountable = 1 << 1,
		EPetOption_Summonable = 1 << 2,
		EPetOption_Combatable = 1 << 3,
	};

protected:
	friend class CNewPetSystem;

	CNewPetActor(LPCHARACTER owner, DWORD vnum, DWORD options = EPetOption_Followable | EPetOption_Summonable);
	//	CPetActor(LPCHARACTER owner, DWORD vnum, const SPetAbility& petAbility, DWORD options = EPetOption_Followable | EPetOption_Summonable);

	virtual ~CNewPetActor();

	virtual bool	Update(DWORD deltaTime);

protected:
	virtual bool	_UpdateFollowAI();				///< 주인을 따라다니는 AI 처리
	virtual bool	_UpdatAloneActionAI(float fMinDist, float fMaxDist);			///< 주인 근처에서 혼자 노는 AI 처리

	/// @TODO
	//virtual bool	_UpdateCombatAI();

private:
	bool Follow(float fMinDistance = 50.f);

public:
	LPCHARACTER		GetCharacter()	const { return m_pkChar; }
	LPCHARACTER		GetOwner()	const { return m_pkOwner; }
	DWORD			GetVID() const { return m_dwVID; }
	DWORD			GetVnum() const { return m_dwVnum; }

	bool			HasOption(ENewPetOptions option) const { return m_dwOptions & option; }

	void			SetName(const char* petName);
	void			SetLevel(DWORD level);

	bool			Mount();
	void			Unmount();

	DWORD			Summon(const char* petName, LPITEM pSummonItem, bool bSpawnFar = false);
	void			Unsummon();

	int             GetSkillCount();

	bool			IsSummoned() const { return 0 != m_pkChar; }
	void			SetSummonItem(LPITEM pItem);
	DWORD			GetSummonItemVID() { return m_dwSummonItemVID; }
	DWORD			GetSummonItemID() { return m_dwSummonItemID; }
	DWORD			GetEvolution() { return m_dwevolution; }
	DWORD			GetLevel() { return m_dwlevel; }
	void			SetEvolution(int lv);
	void			SetExp(DWORD exp, int mode);
	DWORD			GetExp() { return m_dwexp; }
	DWORD			GetExpI() { return m_dwexpitem; }
	DWORD			GetAge() { return m_dwAgeDuration; }
	void			SetNextExp(int nextExp);
	int				GetNextExpFromMob() { return m_dwExpFromMob; }
	int				GetNextExpFromItem() { return m_dwExpFromItem; }
	int				GetLevelStep() { return m_dwlevelstep; }
	int				GetPetType() { return m_dwPetType; }

	void			IncreasePetBonus();
	void			SetItemCube(int pos, int invpos);
	void			ItemCubeFeed(int type);
	void			DoPetSkill(int skillslot);
	void			UpdateTime();
	
	bool			ResetSkill(int skill);
	bool			IncreasePetSkill(int skill);
	bool			IncreasePetEvolution();

	// 버프 주는 함수와 거두는 함수.
	// 이게 좀 괴랄한게, 서버가 ㅄ라서,
	// POINT_MOV_SPEED, POINT_ATT_SPEED, POINT_CAST_SPEED는 PointChange()란 함수만 써서 변경해 봐야 소용이 없는게,
	// PointChange() 이후에 어디선가 ComputePoints()를 하면 싹다 초기화되고,
	// 더 웃긴건, ComputePoints()를 부르지 않으면 클라의 POINT는 전혀 변하지 않는다는 거다.
	// 그래서 버프를 주는 것은 ComputePoints() 내부에서 petsystem->RefreshBuff()를 부르도록 하였고,
	// 버프를 빼는 것은 ClearBuff()를 부르고, ComputePoints를 하는 것으로 한다.
	void			GiveBuff();
	void			ClearBuff();

private:
	int			m_dwlevelstep; //Step livello del pet da da 0 a 4
	int			m_dwExpFromMob; //Exp richiesta per il level 90% del tot
	int			m_dwExpFromItem; //Exp richiesta per il level 10% del tot
	int			m_dwexpitem; // Exp corrente presa dagli item
	int			m_dwevolution; //Stato evoluzione del pet da 1 a 4
	int			m_dwTimePet; //Tempo per il pet
	int			m_dwslotimm;
	int			m_dwPetType;

	DWORD		m_dwImmTime;

	int				m_dwpetslotitem[25];
	int				m_dwskill[3];
	int				m_dwskillslot[3];
	int				m_dwbonuspet[3][2];
	DWORD			m_dwVnum;
	DWORD			m_dwVID;
	DWORD			m_dwlevel;
	DWORD			m_dwexp;
	DWORD			m_dwOptions;
	DWORD			m_dwLastActionTime;
	DWORD			m_dwSummonItemVID;
	DWORD			m_dwSummonItemID;
	DWORD			m_dwSummonItemVnum;

	DWORD			m_dwduration;
	DWORD			m_dwtduration;
	DWORD			m_dwAgeDuration;

	short			m_originalMoveSpeed;

	std::string		m_name;

	LPCHARACTER		m_pkChar;					// Instance of pet(CHARACTER)
	LPCHARACTER		m_pkOwner;
	//	SPetAbility		m_petAbility;				// 능력치
};

/**
*/
class CNewPetSystem
{
public:
	typedef	std::unordered_map<DWORD, CNewPetActor*>		TNewPetActorMap;		/// <VNUM, NewPetActor> map. (한 캐릭터가 같은 vnum의 펫을 여러개 가질 일이 있을까..??)

public:
	CNewPetSystem(LPCHARACTER owner);
	virtual ~CNewPetSystem();

	CNewPetActor* GetByVID(DWORD vid) const;
	CNewPetActor* GetByVnum(DWORD vnum) const;

	bool		Update(DWORD deltaTime);
	void		Destroy();

	size_t		CountSummoned() const;			///< 현재 소환된(실체화 된 캐릭터가 있는) 펫의 개수

public:
	void		SetUpdatePeriod(DWORD ms);

	CNewPetActor* Summon(DWORD mobVnum, LPITEM pSummonItem, const char* petName, bool bSpawnFar, DWORD options = CNewPetActor::EPetOption_Followable | CNewPetActor::EPetOption_Summonable);

	void		Unsummon(DWORD mobVnum, bool bDeleteFromList = false);
	void		Unsummon(CNewPetActor* petActor, bool bDeleteFromList = false);

	// TODO: 진짜 펫 시스템이 들어갈 때 구현. (캐릭터가 보유한 펫의 정보를 추가할 때 라던가...)
	CNewPetActor* AddPet(DWORD mobVnum, const char* petName, const SNewPetAbility& ability, DWORD options = CNewPetActor::EPetOption_Followable | CNewPetActor::EPetOption_Summonable | CNewPetActor::EPetOption_Combatable);

	bool		ResetSkill(int skill);
	bool		IncreasePetSkill(int skill);
	bool		IncreasePetEvolution();

	void		DeletePet(DWORD mobVnum);
	void		DeletePet(CNewPetActor* petActor);
	void		RefreshBuff();
	bool		IsActivePet();
	DWORD		GetNewPetITemID();
	void		SetExp(int iExp, int mode);
	int			GetEvolution();
	int			GetLevel();
	int			GetExp();
	int			GetAge();
	int			GetLevelStep();
	int			GetPetType();
	void		SetItemCube(int pos, int invpos);
	void		ItemCubeFeed(int type);
	void		DoPetSkill(int skillslot);
	void		UpdateTime();

private:
	TNewPetActorMap	m_petActorMap;
	LPCHARACTER		m_pkOwner;					///< 펫 시스템의 Owner
	DWORD			m_dwUpdatePeriod;			///< 업데이트 주기 (ms단위)
	DWORD			m_dwLastUpdateTime;
	LPEVENT			m_pkNewPetSystemUpdateEvent;
	LPEVENT			m_pkNewPetSystemExpireEvent;
};

/**
// Summon Pet
CPetSystem* petSystem = mainChar->GetPetSystem();
CPetActor* petActor = petSystem->Summon(~~~);

DWORD petVID = petActor->GetVID();
if (0 == petActor)
{
	ERROR_LOG(...)
};

// Unsummon Pet
petSystem->Unsummon(petVID);

// Mount Pet
petActor->Mount()..

CPetActor::Update(...)
{
	// AI : Follow, actions, etc...
}

*/

#endif	//__HEADER_PET_SYSTEM__
