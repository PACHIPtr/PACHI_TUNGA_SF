#include "StdAfx.h"
#include "../eterLib/NetPacketHeaderMap.h"

#include "PythonNetworkStream.h"
#include "Packet.h"
#include "NetworkActorManager.h"

#include "GuildMarkDownloader.h"
#include "GuildMarkUploader.h"
#include "MarkManager.h"


// MARK_BUG_FIX
static DWORD gs_nextDownloadMarkTime = 0;
// END_OF_MARK_BUG_FIX

// Packet ---------------------------------------------------------------------------
class CMainPacketHeaderMap : public CNetworkPacketHeaderMap
{
	public:
		CMainPacketHeaderMap()
		{
			Set(HEADER_GC_EMPIRE,				CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCEmpire), false));
			Set(HEADER_GC_WARP,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCWarp), false));
			Set(HEADER_GC_QUEST_INFO,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCQuestInfo), true));
			Set(HEADER_GC_REQUEST_MAKE_GUILD,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBlank), false));
			Set(HEADER_GC_PVP,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPVP), false));
			Set(HEADER_GC_DUEL_START,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDuelStart), true));
			Set(HEADER_GC_CHARACTER_ADD,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterAdd), false));
			Set(HEADER_GC_CHAR_ADDITIONAL_INFO,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterAdditionalInfo), false));
			Set(HEADER_GC_CHARACTER_ADD2,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterAdd2), false));
			Set(HEADER_GC_CHARACTER_UPDATE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterUpdate), false));
#ifdef ENABLE_AFFECT_PACKET_RENEWAL
			Set(HEADER_GC_AFFECT_FLAG_ADD,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketAddAffectFlag), false));
#endif
			Set(HEADER_GC_CHARACTER_UPDATE2,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterUpdate2), false));
			Set(HEADER_GC_CHARACTER_DEL,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCharacterDelete), false));
			Set(HEADER_GC_CHARACTER_MOVE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMove), false));
			Set(HEADER_GC_CHAT,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChat), true));

			Set(HEADER_GC_SYNC_POSITION,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCC2C), true));

			Set (HEADER_GC_LOGIN_SUCCESS,				CNetworkPacketHeaderMap::TPacketType (sizeof (TPacketGCLoginSuccess),			false));
			Set(HEADER_GC_LOGIN_FAILURE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLoginFailure), false));

			Set(HEADER_GC_PLAYER_CREATE_SUCCESS,		 CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPlayerCreateSuccess), false));
			Set(HEADER_GC_PLAYER_CREATE_FAILURE,		 CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCreateFailure), false));
			Set(HEADER_GC_PLAYER_DELETE_SUCCESS,		 CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBlank), false));
			Set(HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBlank), false));

			Set(HEADER_GC_STUN,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCStun), false));
			Set(HEADER_GC_DEAD,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDead), false));

			// SUPPORT_BGM
			Set(HEADER_GC_MAIN_CHARACTER2_EMPIRE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMainCharacter2_EMPIRE), false));
			Set(HEADER_GC_MAIN_CHARACTER3_BGM,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMainCharacter3_BGM), false));
			Set(HEADER_GC_MAIN_CHARACTER4_BGM_VOL,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMainCharacter4_BGM_VOL), false));
			// END_OFSUPPORT_BGM

			Set(HEADER_GC_PLAYER_POINTS,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPoints), false));
			Set(HEADER_GC_PLAYER_POINT_CHANGE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPointChange), false));

			Set(HEADER_GC_ITEM_SET,				CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemSet), false));
			Set(HEADER_GC_ITEM_SET2,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemSet2), false));
			Set(HEADER_GC_ITEM_USE,				CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemUse), false));
			Set(HEADER_GC_ITEM_UPDATE,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemUpdate), false));

			Set(HEADER_GC_ITEM_GROUND_ADD,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemGroundAdd), false));
			Set(HEADER_GC_ITEM_GROUND_DEL,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemGroundDel), false));
			Set(HEADER_GC_ITEM_OWNERSHIP,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemOwnership), false));

			Set(HEADER_GC_QUICKSLOT_ADD,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCQuickSlotAdd), false));
			Set(HEADER_GC_QUICKSLOT_DEL,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCQuickSlotDel), false));
			Set(HEADER_GC_QUICKSLOT_SWAP,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCQuickSlotSwap), false));

			Set(HEADER_GC_WHISPER,				CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCWhisper), false));

			Set(HEADER_GC_CHARACTER_POSITION,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPosition), false));
			Set(HEADER_GC_MOTION,				CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMotion), false));

			Set(HEADER_GC_SHOP,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShop), true));
			Set(HEADER_GC_SHOP_SIGN,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShopSign), false));
			Set(HEADER_GC_EXCHANGE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCExchange), false));

			Set(HEADER_GC_PING,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPing), false));

			Set(HEADER_GC_SCRIPT,			CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCScript), true));
			Set(HEADER_GC_QUEST_CONFIRM,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCQuestConfirm), false));

			Set(HEADER_GC_TARGET,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTarget), false));
