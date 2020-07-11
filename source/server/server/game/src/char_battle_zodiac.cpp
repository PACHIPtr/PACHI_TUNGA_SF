#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "battle.h"
#include "pvp.h"
#include "skill.h"
#include "start_position.h"
#include "profiler.h"
#include "cmd.h"
#include "dungeon.h"
#include "log.h"
#include "unique_item.h"
#include "priv_manager.h"
#include "db.h"
#include "vector.h"
#include "marriage.h"
#include "arena.h"
#include "regen.h"
#include "exchange.h"
#include "shop_manager.h"
#include "ani.h"
#include "packet.h"
#include "party.h"
#include "affect.h"
#include "guild.h"
#include "guild_manager.h"
#include "questmanager.h"
#include "questlua.h"
#include "BlueDragon.h"
#include "DragonLair.h"
#include "war_map.h"

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
DWORD SKILL_VNUM = 270;

BYTE SKILL_LEVEL(BYTE Level)
{
	if (Level >= 40 && Level < 50)
		return 15;
	if (Level >= 50 && Level < 60)
		return 15;
	else if (Level >= 60 && Level < 70)
		return 20;
	else if (Level >= 70 && Level < 80)
		return 25;
	else if (Level >= 80 && Level < 90)
		return 30;
	else if (Level >= 90 && Level < 100)
		return 30;
	else if (Level >= 100 && Level < 136)
		return 30;
	else
		return 20;
}

bool PolyDog(DWORD Vnum)
{
	if (Vnum == 2851 || Vnum == 2852)
		return true;

	return false;
}

bool CHARACTER::IsZodiacBoss()
{
	if (!IsMonster())
		return false;

	if (GetRaceNum() >= 2750 && GetRaceNum() <= 2862)
		return true;

	return false;
}

bool CHARACTER::IsZodiacSubays()
{
	if (!IsMonster())
		return false;

	if (GetRaceNum() == 2750 || 
		GetRaceNum() == 2760 || 
		GetRaceNum() == 2770 || 
		GetRaceNum() == 2780 || 
		GetRaceNum() == 2790 || 
		GetRaceNum() == 2800 || 
		GetRaceNum() == 2810 || 
		GetRaceNum() == 2820 || 
		GetRaceNum() == 2830 || 
		GetRaceNum() == 2840 || 
		GetRaceNum() == 2850 || 
		GetRaceNum() == 2860)
		return true;

	return false;
}

bool CHARACTER::IsZodiacStone()
{
	if (!IsStone())
		return false;

	if (GetRaceNum() >= 20452 && GetRaceNum() <= 20463)
		return true;

	return false;
}

WORD CHARACTER::GetStoneNum() const
{
	if (!IsMonster())
	{
		sys_err("Mob degil!");
		return 0;
	}

	WORD Vnum = GetRaceNum();
	if (Vnum >= 2750 && Vnum <= 2752)
		return 20452;
	else if (Vnum >= 2760 && Vnum <= 2762)
		return 20453;
	else if (Vnum >= 2770 && Vnum <= 2772)
		return 20454;
	else if (Vnum >= 2780 && Vnum <= 2782)
		return 20455;
	else if (Vnum >= 2790 && Vnum <= 2792)
		return 20456;
	else if (Vnum >= 2800 && Vnum <= 2802)
		return 20457;
	else if (Vnum >= 2810 && Vnum <= 2812)
		return 20458;
	else if (Vnum >= 2820 && Vnum <= 2822)
		return 20459;
	else if (Vnum >= 2830 && Vnum <= 2832)
		return 20460;
	else if (Vnum >= 2840 && Vnum <= 2842)
		return 20461;
	else if (Vnum >= 2850 && Vnum <= 2852)
		return 20462;
	else if (Vnum >= 2860 && Vnum <= 2862)
		return 20463;

	return 0;
}
//---------------------------------***-Skill_1-***--------------------------------------------------//

struct FAttackSkill1
{
	LPCHARACTER tch;
	FAttackSkill1(LPCHARACTER c) : tch(c) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(ch->GetX() - tch->GetX(), ch->GetY() - tch->GetY()) <= 1000)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_1_info)
{
	LPCHARACTER Mob;
	zodiac_skill_1_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_1_event)
{
	zodiac_skill_1_info* info = dynamic_cast<zodiac_skill_1_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_1_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_1_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_1_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill1 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		FAttackSkill1 f(Mob);
		pkMap->for_each(f);
	}

	return 0;
}

//---------------------------------***-Skill_2-***--------------------------------------------------//

struct FAttackSkill2
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill2(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 400)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_2_info)
{
	LPCHARACTER Mob;
	zodiac_skill_2_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_2_event)
{
	zodiac_skill_2_info* info = dynamic_cast<zodiac_skill_2_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_2_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_2_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_2_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill2 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill2 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill2 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill2 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill2 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}
	}

	return 0;
}

//---------------------------------***-Skill_3-***--------------------------------------------------//

struct FAttackSkill3
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill3(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 490)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_3_info)
{
	LPCHARACTER Mob;
	int EfektType;
	int EfektX1, EfektX2, EfektX3;
	int EfektY1, EfektY2, EfektY3;
	int DamageX1, DamageX2, DamageX3;
	int DamageY1, DamageY2, DamageY3;

	zodiac_skill_3_info()
	: Mob(nullptr), EfektType(0), 
		EfektX1(0), EfektX2(0), EfektX3(0), 
		EfektY1(0), EfektY2(0), EfektY3(0),
		DamageX1(0), DamageX2(0), DamageX3(0), 
		DamageY1(0), DamageY2(0), DamageY3(0) 
	{}
};

EVENTFUNC(zodiac_skill_3_event)
{
	zodiac_skill_3_info* info = dynamic_cast<zodiac_skill_3_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_3_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_3_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_3_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill3 = nullptr;

	{
		LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());

		{
			Mob->EffectZodiacPacket(info->EfektX1, info->EfektY1, info->EfektType);

			if (pkMap)
			{
				FAttackSkill3 f(Mob, info->DamageX1, info->DamageY1);
				pkMap->for_each(f);
			}
		}

		{
			Mob->EffectZodiacPacket(info->EfektX2, info->EfektY2, info->EfektType);

			if (pkMap)
			{
				FAttackSkill3 f(Mob, info->DamageX2, info->DamageY2);
				pkMap->for_each(f);
			}
		}

		{
			Mob->EffectZodiacPacket(info->EfektX3, info->EfektY3, info->EfektType);

			if (pkMap)
			{
				FAttackSkill3 f(Mob, info->DamageX3, info->DamageY3);
				pkMap->for_each(f);
			}
		}
	}

	return 0;
}


