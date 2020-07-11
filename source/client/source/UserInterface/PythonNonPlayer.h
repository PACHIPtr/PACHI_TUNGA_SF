#pragma once

/*
 *	NPC 데이터 프로토 타잎을 관리 한다.
 */
class CPythonNonPlayer : public CSingleton<CPythonNonPlayer>
{
	public:
		enum  EClickEvent
		{
			ON_CLICK_EVENT_NONE		= 0,
			ON_CLICK_EVENT_BATTLE	= 1,
			ON_CLICK_EVENT_SHOP		= 2,
			ON_CLICK_EVENT_TALK		= 3,
			ON_CLICK_EVENT_VEHICLE	= 4,

			ON_CLICK_EVENT_MAX_NUM,
		};

		enum EMobEnchants
		{   
			MOB_ENCHANT_CURSE,
			MOB_ENCHANT_SLOW,   
			MOB_ENCHANT_POISON,
			MOB_ENCHANT_STUN,   
			MOB_ENCHANT_CRITICAL,
			MOB_ENCHANT_PENETRATE,
			MOB_ENCHANTS_MAX_NUM
		};
		enum EMobResists
		{
			MOB_RESIST_SWORD,
			MOB_RESIST_TWOHAND,
			MOB_RESIST_DAGGER,
			MOB_RESIST_BELL,
			MOB_RESIST_FAN,
			MOB_RESIST_BOW,
			MOB_RESIST_FIRE,
			MOB_RESIST_ELECT,
			MOB_RESIST_MAGIC,
			MOB_RESIST_WIND,
			MOB_RESIST_POISON,
			MOB_RESIST_CLAW,
			MOB_RESISTS_MAX_NUM 
		};

		#define MOB_ATTRIBUTE_MAX_NUM	12
		#define MOB_SKILL_MAX_NUM		5

#pragma pack(push)
#pragma pack(1)
		typedef struct SMobSkillLevel
		{
			DWORD       dwVnum;
			BYTE        bLevel;
		} TMobSkillLevel;

		typedef struct SMobTable
		{
			DWORD       dwVnum;
			char        szName[64 + 1]; 
			char        szLocaleName[64 + 1];

			BYTE        bType;                  // Monster, NPC
			BYTE        bRank;                  // PAWN, KNIGHT, KING
			BYTE        bBattleType;            // MELEE, etc..
			BYTE        bLevel;                 // Level
			BYTE        bSize;

			DWORD       dwGoldMin;
			DWORD       dwGoldMax;
			DWORD       dwExp;
			DWORD       dwMaxHP;
			BYTE        bRegenCycle;
			BYTE        bRegenPercent;
			WORD        wDef;

			DWORD       dwAIFlag;
			DWORD       dwRaceFlag;
			DWORD       dwImmuneFlag;

			BYTE        bStr, bDex, bCon, bInt;
			DWORD       dwDamageRange[2];

			short       sAttackSpeed;
			short       sMovingSpeed;
			BYTE        bAggresiveHPPct;
			WORD        wAggressiveSight;
			WORD        wAttackRange;

			char        cEnchants[MOB_ENCHANTS_MAX_NUM];
			char        cResists[MOB_RESISTS_MAX_NUM];

			DWORD       dwResurrectionVnum;
			DWORD       dwDropItemVnum;

			BYTE        bMountCapacity;
			BYTE        bOnClickType;

			BYTE        bEmpire;
			char        szFolder[64 + 1];
			float       fDamMultiply;
			DWORD       dwSummonVnum;
			DWORD       dwDrainSP;
			DWORD		dwMonsterColor;
		    DWORD       dwPolymorphItemVnum;

			TMobSkillLevel	Skills[MOB_SKILL_MAX_NUM];

		    BYTE		bBerserkPoint;
			BYTE		bStoneSkinPoint;
			BYTE		bGodSpeedPoint;
			BYTE		bDeathBlowPoint;
			BYTE		bRevivePoint;
		} TMobTable;
#pragma pack(pop)

#ifdef ENABLE_MOB_SCALE_SYSTEM
		typedef struct SMobScaleTable
		{
			DWORD	dwMobNum;
			float	fx;
			float	fy;
			float	fz;
		} TMobScaleTable;
		
		enum EMobScale
		{
			MOBSCALETABLE_MOB_NUM,
			MOBSCALETABLE_X,
			MOBSCALETABLE_Y,
			MOBSCALETABLE_Z,
			MOBSCALETABLE_MAX_NUM,
		};		
#endif

		typedef std::list<TMobTable *> TMobTableList;
		typedef std::map<DWORD, TMobTable *> TNonPlayerDataMap;
#ifdef ENABLE_MOB_SCALE_SYSTEM
		typedef std::map<DWORD, TMobScaleTable *> TNonMobScaleDataMap;
#endif

	public:
		CPythonNonPlayer(void);
		virtual ~CPythonNonPlayer(void);

		void Clear();
		void Destroy();

		bool				LoadNonPlayerData(const char * c_szFileName);
#ifdef ENABLE_MOB_SCALE_SYSTEM
		bool				LoadMobScale(const char * c_szFileName);
#endif

		const TMobTable *	GetTable(DWORD dwVnum);
#ifdef ENABLE_MOB_SCALE_SYSTEM
		const TMobScaleTable *	GetScaleTable(DWORD dwVnum);
#endif
		bool				GetName(DWORD dwVnum, const char ** c_pszName);
		bool				GetInstanceType(DWORD dwVnum, BYTE* pbType);
		BYTE				GetEventType(DWORD dwVnum);
		BYTE				GetEventTypeByVID(DWORD dwVID);
		DWORD				GetMonsterColor(DWORD dwVnum);
		const char*			GetMonsterName(DWORD dwVnum);
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
		// TARGET_INFO
		DWORD				GetMonsterMaxHP(DWORD dwVnum);
		DWORD				GetMonsterRaceFlag(DWORD dwVnum);
		DWORD				GetMonsterLevel(DWORD dwVnum);
		DWORD				GetMonsterDamage1(DWORD dwVnum);
		DWORD				GetMonsterDamage2(DWORD dwVnum);
		DWORD				GetMonsterExp(DWORD dwVnum);
		float				GetMonsterDamageMultiply(DWORD dwVnum);
		DWORD				GetMonsterST(DWORD dwVnum);
		DWORD				GetMonsterDX(DWORD dwVnum);
		bool				IsMonsterStone(DWORD dwVnum);
		DWORD				GetMobResist(DWORD dwVnum, BYTE bResistNum);
		BYTE				GetMobRegenCycle(DWORD dwVnum);
		BYTE				GetMobRegenPercent(DWORD dwVnum);
		DWORD				GetMobGoldMin(DWORD dwVnum);
		DWORD				GetMobGoldMax(DWORD dwVnum);
#endif
		BYTE    			GetMobLevel(DWORD dwVnum);
#ifdef ENABLE_MOB_SCALE_SYSTEM
		bool				GetScale(DWORD dwVnum, float & fx, float & fy, float & fz);
#endif

	protected:
		TNonPlayerDataMap	m_NonPlayerDataMap;
#ifdef ENABLE_MOB_SCALE_SYSTEM
		TNonMobScaleDataMap	m_NonMobScaleDataMap;
#endif
};