#include "stdafx.h"
#include "config.h"

// #include <random>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "../../common/length.h"
#include "../../common/tables.h"
#include "p2p.h"
#include "locale_service.h"
#include "char.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "questmanager.h"
#include "questlua.h"
#include "start_position.h"
#include "char_manager.h"
#include "item_manager.h"
#include "sectree_manager.h"
#include "regen.h"
#include "log.h"
#include "db.h"
#include "target.h"
#include "party.h"
#include "minigame.h"
#include <random>
#include <algorithm>

int CMiniGame::MiniGameCatchKing(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	TPacketCGMiniGameCatchKing* p = (TPacketCGMiniGameCatchKing*)data;

	if (uiBytes < sizeof(TPacketCGMiniGameCatchKing))
		return -1;

	const char* c_pData = data + sizeof(TPacketCGMiniGameCatchKing);
	uiBytes -= sizeof(TPacketCGMiniGameCatchKing);

	switch (p->bSubheader)
	{
	case 0:
		MiniGameCatchKingStartGame(ch, p->bSubArgument);
		return 0;

	case 1:
		MiniGameCatchKingDeckCardClick(ch);
		return 0;

	case 2:
		MiniGameCatchKingFieldCardClick(ch, p->bSubArgument);
		return 0;

	case 3:
		MiniGameCatchKingGetReward(ch);
		return 0;

	default:
		sys_err("CMiniGame::MiniGameCatchKing : Unknown subheader %d : %s", p->bSubheader, ch->GetName());
		break;
	}

	return 0;
}

struct MiniGameCatchKingPacketFunc
{
	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		CMiniGame::instance().MiniGameCatchKingEventInfo(d->GetCharacter());
	}
};

void CMiniGame::InitializeMiniGameCatchKing(int iEnable)
{
	if (iEnable)
	{
		SpawnEventNPC(20506);

		const DESC_MANAGER::DESC_SET& descSet = DESC_MANAGER::instance().GetClientSet();
		std::for_each(descSet.begin(), descSet.end(), MiniGameCatchKingPacketFunc());
	}
	else
	{
		CharacterVectorInteractor i;
		CHARACTER_MANAGER::instance().GetCharactersByRaceNum(20506, i);

		for (CharacterVectorInteractor::iterator it = i.begin(); it != i.end(); it++)
		{
			M2_DESTROY_CHARACTER(*it);
		}

		const DESC_MANAGER::DESC_SET& descSet = DESC_MANAGER::instance().GetClientSet();
		std::for_each(descSet.begin(), descSet.end(), MiniGameCatchKingPacketFunc());
	}
}

void CMiniGame::MiniGameCatchKingStartGame(LPCHARACTER pkChar, BYTE bSetCount)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	if (pkChar->MiniGameCatchKingGetGameStatus() == true)
		return;

	if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event") == 0)
		return;

	if (bSetCount < 1 || bSetCount > 5)
	{
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Numarul de seturi pariate este invalid, incearca din nou."));
		return;
	}

	if (pkChar->GetGold() < (30000 * bSetCount))
	{
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Nu ai suficient yang pentru a incepe acest joc."));
		return;
	}

	if (pkChar->CountSpecifyItem(79604) < bSetCount)
	{
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Nu ai suficiente seturi pentru a incepe acest joc."));
		return;
	}

	pkChar->RemoveSpecifyItem(79604, bSetCount);
	pkChar->PointChange(POINT_GOLD, -(30000 * bSetCount));

	std::vector<TCatchKingCard> m_vecFieldCards;

	std::srand(unsigned(std::time(0)));

	for (int i = 0; i < 25; i++)
	{
		if (i >= 0 && i < 7)
			m_vecFieldCards.push_back(TCatchKingCard(1, false));
		else if (i >= 7 && i < 11)
			m_vecFieldCards.push_back(TCatchKingCard(2, false));
		else if (i >= 11 && i < 16)
			m_vecFieldCards.push_back(TCatchKingCard(3, false));
		else if (i >= 16 && i < 21)
			m_vecFieldCards.push_back(TCatchKingCard(4, false));
		else if (i >= 21 && i < 24)
			m_vecFieldCards.push_back(TCatchKingCard(5, false));
		else if (i >= 24)
			m_vecFieldCards.push_back(TCatchKingCard(6, false)); // 6 = K
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(m_vecFieldCards.begin(), m_vecFieldCards.end(), g);

	pkChar->MiniGameCatchKingSetFieldCards(m_vecFieldCards);

	pkChar->MiniGameCatchKingSetBetNumber(bSetCount);
	pkChar->MiniGameCatchKingSetHandCardLeft(12);
	pkChar->MiniGameCatchKingSetGameStatus(true);

	DWORD dwBigScore = MiniGameCatchKingGetMyScore(pkChar);

	TPacketGCMiniGameCatchKing packet;
	packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet.bSubheader = SUBHEADER_GC_CATCH_KING_START;

	packet.wSize = sizeof(packet) + sizeof(dwBigScore);

	pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&dwBigScore, sizeof(DWORD));

	BYTE bCardInHand = pkChar->MiniGameCatchKingGetHandCardLeft();

	TPacketGCMiniGameCatchKing packet2;
	packet2.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet2.bSubheader = SUBHEADER_GC_CATCH_KING_SET_CARD;

	packet2.wSize = sizeof(packet) + sizeof(bCardInHand);

	pkChar->GetDesc()->BufferedPacket(&packet2, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&bCardInHand, sizeof(BYTE));
}

