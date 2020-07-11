#include "stdafx.h"
#include "config.h"
#include "event_manager.h"
#include "desc.h"
#include "desc_manager.h"
#include "sectree_manager.h"
#include "char.h"
#include "char_manager.h"
#include "questmanager.h"
#include "../../common/VnumHelper.h"

namespace eventmanager
{
	void SpawnCardNpc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(MOB_CARD_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCOkeyCardNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 599, 637 },
				{ 21, 595, 618 },
				{ 41, 354, 739 },
				{ 0,  0,   0   },
			};

			SNPCOkeyCardNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(MOB_CARD_EVENT_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(MOB_CARD_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnEasterNpc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCEasterNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 591, 472 },
				{ 21, 517, 742 },
				{ 41, 306, 828 },
				{ 0,  0,   0   },
			};

			SNPCEasterNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(EASTER_EVENT_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnEasterNpc2(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER2_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCEasterNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 599, 637 },
				{ 21, 595, 618 },
				{ 41, 354, 739 },
				{ 0,  0,   0   },
			};

			SNPCEasterNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(EASTER2_EVENT_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER2_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnFlagsRedEmpire(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCEasterNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{ 1, 653, 694 },
				{ 1, 653, 681 },
				{ 1, 651, 689 },
				{ 1, 663, 689 },
				{ 1, 650, 682 },
				{ 1, 660, 682 },
				{ 1, 555, 587 },
				{ 1, 555, 576 },
				{ 1, 561, 587 },
				{ 1, 561, 576 },
				{ 1, 566, 587 },
				{ 1, 566, 576 },
				{ 0,  0,   0   },
			};

			SNPCEasterNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(EASTER_EVENT_FLAG_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnFlagsYellowEmpire(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCEasterNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{ 21, 548, 645 },
				{ 21, 548, 635 },
				{ 21, 555, 645 },
				{ 21, 555, 635 },
				{ 21, 561, 645 },
				{ 21, 561, 635 },
				{ 0,  0,   0   },
			};

			SNPCEasterNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(EASTER_EVENT_FLAG_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnFlagsBlueEmpire(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCEasterNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{ 41, 436, 643 },
				{ 41, 436, 635 },
				{ 41, 431, 643 },
				{ 41, 431, 635 },
				{ 41, 426, 643 },
				{ 41, 426, 635 },
				{ 41, 419, 725 },
				{ 41, 409, 725 },
				{ 41, 416, 718 },
				{ 41, 406, 718 },
				{ 41, 414, 712 },
				{ 41, 404, 712 },
				{ 41, 363, 554 },
				{ 41, 351, 554 },
				{ 41, 365, 560 },
				{ 41, 353, 560 },
				{ 41, 367, 570 },
				{ 41, 355, 570 },
				{ 0,  0,   0   },
			};

			SNPCEasterNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(EASTER_EVENT_FLAG_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(EASTER_EVENT_FLAG_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnHalloweenHairNpc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(HALLOWEEN_HAIR_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCHalloweenHairNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 608, 614 },
				{ 21, 596, 607 },
				{ 41, 359, 744 },
				{ 0,  0,   0   },
			};

			SNPCHalloweenHairNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(HALLOWEEN_HAIR_EVENT_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(HALLOWEEN_HAIR_EVENT_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanHistorianNpc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_HISTORIAN_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 608, 617 },
				{ 21, 596, 610 },
				{ 41, 357, 743 },
				{ 0,  0,   0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_HISTORIAN_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_HISTORIAN_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanBeggar1Npc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR1_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 619, 701 },
				{ 21, 564, 613 },
				{ 41, 344, 723 },
				{ 0,  0,   0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_BEGGAR1_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR1_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanBeggar2Npc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR2_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 697, 580 },
				{ 21, 699, 658 },
				{ 41, 443, 733 },
				{ 0,  0,   0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_BEGGAR2_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR2_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanBeggar3Npc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR3_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 646, 515 },
				{ 21, 613, 715 },
				{ 41, 428, 651 },
				{ 0,  0,   0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_BEGGAR3_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR3_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanBeggar4Npc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR4_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{  1, 554, 550 },
				{ 21, 635, 539 },
				{ 41, 303, 587 },
				{ 0,  0,   0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_BEGGAR4_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR4_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnRamadanBeggar5Npc(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR5_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
				int z;
			}

			positions[] =
			{
				{  1, 702, 629, 0 },
				{ 21, 529, 547, 0 },
				{ 41, 456, 673, 0  },
				{ 0,  0,   0,  0   },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(RAMADAN_EVENT_BEGGAR5_NPC_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(RAMADAN_EVENT_BEGGAR5_NPC_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnShadowWarriorMob(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(SHADOW_WARRIOR_MOB_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SMobShadowWarriorPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{ 64, 719, 1120 },
				{ 61, 693, 230 },
				{ 62, 206, 710 },
				{ 63, 851, 624 },
				{ 0,  0,   0   },
			};

			SMobShadowWarriorPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(SHADOW_WARRIOR_MOB_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(SHADOW_WARRIOR_MOB_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnYearRoundMerchant(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(YEAR_ROUND_MERCHANT_NPC, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
				int z;
			}

			positions[] =
			{
				{  1, 446, 669,	8 },
				{ 21, 686, 592, 4 },
				{ 41, 446, 669,	5 },
				{ 0,  0,   0,	0 },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(YEAR_ROUND_MERCHANT_NPC, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, p->z, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(YEAR_ROUND_MERCHANT_NPC, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnYearRoundCherie(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(YEAR_ROUND_CHERIE_NPC, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SNPCRamadanNpcPosition
			{
				long lMapIndex;
				int x;
				int y;
				int z;
			}

			positions[] =
			{
				{  1, 663, 623, 5 },
				{ 21, 670, 650, 0 },
				{ 41, 350, 625, 5 },
				{ 0,  0,   0,	0 },
			};

			SNPCRamadanNpcPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(YEAR_ROUND_CHERIE_NPC, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, p->z, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(YEAR_ROUND_CHERIE_NPC, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}

	void SpawnBossHuntersBaashidoMob(bool spawn)
	{
		if (spawn == true)
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(BOSS_HUNTERS_BAASHIDO_MOB_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}

			struct SMobShadowWarriorPosition
			{
				long lMapIndex;
				int x;
				int y;
			}

			positions[] =
			{
				{ 64, 719, 1120 },
				{ 61, 693, 230 },
				{ 62, 206, 710 },
				{ 63, 851, 624 },
				{ 0,  0,   0   },
			};

			SMobShadowWarriorPosition* p = positions;
			while (p->lMapIndex)
			{
				if (map_allow_find(p->lMapIndex))
				{
					PIXEL_POSITION posBase;
					if (!SECTREE_MANAGER::instance().GetMapBasePositionByMapIndex(p->lMapIndex, posBase))
					{
						sys_err("cannot get map base position %d", p->lMapIndex);
						p++;
						continue;
					}

					CHARACTER_MANAGER::instance().SpawnMob(BOSS_HUNTERS_BAASHIDO_MOB_VNUM, p->lMapIndex, posBase.x + p->x * 100, posBase.y + p->y * 100, 0, false, -1);
				}

				p++;
			}
		}
		else
		{
			CharacterVectorInteractor i;

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(BOSS_HUNTERS_BAASHIDO_MOB_VNUM, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end())
					M2_DESTROY_CHARACTER(*it++);
			}
		}
	}
}