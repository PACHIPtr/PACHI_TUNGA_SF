#include "stdafx.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "party.h"
#include "messenger_manager.h"
#include "unique_item.h"
#include "xmas_event.h"
#include "affect.h"
#include "locale_service.h"
#include "questmanager.h"
#include "skill.h"
#include "sectree_manager.h"
#include "buffer_manager.h"
#include "dungeon.h"
#ifdef ENABLE_BOSS_TRACKING_SYSTEM
#include "boss_tracking.h"
#endif
#include "features.h"
ACMD(do_reload);
////////////////////////////////////////////////////////////////////////////////
// Input Processor
CInputP2P::CInputP2P()
{
	BindPacketInfo(&m_packetInfoGG);
}

void CInputP2P::Login(LPDESC d, const char* c_pData)
{
	P2P_MANAGER::instance().Login(d, (TPacketGGLogin*)c_pData);
}

void CInputP2P::Logout(LPDESC d, const char* c_pData)
{
	TPacketGGLogout* p = (TPacketGGLogout*)c_pData;
	P2P_MANAGER::instance().Logout(p->szName);
}

int CInputP2P::Relay(LPDESC d, const char* c_pData, size_t uiBytes)
{
	TPacketGGRelay* p = (TPacketGGRelay*)c_pData;

	if (uiBytes < sizeof(TPacketGGRelay) + p->lSize)
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	sys_log(0, "InputP2P::Relay : %s size %d", p->szName, p->lSize);

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(p->szName);

	const BYTE* c_pbData = (const BYTE*)(c_pData + sizeof(TPacketGGRelay));

	if (!pkChr)
		return p->lSize;

	if (*c_pbData == HEADER_GC_WHISPER)
	{
		if (pkChr->IsBlockMode(BLOCK_WHISPER))
		{
			// ±Ó¼Ó¸» °ÅºÎ »óÅÂ¿¡¼­ ±Ó¼Ó¸» °ÅºÎ.
			return p->lSize;
		}

		char buf[1024];
		memcpy(buf, c_pbData, MIN(p->lSize, sizeof(buf)));

		TPacketGCWhisper* p2 = (TPacketGCWhisper*)buf;
		// bType »óÀ§ 4ºñÆ®: Empire ¹øÈ£
		// bType ÇÏÀ§ 4ºñÆ®: EWhisperType
		//BYTE bToEmpire = (p2->bType >> 4);
		p2->bType = p2->bType & 0x0F;
		if (p2->bType == 0x0F) {
			// ½Ã½ºÅÛ ¸Þ¼¼Áö ±Ó¼Ó¸»Àº bTypeÀÇ »óÀ§ºñÆ®±îÁö ¸ðµÎ »ç¿ëÇÔ.
			p2->bType = WHISPER_TYPE_SYSTEM;
		}

		pkChr->GetDesc()->Packet(buf, p->lSize);
	}
	else
		pkChr->GetDesc()->Packet(c_pbData, p->lSize);

	return (p->lSize);
}

#ifdef ENABLE_FULL_NOTICE
int CInputP2P::Notice(LPDESC d, const char* c_pData, size_t uiBytes, bool bBigFont)
#else
int CInputP2P::Notice(LPDESC d, const char* c_pData, size_t uiBytes)
#endif
{
	TPacketGGNotice* p = (TPacketGGNotice*)c_pData;

	if (uiBytes < sizeof(TPacketGGNotice) + p->lSize)
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char szBuf[256 + 1];
	strlcpy(szBuf, c_pData + sizeof(TPacketGGNotice), MIN(p->lSize + 1, sizeof(szBuf)));
#ifdef ENABLE_FULL_NOTICE
	SendNotice(szBuf, bBigFont);
#else
	SendNotice(szBuf);
#endif
	return (p->lSize);
}

