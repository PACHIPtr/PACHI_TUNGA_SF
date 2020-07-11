#ifndef __CLIENT_VNUM_HELPER__
#define	__CLIENT_VNUM_HELPER__

class CItemVnumHelper
{
	public:
		static	const bool	IsSetaouSteelArmor(DWORD vnum) { return (vnum >= 12010 && vnum <= 12049) || (21080 <= vnum && vnum <= 21089); }
		static	const bool	IsJotunArmors(DWORD vnum) { return 20760 <= vnum && vnum <= 20959; }
		static	const bool	IsZodiacArmors(DWORD vnum) { return (19290 <= vnum && vnum <= 19899) || (21200 <= vnum && vnum <= 21209); }
		static	const bool	IsKyanitArmors(DWORD vnum) { return 12050 <= vnum && vnum <= 12099; }
		static	const bool	IsBlackDragonWeaponSword(DWORD vnum) { return 320 <= vnum && vnum <= 355; }
		static	const bool	IsBlackDragonWeaponBow(DWORD vnum)	{ return 2210 <= vnum && vnum <= 2225; }
		static	const bool	IsBlackDragonWeaponDagger(DWORD vnum) { return 1190 <= vnum && vnum <= 1205; }
		static	const bool	IsBlackDragonWeaponFan(DWORD vnum) { return 7310 <= vnum && vnum <= 7325; }
		static	const bool	IsBlackDragonWeaponBell(DWORD vnum) { return 5170 <= vnum && vnum <= 5185; }
		static	const bool	IsBlackDragonWeaponClaw(DWORD vnum) { return 6130 <= vnum && vnum <= 6145; }
		static	const bool	IsBlackDragonWeaponTwoHanded(DWORD vnum) { return 3230 <= vnum && vnum <= 3245; }
		static	const bool	IsBlackDragonArmor(DWORD vnum) { return 21210 <= vnum && vnum <= 21305; }
};

#endif