#ifndef	__HEADER_MOUNT_SYSTEM__
#define	__HEADER_MOUNT_SYSTEM__

class CHARACTER;

class CMountActor
{
protected:
	friend class CMountSystem;

	CMountActor(LPCHARACTER owner, DWORD vnum);

	virtual ~CMountActor();

	virtual bool	Update(DWORD deltaTime);
	virtual bool	_UpdateFollowAI();

private:
	bool Follow(float fMinDistance = 50.f);

public:
	LPCHARACTER		GetCharacter()	const { return m_pkChar; }
	LPCHARACTER		GetOwner()	const { return m_pkOwner; }
	DWORD			GetVID() const { return m_dwVID; }
	DWORD			GetVnum() const { return m_dwVnum; }
	void			SetName();
	bool			Mount(LPITEM mountItem);
	void			Unmount();
	DWORD			Summon(LPITEM pSummonItem, bool bSpawnFar = false);
	void			Unsummon();
	bool			IsSummoned() const { return 0 != m_pkChar; }
	void			SetSummonItem(LPITEM pItem);
	DWORD			GetSummonItemVID() { return m_dwSummonItemVID; }
private:
	DWORD			m_dwVnum;
	DWORD			m_dwVID;
	DWORD			m_dwLastActionTime;
	DWORD			m_dwSummonItemVID;
	DWORD			m_dwSummonItemVnum;

	short			m_originalMoveSpeed;

	std::string		m_name;

	LPCHARACTER		m_pkChar;
	LPCHARACTER		m_pkOwner;
};

class CMountSystem
{
public:
	typedef	std::unordered_map<DWORD, CMountActor*>		TMountActorMap;

public:
	CMountSystem(LPCHARACTER owner);
	virtual ~CMountSystem();

	CMountActor* GetByVID(DWORD vid) const;
	CMountActor* GetByVnum(DWORD vnum) const;

	bool		Update(DWORD deltaTime);
	void		Destroy();

	size_t		CountSummoned() const;

public:
	void		SetUpdatePeriod(DWORD ms);

	void		Summon(DWORD mobVnum, LPITEM pSummonItem, bool bSpawnFar);

	void		Unsummon(DWORD mobVnum, bool bDeleteFromList = false);
	void		Unsummon(CMountActor* mountActor, bool bDeleteFromList = false);

	void		Mount(DWORD mobVnum, LPITEM mountItem);
	void		Unmount(DWORD mobVnum);

	void		DeleteMount(DWORD mobVnum);
	void		DeleteMount(CMountActor* mountActor);
private:
	TMountActorMap	m_mountActorMap;
	LPCHARACTER		m_pkOwner;
	DWORD			m_dwUpdatePeriod;
	DWORD			m_dwLastUpdateTime;
	LPEVENT			m_pkMountSystemUpdateEvent;
};

#endif
