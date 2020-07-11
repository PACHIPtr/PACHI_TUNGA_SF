class CBossTracking : public singleton<CBossTracking>
{
	enum EBossTracking
	{
		BOSS_TRACKING_MAX_NUM = 6,
	};

public:
	typedef struct BossTracking
	{
		DWORD dead_time;
		DWORD regen_time;
		BYTE channel;
		DWORD mob_vnum;

		BossTracking()
		{
			dead_time = 0;
			regen_time = 0;
			channel = 0;
			mob_vnum = 0;
		}
	} TBossTracking;
	CBossTracking();
	virtual ~CBossTracking();
	DWORD GetDeadTime(BYTE channel, DWORD dwMobVnum);
	DWORD GetRegenTime(BYTE channel, DWORD dwMobVnum);
	void SetDeadTime(BYTE channel, DWORD dwMobVnum, DWORD dwTime);
	void SetRegenTime(BYTE channel, DWORD dwMobVnum, DWORD dwTime);
	bool IsTrackingMob(DWORD dwMobVnum);
	void SendP2PPacket(DWORD dwMobVnum);
	void SendClientPacket(LPCHARACTER pkChr, BYTE channel, DWORD dwMobVnum);
protected:
	std::vector<TBossTracking> m_bossTrackingVector;
};