int CInputP2P::GuildNotice(LPDESC d, const char* c_pData, size_t uiBytes)
{
	TPacketGGGuildNotice* p = (TPacketGGGuildNotice*)c_pData;

	if (uiBytes < p->lSize + sizeof(TPacketGGGuildNotice))
		return -1;

	if (p->lSize < 0)
	{
		sys_err("invalid packet length %d", p->lSize);
		d->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char szBuf[256 + 1];
	strlcpy(szBuf, c_pData + sizeof(TPacketGGGuildNotice), MIN(p->lSize + 1, sizeof(szBuf)));
	SendGuildNotice(p->dwGuildID, szBuf);
	return (p->lSize);
}

int CInputP2P::Guild(LPDESC d, const char* c_pData, size_t uiBytes)
{
	TPacketGGGuild* p = (TPacketGGGuild*)c_pData;
	uiBytes -= sizeof(TPacketGGGuild);
	c_pData += sizeof(TPacketGGGuild);

	CGuild* g = CGuildManager::instance().FindGuild(p->dwGuild);

	switch (p->bSubHeader)
	{
	case GUILD_SUBHEADER_GG_CHAT:
	{
		if (uiBytes < sizeof(TPacketGGGuildChat))
			return -1;

		TPacketGGGuildChat* p = (TPacketGGGuildChat*)c_pData;

		if (g)
			g->P2PChat(p->szText);

		return sizeof(TPacketGGGuildChat);
	}

	case GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS:
	{
		if (uiBytes < sizeof(int))
			return -1;

		int iBonus = *((int*)c_pData);
		CGuild* pGuild = CGuildManager::instance().FindGuild(p->dwGuild);
		if (pGuild)
		{
			pGuild->SetMemberCountBonus(iBonus);
		}
		return sizeof(int);
	}
	default:
		sys_err("UNKNOWN GUILD SUB PACKET");
		break;
	}
	return 0;
}

struct FuncShout
{
	const char* m_str;
	BYTE m_bEmpire;

	FuncShout(const char* str, BYTE bEmpire) : m_str(str), m_bEmpire(bEmpire)
	{
	}

	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		d->GetCharacter()->ChatPacket(CHAT_TYPE_SHOUT, "%s", m_str);
	}
};

void SendShout(const char* szText, BYTE bEmpire)
{
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), FuncShout(szText, bEmpire));
}

void CInputP2P::Shout(const char* c_pData)
{
	TPacketGGShout* p = (TPacketGGShout*)c_pData;
	SendShout(p->szText, p->bEmpire);
}

void CInputP2P::Disconnect(const char* c_pData)
{
	TPacketGGDisconnect* p = (TPacketGGDisconnect*)c_pData;

	LPDESC d = DESC_MANAGER::instance().FindByLoginName(p->szLogin);

	if (!d)
		return;

	if (!d->GetCharacter())
	{
		d->SetPhase(PHASE_CLOSE);
	}
	else
		d->DisconnectOfSameLogin();
}

void CInputP2P::Setup(LPDESC d, const char* c_pData)
{
	TPacketGGSetup* p = (TPacketGGSetup*)c_pData;
	sys_log(0, "P2P: Setup %s:%d", d->GetHostName(), p->wPort);
	d->SetP2P(d->GetHostName(), p->wPort, p->bChannel);
}

void CInputP2P::MessengerAdd(const char* c_pData)
{
	TPacketGGMessenger* p = (TPacketGGMessenger*)c_pData;
	sys_log(0, "P2P: Messenger Add %s %s", p->szAccount, p->szCompanion);
	MessengerManager::instance().__AddToList(p->szAccount, p->szCompanion);
}

void CInputP2P::MessengerRemove(const char* c_pData)
{
	TPacketGGMessenger* p = (TPacketGGMessenger*)c_pData;
	sys_log(0, "P2P: Messenger Remove %s %s", p->szAccount, p->szCompanion);
	MessengerManager::instance().__RemoveFromList(p->szAccount, p->szCompanion);
}

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
void CInputP2P::MessengerBlockAdd(const char* c_pData)
{
	TPacketGGMessenger* p = (TPacketGGMessenger*)c_pData;
	MessengerManager::instance().__AddToBlockList(p->szAccount, p->szCompanion);
}

void CInputP2P::MessengerBlockRemove(const char* c_pData)
{
	TPacketGGMessenger* p = (TPacketGGMessenger*)c_pData;
	MessengerManager::instance().__RemoveFromBlockList(p->szAccount, p->szCompanion);
}
#endif

void CInputP2P::FindPosition(LPDESC d, const char* c_pData)
{
	TPacketGGFindPosition* p = (TPacketGGFindPosition*)c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->dwTargetPID);
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	if (ch)
#else
	if (ch && ch->GetMapIndex() < 10000)
#endif
	{
		TPacketGGWarpCharacter pw;
		pw.header = HEADER_GG_WARP_CHARACTER;
		pw.pid = p->dwFromPID;
		pw.x = ch->GetX();
		pw.y = ch->GetY();
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
		pw.mapIndex = (ch->GetMapIndex() < 10000) ? 0 : ch->GetMapIndex();
#endif
		d->Packet(&pw, sizeof(pw));
	}
}

void CInputP2P::WarpCharacter(const char* c_pData)
{
	TPacketGGWarpCharacter* p = (TPacketGGWarpCharacter*)c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->pid);
	if (ch)
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
		ch->WarpSet(p->x, p->y, p->mapIndex);
