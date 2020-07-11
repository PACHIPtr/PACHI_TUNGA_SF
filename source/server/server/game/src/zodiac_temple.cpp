#include "stdafx.h"
#include "zodiac_temple.h"
#include "dungeon.h"
#include "char.h"
#include "char_manager.h"
#include "party.h"
#include "affect.h"
#include "packet.h"
#include "desc.h"
#include "config.h"
#include "regen.h"
#include "start_position.h"
#include "item.h"
#include "db.h"
#include "log.h"
#include "item_manager.h"
#include "utils.h"
#include "questmanager.h"
#include "entity.h"
#include "sectree_manager.h"
#include "mob_manager.h"

int GoToPosition[9][2] =
{
	{3332, 14310},//260 - 245
	{3328, 14815},//260 - 750
	{3328, 15331},//260 - 1250
	//---------//
	{3839, 14299},//750 - 245
	{3839, 14812},//750 - 750
	{3839, 15331},//750 - 1250
	//---------//
	{4359, 14294},//1250 - 245
	{4352, 14814},//1250 - 750
	{4357, 15331},//1250 - 1250
};

int SpawnPosition[9][2] =
{
	{3333, 14344},
	{3328, 14848},
	{3328, 15365},
	//---------//
	{3839, 14335},
	{3839, 14848},
	{3839, 15363},
	//---------//
	{4359, 14327},
	{4352, 14840},
	{4357, 15360},
};

const char * c_Portal[13] =
{
	"","Zi","Chou","Yin","Mao","Chen","Si","Wu","Wei","Shen","Yu","Xu","Hai",
};

//--------------------------------------------------------------------------------------------//

EVENTINFO(Floor_event_data)
{
	int seconds;
	BYTE floor;
	long mapIndex;

	Floor_event_data()
	: seconds(0), floor(0), mapIndex(0)
	{}
};

EVENTFUNC(Floor_event_data_start)
{
	Floor_event_data * info = dynamic_cast<Floor_event_data *>(event->info);
	if (info == nullptr)
	{
		sys_err( "Floor_event_data_start> <Factor> nullptr pointer" );
		return 0;
	}

	LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(info->mapIndex);
	if (pZodiac == nullptr)
	{
		sys_err("Cannot LPZODIAC");
		return 0;
	}

	int * pSecondMaintenance = &(info->seconds);
	if (*pSecondMaintenance == 0 || *pSecondMaintenance < 1)
	{
		pZodiac->NewFloor(info->floor);

		if (pZodiac->zodiac_floor_event_data)
			pZodiac->zodiac_floor_event_data = nullptr;

		return 0;
	}

	--*pSecondMaintenance;
	return passes_per_sec;
}

EVENTINFO(Floor_remaining_time_event_data)
{
	int seconds;
	BYTE seconds2;
	long mapIndex;
	BYTE floor;

	Floor_remaining_time_event_data()
	: seconds(0), seconds2(0), mapIndex(0), floor(0)
	{}
};

EVENTFUNC(Floor_remaining_time_event_data_start)
{
	Floor_remaining_time_event_data * info = dynamic_cast<Floor_remaining_time_event_data *>(event->info);
	if (info == nullptr)
	{
		sys_err( "Floor_remaining_time_event_data_start> <Factor> nullptr pointer" );
		return 0;
	}

	LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(info->mapIndex);
	if (pZodiac == nullptr)
	{
		sys_err("Cannot LPZODIAC");
		return 0;
	}

	int * pSecondMaintenance = &(info->seconds);
	if (*pSecondMaintenance == 0 || *pSecondMaintenance < 1)
	{
		pZodiac->TimeIsUp();

		if (pZodiac->zodiac_remaining_time_temple_floor)
			pZodiac->zodiac_remaining_time_temple_floor = nullptr;

		return 0;
	}

	BYTE * time = &(info->seconds2);

	BYTE Floor = info->floor;
	if (Floor != 7 && 
		Floor != 8 && 
		Floor != 11 && 
		Floor != 13 && 
		Floor != 14 && 
		Floor != 15 && 
		Floor != 17 && 
		Floor != 18 && 
		Floor != 21 && 
		Floor != 22 && 
		Floor != 27 && 
		Floor != 28 && 
		Floor != 29 && 
		Floor != 30 && 
		Floor != 34 && 
		Floor != 35 && 
		Floor != 36 && 
		Floor != 37 && 
		Floor != 38 && 
		Floor != 39 && 
		Floor != 40)
	{
		++*time;
	}

	if (*time == 2)
	{
		pZodiac->ControlMob();
		*time = 0;
	}

	--*pSecondMaintenance;
	return passes_per_sec;
}

EVENTINFO(Exit_temple_event_data)
{
	int seconds;
	long mapIndex;

	Exit_temple_event_data()
	: seconds(0), mapIndex(0)
	{}
};

EVENTFUNC(Exit_temple_event_data_start)
{
	Exit_temple_event_data * info = dynamic_cast<Exit_temple_event_data *>(event->info);
	if (info == nullptr)
	{
		sys_err( "Exit_temple_event_data_start> <Factor> nullptr pointer" );
		return 0;
	}

	LPZODIAC pZodiac = CZodiacManager::instance().FindByMapIndex(info->mapIndex);
	if (pZodiac == nullptr)
	{
		sys_err("Cannot LPZODIAC");
		return 0;
	}

	int * pSecondMaintenance = &(info->seconds);
	if (*pSecondMaintenance == 0 || *pSecondMaintenance < 1)
	{
		pZodiac->ExitTemple();

		if (pZodiac->zodiac_exit_temple_event_data)
			pZodiac->zodiac_exit_temple_event_data = nullptr;

		return 0;
	}

	--*pSecondMaintenance;
	return passes_per_sec;
}

//--------------------------------------------------------------------------------------------//

CZodiac::CZodiac(IdType id, long lMapIndex, BYTE bPortal)
	: m_id(id), m_lMapIndex(lMapIndex), m_bPortal(bPortal)
{
	Initialize();
}

CZodiac::~CZodiac()
{
	if (m_pParty != nullptr)
	{
		m_pParty->SetZodiac_for_Only_party(nullptr);
	}

	event_cancel(&zodiac_floor_event_data);
	event_cancel(&zodiac_remaining_time_temple_floor);
	event_cancel(&zodiac_exit_temple_event_data);
	event_cancel(&deadEvent);
}

void CZodiac::Initialize()
{
	regen_id_ = 0;
	m_iMonsterCount = 0;
	m_iMobKill = 0;
	m_iTotalMonster = 0;
	m_iStoneKill = 0;
	m_iBossKill = 0;
	m_bFloor = 0;
	m_bNextFloor = 0;
	m_bNextFloorControl = false;
	m_bPosition = 0;
	m_dwGetGlobalTime = 0;
	m_bTimeDown = false;
	m_pParty = nullptr;
	zodiac_floor_event_data = nullptr;
	zodiac_remaining_time_temple_floor = nullptr;
	zodiac_exit_temple_event_data = nullptr;
}

void CZodiac::SetFlag(std::string name, int value)
{
	auto it = m_map_Flag.find(name);
	if (it != m_map_Flag.end())
		it->second = value;
	else
		m_map_Flag.insert(make_pair(name, value));
}

int CZodiac::GetFlag(std::string name)
{
	auto it = m_map_Flag.find(name);
	if (it != m_map_Flag.end())
		return it->second;
	else
		return 0;
}

///-------------------------------------------------------------------------------------------///
struct FWarpToPosition
{
	long lMapIndex;
	long x;
	long y;
	FWarpToPosition(long lMapIndex, long x, long y)
		: lMapIndex(lMapIndex), x(x), y(y)
		{}

