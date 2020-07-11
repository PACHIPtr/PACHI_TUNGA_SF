#include "stdafx.h"
#include "constants.h"
#include "pvp.h"
#include "crc32.h"
#include "packet.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "config.h"
#include "sectree_manager.h"
#include "buffer_manager.h"
#include "locale_service.h"
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
#include "combat_zone.h"
#endif

using namespace std;

CPVP::CPVP(DWORD dwPID1, DWORD dwPID2)
{
	if (dwPID1 > dwPID2)
	{
		m_players[0].dwPID = dwPID1;
		m_players[1].dwPID = dwPID2;
		m_players[0].bAgree = true;
	}
	else
	{
		m_players[0].dwPID = dwPID2;
		m_players[1].dwPID = dwPID1;
		m_players[1].bAgree = true;
	}

	DWORD adwID[2];
	adwID[0] = m_players[0].dwPID;
	adwID[1] = m_players[1].dwPID;
	m_dwCRC = GetFastHash((const char*)& adwID, 8);
	m_bRevenge = false;

	SetLastFightTime();
}

CPVP::CPVP(CPVP& k)
{
	m_players[0] = k.m_players[0];
	m_players[1] = k.m_players[1];

	m_dwCRC = k.m_dwCRC;
	m_bRevenge = k.m_bRevenge;

	SetLastFightTime();
}

CPVP::~CPVP()
{
}

void CPVP::Packet(bool bDelete)
{
	if (!m_players[0].dwVID || !m_players[1].dwVID)
	{
		if (bDelete)
			sys_err("nullptr vid when removing %u %u", m_players[0].dwVID, m_players[0].dwVID);

		return;
	}

	TPacketGCPVP pack;

	pack.bHeader = HEADER_GC_PVP;

	if (bDelete)
	{
		pack.bMode = PVP_MODE_NONE;
		pack.dwVIDSrc = m_players[0].dwVID;
		pack.dwVIDDst = m_players[1].dwVID;
	}
	else if (IsFight())
	{
		pack.bMode = PVP_MODE_FIGHT;
		pack.dwVIDSrc = m_players[0].dwVID;
		pack.dwVIDDst = m_players[1].dwVID;
	}
	else
	{
		pack.bMode = m_bRevenge ? PVP_MODE_REVENGE : PVP_MODE_AGREE;

		if (m_players[0].bAgree)
		{
			pack.dwVIDSrc = m_players[0].dwVID;
			pack.dwVIDDst = m_players[1].dwVID;
		}
		else
		{
			pack.dwVIDSrc = m_players[1].dwVID;
			pack.dwVIDDst = m_players[0].dwVID;
		}
	}

	const DESC_MANAGER::DESC_SET& c_rSet = DESC_MANAGER::instance().GetClientSet();
	DESC_MANAGER::DESC_SET::const_iterator it = c_rSet.begin();

	while (it != c_rSet.end())
	{
		LPDESC d = *it++;

		if (d->IsPhase(PHASE_GAME) || d->IsPhase(PHASE_DEAD))
			d->Packet(&pack, sizeof(pack));
	}
}

bool CPVP::Agree(DWORD dwPID)
{
	m_players[m_players[0].dwPID != dwPID ? 1 : 0].bAgree = true;

	if (IsFight())
	{
		Packet();
		return true;
	}
	return false;
}

bool CPVP::IsFight()
{
	return (m_players[0].bAgree == m_players[1].bAgree) && m_players[0].bAgree;
}

void CPVP::Win(DWORD dwPID)
{
	int iSlot = m_players[0].dwPID != dwPID ? 1 : 0;

	m_bRevenge = true;

	m_players[iSlot].bAgree = true; // 자동으로 동의
	m_players[!iSlot].bCanRevenge = true;
	m_players[!iSlot].bAgree = false;

	Packet();
}

bool CPVP::CanRevenge(DWORD dwPID)
{
	return m_players[m_players[0].dwPID != dwPID ? 1 : 0].bCanRevenge;
}