void CMiniGame::MiniGameCatchKingDeckCardClick(LPCHARACTER pkChar)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event") == 0)
		return;

	if (pkChar->MiniGameCatchKingGetGameStatus() == false)
		return;

	if (pkChar->MiniGameCatchKingGetHandCard())
		return;

	BYTE bCardLeft = pkChar->MiniGameCatchKingGetHandCardLeft();

	if (bCardLeft)
	{
		if (bCardLeft <= 12 && bCardLeft > 7)
			pkChar->MiniGameCatchKingSetHandCard(1);
		else if (bCardLeft <= 7 && bCardLeft > 5)
			pkChar->MiniGameCatchKingSetHandCard(2);
		else if (bCardLeft <= 5 && bCardLeft > 3)
			pkChar->MiniGameCatchKingSetHandCard(3);
		else if (bCardLeft == 3)
			pkChar->MiniGameCatchKingSetHandCard(4);
		else if (bCardLeft == 2)
			pkChar->MiniGameCatchKingSetHandCard(5);
		else if (bCardLeft == 1)
			pkChar->MiniGameCatchKingSetHandCard(6);
	}
	else
		return;

	BYTE bCardInHand = pkChar->MiniGameCatchKingGetHandCard();

	if (!bCardInHand)
		return;

	pkChar->MiniGameCatchKingSetHandCardLeft(bCardLeft - 1);

	TPacketGCMiniGameCatchKing packet;
	packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet.bSubheader = SUBHEADER_GC_CATCH_KING_SET_CARD;

	packet.wSize = sizeof(packet) + sizeof(bCardInHand);

	pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&bCardInHand, sizeof(BYTE));
}

