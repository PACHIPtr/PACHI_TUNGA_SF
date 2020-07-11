#include "../../common/service.h"
#include "dungeon.h"
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
#include "zodiac_temple.h"
#endif

enum ERegenModes
{
	MODE_TYPE = 0,
	MODE_SX = 1,
	MODE_SY = 2,
	MODE_EX = 3,
	MODE_EY = 4,
	MODE_Z_SECTION = 5,
	MODE_DIRECTION = 6,
	MODE_REGEN_TIME = 7,
	MODE_REGEN_PERCENT = 8,
	MODE_MAX_COUNT = 9,
	MODE_VNUM = 10,
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	MODE_LEVEL = 11,
#endif
};

enum ERegenTypes
{
	REGEN_TYPE_MOB = 0,
	REGEN_TYPE_GROUP = 1,
	REGEN_TYPE_EXCEPTION = 2,
	REGEN_TYPE_GROUP_GROUP = 3,
	REGEN_TYPE_ANYWHERE = 4,
};

typedef struct regen
{
	LPREGEN	prev, next;
	long	lMapIndex;
	int		type;
	int		sx, sy, ex, ey;
	BYTE	z_section;

	BYTE	direction;

	DWORD	time;

	int		max_count;
	int		count;
	int 	vnum;

	bool	is_aggressive;

	LPEVENT	event;

	size_t id; // to help dungeon regen identification
	
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
	BYTE level;
#endif

	regen() :
		prev(nullptr), next(nullptr),
		lMapIndex(0),
		type(0),
		sx(0), sy(0), ex(0), ey(0),
		z_section(0),
		direction(0),
		time(0),
		max_count(0),
		count(0),
		vnum(0),
		is_aggressive(0),
		event(nullptr),
		id(0)
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		, level(0)
#endif
	{}
} REGEN;

EVENTINFO(regen_event_info)
{
	LPREGEN 	regen;

	regen_event_info()
		: regen(0)
	{
	}
};

typedef regen_event_info REGEN_EVENT_INFO;

typedef struct regen_exception
{
	LPREGEN_EXCEPTION prev, next;

	int		sx, sy, ex, ey;
	BYTE	z_section;
} REGEN_EXCEPTION;

class CDungeon;

EVENTINFO(dungeon_regen_event_info)
{
	LPREGEN 	regen;
	CDungeon::IdType dungeon_id;

	dungeon_regen_event_info()
		: regen(0)
		, dungeon_id(0)
	{
	}
};

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
class CZodiac;

EVENTINFO(zodiac_regen_event_info)
{
	LPREGEN 	regen;
	CZodiac::IdType zodiac_id;

	zodiac_regen_event_info() 
	: regen( 0 )
	, zodiac_id( 0 )
	{
	}
};

extern bool	regen_zodiac(const char* filename, long lMapIndex, int base_x, int base_y, LPZODIAC pZodiac, bool bOnce = true );
#endif

extern bool	regen_load(const char* filename, long lMapIndex, int base_x, int base_y);
extern bool	regen_do(const char* filename, long lMapIndex, int base_x, int base_y, LPDUNGEON pDungeon, bool bOnce = true);
extern bool	regen_load_in_file(const char* filename, long lMapIndex, int base_x, int base_y);
extern void	regen_free();

extern bool	is_regen_exception(long x, long y);
extern void	regen_reset(int x, int y);