	void operator()(LPENTITY ent)
	{
		if (!ent->IsType(ENTITY_CHARACTER))
		{
			return;
		}

		LPCHARACTER ch = (LPCHARACTER)ent;
		if (!ch->IsPC())
		{
			return;
		}

		if (ch->GetMapIndex() == lMapIndex)
		{
			ch->Show(lMapIndex, x, y, 0);
			ch->Stop();
		}
		else
		{
			ch->WarpSet(x,y,lMapIndex);
		}
	}
};

void CZodiac::Jump(LPCHARACTER ch, long lFromMapIndex, int x, int y)
{
	if (!ch)
		return;

	if (!ch->IsPC())
	{
		sys_err("cannot by character");
		return;
	}

	x *= 100;
	y *= 100;

	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(lFromMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", lFromMapIndex);
		return;
	}

	if (ch->GetMapIndex() == (long)m_lMapIndex)
	{
		ch->Show(m_lMapIndex, x, y, 0);
		ch->Stop();
	}
	else
	{
		ch->WarpSet(x, y, m_lMapIndex);
	}
}

void CZodiac::JumpAll(long lFromMapIndex, int x, int y)
{
	x *= 100;
	y *= 100;

	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(lFromMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", lFromMapIndex);
		return;
	}

	FWarpToPosition f(m_lMapIndex, x, y);
	pMap->for_each(f);
}

void CZodiac::JumpParty(LPPARTY pParty, long lFromMapIndex, int x, int y)
{
	if (!pParty)
		return;

	x *= 100;
	y *= 100;

	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(lFromMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", lFromMapIndex);
		return;
	}

	if (pParty->GetZodiac_for_Only_party() == nullptr)
	{
		if (m_pParty == nullptr)
		{
			m_pParty = pParty;
		}
		else if (m_pParty != pParty)
		{
			sys_err ("Zodiac already has party. Another party cannot jump in Zodiac : index %d", GetMapIndex());
			return;
		}

		pParty->SetZodiac_for_Only_party(this);
	}

	FWarpToPosition f(m_lMapIndex, x, y);
	pParty->ForEachOnMapMember(f, lFromMapIndex);
}
///-------------------------------------------------------------------------------------------///

void CZodiac::SpawnRegen(const char* filename, bool bOnce)
{
	if (!filename)
	{
		sys_err("CZodiac::SpawnRegen(filename=nullptr, bOnce=%d) - m_lMapIndex[%d]", bOnce, m_lMapIndex); 
		return;
	}

	LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pkSectreeMap)
	{
		sys_err("CZodiac::SpawnRegen(filename=%s, bOnce=%d) - m_lMapIndex[%d]", filename, bOnce, m_lMapIndex); 
		return;
	}

	regen_zodiac(filename, m_lMapIndex, pkSectreeMap->m_setting.iBaseX, pkSectreeMap->m_setting.iBaseY, this, bOnce);
}

void CZodiac::AddRegen(LPREGEN regen)
{
	regen->id = regen_id_++;
	m_regen.push_back(regen);
}

void CZodiac::ClearRegen()
{
	for (auto it = m_regen.begin(); it != m_regen.end(); ++it)
	{
		LPREGEN regen = *it;

		event_cancel(&regen->event);
		M2_DELETE(regen);
	}

	m_regen.clear();
}

bool CZodiac::IsValidRegen(LPREGEN regen, size_t regen_id)
{
	auto it = std::find(m_regen.begin(), m_regen.end(), regen);
	if (it == m_regen.end())
	{
		return false;
	}

	LPREGEN found = *it;
	return (found->id == regen_id);
}
//-----------------------------------------------------------------------------------------------//

#ifdef ENABLE_ZODIAC_TEMPLE_CHAT
struct FZodiacNotice
{
	FZodiacNotice(const char * psz) : m_psz(psz)
	{
	}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, "%s", m_psz);
		}
	}

	const char * m_psz;
};

void CZodiac::ZodiacMessage(const char* msg)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", MAP_INDEX);
		return;
	}

	FZodiacNotice f(msg);
	pMap->for_each(f);
}

struct FZodiacNoticeFloor
{
	BYTE floor;
	FZodiacNoticeFloor(BYTE f) : floor(f)
	{
	}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->ChatPacket(CHAT_TYPE_ZODIAC_NOTICE, LC_TEXT("Misyon tamamlandi. Grup lideri butonu kullandigi anda, dogrudan %u.kata isinlanirsiniz."), floor);
		}
	}
};

void CZodiac::ZodiacMessageMisyon(BYTE Floor)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return;
	}

	FZodiacNoticeFloor f(Floor);
	pMap->for_each(f);
}

struct FZodiacMessageFloor
{
	BYTE Floor;
	BYTE Count;
	FZodiacMessageFloor(BYTE f, BYTE c) : Floor(f), Count(c)
	{
	}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
			{
				ch->ZodiacFloorMessage(Floor);
			}
		}
	}
};

void CZodiac::ZodiacFloorMessage(BYTE Floor, BYTE Count)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return;
	}

	FZodiacMessageFloor f(Floor, Count);
	pMap->for_each(f);
}

struct FZodiacSubMessageFloor
{
	const char * m_psz;
	FZodiacSubMessageFloor(const char * psz) : m_psz(psz)
	{
	}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
			{
				ch->ChatPacket(CHAT_TYPE_ZODIAC_SUB_NOTICE, "%s", m_psz);
			}
		}
	}
};

void CZodiac::ZodiacFloorSubMessage(const char * msg)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return;
	}

	FZodiacSubMessageFloor f(msg);
	pMap->for_each(f);
}

struct FZodiacNoticeClear
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->ChatPacket(CHAT_TYPE_ZODIAC_NOTICE_CLEAR, "Zodiac");
		}
	}
};

void CZodiac::ZodiacMessageClear()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", MAP_INDEX);
		return;
	}

	FZodiacNoticeClear f;
	pMap->for_each(f);
}
#endif

struct FZodiacTime
{
	BYTE currentfloor, nextfloor;
	int time, time2;

	FZodiacTime(BYTE c, BYTE n, int t, int t2)
		: currentfloor(c), nextfloor(n), time(t), time2(t2)
	{}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacTime %d %d %d %d", currentfloor, nextfloor, time, time2);
		}
	}
};

void CZodiac::ZodiacTime(BYTE currentfloor, BYTE nextfloor, int time, int time2)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", MAP_INDEX);
		return;
	}

	FZodiacTime f(currentfloor, nextfloor, time, time2);
	pMap->for_each(f);
}

struct FZodiacTimeClear
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->ChatPacket(CHAT_TYPE_COMMAND, "ZodiacTimeClear");
		}
	}
};

void CZodiac::ZodiacTimeClear()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", MAP_INDEX);
		return;
	}

	FZodiacTimeClear f;
	pMap->for_each(f);
}
//----------------------------------------------------------------------------------------//

