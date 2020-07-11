#ifndef __INC_EVENT_MANAGER_H
#define __INC_EVENT_MANAGER_H

#include "../../common/service.h"

namespace eventmanager
{
	enum EEventManagerEnums
	{
		MOB_CARD_EVENT_NPC_VNUM = 20417,
		EASTER_EVENT_NPC_VNUM = 30129,
		EASTER2_EVENT_NPC_VNUM = 30131,
		EASTER_EVENT_FLAG_VNUM = 20143,
		HALLOWEEN_HAIR_EVENT_NPC_VNUM = 33008,
		RAMADAN_EVENT_HISTORIAN_NPC_VNUM = 33002,
		RAMADAN_EVENT_BEGGAR1_NPC_VNUM = 33003,
		RAMADAN_EVENT_BEGGAR2_NPC_VNUM = 33004,
		RAMADAN_EVENT_BEGGAR3_NPC_VNUM = 33005,
		RAMADAN_EVENT_BEGGAR4_NPC_VNUM = 33006,
		RAMADAN_EVENT_BEGGAR5_NPC_VNUM = 33007,
		SHADOW_WARRIOR_MOB_VNUM = 6420,
		YEAR_ROUND_MERCHANT_NPC = 20431,
		YEAR_ROUND_CHERIE_NPC = 20430,
		BOSS_HUNTERS_BAASHIDO_MOB_VNUM = 6421,
	};

	enum EBossHunterEnums
	{
		DAY0_REQUIRED_POINT = 100,
		DAY1_REQUIRED_POINT = 200,
		DAY2_REQUIRED_POINT = 300,
		DAY3_REQUIRED_POINT = 400,
		DAY4_REQUIRED_POINT = 500,
		DAY5_REQUIRED_POINT = 600,
		DAY6_REQUIRED_POINT = 700,
		DAY7_REQUIRED_POINT = 800,
	};

	void SpawnCardNpc(bool spawn);
	void SpawnEasterNpc(bool spawn);
	void SpawnEasterNpc2(bool spawn);
	void SpawnFlagsRedEmpire(bool spawn);
	void SpawnFlagsYellowEmpire(bool spawn);
	void SpawnFlagsBlueEmpire(bool spawn);
	void SpawnHalloweenHairNpc(bool spawn);
	void SpawnRamadanHistorianNpc(bool spawn);
	void SpawnRamadanBeggar1Npc(bool spawn);
	void SpawnRamadanBeggar2Npc(bool spawn);
	void SpawnRamadanBeggar3Npc(bool spawn);
	void SpawnRamadanBeggar4Npc(bool spawn);
	void SpawnRamadanBeggar5Npc(bool spawn);
	void SpawnShadowWarriorMob(bool spawn);
	void SpawnYearRoundMerchant(bool spawn);
	void SpawnYearRoundCherie(bool spawn);
	void SpawnBossHuntersBaashidoMob(bool spawn);
}
#endif
