#pragma once

enum
{
	EVENT_MOON_DROP							= 0,
	EVENT_HEXAGON_DROP 						= 1,
	EVENT_CARVING_STONE_DROP				= 2,
	EVENT_RAMADAN							= 3,
	EVENT_XMAS								= 4,
	EVENT_EASTER							= 5,
	EVENT_FOOTBALL_DROP						= 6,
	EVENT_VALENTINE_DROP					= 7,
	EVENT_PET_DROP							= 8,
	EVENT_KIDS_DAY_DROP						= 9,
	EVENT_HALLOWEEN							= 10,
	EVENT_OLYMPIC_BADGE						= 11,
	EVENT_OKEY_CARD							= 12,
	EVENT_MOUNT_DROP						= 13,
	EVENT_ALIGNMENT							= 14,
	EVENT_YEAR_ROUND						= 15,
	EVENT_BOSS_HUNTERS						= 16,
	EVENT_NEW_FISH							= 17,
	EVENT_MINING							= 18,
	EVENT_HARVEST_FESTIVAL					= 19,
	EVENT_MAKE_WONSO						= 20,
	EVENT_CATCH_KING						= 21,
	EVENT_GOLDEN_RAIN						= 22,
	EVENT_SUPER_METIN						= 23,
	EVENT_LUCKY_BOX							= 24,
	EVENT_MAX_NUM							= 25,
};

class CPythonGameEvents : public CSingleton<CPythonGameEvents>
{	
	public:		
		CPythonGameEvents();
		virtual ~CPythonGameEvents();
		
		void	SetActivateEvent(bool isActivate, BYTE bEventID);
		void	SetEventTime(BYTE bEventID, DWORD event_time);
		bool	IsActivateEvent(BYTE bEventID);
		DWORD	GetEventTime(BYTE bEventID);
	protected:
		bool	m_pkActivateEvents[EVENT_MAX_NUM];
		DWORD	m_dwEventEndTime[EVENT_MAX_NUM];
	private:
};