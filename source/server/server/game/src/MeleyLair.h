#pragma once
#include "../../common/service.h"

#ifdef ENABLE_MELEY_LAIR_DUNGEON
#include "../../common/length.h"
#include "../../common/item_length.h"
#include "../../common/tables.h"
#include "guild.h"
#include "char.h"
#include "char_manager.h"
#include "sectree_manager.h"

namespace MeleyLair
{
	enum eConfig
	{
		MAP_INDEX = 212,
		SUBMAP_INDEX = 62,
		MIN_LVL = 3,
		PARTECIPANTS_LIMIT = 16,
		LADDER_POINTS_COST = 600,
		LADDER_POINTS_RETURN = 300,
		COOLDOWN_DUNGEON = 10800,
		NPC_VNUM = 20419,
		GATE_VNUM = 20388,
		BOSS_VNUM = 6193,
		STATUE_VNUM = 6118,
		CHEST_VNUM = 20420,
		REWARD_ITEMCHEST_VNUM_1 = 50270,
		REWARD_ITEMCHEST_VNUM_2 = 50271,
		TIME_LIMIT_DUNGEON = 3600,
		SEAL_VNUM_KILL_STATUE = 30341,
		TIME_LIMIT_TO_KILL_STATUE = 10,
		TIME_RESPAWN_COMMON_STEP1 = 5,
		MOBCOUNT_RESPAWN_COMMON_STEP = 12,
		MOBVNUM_RESPAWN_COMMON_STEP1 = 6112,
		TIME_RESPAWN_COMMON_STEP2 = 30,
		MOBVNUM_RESPAWN_COMMON_STEP2 = 6113,
		MOBVNUM_RESPAWN_STONE_STEP2 = 20422,
		MOBCOUNT_RESPAWN_STONE_STEP2 = 4,
		TIME_RESPAWN_COMMON_STEP3 = 30,
		MOBVNUM_RESPAWN_COMMON_STEP3 = 6115,
		MOBVNUM_RESPAWN_BOSS_STEP3 = 6116,
		MOBVNUM_RESPAWN_SUBBOSS_STEP3 = 6117,
		MOBCOUNT_RESPAWN_BOSS_STEP3 = 4,
	};

	extern int32_t stoneSpawnPos[MOBCOUNT_RESPAWN_STONE_STEP2][2];
	extern int32_t monsterSpawnPos[MOBCOUNT_RESPAWN_COMMON_STEP][2];

	class CMgrMap
	{
	public:
		CMgrMap(int32_t lMapIndex, uint32_t dwGuildID);
		~CMgrMap();
		void	Destroy();
		int32_t	GetMapIndex() const { return map_index; }
		uint32_t	GetGuildID() const { return guild_id; }
		uint8_t	GetDungeonStep() const { return dungeon_step; }
		void	SetDungeonStep(uint8_t bStep);
		void	StartDungeonStep(uint8_t bStep);
		uint32_t	GetDungeonTimeStart() const { return time_start; }
		void	SetDungeonTimeStart(uint32_t dwTime) { time_start = dwTime; }
		uint32_t	GetLastStoneKilledTime() const { return last_stoneKilled; }
		void	SetLastStoneKilledTime(uint32_t dwTime) { last_stoneKilled = dwTime; }
		uint32_t	GetKillCountStones() const { return kill_stonesCount; }
		void	SetKillCountStones(uint32_t dwCount) { kill_stonesCount = dwCount; }
		uint32_t	GetKillCountBosses() const { return kill_bossesCount; }
		void	SetKillCountBosses(uint32_t dwCount) { kill_bossesCount = dwCount; }
		uint32_t	GetRewardTime() const { return reward; }
		void	SetRewardTime(uint32_t dwTime) { reward = dwTime; }
		uint32_t	GetPartecipantsCount() const;
		void	Partecipant(bool bInsert, uint32_t dwPlayerID);
		bool	IsPartecipant(uint32_t dwPlayerID);
		CHARACTER* Spawn(uint32_t dwVnum, int32_t iX, int32_t iY, int32_t iDir = 0, bool bSpawnMotion = false);
		void	Start();
		void	StartDungeon(CHARACTER* pkChar);
		void	EndDungeon(bool bSuccess, bool bGiveBack);
		void	EndDungeonWarp();
		bool	Damage(CHARACTER* pkStatue);
		void	OnKill(uint32_t dwVnum);
		void	OnKillStatue(CItem* pkItem, CHARACTER* pkChar, CHARACTER* pkStatue);
		void	DungeonResult();
		bool	CheckRewarder(uint32_t dwPlayerID);
		void	GiveReward(CHARACTER* pkChar, uint8_t bReward);
		LPSECTREE_MAP& GetMapSectree() { return pkSectreeMap; }
		CHARACTER*& GetBossChar() { return pkBoss; }
		CHARACTER*& GetStatue1Char() { return pkStatue1; }
		CHARACTER*& GetStatue2Char() { return pkStatue2; }
		CHARACTER*& GetStatue3Char() { return pkStatue3; }
		CHARACTER*& GetStatue4Char() { return pkStatue4; }