void CPVP::SetVID(DWORD dwPID, DWORD dwVID)
{
	if (m_players[0].dwPID == dwPID)
		m_players[0].dwVID = dwVID;
	else
		m_players[1].dwVID = dwVID;
}

void CPVP::SetLastFightTime()
{
	m_dwLastFightTime = get_dword_time();
}

DWORD CPVP::GetLastFightTime()
{
	return m_dwLastFightTime;
}

CPVPManager::CPVPManager()
{
}

CPVPManager::~CPVPManager()
{
}
void CPVPManager::Insert(LPCHARACTER pkChr, LPCHARACTER pkVictim)
{
	if (pkChr->IsDead() || pkVictim->IsDead())
		return;

	CPVP kPVP(pkChr->GetPlayerID(), pkVictim->GetPlayerID());

	CPVP * pkPVP;

	if ((pkPVP = Find(kPVP.m_dwCRC)))
	{
		// 복수할 수 있으면 바로 싸움!
		if (pkPVP->Agree(pkChr->GetPlayerID()))
		{
			pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s님과의 대결 시작!"), pkChr->GetName());
			pkChr->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s님과의 대결 시작!"), pkVictim->GetName());

			pkVictim->EffectPacket(SE_PVP_BEGIN1);
			pkChr->EffectPacket(SE_PVP_BEGIN2);
		}
		return;
	}

	pkPVP = M2_NEW CPVP(kPVP);

	pkPVP->SetVID(pkChr->GetPlayerID(), pkChr->GetVID());
	pkPVP->SetVID(pkVictim->GetPlayerID(), pkVictim->GetVID());

	m_map_pkPVP.insert(map<DWORD, CPVP*>::value_type(pkPVP->m_dwCRC, pkPVP));

	m_map_pkPVPSetByID[pkChr->GetPlayerID()].insert(pkPVP);
	m_map_pkPVPSetByID[pkVictim->GetPlayerID()].insert(pkPVP);

	pkPVP->Packet();

	char msg[CHAT_MAX_LEN + 1];
	snprintf(msg, sizeof(msg), LC_TEXT("%s님이 대결신청을 했습니다. 승낙하려면 대결동의를 하세요."), pkChr->GetName());

	pkVictim->ChatPacket(CHAT_TYPE_INFO, msg);
	pkChr->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s에게 대결신청을 했습니다."), pkVictim->GetName());

	pkChr->EffectPacket(SE_PVP_OPEN1);
	pkVictim->EffectPacket(SE_PVP_OPEN2);

	// NOTIFY_PVP_MESSAGE
	LPDESC pkVictimDesc = pkVictim->GetDesc();
	if (pkVictimDesc)
	{
		TPacketGCWhisper pack;
		int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);
		pack.bHeader = HEADER_GC_WHISPER;
		pack.wSize = sizeof(TPacketGCWhisper) + len;
		pack.bType = WHISPER_TYPE_SYSTEM;
		strlcpy(pack.szNameFrom, pkChr->GetName(), sizeof(pack.szNameFrom));
		TEMP_BUFFER buf;
		buf.write(&pack, sizeof(TPacketGCWhisper));
		buf.write(msg, len);
		pkVictimDesc->Packet(buf.read_peek(), buf.size());
	}
	// END_OF_NOTIFY_PVP_MESSAGE
}

void CPVPManager::ConnectEx(LPCHARACTER pkChr, bool bDisconnect)
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return;

	DWORD dwVID = bDisconnect ? 0 : pkChr->GetVID();

	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;
		pkPVP->SetVID(pkChr->GetPlayerID(), dwVID);
	}
}

void CPVPManager::Connect(LPCHARACTER pkChr)
{
	ConnectEx(pkChr, false);
}

void CPVPManager::Disconnect(LPCHARACTER pkChr)
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return;

	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end()) {
		CPVP* pkPVP = *it2++;
		pkPVP->Packet(true);
		Delete(pkPVP);
	}
}