//---------------------------------***-Skill_4-***--------------------------------------------------//
struct FAttackSkill4
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill4(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 490)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (tch->GetRaceNum() >= 2750 && tch->GetRaceNum() <= 2752)
					{
						DWORD dwCurTime = get_dword_time();
						tch->SendMovePacket(FUNC_MOB_SKILL, 4, ch->GetX(), ch->GetY(), 0, dwCurTime);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_4_info)
{
	LPCHARACTER Mob;
	int EfektX, EfektY;
	zodiac_skill_4_info() : Mob(nullptr), EfektX(0), EfektY(0) {}
};

EVENTFUNC(zodiac_skill_4_event)
{
	zodiac_skill_4_info* info = dynamic_cast<zodiac_skill_4_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_4_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_4_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_4_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill4 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		FAttackSkill4 f(Mob, info->EfektX, info->EfektY);
		pkMap->for_each(f);
	}

	return 0;
}

//---------------------------------***-Skill_5-***--------------------------------------------------//

struct FAttackSkill5
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill5(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 400)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_5_info)
{
	LPCHARACTER Mob;
	zodiac_skill_5_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_5_event)
{
	zodiac_skill_5_info* info = dynamic_cast<zodiac_skill_5_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_5_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_5_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_5_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill5 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill5 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill5 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill5 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill5 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}
	}

	return 0;
}

//---------------------------------***-Skill_6-***--------------------------------------------------//

struct FAttackSkill6
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill6(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 400)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_6_info)
{
	LPCHARACTER Mob;
	zodiac_skill_6_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_6_event)
{
	zodiac_skill_6_info* info = dynamic_cast<zodiac_skill_6_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_6_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_6_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_6_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill6 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill6 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill6 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill6 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}
	}

	return 0;
}

//---------------------------------***-Skill_7-***--------------------------------------------------//

struct FAttackSkill7
{
	LPCHARACTER tch;
	FAttackSkill7(LPCHARACTER c) : tch(c) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(ch->GetX() - tch->GetX(), ch->GetY() - tch->GetY()) <= 1000)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_7_info)
{
	LPCHARACTER Mob;
	zodiac_skill_7_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_7_event)
{
	zodiac_skill_7_info* info = dynamic_cast<zodiac_skill_7_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_7_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_7_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_7_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill7 = nullptr;

	LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(Mob->GetMapIndex());
	if (pZodiac)
	{
		pZodiac->PolyCharacter(Mob);
	}

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		FAttackSkill7 f(Mob);
		pkMap->for_each(f);
	}

	return 0;
}

//---------------------------------***-Skill_8-***--------------------------------------------------//

struct FAttackSkill8
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill8(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 400)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_8_info)
{
	LPCHARACTER Mob;
	zodiac_skill_8_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_8_event)
{
	zodiac_skill_8_info* info = dynamic_cast<zodiac_skill_8_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_8_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_8_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_8_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill8 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill8 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill8 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 1300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill8 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}
	}

	return 0;
}

//---------------------------------***-Skill_9-***--------------------------------------------------//

struct FAttackSkill9
{
	LPCHARACTER tch;
	long Mob_X, Mob_Y;
	FAttackSkill9(LPCHARACTER c, long x, long y) : tch(c), Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(Mob_X - ch->GetX(), Mob_Y - ch->GetY()) <= 400)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!ch->IsPolymorphed() && !tch->IsZodiacSubays())
					{
						DWORD GetPolyNum = PolyDog(tch->GetRaceNum()) == false ? tch->GetStoneNum() : 101;
						ch->SetPolymorph(GetPolyNum, false);
					}
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_9_info)
{
	LPCHARACTER Mob;
	zodiac_skill_9_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_9_event)
{
	zodiac_skill_9_info* info = dynamic_cast<zodiac_skill_9_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_9_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_9_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_9_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill9 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 300, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill9 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}

		{
			float fx, fy;
			GetDeltaByDegree(Mob->GetRotation(), 800, &fx, &fy);

			PIXEL_POSITION pos = Mob->GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			FAttackSkill9 f(Mob, pos.x, pos.y);
			pkMap->for_each(f);
		}
	}

	return 0;
}

//---------------------------------***-Skill_10-***--------------------------------------------------//

struct FAttackSkill10
{
	LPCHARACTER tch;
	FAttackSkill10(LPCHARACTER c) : tch(c) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch || !tch)
			{
				sys_err("CH VEYA TCH YOK!");
				return;
			}

			if (ch->IsPC() && !ch->IsDead())
			{
				if (DISTANCE_APPROX(ch->GetX() - tch->GetX(), ch->GetY() - tch->GetY()) <= 1000)
				{
					BYTE sans = number(0, 5) == 3 ? 1 : 0;
					tch->ZodiacSkillAttack(ch, SKILL_VNUM+sans, SKILL_LEVEL(tch->GetLevel()));

					if (!tch->IsZodiacSubays())
						ch->AddAffect(AFFECT_STUN, POINT_NONE, 0, AFF_STUN, 5, 0, true);
				}
			}
		}
	}
};

EVENTINFO(zodiac_skill_10_info)
{
	LPCHARACTER Mob;
	zodiac_skill_10_info() : Mob(nullptr) {}
};

