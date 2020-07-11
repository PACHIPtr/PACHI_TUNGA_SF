#include "stdafx.h"
#include "char.h"
TJobInitialPoints JobInitialPoints[JOB_MAX_NUM] =
/*
   {
   int st, ht, dx, iq;
   int max_hp, max_sp;
   int hp_per_ht, sp_per_iq;
   int hp_per_lv_begin, hp_per_lv_end;
   int sp_per_lv_begin, sp_per_lv_end;
   int max_stamina;
   int stamina_per_con;
   int stamina_per_lv_begin, stamina_per_lv_end;
   }
 */
{
	{6, 4, 3, 3, 600, 200, 40, 20, 44, 44, 18, 22, 800, 5, 1, 3}, // WARRIOR
	{4, 3, 6, 3, 650, 200, 40, 20, 44, 44, 18, 22, 800, 5, 1, 3}, // ASSASSIN
	{5, 3, 3, 5, 650, 200, 40, 20, 44, 44, 18, 22, 800, 5, 1, 3}, // SURA
	{3, 4, 3, 6, 700, 200, 40, 20, 44, 44, 18, 22, 800, 5, 1, 3}, // SHAMAN
	{6, 6, 2, 2, 600, 200, 40, 20, 44, 44, 18, 22, 800, 5, 1, 3}, // WOLF
};

const TMobRankStat MobRankStats[MOB_RANK_MAX_NUM] =
/*
   {
   int         iGoldPercent;
   }
 */
{
	{  20,  }, // MOB_RANK_PAWN,
	{  20,  }, // MOB_RANK_S_PAWN,
	{  25,  }, // MOB_RANK_KNIGHT,
	{  30,  }, // MOB_RANK_S_KNIGHT,
	{  50,  }, // MOB_RANK_BOSS,
	{ 100,  }  // MOB_RANK_KING,
};

TBattleTypeStat BattleTypeStats[BATTLE_TYPE_MAX_NUM] =
/*
   {
   int         AttGradeBias;
   int         DefGradeBias;
   int         MagicAttGradeBias;
   int         MagicDefGradeBias;
   }
 */
{
	{	  0,	  0,	  0,	-10	}, // BATTLE_TYPE_MELEE,
	{	 10,	-20,	-10,	-15	}, // BATTLE_TYPE_RANGE,
	{	 -5,	 -5,	 10,	 10	}, // BATTLE_TYPE_MAGIC,
	{	  0,	  0,	  0,	  0	}, // BATTLE_TYPE_SPECIAL,
	{	 10,	-10,	  0,	-15	}, // BATTLE_TYPE_POWER,
	{	-10,	 10,	-10,	  0	}, // BATTLE_TYPE_TANKER,
	{	 20,	-20,	  0,	-10	}, // BATTLE_TYPE_SUPER_POWER,
	{	-20,	 20,	-10,	  0	}, // BATTLE_TYPE_SUPER_TANKER,
};

const DWORD* exp_table = nullptr;

#ifdef ENABLE_GROWTH_PET_SYSTEM
DWORD* exppet_table = nullptr;
//const DWORD pets = 4;
const DWORD Pet_Table[6][2] =
{
	{ 55701, 34041 },
	{ 55702, 34045 },
	{ 55703, 34049 },
	{ 55704, 34053 },
	{ 55705, 34057 },
	{ 55706, 34061 }
};