#else
		ch->WarpSet(p->x, p->y);
#endif
}

void CInputP2P::GuildWarZoneMapIndex(const char* c_pData)
{
	TPacketGGGuildWarMapIndex* p = (TPacketGGGuildWarMapIndex*)c_pData;
	CGuildManager& gm = CGuildManager::instance();

	sys_log(0, "P2P: GuildWarZoneMapIndex g1(%u) vs g2(%u), mapIndex(%d)", p->dwGuildID1, p->dwGuildID2, p->lMapIndex);

	CGuild* g1 = gm.FindGuild(p->dwGuildID1);
	CGuild* g2 = gm.FindGuild(p->dwGuildID2);

	if (g1 && g2)
	{
		g1->SetGuildWarMapIndex(p->dwGuildID2, p->lMapIndex);
		g2->SetGuildWarMapIndex(p->dwGuildID1, p->lMapIndex);
	}
}

void CInputP2P::Transfer(const char* c_pData)
{
	TPacketGGTransfer* p = (TPacketGGTransfer*)c_pData;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(p->szName);

	if (ch)
		ch->WarpSet(p->lX, p->lY);
}

void CInputP2P::XmasWarpSanta(const char* c_pData)
{
	TPacketGGXmasWarpSanta* p = (TPacketGGXmasWarpSanta*)c_pData;

	if (p->bChannel == g_bChannel && map_allow_find(p->lMapIndex))
	{
		int	iNextSpawnDelay = 50 * 60;

		xmas::SpawnSanta(p->lMapIndex, iNextSpawnDelay);

		TPacketGGXmasWarpSantaReply pack_reply;
		pack_reply.bHeader = HEADER_GG_XMAS_WARP_SANTA_REPLY;
		pack_reply.bChannel = g_bChannel;
		P2P_MANAGER::instance().Send(&pack_reply, sizeof(pack_reply));
	}
}

void CInputP2P::XmasWarpSantaReply(const char* c_pData)
{
	TPacketGGXmasWarpSantaReply* p = (TPacketGGXmasWarpSantaReply*)c_pData;

	if (p->bChannel == g_bChannel)
	{
		CharacterVectorInteractor i;

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(xmas::MOB_SANTA_VNUM, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();

			while (it != i.end()) {
				M2_DESTROY_CHARACTER(*it++);
			}
		}
	}
}

void CInputP2P::LoginPing(LPDESC d, const char* c_pData)
{
	TPacketGGLoginPing* p = (TPacketGGLoginPing*)c_pData;

	if (!g_pkAuthMasterDesc) // If I am master, I have to broadcast
		P2P_MANAGER::instance().Send(p, sizeof(TPacketGGLoginPing), d);
}

// BLOCK_CHAT
void CInputP2P::BlockChat(const char* c_pData)
{
	TPacketGGBlockChat* p = (TPacketGGBlockChat*)c_pData;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(p->szName);

	if (ch)
	{
		sys_log(0, "BLOCK CHAT apply name %s dur %d", p->szName, p->lBlockDuration);
		ch->AddAffect(AFFECT_BLOCK_CHAT, POINT_NONE, 0, AFF_NONE, p->lBlockDuration, 0, true);
	}
	else
	{
		sys_log(0, "BLOCK CHAT fail name %s dur %d", p->szName, p->lBlockDuration);
	}
}
// END_OF_BLOCK_CHAT

#ifdef ENABLE_BOSS_TRACKING_SYSTEM
void CInputP2P::BossTracking(LPDESC d, const char* c_pData)
{
	TPacketGGBossTracking* p = (TPacketGGBossTracking*)c_pData;
	CBossTracking::instance().SetDeadTime(p->channel, p->mob_vnum, p->dead_time);
	CBossTracking::instance().SetRegenTime(p->channel, p->mob_vnum, p->regen_time);
}
#endif

#ifdef ENABLE_OFFLINESHOP_MESSAGE_SYSTEM
void CInputP2P::SendOfflineShopMessage(LPDESC d, const char * c_pData)
{
	TPacketGGOfflineShopMessage * p = (TPacketGGOfflineShopMessage *)c_pData;
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(p->dwTargetPID);
	
	if (ch)
	{
		LPDESC pkVictimDesc = ch->GetDesc();

		if (pkVictimDesc)
		{
			char msg[CHAT_MAX_LEN + 1];
			snprintf(msg, sizeof(msg), LC_TEXT("Your item %s sold, buyer is: %s."), p->szItemName, p->szName);

			TPacketGCWhisper pack;

			int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

			pack.bHeader = HEADER_GC_WHISPER;
			pack.wSize = sizeof(TPacketGCWhisper) + len;
			pack.bType = WHISPER_TYPE_SYSTEM;
			pack.bLevel = 0;
			strlcpy(pack.szNameFrom, "[Dükkan]", sizeof(pack.szNameFrom));

			TEMP_BUFFER buf;

			buf.write(&pack, sizeof(TPacketGCWhisper));
			buf.write(msg, len);

			pkVictimDesc->Packet(buf.read_peek(), buf.size());
		}
	}
}
#endif