void CMiniGame::MiniGameCatchKingFieldCardClick(LPCHARACTER pkChar, BYTE bFieldPos)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event") == 0)
		return;

	if (pkChar->MiniGameCatchKingGetGameStatus() == false)
		return;

	if (bFieldPos < 0 or bFieldPos > 24)
		return;

	BYTE bHandCard = pkChar->MiniGameCatchKingGetHandCard();
	TCatchKingCard filedCard = pkChar->m_vecCatchKingFieldCards[bFieldPos];

	if (!bHandCard)
		return;

	if (filedCard.bIsExposed == true)
		return;

	DWORD dwPoints = 0;
	bool bDestroyCard = false;
	bool bKeepFieldCard = false;
	bool bGetReward = false;

	if (bHandCard < 5)
	{
		if (bHandCard < filedCard.bIndex)
		{
			dwPoints = 0;
			bDestroyCard = true;
			bKeepFieldCard = false;
		}
		else if (bHandCard == filedCard.bIndex)
		{
			dwPoints = filedCard.bIndex * 10;
			bDestroyCard = true;
			bKeepFieldCard = true;
		}
		else
		{
			dwPoints = filedCard.bIndex * 10;
			bDestroyCard = false;
			bKeepFieldCard = true;
		}
	}

	int checkPos[8];
	checkPos[0] = bFieldPos - 5;
	checkPos[1] = bFieldPos + 5;
	checkPos[2] = (bFieldPos % 10 == 4 || bFieldPos % 10 == 9) ? -1 : (bFieldPos + 1);
	checkPos[3] = (bFieldPos % 10 == 0 || bFieldPos % 10 == 5) ? -1 : (bFieldPos - 1);
	checkPos[4] = (bFieldPos % 10 == 4 || bFieldPos % 10 == 9) ? -1 : (bFieldPos - 5 + 1);
	checkPos[5] = (bFieldPos % 10 == 4 || bFieldPos % 10 == 9) ? -1 : (bFieldPos + 5 + 1);
	checkPos[6] = (bFieldPos % 10 == 0 || bFieldPos % 10 == 5) ? -1 : (bFieldPos - 5 - 1);
	checkPos[7] = (bFieldPos % 10 == 0 || bFieldPos % 10 == 5) ? -1 : (bFieldPos + 5 - 1);

	bool isFiveNearby = false;

	for (int i = 0; i < 25; i++)
	{
		if (isFiveNearby)
			break;

		for (int j = 0; j < sizeof(checkPos) / sizeof(checkPos[0]); j++)
		{
			if (checkPos[j] < 0 || checkPos[j] >= 25)
				continue;

			if (i == checkPos[j] && pkChar->m_vecCatchKingFieldCards[i].bIndex == 5)
			{
				isFiveNearby = true;
				break;
			}
		}
	}

	if (bHandCard == 5)
	{
		if (isFiveNearby)
		{
			dwPoints = 0;
			bDestroyCard = true;
			bKeepFieldCard = (bHandCard >= filedCard.bIndex) ? true : false;
		}
		else
		{
			dwPoints = (bHandCard >= filedCard.bIndex) ? filedCard.bIndex * 10 : 0;
			bDestroyCard = (bHandCard > filedCard.bIndex) ? false : true;
			bKeepFieldCard = (bHandCard >= filedCard.bIndex) ? true : false;
		}
	}

	if (bHandCard == 6)
	{
		dwPoints = (bHandCard == filedCard.bIndex) ? 100 : 0;
		bDestroyCard = true;
		bKeepFieldCard = (bHandCard == filedCard.bIndex) ? true : false;
	}

	if (bKeepFieldCard)
	{
		pkChar->m_vecCatchKingFieldCards[bFieldPos].bIsExposed = true;
	}

	int checkRowPos[4];
	checkRowPos[0] = 5 * (bFieldPos / 5);
	checkRowPos[1] = 4 + (5 * (bFieldPos / 5));
	checkRowPos[2] = bFieldPos - (5 * (bFieldPos / 5));
	checkRowPos[3] = bFieldPos + 20 - (5 * (bFieldPos / 5));

	bool isHorizontalRow = true;
	bool isVerticalRow = true;

	for (int row = checkRowPos[0]; row <= checkRowPos[1]; row += 1)
	{
		if (!pkChar->m_vecCatchKingFieldCards[row].bIsExposed)
		{
			isHorizontalRow = false;
			break;
		}
	}

	for (int col = checkRowPos[2]; col <= checkRowPos[3]; col += 5)
	{
		if (!pkChar->m_vecCatchKingFieldCards[col].bIsExposed)
		{
			isVerticalRow = false;
			break;
		}
	}

	dwPoints += isHorizontalRow ? 10 : 0;
	dwPoints += isVerticalRow ? 10 : 0;

	if (dwPoints)
	{
		pkChar->MiniGameCatchKingSetScore(pkChar->MiniGameCatchKingGetScore() + dwPoints);
	}

	bool isTheEnd = false;

	if (bDestroyCard)
	{
		bGetReward = (bHandCard == 6 && pkChar->MiniGameCatchKingGetScore() >= 10) ? true : false;
		isTheEnd = (bHandCard == 6) ? true : false;
		pkChar->MiniGameCatchKingSetHandCard(0);
	}

	BYTE bRowType = 0;
	if (isHorizontalRow && !isVerticalRow)
	{
		bRowType = 1;
	}
	else if (!isHorizontalRow && isVerticalRow)
	{
		bRowType = 2;
	}
	else if (isHorizontalRow && isVerticalRow)
	{
		bRowType = 3;
	}

	TPacketGCMiniGameCatchKing packet;
	packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet.bSubheader = SUBHEADER_GC_CATCH_KING_RESULT_FIELD;

	TPacketGCMiniGameCatchKingResult packetSecond;
	packetSecond.dwPoints = pkChar->MiniGameCatchKingGetScore();
	packetSecond.bRowType = bRowType;
	packetSecond.bCardPos = bFieldPos;
	packetSecond.bCardValue = filedCard.bIndex;
	packetSecond.bKeepFieldCard = bKeepFieldCard;
	packetSecond.bDestroyHandCard = bDestroyCard;
	packetSecond.bGetReward = bGetReward;
	packetSecond.bIsFiveNearBy = isFiveNearby;

	packet.wSize = sizeof(packet) + sizeof(packetSecond);

	pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&packetSecond, sizeof(TPacketGCMiniGameCatchKingResult));

	if (isTheEnd)
	{
		for (int i = 0; i < 25; i++)
		{
			if (!pkChar->m_vecCatchKingFieldCards[i].bIsExposed)
			{
				TPacketGCMiniGameCatchKing packet;
				packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
				packet.bSubheader = SUBHEADER_GC_CATCH_KING_SET_END_CARD;

				TPacketGCMiniGameCatchKingSetEndCard packetSecond;
				packetSecond.bCardPos = i;
				packetSecond.bCardValue = pkChar->m_vecCatchKingFieldCards[i].bIndex;

				packet.wSize = sizeof(packet) + sizeof(packetSecond);

				pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
				pkChar->GetDesc()->Packet(&packetSecond, sizeof(TPacketGCMiniGameCatchKingSetEndCard));
			}
		}
	}
}