const DWORD Pet_Skill_Table[20][23] =
{
	//Level Like Index ->> Bonus Value --> typeSkill --> CoolDown -->> Percentage
	{ 76, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 78, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 77, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 79, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 93, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 }, //5
	{ 64, 0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 10 },
	{ 95, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12 },
	{ 9, 0, 0, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17 },
	{ 16, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9 },
	{ 4, 1, 480, 17, 68, 153, 272, 425, 612, 833, 1088, 1377, 1700, 2057, 2448, 2873, 3332, 3825, 4352, 4913, 5508, 6137, 6800 }, //10
	{ 23, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5 },
	{ 24, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 6, 6 },
	{ 27, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11 },
	{ 28, 0, 0, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7 },
	{ 44, 0, 0, 5, 10, 15, 20, 25, 31, 36, 41, 46, 51, 56, 61, 67, 72, 77, 82, 87, 92, 97, 103 }, //15
	{ 52, 0, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6 },
	{ 11, 1, 600, 2, 4, 6, 7, 8, 9, 11, 12, 13, 15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27 },
	{ 6, 1, 480, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 109, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // metin
	{ 63, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 } // canavar
};

const DWORD Pet_SKill_TablePerc[3][20] =
{
	{ 2, 5, 8, 11, 13, 16, 19, 22, 24, 27, 30, 33, 35, 38, 41, 44, 46, 49, 52, 55 },
	{ 2, 5, 7, 10, 12, 15, 17, 20, 23, 25, 27, 30, 32, 35, 37, 40, 42, 45, 47, 50 },
	{ 2, 5, 8, 11, 13, 16, 19, 22, 25, 27, 30, 33, 36, 38, 41, 44, 47, 50, 52, 55 }
};

DWORD exppet_table_common[105];
#endif

#ifdef ENABLE_BUFFI_SYSTEM
const DWORD exp_supportshaman_table[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,
	300,
	800,
	1500,
	2500,
	4300,
	7200,
	11000,
	17000,
	24000,
	33001,
	43000,
	58000,
	76000,
	100000,
	130000,
	169000,
	219000,
	283000,
	365000,
	472000,
	610000,
	705000,
	813000,
	937000,
	1077000,
	1237000,
	1418000,
	1624000,
	1857000,
	2122000,
	2421000,
	2761000,
	3145000,
	3580000,
	4073000,
	4632000,
	5194000,
	5717000,
	6264000,
	6837000,
	7600000,
	8274000,
	8990000,
	9753000,
	10560000,
	11410000,
	12320000,
	13270000,
	14280000,
	15340000,
	16870000,
	18960000,
	19980000,
	21420000,
	22930000,
	24530000,
	26200000,
	27960000,
	29800000,
	32780000,
	36060000,
	39670000,
	43640000,
	48000000,
	52800000,
	58080000,
	63890000,
	70280000,
	77310000,
	85040000,
	93540000,
	102900000,
	113200000,
	124500000,
	137000000,
	150700000,
	165700000,
	236990000,
	260650000,
	286780000,
	315380000,
	346970000,
	381680000,
	419770000,
	461760000,
	508040000,
	558740000,
	614640000,
	676130000,
	743730000,
	1041222000,
	1145344200,
	1259878620,
	1385866482,
	1524453130,
	1676898443,
	1844588288,
	2029047116,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
	2147483646,
};
#endif

const DWORD exp_table_euckr[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,			// 0

	100,
	150,
	260,
	380,
	600,

	1300,
	3300,
	5700,
	8700,
	12800,		// 10
	18000,
	25000,
	36000,
	52000,
	73000,
	100000,
	125000,
	160000,
	220000,
	280000,		// 20
	370000,
	540000,
	670000,
	880000,
	1000000,

	1237000,
	1418000,
	1624000,
	1857000,
	2122000,	// 30
	2421000,
	2761000,
	3145000,
	3580000,
	4073000,
	4632000,
	5194000,
	5717000,
	6264000,
	6837000,	// 40
	7600000,
	8274000,
	8990000,
	9753000,
	10560000,
	11410000,
	12320000,
	13270000,
	14280000,
	15340000,	// 50
	16870000,
	18960000,
	19980000,
	21420000,
	22930000,
	24530000,
	26200000,
	27960000,
	29800000,
	32780000,	// 60
	36060000,
	39670000,
	43640000,
	48000000,
	52800000,
	58080000,
	63890000,
	70280000,
	77310000,
	85040000, 	// 70
	93540000,
	102900000,
	113200000,
	124500000,
	137000000,
	150700000,
	165700000,
	236990000,
	260650000,
	286780000,	// 80
	315380000,
	346970000,
	381680000,
	419770000,
	461760000,
	508040000,
	558740000,
	614640000,
	676130000,
	743730000,	// 90
	1041222000,
	1145344200,
	1259878620,
	1385866482,
	1524453130,
	1676898443,
	1844588288,
	2029047116,
	2100000000,	//	99		99레벨일 때 필요경험치 (100레벨이 되기 위한)
	2100000000,	//	100
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	105
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	110
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	115
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	120
};

const DWORD exp_table_common[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,	//	0
	300,		//500
	800,		//700
	1500,		//1000
	2500,
	4300,
	7200,
	11000,
	17000,
	24000,
	33000,	//	10
	43000,
	58000,
	76000,
	100000,
	130000,
	169000,
	219000,
	283000,
	365000,
	472000,	//	20
	610000,
	705000,
	813000,
	937000,
	1077000,
	1237000,
	1418000,
	1624000,
	1857000,
	2122000,	//	30
	2421000,
	2761000,
	3145000,
	3580000,
	4073000,
	4632000,
	5194000,
	5717000,
	6264000,
	6837000,	//	40
	7600000,
	8274000,
	8990000,
	9753000,
	10560000,
	11410000,
	12320000,
	13270000,
	14280000,
	15340000,	//	50
	16870000,
	18960000,
	19980000,
	21420000,
	22930000,
	24530000,
	26200000,
	27960000,
	29800000,
	32780000,	//	60
	36060000,
	39670000,
	43640000,
	48000000,
	52800000,
	58080000,
	63890000,
	70280000,
	77310000,
	85040000,	//	70
	93540000,
	102900000,
	113200000,
	124500000,
	137000000,
	150700000,
	165700000,
	236990000,
	260650000,
	286780000,	//	80
	315380000,
	346970000,
	381680000,
	419770000,
	461760000,
	508040000,
	558740000,
	614640000,
	676130000,
	743730000,	//	90
	1041222000,
	1145344200,
	1259878620,
	1385866482,
	1524453130,
	1676898443,
	1844588288,
	1929047116,
	2000000000,	//	99레벨 일 때 필요경험치 (100레벨이 되기 위한)
	2020000000,	//	100
	2040000000,
	2060000000,
	2080000000,
	2090000000,
	2100000000,	//	105
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	110
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	115
	2100000000,
	2100000000,
	2100000000,
	2100000000,
	2100000000,	//	120
	2251500000,
	2253000000,
	2254500000,
	2256000000,
	2257500000,	//	125
	2259000000,
	2260500000,
	2262000000,
	2263500000,
	2265000000,	//	130
	2266500000,
	2268000000,
	2269500000,
	2271000000,
	2272500000,	//	135
	2274000000,
	2275500000,
	2277000000,
	2278500000,
	2280000000,	//	140
	2281800000,
	2283600000,
	2285400000,
	2287200000,
	2289000000,	//	145
	2290800000,
	2292600000,
	2294400000,
	2296200000,
	2298000000,	//	150
	2299800000,
	2301600000,
	2303400000,
	2305200000,
	2307000000,	//	155
	2308800000,
	2310600000,
	2312400000,
	2314200000,
	2316000000,	//	160
	2317800000,
	2319600000,
	2321400000,
	2323200000,
	2325000000,	//	165
	2326800000,
	2328600000,
	2330400000,
	2332200000,
	2334000000,	//	170
	2335800000,
	2337600000,
	2339400000,
	2341200000,
	2343000000,	//	175
	2344800000,
	2346600000,
	2348400000,
	2350200000,
	2352000000,	//	180
	2353800000,
	2355600000,
	2357400000,
	2359200000,
	2361000000,	//	185
	2362800000,
	2364600000,
	2366400000,
	2368200000,
	2370000000,	//	190
	2371800000,
	2373600000,
	2375400000,
	2377200000,
	2379000000,	//	195
	2380800000,
	2382600000,
	2384400000,
	2386200000,
	2388000000,	//	200
	2389800000,
	2391600000,
	2393400000,
	2395200000,
	2397000000,	//	205
	2398800000,
	2400600000,
	2402400000,
	2404200000,
	2406000000,	//	210
	2407800000,
	2409600000,
	2411400000,
	2413200000,
	2415000000,	//	215
	2416800000,
	2418600000,
	2420400000,
	2422200000,
	2424000000,	//	220
	2425800000,
	2427600000,
	2429400000,
	2431200000,
	2433000000,	//	225
	2434800000,
	2436600000,
	2438400000,
	2440200000,
	2442000000,	//	230
	2443800000,
	2445600000,
	2447400000,
	2449200000,
	2451000000,	//	235
	2452800000,
	2454600000,
	2456400000,
	2458200000,
	2460000000,	//	240
	2461800000,
	2463600000,
	2465400000,
	2467200000,
	2469000000,	//	245
	2470800000,
	2472600000,
	2474400000,
	2478000000,
	2500000000,	//	250
};

const DWORD exp_table_newcibn[PLAYER_EXP_TABLE_MAX + 1] =
{
	300,	// 0
	800,	// 1
	1500,
	2500,
	4300,
	7200,
	11400,
	17000,
	24000,
	33000,
	46000,	// 10
	62000,
	83000,
	111000,
	149000,
	200000,
	268000,
	360000,
	482000,
	647000,
	868000,	// 20
	996000,
	1143000,
	1312000,
	1506000,
	1729000,
	1984000,
	2277000,
	2614000,
	3000000,
	3443000,	// 30
	3952000,
	4536000,
	5206000,
	5975000,
	6858000,
	7730000,
	8504000,
	9307000,
	10140000,
	11330000,	// 40
	12320000,
	13370000,
	14490000,
	15670000,
	16920000,
	18240000,
	19630000,
	21090000,
	22630000,
	24670000,	// 50
	26890000,
	29310000,
	31950000,
	34820000,
	37960000,
	41370000,
	45100000,
	49160000,
	53580000,
	58400000,	// 60
	63660000,
	69390000,
	75630000,
	82440000,
	89860000,
	97950000,
	106760000,
	136370000,
	151800000,
	168300000,	// 70
	250000000,
	340000000,
	450000000,
	570000000,
	690000000,
	810000000,
	912600000,
	1004000000,
	1094000000,
	1182000000,	// 80
	1269000000,
	1354000000,
	1438000000,
	1521000000,
	1603000000,
	1684000000,
	1764000000,
	1844000000,
	1922000000,
	2000000000,	// 90
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	99		99레벨일 때 필요경험치 (100레벨이 되기 위한).. 현재 CIBN이 어떻게 운영하고 있는 지 모르니 신규 테이블을 쓰지 않고 기존값 계속 연장 유지
	2000000000,	//	100
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	105
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	110
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	115
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	120
};

const int* aiPercentByDeltaLev = nullptr;
const int* aiPercentByDeltaLevForBoss = nullptr;

// 적과 나와의 레벨차이에 의한 계산에 사용되는 테이블
// MIN(MAX_EXP_DELTA_OF_LEV - 1, (적렙 + 15) - 내렙))
const int aiPercentByDeltaLevForBoss_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,      // -15  0
	3,          // -14  1
	5,          // -13  2
	7,          // -12  3
	15,         // -11  4
	30,         // -10  5
	60,         // -9   6
	90,         // -8   7
	91,         // -7   8
	92,         // -6   9
	93,         // -5   10
	94,         // -4   11
	95,         // -3   12
	97,         // -2   13
	99,         // -1   14
	100,        // 0    15
	105,        // 1    16
	110,        // 2    17
	115,        // 3    18
	120,        // 4    19
	125,        // 5    20
	130,        // 6    21
	135,        // 7    22
	140,        // 8    23
	145,        // 9    24
	150,        // 10   25
	155,        // 11   26
	160,        // 12   27
	165,        // 13   28
	170,        // 14   29
	180         // 15   30
};

const int aiPercentByDeltaLev_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,  //  -15 0
	5,  //  -14 1
	10, //  -13 2
	20, //  -12 3
	30, //  -11 4
	50, //  -10 5
	70, //  -9  6
	80, //  -8  7
	85, //  -7  8
	90, //  -6  9
	92, //  -5  10
	94, //  -4  11
	96, //  -3  12
	98, //  -2  13
	100,    //  -1  14
	100,    //  0   15
	105,    //  1   16
	110,    //  2   17
	115,    //  3   18
	120,    //  4   19
	125,    //  5   20
	130,    //  6   21
	135,    //  7   22
	140,    //  8   23
	145,    //  9   24
	150,    //  10  25
	155,    //  11  26
	160,    //  12  27
	165,    //  13  28
	170,    //  14  29
	180,    //  15  30
};