#ifdef ENABLE_TARGET_INFORMATION_SYSTEM
			Set(HEADER_GC_TARGET_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTargetInfo), false));
#endif
			Set(HEADER_GC_MOUNT,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMount), false));

			Set(HEADER_GC_CHANGE_SPEED,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChangeSpeed), false));

			Set(HEADER_GC_HANDSHAKE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCHandshake), false));
			Set(HEADER_GC_HANDSHAKE_OK,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBlank), false));
			Set(HEADER_GC_BINDUDP,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBindUDP), false));
			Set(HEADER_GC_OWNERSHIP,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCOwnership), false));
			Set(HEADER_GC_CREATE_FLY,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCreateFly), false));
#ifdef _IMPROVED_PACKET_ENCRYPTION_
			Set(HEADER_GC_KEY_AGREEMENT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketKeyAgreement), false));
			Set(HEADER_GC_KEY_AGREEMENT_COMPLETED, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketKeyAgreementCompleted), false));
#endif
			Set(HEADER_GC_ADD_FLY_TARGETING, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCFlyTargeting), false));
			Set(HEADER_GC_FLY_TARGETING, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCFlyTargeting), false));

			Set(HEADER_GC_PHASE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPhase), false));
			Set(HEADER_GC_SKILL_LEVEL,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSkillLevel), false));
			Set(HEADER_GC_SKILL_LEVEL_NEW,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSkillLevelNew), false));

			Set(HEADER_GC_MESSENGER,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMessenger), true));
			Set(HEADER_GC_GUILD,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCGuild), true));

			Set(HEADER_GC_PARTY_INVITE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyInvite), false));
			Set(HEADER_GC_PARTY_ADD,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyAdd), false));
			Set(HEADER_GC_PARTY_UPDATE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyUpdate), false));
			Set(HEADER_GC_PARTY_REMOVE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyRemove), false));
			Set(HEADER_GC_PARTY_LINK,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyLink), false));
			Set(HEADER_GC_PARTY_UNLINK,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyUnlink), false));
			Set(HEADER_GC_PARTY_PARAMETER,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPartyParameter), false));

			Set(HEADER_GC_SAFEBOX_SET,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemSet), false));
			Set(HEADER_GC_SAFEBOX_DEL,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemDel), false));
			Set(HEADER_GC_SAFEBOX_WRONG_PASSWORD,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSafeboxWrongPassword), false));
			Set(HEADER_GC_SAFEBOX_SIZE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSafeboxSize), false));

			Set(HEADER_GC_FISHING,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCFishing), false));
			Set(HEADER_GC_DUNGEON, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDungeon), true));
			//Set(HEADER_GC_SLOW_TIMER, CNetworkPacketHeaderMap::TPacketType(sizeof(BYTE), false));
			Set(HEADER_GC_TIME, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTime), false));
			Set(HEADER_GC_WALK_MODE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCWalkMode), false));
			Set(HEADER_GC_CHANGE_SKILL_GROUP, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChangeSkillGroup), false));
			Set(HEADER_GC_REFINE_INFORMATION, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCRefineInformation), false));
			Set(HEADER_GC_SEPCIAL_EFFECT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSpecialEffect), false));
			Set(HEADER_GC_NPC_POSITION, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCNPCPosition), true));
			Set(HEADER_GC_CHANGE_NAME, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChangeName), false));
			Set(HEADER_GC_LOGIN_KEY, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLoginKey), false));

			Set(HEADER_GC_AUTH_SUCCESS, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAuthSuccess), false));
			Set(HEADER_GC_CHANNEL, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChannel), false));
#ifdef ENABLE_VIEW_EQUIPMENT_SYSTEM
			Set(HEADER_GC_VIEW_EQUIP, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCViewEquip), false));