void CZodiac::ControlMob()
{
	/*if (ControlCharacter() == true)
	{
		sys_err("Adam kalmamis.");
		CZodiacManager::instance().Destroy(m_id);
		return;
	}*/
	
	if (zodiac_floor_event_data)
	{
		return;
	}

	if (zodiac_exit_temple_event_data)
	{
		return;
	}

	BYTE Floor = GetFloor();

	if (Floor >= 40)
	{
		return;
	}

	if (Floor == 1)
	{
		/*if ((get_global_time() - GetTime()) > 120 && m_bTimeDown == false)
		{
			m_bTimeDown = true;
			DWORD Time = 10*60;
			DWORD Time_1 = (get_global_time()+1) - GetTime();
			ZodiacTime(Floor, 1, Time, Time_1);
		}*/

		if ((get_global_time() - GetTime()) > 60 && GetFlag("floor1secount60") == 0)
		{
			SetFlag("floor1secount60", 1);
			DWORD Time = 10*60;
			DWORD Time_1 = (get_global_time()+1) - GetTime();
			ZodiacTime(Floor, 4, Time, Time_1);
		}
		else if ((get_global_time() - GetTime()) > 120 && GetFlag("floor1secount120") == 0)
		{
			SetFlag("floor1secount120", 1);
			DWORD Time = 10*60;
			DWORD Time_1 = (get_global_time()+1) - GetTime();
			ZodiacTime(Floor, 3, Time, Time_1);
		}
		else if ((get_global_time() - GetTime()) > 180 && GetFlag("floor1secount180") == 0)
		{
			SetFlag("floor1secount180", 1);
			DWORD Time = 10*60;
			DWORD Time_1 = (get_global_time()+1) - GetTime();
			ZodiacTime(Floor, 2, Time, Time_1);
		}
		else if ((get_global_time() - GetTime()) > 240 && GetFlag("floor1secount240") == 0)
		{
			SetFlag("floor1secount240", 1);
			DWORD Time = 10*60;
			DWORD Time_1 = (get_global_time()+1) - GetTime();
			ZodiacTime(Floor, 1, Time, Time_1);
		}
	}

	if (CountMonster() == 0)
	{
		if (Floor == 1)
		{
			if ((get_global_time() - GetTime()) <= 60)
			{
				Floor = 6;

				if (zodiac_remaining_time_temple_floor)
				{
					event_cancel(&zodiac_remaining_time_temple_floor);
					zodiac_remaining_time_temple_floor = nullptr;
				}

				m_bNextFloor = Floor;
				m_bNextFloorControl = true;

				ZodiacMessageMisyon(Floor);
				ZodiacTime(1, 5, 10);
				NextFloorButton();
				return;
			}
			else if ((get_global_time() - GetTime()) <= 120)
			{
				Floor = 5;

				if (zodiac_remaining_time_temple_floor)
				{
					event_cancel(&zodiac_remaining_time_temple_floor);
					zodiac_remaining_time_temple_floor = nullptr;
				}

				m_bNextFloor = Floor;
				m_bNextFloorControl = true;

				ZodiacMessageMisyon(Floor);
				ZodiacTime(1, 4, 10);
				NextFloorButton();
				return;
			}
			else if ((get_global_time() - GetTime()) <= 180)
			{
				Floor = 3;

				if (zodiac_remaining_time_temple_floor)
				{
					event_cancel(&zodiac_remaining_time_temple_floor);
					zodiac_remaining_time_temple_floor = nullptr;
				}

				m_bNextFloor = Floor;
				m_bNextFloorControl = true;

				ZodiacMessageMisyon(Floor);
				ZodiacTime(1, 2, 10);
				NextFloorButton();
				return;
			}
			else if ((get_global_time() - GetTime()) <= 240)
			{
				Floor = 2;

				if (zodiac_remaining_time_temple_floor)
				{
					event_cancel(&zodiac_remaining_time_temple_floor);
					zodiac_remaining_time_temple_floor = nullptr;
				}

				m_bNextFloor = Floor;
				m_bNextFloorControl = true;

				ZodiacMessageMisyon(Floor);
				ZodiacTime(1, 1, 10);
				NextFloorButton();
				return;
			}
		}

		if (zodiac_remaining_time_temple_floor)
		{
			event_cancel(&zodiac_remaining_time_temple_floor);
			zodiac_remaining_time_temple_floor = nullptr;
		}

		Floor += 1;

		m_bNextFloor = Floor;
		m_bNextFloorControl = true;

		ZodiacMessageMisyon(Floor);
		ZodiacTime(Floor-1, 1, 10);
		NextFloorButton();
	}
}

struct FCountMonster
{
	int n;
	FCountMonster() : n(0) {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch->IsPC() && !ch->IsDead() && !ch->IsPet() && !ch->IsSupportShaman() && !ch->IsMount() && !ch->IsNPC() && !ch->IsNewPet())
				n++;
		}
	}
};

int CZodiac::CountMonster()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return -1;
	}

	return SECTREE_MANAGER::instance().GetMonsterCountInMap(m_lMapIndex);;
}

struct FCountIsBoss
{
	int n;
	FCountIsBoss() : n(0) {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch->IsPC() && !ch->IsDead() && (ch->IsMonster() || ch->IsStone()))
			{
				if (ch->IsZodiacBoss())
				{
					n++;
				}
			}
		}
	}
};

int CZodiac::CountIsBoss()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return 0;
	}

	FCountIsBoss f;
	pMap->for_each(f);
	return f.n;
}

struct FCountIsStone
{
	int n;
	FCountIsStone() : n(0) {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch->IsPC() && !ch->IsDead() && (ch->IsMonster() || ch->IsStone()))
			{
				if (ch->IsStone())
				{
					n++;
				}
			}
		}
	}
};

int CZodiac::CountIsStone()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return 0;
	}

	FCountIsStone f;
	pMap->for_each(f);
	return f.n;
}

struct FCountCharacter
{
	int n;
	FCountCharacter() : n(0) {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->IsPC())
				n++;
		}
	}
};

bool CZodiac::ControlCharacter()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return false;
	}

	FCountCharacter f;
	pMap->for_each(f);
	int count = f.n;
	if (count == 0)
		return true;

	return false;
}

struct FZodiacExitAll
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
				ch->WarpSet(333200, 1431000, 0);
		}
	}
};

void CZodiac::ExitTemple()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return;
	}

	FZodiacExitAll f;
	pMap->for_each(f);
}

void CZodiac::TimeIsUp()
{
	if (zodiac_floor_event_data)
		event_cancel(&zodiac_floor_event_data);

	if (zodiac_remaining_time_temple_floor)
		event_cancel(&zodiac_remaining_time_temple_floor);

	if (zodiac_exit_temple_event_data)
		event_cancel(&zodiac_exit_temple_event_data);

	if (deadEvent)
		event_cancel(&deadEvent);

	KillAll();

	ZodiacFloorMessage(43);
	ZodiacTime(GetFloor(), 0, 10);

	Exit_temple_event_data * info = AllocEventInfo<Exit_temple_event_data>();
	info->seconds = 10;
	info->mapIndex = GetMapIndex();
	zodiac_exit_temple_event_data = event_create(Exit_temple_event_data_start, info, 1);
}

struct FPurgeSectree
{
	void operator () (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;

			if (!ch)
				return;

			if (!ch->IsPC()
				&& !ch->IsDead()
				&& !ch->IsPet()
				&& !ch->IsNewPet()
				&& !ch->IsSupportShaman()
				&& !ch->IsMount())
			{
				M2_DESTROY_CHARACTER(ch);
			}
		}
		else if (ent->IsType(ENTITY_ITEM))
		{
			LPITEM item = (LPITEM) ent;
			M2_DESTROY_ITEM(item);
		}
		else
			sys_err("unknown entity type %d is in zodiac", ent->GetType());
	}
};

void CZodiac::Purge()
{
	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FPurgeSectree f;
	pkMap->for_each(f);
}

struct FKillSectree
{
	void operator () (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;

			if (!ch->IsPC() && !ch->IsDead()
				&& !ch->IsPet()
				&& !ch->IsNewPet()
				&& !ch->IsSupportShaman()
				&& !ch->IsMount()
				)
			{
				ch->Dead();

				if (ch->GetSectree())
				{
					ch->GetSectree()->RemoveEntity(ch);
					ch->ViewCleanup();
				}
			}
		}
	}
};

