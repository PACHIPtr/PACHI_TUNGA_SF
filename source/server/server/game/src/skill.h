#ifndef __INC_METIN_II_GAME_CSkillManager_H__
#define __INC_METIN_II_GAME_CSkillManager_H__

#include "../../libthecore/libthecore/poly.h"

enum ESkillFlags
{
	SKILL_FLAG_ATTACK = (1 << 0),	// 공격 기술
	SKILL_FLAG_USE_MELEE_DAMAGE = (1 << 1),	// 기본 밀리 타격치를 b 값으로 사용
	SKILL_FLAG_COMPUTE_ATTGRADE = (1 << 2),	// 공격등급을 계산한다
	SKILL_FLAG_SELFONLY = (1 << 3),	// 자신에게만 쓸 수 있음
	SKILL_FLAG_USE_MAGIC_DAMAGE = (1 << 4),	// 기본 마법 타격치를 b 값으로 사용
	SKILL_FLAG_USE_HP_AS_COST = (1 << 5),	// HP를 SP대신 쓴다
	SKILL_FLAG_COMPUTE_MAGIC_DAMAGE = (1 << 6),
	SKILL_FLAG_SPLASH = (1 << 7),
	SKILL_FLAG_GIVE_PENALTY = (1 << 8),	// 쓰고나면 잠시동안(3초) 2배 데미지를 받는다.
	SKILL_FLAG_USE_ARROW_DAMAGE = (1 << 9),	// 기본 화살 타격치를 b 값으로 사용
	SKILL_FLAG_PENETRATE = (1 << 10),	// 방어무시
	SKILL_FLAG_IGNORE_TARGET_RATING = (1 << 11),	// 상대 레이팅 무시
	SKILL_FLAG_SLOW = (1 << 12),	// 슬로우 공격
	SKILL_FLAG_STUN = (1 << 13),	// 스턴 공격
	SKILL_FLAG_HP_ABSORB = (1 << 14),	// HP 흡수
	SKILL_FLAG_SP_ABSORB = (1 << 15),	// SP 흡수
	SKILL_FLAG_FIRE_CONT = (1 << 16),	// FIRE 지속 데미지
	SKILL_FLAG_REMOVE_BAD_AFFECT = (1 << 17),	// 나쁜효과 제거
	SKILL_FLAG_REMOVE_GOOD_AFFECT = (1 << 18),	// 나쁜효과 제거
	SKILL_FLAG_CRUSH = (1 << 19),	// 상대방을 날림
	SKILL_FLAG_POISON = (1 << 20),	// 독 공격
	SKILL_FLAG_TOGGLE = (1 << 21),	// 토글
	SKILL_FLAG_DISABLE_BY_POINT_UP = (1 << 22),	// 찍어서 올릴 수 없다.
	SKILL_FLAG_CRUSH_LONG = (1 << 23),	// 상대방을 멀리 날림
	SKILL_FLAG_WIND = (1 << 24),	// 바람 속성
	SKILL_FLAG_ELEC = (1 << 25),	// 전기 속성
	SKILL_FLAG_FIRE = (1 << 26),	// 불 속성
	SKILL_FLAG_BLEEDING = (1 << 27),
	SKILL_FLAG_PARTY = (1 << 28),
#ifdef ENABLE_KNOCKBACK_SKILL
	SKILL_FLAG_KNOCKBACK	= (1 << 29),
#endif
};

enum
{
	SKILL_PENALTY_DURATION = 3,
	SKILL_TYPE_HORSE = 5,
};

enum ESkillIndexes
{
	SKILL_RESERVED = 0,

	SKILL_SAMYEON = 1,
	SKILL_PALBANG,
	SKILL_JEONGWI,
	SKILL_GEOMKYUNG,
	SKILL_TANHWAN,

	SKILL_GIGONGCHAM = 16,
	SKILL_GYOKSAN,
	SKILL_DAEJINGAK,
	SKILL_CHUNKEON,
	SKILL_GEOMPUNG,

	SKILL_AMSEOP = 31,
	SKILL_GUNGSIN,
	SKILL_CHARYUN,
	SKILL_EUNHYUNG,
	SKILL_SANGONG,

	SKILL_YEONSA = 46,
	SKILL_KWANKYEOK,
	SKILL_HWAJO,
	SKILL_GYEONGGONG,
	SKILL_GIGUNG,

	SKILL_SWAERYUNG = 61,
	SKILL_YONGKWON,
	SKILL_GWIGEOM,
	SKILL_TERROR,
	SKILL_JUMAGAP,
	SKILL_PABEOB,

	SKILL_MARYUNG = 76,
	SKILL_HWAYEOMPOK,
	SKILL_MUYEONG,
	SKILL_MANASHILED,
	SKILL_TUSOK,
	SKILL_MAHWAN,

	SKILL_BIPABU = 91,
	SKILL_YONGBI,
	SKILL_PAERYONG,

	SKILL_HOSIN,
	SKILL_REFLECT,
	SKILL_GICHEON,

	SKILL_NOEJEON = 106,
	SKILL_BYEURAK,
	SKILL_CHAIN,
	SKILL_JEONGEOP,
	SKILL_KWAESOK,
	SKILL_JEUNGRYEOK,

	SKILL_7_A_ANTI_TANHWAN = 112,
	SKILL_7_B_ANTI_AMSEOP,
	SKILL_7_C_ANTI_SWAERYUNG,
	SKILL_7_D_ANTI_YONGBI,
	SKILL_8_A_ANTI_GIGONGCHAM,
	SKILL_8_B_ANTI_YEONSA,
	SKILL_8_C_ANTI_MAHWAN,
	SKILL_8_D_ANTI_BYEURAK,

