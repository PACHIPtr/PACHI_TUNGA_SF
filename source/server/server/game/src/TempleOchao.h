#ifndef __TEMPLE_OCHAO_H
#define __TEMPLE_OCHAO_H
#define TEMPLE_OCHAO_MAP_INDEX 354
#define TEMPLE_OCHAO_ROOMS 11
#define TEMPLE_OCHAO_GUARDIAN 6400
#define TEMPLE_OCHAO_PORTAL 20415
#define TEMPLE_OCHAO_PORTAL_SHOW 60
#define TEMPLE_OCHAO_CHECK_ACTIVITY 30
#define TEMPLE_OCHAO_NO_ACTIVITY 180
#define TEMPLE_OCHAO_ATTACKED 600

namespace TempleOchao
{
	class CMgr : public singleton<CMgr>
	{
	public:
		void	Initialize();
		void	Destroy();
		void	Prepare();
		void	Spawn();
		void	OnGuardianKilled(int iX, int iY, int iZ);
		void	ClearPrepare();
		void	ChangeRoom();
		void	SetRoom(int iRoom) { i_room = iRoom; }
		int		GetRoom() { return i_room; }
		void	RandomRoom(int& iRoom, int& iX, int& iY, int& iZ);
		void	SetGuardianVID(DWORD dwGuardianVID) { dw_GuardianVID = dwGuardianVID; }
		DWORD	GetGuardianVID() { return dw_GuardianVID; }
		void	SetPortalVID(DWORD dwPortalVID) { dw_PortalVID = dwPortalVID; }
		DWORD	GetPortalVID() { return dw_PortalVID; }
		void	GuardianAttacked();
	private:
		int		i_room;
		DWORD	dw_GuardianVID, dw_PortalVID;
		bool	b_OnGuardianKilled;
	protected:
		LPEVENT	s_pkGuardianKilled, s_pkGuardianActivity;
	};
}
#endif