void CZodiac::KillAll()
{
	if (CountMonster() == 0)
		return;

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FKillSectree f;
	pkMap->for_each(f);
}

//----------------------------------------------------------------------------------------//

void CZodiac::NewFloor(BYTE Floor)
{
	DWORD Second = 0;
	if (Floor >= 1 && Floor < 7)
		Second = 10*60;
	else if (Floor == 7)
		Second = 5*60;
	else if (Floor >= 8 && Floor < 14)
		Second = 10*60;
	else if (Floor == 14)
		Second = 5*60;
	else if (Floor >= 15 && Floor < 21)
		Second = 10*60;
	else if (Floor == 21)
		Second = 5*60;
	else if (Floor >= 22 && Floor < 28)
		Second = 10*60;
	else if (Floor == 28)
		Second = 5*60;
	else if (Floor >= 29 && Floor < 35)
		Second = 10*60;
	else if (Floor >= 35 && Floor <= 40)
		Second = 5*60;

	m_dwGetGlobalTime = get_global_time();
	// m_bTimeDown = false;

	if (zodiac_floor_event_data)
	{
		event_cancel(&zodiac_floor_event_data);
		zodiac_floor_event_data = nullptr;
	}

	if (zodiac_remaining_time_temple_floor)
	{
		event_cancel(&zodiac_remaining_time_temple_floor);
		zodiac_remaining_time_temple_floor = nullptr;
	}

	Floor_remaining_time_event_data * info = AllocEventInfo<Floor_remaining_time_event_data>();
	info->seconds = Second;
	info->mapIndex = m_lMapIndex;
	info->floor = Floor;
	zodiac_remaining_time_temple_floor = event_create(Floor_remaining_time_event_data_start, info, 1);

	m_bFloor = Floor;
	m_bNextFloorControl = false;

	m_iMobKill = 0;
	m_iStoneKill = 0;
	m_iBossKill = 0;
	m_iTotalMonster = 0;

	ZodiacFloorMessage(Floor);
	ZodiacTime(Floor, 1, Second);

	BYTE Portal = 0;
	if ((Portal = GetPortal()) == 0)
	{
		sys_err("cannot find by map::Portal");
		return;
	}

	if (Floor == 1)
	{
		ZodiacTime(Floor, 5, Second);

		m_bPosition = 1;
		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		ProcessZodyakItems(false);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 2)
	{
		m_bPosition = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 3)
	{
		m_bPosition = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 4)
	{
		m_bPosition = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 5)
	{
		m_bPosition = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 6)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(1)");

		SpawnBoss(1);
	}
	else if (Floor == 7)
	{
		m_bPosition = 1;

		SetFlag("bonus", 0);
		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 8)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 8;
		m_iTotalMonster = 8;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(8)");

		for (BYTE b = 0; b < 8; ++b)
			SpawnStone();
	}
	else if (Floor == 9)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 10)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 11)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iBossKill = 3;
		m_iTotalMonster = 3;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/3)");

		for (BYTE b = 0; b < 4; ++b)
			SpawnStone();

		for (BYTE b = 0; b < 3; ++b)
			SpawnBoss(0);
	}
	else if (Floor == 12)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 13)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 8;
		m_iTotalMonster = 8;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/8)");

		for (BYTE b = 0; b < 8; ++b)
			SpawnStone();
	}
	else if (Floor == 14)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 10;
		m_iTotalMonster = 10;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/10)");

		SpawnStone(true);
	}
	else if (Floor == 15)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iMobKill = 100;
		m_iTotalMonster = 100;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(100)");

		for (BYTE b = 0; b < 15; ++b)
			SpawnMob();
	}
	else if (Floor == 16)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 17)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iBossKill = 1;
		m_iTotalMonster = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/1)");

		SpawnBoss(0);

		for (BYTE b = 0; b < 4; ++b)
			SpawnStone();
	}
	else if (Floor == 18)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 7;
		m_iTotalMonster = 7;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(7)");

		for (BYTE b = 0; b < 7; ++b)
			SpawnStone();
	}
	else if (Floor == 19)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 20)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 21)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		SetFlag("bonus", 0);

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 22)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iBossKill = 3;
		m_iTotalMonster = 3;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/3)");

		for (BYTE b = 0; b < 3; ++b)
			SpawnMob();

		for (BYTE b = 0; b < 3; ++b)
			SpawnBoss(0);
	}
	else if (Floor == 23)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 24)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 4;
		m_iTotalMonster = 4;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 25)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 26)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 27)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 4;
		m_iTotalMonster = 4;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(4)");

		for (BYTE b = 0; b < 5; ++b)
			SpawnMob();

		for (BYTE b = 0; b < 4; ++b)
			SpawnStone();
	}
	else if (Floor == 28)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 10;
		m_iTotalMonster = 10;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/10)");

		SpawnStone(true);
	}
	else if (Floor == 29)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 8;
		m_iTotalMonster = 8;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/8)");

		for (BYTE b = 0; b < 8; ++b)
			SpawnStone();
	}
	else if (Floor == 30)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iStoneKill = 8;
		m_iTotalMonster = 8;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(0/8)");

		for (BYTE b = 0; b < 8; ++b)
			SpawnStone();
	}
	else if (Floor == 31)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 32)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 33)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 7; ++b)
			SpawnMob();
	}
	else if (Floor == 34)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		m_iMobKill = 80;
		m_iBossKill = 1;

		m_iTotalMonster = 80;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		ZodiacFloorMessage(Floor);
		ZodiacFloorSubMessage("(80)");

		for (BYTE b = 0; b < 12; ++b)
			SpawnMob();
	}
	//------------------------------------------------------------------------//
	else if (Floor == 35)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 36)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 37)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 38)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	else if (Floor == 39)
	{
		BYTE n = number(1, 8);

		m_bPosition = n+1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[n][0], GoToPosition[n][1]);

		for (BYTE b = 0; b < 2; ++b)
			SpawnStone();
	}
	//------------------------------------------------------------------------//
	else if (Floor == 40)
	{
		m_bPosition = 1;

		KillAll();

		JumpAll(GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

		ZodiacTime(40, 0, Second);
		ZodiacFloorMessage(40);

		SpawnRegen("data/dungeon/zodiac/zodiac_seller.txt");

		ProcessZodyakItems(true);
	}
	//------------------------------------------------------------------------//
}

//--------------------------------------------------------------------------------------------//

void CZodiac::SetPartyNull()
{
	m_pParty = nullptr;
}

struct FWarpToZodiac
{
	FWarpToZodiac(long lMapIndex, LPZODIAC z) : m_lMapIndex(lMapIndex), m_pkZodiac(z)
	{
		LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
		m_x = pkSectreeMap->m_setting.posSpawn.x;
		m_y = pkSectreeMap->m_setting.posSpawn.y; 
	}

	void operator () (LPCHARACTER ch)
	{
		ch->SaveExitLocation();
		ch->WarpSet(m_x, m_y, m_lMapIndex);
	}

	long m_lMapIndex;
	long m_x;
	long m_y;
	LPZODIAC m_pkZodiac;
};

