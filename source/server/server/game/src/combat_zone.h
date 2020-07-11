/*********************************************************************
* title_name		: Combat Zone (Official Webzen 16.4)
* date_created		: 2017.05.21
* filename			: combat_zone.h
* author			: VegaS
* version_actual	: Version 0.2.0
*/

#define COMBAT_ZONE_FLAG_WAIT_TIME_JOIN "combat_zone.join_time"
#define COMBAT_ZONE_FLAG_BUY_LAST_TIME	"combat_zone.last_buy"
#define COMBAT_ZONE_FLAG_WAIT_TIME_REQUEST_POTION "combat_zone.request_potion"
#define COMBAT_ZONE_FLAG_MONSTERS_KILLED "combat_zone.monsters_killed"
#define COMBAT_ZONE_FLAG_LIMIT_POINTS "combat_zone.limit_points"
#define COMBAT_ZONE_FLAG_KILL_LAST_TIME "combat_zone.kill_%d"
#define SKILL_COUNT_INDEX 6

#define COMBAT_ZONE_SET_SKILL_PERFECT // Set perfect skills <disable - enable>;
#define COMBAT_ZONE_SHOW_EFFECT_POTION // When you use potion battle will show a effect image <disable - enable>;
#define COMBAT_ZONE_SHOW_SERVER_TIME_ZONE_ON_CHAT // Show the local timezone on freebsd server, that is good for people can know what hour it is on server, because maybe they are other country and didn't have same time.
#define ENABLE_BATTLE_ZONE_SYSTEM_HIDE_INFO_USER // Disable if you want for people can see names of players, align, level in map, target, minimap name etc.

enum ECombatZoneSettings
{
	BATTLE_POTION_MAX_HP = 50000, // The number of points of life will increase +50,000 with the special potion, the points are only valid until you die, when you die you will lose the effect of the potion;
	BATTLE_POTION_MAX_ATT = 150, // Attack will be increased with 100
	ITEM_COMBAT_ZONE_BATTLE_POTION_COUNT = 3, // Give 3 counts potion when on every 6 hours to request;

	COMBAT_ZONE_SHOP_MAX_LIMIT_POINTS = 100, // The maximum limit of points that is admitted per day, every evening at 00:00, resets to 0;
	COMBAT_ZONE_MONSTER_KILL_MAX_LIMIT = 100, // The maximum limit you need to kill monsters to get the potion once every 6 hours;

	COMBAT_ZONE_MAX_DEATHS_TO_INCREASE_TIMER_RESTART = 4, // When you have 4 deaths that means 20 seconds + 10 default = 30, will be reseted the deaths to 0, so next time for can restart will be default 10 secs, when you dead again + 5 = 15 etc;
	COMBAT_ZONE_INCREASE_SECONDS_RESTART = 5,

	COMBAT_ZONE_MIN_LEVEL = 50, // Minimum attendance level.

	COMBAT_ZONE_REDUCTION_MAX_MOVEMENT_SPEED = 100, // When he announced withdrawal will suffer a reduction of Movement Speed, and will be seted to 100 all time.

	COMBAT_ZONE_WAIT_TIME_TO_PARTICIPATE = 10 * 60, // Waiting time to be able to participate again - 10 minutes;
	COMBAT_ZONE_WAIT_TIME_TO_REQUEST_POTION = 6 * 60 * 60, // Waiting time to ask for potions - 6 hours;
	COMBAT_ZONE_TARGET_NEED_TO_STAY_ALIVE = 2 * 60, // The wait time you have to survive when you announced your withdrawal - 2 minutes;
	COMBAT_ZONE_WAIT_TIME_KILL_AGAIN_PLAYER = 5 * 60, // You have not received any points from this player, not passed the 5 minutes since his last killing - 5 Minutes;

	COMBAT_ZONE_ADD_POINTS_NORMAL_KILLING = 1, // When you kill a player regular;
	COMBAT_ZONE_ADD_POINTS_TARGET_KILLING = 5, // When you kill a player who has announced his withdrawal, you will receive 5 points;

	COMBAT_ZONE_REQUIRED_POINTS_TO_LEAVING = 5, // How many points you need for can announced your withdrawal - 5 points;

	COMBAT_ZONE_REQUIRED_POINTS_TO_LEAVING_WHEN_DEAD = 5, // (5 = ( < 5 -> 1, 2, 3, 4) ) -> Under 5 Battle Points: If a player who announced his withdrawal from the War Zone is killed and dropped below 5 points, he will be able to leave the in 15 seconds. It does not have to be revived.