const DWORD party_exp_distribute_table[PLAYER_MAX_LEVEL_CONST + 1] =
{
	0,
	10,		10,		10,		10,		15,		15,		20,		25,		30,		40,		// 1 - 10
	50,		60,		80,		100,	120,	140,	160,	184,	210,	240,	// 11 - 20
	270,	300,	330,	360,	390,	420,	450,	480,	510,	550,	// 21 - 30
	600,	640,	700,	760,	820,	880,	940,	1000,	1100,	1180,	// 31 - 40
	1260,	1320,	1380,	1440,	1500,	1560,	1620,	1680,	1740,	1800,	// 41 - 50
	1860,	1920,	2000,	2100,	2200,	2300,	2450,	2600,	2750,	2900,	// 51 - 60
	3050,	3200,	3350,	3500,	3650,	3800,	3950,	4100,	4250,	4400,	// 61 - 70
	4600,	4800,	5000,	5200,	5400,	5600,	5800,	6000,	6200,	6400,	// 71 - 80
	6600,	6800,	7000,	7200,	7400,	7600,	7800,	8000,	8200,	8400,	// 81 - 90
	8600,	8800,	9000,	9200,	9400,	9600,	9800,	10000,	10200,	10400,	// 91 - 100
	10600,	10800,	11000,	11200,	11400,	11600,	11800,	12000,	12200,	12400,	// 101 - 110
	12600,	12800,	13000,	13200,	13400,	13600,	13800,	14000,	14200,	14400,	// 111 - 120
	14600,	14800,	15000,	15200,	15400,	15600,	15800,	16000,	16200,	16400,	// 121 - 130
	16600,	16800,	17000,	17200,	17400,	17600,	17800,	18000,	18200,	18400,	// 131 - 140
	18600,	18800,	19000,	19200,	19400,	19600,	19800,	20000,	20200,	20400,	// 141 - 150
	20600,	20800,	21000,	21200,	21400,	21600,	21800,	22000,	22200,	22400,	// 151 - 160
	22600,	22800,	23000,	23200,	23400,	23600,	23800,	24000,	24200,	24400,	// 161 - 170
	24600,	24800,	25000,	25200,	25400,	25600,	25800,	26000,	26200,	26400,	// 171 - 180
	26600,	26800,	27000,	27200,	27400,	27600,	27800,	28000,	28200,	28400,	// 181 - 190
	28600,	28800,	29000,	29200,	29400,	29600,	29800,	30000,	30200,	30400,	// 191 - 200
	30600,	30800,	31000,	31200,	31400,	31600,	31800,	32000,	32200,	32400,	// 201 - 210
	32600,	32800,	33000,	33200,	33400,	33600,	33800,	34000,	34200,	34400,	// 211 - 220
	34600,	34800,	35000,	35200,	35400,	35600,	35800,	36000,	36200,	36400,	// 221 - 230
	36600,	36800,	37000,	37200,	37400,	37600,	37800,	38000,	38200,	38400,	// 231 - 240
	38600,	38800,	39000,	39200,	39400,	39600,	39800,	40000,	40200,	40400,	// 241 - 250
};

Coord aArroundCoords[ARROUND_COORD_MAX_NUM] =
{
	{	     0,	      0	    },
	{        0,      50     },
	{       35,     35      },
	{       50,     -0      },
	{       35,     -35     },
	{       0,      -50     },
	{       -35,    -35     },
	{       -50,    0       },
	{       -35,    35      },
	{       0,      100     },
	{       71,     71      },
	{       100,    -0      },
	{       71,     -71     },
	{       0,      -100    },
	{       -71,    -71     },
	{       -100,   0       },
	{       -71,    71      },
	{       0,      150     },
	{       106,    106     },
	{       150,    -0      },
	{       106,    -106    },
	{       0,      -150    },
	{       -106,   -106    },
	{       -150,   0       },
	{       -106,   106     },
	{       0,      200     },
	{       141,    141     },
	{       200,    -0      },
	{       141,    -141    },
	{       0,      -200    },
	{       -141,   -141    },
	{       -200,   0       },
	{       -141,   141     },
	{       0,      250     },
	{       177,    177     },
	{       250,    -0      },
	{       177,    -177    },
	{       0,      -250    },
	{       -177,   -177    },
	{       -250,   0       },
	{       -177,   177     },
	{       0,      300     },
	{       212,    212     },
	{       300,    -0      },
	{       212,    -212    },
	{       0,      -300    },
	{       -212,   -212    },
	{       -300,   0       },
	{       -212,   212     },
	{       0,      350     },
	{       247,    247     },
	{       350,    -0      },
	{       247,    -247    },
	{       0,      -350    },
	{       -247,   -247    },
	{       -350,   0       },
	{       -247,   247     },
	{       0,      400     },
	{       283,    283     },
	{       400,    -0      },
	{       283,    -283    },
	{       0,      -400    },
	{       -283,   -283    },
	{       -400,   0       },
	{       -283,   283     },
	{       0,      450     },
	{       318,    318     },
	{       450,    -0      },
	{       318,    -318    },
	{       0,      -450    },
	{       -318,   -318    },
	{       -450,   0       },
	{       -318,   318     },
	{       0,      500     },
	{       354,    354     },
	{       500,    -0      },
	{       354,    -354    },
	{       0,      -500    },
	{       -354,   -354    },
	{       -500,   0       },
	{       -354,   354     },
	{       0,      550     },
	{       389,    389     },
	{       550,    -0      },
	{       389,    -389    },
	{       0,      -550    },
	{       -389,   -389    },
	{       -550,   0       },
	{       -389,   389     },
	{       0,      600     },
	{       424,    424     },
	{       600,    -0      },
	{       424,    -424    },
	{       0,      -600    },
	{       -424,   -424    },
	{       -600,   0       },
	{       -424,   424     },
	{       0,      650     },
	{       460,    460     },
	{       650,    -0      },
	{       460,    -460    },
	{       0,      -650    },
	{       -460,   -460    },
	{       -650,   0       },
	{       -460,   460     },
	{       0,      700     },
	{       495,    495     },
	{       700,    -0      },
	{       495,    -495    },
	{       0,      -700    },
	{       -495,   -495    },
	{       -700,   0       },
	{       -495,   495     },
	{       0,      750     },
	{       530,    530     },
	{       750,    -0      },
	{       530,    -530    },
	{       0,      -750    },
	{       -530,   -530    },
	{       -750,   0       },
	{       -530,   530     },
	{       0,      800     },
	{       566,    566     },
	{       800,    -0      },
	{       566,    -566    },
	{       0,      -800    },
	{       -566,   -566    },
	{       -800,   0       },
	{       -566,   566     },
	{       0,      850     },
	{       601,    601     },
	{       850,    -0      },
	{       601,    -601    },
	{       0,      -850    },
	{       -601,   -601    },
	{       -850,   0       },
	{       -601,   601     },
	{       0,      900     },
	{       636,    636     },
	{       900,    -0      },
	{       636,    -636    },
	{       0,      -900    },
	{       -636,   -636    },
	{       -900,   0       },
	{       -636,   636     },
	{       0,      950     },
	{       672,    672     },
	{       950,    -0      },
	{       672,    -672    },
	{       0,      -950    },
	{       -672,   -672    },
	{       -950,   0       },
	{       -672,   672     },
	{       0,      1000    },
	{       707,    707     },
	{       1000,   -0      },
	{       707,    -707    },
	{       0,      -1000   },
	{       -707,   -707    },
	{       -1000,  0       },
	{       -707,   707     },
};

const DWORD guild_exp_table[GUILD_MAX_LEVEL + 1] =
{
	0,
	15000UL,
	45000UL,
	90000UL,
	160000UL,
	235000UL,
	325000UL,
	430000UL,
	550000UL,
	685000UL,
	835000UL,
	1000000UL,
	1500000UL,
	2100000UL,
	2800000UL,
	3600000UL,
	4500000UL,
	6500000UL,
	8000000UL,
	10000000UL,
	42000000UL
};

// INTERNATIONAL_VERSION 길드경험치
const DWORD guild_exp_table2[GUILD_MAX_LEVEL + 1] =
{
	0,
	6000UL,
	18000UL,
	36000UL,
	64000UL,
	94000UL,
	130000UL,
	172000UL,
	220000UL,
	274000UL,
	334000UL,
	400000UL,
	600000UL,
	840000UL,
	1120000UL,
	1440000UL,
	1800000UL,
	2600000UL,
	3200000UL,
	4000000UL,
	16800000UL
};
// END_OF_INTERNATIONAL_VERSION 길드경험치

