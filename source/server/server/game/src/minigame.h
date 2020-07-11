#ifndef __INC_METIN_II_MINI_GAME_H__
#define __INC_METIN_II_MINI_GAME_H__

#include "../../common/stl.h"
#include "../../common/length.h"
#include "../../common/tables.h"
#include "packet.h"
#include "questmanager.h"
#include "questlua.h"

class CMiniGame : public singleton<CMiniGame>
{
public:
	CMiniGame();
	virtual ~CMiniGame();

	bool Initialize();
	void Destroy();

	void InitializeAttendance(int iEnable);
	void InitializeEasterEvent(int iEnable);

	void SpawnEventNPC(DWORD dwVnum);
	void StartAttendanceRegen();
	void StartEasterStoneRegen();

	void AttendanceMonsterAttack(LPCHARACTER pkAttacker, LPCHARACTER pkVictim);
	void AttendanceEventInfo(LPCHARACTER pChar);
	void AttendanceEventRequestReward(LPCHARACTER pChar);
	bool ReadRewardItemFile(const char* c_pszFileName);

	/****/
	void ShadowWarriorMonsterAttack(LPCHARACTER pkAttacker, LPCHARACTER pkVictim);
#ifdef ENABLE_MINI_GAME_CATCH_KING
public:
	int MiniGameCatchKing(LPCHARACTER ch, const char* data, size_t uiBytes);

	void MiniGameCatchKingEventInfo(LPCHARACTER pkChar);
	void InitializeMiniGameCatchKing(int iEnable);
	void InitializeMiniGameCatchKingEndTime(int iEndTime) { iRumiEndTime = iEndTime; }
	void MiniGameCatchKingCheckEnd();

	void MiniGameCatchKingStartGame(LPCHARACTER pkChar, BYTE bSetCount);
	void MiniGameCatchKingDeckCardClick(LPCHARACTER pkChar);
	void MiniGameCatchKingFieldCardClick(LPCHARACTER pkChar, BYTE bFieldPos);
	void MiniGameCatchKingGetReward(LPCHARACTER pkChar);

	void MiniGameCatchKingRegisterScore(LPCHARACTER pkChar, DWORD dwScore);
	int MiniGameCatchKingGetScore(lua_State* L, bool isTotal);
	int MiniGameCatchKingGetMyScore(LPCHARACTER pkChar);
protected:
	int iCatchKingEndTime;
	int iRumiEndTime;
#endif
private:
	std::vector<TRewardItem>	attendanceRewardVec;
};

#endif