	COMBAT_ZONE_CHEST_MAX_OPENED = 3, // How many time you can open the chests [FIELD_BOX_1, FIELD_BOX_2, WOODEN_CHEST];
	COMBAT_ZONE_NEED_CHANNEL = 1, // Channel where you want to working the combat zone;

	COMBAT_ZONE_JOIN_WARP_SECOND = 3, // When he press button to join will teleport him in 3 seconds on combat zone map;
	COMBAT_ZONE_LEAVE_WITH_TARGET_COUNTDOWN_WARP_SECONDS = 15, // When he leaving the cz he need to wait 2 minute, but when time is 15 seconds will start a countdown from 15 to 0 and teleport him to map1;
	COMBAT_ZONE_LEAVE_WHEN_DEAD_UNDER_MIN_POINTS = 15, //  Under 5 Battle Points: If a player who announced his withdrawal from the War Zone is killed and dropped below 5 points, he will be able to leave the in 15 seconds. It does not have to be revived.
	COMBAT_ZONE_LEAVE_REGULAR_COUNTDOWN_WARP_SECONDS = 3, // When he leaving the cz when he have 0 points or under 5 points, without target attached;

	DEF_MULTIPLIER = 5,
	DEF_ADDED_BONUS = 350, // Change value of bonus algorithm by level, here is example how it works: http://cpp.sh/3teof change with your settings and press RUN.
};

enum ECombatZoneGeneral
{
	COMBAT_ZONE_MAX_ROWS_RANKING = 10,
	COMBAT_ZONE_EMPTY_VALUE_ROW = 999,
	COMBAT_ZONE_TYPE_RANKING_WEEKLY = 1,
	COMBAT_ZONE_TYPE_RANKING_ALL = 2,
	COMBAT_ZONE_MAX_TOP_RANK = 3,
	COMBAT_ZONE_CAN_START = 1,
	COMBAT_ZONE_CAN_FINISH = 2,
	COMBAT_ZONE_MAP_INDEX = 246,
	COMBAT_ZONE_INDEX_TARGET = 9999,
	COMBAT_ZONE_GET_POINTS = 1,
	COMBAT_ZONE_GET_RANK = 2,
	COMBAT_ZONE_MAX_POS_TELEPORT = 10,
	COMBAT_ZONE_SET_SKILL_MAX_LEVEL = 1,
	COMBAT_ZONE_GET_SKILL_BACK_BY_CACHE = 2,
	COMBAT_ZONE_ADD_LEAVING_TARGET = 1,
	COMBAT_ZONE_REMOVE_LEAVING_TARGET = 2,
	COMBAT_ZONE_START_INFO_HOURS = 0,
	COMBAT_ZONE_START_INFO_MINUTES = 1,
	COMBAT_ZONE_DURATION = 2,
	COMBAT_ZONE_DIVIDE_NUM_POINTS = 2,
	COMBAT_ZONE_ADD_DEATHS_POINTS = 1,
	COMBAT_ZONE_MAX_ARGS = 4,
};

enum ECombatZoneItemsVnums
{
	ITEM_COMBAT_ZONE_BATTLE_POTION = 27125,
	ITEM_COMBAT_ZONE_IRON_CHEST = 50285,
	ITEM_COMBAT_ZONE_GOLDEN_CHEST = 50286,
	ITEM_COMBAT_ZONE_FIELD_BOX_1 = 50287,
	ITEM_COMBAT_ZONE_FIELD_BOX_2 = 50288,
	ITEM_COMBAT_ZONE_REINCARNATION = 50289,
	ITEM_COMBAT_ZONE_WOODEN_CHEST = 50290,
};

enum ECombatZoneActions
{
	COMBAT_ZONE_ACTION_NONE,
	COMBAT_ZONE_ACTION_OPEN_RANKING,
	COMBAT_ZONE_ACTION_CHANGE_PAGE_RANKING,
	COMBAT_ZONE_ACTION_PARTICIPATE,
	COMBAT_ZONE_ACTION_LEAVE,
	COMBAT_ZONE_ACTION_REQUEST_POTION,
};