void CMiniGame::MiniGameCatchKingGetReward(LPCHARACTER pkChar)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	if (quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event") == 0)
		return;

	if (pkChar->MiniGameCatchKingGetGameStatus() == false)
		return;

	if (pkChar->MiniGameCatchKingGetHandCard())
		return;

	if (pkChar->MiniGameCatchKingGetHandCardLeft())
		return;

	DWORD dwRewardVnum = 0;
	BYTE bReturnCode = 0;
	DWORD dwScore = pkChar->MiniGameCatchKingGetScore();

	if (dwScore >= 10 && dwScore < 400)
		dwRewardVnum = 50930;
	else if (dwScore >= 400 && dwScore < 550)
		dwRewardVnum = 50929;
	else if (dwScore >= 550)
		dwRewardVnum = 50928;

	MiniGameCatchKingRegisterScore(pkChar, dwScore);

	if (dwRewardVnum)
	{
		pkChar->MiniGameCatchKingSetScore(0);
		pkChar->AutoGiveItem(dwRewardVnum, pkChar->MiniGameCatchKingGetBetNumber());
		pkChar->MiniGameCatchKingSetBetNumber(0);
		pkChar->MiniGameCatchKingSetGameStatus(false);
		pkChar->m_vecCatchKingFieldCards.clear();

		bReturnCode = 0;
	}
	else
	{
		bReturnCode = 1;
	}

	TPacketGCMiniGameCatchKing packet;
	packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet.bSubheader = SUBHEADER_GC_CATCH_KING_REWARD;

	packet.wSize = sizeof(packet) + sizeof(bReturnCode);

	pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&bReturnCode, sizeof(BYTE));
}

void CMiniGame::MiniGameCatchKingRegisterScore(LPCHARACTER pkChar, DWORD dwScore)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	char querySelect[256];

	snprintf(querySelect, sizeof(querySelect),
		"SELECT max_score FROM log.log_catck_king_event WHERE name = '%s' LIMIT 1;", pkChar->GetName());

	std::unique_ptr<SQLMsg> pSelectMsg(DBManager::instance().DirectQuery(querySelect));
	SQLResult * resSelect = pSelectMsg->Get();

	if (resSelect && resSelect->uiNumRows > 0)
	{
		DWORD maxScore;
		MYSQL_ROW row = mysql_fetch_row(resSelect->pSQLResult);
		str_to_number(maxScore, row[0]);

		if (dwScore > maxScore)
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE log.log_catck_king_event SET max_score = %d, total_score = total_score + %d WHERE name = '%s';", dwScore, dwScore, pkChar->GetName()));
		else
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE log.log_catck_king_event SET total_score = total_score + %d WHERE name = '%s';", dwScore, pkChar->GetName()));
	}
	else
	{
		DBManager::instance().DirectQuery("REPLACE INTO log.log_catck_king_event (name, empire, max_score, total_score) VALUES ('%s', %d, %d, %d);",
			pkChar->GetName(), pkChar->GetEmpire(), dwScore, dwScore);
	}
}