void CZodiac::Join(LPCHARACTER ch)
{
	if (!ch)
		return;

	if (SECTREE_MANAGER::instance().GetMap(m_lMapIndex) == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FWarpToZodiac(m_lMapIndex, this) (ch);
}

void CZodiac::JoinParty(LPPARTY pParty)
{
	if (!pParty)
	{
		sys_err("Cannot Party");
		return;
	}

	pParty->SetZodiac(this);
	m_map_pkParty.insert(std::make_pair(pParty, 0));

	if (SECTREE_MANAGER::instance().GetMap(m_lMapIndex) == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FWarpToZodiac f(m_lMapIndex, this);
	pParty->ForEachOnlineMember(f);
}

void CZodiac::QuitParty(LPPARTY pParty)
{
	if (!pParty)
	{
		sys_err("Cannot Party");
		return;
	}

	pParty->SetZodiac(nullptr);

	TPartyMap::iterator it = m_map_pkParty.find(pParty);
	if (it != m_map_pkParty.end())
		m_map_pkParty.erase(it);
}

void CZodiac::IncPartyMember(LPPARTY pParty, LPCHARACTER ch)
{
	if (!ch)
		return;

	if (!pParty)
	{
		sys_err("Cannot Party");
		return;
	}

	TPartyMap::iterator it = m_map_pkParty.find(pParty);

	if (it != m_map_pkParty.end())
		it->second++;
	else
		m_map_pkParty.insert(std::make_pair(pParty, 1));

	IncMember(ch);
}

void CZodiac::DecPartyMember(LPPARTY pParty, LPCHARACTER ch)
{
	if (!ch)
		return;

	if (!pParty)
	{
		sys_err("Cannot Party");
		return;
	}

	if (quest::CQuestManager::instance().GetEventFlag("zodyak_dusme_2_kapat") == 1)
		return;

	TPartyMap::iterator it = m_map_pkParty.find(pParty);

	if (it == m_map_pkParty.end())
		sys_err("cannot find party");
	else
	{
		it->second--;

		if (it->second == 0)
			QuitParty(pParty);
	}

	DecMember(ch);
}

void CZodiac::IncMember(LPCHARACTER ch)
{
	if (!ch)
		return;

	if (m_set_pkCharacter.find(ch) == m_set_pkCharacter.end())
		m_set_pkCharacter.insert(ch);

	event_cancel(&deadEvent);
}

///-----------------------------------------------------------------------------------------///

EVENTINFO(zodiac_id_info)
{
	CZodiac::IdType zodiac_id;

	zodiac_id_info() : zodiac_id(0)
	{}
};

EVENTFUNC(zodiac_dead_event)
{
	zodiac_id_info* info = dynamic_cast<zodiac_id_info*>(event->info);

	if (info == nullptr)
	{
		sys_err("zodiac_dead_event> <Factor> nullptr pointer");
		return 0;
	}

	LPZODIAC pZodiac = CZodiacManager::instance().Find(info->zodiac_id);
	if (pZodiac == nullptr)
	{
		return 0;
	}

	pZodiac->deadEvent = nullptr;

	CZodiacManager::instance().Destroy(info->zodiac_id);
	return 0;
}

void CZodiac::DecMember(LPCHARACTER ch)
{
	if (!ch)
		return;

	if (quest::CQuestManager::instance().GetEventFlag("zodyak_dusme_1_kapat") == 1)
		return;

	if (m_set_pkCharacter.empty())
	{
		sys_err("DecMember::Liste zaten bos!");

		zodiac_id_info* info = AllocEventInfo<zodiac_id_info>();
		info->zodiac_id = m_id;

		event_cancel(&deadEvent);
		deadEvent = event_create(zodiac_dead_event, info, PASSES_PER_SEC(60*5));
		return;
	}

	if (quest::CQuestManager::instance().GetEventFlag("zodyak_dusme_1_yontem") == 0)
	{
		auto it = m_set_pkCharacter.find(ch);
		if (it == m_set_pkCharacter.end())
		{
			return;
		}

		m_set_pkCharacter.erase(it);
	}
	else
	{
		for (auto it = m_set_pkCharacter.begin(); it != m_set_pkCharacter.end(); ++it)
		{
			if (ch == *it)
				m_set_pkCharacter.erase(*it);
		}
	}

	if (m_set_pkCharacter.empty())
	{
		zodiac_id_info* info = AllocEventInfo<zodiac_id_info>();
		info->zodiac_id = m_id;

		event_cancel(&deadEvent);
		deadEvent = event_create(zodiac_dead_event, info, PASSES_PER_SEC(10));
	}
}

//----------------------------------------------------------------------------------------------------//

struct FPolyMonster
{
	long Mob_X, Mob_Y;
	FPolyMonster(long x, long y) : Mob_X(x), Mob_Y(y) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->IsPC() && !ch->IsDead() && !ch->IsPolymorphed())
			{
				if (DISTANCE_APPROX(ch->GetX() - Mob_X, ch->GetY() - Mob_Y) > 1000)
				{
					ch->SetPolymorph(101, false);
				}
			}
		}
	}
};

void CZodiac::PolyCharacter(LPCHARACTER Mob)
{
	if (!Mob)
	{
		sys_err("Cannot Mob!");
		return;
	}

	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FPolyMonster f(Mob->GetX(), Mob->GetY());
	pkMap->for_each(f);
}

void CZodiac::StartLogin()
{
	m_bFloor = 0;
	m_bNextFloor = 0;
	m_bNextFloorControl = false;

	if (zodiac_floor_event_data)
	{
		sys_err("Bug Fixed");
		return;
	}

	Floor_event_data * info = AllocEventInfo<Floor_event_data>();
	info->seconds = 14;
	info->floor = 1;
	info->mapIndex = m_lMapIndex;
	zodiac_floor_event_data = event_create(Floor_event_data_start, info, 1);
}

///////////////////////////////

struct FProcessZodyakItem
{
	bool Enable;
	FProcessZodyakItem(bool e) : Enable(e) {};

	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch && ch->IsPC())
			{
				if (Enable)
				{
					for (int i = 1; i <= 22; i++)
					{
						ch->SetZodiacItems(33000+i, 50);
					}

					ch->SetZodiacItems(33025, 200);
					ch->SetZodiacItems(72329, 1);
				}
				else
				{
					for (int i = 1; i <= 22; i++)
					{
						ch->SetZodiacItems(33000+i, 0);
						ch->SetPurchaseZodiacItems(33000+i, 0);
					}

					ch->SetZodiacItems(33025, 0);
					ch->SetZodiacItems(72329, 0);

					ch->SetPurchaseZodiacItems(33025, 0);
					ch->SetPurchaseZodiacItems(72329, 0);
				}
			}
		}
	}
};

void CZodiac::ProcessZodyakItems(bool enable)
{
	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FProcessZodyakItem f(enable);
	pkMap->for_each(f);
}

struct FNextFloorButton
{
	LPZODIAC m_pkZodiac;
	FNextFloorButton(LPZODIAC z) : m_pkZodiac(z)
	{}

	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (!m_pkZodiac)
			{
				sys_err("Cannot Zodiac");
				return;
			}

			if (ch->IsPC())
			{
				if ((ch->GetParty() && ch->GetParty()->GetLeaderPID() == ch->GetPlayerID()) || !ch->GetParty())
				{
					ch->ChatPacket(CHAT_TYPE_COMMAND, "NextFloorButton");
				}
				else if (ch->GetParty() && ch->GetParty()->GetLeaderPID() != ch->GetPlayerID())
				{
					if (CHARACTER_MANAGER::instance().FindByPID(ch->GetParty()->GetLeaderPID()) == NULL)
					{
						if (m_pkZodiac->IsNextFloorStatus() == true)
						{
							m_pkZodiac->SetNextFloorStatus(false);
							ch->ChatPacket(CHAT_TYPE_COMMAND, "NextFloorButton");
							return;
						}
					}
				}
			}
		}
	}
};

void CZodiac::NextFloorButton()
{
	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	SetNextFloorStatus(true);

	FNextFloorButton f(this);
	pkMap->for_each(f);
}

///--------------------------------------------------------------------------------------------------///

BYTE MobLevels(BYTE Floor)
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