void CPVPManager::GiveUp(LPCHARACTER pkChr, DWORD dwKillerPID) // This method is calling from no where yet.
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return;

	sys_log(1, "PVPManager::Dead %d", pkChr->GetPlayerID());
	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;

		DWORD dwCompanionPID;

		if (pkPVP->m_players[0].dwPID == pkChr->GetPlayerID())
			dwCompanionPID = pkPVP->m_players[1].dwPID;
		else
			dwCompanionPID = pkPVP->m_players[0].dwPID;

		if (dwCompanionPID != dwKillerPID)
			continue;

		pkPVP->SetVID(pkChr->GetPlayerID(), 0);

		m_map_pkPVPSetByID.erase(dwCompanionPID);

		it->second.erase(pkPVP);

		if (it->second.empty())
			m_map_pkPVPSetByID.erase(it);

		m_map_pkPVP.erase(pkPVP->m_dwCRC);

		pkPVP->Packet(true);
		M2_DELETE(pkPVP);
		break;
	}
}

// 리턴값: 0 = PK, 1 = PVP
// PVP를 리턴하면 경험치나 아이템을 떨구고 PK면 떨구지 않는다.
bool CPVPManager::Dead(LPCHARACTER pkChr, DWORD dwKillerPID)
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return false;

	bool found = false;

	sys_log(1, "PVPManager::Dead %d", pkChr->GetPlayerID());
	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;

		DWORD dwCompanionPID;

		if (pkPVP->m_players[0].dwPID == pkChr->GetPlayerID())
			dwCompanionPID = pkPVP->m_players[1].dwPID;
		else
			dwCompanionPID = pkPVP->m_players[0].dwPID;

		if (dwCompanionPID == dwKillerPID)
		{
			if (pkPVP->IsFight())
			{
				pkPVP->SetLastFightTime();
				pkPVP->Win(dwKillerPID);

				LPCHARACTER pkPVPChar = CHARACTER_MANAGER::instance().FindByPID(dwKillerPID);

				if (pkPVPChar)
					pkPVPChar->EffectPacket(SE_PVP_WIN);

				found = true;
				break;
			}
			else if (get_dword_time() - pkPVP->GetLastFightTime() <= 15000)
			{
				found = true;
				break;
			}
		}
	}

	return found;
}