const int aiMobEnchantApplyIdx[MOB_ENCHANTS_MAX_NUM] =
{
	APPLY_CURSE_PCT,
	APPLY_SLOW_PCT,
	APPLY_POISON_PCT,
	APPLY_STUN_PCT,
	APPLY_CRITICAL_PCT,
	APPLY_PENETRATE_PCT,
};

const int aiMobResistsApplyIdx[MOB_RESISTS_MAX_NUM] =
{
	APPLY_RESIST_SWORD,
	APPLY_RESIST_TWOHAND,
	APPLY_RESIST_DAGGER,
	APPLY_RESIST_BELL,
	APPLY_RESIST_FAN,
	APPLY_RESIST_BOW,
	APPLY_RESIST_FIRE,
	APPLY_RESIST_ELEC,
	APPLY_RESIST_MAGIC,
	APPLY_RESIST_WIND,
	APPLY_POISON_REDUCE,
};

const int aiSocketPercentByQty[5][4] =
{
	{  0,  0,  0,  0 },
	{  3,  0,  0,  0 },
	{ 10,  1,  0,  0 },
	{ 15, 10,  1,  0 },
	{ 20, 15, 10,  1 }
};

const int aiWeaponSocketQty[WEAPON_NUM_TYPES] =
{
	4, // WEAPON_SWORD,
	4, // WEAPON_DAGGER,
	4, // WEAPON_BOW,
	4, // WEAPON_TWO_HANDED,
	4, // WEAPON_BELL,
	4, // WEAPON_FAN,
	0, // WEAPON_ARROW,
	0, // WEAPON_MOUNT_SPEAR,
	4, // WEAPON_CLAW
#ifdef ENABLE_QUIVER_SYSTEM
	0, // WEAPON_QUIVER
#endif
};

const int aiArmorSocketQty[ARMOR_NUM_TYPES] =
{
	4, // ARMOR_BODY,
	1, // ARMOR_HEAD,
	1, // ARMOR_SHIELD,
	0, // ARMOR_WRIST,
	0, // ARMOR_FOOTS,
	0  // ARMOR_ACCESSORY
};

TItemAttrMap g_map_itemAttr;
TItemAttrMap g_map_itemRare;