DWORD GroupMobVnum(BYTE Portal, BYTE Floor)
{
	if (Portal == 1)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2900;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2901, 2902), MAX(2901, 2902));
			else
				return number(MIN(2900, 2902), MAX(2900, 2902));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2900, 2902), MAX(2900, 2902));
			else
				return number(MIN(2901, 2902), MAX(2901, 2902));
		}
		else
			return 0;
	}
	else if (Portal == 2)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2903;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2904, 2905), MAX(2904, 2905));
			else
				return number(MIN(2903, 2905), MAX(2903, 2905));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2903, 2905), MAX(2903, 2905));
			else
				return number(MIN(2904, 2905), MAX(2904, 2905));
		}
		else
			return 0;
	}
	else if (Portal == 3)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2906;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2907, 2908), MAX(2907, 2908));
			else
				return number(MIN(2906, 2908), MAX(2906, 2908));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2906, 2908), MAX(2906, 2908));
			else
				return number(MIN(2907, 2908), MAX(2907, 2908));
		}
		else
			return 0;
	}
	else if (Portal == 4)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2909;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2910, 2911), MAX(2910, 2911));
			else
				return number(MIN(2909, 2911), MAX(2909, 2911));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2909, 2911), MAX(2909, 2911));
			else
				return number(MIN(2910, 2911), MAX(2910, 2911));
		}
		else
			return 0;
	}
	else if (Portal == 5)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2912;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2913, 2914), MAX(2913, 2914));
			else
				return number(MIN(2912, 2914), MAX(2912, 2914));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2912, 2914), MAX(2912, 2914));
			else
				return number(MIN(2913, 2914), MAX(2913, 2914));
		}
		else
			return 0;
	}
	else if (Portal == 6)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2915;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2916, 2917), MAX(2916, 2917));
			else
				return number(MIN(2915, 2917), MAX(2917, 2917));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2915, 2917), MAX(2915, 2917));
			else
				return number(MIN(2916, 2917), MAX(2916, 2917));
		}
		else
			return 0;
	}
	else if (Portal == 7)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2918;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2919, 2920), MAX(2919, 2920));
			else
				return number(MIN(2918, 2920), MAX(2918, 2920));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2918, 2920), MAX(2918, 2920));
			else
				return number(MIN(2919, 2920), MAX(2919, 2920));
		}
		else
			return 0;
	}
	else if (Portal == 8)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2921;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2922, 2923), MAX(2922, 2923));
			else
				return number(MIN(2921, 2923), MAX(2921, 2923));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2921, 2923), MAX(2921, 2923));
			else
				return number(MIN(2922, 2923), MAX(2922, 2923));
		}
		else
			return 0;
	}
	else if (Portal == 9)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2924;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2925, 2926), MAX(2925, 2926));
			else
				return number(MIN(2924, 2926), MAX(2924, 2926));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2924, 2926), MAX(2924, 2926));
			else
				return number(MIN(2925, 2926), MAX(2925, 2926));
		}
		else
			return 0;
	}
	else if (Portal == 10)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2927;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2928, 2929), MAX(2928, 2929));
			else
				return number(MIN(2927, 2929), MAX(2927, 2929));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2927, 2929), MAX(2927, 2929));
			else
				return number(MIN(2928, 2929), MAX(2928, 2929));
		}
		else
			return 0;
	}
	else if (Portal == 11)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2930;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2936, 2937), MAX(2936, 2937));
			else
				return number(MIN(2930, 2932), MAX(2930, 2932));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2930, 2932), MAX(2930, 2932));
			else
				return number(MIN(2936, 2937), MAX(2936, 2937));
		}
		else
			return 0;
	}
	else if (Portal == 12)
	{
		if (Floor >= 1 && Floor <= 8)
		{
			return 2933;
		}
		else if (Floor >= 9 && Floor <= 20)
		{
			if (number(0, 7) == 4)
				return number(MIN(2934, 2935), MAX(2934, 2935));
			else
				return number(MIN(2933, 2935), MAX(2933, 2935));
		}
		else if (Floor > 20 && Floor <= 39)
		{
			if (number(0, 7) == 4)
				return number(MIN(2933, 2935), MAX(2933, 2935));
			else
				return number(MIN(2934, 2935), MAX(2934, 2935));
		}
		else
			return 0;
	}
	else
		return 0;
}

void CZodiac::SpawnMob()
{
	BYTE P = GetPortal();
	BYTE F = GetFloor();
	BYTE N = m_bPosition-1;

	long GetX = SpawnPosition[N][0];
	long GetY = SpawnPosition[N][1];

	GetX *= 100;
	GetY *= 100;

	CHARACTER_MANAGER::instance().SpawnGroupZodiac(GroupMobVnum(P,F), GetMapIndex(), GetX - 2500, GetY - 2500, GetX + 2500, GetY + 2500, nullptr, true, this, MobLevels(F));
}

BYTE SubayLevels(BYTE Floor)
{
	BYTE Level = 50;
	BYTE Dongu = 10;

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

DWORD StatueVnum(BYTE Portal)
{
	if (Portal == 1)
		return 20452;
	else if (Portal == 2)
		return 20453;
	else if (Portal == 3)
		return 20454;
	else if (Portal == 4)
		return 20455;
	else if (Portal == 5)
		return 20456;
	else if (Portal == 6)
		return 20457;
	else if (Portal == 7)
		return 20458;
	else if (Portal == 8)
		return 20459;
	else if (Portal == 9)
		return 20460;
	else if (Portal == 10)
		return 20461;
	else if (Portal == 11)
		return 20462;
	else if (Portal == 12)
		return 20463;
	else
		return 0;
}

void CZodiac::SpawnStone(bool Statue)
{
	BYTE N = m_bPosition-1;

	long GetX1, GetY1, GetX2, GetY2;

	GetX1 = GetX2 = SpawnPosition[N][0]*100;
	GetY1 = GetY2 = SpawnPosition[N][1]*100;

	GetX1 -= 2200;
	GetY1 -= 2200;

	GetX2 += 2200;
	GetY2 += 2200;

	const CMob* pkMob = nullptr;
	DWORD vnum = 0;

	if (Statue == true)
	{
		vnum = StatueVnum(GetPortal());
	}
	else
	{
		vnum = 2900+number(0, 8);

		if (vnum == 2904 || vnum == 2905 || vnum == 2906)
		{
			if (number(0,3) == 1)
				vnum = 2900+number(0, 3);
			else
				vnum = 2900+number(7, 8);
		}
	}

	if ((pkMob = CMobManager::instance().Get(vnum)) == nullptr)
		vnum = 0;

	if (vnum != 0 && pkMob != nullptr)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum, 
			GetMapIndex(), 
			GetX1 - number(300, 500), 
			GetY1 - number(300, 500), 
			GetX2 + number(300, 500), 
			GetY2 + number(300, 500), 
			true, 
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			false,
			(GetFloor() == 6 || GetFloor() == 7) ? 40 : SubayLevels(GetFloor()));

		if (ch)
		{
			int newHP = ch->GetMaxHP()+(ch->GetLevel()*5000);
			ch->SetMaxHP(newHP);
			ch->SetHP(newHP);
			ch->UpdatePacket();
		}
	}
}