EVENTFUNC(zodiac_skill_10_event)
{
	zodiac_skill_10_info* info = dynamic_cast<zodiac_skill_10_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_10_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	if (Mob == nullptr)
	{
		sys_err("zodiac_skill_10_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_10_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill10 = nullptr;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(Mob->GetMapIndex());
	if (pkMap)
	{
		FAttackSkill10 f(Mob);
		pkMap->for_each(f);
	}

	return 0;
}

//---------------------------------***-Skill_11-***--------------------------------------------------//

EVENTINFO(zodiac_skill_11_info)
{
	LPCHARACTER Mob;
	LPCHARACTER Character;
	zodiac_skill_11_info() : Mob(nullptr), Character(nullptr) {}
};

EVENTFUNC(zodiac_skill_11_event)
{
	zodiac_skill_11_info* info = dynamic_cast<zodiac_skill_11_info*>(event->info);
	if (info == nullptr)
	{
		sys_err( "zodiac_skill_10_event> <Factor> nullptr pointer" );
		return 0;
	}

	LPCHARACTER Mob = info->Mob;
	LPCHARACTER Character = info->Character;
	if (Mob == nullptr || Character == nullptr)
	{
		sys_err("zodiac_skill_10_event> Cannot Mob");
		return 0;
	}

	if (!Mob->IsMonster() && !Mob->IsStone() && !Mob->IsZodiacBoss())
	{
		sys_err("zodiac_skill_10_event> Is cannot Monster or Stone");
		return 0;
	}

	Mob->m_pkZodiacSkill11 = nullptr;

	BYTE sans = number(0, 5) == 3 ? 1 : 0;
	Mob->ZodiacSkillAttack(Character, SKILL_VNUM+sans, SKILL_LEVEL(Mob->GetLevel()));
	return 0;
}

//---------------------------------------------------------------------------------------------------//

void CHARACTER::ZodiacDamage(BYTE Type, LPCHARACTER Victim)
{
	if (!IsZodiacBoss())
	{
		sys_err("Not zodiac boss! RaceNum::[%u]", GetRaceNum());
		return;
	}

	DWORD dwCurTime = get_dword_time();
	SendMovePacket(FUNC_MOB_SKILL, Type, GetX(), GetY(), 0, dwCurTime);

	// 0 > Buyuk merkez yuvarlak
	// 1 > Bastaki buyuk digerleri kucuk
	// 2 > Rastgele cikan 3 orta yuvarlak
	// 3 > Orta ilerideki yuvarlak
	// 4 > Ard arda 4 kucuk yuvarlak
	// 5 > Ard arda 3 Buyuk yuvarlak
	// 6 > Mavi cember disindaki herkesi Kopek yapar
	// 7 > Kucuk - Orta - Buyuk
	// 8 > Kucuk - Orta
	// 9 > Sersemletme
	// 10 > Yilanin skili

	if (GetRaceNum() >= 2750 && GetRaceNum() <= 2752)
	{
		if (Type == 0)
			Type = 3;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 6;
	}
	else if (GetRaceNum() >= 2760 && GetRaceNum() <= 2762)
	{
		if (Type == 0)
			Type = 4;
		else if (Type == 1)
			Type = 3;
		else if (Type == 2)
			Type = 0;
	}
	else if (GetRaceNum() >= 2770 && GetRaceNum() <= 2772)
	{
		if (Type == 0)
			Type = 5;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 2;
	}
	else if (GetRaceNum() >= 2780 && GetRaceNum() <= 2782)
	{
		if (Type == 0)
			Type = 7;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 8;
	}
	else if (GetRaceNum() >= 2790 && GetRaceNum() <= 2792)
	{
		if (Type == 0)
			Type = 4;
		else if (Type == 1)
			Type = 2;
		else if (Type == 2)
			Type = 0;
	}
	else if (GetRaceNum() >= 2800 && GetRaceNum() <= 2802)
	{
		if (Type == 0)
			Type = 10;
		if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 2;
	}
	else if (GetRaceNum() >= 2810 && GetRaceNum() <= 2812)
	{
		if (Type == 0)
			Type = 5;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 2;
	}
	else if (GetRaceNum() >= 2820 && GetRaceNum() <= 2822)
	{
		if (Type == 0)
			Type = 8;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 9;
	}
	else if (GetRaceNum() >= 2830 && GetRaceNum() <= 2832)
	{
		if (Type == 0)
			Type = 0;
		else if (Type == 1)
			Type = 1;
		else if (Type == 2)
			Type = 2;
	}
	else if (GetRaceNum() >= 2840 && GetRaceNum() <= 2842)
	{
		if (Type == 0)
			Type = 8;
		else if (Type == 1)
			Type = 0;
		else if (Type == 2)
			Type = 2;
	}
	else if (GetRaceNum() >= 2850 && GetRaceNum() <= 2852)
	{
		if (Type == 0)
			Type = 5;
		else if (Type == 1)
			Type = 3;
		else if (Type == 2)
			Type = 0;
	}
	else if (GetRaceNum() >= 2860 && GetRaceNum() <= 2862)
	{
		if (Type == 0)
			Type = 0;
		else if (Type == 1)
			Type = 3;
		else if (Type == 2)
			Type = 9;
	}

	if (Type == 0)
	{
		long x_amk = GetX();
		long y_amk = GetY();

		x_amk /= 100;
		y_amk /= 100;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			x_amk -= p.x / 100;
			y_amk -= p.y / 100;
		}

		x_amk *= 100;
		y_amk *= 100;

		EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE);

		if (m_pkZodiacSkill1)
		{
			event_cancel(&m_pkZodiacSkill1);
		}

		zodiac_skill_1_info * Info = AllocEventInfo<zodiac_skill_1_info>();
		Info->Mob = this;
		m_pkZodiacSkill1 = event_create(zodiac_skill_1_event, Info, PASSES_PER_SEC(2));
	}
	else if (Type == 1)
	{
		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		{
			long x_amk = GetX();
			long y_amk = GetY();

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 600, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1200, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1800, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		if (m_pkZodiacSkill2)
		{
			event_cancel(&m_pkZodiacSkill2);
		}

		zodiac_skill_2_info * Info = AllocEventInfo<zodiac_skill_2_info>();
		Info->Mob = this;
		m_pkZodiacSkill2 = event_create(zodiac_skill_2_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
	else if (Type == 2)
	{
		//--------------------------------------------------------------------------------------------//

		if (m_pkZodiacSkill3)
		{
			event_cancel(&m_pkZodiacSkill3);
		}

		zodiac_skill_3_info * Info = AllocEventInfo<zodiac_skill_3_info>();

		Info->Mob = this;

		{
			if (GetRaceNum() >= 2770 && GetRaceNum() <= 2772)
				Info->EfektType = SE_METEOR;
			else if (GetRaceNum() >= 2790 && GetRaceNum() <= 2792)
				Info->EfektType = SE_BEAD_RAIN;
			else if (GetRaceNum() >= 2800 && GetRaceNum() <= 2802)
				Info->EfektType = SE_ARROW_RAIN;
			else if (GetRaceNum() >= 2810 && GetRaceNum() <= 2812)
				Info->EfektType = SE_HORSE_DROP;
			else if (GetRaceNum() >= 2830 && GetRaceNum() <= 2832)
				Info->EfektType = SE_FALL_ROCK;
			else if (GetRaceNum() >= 2840 && GetRaceNum() <= 2842)
				Info->EfektType = SE_EGG_DROP;
		}

		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		int EfektX_1 = 0, EfektX_2 = 0, EfektX_3 = 0;
		int EfektY_1 = 0, EfektY_2 = 0, EfektY_3 = 0;

		EfektX_1 = -(700+number(0, 300));
		EfektY_1 = -(500+number(0, 300));

		EfektX_2 = 0;
		EfektY_2 = (500+number(0, 300));

		EfektX_3 = (700+number(0, 300));
		EfektY_3 = -(500+number(0, 300));

		if (number(0, 2) == 1)
		{
			EfektX_1 *= -1;
			EfektY_1 *= -1;

			EfektX_2 *= -1;
			EfektY_2 *= -1;

			EfektX_3 *= -1;
			EfektY_3 *= -1;
		}

		{
			long x_amk = GetX()+EfektX_1;
			long y_amk = GetY()+EfektY_1;

			Info->DamageX1 = x_amk;
			Info->DamageY1 = y_amk;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			Info->EfektX1 = x_amk;
			Info->EfektY1 = y_amk;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		{
			long x_amk = GetX()+EfektX_2;
			long y_amk = GetY()+EfektY_2;

			Info->DamageX2 = x_amk;
			Info->DamageY2 = y_amk;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			Info->EfektX2 = x_amk;
			Info->EfektY2 = y_amk;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		{
			long x_amk = GetX()+EfektX_3;
			long y_amk = GetY()+EfektY_3;

			Info->DamageX3 = x_amk;
			Info->DamageY3 = y_amk;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			Info->EfektX3 = x_amk;
			Info->EfektY3 = y_amk;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		m_pkZodiacSkill3 = event_create(zodiac_skill_3_event, Info, PASSES_PER_SEC(2));

		//--------------------------------------------------------------------------------------------//
	}
	else if (Type == 3)
	{
		float fx, fy;
		GetDeltaByDegree(GetRotation(), 500, &fx, &fy);

		PIXEL_POSITION pos = GetXYZ();

		pos.x += (long) fx;
		pos.y += (long) fy;

		long x_amk = pos.x;
		long y_amk = pos.y;

		x_amk /= 100;
		y_amk /= 100;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			x_amk -= p.x / 100;
			y_amk -= p.y / 100;
		}

		x_amk *= 100;
		y_amk *= 100;

		EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);

		if (m_pkZodiacSkill4)
		{
			event_cancel(&m_pkZodiacSkill4);
		}

		zodiac_skill_4_info * Info = AllocEventInfo<zodiac_skill_4_info>();
		Info->Mob = this;
		Info->EfektX = x_amk;
		Info->EfektY = y_amk;
		m_pkZodiacSkill4 = event_create(zodiac_skill_4_event, Info, PASSES_PER_SEC(2));
	}
	else if (Type == 4)
	{
		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 200, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 700, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1200, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1700, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		if (m_pkZodiacSkill5)
		{
			event_cancel(&m_pkZodiacSkill5);
		}

		zodiac_skill_5_info * Info = AllocEventInfo<zodiac_skill_5_info>();
		Info->Mob = this;
		m_pkZodiacSkill5 = event_create(zodiac_skill_5_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
	else if (Type == 5)
	{
		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		{
			long x_amk = GetX();
			long y_amk = GetY();

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 500, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1000, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		if (m_pkZodiacSkill6)
		{
			event_cancel(&m_pkZodiacSkill6);
		}

		zodiac_skill_6_info * Info = AllocEventInfo<zodiac_skill_6_info>();
		Info->Mob = this;
		m_pkZodiacSkill6 = event_create(zodiac_skill_6_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
	else if (Type == 6)
	{
		long x_amk = GetX();
		long y_amk = GetY();

		x_amk /= 100;
		y_amk /= 100;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			x_amk -= p.x / 100;
			y_amk -= p.y / 100;
		}

		x_amk *= 100;
		y_amk *= 100;

		EffectZodiacPacket(x_amk, y_amk, SE_SKILL_SAFE_ZONE);

		if (m_pkZodiacSkill7)
		{
			event_cancel(&m_pkZodiacSkill7);
		}

		zodiac_skill_7_info * Info = AllocEventInfo<zodiac_skill_7_info>();
		Info->Mob = this;
		m_pkZodiacSkill7 = event_create(zodiac_skill_7_event, Info, PASSES_PER_SEC(2));
	}
	else if (Type == 7)
	{
		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 200, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_KUCUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 700, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_ORTA);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 1200, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		if (m_pkZodiacSkill8)
		{
			event_cancel(&m_pkZodiacSkill8);
		}

		zodiac_skill_8_info * Info = AllocEventInfo<zodiac_skill_8_info>();
		Info->Mob = this;
		m_pkZodiacSkill8 = event_create(zodiac_skill_8_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
	else if (Type == 8)
	{
		long XX = 0;
		long YY = 0;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			XX = p.x / 100;
			YY = p.y / 100;
		}

		{
			long x_amk = GetX();
			long y_amk = GetY();

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_BUYUK);
		}

		{
			float fx, fy;
			GetDeltaByDegree(GetRotation(), 500, &fx, &fy);

			PIXEL_POSITION pos = GetXYZ();

			pos.x += (long) fx;
			pos.y += (long) fy;

			long x_amk = pos.x;
			long y_amk = pos.y;

			x_amk /= 100;
			y_amk /= 100;

			x_amk -= XX;
			y_amk -= YY;

			x_amk *= 100;
			y_amk *= 100;

			EffectZodiacPacket(x_amk, y_amk, SE_SKILL_DAMAGE_ZONE_ORTA);
		}

		if (m_pkZodiacSkill9)
		{
			event_cancel(&m_pkZodiacSkill9);
		}

		zodiac_skill_9_info * Info = AllocEventInfo<zodiac_skill_9_info>();
		Info->Mob = this;
		m_pkZodiacSkill9 = event_create(zodiac_skill_9_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
	else if (Type == 9)
	{
		long x_amk = GetX();
		long y_amk = GetY();

		x_amk /= 100;
		y_amk /= 100;

		PIXEL_POSITION p;
		if (SECTREE_MANAGER::instance().GetMapBasePosition(GetX(), GetY(), p))
		{
			x_amk -= p.x / 100;
			y_amk -= p.y / 100;
		}

		x_amk *= 100;
		y_amk *= 100;

		EffectZodiacPacket(x_amk, y_amk, SE_SKILL_SAFE_ZONE);

		if (m_pkZodiacSkill10)
		{
			event_cancel(&m_pkZodiacSkill10);
		}

		zodiac_skill_10_info * Info = AllocEventInfo<zodiac_skill_10_info>();
		Info->Mob = this;
		m_pkZodiacSkill10 = event_create(zodiac_skill_10_event, Info, PASSES_PER_SEC(2));
	}
	else if (Type == 10)
	{
		if (!Victim || Victim == nullptr)
			return;

		Victim->EffectPacket(SE_SKILL_DAMAGE_ZONE_KUCUK);

		if (m_pkZodiacSkill11)
		{
			event_cancel(&m_pkZodiacSkill11);
		}

		zodiac_skill_11_info * Info = AllocEventInfo<zodiac_skill_11_info>();
		Info->Mob = this;
		Info->Character = Victim;
		m_pkZodiacSkill11 = event_create(zodiac_skill_11_event, Info, PASSES_PER_SEC(2) - (passes_per_sec / 2));
	}
}

//-----------------------------------------------------------------------------------------------------//
struct FuncZodiacSplashDamage
{
	FuncZodiacSplashDamage(CSkillProto * pkSk, LPCHARACTER pkChr, BYTE bUseSkillPower)
		: m_pkSk(pkSk), m_pkChr(pkChr), m_bUseSkillPower(bUseSkillPower)
	{}

	void operator () (LPCHARACTER pkChrVictim)
	{
		if (!m_pkChr || !pkChrVictim)
		{
			return;
		}

		if (!battle_is_attackable(m_pkChr, pkChrVictim))
		{
			return;
		}

		m_pkSk->SetPointVar("k", 1.0 * m_bUseSkillPower * m_pkSk->bMaxLevel / 100);
		m_pkSk->SetPointVar("lv", m_pkChr->GetLevel());

		m_pkSk->SetPointVar("iq", m_pkChr->GetPoint(POINT_IQ) - (135-m_pkChr->GetLevel()));
		m_pkSk->SetPointVar("str", m_pkChr->GetPoint(POINT_ST) - (135-m_pkChr->GetLevel()));
		m_pkSk->SetPointVar("dex", m_pkChr->GetPoint(POINT_DX) - (135-m_pkChr->GetLevel()));
		m_pkSk->SetPointVar("con", m_pkChr->GetPoint(POINT_HT) - (135-m_pkChr->GetLevel()));
		m_pkSk->SetPointVar("def", m_pkChr->GetPoint(POINT_DEF_GRADE) - (135-m_pkChr->GetLevel()));
		m_pkSk->SetPointVar("odef", m_pkChr->GetPoint(POINT_DEF_GRADE) - m_pkChr->GetPoint(POINT_DEF_GRADE_BONUS));
		m_pkSk->SetPointVar("horse_level", m_pkChr->GetHorseLevel());

		bool bIgnoreDefense = false;
		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_PENETRATE))
		{
			int iPenetratePct = (int) m_pkSk->kPointPoly2.Eval();
			if (number(1, 100) <= iPenetratePct)
				bIgnoreDefense = true;
		}

		bool bIgnoreTargetRating = false;
		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_IGNORE_TARGET_RATING))
		{
			int iPct = (int) m_pkSk->kPointPoly2.Eval();
			if (number(1, 100) <= iPct)
				bIgnoreTargetRating = true;
		}

		m_pkSk->SetPointVar("ar", CalcAttackRating(m_pkChr, pkChrVictim, bIgnoreTargetRating));
		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_USE_MELEE_DAMAGE))
			m_pkSk->SetPointVar("atk", CalcMeleeDamage(m_pkChr, pkChrVictim, true, bIgnoreTargetRating));
		else if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_USE_ARROW_DAMAGE))
		{
			LPITEM pkBow, pkArrow;
			if (1 == m_pkChr->GetArrowAndBow(&pkBow, &pkArrow, 1))
				m_pkSk->SetPointVar("atk", CalcArrowDamage(m_pkChr, pkChrVictim, pkBow, pkArrow, true));
			else
				m_pkSk->SetPointVar("atk", 0);
		}

		if (m_pkSk->bPointOn == POINT_MOV_SPEED)
			m_pkSk->kPointPoly.SetVar("maxv", pkChrVictim->GetLimitPoint(POINT_MOV_SPEED));

		m_pkSk->SetPointVar("maxhp", pkChrVictim->GetMaxHP());
		m_pkSk->SetPointVar("maxsp", pkChrVictim->GetMaxSP());

		m_pkSk->SetPointVar("chain", m_pkChr->GetChainLightningIndex());
		m_pkChr->IncChainLightningIndex();

		m_pkSk->SetPointVar("ek", m_pkChr->GetAffectedEunhyung()*1./100);

		int iAmount = 0;
		if (m_pkChr->GetUsedSkillMasterType(m_pkSk->dwVnum) >= SKILL_GRAND_MASTER)
		{
			iAmount = (int) m_pkSk->kMasterBonusPoly.Eval();
		}
		else
		{
			iAmount = (int) m_pkSk->kPointPoly.Eval();
		}

		iAmount = -iAmount;

		int iDam;

		iDam = CalcBattleDamage(iAmount, m_pkChr->GetLevel(), pkChrVictim->GetLevel());

		#ifdef __ANTI_RESIST_MAGIC_BONUS__
		int reduce_resist_magic = 0;
		#endif

		EDamageType dt = DAMAGE_TYPE_NONE;
		switch (m_pkSk->bSkillAttrType)
		{
			case SKILL_ATTR_TYPE_NORMAL:
				break;

			case SKILL_ATTR_TYPE_MELEE:
				{
					dt = DAMAGE_TYPE_MELEE;

					if (!bIgnoreDefense)
						iDam -= pkChrVictim->GetPoint(POINT_DEF_GRADE);
				}
				break;

			case SKILL_ATTR_TYPE_RANGE:
				dt = DAMAGE_TYPE_RANGE;
				iDam = iDam * (100 - pkChrVictim->GetPoint(POINT_RESIST_BOW)) / 100;
				break;

			case SKILL_ATTR_TYPE_MAGIC:
				dt = DAMAGE_TYPE_MAGIC;
				iDam = CalcAttBonus(m_pkChr, pkChrVictim, iDam);

				#ifdef __ANTI_RESIST_MAGIC_BONUS__
				reduce_resist_magic = pkChrVictim->GetPoint(POINT_RESIST_MAGIC);

				if (m_pkChr->GetPoint(POINT_ANTI_RESIST_MAGIC) > 0)
				{
					// Fix Magic Resistance : If player have more than 100% magic resistance, will be modified to 100%;
					int fix_magic_resistance = (pkChrVictim->GetPoint(POINT_RESIST_MAGIC) > 100) ? (int)(100) : (int)(pkChrVictim->GetPoint(POINT_RESIST_MAGIC));
					// End Fix;

					reduce_resist_magic = fix_magic_resistance - m_pkChr->GetPoint(POINT_ANTI_RESIST_MAGIC);
					if (reduce_resist_magic < 1)
						reduce_resist_magic = 0;
				}
				#endif

				#ifdef __ANTI_RESIST_MAGIC_BONUS__
				iDam = iDam * (100 - reduce_resist_magic) / 100;
				#else
				iDam = iDam * (100 - pkChrVictim->GetPoint(POINT_RESIST_MAGIC)) / 100;
				#endif
				break;

			default:
				sys_err("Unknown skill attr type %u vnum %u", m_pkSk->bSkillAttrType, m_pkSk->dwVnum);
				break;
		}

		if (pkChrVictim->IsNPC())
		{
			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_WIND))
			{
				iDam = iDam * (100 - pkChrVictim->GetPoint(POINT_RESIST_WIND)) / 100;
			}

			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_ELEC))
			{
				iDam = iDam * (100 - pkChrVictim->GetPoint(POINT_RESIST_ELEC)) / 100;
			}

			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_FIRE))
			{
				iDam = iDam * (100 - pkChrVictim->GetPoint(POINT_RESIST_FIRE)) / 100;
			}
		}

		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_COMPUTE_MAGIC_DAMAGE))
			dt = DAMAGE_TYPE_MAGIC;

		if (pkChrVictim->CanBeginFight())
			pkChrVictim->BeginFight(m_pkChr);

		/*if (m_pkSk->dwVnum == 270)
		{
			int eksi_deger = (135-m_pkChr->GetLevel()) * number(0, 10);
			if (eksi_deger > 0)
				iDam -= eksi_deger;
		}
		if (m_pkSk->dwVnum == 271)
		{
			int eksi_deger = (135-m_pkChr->GetLevel()) * number(15, 30);
			if (eksi_deger > 0)
				iDam -= eksi_deger;
		}

		if (m_pkChr->IsZodiacSubays())
		{
			int eksi_deger = (135-m_pkChr->GetLevel()) * number(70, 80);
			if (eksi_deger > 0)
				iDam -= eksi_deger;
		}*/

		if (!pkChrVictim->Damage(m_pkChr, iDam, dt) && !pkChrVictim->IsStun())
		{
			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_REMOVE_GOOD_AFFECT))
			{
				int iAmount2 = (int) m_pkSk->kPointPoly2.Eval();
				int iDur2 = (int) m_pkSk->kDurationPoly2.Eval();
				iDur2 += m_pkChr->GetPoint(POINT_PARTY_BUFFER_BONUS);

				if (number(1, 100) <= iAmount2)
				{
					pkChrVictim->RemoveGoodAffect();
					pkChrVictim->AddAffect(m_pkSk->dwVnum, POINT_NONE, 0, AFF_PABEOP, iDur2, 0, true);
				}
			}

			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_SLOW | SKILL_FLAG_STUN | SKILL_FLAG_FIRE_CONT | SKILL_FLAG_POISON | SKILL_FLAG_BLEEDING))
			{
				int iPct = (int) m_pkSk->kPointPoly2.Eval();
				int iDur = (int) m_pkSk->kDurationPoly2.Eval();

				iDur += m_pkChr->GetPoint(POINT_PARTY_BUFFER_BONUS);

				if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_STUN))
				{
					SkillAttackAffect(pkChrVictim, iPct, IMMUNE_STUN, AFFECT_STUN, POINT_NONE, 0, AFF_STUN, iDur, m_pkSk->szName);
				}
				else if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_SLOW))
				{
					SkillAttackAffect(pkChrVictim, iPct, IMMUNE_SLOW, AFFECT_SLOW, POINT_MOV_SPEED, -30, AFF_SLOW, iDur, m_pkSk->szName);
				}
				else if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_FIRE_CONT))
				{
					m_pkSk->SetDurationVar("k", 1.0 * m_bUseSkillPower * m_pkSk->bMaxLevel / 100);
					m_pkSk->SetDurationVar("iq", m_pkChr->GetPoint(POINT_IQ));

					iDur = (int)m_pkSk->kDurationPoly2.Eval();
					int bonus = m_pkChr->GetPoint(POINT_PARTY_BUFFER_BONUS);

					if (bonus != 0)
					{
						iDur += bonus / 2;
					}

					if (number(1, 100) <= iDur)
					{
						pkChrVictim->AttackedByFire(m_pkChr, iPct, 5);
					}
				}
				else if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_POISON))
				{
					if (number(1, 100) <= iPct && !pkChrVictim->IsAffectFlag(AFF_BLEEDING))
						pkChrVictim->AttackedByPoison(m_pkChr);
				}
				else if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_BLEEDING))
				{
					if (number(1, 100) <= iPct && !pkChrVictim->IsAffectFlag(AFF_POISON))
						pkChrVictim->AttackedByBleeding(m_pkChr);
				}
			}

			if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_CRUSH | SKILL_FLAG_CRUSH_LONG) &&
				!IS_SET(pkChrVictim->GetAIFlag(), AIFLAG_NOMOVE))
			{
				float fCrushSlidingLength = 200;

				if (m_pkChr->IsNPC())
					fCrushSlidingLength = 400;

				if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_CRUSH_LONG))
					fCrushSlidingLength *= 2;

				float fx, fy;
				float degree = GetDegreeFromPositionXY(m_pkChr->GetX(), m_pkChr->GetY(), pkChrVictim->GetX(), pkChrVictim->GetY());

				if (m_pkSk->dwVnum == SKILL_HORSE_WILDATTACK)
				{
					degree -= m_pkChr->GetRotation();
					degree = fmod(degree, 360.0f) - 180.0f;

					if (degree > 0)
						degree = m_pkChr->GetRotation() + 90.0f;
					else
						degree = m_pkChr->GetRotation() - 90.0f;
				}

				GetDeltaByDegree(degree, fCrushSlidingLength, &fx, &fy);
				sys_log(0, "CRUSH! %s -> %s (%d %d) -> (%d %d)", m_pkChr->GetName(), pkChrVictim->GetName(), pkChrVictim->GetX(), pkChrVictim->GetY(), (long)(pkChrVictim->GetX()+fx), (long)(pkChrVictim->GetY()+fy));
				long tx = (long)(pkChrVictim->GetX()+fx);
				long ty = (long)(pkChrVictim->GetY()+fy);

				pkChrVictim->Sync(tx, ty);
				pkChrVictim->Goto(tx, ty);
				pkChrVictim->CalculateMoveDuration();
			}
		}

		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_HP_ABSORB))
		{
			int iPct = (int) m_pkSk->kPointPoly2.Eval();
			m_pkChr->PointChange(POINT_HP, iDam * iPct / 100);
		}

		if (IS_SET(m_pkSk->dwFlag, SKILL_FLAG_SP_ABSORB))
		{
			int iPct = (int) m_pkSk->kPointPoly2.Eval();
			m_pkChr->PointChange(POINT_SP, iDam * iPct / 100);
		}
	}

	CSkillProto * m_pkSk;
	LPCHARACTER	m_pkChr;
	BYTE m_bUseSkillPower;
};