const TApplyInfo aApplyInfo[MAX_APPLY_NUM] =
{
	{POINT_NONE,},						// apply_name = APPLY_NONE & apply_value = 0
	{POINT_MAX_HP,},					// apply_name = APPLY_MAX_HP & apply_value = 1
	{POINT_MAX_SP,},					// apply_name = APPLY_MAX_SP & apply_value = 2
	{POINT_HT,},						// apply_name = APPLY_CON & apply_value = 3
	{POINT_IQ,},						// apply_name = APPLY_INT & apply_value = 4
	{POINT_ST,},						// apply_name = APPLY_STR & apply_value = 5
	{POINT_DX,},						// apply_name = APPLY_DEX & apply_value = 6
	{POINT_ATT_SPEED,},					// apply_name = APPLY_ATT_SPEED & apply_value = 7
	{POINT_MOV_SPEED,},					// apply_name = APPLY_MOV_SPEED & apply_value = 8
	{POINT_CASTING_SPEED,},				// apply_name = APPLY_CAST_SPEED & apply_value = 9
	{POINT_HP_REGEN,},					// apply_name = APPLY_HP_REGEN & apply_value = 10
	{POINT_SP_REGEN,},					// apply_name = APPLY_SP_REGEN & apply_value = 11
	{POINT_POISON_PCT,},				// apply_name = APPLY_POISON_PCT & apply_value = 12
	{POINT_STUN_PCT,},   				// apply_name = APPLY_STUN_PCT & apply_value = 13
	{POINT_SLOW_PCT,},					// apply_name = APPLY_SLOW_PCT & apply_value = 14
	{POINT_CRITICAL_PCT,},				// apply_name = APPLY_CRITICAL_PCT & apply_value = 15
	{POINT_PENETRATE_PCT,},				// apply_name = APPLY_PENETRATE_PCT & apply_value = 16
	{POINT_ATTBONUS_HUMAN,},			// apply_name = APPLY_ATTBONUS_HUMAN & apply_value = 17
	{POINT_ATTBONUS_ANIMAL,},			// apply_name = APPLY_ATTBONUS_ANIMAL & apply_value = 18
	{POINT_ATTBONUS_ORC,},				// apply_name = APPLY_ATTBONUS_ORC & apply_value = 19
	{POINT_ATTBONUS_MILGYO,},			// apply_name = APPLY_ATTBONUS_MILGYO & apply_value = 20
	{POINT_ATTBONUS_UNDEAD,},			// apply_name = APPLY_ATTBONUS_UNDEAD & apply_value = 21
	{POINT_ATTBONUS_DEVIL,},			// apply_name = APPLY_ATTBONUS_DEVIL & apply_value = 22
	{POINT_STEAL_HP,},					// apply_name = APPLY_STEAL_HP & apply_value = 23
	{POINT_STEAL_SP,},					// apply_name = APPLY_STEAL_SP & apply_value = 24
	{POINT_MANA_BURN_PCT,},				// apply_name = APPLY_MANA_BURN_PCT & apply_value = 25
	{POINT_DAMAGE_SP_RECOVER,},			// apply_name = APPLY_DAMAGE_SP_RECOVER & apply_value = 26
	{POINT_BLOCK,},						// apply_name = APPLY_BLOCK & apply_value = 27
	{POINT_DODGE,},						// apply_name = APPLY_DODGE & apply_value = 28
	{POINT_RESIST_SWORD,},				// apply_name = APPLY_RESIST_SWORD & apply_value = 29
	{POINT_RESIST_TWOHAND,},			// apply_name = APPLY_RESIST_TWOHAND & apply_value = 30
	{POINT_RESIST_DAGGER,},				// apply_name = APPLY_RESIST_DAGGER & apply_value = 31
	{POINT_RESIST_BELL,},				// apply_name = APPLY_RESIST_BELL & apply_value = 32
	{POINT_RESIST_FAN,},				// apply_name = APPLY_RESIST_FAN & apply_value = 33
	{POINT_RESIST_BOW,},				// apply_name = APPLY_RESIST_BOW & apply_value = 34
	{POINT_RESIST_FIRE,},				// apply_name = APPLY_RESIST_FIRE & apply_value = 35
	{POINT_RESIST_ELEC,},				// apply_name = APPLY_RESIST_ELEC & apply_value = 36
	{POINT_RESIST_MAGIC,},				// apply_name = APPLY_RESIST_MAGIC & apply_value = 37
	{POINT_RESIST_WIND,},				// apply_name = APPLY_RESIST_WIND & apply_value = 38
	{POINT_REFLECT_MELEE,},				// apply_name = APPLY_REFLECT_MELEE & apply_value = 39
	{POINT_REFLECT_CURSE,},				// apply_name = APPLY_REFLECT_CURSE & apply_value = 40
	{POINT_POISON_REDUCE,},				// apply_name = APPLY_POISON_REDUCE & apply_value = 41
	{POINT_KILL_SP_RECOVER,},			// apply_name = APPLY_KILL_SP_RECOVER & apply_value = 42
	{POINT_EXP_DOUBLE_BONUS,},			// apply_name = APPLY_EXP_DOUBLE_BONUS & apply_value = 43
	{POINT_GOLD_DOUBLE_BONUS,},			// apply_name = APPLY_GOLD_DOUBLE_BONUS & apply_value = 44
	{POINT_ITEM_DROP_BONUS,},			// apply_name = APPLY_ITEM_DROP_BONUS & apply_value = 45
	{POINT_POTION_BONUS,},				// apply_name = APPLY_POTION_BONUS & apply_value = 46
	{POINT_KILL_HP_RECOVERY,},			// apply_name = APPLY_KILL_HP_RECOVER & apply_value = 47
	{POINT_IMMUNE_STUN,},				// apply_name = APPLY_IMMUNE_STUN & apply_value = 48
	{POINT_IMMUNE_SLOW,},				// apply_name = APPLY_IMMUNE_SLOW & apply_value = 49
	{POINT_IMMUNE_FALL,},				// apply_name = APPLY_IMMUNE_FALL & apply_value = 50
	{POINT_NONE,},						// apply_name = APPLY_SKILL & apply_value = 51
	{POINT_BOW_DISTANCE,},				// apply_name = APPLY_BOW_DISTANCE & apply_value = 52
	{POINT_ATT_GRADE_BONUS,},			// apply_name = APPLY_ATT_GRADE & apply_value = 53
	{POINT_DEF_GRADE_BONUS,},			// apply_name = APPLY_DEF_GRADE & apply_value = 54
	{POINT_MAGIC_ATT_GRADE_BONUS,},		// apply_name = APPLY_MAGIC_ATT_GRADE & apply_value = 55
	{POINT_MAGIC_DEF_GRADE_BONUS,},		// apply_name = APPLY_MAGIC_DEF_GRADE & apply_value = 56
	{POINT_CURSE_PCT,},					// apply_name = APPLY_CURSE_PCT & apply_value = 57
	{POINT_MAX_STAMINA},				// apply_name = APPLY_MAX_STAMINA & apply_value = 58
	{POINT_ATTBONUS_WARRIOR},			// apply_name = APPLY_ATTBONUS_WARRIOR & apply_value = 59
	{POINT_ATTBONUS_ASSASSIN},			// apply_name = APPLY_ATTBONUS_ASSASSIN & apply_value = 60
	{POINT_ATTBONUS_SURA},				// apply_name = APPLY_ATTBONUS_SURA & apply_value = 61
	{POINT_ATTBONUS_SHAMAN},			// apply_name = APPLY_ATTBONUS_SHAMAN & apply_value = 62
	{POINT_ATTBONUS_MONSTER},			// apply_name = APPLY_ATTBONUS_MONSTER & apply_value = 63
	{POINT_ATT_BONUS},					// apply_name = APPLY_MALL_ATTBONUS  & apply_value = 64
	{POINT_MALL_DEFBONUS},				// apply_name = NONE & apply_value = 65
	{POINT_MALL_EXPBONUS},				// apply_name = APPLY_MALL_EXPBONUS & apply_value = 66
	{POINT_MALL_ITEMBONUS},				// apply_name = NONE & apply_value = 67
	{POINT_MALL_GOLDBONUS},				// apply_name = NONE & apply_value = 68
	{POINT_MAX_HP_PCT},					// apply_name = NONE & apply_value = 69
	{POINT_MAX_SP_PCT},					// apply_name = NONE & apply_value = 70
	{POINT_SKILL_DAMAGE_BONUS},			// apply_name = NONE & apply_value = 71
	{POINT_NORMAL_HIT_DAMAGE_BONUS},	// apply_name = NONE & apply_value = 72
	{POINT_SKILL_DEFEND_BONUS},			// apply_name = NONE & apply_value = 73
	{POINT_NORMAL_HIT_DEFEND_BONUS},	// apply_name = NONE & apply_value = 74
	{APPLY_EXTRACT_HP_PCT,},						// apply_name = APPLY_EXTRACT_HP_PCT & apply_value = 77
	{POINT_RESIST_WARRIOR,},			// apply_name = APPLY_RESIST_WARRIOR & apply_value = 78
	{POINT_RESIST_ASSASSIN,},			// apply_name = APPLY_RESIST_ASSASSIN & apply_value = 79
	{POINT_RESIST_SURA,},				// apply_name = APPLY_RESIST_SURA & apply_value = 80
	{POINT_RESIST_SHAMAN,},				// apply_name = APPLY_RESIST_SHAMAN & apply_value = 81
	{POINT_ENERGY},						// apply_name = NONE & apply_value = 82
	{POINT_DEF_GRADE},					// apply_name = DEF_GRADE_BONUS & apply_value = 83
	{POINT_COSTUME_ATTR_BONUS},			// apply_name = NONE & apply_value = 84
	{POINT_MAGIC_ATT_BONUS_PER},		// apply_name = NONE & apply_value = 85
	{POINT_MELEE_MAGIC_ATT_BONUS_PER},	// apply_name = APPLY_MELEE_MAGIC_ATTBONUS_PER & apply_value = 86
	{POINT_RESIST_ICE,},				// apply_name = APPLY_RESIST_ICE & apply_value = 87
	{POINT_RESIST_EARTH,},				// apply_name = APPLY_RESIST_EARTH & apply_value = 88
	{POINT_RESIST_DARK,},				// apply_name = APPLY_RESIST_DARK & apply_value = 89
	{POINT_RESIST_CRITICAL,},			// apply_name = APPLY_ANTI_CRITICAL_PCT & apply_value = 90
	{POINT_RESIST_PENETRATE,},			// apply_name = APPLY_ANTI_PENETRATE_PCT & apply_value = 91
	{POINT_BLEEDING_PCT,}, 				// apply_name = APPLY_BLEEDING_PCT & apply_value = 92
	{POINT_BLEEDING_REDUCE,}, 			// apply_name = APPLY_BLEEDING_REDUCE & apply_value = 93
	{POINT_ATTBONUS_WOLFMAN,},   		// apply_name = APPLY_ATTBONUS_WOLFMAN & apply_value = 94
	{POINT_RESIST_WOLFMAN,},   			// apply_name = APPLY_RESIST_WOLFMAN & apply_value =95

	{POINT_RESIST_CLAW,},   			// apply_name = APPLY_RESIST_CLAW & apply_value = 96
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	{POINT_ANTI_RESIST_MAGIC,},	// APPLY_ANTI_RESIST_MAGIC (type: 97)
#endif
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	{ POINT_ATTBONUS_SWORD,		},
	{ POINT_ATTBONUS_TWOHANDED,		},
	{ POINT_ATTBONUS_DAGGER,		},
	{ POINT_ATTBONUS_BELL,		},
	{ POINT_ATTBONUS_FAN,		},
	{ POINT_ATTBONUS_BOW,		},
#endif
#ifdef ENABLE_ELEMENT_NEW_BONUSES
	{ POINT_ATTBONUS_ELEC,},
	{ POINT_ATTBONUS_FIRE, },
	{ POINT_ATTBONUS_ICE, },
	{ POINT_ATTBONUS_WIND, },
	{ POINT_ATTBONUS_EARTH, },
	{ POINT_ATTBONUS_DARK, },
#endif
	{ POINT_RESIST_HUMAN,		},
	{ POINT_ATTBONUS_STONE,		},
	{ POINT_ATTBONUS_DESERT,	},
	{ POINT_ATTBONUS_CZ,		},
#ifdef ENABLE_BONUS_STRONG_AGAINST_BOSS
	{ POINT_ATTBONUS_RAZADOR,	},
	{ POINT_ATTBONUS_NEMERE,	},
	{ POINT_ATTBONUS_HYDRA,		},
	{ POINT_ATTBONUS_JOTUN_THRYM,	},
	{ POINT_ATTBONUS_LUSIFER,	},
	{ POINT_ATTBONUS_BERAN_SETAOU,	},
	{ POINT_ATTBONUS_AZRAEL,	},
	{ POINT_ATTBONUS_MELEY,		},
#endif
#ifdef ENABLE_MUSHROOM_DUNGEON
	{ POINT_ATTBONUS_MUSHROOM,	},
	{ POINT_RESIST_MUSHROOM,	},
#endif
#ifdef ENABLE_WATER_DUNGEON
	{ POINT_ATTBONUS_WATER,		},
	{ POINT_RESIST_WATER,		},
#endif
#ifdef ENABLE_BONUS_TO_BOSS
	{ POINT_ATTBONUS_BOSS,		},
#endif
};

const int aiItemMagicAttributePercentHigh[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//25, 25, 40, 8, 2,
	30, 40, 20, 8, 2
};

const int aiItemMagicAttributePercentLow[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//45, 25, 20, 10, 0,
	50, 40, 10, 0, 0
};

// ADD_ITEM_ATTRIBUTE
int aiItemAttributeAddPercent[ITEM_ATTRIBUTE_MAX_NUM] =
{
	100, 80, 60, 50, 30, 0, 0,
};
// END_OF_ADD_ITEM_ATTRIBUTE

#ifdef ENABLE_COSTUME_ENCHANT_SYSTEM
int aiCostumeAttributeAddPercent[COSTUME_ATTRIBUTE_MAX_NUM] =
{
	100, 60, 20,
};

int aiCostumeAttributeLevelPercent[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	0, 5, 20, 55, 20,
};
#endif