DWORD BossFuckVnum(BYTE Flag, BYTE Portal)
{
	if (Flag == 0)
	{
		if (Portal == 1)
		{
			return 2750;
		}
		else if (Portal == 2)
		{
			return 2760;
		}
		else if (Portal == 3)
		{
			return 2770;
		}
		else if (Portal == 4)
		{
			return 2780;
		}
		else if (Portal == 5)
		{
			return 2790;
		}
		else if (Portal == 6)
		{
			return 2800;
		}
		else if (Portal == 7)
		{
			return 2810;
		}
		else if (Portal == 8)
		{
			return 2820;
		}
		else if (Portal == 9)
		{
			return 2830;
		}
		else if (Portal == 10)
		{
			return 2840;
		}
		else if (Portal == 11)
		{
			return 2850;
		}
		else if (Portal == 12)
		{
			return 2860;
		}
		else
			return 0;
	}
	else if (Flag == 1)
	{
		if (Portal == 1)
		{
			return 2751;
		}
		else if (Portal == 2)
		{
			return 2761;
		}
		else if (Portal == 3)
		{
			return 2771;
		}
		else if (Portal == 4)
		{
			return 2781;
		}
		else if (Portal == 5)
		{
			return 2791;
		}
		else if (Portal == 6)
		{
			return 2801;
		}
		else if (Portal == 7)
		{
			return 2811;
		}
		else if (Portal == 8)
		{
			return 2821;
		}
		else if (Portal == 9)
		{
			return 2831;
		}
		else if (Portal == 10)
		{
			return 2841;
		}
		else if (Portal == 11)
		{
			return 2851;
		}
		else if (Portal == 12)
		{
			return 2861;
		}
		else
			return 0;
	}
	else if (Flag == 2)
	{
		if (Portal == 1)
		{
			return 2752;
		}
		else if (Portal == 2)
		{
			return 2762;
		}
		else if (Portal == 3)
		{
			return 2772;
		}
		else if (Portal == 4)
		{
			return 2782;
		}
		else if (Portal == 5)
		{
			return 2792;
		}
		else if (Portal == 6)
		{
			return 2802;
		}
		else if (Portal == 7)
		{
			return 2812;
		}
		else if (Portal == 8)
		{
			return 2822;
		}
		else if (Portal == 9)
		{
			return 2832;
		}
		else if (Portal == 10)
		{
			return 2842;
		}
		else if (Portal == 11)
		{
			return 2852;
		}
		else if (Portal == 12)
		{
			return 2862;
		}
		else
			return 0;
	}
	else
		return 0;
}

void CZodiac::SpawnBoss(BYTE Flag)
{
	BYTE N = m_bPosition-1;

	long GetX1, GetY1, GetX2, GetY2;

	GetX1 = GetX2 = SpawnPosition[N][0]*100;
	GetY1 = GetY2 = SpawnPosition[N][1]*100;

	GetX1 -= 2200;
	GetY1 -= 2200;

	GetX2 += 2200;
	GetY2 += 2200;

	const CMob* pkMob = nullptr;

	DWORD vnum = BossFuckVnum(Flag, GetPortal());

	if ((pkMob = CMobManager::instance().Get(vnum)) == nullptr)
		vnum = 0;

	if (vnum != 0 && pkMob != nullptr)
	{
		CHARACTER_MANAGER::instance().SpawnMobRange(vnum, 
			GetMapIndex(), 
			GetX1 - number(300, 500), 
			GetY1 - number(300, 500), 
			GetX2 + number(300, 500), 
			GetY2 + number(300, 500), 
			true, 
			true,
			true,
			(GetFloor() == 6 || GetFloor() == 7) ? 40 : SubayLevels(GetFloor()));
	}
}

///-----------------------------------------------------------------------------------------///

struct FGiveGiftBox
{
	void operator() (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (!ch)
			{
				sys_err("Cannot ch");
				return;
			}

			if (ch->IsPC())
			{
				ch->AutoGiveItem(33033, 1);
			}
		}
	}
};

void CZodiac::GiveGiftBox()
{
	LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (pkMap == nullptr)
	{
		sys_err("CZodiac: SECTREE_MAP not found for #%ld", m_lMapIndex);
		return;
	}

	FGiveGiftBox f;
	pkMap->for_each(f);
}

///-----------------------------------------------------------------------------------------///
///-----------------------------------------------------------------------------------------///
///-----------------------------------------------------------------------------------------///

CZodiacManager::CZodiacManager()
	: next_id_(0)
{
}

CZodiacManager::~CZodiacManager()
{
}

LPZODIAC CZodiacManager::Create(BYTE bPortal)
{
	DWORD lMapIndex = SECTREE_MANAGER::instance().CreatePrivateMap(MAP_INDEX);

	if (!lMapIndex) 
	{
		sys_log( 0, "Fail to Create Zodiac : OrginalMapindex %d NewMapindex %d", MAP_INDEX, lMapIndex );
		return nullptr;
	}

	CZodiac::IdType id = next_id_++;
	while (Find(id) != nullptr)
	{
		id = next_id_++;
	}

	//sys_err("New Map Index:: %u, Zodiac Id:: %u", lMapIndex, id);

	LPZODIAC pZodiac = M2_NEW CZodiac(id, lMapIndex, bPortal);
	if (!pZodiac)
	{
		sys_err("M2_NEW CZodiac failed");
		return nullptr;
	}

	m_map_pkZodiac.insert(std::make_pair(id, pZodiac));
	m_map_pkMapZodiac.insert(std::make_pair(lMapIndex, pZodiac));

	return pZodiac;
}

void CZodiacManager::Destroy(CZodiac::IdType zodiac_id)
{
	LPZODIAC pZodiac = Find(zodiac_id);
	if (pZodiac == nullptr)
	{
		return;
	}

	sys_log(0, "ZODIAC destroy : map index %u", zodiac_id);

	if (pZodiac->zodiac_floor_event_data)
		event_cancel(&pZodiac->zodiac_floor_event_data);

	if (pZodiac->zodiac_remaining_time_temple_floor)
		event_cancel(&pZodiac->zodiac_remaining_time_temple_floor);

	if (pZodiac->zodiac_exit_temple_event_data)
		event_cancel(&pZodiac->zodiac_exit_temple_event_data);

	m_map_pkZodiac.erase(zodiac_id);

	long lMapIndex = pZodiac->m_lMapIndex;
	m_map_pkMapZodiac.erase(lMapIndex);

	DWORD server_timer_arg = lMapIndex;
	quest::CQuestManager::instance().CancelServerTimers(server_timer_arg);

	SECTREE_MANAGER::instance().DestroyPrivateMap(lMapIndex);
	M2_DELETE(pZodiac);
}

LPZODIAC CZodiacManager::Find(CZodiac::IdType zodiac_id)
{
	auto it = m_map_pkZodiac.find(zodiac_id);
	if (it != m_map_pkZodiac.end())
		return it->second;

	return nullptr;
}

LPZODIAC CZodiacManager::FindByMapIndex(long lMapIndex)
{
	auto it = m_map_pkMapZodiac.find(lMapIndex);
	if (it != m_map_pkMapZodiac.end())
	{
		return it->second;
	}

	return nullptr;
}

void CZodiacManager::StartTemple(LPCHARACTER pkChar, BYTE portal)
{
	if (!pkChar)
	{
		sys_err("Cannot LPCHARACTER");
		return;
	}

	LPZODIAC pZodiac = Create(portal);
	if (!pZodiac)
	{
		sys_err("cannot create zodiac %d", MAP_INDEX);
		return;
	}

	if (pkChar->GetParty() != nullptr)
		pZodiac->JumpParty(pkChar->GetParty(), pkChar->GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);
	else
		pZodiac->Jump(pkChar, pkChar->GetMapIndex(), GoToPosition[0][0], GoToPosition[0][1]);

	if (portal > 0)
	{
		pZodiac->m_bPortal = portal;
		pZodiac->StartLogin();
	}
	else
		pZodiac->ExitTemple();
}