	private:
		int32_t	map_index;
		uint32_t	guild_id, time_start, last_stoneKilled, kill_stonesCount, kill_bossesCount, reward;
		uint8_t	dungeon_step;
		std::vector<uint32_t>	v_Partecipants, v_Already, v_Rewards;
		LPSECTREE_MAP	pkSectreeMap;
		CHARACTER* pkMainNPC;
		CHARACTER* pkGate;
		CHARACTER* pkBoss;
		CHARACTER* pkStatue1;
		CHARACTER* pkStatue2;
		CHARACTER* pkStatue3;
		CHARACTER* pkStatue4;

	protected:
		LPEVENT	e_pEndEvent, e_pWarpEvent, e_SpawnEvent, e_SEffectEvent, e_DestroyStatues;
	};

	class CMgr : public singleton<CMgr>
	{
	public:
		void	Initialize();
		void	Destroy();
		void	Register(CHARACTER* pkChar, int32_t& iRes1, int32_t& iRes2);
		bool	isRegistered(CGuild* pkGuild, int32_t& iCH);
		bool	Enter(CGuild* pkGuild, CHARACTER* pkChar, int32_t& iLimit);
		void	Leave(CGuild* pkGuild, CHARACTER* pkChar, bool bSendOut);
		void	LeaveRequest(CGuild* pkGuild, CHARACTER* pkChar);
		bool	IsMeleyMap(int32_t lMapIndex);
		void	Check(CGuild* pkGuild, CHARACTER* pkChar);
		void	WarpOut(CHARACTER* pkChar);
		void	SetXYZ(int32_t lX, int32_t lY, int32_t lZ) { lMapCenterPos.x = lX, lMapCenterPos.y = lY, lMapCenterPos.z = lZ; }
		const	PIXEL_POSITION& GetXYZ() const { return lMapCenterPos; }
		void	SetSubXYZ(int32_t lX, int32_t lY, int32_t lZ) { lSubMapPos.x = lX, lSubMapPos.y = lY, lSubMapPos.z = lZ; }
		const	PIXEL_POSITION& GetSubXYZ() const { return lSubMapPos; }
		void	Start(CHARACTER* pkChar, CGuild* pkGuild);
		bool	Damage(CHARACTER* pkStatue, CGuild* pkGuild);
		void	Remove(uint32_t dwGuildID);
		void	OnKill(uint32_t dwVnum, CGuild* pkGuild);
		void	OnKillStatue(CItem* pkItem, CHARACTER* pkChar, CHARACTER* pkStatue, CGuild* pkGuild);
		void	OnKillCommon(CHARACTER* pkMonster, CHARACTER* pkChar, CGuild* pkGuild);
		bool	CanGetReward(CHARACTER* pkChar, CGuild* pkGuild);
		void	Reward(CHARACTER* pkChar, CGuild* pkGuild, uint8_t bReward);
		void	OpenRanking(CHARACTER* pkChar);
		void	MemberRemoved(CHARACTER* pkChar, CGuild* pkGuild);
		void	GuildRemoved(CGuild* pkGuild);

	private:
		PIXEL_POSITION	lMapCenterPos, lSubMapPos;
		std::map<uint32_t, CMgrMap*> m_RegGuilds;
	};
};
#endif