#endif
			Set(HEADER_GC_LAND_LIST, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLandList), true));

			Set(HEADER_GC_TARGET_UPDATE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTargetUpdate), false));
			Set(HEADER_GC_TARGET_DELETE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTargetDelete), false));
			Set(HEADER_GC_TARGET_CREATE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTargetCreate), false));

			Set(HEADER_GC_AFFECT_ADD, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAffectAdd), false));
			Set(HEADER_GC_AFFECT_REMOVE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAffectRemove), false));

			Set(HEADER_GC_MALL_OPEN, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMallOpen), false));
			Set(HEADER_GC_MALL_SET, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemSet), false));
			Set(HEADER_GC_MALL_DEL, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCItemDel), false));

			Set(HEADER_GC_LOVER_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLoverInfo), false));
			Set(HEADER_GC_LOVE_POINT_UPDATE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCLovePointUpdate), false));

			Set(HEADER_GC_DIG_MOTION, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDigMotion), false));
			Set(HEADER_GC_DAMAGE_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDamageInfo), false));

			Set(HEADER_GC_SPECIFIC_EFFECT,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSpecificEffect), false));
			Set(HEADER_GC_DRAGON_SOUL_REFINE,		CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCDragonSoulRefine), false));
			
#ifdef ENABLE_CHANGELOOK_SYSTEM
			Set(HEADER_GC_CL, 								CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketChangeLook),						false));
#endif	

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
			Set(HEADER_GC_SHOPSEARCH_SET, 								CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCShopSearchItemSet),						false));
#endif

#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
			Set(HEADER_GC_EVENT_INFO, 						CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCEventInfo),					false));
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
			Set(HEADER_GC_MINI_GAME_CATCH_KING, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMiniGameCatchKing), true));
#endif


#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
			Set(HEADER_GC_RANKING_COMBAT_ZONE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCCombatZoneRanking), false));
			Set(HEADER_GC_SEND_COMBAT_ZONE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSendCombatZone), false));
#endif
#ifdef ENABLE_BUFFI_SYSTEM
			Set(HEADER_GC_SUPPORT_SHAMAN_SKILL,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSupportShamanSkill), false));
#endif
#ifdef ENABLE_ACCE_SYSTEM
			Set(HEADER_GC_SASH, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketSash), false));
#endif
#ifdef ENABLE_HEALTH_BOARD_SYSTEM
			Set(HEADER_GC_HEALTH_BOARD, 					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCHealthBoard), 				false));
#endif
#ifdef ENABLE_EXTEND_INVENTORY_SYSTEM	
			Set(HEADER_GC_EXTEND_INVENTORY,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCExtendInventory),			false));
#endif
#ifdef ENABLE_EXCHANGE_WINDOW_SYSTEM
			Set(HEADER_GC_EXCHANGE_INFO,					CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCExchageInfo),				false));
#endif
#ifdef ENABLE_SHOW_CHEST_DROP_SYSTEM
			Set(HEADER_GC_CHEST_DROP_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCChestDropInfo), true));
#endif
#ifdef ENABLE_FISH_JIGSAW_EVENT
			Set(HEADER_GC_FISH_EVENT_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCFishEventInfo), false));
#endif

#ifdef ENABLE_GEM_SYSTEM
			Set(HEADER_GC_GEM_SHOP_OPEN, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCGemShopOpen), false));
			Set(HEADER_GC_GEM_SHOP_REFRESH, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCGemShopRefresh), false));
#endif

#ifdef ENABLE_ATTENDANCE_EVENT
			Set(HEADER_GC_HIT_COUNT_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCHitCountInfo), false));
			Set(HEADER_GC_ATTENDANCE_EVENT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAttendanceEvent), false));
			Set(HEADER_GC_ATTENDANCE_EVENT_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAttendanceEventInfo), true));
#endif

#ifdef ENABLE_BOSS_TRACKING
			Set(HEADER_GC_BOSS_TRACKING, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCBossTracking), false));
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM
			Set(HEADER_GC_MAILBOX_RECEIVE,	CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCMailBoxReceive), false));
#endif

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
			Set(HEADER_GC_SEPCIAL_ZODIAC_EFFECT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSpecialZodiacEffect), false));
#endif

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
			Set(HEADER_GC_SAFEBOX_MONEY_CHANGE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSafeboxMoneyChange), false));
#endif