enum EPacketCGCombatZoneSubHeaderType
{
	COMBAT_ZONE_SUB_HEADER_NONE,
	COMBAT_ZONE_SUB_HEADER_ADD_LEAVING_TARGET,
	COMBAT_ZONE_SUB_HEADER_REMOVE_LEAVING_TARGET,
	COMBAT_ZONE_SUB_HEADER_FLASH_ON_MINIMAP,
	COMBAT_ZONE_SUB_HEADER_OPEN_RANKING,
	COMBAT_ZONE_SUB_HEADER_REFRESH_SHOP,
};

enum ECombatZoneStates
{
	STATE_CLOSED,
	STATE_OPENED,
};

enum ETypeDay
{
	DAY_MONDAY,
	DAY_TUESDAY,
	DAY_WEDNESDAY,
	DAY_THURSDAY,
	DAY_FRIDAY,
	DAY_SATURDAY,
	DAY_SUNDAY,
	DAY_MAX_NUM
};

enum ECombatZonePadders
{
	PAD_DAY,
	PAD_HOUR,
	PAD_MIN,
	PAD_SEC
};

enum ECombatZonesSkillsCache
{
	SKILL_VNUM_1,
	SKILL_VNUM_2,
	SKILL_VNUM_3,
	SKILL_VNUM_4,
	SKILL_VNUM_5,
	SKILL_VNUM_6,
};

struct SCombatZoneRespawnData
{
	int x;
	int y;
};

struct SCombatZoneSkillsCacheData
{
	DWORD dwSkillVnum;
	DWORD dwSkillLevel;
};

class CCombatZoneManager : public singleton<CCombatZoneManager>
{
public:
	bool Initialize();
	void Destroy();

	LPEVENT		m_pkCombatZoneEvent;

	DWORD GetCurrentDay();
	DWORD GetRandomPos() { return number(0, COMBAT_ZONE_MAX_POS_TELEPORT - 1); }
	DWORD GetRank(LPCHARACTER ch);
	DWORD GetValueByType(LPCHARACTER ch, int state);
	const DWORD* GetSkillList(LPCHARACTER ch);
	DWORD GetFirstDayHour();

	bool CanUseItem(LPCHARACTER ch, LPITEM item);
	bool AnalyzeTimeZone(DWORD state_type, DWORD iDay);
	bool IsCombatZoneMap(int nMapIndex);
	bool CanJoin(LPCHARACTER ch);
	bool GetIsWeekend();
	bool IsRunning();
	bool CanUseAction(LPCHARACTER ch, DWORD bType);

	void ShowCurrentTimeZone(LPCHARACTER ch);
	void RemoveAffect(LPCHARACTER ch);

	void OnResetRanking();

	void RefreshLeavingTargetSign(LPCHARACTER ch);
	void SendLeavingTargetSign(LPCHARACTER ch, DWORD dwType);
	void ActTargetSignMap(LPCHARACTER ch, DWORD bType);
	void SendCombatZoneInfoPacket(LPCHARACTER pkTarget, DWORD sub_header, std::vector<DWORD> m_vec_infoData);
	void RequestPotion(LPCHARACTER ch);
	void CalculatePointsByKiller(LPCHARACTER ch, bool isAttachedTarget);
	void SetStatus(DWORD iValue);
	void SetCurrentCombatZonePoints(LPCHARACTER ch);
	void Join(LPCHARACTER ch);
	void Leave(LPCHARACTER ch);
	void AppendSkillCache(LPCHARACTER ch);
	void SetSkill(LPCHARACTER ch, DWORD state);
	void OnRestart(LPCHARACTER ch, int subcmd);
	void Flash();
	void Announcement(const char* format, ...);
	void AnnouncementEx(const char* format, ...);
	void CheckEventStatus();
	void OnLogout(LPCHARACTER ch);
	void OnLogin(LPCHARACTER ch);
	void OnDead(LPCHARACTER pkKiller, LPCHARACTER pkDead);
	void RequestAction(LPCHARACTER ch, const char* c_pData);
	void RequestRanking(LPCHARACTER ch, DWORD dwState);
	/*
		int Get(int state_type, int iDay);
		std::string GetInformationAboutOpen();
		bool GetTargetInfo(LPCHARACTER ch);
		void RequestUpdateRanking(const char* memberName, int memberEmpire, int memberPoints);
		void RequestSetMemberRankFlag();
		void AddRankFlag(const char* memberName, int memberRankIndex);
		void RequestDirectQuery(int iAction);
		void CreateTarget(LPCHARACTER ch);
		void DeleteTarget(LPCHARACTER ch);
		void RequestResetRanking();
	*/
};