int CMiniGame::MiniGameCatchKingGetScore(lua_State * L, bool isTotal)
{
	DWORD index = 1;
	lua_newtable(L);

	char querySelect[256];

	if (isTotal)
	{
		snprintf(querySelect, sizeof(querySelect),
			"SELECT name, empire, total_score FROM log.log_catck_king_event ORDER BY total_score DESC LIMIT 10;");
	}
	else
	{
		snprintf(querySelect, sizeof(querySelect),
			"SELECT name, empire, max_score FROM log.log_catck_king_event ORDER BY max_score DESC LIMIT 10;");
	}

	std::unique_ptr<SQLMsg> pSelectMsg(DBManager::instance().DirectQuery(querySelect));
	SQLResult* resSelect = pSelectMsg->Get();

	if (resSelect && resSelect->uiNumRows > 0)
	{
		for (uint i = 0; i < resSelect->uiNumRows; i++)
		{
			MYSQL_ROW row = mysql_fetch_row(resSelect->pSQLResult);
			BYTE bEmpire;
			DWORD dwScore;

			str_to_number(bEmpire, row[1]);
			str_to_number(dwScore, row[2]);

			lua_newtable(L);

			lua_pushstring(L, row[0]);
			lua_rawseti(L, -2, 1);

			lua_pushnumber(L, bEmpire);
			lua_rawseti(L, -2, 2);

			lua_pushnumber(L, dwScore);
			lua_rawseti(L, -2, 3);

			lua_rawseti(L, -2, index++);
		}
	}

	return 0;
}

int CMiniGame::MiniGameCatchKingGetMyScore(LPCHARACTER pkChar)
{
	if (pkChar == nullptr)
		return 0;

	if (!pkChar->GetDesc())
		return 0;

	char querySelect[256];

	snprintf(querySelect, sizeof(querySelect), "SELECT max_score FROM log.log_catck_king_event WHERE name = '%s' LIMIT 1;", pkChar->GetName());

	std::unique_ptr<SQLMsg> pSelectMsg(DBManager::instance().DirectQuery(querySelect));
	SQLResult * resSelect = pSelectMsg->Get();

	if (resSelect && resSelect->uiNumRows > 0)
	{
		DWORD dwScore = 0;
		MYSQL_ROW row = mysql_fetch_row(resSelect->pSQLResult);

		str_to_number(dwScore, row[0]);

		return dwScore;
	}

	return 0;
}

void CMiniGame::MiniGameCatchKingEventInfo(LPCHARACTER pkChar)
{
	if (pkChar == nullptr)
		return;

	if (!pkChar->GetDesc())
		return;

	bool bIsEnable = quest::CQuestManager::instance().GetEventFlag("enable_catch_king_event");

	TPacketGCMiniGameCatchKing packet;
	packet.bHeader = HEADER_GC_MINI_GAME_CATCH_KING;
	packet.bSubheader = SUBHEADER_GC_CATCH_KING_EVENT_INFO;

	packet.wSize = sizeof(packet) + sizeof(bIsEnable);

	pkChar->GetDesc()->BufferedPacket(&packet, sizeof(TPacketGCMiniGameCatchKing));
	pkChar->GetDesc()->Packet(&bIsEnable, sizeof(bool));
}

void CMiniGame::MiniGameCatchKingCheckEnd()
{
	if (time(0) > iCatchKingEndTime && iCatchKingEndTime != 0)
	{
		quest::CQuestManager::instance().RequestSetEventFlag("enable_catch_king_event", 0);
		iCatchKingEndTime = 0;
	}
}