#ifdef ENABLE_AURA_SYSTEM
			Set(HEADER_GC_AURA, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketAura), false));
#endif
	
			Set(HEADER_GC_FEATURE_ENABLE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCFeatureEnable), false));
			
#ifdef ENABLE_SKILL_RENEWAL
			Set(HEADER_GC_SKILL_BOOK_COUNT, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCSkillBookCount), false));
#endif

#ifdef ENABLE_AVERAGE_ITEM_PRICE
			Set(HEADER_GC_AVERAGE_ITEM_PRICE, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCAverageSearch), false));
#endif

#ifdef ENABLE_GUILD_RANKING_SYSTEM
			Set(HEADER_GC_GUILD_RANK_SYSTEM, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCGuildRankSystem), false));
#endif
		}
};

int g_iLastPacket[2] = { 0, 0 };

void CPythonNetworkStream::ExitApplication()
{
	SendChatPacket("/quit");
}

void CPythonNetworkStream::ExitGame()
{
	SendChatPacket("/phase_select");
}


void CPythonNetworkStream::LogOutGame()
{
	AbsoluteExitGame();
}

void CPythonNetworkStream::AbsoluteExitGame()
{
	if (!IsOnline())
		return;

	OnRemoteDisconnect();		
	Disconnect();
}

void CPythonNetworkStream::AbsoluteExitApplication()
{
	PostQuitMessage(0);
}

DWORD CPythonNetworkStream::GetGuildID()
{
	return m_dwGuildID;
}

UINT CPythonNetworkStream::UploadMark(const char * c_szImageFileName)
{
	// MARK_BUG_FIX
	// 길드를 만든 직후는 길드 아이디가 0이다.
	if (0 == m_dwGuildID)
		return ERROR_MARK_UPLOAD_NEED_RECONNECT;

	gs_nextDownloadMarkTime = 0;
	// END_OF_MARK_BUG_FIX

	UINT uError=ERROR_UNKNOWN;
	CGuildMarkUploader& rkGuildMarkUploader=CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.Connect(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, m_dwGuildID, c_szImageFileName, &uError))
	{
		switch (uError)
		{
			case CGuildMarkUploader::ERROR_CONNECT:
				return ERROR_CONNECT_MARK_SERVER;
				break;
			case CGuildMarkUploader::ERROR_LOAD:
				return ERROR_LOAD_MARK;
				break;
			case CGuildMarkUploader::ERROR_WIDTH:
				return ERROR_MARK_WIDTH;
				break;
			case CGuildMarkUploader::ERROR_HEIGHT:
				return ERROR_MARK_HEIGHT;
				break;
			default:
				return ERROR_UNKNOWN;
		}
	}

	// MARK_BUG_FIX	
	__DownloadMark();
	// END_OF_MARK_BUG_FIX
	
	if (CGuildMarkManager::INVALID_MARK_ID == CGuildMarkManager::Instance().GetMarkID(m_dwGuildID))
		return ERROR_MARK_CHECK_NEED_RECONNECT;

	return ERROR_NONE;
}

UINT CPythonNetworkStream::UploadSymbol(const char* c_szImageFileName)
{
	UINT uError=ERROR_UNKNOWN;
	CGuildMarkUploader& rkGuildMarkUploader=CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.ConnectToSendSymbol(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, m_dwGuildID, c_szImageFileName, &uError))
	{
		switch (uError)
		{
			case CGuildMarkUploader::ERROR_CONNECT:
				return ERROR_CONNECT_MARK_SERVER;
				break;
			case CGuildMarkUploader::ERROR_LOAD:
				return ERROR_LOAD_MARK;
				break;
			case CGuildMarkUploader::ERROR_WIDTH:
				return ERROR_MARK_WIDTH;
				break;
			case CGuildMarkUploader::ERROR_HEIGHT:
				return ERROR_MARK_HEIGHT;
				break;
			default:
				return ERROR_UNKNOWN;
		}
	}

	return ERROR_NONE;
}

void CPythonNetworkStream::__DownloadMark()
{
	// 3분 안에는 다시 접속하지 않는다.
	DWORD curTime = ELTimer_GetMSec();

	if (curTime < gs_nextDownloadMarkTime)
		return;

	gs_nextDownloadMarkTime = curTime + 60000 * 3; // 3분

	CGuildMarkDownloader& rkGuildMarkDownloader = CGuildMarkDownloader::Instance();
	rkGuildMarkDownloader.Connect(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey);
}