const int aiExpLossPercents[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 4, // 1 - 10
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // 11 - 20
	4, 4, 4, 4, 4, 4, 4, 3, 3, 3, // 21 - 30
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 31 - 40
	3, 3, 3, 3, 2, 2, 2, 2, 2, 2, // 41 - 50
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 51 - 60
	2, 2, 1, 1, 1, 1, 1, 1, 1, 1, // 61 - 70
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 71 - 80
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 81 - 90
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 91 - 100
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 101 - 110
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 111 - 120
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 121 - 130
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 131 - 140
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 141 - 150
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 151 - 160
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 161 - 170
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 171 - 180
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 181 - 190
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 191 - 200
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 201 - 210
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 211 - 220
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 221 - 230
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 231 - 240
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 241 - 250
};

const int aiSkillBookCountForLevelUp[10] =
{
	3, 3, 3, 3, 3, 4, 4, 5, 5, 6
};

// ADD_GRANDMASTER_SKILL
const int aiGrandMasterSkillBookCountForLevelUp[10] =
{
	3, 3, 5, 5, 7, 7, 10, 10, 10, 20,
};

const int aiGrandMasterSkillBookMinCount[10] =
{
	//	1, 1, 3, 5, 10, 15, 20, 30, 40, 50,
	//	3, 3, 5, 5, 10, 10, 15, 15, 20, 30
		1, 1, 1, 2,  2,  3,  3,  4,  5,  6
};

const int aiGrandMasterSkillBookMaxCount[10] =
{
	//	6, 15, 30, 45, 60, 80, 100, 120, 160, 200,
	//	6, 10, 15, 20, 30, 40, 50, 60, 70, 80
		5,  7,  9, 11, 13, 15, 20, 25, 30, 35
};
// END_OF_ADD_GRANDMASTER_SKILL

const int CHN_aiPartyBonusExpPercentByMemberCount[9] =
{
	0, 0, 12, 18, 26, 40, 53, 70, 100
};

// UPGRADE_PARTY_BONUS
const int KOR_aiPartyBonusExpPercentByMemberCount[9] =
{
	0,
	0,
	30, // 66% * 2 - 100
	60, // 53% * 3 - 100
	75, // 44% * 4 - 100
	90, // 38% * 5 - 100
	105, // 34% * 6 - 100
	110, // 30% * 7 - 100
	140, // 30% * 8 - 100
};

const int KOR_aiUniqueItemPartyBonusExpPercentByMemberCount[9] =
{
	0,
	0,
	15 * 2,
	14 * 3,
	13 * 4,
	12 * 5,
	11 * 6,
	10 * 7,
	10 * 8,
};
// END_OF_UPGRADE_PARTY_BONUS

const int* aiChainLightningCountBySkillLevel = nullptr;

const int aiChainLightningCountBySkillLevel_euckr[SKILL_MAX_LEVEL + 1] =
{
	0,	// 0
	2,	// 1
	2,	// 2
	2,	// 3
	2,	// 4
	2,	// 5
	2,	// 6
	2,	// 7
	2,	// 8
	3,	// 9
	3,	// 10
	3,	// 11
	3,	// 12
	3,	// 13
	3,	// 14
	3,	// 15
	3,	// 16
	3,	// 17
	3,	// 18
	4,	// 19
	4,	// 20
	4,	// 21
	4,	// 22
	4,	// 23
	5,	// 24
	5,	// 25
	5,	// 26
	5,	// 27
	5,	// 28
	5,	// 29
	5,	// 30
	5,	// 31
	5,	// 32
	5,	// 33
	5,	// 34
	5,	// 35
	5,	// 36
	5,	// 37
	5,	// 38
	5,	// 39
	5,	// 40
};

const SStoneDropInfo aStoneDrop[STONE_INFO_MAX_NUM] =
{
	//  mob		pct	{+0	+1	+2	+3	+4}
	{8001,	60,	{30,	30,	30,	9,	1}	},
	{8002,	60,	{30,	30,	30,	9,	1}	},
	{8003,	60,	{30,	30,	30,	9,	1}	},
	{8004,	60,	{30,	30,	30,	9,	1}	},
	{8005,	60,	{30,	30,	30,	9,	1}	},
	{8006,	60,	{28,	29,	31,	11,	1}	},
	{8007,	60,	{24,	29,	32,	13,	2}	},
	{8008,	60,	{22,	28,	33,	15,	2}	},
	{8009,	60,	{21,	27,	33,	17,	2}	},
	{8010,	60,	{18,	26,	34,	20,	2}	},
	{8011,	60,	{14,	26,	35,	22,	3}	},
	{8012,	60,	{10,	26,	37,	24,	3}	},
	{8013,	60,	{2,	26,	40,	29,	3}	},
	{8014,	60,	{0,	26,	41,	30,	3}	},
	{8024,	60,	{0,	26,	41,	30,	3}	},
	{8025,	60,	{0,	26,	41,	30,	3}	},
	{8026,	60,	{0,	26,	41,	30,	3}	},
	{8027,	60,	{0,	26,	41,	30,	3}	},
	{8051,	60,	{0,	26,	41,	30,	3}	},
	{8052,	60,	{0,	26,	41,	30,	3}	},
	{8053,	60,	{0,	26,	41,	30,	3}	},
	{8054,	60,	{0,	26,	41,	30,	3}	},
	{8055,	60,	{0,	26,	41,	30,	3}	},
	{8056,	60,	{0,	26,	41,	30,	3}	},
	{8059,	60,	{0,	26,	41,	30,	3}	},
	{8061,	60,	{0,	26,	41,	30,	3}	},
};

const char* c_apszEmpireNames[EMPIRE_MAX_NUM] =
{
	"absolute monarchy",
	"New Hydrangea",
	"Cloth country",
	"Wrath Bureau"
};

const char* c_apszPrivNames[MAX_PRIV_NUM] =
{
	"None",
	"Probability item is out",
	"Probability money out",
	"This game jackpot money odds",
	"Experience Scale",
};

const int aiPolymorphPowerByLevel[SKILL_MAX_LEVEL + 1] =
{
	10,   // 1
	11,   // 2
	11,   // 3
	12,   // 4
	13,   // 5
	13,   // 6
	14,   // 7
	15,   // 8
	16,   // 9
	17,   // 10
	18,   // 11
	19,   // 12
	20,   // 13
	22,   // 14
	23,   // 15
	24,   // 16
	26,   // 17
	27,   // 18
	29,   // 19
	31,   // 20
	33,   // 21
	35,   // 22
	37,   // 23
	39,   // 24
	41,   // 25
	44,   // 26
	46,   // 27
	49,   // 28
	52,   // 29
	55,   // 30
	59,   // 31
	62,   // 32
	66,   // 33
	70,   // 34
	74,   // 35
	79,   // 36
	84,   // 37
	89,   // 38
	94,   // 39
	100,  // 40
};

TGuildWarInfo KOR_aGuildWarInfo[GUILD_WAR_TYPE_MAX_NUM] =
/*
   {
   long lMapIndex;
   int iWarPrice;
   int iWinnerPotionRewardPctToWinner;
   int iLoserPotionRewardPctToWinner;
   int iInitialScore;
   int iEndScore;
   };
 */
{
	{ 0,        0,      0,      0,      0,      0       },
	{ 110,      0,      100,    50,     0,      100     },
	{ 111,      0,      100,    50,     0,      10      },
};

//
// 악세서리 소켓용 수치들
//

// 다이아몬드로 소켓을 추가할 때 확률
const int aiAccessorySocketAddPct[ITEM_ACCESSORY_SOCKET_MAX_NUM] =
{
	50, 50, 50
};

// 악세서리 수치 값의 몇%만큼의 성능을 추가하는지
const int aiAccessorySocketEffectivePct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	0, 10, 20, 40
};

// 소켓 지속시간 24, 12, 6
const int aiAccessorySocketDegradeTime[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	0, 3600 * 24, 3600 * 12, 3600 * 6
};

// 소켓 장착 성공률
const int aiAccessorySocketPutPct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	90, 80, 70, 0
};
// END_OF_ACCESSORY_REFINE

#include "../../common/length.h"
// from import_item_proto.c
typedef struct SValueName
{
	const char* c_pszName;
	long		lValue;
} TValueName;