	SKILL_LEADERSHIP = 121,
	SKILL_COMBO = 122,
	SKILL_CREATE = 123,
	SKILL_MINING = 124,

	SKILL_LANGUAGE1 = 126,
	SKILL_LANGUAGE2 = 127,
	SKILL_LANGUAGE3 = 128,
	SKILL_POLYMORPH = 129,

	SKILL_HORSE = 130,
	SKILL_HORSE_SUMMON = 131,
	SKILL_HORSE_WILDATTACK = 137,
	SKILL_HORSE_CHARGE = 138,
	SKILL_HORSE_ESCAPE = 139,
	SKILL_HORSE_WILDATTACK_RANGE = 140,

	SKILL_ADD_HP = 141,
	SKILL_RESIST_PENETRATE = 142,

	GUILD_SKILL_START = 151,
	GUILD_SKILL_EYE = 151,
	GUILD_SKILL_BLOOD = 152,
	GUILD_SKILL_BLESS = 153,
	GUILD_SKILL_SEONGHWI = 154,
	GUILD_SKILL_ACCEL = 155,
	GUILD_SKILL_BUNNO = 156,
	GUILD_SKILL_JUMUN = 157,
	GUILD_SKILL_TELEPORT = 158,
	GUILD_SKILL_DOOR = 159,
	GUILD_SKILL_END = 162,
	GUILD_SKILL_COUNT = GUILD_SKILL_END - GUILD_SKILL_START + 1,

	// Wolfman Skills
	SKILL_CHAYEOL = 170,
	SKILL_SALPOONG = 171,
	SKILL_GONGDAB = 172,
	SKILL_PASWAE = 173,
	SKILL_JEOKRANG = 174,
	SKILL_CHEONGRANG = 175,

#ifdef ENABLE_7AND8TH_SKILLS_SYSTEM
	SKILL_ANTI_PALBANG = 221,
	SKILL_ANTI_AMSEOP = 222,
	SKILL_ANTI_SWAERYUNG = 223,
	SKILL_ANTI_YONGBI = 224,
	SKILL_ANTI_GIGONGCHAM = 225,
	SKILL_ANTI_HWAJO = 226,
	SKILL_ANTI_MARYUNG = 227,
	SKILL_ANTI_BYEURAK = 228,
#ifdef ENABLE_WOLFMAN_CHARACTER
	SKILL_ANTI_SALPOONG = 229,
#endif
	SKILL_HELP_PALBANG = 236,
	SKILL_HELP_AMSEOP = 237,
	SKILL_HELP_SWAERYUNG = 238,
	SKILL_HELP_YONGBI = 239,
	SKILL_HELP_GIGONGCHAM = 240,
	SKILL_HELP_HWAJO = 241,
	SKILL_HELP_MARYUNG = 242,
	SKILL_HELP_BYEURAK = 243,
#ifdef ENABLE_WOLFMAN_CHARACTER
	SKILL_HELP_SALPOONG = 244,
#endif
#endif
};

class CSkillProto
{
public:
	char	szName[64];
	DWORD	dwVnum;			// 번호

	DWORD	dwType;			// 0: 전직업, 1: 무사, 2: 자객, 3: 수라, 4: 무당
	BYTE	bMaxLevel;		// 최대 수련도
	BYTE	bLevelLimit;		// 레벨제한
	int	iSplashRange;		// 스플래쉬 거리 제한

	BYTE	bPointOn;		// 어디에 결과값을 적용 시키는가? (타격치, MAX HP, HP REGEN 등등등)
	CPoly	kPointPoly;		// 결과값 만드는 공식

	CPoly	kSPCostPoly;		// 사용 SP 공식
	CPoly	kDurationPoly;		// 지속 시간 공식
	CPoly	kDurationSPCostPoly;	// 지속 SP 공식
	CPoly	kCooldownPoly;		// 쿨다운 시간 공식
	CPoly	kMasterBonusPoly;	// 마스터일 때 보너스 공식
	CPoly	kSplashAroundDamageAdjustPoly;	// 스플래쉬 공격일 경우 주위 적에게 입히는 데미지 감소 비율

	DWORD	dwFlag;			// 스킬옵션
	DWORD	dwAffectFlag;		// 스킬에 맞은 경우 적용되는 Affect

	BYTE	bLevelStep;		// 한번에 올리는데 필요한 스킬 포인트 수
	DWORD	preSkillVnum;		// 배우는데 필요한 이전에 배워야할 스킬
	BYTE	preSkillLevel;		// 이전에 배워야할 스킬의 레벨

	long	lMaxHit;

	BYTE	bSkillAttrType;

	// 2차 적용
	BYTE	bPointOn2;
	CPoly	kPointPoly2;
	CPoly	kDurationPoly2;
	DWORD	dwFlag2;
	DWORD	dwAffectFlag2;

	DWORD   dwTargetRange;

	bool	IsChargeSkill()
	{
		return dwVnum == SKILL_TANHWAN || dwVnum == SKILL_HORSE_CHARGE;
	}

	// 3차 적용
	BYTE bPointOn3;
	CPoly kPointPoly3;
	CPoly kDurationPoly3;

	CPoly kGrandMasterAddSPCostPoly;

	void SetPointVar(const std::string& strName, double dVar);
	void SetDurationVar(const std::string& strName, double dVar);
	void SetSPCostVar(const std::string& strName, double dVar);
};

class CSkillManager : public singleton<CSkillManager>
{
public:
	CSkillManager();
	virtual ~CSkillManager();

	bool Initialize(TSkillTable* pTab, int iSize);
	CSkillProto* Get(DWORD dwVnum);
	CSkillProto* Get(const char* c_pszSkillName);

protected:
	std::map<DWORD, CSkillProto*> m_map_pkSkillProto;
};

#endif
