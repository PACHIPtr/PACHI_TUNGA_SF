#ifndef __INC_METIN_II_GAME_SCANNER_H__
#define __INC_METIN_II_GAME_SCANNER_H__

EVENTINFO(ScannerInfo)
{
	// <Factor> Removed unsafe copy of CHARACTER* here
	//LPCHARACTER pkChr;

	int         iID;
	int         dwVID;

	void		(*found) (DWORD, DWORD); //Callback!

	ScannerInfo() :
		iID(0),
		dwVID(0),
		found(nullptr) {
	}
};

class CNearbyScanner : public singleton<CNearbyScanner>
{
public:
	CNearbyScanner();
	virtual ~CNearbyScanner();

	void BeginScanner(DWORD dwVID, void(*found) (DWORD, DWORD));
	void EndScanner(DWORD dwVID);

	void Die(DWORD dwVID);
	ScannerInfo* GetScannerInfo(DWORD dwVID);

	LPEVENT GetScannerEvent(DWORD dwVID);

protected:
	// <vid, event>
	std::map<DWORD, std::list<LPEVENT> > m_map_kListEvent;

	//Internal id counter
	DWORD m_iID;
};

#endif
