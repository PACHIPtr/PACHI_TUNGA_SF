#ifndef __HEADER_VNUM_HELPER__
#define	__HEADER_VNUM_HELPER__

#include "service.h"

class CItemVnumHelper
{
public:
	static bool	IsPhoenix(DWORD vnum) { return 53001 == vnum; }
	static bool	IsRamadanMoonRing(DWORD vnum) { return 71135 == vnum; }
	static bool	IsHalloweenCandy(DWORD vnum) { return 71136 == vnum; }
	static bool	IsHappinessRing(DWORD vnum) { return 71143 == vnum; }
	static bool	Is_Ruzgar_Ayakkabi(DWORD vnum) { return 72701 == vnum; }
	static bool	IsLovePendant(DWORD vnum) { return 71145 == vnum; }
};

class CMobVnumHelper
{
public:
	static	bool	IsPhoenix(DWORD vnum) { return 34001 == vnum; }
	static	bool	IsIcePhoenix(DWORD vnum) { return 34003 == vnum; }
	static	bool	IsPetUsingPetSystem(DWORD vnum) { return (IsPhoenix(vnum) || IsReindeerYoung(vnum)) || IsIcePhoenix(vnum); }
	static	bool	IsReindeerYoung(DWORD vnum) { return 34002 == vnum; }
	static	bool	IsRamadanBlackHorse(DWORD vnum) { return 20119 == vnum || 20219 == vnum || 22022 == vnum; }
#ifdef ENABLE_EVENT_SYSTEM
	static	bool	IsShadowWarriorMob(DWORD vnum) { return 6420 == vnum; }
	static	bool	IsBaashidoWarriorMob(DWORD vnum) { return 6421 == vnum; }
	static	bool	IsBossHuntersMob(DWORD vnum) { return 6415 == vnum || 6416 == vnum || 6417 == vnum || 6419 == vnum; }
#endif
	static	bool	IsEventBeranSetaou(DWORD vnum) { return 6415 == vnum; }
	static	bool	IsEventNemere(DWORD vnum) { return 6416 == vnum; }
	static	bool	IsEventRazador(DWORD vnum) { return 6417 == vnum; }
	static	bool	IsEventJotunThrym(DWORD vnum) { return 6418 == vnum; }
	static	bool	IsEventRedDragon(DWORD vnum) { return 6419 == vnum; }
};

class CVnumHelper
{
};

#endif	//__HEADER_VNUM_HELPER__
