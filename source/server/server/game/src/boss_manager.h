#pragma once
class CBossManager : public singleton<CBossManager>
{
	typedef struct SBossTable
	{
		DWORD	regen_time;
		DWORD	dead_time;
	} TBossTable;

	typedef std::map<DWORD, TBossTable*> TBossDataMap;
public:
	CBossManager(void);
	virtual ~CBossManager(void);

	const TBossTable* GetTable(DWORD dwVnum);

	void Dead(DWORD dwBossVnum, DWORD regen_time, DWORD dead_time);

	DWORD GetRegenTime(DWORD dwBossVnum);
	DWORD GetDeadTime(DWORD dwBossVnum);

	const bool IsRegisteredBoss(DWORD vnum);

	void SendBossInformation(LPCHARACTER pkChr, DWORD dwBossVnum);

	void Update();
	void Destroy();
protected:
	TBossDataMap	m_BossTableDataMap;
};