void CPythonNetworkStream::__DownloadSymbol(const std::vector<DWORD> & c_rkVec_dwGuildID)
{
	CGuildMarkDownloader& rkGuildMarkDownloader=CGuildMarkDownloader::Instance();
	rkGuildMarkDownloader.ConnectToRecvSymbol(m_kMarkAuth.m_kNetAddr, m_kMarkAuth.m_dwHandle, m_kMarkAuth.m_dwRandomKey, c_rkVec_dwGuildID);
}

void CPythonNetworkStream::SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd)
{
	if (ePhaseWnd>=PHASE_WINDOW_NUM)
		return;

	m_apoPhaseWnd[ePhaseWnd]=poPhaseWnd;
}

void CPythonNetworkStream::ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd)
{
	if (ePhaseWnd>=PHASE_WINDOW_NUM)
		return;

	if (poPhaseWnd != m_apoPhaseWnd[ePhaseWnd])
		return;

	m_apoPhaseWnd[ePhaseWnd]=0;
}

void CPythonNetworkStream::SetServerCommandParserWindow(PyObject* poWnd)
{
	m_poSerCommandParserWnd = poWnd;
}

bool CPythonNetworkStream::IsSelectedEmpire()
{
	if (m_dwEmpireID)
		return true;
	
	return false;
}

UINT CPythonNetworkStream::GetAccountCharacterSlotDatau(UINT iSlot, UINT eType)
{
	if (iSlot >= PLAYER_PER_ACCOUNT)
		return 0;
		
	TSimplePlayerInformation&	rkSimplePlayerInfo=m_akSimplePlayerInfo[iSlot];
	
	switch (eType)
	{
		case ACCOUNT_CHARACTER_SLOT_ID:
			return rkSimplePlayerInfo.dwID;
		case ACCOUNT_CHARACTER_SLOT_RACE:
			return rkSimplePlayerInfo.byJob;
		case ACCOUNT_CHARACTER_SLOT_LEVEL:
			return rkSimplePlayerInfo.byLevel;
		case ACCOUNT_CHARACTER_SLOT_STR:
			return rkSimplePlayerInfo.byST;
		case ACCOUNT_CHARACTER_SLOT_DEX:
			return rkSimplePlayerInfo.byDX;
		case ACCOUNT_CHARACTER_SLOT_HTH:
			return rkSimplePlayerInfo.byHT;
		case ACCOUNT_CHARACTER_SLOT_INT:			
			return rkSimplePlayerInfo.byIQ;
		case ACCOUNT_CHARACTER_SLOT_PLAYTIME:
			return rkSimplePlayerInfo.dwPlayMinutes;
		case ACCOUNT_CHARACTER_SLOT_FORM:
			return rkSimplePlayerInfo.wMainPart;
		case ACCOUNT_CHARACTER_SLOT_PORT:
			return rkSimplePlayerInfo.wPort;
		case ACCOUNT_CHARACTER_SLOT_GUILD_ID:
			return m_adwGuildID[iSlot];
		case ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG:
			return rkSimplePlayerInfo.bChangeName;
		case ACCOUNT_CHARACTER_SLOT_HAIR:
			return rkSimplePlayerInfo.wHairPart;
#ifdef ENABLE_ACCE_SYSTEM
		case ACCOUNT_CHARACTER_SLOT_SASH:
			return rkSimplePlayerInfo.wSashPart;
#endif
		case ACCOUNT_CHARACTER_SLOT_LAST_PLAYTIME:
			return rkSimplePlayerInfo.last_play;
	}
	return 0;
}

