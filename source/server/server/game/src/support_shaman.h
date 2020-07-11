#ifndef	__HEADER_SupportShaman_SYSTEM__
#define	__HEADER_SupportShaman_SYSTEM__

class CHARACTER;

struct SShamanAbility
{
};

/**
*/
class CSupportShamanActor //: public CHARACTER
{
public:
	enum EShamanOptions
	{
		ESupportShamanOption_Followable = 1 << 0,
		ESupportShamanOption_Mountable = 1 << 1,
		ESupportShamanOption_Summonable = 1 << 2,
		ESupportShamanOption_Combatable = 1 << 3,
	};

protected:
	friend class CSupportShaman;

	CSupportShamanActor(LPCHARACTER owner, DWORD vnum, DWORD options = ESupportShamanOption_Followable | ESupportShamanOption_Summonable);

	virtual ~CSupportShamanActor();

	virtual bool	Update(DWORD deltaTime);

protected:
	virtual bool	_UpdateFollowAI();				///< 주인을 따라다니는 AI 처리
	virtual bool	_UpdatAloneActionAI(float fMinDist, float fMaxDist);			///< 주인 근처에서 혼자 노는 AI 처리

private:
	bool Follow(float fMinDistance = 50.f);

public:
	LPCHARACTER		GetCharacter()	const { return m_pkChar; }
	LPCHARACTER		GetOwner()	const { return m_pkOwner; }
	DWORD			GetVID() const { return m_dwVID; }
	DWORD			GetVnum() const { return m_dwVnum; }

	bool			HasOption(EShamanOptions option) const { return m_dwOptions & option; }

	void			SetName();
	void			SetLevel(DWORD level);

	bool			Mount();
	void			Unmount();

	DWORD			Summon(const char* SupportShamanName, LPITEM pSummonItem, bool bSpawnFar = false);
	void			Unsummon();

	bool			IsSummoned() const { return 0 != m_pkChar; }
	void			SetSummonItem(LPITEM pItem);
	DWORD			GetLevel() { return m_dwlevel; }
	void			SetLastSkillTime(DWORD time) { m_dwLastSkillTime = time; }
	void			SetCostume();
	void			SetWeapon();
#ifdef ENABLE_ACCE_SYSTEM
	void			SetSash();
#endif
	void			SetHair();
	DWORD			GetLastSkillTime() { return m_dwLastSkillTime; }
	void			SetLastExpUpdate(DWORD time) { m_dwLastExpUpdateTime = time; }
	DWORD			GetLastExpUpdate() { return m_dwLastExpUpdateTime; }
	void			SetExp(DWORD exp);
	void			UpdateItemExp();
	void			UpdatePacketSupportShamanActor();
	void			RefreshCostume();
	DWORD			GetExp() { return m_dwExp; }
	void			SetNextExp(int nextExp);
	int				GetNextExp() { return m_dwNextExp; }
	int				GetIntSkill();
	int				GetSupportShamanVID();
	void			UseSkill();
	void			SetIntSkill();
	DWORD			GetSummonItemVID() { return m_dwSummonItemVID; }

	void			PickUpItems(int range);
	void			BringItem();
	void			SetPickup(bool is_pickup) { m_is_pickup = is_pickup; }
	bool			IsPickup() { return m_is_pickup; }
	void			SetPickupItem(LPITEM item) { m_pickup_item = item; }
	LPITEM			GetPickupItem() { return m_pickup_item; }

	void			GiveBuff();
	void			ClearBuff();

private:
	DWORD			m_dwVnum;
	DWORD			m_dwVID;
	DWORD			m_dwOptions;
	DWORD			m_dwLastActionTime;
	DWORD			m_dwSummonItemVID;
	DWORD			m_dwSummonItemVnum;
	DWORD			m_dwExp;
	DWORD			m_dwLastSkillTime;
	DWORD			m_dwLastExpUpdateTime;
	DWORD			m_dwIntSkill;
	DWORD			m_dwlevel;
	DWORD			m_dwNextExp;
	short			m_originalMoveSpeed;

	std::string		first_name;
	std::string		second_name;
	std::string		m_speciality;
	LPCHARACTER		m_pkChar;
	LPCHARACTER		m_pkOwner;
	bool			m_is_pickup;
	LPITEM			m_pickup_item;
};

/**
*/
class CSupportShaman
{
public:
	typedef	std::unordered_map<DWORD, CSupportShamanActor*>		TSupportShamanActorMap;

public:
	CSupportShaman(LPCHARACTER owner);
	virtual ~CSupportShaman();

	CSupportShamanActor* GetByVID(DWORD vid) const;
	CSupportShamanActor* GetByVnum(DWORD vnum) const;

	bool		Update(DWORD deltaTime);
	void		Destroy();

	size_t		CountSummoned() const;

public:
	void		SetUpdatePeriod(DWORD ms);

	CSupportShamanActor* Summon(DWORD mobVnum, LPITEM pSummonItem, const char* SupportShamanName, bool bSpawnFar, DWORD options = CSupportShamanActor::ESupportShamanOption_Followable | CSupportShamanActor::ESupportShamanOption_Summonable);

	void		Unsummon(DWORD mobVnum, bool bDeleteFromList = false);
	void		Unsummon(CSupportShamanActor* SupportShamanActor, bool bDeleteFromList = false);
	void		SetExp(int iExp);
	int			GetLevel();
	int			GetExp();
	bool		IsActiveSupportShaman();
	CSupportShamanActor* GetActiveSupportShaman();
	CSupportShamanActor* AddShaman(DWORD mobVnum, const char* SupportShamanName, const SShamanAbility& ability, DWORD options = CSupportShamanActor::ESupportShamanOption_Followable | CSupportShamanActor::ESupportShamanOption_Summonable | CSupportShamanActor::ESupportShamanOption_Combatable);

	void		DeleteSupportShaman(DWORD mobVnum);
	void		DeleteSupportShaman(CSupportShamanActor* SupportShamanActor);
	void		RefreshBuff();

private:
	TSupportShamanActorMap	m_SupportShamanActorMap;
	LPCHARACTER		m_pkOwner;
	DWORD			m_dwUpdatePeriod;
	DWORD			m_dwLastUpdateTime;
	LPEVENT			m_pkSupportShamanUpdateEvent;
};
#endif	//__HEADER_SupportShaman_SYSTEM__