TValueName c_aApplyTypeNames[] =
{
	{"STR", APPLY_STR},
	{"DEX", APPLY_DEX},
	{"CON", APPLY_CON},
	{"INT", APPLY_INT},
	{"MAX_HP", APPLY_MAX_HP},
	{"MAX_SP", APPLY_MAX_SP},
	{"MAX_STAMINA", APPLY_MAX_STAMINA},
	{"POISON_REDUCE", APPLY_POISON_REDUCE},
	{"EXP_DOUBLE_BONUS", APPLY_EXP_DOUBLE_BONUS},
	{"GOLD_DOUBLE_BONUS", APPLY_GOLD_DOUBLE_BONUS},
	{"ITEM_DROP_BONUS", APPLY_ITEM_DROP_BONUS},
	{"HP_REGEN", APPLY_HP_REGEN},
	{"SP_REGEN", APPLY_SP_REGEN},
	{"ATTACK_SPEED", APPLY_ATT_SPEED},
	{"MOVE_SPEED", APPLY_MOV_SPEED},
	{"CAST_SPEED", APPLY_CAST_SPEED},
	{"ATT_BONUS", APPLY_ATT_GRADE_BONUS},
	{"DEF_BONUS", APPLY_DEF_GRADE_BONUS},
	{"MAGIC_ATT_GRADE", APPLY_MAGIC_ATT_GRADE},
	{"MAGIC_DEF_GRADE", APPLY_MAGIC_DEF_GRADE},
	{"SKILL", APPLY_SKILL},
	{"ATTBONUS_ANIMAL", APPLY_ATTBONUS_ANIMAL},
	{"ATTBONUS_UNDEAD", APPLY_ATTBONUS_UNDEAD},
	{"ATTBONUS_DEVIL", APPLY_ATTBONUS_DEVIL},
	{"ATTBONUS_HUMAN", APPLY_ATTBONUS_HUMAN},
	{"ADD_BOW_DISTANCE", APPLY_BOW_DISTANCE},
	{"DODGE", APPLY_DODGE},
	{"BLOCK", APPLY_BLOCK},
	{"RESIST_SWORD", APPLY_RESIST_SWORD},
	{"RESIST_TWOHAND", APPLY_RESIST_TWOHAND},
	{"RESIST_DAGGER", APPLY_RESIST_DAGGER},
	{"RESIST_BELL", APPLY_RESIST_BELL},
	{"RESIST_FAN", APPLY_RESIST_FAN},
	{"RESIST_BOW", APPLY_RESIST_BOW},
	{"RESIST_FIRE", APPLY_RESIST_FIRE},
	{"RESIST_ELEC", APPLY_RESIST_ELEC},
	{"RESIST_MAGIC", APPLY_RESIST_MAGIC},
	{"RESIST_WIND", APPLY_RESIST_WIND},
	{"REFLECT_MELEE", APPLY_REFLECT_MELEE},
	{"REFLECT_CURSE", APPLY_REFLECT_CURSE},
	{"RESIST_ICE", APPLY_RESIST_ICE},
	{"RESIST_EARTH", APPLY_RESIST_EARTH},
	{"RESIST_DARK", APPLY_RESIST_DARK},
	{"RESIST_CRITICAL", APPLY_ANTI_CRITICAL_PCT},
	{"RESIST_PENETRATE", APPLY_ANTI_PENETRATE_PCT},
	{"POISON", APPLY_POISON_PCT},
	{"SLOW", APPLY_SLOW_PCT},
	{"STUN", APPLY_STUN_PCT},
	{"STEAL_HP", APPLY_STEAL_HP},
	{"STEAL_SP", APPLY_STEAL_SP},
	{"MANA_BURN_PCT", APPLY_MANA_BURN_PCT},
	{"CRITICAL", APPLY_CRITICAL_PCT},
	{"PENETRATE", APPLY_PENETRATE_PCT},
	{"KILL_SP_RECOVER", APPLY_KILL_SP_RECOVER},
	{"KILL_HP_RECOVER", APPLY_KILL_HP_RECOVER},
	{"PENETRATE_PCT", APPLY_PENETRATE_PCT},
	{"CRITICAL_PCT", APPLY_CRITICAL_PCT},
	{"POISON_PCT", APPLY_POISON_PCT},
	{"STUN_PCT", APPLY_STUN_PCT},
	{"ATT_BONUS_TO_WARRIOR", APPLY_ATTBONUS_WARRIOR},
	{"ATT_BONUS_TO_ASSASSIN", APPLY_ATTBONUS_ASSASSIN},
	{"ATT_BONUS_TO_SURA", APPLY_ATTBONUS_SURA},
	{"ATT_BONUS_TO_SHAMAN", APPLY_ATTBONUS_SHAMAN},
	{"ATT_BONUS_TO_MONSTER", APPLY_ATTBONUS_MONSTER},
	{"ATT_BONUS_TO_MOB", APPLY_ATTBONUS_MONSTER},
	{"MALL_ATTBONUS", APPLY_MALL_ATTBONUS},
	{"MALL_EXPBONUS", APPLY_MALL_EXPBONUS},
	{"MALL_DEFBONUS", APPLY_MALL_DEFBONUS},
	{"MALL_ITEMBONUS", APPLY_MALL_ITEMBONUS},
	{"MALL_GOLDBONUS", APPLY_MALL_GOLDBONUS},
	{"MAX_HP_PCT", APPLY_MAX_HP_PCT},
	{"MAX_SP_PCT", APPLY_MAX_SP_PCT},
	{"SKILL_DAMAGE_BONUS", APPLY_SKILL_DAMAGE_BONUS},
	{"NORMAL_HIT_DAMAGE_BONUS", APPLY_NORMAL_HIT_DAMAGE_BONUS},
	{"SKILL_DEFEND_BONUS", APPLY_SKILL_DEFEND_BONUS},
	{"NORMAL_HIT_DEFEND_BONUS", APPLY_NORMAL_HIT_DEFEND_BONUS},
	{"RESIST_WARRIOR", APPLY_RESIST_WARRIOR},
	{"RESIST_ASSASSIN", APPLY_RESIST_ASSASSIN},
	{"RESIST_SURA", APPLY_RESIST_SURA},
	{"RESIST_SHAMAN", APPLY_RESIST_SHAMAN},
	{"INFINITE_AFFECT_DURATION", 0x1FFFFFFF},
	{"ENERGY", APPLY_ENERGY},
	{"COSTUME_ATTR_BONUS", APPLY_COSTUME_ATTR_BONUS},
	{"MAGIC_ATTBONUS_PER", APPLY_MAGIC_ATTBONUS_PER},
	{"MELEE_MAGIC_ATTBONUS_PER", APPLY_MELEE_MAGIC_ATTBONUS_PER},
	{"BLEEDING_PCT", APPLY_BLEEDING_PCT},
	{"BLEEDING_REDUCE", APPLY_BLEEDING_REDUCE},
	{"ATT_BONUS_TO_WOLFMAN", APPLY_ATTBONUS_WOLFMAN},
	{"RESIST_WOLFMAN", APPLY_RESIST_WOLFMAN},
	{"RESIST_CLAW", APPLY_RESIST_CLAW},
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	{"ANTI_RESIST_MAGIC", APPLY_ANTI_RESIST_MAGIC},
#endif
#ifdef ENABLE_BONUS_STRONG_AGAINST_WEAPON
	{ "ATT_BONUS_TO_SWORD",	APPLY_ATTBONUS_SWORD	},
	{ "ATT_BONUS_TO_TWOHAND",	APPLY_ATTBONUS_TWOHANDED	},
	{ "ATT_BONUS_TO_DAGGER",	APPLY_ATTBONUS_DAGGER    },
	{ "ATT_BONUS_TO_BELL",	APPLY_ATTBONUS_BELL	},
	{ "ATT_BONUS_TO_FAN",	APPLY_ATTBONUS_FAN	},
	{ "ATT_BONUS_TO_BOW",	APPLY_ATTBONUS_BOW	},
#endif
	{ "ATTBONUS_STONE",	APPLY_ATTBONUS_STONE	},
	{ "ATTBONUS_BOSS", APPLY_ATTBONUS_BOSS		},
    { "ATTBONUS_ELEC",	APPLY_ATTBONUS_ELEC	},
    { "ATTBONUS_FIRE",	APPLY_ATTBONUS_FIRE	},
    { "ATTBONUS_ICE",	APPLY_ATTBONUS_ICE    },
    { "ATTBONUS_WIND",	APPLY_ATTBONUS_WIND	},
    { "ATTBONUS_EARTH",	APPLY_ATTBONUS_EARTH	},
    { "ATTBONUS_DARK",	APPLY_ATTBONUS_DARK	},
	{nullptr, 0}
};
// from import_item_proto.c