void CZodiacManager::DeadMob(LPCHARACTER Mob, DWORD mapIndex)
{
	if (!Mob)
	{
		sys_err("Cannot Monster");
		return;
	}

	if (!Mob->IsZodiacBoss() && !Mob->IsStone() && !Mob->IsMonster())
	{
		sys_err("Cannot mob or boss");
		return;
	}

	if (!mapIndex)
	{
		sys_err("Cannot mapIndex");
		return;
	}

	LPZODIAC pZodiac = FindByMapIndex(mapIndex);
	if (!pZodiac)
	{
		sys_err("cannot create zodiac %d", mapIndex);
		return;
	}

	if (pZodiac->zodiac_floor_event_data || pZodiac->zodiac_exit_temple_event_data)
	{
		sys_err("Bug Fixed");
		return;
	}

	BYTE Floor = pZodiac->GetFloor();

	if (Floor >= 40)
	{
		sys_err("Now Floor 40");
		return;
	}

	if (Floor != 7 && 
		Floor != 8 && 
		Floor != 11 && 
		Floor != 13 && 
		Floor != 14 && 
		Floor != 15 && 
		Floor != 17 && 
		Floor != 18 && 
		Floor != 21 && 
		Floor != 22 && 
		Floor != 27 && 
		Floor != 28 && 
		Floor != 29 && 
		Floor != 30 && 
		Floor != 34 && 
		Floor != 35 && 
		Floor != 36 && 
		Floor != 37 && 
		Floor != 38 && 
		Floor != 39
		)
		return;

	int Boss = pZodiac->CountIsBoss()-1;
	int Stone = pZodiac->CountIsStone()-1;
	int Mobb = 0;

	if (Floor == 11 || Floor == 22)
	{
		if (Mob->IsZodiacBoss())
		{
			Boss = --pZodiac->m_iBossKill;
			if (Boss > 0)
			{
				char szText[32+1];
				snprintf(szText, sizeof(szText), "(%d/%d)", pZodiac->m_iTotalMonster-Boss, pZodiac->m_iTotalMonster);
				pZodiac->ZodiacFloorSubMessage(szText);
				return;
			}
		}
		else
			return;
	}
	else if (Floor == 17)
	{
		if (!Mob->IsZodiacBoss())
		{
			return;
		}
	}
	else if (Floor == 8 || Floor == 13 || Floor == 14 || Floor == 18 || Floor == 27 || Floor == 28 || Floor == 29 || Floor == 30)
	{
		if (Mob->IsStone())
		{
			Stone = --pZodiac->m_iStoneKill;
			if (Stone > 0)
			{
				char szText[32+1];
				if (Floor == 13 || Floor == 14 || Floor == 28 || Floor == 29 || Floor == 30)
				{
					snprintf(szText, sizeof(szText), "(%d/%d)", pZodiac->m_iTotalMonster-Stone, pZodiac->m_iTotalMonster);
					pZodiac->ZodiacFloorSubMessage(szText);
				}
				else
				{
					snprintf(szText, sizeof(szText), "(%d)", Stone);
					pZodiac->ZodiacFloorSubMessage(szText);
				}

				return;
			}
		}
		else
			return;
	}
	else if (Floor == 15)
	{
		Mobb = --pZodiac->m_iMobKill;
		if (Mobb > 0)
		{
			char szText[32+1];
			snprintf(szText, sizeof(szText), "(%d)", Mobb);
			pZodiac->ZodiacFloorSubMessage(szText);
			return;
		}
	}
	else if (Floor == 34)
	{
		if (!Mob->IsStone() && !Mob->IsZodiacBoss())
		{
			Mobb = --pZodiac->m_iMobKill;
			if (Mobb > 0)
			{
				char szText[32+1];
				snprintf(szText, sizeof(szText), "(%d)", Mobb);
				pZodiac->ZodiacFloorSubMessage(szText);
				return;
			}
			else
			{
				if (pZodiac->GetFlag("34boss") == 0)
				{
					pZodiac->SetFlag("34boss", 1);

					pZodiac->KillAll();

					pZodiac->SpawnBoss(2);

					pZodiac->ZodiacFloorMessage(17);
					pZodiac->ZodiacFloorSubMessage("(1)");
				}

				return;
			}
		}
		else if (Mob->IsZodiacBoss())
		{
			Boss = --pZodiac->m_iBossKill;
			if (Boss > 0)
			{
				return;
			}
			else
			{
				pZodiac->GiveGiftBox();
			}
		}
		else
			return;
	}
	else if (Floor == 7 || Floor == 21)
	{
		if (Mob->IsStone() && pZodiac->GetFlag("bonus") == 0)
		{
			pZodiac->SetFlag("bonus", 1);

			pZodiac->KillAll();

			for (BYTE b = 0; b < 2; ++b)
				pZodiac->SpawnStone();

			return;
		}
	}
	else if (Floor >= 35 && Floor < 40)
	{
		if (number(1, 7) == 4)
		{
			pZodiac->KillAll();

			if (pZodiac->zodiac_remaining_time_temple_floor)
			{
				event_cancel(&pZodiac->zodiac_remaining_time_temple_floor);
				pZodiac->zodiac_remaining_time_temple_floor = nullptr;
			}

			pZodiac->ZodiacMessageMisyon(40);
			pZodiac->ZodiacTime(Floor-1, 40-(Floor-1), 10);

			pZodiac->m_bNextFloor = 40;
			pZodiac->m_bNextFloorControl = true;
			pZodiac->NextFloorButton();
			return;
		}
	}

	pZodiac->KillAll();

	if (pZodiac->zodiac_remaining_time_temple_floor)
	{
		event_cancel(&pZodiac->zodiac_remaining_time_temple_floor);
		pZodiac->zodiac_remaining_time_temple_floor = nullptr;
	}

	if (Floor == 7 || Floor == 21)
	{
		if (number(0, 50) == 15)
		{
			pZodiac->ZodiacMessageMisyon(Floor);
		}
		else
		{
			pZodiac->ZodiacFloorMessage(42);
		}
	}

	Floor += 1;

	if (Floor == 40)
	{
		pZodiac->ZodiacFloorMessage(41);
		pZodiac->ZodiacTime(Floor, 0, 10);

		Exit_temple_event_data * info = AllocEventInfo<Exit_temple_event_data>();
		info->seconds = 10;
		info->mapIndex = pZodiac->GetMapIndex();
		pZodiac->zodiac_exit_temple_event_data = event_create(Exit_temple_event_data_start, info, 1);
		return;
	}

	pZodiac->m_bNextFloor = Floor;
	pZodiac->m_bNextFloorControl = true;

	if (Floor != 8 && Floor != 22)
		pZodiac->ZodiacMessageMisyon(Floor);

	pZodiac->ZodiacTime(Floor-1, 1, 10);
	pZodiac->NextFloorButton();
}

void CZodiacManager::DeadPC(DWORD mapIndex)
{
	if (!mapIndex)
	{
		sys_err("Cannot mapIndex");
		return;
	}

	LPZODIAC pZodiac = FindByMapIndex(mapIndex);
	if (!pZodiac)
	{
		sys_err("cannot create zodiac %d", mapIndex);
		return;
	}

	BYTE Floor = pZodiac->GetFloor();

	if (Floor == 6 || Floor == 12 || Floor == 19 || Floor == 24)
	{
		if (pZodiac->zodiac_exit_temple_event_data)
		{
			sys_err("Bug fixed");
			return;
		}

		pZodiac->KillAll();

		pZodiac->ZodiacFloorMessage(41);
		pZodiac->ZodiacTime(Floor, 1, 10);

		Exit_temple_event_data * info = AllocEventInfo<Exit_temple_event_data>();
		info->seconds = 10;
		info->mapIndex = pZodiac->GetMapIndex();
		pZodiac->zodiac_exit_temple_event_data = event_create(Exit_temple_event_data_start, info, 1);
	}
}