int CHARACTER::ZodiacSkillAttack(LPCHARACTER Victim, DWORD dwVnum, BYTE bSkillLevel)
{
	if (!dwVnum || dwVnum == 0)
		return BATTLE_NONE;

	if (!bSkillLevel || bSkillLevel == 0)
		return BATTLE_NONE;

	if (!IsMonster() || !IsZodiacBoss())
		return BATTLE_NONE;

	if (!Victim)
		return BATTLE_NONE;

	if (GetMountVnum())
		return BATTLE_NONE;

	if (IsPolymorphed())
		return BATTLE_NONE;

	if (g_bSkillDisable)
		return BATTLE_NONE;

	CSkillProto * pkSk = CSkillManager::instance().Get(dwVnum);

	if (!pkSk)
		return BATTLE_NONE;

	if (!IS_SET(pkSk->dwFlag, SKILL_FLAG_SPLASH))
		return BATTLE_NONE;

	const float k = 1.0 * GetSkillPower(pkSk->dwVnum, bSkillLevel) * pkSk->bMaxLevel / 100;

	pkSk->SetPointVar("k", k);
	pkSk->kSplashAroundDamageAdjustPoly.SetVar("k", k);

	if (IS_SET(pkSk->dwFlag, SKILL_FLAG_USE_MELEE_DAMAGE))
	{
		pkSk->SetPointVar("atk", CalcMeleeDamage(this, this, true, false));
	}
	else if (IS_SET(pkSk->dwFlag, SKILL_FLAG_USE_MAGIC_DAMAGE))
	{
		pkSk->SetPointVar("atk", CalcMagicDamage(this, this));
	}
	else if (IS_SET(pkSk->dwFlag, SKILL_FLAG_USE_ARROW_DAMAGE))
	{
		LPITEM pkBow, pkArrow;
		if (1 == GetArrowAndBow(&pkBow, &pkArrow, 1))
		{
			pkSk->SetPointVar("atk", CalcArrowDamage(this, this, pkBow, pkArrow, true));
		}
		else
		{
			pkSk->SetPointVar("atk", 0);
		}
	}

	if (pkSk->bPointOn == POINT_MOV_SPEED)
	{
		pkSk->SetPointVar("maxv", this->GetLimitPoint(POINT_MOV_SPEED));
	}

	pkSk->SetPointVar("lv", GetLevel());
	pkSk->SetPointVar("iq", GetPoint(POINT_IQ));
	pkSk->SetPointVar("str", GetPoint(POINT_ST));
	pkSk->SetPointVar("dex", GetPoint(POINT_DX));
	pkSk->SetPointVar("con", GetPoint(POINT_HT));
	pkSk->SetPointVar("maxhp", this->GetMaxHP());
	pkSk->SetPointVar("maxsp", this->GetMaxSP());
	pkSk->SetPointVar("chain", 0);
	pkSk->SetPointVar("ar", CalcAttackRating(this, this));
	pkSk->SetPointVar("def", GetPoint(POINT_DEF_GRADE));
	pkSk->SetPointVar("odef", GetPoint(POINT_DEF_GRADE) - GetPoint(POINT_DEF_GRADE_BONUS));
	pkSk->SetPointVar("horse_level", GetHorseLevel());

	if (pkSk->bSkillAttrType != SKILL_ATTR_TYPE_NORMAL)
		OnMove(true);

	pkSk->SetDurationVar("k", k/*bSkillLevel*/);

	int iAmount = (int) pkSk->kPointPoly.Eval();
	int iAmount2 = (int) pkSk->kPointPoly2.Eval();

	if (GetUsedSkillMasterType(pkSk->dwVnum) >= SKILL_GRAND_MASTER)
	{
		iAmount = (int) pkSk->kMasterBonusPoly.Eval();
	}

	if (IS_SET(pkSk->dwFlag, SKILL_FLAG_REMOVE_BAD_AFFECT))
	{
		if (number(1, 100) <= iAmount2)
		{
			RemoveBadAffect();
		}
	}
	// END_OF_ADD_GRANDMASTER_SKILL

	if (IS_SET(pkSk->dwFlag, SKILL_FLAG_ATTACK | SKILL_FLAG_USE_MELEE_DAMAGE | SKILL_FLAG_USE_MAGIC_DAMAGE))
	{
		if (pkSk->bPointOn == POINT_HP && iAmount < 0)
		{
			// FuncZodiacSplashDamage f(pkSk, this, Victim, GetSkillPower(dwVnum, bSkillLevel));

			FuncZodiacSplashDamage(pkSk, this, GetSkillPower(dwVnum, bSkillLevel)) (Victim);

			/*if (IS_SET(pkSk->dwFlag, SKILL_FLAG_SPLASH))
			{
				if (GetSectree())
					GetSectree()->ForEachAround(f);
			}
			else
			{
				f(this);
			}*/
		}

		return BATTLE_DAMAGE;
	}

	return BATTLE_NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FuncKanonaAttack
{
	LPCHARACTER m_pkKanone;
	FuncKanonaAttack(LPCHARACTER ch) : m_pkKanone(ch) {};

	void operator () (LPENTITY ent)
	{
		if (!ent->IsType(ENTITY_CHARACTER))
			return;

		LPCHARACTER pkChr = (LPCHARACTER) ent;

		if (!pkChr || !m_pkKanone)
			return;

		if (DISTANCE_APPROX(pkChr->GetX() - m_pkKanone->GetX(), pkChr->GetY() - m_pkKanone->GetY()) <= 2000)
		{
			if (pkChr->IsZodiacBoss())
			{
				if (number(0, 100) <= 5)
				{
					int iDam = pkChr->GetMaxHP() / 10;
					pkChr->Damage(m_pkKanone, iDam, DAMAGE_TYPE_NORMAL);
				}
			}
		}
	}
};

void CHARACTER::KanoneDamage()
{
	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());
	if (pkMap)
	{
		FuncKanonaAttack f(this);
		pkMap->for_each(f);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD SubayVnum(BYTE Vnum)
{
	switch(Vnum)
	{
		case 1:
			return 2750;
			break;

		case 2:
			return 2760;
			break;

		case 3:
			return 2770;
			break;

		case 4:
			return 2780;
			break;

		case 5:
			return 2790;
			break;

		case 6:
			return 2800;
			break;

		case 7:
			return 2810;
			break;

		case 8:
			return 2820;
			break;

		case 9:
			return 2830;
			break;

		case 10:
			return 2840;
			break;

		case 11:
			return 2850;
			break;

		case 12:
			return 2860;
			break;

		default:
			return 0;
			break;
	}
}

BYTE MobLevel(BYTE Floor)
{
	if (Floor >= 1 && Floor <= 3)
	{
		return number(15, 30);
	}
	if (Floor >= 4 && Floor <= 6)
	{
		return number(30, 45);
	}
	else if (Floor >= 7 && Floor <= 9)
	{
		return number(45, 60);
	}
	else if (Floor >= 10 && Floor <= 12)
	{
		return number(60, 75);
	}
	else if (Floor >= 13 && Floor <= 15)
	{
		return number(75, 80);
	}
	else if (Floor >= 16 && Floor <= 21)
	{
		return number(80, 85);
	}
	else if (Floor >= 22 && Floor <= 28)
	{
		return number(85, 100);
	}
	else if (Floor >= 29 && Floor <= 34)
	{
		return number(100, 135);
	}
	else
		return 0;
}

BYTE SubayLevel(BYTE Floor)
{
	BYTE Level = 50;
	BYTE Dongu = 11;

	for (BYTE b = 8; b < 40; ++b)
	{
		if (Level > 135)
			return 135;

		if (b == Floor)
			return Level;

		if ((Dongu - b) == 0)
		{
			Dongu += 3;
			Level += 10;
		}
	}

	return 0;
}

DWORD GroupVnum(BYTE Portal)
{
	if (Portal == 1)
		return number(MIN(2900, 2902), MAX(2900, 2902));
	else if (Portal == 2)
		return number(MIN(2903, 2905), MAX(2903, 2905));
	else if (Portal == 3)
		return number(MIN(2906, 2908), MAX(2906, 2908));
	else if (Portal == 4)
		return number(MIN(2909, 2911), MAX(2909, 2911));
	else if (Portal == 5)
		return number(MIN(2912, 2914), MAX(2912, 2914));
	else if (Portal == 6)
		return number(MIN(2915, 2917), MAX(2915, 2917));
	else if (Portal == 7)
		return number(MIN(2918, 2920), MAX(2918, 2920));
	else if (Portal == 8)
		return number(MIN(2921, 2923), MAX(2921, 2923));
	else if (Portal == 9)
		return number(MIN(2924, 2926), MAX(2924, 2926));
	else if (Portal == 10)
		return number(MIN(2927, 2929), MAX(2927, 2929));
	else if (Portal == 11)
		return number(MIN(2930, 2932), MAX(2930, 2932));
	else if (Portal == 12)
		return number(MIN(2933, 2935), MAX(2933, 2935));
	else
		return 0;
}

void CHARACTER::SpawnZodiacGroup(LPZODIAC pZodiac)
{
	if (!pZodiac)
	{
		sys_err("Zodyakci yok!");
		return;
	}

	BYTE Floor = pZodiac->GetFloor();

	if (Floor == 7 || Floor == 21)
		return;

	BYTE Portal = pZodiac->GetPortal();
	BYTE Level = MobLevel(Floor);

	m_dwStateDuration = PASSES_PER_SEC(1);

	DWORD dwVnum = GroupVnum(Portal);
	if (dwVnum == 0)
	{
		sys_err("PORTAL YANLIS AQ");
		return;
	}

	SendMovePacket(FUNC_ATTACK, 0, GetX(), GetY(), 0);

	CHARACTER_MANAGER::instance().SelectStone(this);
	CHARACTER_MANAGER::instance().SpawnGroupZodiac(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000, nullptr, true, pZodiac, Level);
	CHARACTER_MANAGER::instance().SpawnGroupZodiac(dwVnum, GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000, nullptr, true, pZodiac, Level);

	if (number(0, 7) == 3)
		CHARACTER_MANAGER::instance().SpawnGroupZodiac(SubayVnum(Portal), GetMapIndex(), GetX() - 1000, GetY() - 1000, GetX() + 1000, GetY() + 1000, nullptr, true, pZodiac, SubayLevel(Floor));

	CHARACTER_MANAGER::instance().SelectStone(nullptr);

	UpdatePacket();
	return;
}

void CHARACTER::SpawnZodiacStone(LPZODIAC pZodiac)
{
	if (!pZodiac)
	{
		sys_err("Zodyakci yok!");
		return;
	}

	BYTE Floor = pZodiac->GetFloor();

	const CMob* pkMob = nullptr;
	DWORD vnum = 0;

	vnum = 2900+number(0, 8);

	if (vnum == 2904 || vnum == 2905 || vnum == 2906)
	{
		if (number(0,2) == 1)
			vnum = 2900+number(0, 3);
		else
			vnum = 2900+number(7, 8);
	}

	if ((pkMob = CMobManager::instance().Get(vnum)) == nullptr)
		vnum = 0;

	if (vnum != 0 && pkMob != nullptr)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum, 
			GetMapIndex(), 
			GetX() - number(1500, 3000), 
			GetY() - number(1500, 3000), 
			GetX() + number(1500, 3000), 
			GetY() + number(1500, 3000), 
			true, 
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			false,
			(Floor == 6 || Floor == 7) ? 40 : SubayLevel(Floor));

		if (ch)
		{
			int newHP = ch->GetMaxHP()+(ch->GetLevel()*5000);
			ch->SetMaxHP(newHP);
			ch->SetHP(newHP);
			ch->UpdatePacket();
		}
	}
}

#endif