bool CPVPManager::CanAttack(LPCHARACTER pkChr, LPCHARACTER pkVictim)
{
	switch (pkVictim->GetCharType())
	{
	case CHAR_TYPE_NPC:
	case CHAR_TYPE_WARP:
	case CHAR_TYPE_GOTO:
		return false;
	}

	if (pkChr == pkVictim)  // 내가 날 칠라고 하네 -_-
		return false;

	if (pkVictim->IsNPC() && pkChr->IsNPC() && !pkChr->IsGuardNPC())
		return false;

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (CCombatZoneManager::Instance().IsCombatZoneMap(pkChr->GetMapIndex()) || CCombatZoneManager::Instance().IsCombatZoneMap(pkVictim->GetMapIndex()))
		return true;
#endif

	if (pkVictim->IsPC() && pkVictim->GetMyShop())
		return false;

	if (true == pkChr->IsHorseRiding())
	{
		if (pkChr->GetHorseLevel() > 0 && 1 == pkChr->GetHorseGrade())
			return false;
	}
	else
	{
		switch (pkChr->GetMountVnum())
		{
		case 0:
		case 20030:
		case 20110:
		case 20111:
		case 20112:
		case 20113:
		case 20114:
		case 20115:
		case 20116:
		case 20117:
		case 20118:
			//신규 탈것 고급
		case 20205:
		case 20206:
		case 20207:
		case 20208:
		case 20209:
		case 20210:
		case 20211:
		case 20212:
		case 20119:		// 라마단 흑마
		case 20219:		// 라마단 흑마 클론 (할로윈용)
		case 20220:		// 크리스마스 탈것
		case 20221:		// 전갑 백웅
		case 20222:		// 전갑 팬더
		case 20120:
		case 20121:
		case 20122:
		case 20123:
		case 20124:
		case 20125:
		case 20214:		// 난폭한 전갑순순록
		case 20215:		// 용맹한 전갑순순록
		case 20217:		// 난폭한 전갑암순록
		case 20218:		// 용맹한 전갑암순록
		case 20224:		// 난폭한 전갑석룡자
		case 20225:		// 용맹한 전갑석룡자
		case 20226:		//	유니콘
		case 20227:
		case 20229:
		case 20230:
		case 20231:
		case 20232:
		case 20233:
		case 20234:
		case 20235:
		case 20236:
		case 20237:
		case 20238:
		case 20239:
		case 20240:
		case 20241:
		case 20242:
		case 20243:
		case 20244:
		case 20245:
		case 20246:
		case 20247:
		case 20248:
		case 20249:
		case 20250:
		case 20251:
		case 20252:
		case 20253:
		case 20254:
		case 20255:
		case 20256:
		case 20257:
		case 20258:
		case 20259:
		case 20530:
		case 20531:
		case 20532:
		case 20533:
		case 20534:
		case 20535:
		case 20536:
		case 20537:
		case 30240:
		case 30241:
		case 30242:
		case 30243:
		case 30244:
		case 20538:
		case 20539:
		case 20540:
		case 20541:
		case 20542:
		case 20543:
		case 20544:
		case 20545:
		case 20546:
		case 20547:
		case 20548:
		case 20549:
		case 20550:
		case 20551:
		case 20552:
		case 20553:
		case 20558:
		case 20559:
			break;

		default:
			return false;
		}
	}

	if (pkVictim->IsNPC() || pkChr->IsNPC())
	{
		return true;
	}

	if (pkVictim->IsObserverMode() || pkChr->IsObserverMode())
		return false;

	{
		BYTE bMapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(pkChr->GetMapIndex());

		if ((pkChr->GetPKMode() == PK_MODE_PROTECT && pkChr->GetEmpire() == bMapEmpire) || (pkVictim->GetPKMode() == PK_MODE_PROTECT && pkVictim->GetEmpire() == bMapEmpire))
		{
			return false;
		}
	}

	if (pkChr->GetEmpire() != pkVictim->GetEmpire())
	{
		// @warme1
		{
			if (pkChr->GetPKMode() == PK_MODE_PROTECT || pkVictim->GetPKMode() == PK_MODE_PROTECT)
			{
				return false;
			}
		}

		return true;
	}

	bool beKillerMode = false;

	if (pkVictim->GetParty() && pkVictim->GetParty() == pkChr->GetParty())
	{
		return false;
		// Cannot attack same party on any pvp model
	}
	else
	{
		if (pkVictim->IsKillerMode())
		{
			return true;
		}

		if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() >= 0)
		{
			if (g_protectNormalPlayer)
			{
				// 범법자는 평화모드인 착한사람을 공격할 수 없다.
				if (PK_MODE_PEACE == pkVictim->GetPKMode())
					return false;
			}
		}

		switch (pkChr->GetPKMode())
		{
		case PK_MODE_PEACE:
		case PK_MODE_REVENGE:
			// Cannot attack same guild
			if (pkVictim->GetGuild() && pkVictim->GetGuild() == pkChr->GetGuild())
				break;

			if (pkChr->GetPKMode() == PK_MODE_REVENGE)
			{
				if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() >= 0)
				{
					pkChr->SetKillerMode(true);
					return true;
				}
				else if (pkChr->GetAlignment() >= 0 && pkVictim->GetAlignment() < 0)
					return true;
			}
			break;

		case PK_MODE_GUILD:
			// Same implementation from PK_MODE_FREE except for attacking same guild
			if (!pkChr->GetGuild() || (pkVictim->GetGuild() != pkChr->GetGuild()))
			{
				if (pkVictim->GetAlignment() >= 0)
				{
					pkChr->SetKillerMode(true);
				}
				else if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() < 0)
				{
					pkChr->SetKillerMode(true);
				}

				return true;
			}
			break;

		case PK_MODE_FREE:
			if (pkVictim->GetAlignment() >= 0)
			{
				pkChr->SetKillerMode(true);
			}
			else if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() < 0)
			{
				pkChr->SetKillerMode(true);
			}
			return true;
			break;
		}
	}

	CPVP kPVP(pkChr->GetPlayerID(), pkVictim->GetPlayerID());
	CPVP* pkPVP = Find(kPVP.m_dwCRC);

	if (!pkPVP || !pkPVP->IsFight())
	{
		if (beKillerMode)
			pkChr->SetKillerMode(true);

		return (beKillerMode);
	}

	pkPVP->SetLastFightTime();
	return true;
}