void CInputP2P::FeatureEnable(const char * c_pData)
{
	TPacketGGFeatureEnable* p = (TPacketGGFeatureEnable*) c_pData;
	features::SetFeatureEnabled(p->feature_index, p->enabled, true);
}

int CInputP2P::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)
{
	if (test_server)
		sys_log(0, "CInputP2P::Anlayze[Header %d]", bHeader);

	int iExtraLen = 0;

	switch (bHeader)
	{
	case HEADER_GG_SETUP:
		Setup(d, c_pData);
		break;

	case HEADER_GG_LOGIN:
		Login(d, c_pData);
		break;

	case HEADER_GG_LOGOUT:
		Logout(d, c_pData);
		break;

	case HEADER_GG_RELAY:
		if ((iExtraLen = Relay(d, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_GG_NOTICE:
		if ((iExtraLen = Notice(d, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_GG_SHUTDOWN:
		sys_err("Accept shutdown p2p command from %s.", d->GetHostName());
		Shutdown(10);
		break;

	case HEADER_GG_GUILD:
		if ((iExtraLen = Guild(d, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;

	case HEADER_GG_SHOUT:
		Shout(c_pData);
		break;

	case HEADER_GG_DISCONNECT:
		Disconnect(c_pData);
		break;

	case HEADER_GG_MESSENGER_ADD:
		MessengerAdd(c_pData);
		break;

	case HEADER_GG_MESSENGER_REMOVE:
		MessengerRemove(c_pData);
		break;

#ifdef ENABLE_MESSENGER_BLOCK_SYSTEM
	case HEADER_GG_MESSENGER_BLOCK_ADD:
		MessengerBlockAdd(c_pData);
		break;

	case HEADER_GG_MESSENGER_BLOCK_REMOVE:
		MessengerBlockRemove(c_pData);
		break;
#endif

	case HEADER_GG_FIND_POSITION:
		FindPosition(d, c_pData);
		break;

	case HEADER_GG_WARP_CHARACTER:
		WarpCharacter(c_pData);
		break;

	case HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX:
		GuildWarZoneMapIndex(c_pData);
		break;

	case HEADER_GG_TRANSFER:
		Transfer(c_pData);
		break;

	case HEADER_GG_XMAS_WARP_SANTA:
		XmasWarpSanta(c_pData);
		break;

	case HEADER_GG_XMAS_WARP_SANTA_REPLY:
		XmasWarpSantaReply(c_pData);
		break;

	case HEADER_GG_CHECK_CLIENT_VERSION:
		CheckClientVersion();
		break;

	case HEADER_GG_LOGIN_PING:
		LoginPing(d, c_pData);
		break;

	case HEADER_GG_BLOCK_CHAT:
		BlockChat(c_pData);
		break;

#ifdef ENABLE_GUILD_NOTICE_SYSTEM
	case HEADER_GG_GUILD_NOTICE:
		if ((iExtraLen = GuildNotice(d, c_pData, m_iBufferLeft)) < 0)
			return -1;
		break;
#endif

	case HEADER_GG_RELOAD_COMMAND:
		do_reload(nullptr, ((TPacketGGReloadCommand*)c_pData)->argument, 0, 0);
		break;

#ifdef ENABLE_FULL_NOTICE
	case HEADER_GG_BIG_NOTICE:
		if ((iExtraLen = Notice(d, c_pData, m_iBufferLeft, true)) < 0)
			return -1;
		break;
#endif

#ifdef ENABLE_BOSS_TRACKING_SYSTEM
	case HEADER_GG_BOSS_TRACKING:
		BossTracking(d, c_pData);
		break;
#endif

#ifdef ENABLE_OFFLINESHOP_MESSAGE_SYSTEM
	case HEADER_GG_OFFLINE_SHOP_SEND_MESSAGE:
		SendOfflineShopMessage(d, c_pData);
		break;
#endif

	case HEADER_GG_FEATURE_ENABLE:
		FeatureEnable(c_pData);
		break;
	}

	return (iExtraLen);
}