long FN_get_apply_type(const char* apply_type_string)
{
	TValueName* value_name;
	for (value_name = c_aApplyTypeNames; value_name->c_pszName; ++value_name)
	{
		if (0 == strcasecmp(value_name->c_pszName, apply_type_string))
			return value_name->lValue;
	}
	return 0;
}

const char* FN_MAP_NAME_STRING(int mapindex)
{
	if (mapindex >= 10000)
		return LC_TEXT("metin2_map_dungeon_string");

	switch (mapindex)
	{
	case 1: return LC_TEXT("metin2_map_a1");
	case 3: return LC_TEXT("metin2_map_a3");
	case 4: return LC_TEXT("metin2_map_guild_01");
	case 5: return LC_TEXT("metin2_map_monkey_dungeon_11");
	case 6: return LC_TEXT("metin2_guild_village_01");
	case 21: return LC_TEXT("metin2_map_b1");
	case 23: return LC_TEXT("metin2_map_b3");
	case 24: return LC_TEXT("metin2_map_guild_02");
	case 25: return LC_TEXT("metin2_map_monkey_dungeon_12");
	case 26: return LC_TEXT("metin2_guild_village_02");
	case 41: return LC_TEXT("metin2_map_c1");
	case 43: return LC_TEXT("metin2_map_c3");
	case 44: return LC_TEXT("metin2_map_guild_03");
	case 45: return LC_TEXT("metin2_map_monkey_dungeon_13");
	case 46: return LC_TEXT("metin2_guild_village_03");
	case 51: return LC_TEXT("metin2_map_wolf");
	case 61: return LC_TEXT("map_n_snowm_01");
	case 62: return LC_TEXT("metin2_map_n_flame_01");
	case 63: return LC_TEXT("metin2_map_n_desert_01");
	case 64: return LC_TEXT("map_n_threeway");
	case 65: return LC_TEXT("metin2_map_milgyo");
	case 66: return LC_TEXT("metin2_map_deviltower1");
	case 67: return LC_TEXT("metin2_map_trent");
	case 68: return LC_TEXT("metin2_map_trent02");
	case 69: return LC_TEXT("metin2_map_WL_01");
	case 70: return LC_TEXT("metin2_map_nusluck01");
	case 71: return LC_TEXT("metin2_map_spiderdungeon_02");
	case 72: return LC_TEXT("metin2_map_skipia_dungeon_01");
	case 73: return LC_TEXT("metin2_map_skipia_dungeon_02");
	case 81: return LC_TEXT("metin2_map_wedding_01");
	case 100: return LC_TEXT("metin2_map_fielddungeon");
	case 101: return LC_TEXT("metin2_map_resources_zon");
	case 103: return LC_TEXT("metin2_map_t1");
	case 104: return LC_TEXT("metin2_map_spiderdungeon");
	case 105: return LC_TEXT("metin2_map_t2");
	case 107: return LC_TEXT("metin2_map_monkey_dungeon");
	case 108: return LC_TEXT("metin2_map_monkey_dungeon2");
	case 109: return LC_TEXT("metin2_map_monkey_dungeon3");
	case 110: return LC_TEXT("metin2_map_t3");
	case 111: return LC_TEXT("metin2_map_t4");
	case 112: return LC_TEXT("metin2_map_duel");
	case 113: return LC_TEXT("metin2_map_oxevent");
	case 114: return LC_TEXT("metin2_map_sungzi");
	case 118: return LC_TEXT("metin2_map_sungzi_flame_hill_01");
	case 119: return LC_TEXT("metin2_map_sungzi_flame_hill_02");
	case 120: return LC_TEXT("metin2_map_sungzi_flame_hill_03");
	case 121: return LC_TEXT("metin2_map_sungzi_snow");
	case 122: return LC_TEXT("metin2_map_sungzi_snow_pass01");
	case 123: return LC_TEXT("metin2_map_sungzi_snow_pass02");
	case 124: return LC_TEXT("metin2_map_sungzi_snow_pass03");
	case 125: return LC_TEXT("metin2_map_sungzi_desert_01");
	case 126: return LC_TEXT("metin2_map_sungzi_desert_hill_01");
	case 127: return LC_TEXT("metin2_map_sungzi_desert_hill_02");
	case 128: return LC_TEXT("metin2_map_sungzi_desert_hill_03");
	case 181: return LC_TEXT("metin2_map_empirewar01");
	case 182: return LC_TEXT("metin2_map_empirewar02");
	case 183: return LC_TEXT("metin2_map_empirewar03");
	case 200: return LC_TEXT("gm_guild_build");
	case 207: return LC_TEXT("metin2_map_skipia_dungeon_boss");
	case 212: return LC_TEXT("metin2_map_n_flame_dragon");
	case 216: return LC_TEXT("metin2_map_devilcatacomb");
	case 217: return LC_TEXT("metin2_map_spiderdungeon_03");
	case 246: return LC_TEXT("metin2_map_battlefied");
	case 301: return LC_TEXT("Metin2_map_CapeDragonHead");
	case 302: return LC_TEXT("metin2_map_dawnmistwood");
	case 303: return LC_TEXT("metin2_map_BayBlackSand");
	case 304: return LC_TEXT("metin2_map_Mt_Thunder");
	case 305: return LC_TEXT("metin2_map_sungzi_flame_pass01");
	case 306: return LC_TEXT("metin2_map_sungzi_flame_pass02");
	case 307: return LC_TEXT("metin2_map_sungzi_flame_pass03");
	case 351: return LC_TEXT("metin2_map_n_flame_dungeon_01");
	case 352: return LC_TEXT("metin2_map_n_snow_dungeon_01");
	case 353: return LC_TEXT("metin2_map_dawnmist_dungeon_01");
	case 354: return LC_TEXT("metin2_map_mt_th_dungeon_01");
	case 355: return LC_TEXT("metin2_map_dawnmist_dungeon_02");
	case 356: return LC_TEXT("metin2_12zi_stage");
	default: return LC_TEXT("metin2_map_unknown");
	}
}

const char* FN_CHANNEL_NAME_STRING(int channel)
{
	switch (channel)
	{
	case 1: return LC_TEXT("metin2_channel_1");
	case 2: return LC_TEXT("metin2_channel_2");
	case 3: return LC_TEXT("metin2_channel_3");
	case 4: return LC_TEXT("metin2_channel_4");
	case 5: return LC_TEXT("metin2_channel_5");
	case 6: return LC_TEXT("metin2_channel_6");
	default: return LC_TEXT("metin2_channel_unknown");
	}
}

const DWORD BiyologSistemi[11][16] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{ 30006, 10, 0, 100, 30220, 50109, 8, 10, 0,0,0,0,0,0,0,0},
	{ 30047, 15, 0, 100, 30221, 50110, 7, 5, 0,0,0,0,0,0,0,0},
	{ 30015, 15, 0, 100, 30222, 50111, 54, 60, 0,0,0,0,0,0,0,0},
	{ 30050, 20, 0, 100, 30223, 50112, 53, 50, 0,0,0,0,0,0,0,0},
	{ 30165, 25, 0, 100, 30224, 50113, 8, 11, 83,10,0,0,0,0,0,0},
	{ 30166, 30, 0, 100, 30225, 50114, 7, 6, POINT_ATT_BONUS,10,0,0,0,0,0,0},
	{ 30167, 40, 0, 100, 30226, 50115, 78, 10, 79,10,80,10,81,10,POINT_RESIST_WOLFMAN,10},
	{ 30168, 50, 0, 100, 30227, 50114, POINT_ATTBONUS_WOLFMAN, 10,POINT_ATTBONUS_WARRIOR,10,POINT_ATTBONUS_ASSASSIN,10,POINT_ATTBONUS_SHAMAN,10,POINT_ATTBONUS_SURA,10},
	{ 30251, 10, 0, 100, 0, 0, 1, 1000, 54,120,53,50,0,0,0,0},
	{ 30252, 20, 0, 100, 30228, 0, 1, 1100, 54,140,53,60,0,0,0,0}
};