const char* CPythonNetworkStream::GetAccountCharacterSlotDataz(UINT iSlot, UINT eType)
{
	static const char* sc_szEmpty="";

	if (iSlot >= PLAYER_PER_ACCOUNT)
		return sc_szEmpty;
		
	TSimplePlayerInformation&	rkSimplePlayerInfo=m_akSimplePlayerInfo[iSlot];
	
	switch (eType)
	{
		case ACCOUNT_CHARACTER_SLOT_ADDR:
			{				
				BYTE ip[4];

				const int LEN = 4;
				for (int i = 0; i < LEN; i++)
				{
					ip[i] = BYTE(rkSimplePlayerInfo.lAddr&0xff);
					rkSimplePlayerInfo.lAddr>>=8;
				}


				static char s_szAddr[256];
				sprintf(s_szAddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
				return s_szAddr;
			}
			break;
		case ACCOUNT_CHARACTER_SLOT_NAME:
			return rkSimplePlayerInfo.szName;
			break;		
		case ACCOUNT_CHARACTER_SLOT_GUILD_NAME:
			return m_astrGuildName[iSlot].c_str();
			break;
	}
	return sc_szEmpty;
}

void CPythonNetworkStream::ConnectLoginServer(const char* c_szAddr, UINT uPort)
{
	CNetworkStream::Connect(c_szAddr, uPort);		
}

void CPythonNetworkStream::SetMarkServer(const char* c_szAddr, UINT uPort)
{
	m_kMarkAuth.m_kNetAddr.Set(c_szAddr, uPort);
}

void CPythonNetworkStream::ConnectGameServer(UINT iChrSlot)
{
	if (iChrSlot >= PLAYER_PER_ACCOUNT)
		return;

	m_dwSelectedCharacterIndex = iChrSlot;

	__DirectEnterMode_Set(iChrSlot);

	TSimplePlayerInformation&	rkSimplePlayerInfo=m_akSimplePlayerInfo[iChrSlot];	
	CNetworkStream::Connect((DWORD)rkSimplePlayerInfo.lAddr, rkSimplePlayerInfo.wPort);
}

#ifdef ENABLE_CLIENT_VERSION_SYSTEM
void CPythonNetworkStream::SetLoginInfo(const char* c_szID, const char* c_szPassword, const char* c_szClientVersion, const char* c_szPin)
#else
void CPythonNetworkStream::SetLoginInfo(const char* c_szID, const char* c_szPassword)
#endif
{
	m_stID=c_szID;
	m_stPassword=c_szPassword;
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
	m_stClientVersion = c_szClientVersion;
	m_stPin = c_szPin;
#endif
}

void CPythonNetworkStream::ClearLoginInfo( void )
{
	m_stPassword = "";
#ifdef ENABLE_CLIENT_VERSION_SYSTEM
	m_stClientVersion = "";
	m_stPin = "";
#endif
}

void CPythonNetworkStream::SetLoginKey(DWORD dwLoginKey)
{
	m_dwLoginKey = dwLoginKey;
}

bool CPythonNetworkStream::CheckPacket(TPacketHeader * pRetHeader)
{
	*pRetHeader = 0;

	static CMainPacketHeaderMap s_packetHeaderMap;

	TPacketHeader header;

	if (!Peek(sizeof(TPacketHeader), &header))
		return false;

	if (0 == header)
	{
		if (!Recv(sizeof(TPacketHeader), &header))
			return false;
		
		while (Peek(sizeof(TPacketHeader), &header))
		{
			if (0 == header)
			{
				if (!Recv(sizeof(TPacketHeader), &header))
					return false;
			}
			else
			{
				break;
			}
		}

		if (0 == header)
			return false;
	}

	CNetworkPacketHeaderMap::TPacketType PacketType;

	if (!s_packetHeaderMap.Get(header, &PacketType))
	{
		TraceError("Unknown packet header: %d, last: %d -> %d", header, g_iLastPacket[0], g_iLastPacket[1]);
		//ClearRecvBuffer();
		//PostQuitMessage(0);
		return false;
	}

	// Code for dynamic size packet
	if (PacketType.isDynamicSizePacket)
	{
		TDynamicSizePacketHeader DynamicSizePacketHeader;

		if (!Peek(sizeof(TDynamicSizePacketHeader), &DynamicSizePacketHeader))
			return false;

		if (!Peek(DynamicSizePacketHeader.size))
		{
			Tracef ("CPythonNetworkStream::CheckPacket - Not enough dynamic packet size: header %d packet size: %d\n", DynamicSizePacketHeader.header, DynamicSizePacketHeader.size);
			return false;
		}
	}
	else
	{
		if (!Peek(PacketType.iPacketSize))
		{
			//Tracef("Not enough packet size: header %d packet size: %d, recv buffer size: %d",
			//	header,
			//	PacketType.iPacketSize,
			//	GetRecvBufferSize());
			return false;
		}
	}

	if (!header)
		return false;

	*pRetHeader = header;	

	g_iLastPacket[0] = g_iLastPacket[1];
	g_iLastPacket[1] = header;
	return true;
}

bool CPythonNetworkStream::RecvErrorPacket(int header)
{
	TraceError("Phase %s does not handle this header (header: %d, last: %d, %d)",
		m_strPhase.c_str(), header, g_iLastPacket[0], g_iLastPacket[1]);

	ClearRecvBuffer();
	return true;
}

bool CPythonNetworkStream::RecvPhasePacket()
{
	TPacketGCPhase packet_phase;

	if (!Recv(sizeof(TPacketGCPhase), &packet_phase))
		return false;

	switch (packet_phase.phase)
	{
		case PHASE_CLOSE:				// 끊기는 상태 (또는 끊기 전 상태)
			ClosePhase();
			break;

		case PHASE_HANDSHAKE:			// 악수..;;
			SetHandShakePhase();
			break;

		case PHASE_LOGIN:				// 로그인 중
			SetLoginPhase();
			break;

		case PHASE_SELECT:				// 캐릭터 선택 화면
			SetSelectPhase();
	
			// MARK_BUG_FIX
			__DownloadMark();
			// END_OF_MARK_BUG_FIX
			break;

		case PHASE_LOADING:				// 선택 후 로딩 화면
			SetLoadingPhase();
			break;

		case PHASE_GAME:				// 게임 화면
			SetGamePhase();
			break;

		case PHASE_DEAD:				// 죽었을 때.. (게임 안에 있는 것일 수도..)
			break;
	}

	return true;
}

bool CPythonNetworkStream::RecvPingPacket()
{
	Tracef("recv ping packet. (securitymode %u)\n", IsSecurityMode());

	TPacketGCPing kPacketPing;

	if (!Recv(sizeof(TPacketGCPing), &kPacketPing))
		return false;

	m_dwLastGamePingTime = ELTimer_GetMSec();

	TPacketCGPong kPacketPong;
	kPacketPong.bHeader = HEADER_CG_PONG;

	if (!Send(sizeof(TPacketCGPong), &kPacketPong))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvDefaultPacket(int header)
{
	if (!header)
		return true;

	TraceError ("Unprocessed packet header %d, state %s\n", header, m_strPhase.c_str());
	ClearRecvBuffer();
	return true;
}

bool CPythonNetworkStream::OnProcess()
{
	if (m_isStartGame)
	{
		m_isStartGame = FALSE;

		PyCallClassMemberFunc(m_poHandler, "SetGamePhase", Py_BuildValue("()"));
//		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartGame", Py_BuildValue("()"));
	}
	
	m_rokNetActorMgr->Update();

	if (m_phaseProcessFunc.IsEmpty())
		return true;
	m_phaseProcessFunc.Run();
	return true;
}


// Set
void CPythonNetworkStream::SetOffLinePhase()
{
	if ("OffLine" != m_strPhase)
		m_phaseLeaveFunc.Run();

	m_strPhase = "OffLine";

	Tracen (" ");
	Tracen ("## Network - OffLine Phase ##");
	Tracen (" ");
	Tracen (" ");

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::OffLinePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveOfflinePhase);

	SetGameOffline();

	m_dwSelectedCharacterIndex = 0;

	__DirectEnterMode_Initialize();
	__BettingGuildWar_Initialize();
}


void CPythonNetworkStream::ClosePhase()
{
	PyCallClassMemberFunc(m_poHandler, "SetLoginPhase", Py_BuildValue("()"));
}

// Game Online
void CPythonNetworkStream::SetGameOnline()
{
	m_isGameOnline = TRUE;
}

void CPythonNetworkStream::SetGameOffline()
{
	m_isGameOnline = FALSE;
}

BOOL CPythonNetworkStream::IsGameOnline()
{
	return m_isGameOnline;
}

// Handler
void CPythonNetworkStream::SetHandler(PyObject* poHandler)
{
	m_poHandler = poHandler;
}

// ETC
DWORD CPythonNetworkStream::GetMainActorVID()
{
	return m_dwMainActorVID;
}

DWORD CPythonNetworkStream::GetMainActorRace()
{
	return m_dwMainActorRace;
}

DWORD CPythonNetworkStream::GetMainActorEmpire()
{
	return m_dwMainActorEmpire;
}

DWORD CPythonNetworkStream::GetMainActorSkillGroup()
{
	return m_dwMainActorSkillGroup;
}

void CPythonNetworkStream::SetEmpireID(DWORD dwEmpireID)
{
	m_dwEmpireID = dwEmpireID;
}

DWORD CPythonNetworkStream::GetEmpireID()
{
	return m_dwEmpireID;
}

void CPythonNetworkStream::__ClearSelectCharacterData()
{
	NANOBEGIN
	memset(&m_akSimplePlayerInfo, 0, sizeof(m_akSimplePlayerInfo));

	for (int i = 0; i < PLAYER_PER_ACCOUNT; ++i)
	{
		m_adwGuildID[i] = 0;
		m_astrGuildName[i] = "";
	}
	NANOEND
}

void CPythonNetworkStream::__DirectEnterMode_Initialize()
{
	m_kDirectEnterMode.m_isSet=false;
	m_kDirectEnterMode.m_dwChrSlotIndex=0;	
}

void CPythonNetworkStream::__DirectEnterMode_Set(UINT uChrSlotIndex)
{
	m_kDirectEnterMode.m_isSet=true;
	m_kDirectEnterMode.m_dwChrSlotIndex=uChrSlotIndex;
}

bool CPythonNetworkStream::__DirectEnterMode_IsSet()
{
	return m_kDirectEnterMode.m_isSet;
}

void CPythonNetworkStream::__InitializeMarkAuth()
{
	m_kMarkAuth.m_dwHandle=0;
	m_kMarkAuth.m_dwRandomKey=0;
}

void CPythonNetworkStream::__BettingGuildWar_Initialize()
{
	m_kBettingGuildWar.m_dwBettingMoney=0;
	m_kBettingGuildWar.m_dwObserverCount=0;
}

void CPythonNetworkStream::__BettingGuildWar_SetObserverCount(UINT uObserverCount)
{
	m_kBettingGuildWar.m_dwObserverCount=uObserverCount;
}

void CPythonNetworkStream::__BettingGuildWar_SetBettingMoney(UINT uBettingMoney)
{
	m_kBettingGuildWar.m_dwBettingMoney=uBettingMoney;
}

DWORD CPythonNetworkStream::EXPORT_GetBettingGuildWarValue(const char* c_szValueName)
{
	if (stricmp(c_szValueName, "OBSERVER_COUNT") == 0)
		return m_kBettingGuildWar.m_dwObserverCount;

	if (stricmp(c_szValueName, "BETTING_MONEY") == 0)
		return m_kBettingGuildWar.m_dwBettingMoney;

	return 0;
}

void CPythonNetworkStream::__ServerTimeSync_Initialize()
{
	m_kServerTimeSync.m_dwChangeClientTime=0;
	m_kServerTimeSync.m_dwChangeServerTime=0;
}

void CPythonNetworkStream::SetWaitFlag()
{
	m_isWaitLoginKey = TRUE;
}

void CPythonNetworkStream::SendEmoticon(UINT eEmoticon)
{
	if(eEmoticon < m_EmoticonStringVector.size())
		SendChatPacket(m_EmoticonStringVector[eEmoticon].c_str());
	else
		assert(false && "SendEmoticon Error");
}

CPythonNetworkStream::CPythonNetworkStream()
{
	m_rokNetActorMgr=new CNetworkActorManager;

	memset(m_akSimplePlayerInfo, 0, sizeof(m_akSimplePlayerInfo));

	m_phaseProcessFunc.Clear();

	m_dwEmpireID = 0;
	m_dwGuildID = 0;

	m_dwMainActorVID = 0;
	m_dwMainActorRace = 0;
	m_dwMainActorEmpire = 0;
	m_dwMainActorSkillGroup = 0;
	m_poHandler = nullptr;

	m_dwLastGamePingTime = 0;

	m_dwLoginKey = 0;
	m_isWaitLoginKey = FALSE;
	m_isStartGame = FALSE;
	m_isEnableChatInsultFilter = FALSE;
	m_bComboSkillFlag = FALSE;
	m_strPhase = "OffLine";
	
	__InitializeGamePhase();
	__InitializeMarkAuth();

	__DirectEnterMode_Initialize();
	__BettingGuildWar_Initialize();

	std::fill(m_apoPhaseWnd, m_apoPhaseWnd+PHASE_WINDOW_NUM, (PyObject*)nullptr);
	m_poSerCommandParserWnd = nullptr;

	SetOffLinePhase();
}

CPythonNetworkStream::~CPythonNetworkStream()
{
	Tracen("PythonNetworkMainStream Clear");
}

void CPythonNetworkStream::HideQuestWindows()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "HideAllQuestWindow", Py_BuildValue("()"));
}