CPVP* CPVPManager::Find(DWORD dwCRC)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.find(dwCRC);

	if (it == m_map_pkPVP.end())
		return nullptr;

	return it->second;
}

void CPVPManager::Delete(CPVP * pkPVP)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.find(pkPVP->m_dwCRC);

	if (it == m_map_pkPVP.end())
		return;

	m_map_pkPVP.erase(it);
	m_map_pkPVPSetByID[pkPVP->m_players[0].dwPID].erase(pkPVP);
	m_map_pkPVPSetByID[pkPVP->m_players[1].dwPID].erase(pkPVP);

	M2_DELETE(pkPVP);
}

void CPVPManager::SendList(LPDESC d)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.begin();

	DWORD dwVID = d->GetCharacter()->GetVID();

	TPacketGCPVP pack;

	pack.bHeader = HEADER_GC_PVP;

	while (it != m_map_pkPVP.end())
	{
		CPVP* pkPVP = (it++)->second;

		if (!pkPVP->m_players[0].dwVID || !pkPVP->m_players[1].dwVID)
			continue;

		// VID가 둘다 있을 경우에만 보낸다.
		if (pkPVP->IsFight())
		{
			pack.bMode = PVP_MODE_FIGHT;
			pack.dwVIDSrc = pkPVP->m_players[0].dwVID;
			pack.dwVIDDst = pkPVP->m_players[1].dwVID;
		}
		else
		{
			pack.bMode = pkPVP->m_bRevenge ? PVP_MODE_REVENGE : PVP_MODE_AGREE;

			if (pkPVP->m_players[0].bAgree)
			{
				pack.dwVIDSrc = pkPVP->m_players[0].dwVID;
				pack.dwVIDDst = pkPVP->m_players[1].dwVID;
			}
			else
			{
				pack.dwVIDSrc = pkPVP->m_players[1].dwVID;
				pack.dwVIDDst = pkPVP->m_players[0].dwVID;
			}
		}

		d->Packet(&pack, sizeof(pack));
		sys_log(1, "PVPManager::SendList %d %d", pack.dwVIDSrc, pack.dwVIDDst);

		if (pkPVP->m_players[0].dwVID == dwVID)
		{
			LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(pkPVP->m_players[1].dwVID);
			if (ch && ch->GetDesc())
			{
				LPDESC d = ch->GetDesc();
				d->Packet(&pack, sizeof(pack));
			}
		}
		else if (pkPVP->m_players[1].dwVID == dwVID)
		{
			LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(pkPVP->m_players[0].dwVID);
			if (ch && ch->GetDesc())
			{
				LPDESC d = ch->GetDesc();
				d->Packet(&pack, sizeof(pack));
			}
		}
	}
}

void CPVPManager::Process()
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.begin();

	while (it != m_map_pkPVP.end())
	{
		CPVP* pvp = (it++)->second;

		if (get_dword_time() - pvp->GetLastFightTime() > 600000) // 10분 이상 싸움이 없었으면
		{
			pvp->Packet(true);
			Delete(pvp);
		}
	}
}