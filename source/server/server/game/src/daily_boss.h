class CDailyBoss : public singleton<CDailyBoss>
{
	enum DailyBossSettings
	{
		START_HOUR = 18,
		START_MINUTE = 00,
		//

		MOB_VNUM = 2092,
		MAP_INDEX = 65,
		SPAWN_X = 134,
		SPAWN_Y = 78,

		// system days settigns
		MONDAY = 1,
		TUESDAY = 2,
		WEDNESDAY = 3,
		THURSDAY = 4,
		FRIDAY = 5,
		SATURDAY = 6,
		SUNDAY = 0,

		// if you want boss every day then set here true
		ENABLE_DAILY = true,

		// if not, please choose day from "system days settings", default is sunday
		CHOOSEN_DAY = SUNDAY,
	};

public:
	CDailyBoss();
	virtual ~CDailyBoss();
	void Check(int day, int hour, int minute, int second);
	int BossVnum() { return MOB_VNUM; }
	bool IsBossMap(int mapIndex);
	void OnKill(LPCHARACTER pkKiller);
	int GetStartHour() { return START_HOUR; }
	int GetStartMinute() { return START_MINUTE; }
	bool IsDay();
private:
	void SendNoticeLine(const char* format, ...);
	bool Spawn();
};