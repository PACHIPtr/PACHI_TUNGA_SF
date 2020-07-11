#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "questmanager.h"
#include "sectree_manager.h"
#include "char.h"
#include "char_manager.h"
#include "packet.h"
#include "nearby_scanner.h"

/////////////////////////////////////////////////////////////////////
// Packet
/////////////////////////////////////////////////////////////////////
CNearbyScanner::CNearbyScanner() : m_iID(0)
{
}

CNearbyScanner::~CNearbyScanner()
{
}

EVENTFUNC(scanner_event)
{
	ScannerInfo* info = dynamic_cast<ScannerInfo*>(event->info);

	if (!info)
	{
		sys_err("scanner_event> <Factor> nullptr pointer");
		return 0;
	}

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().Find(info->dwVID);
	if (!pkChr) {
		return 0;
	}

	struct FindTarget
	{
		LPCHARACTER m_ch;
		FindTarget(LPCHARACTER ch) {
			m_ch = ch;
		}

		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;
				if (ch->IsPC())
				{
					//Let's see the distance
					if (DISTANCE_APPROX(m_ch->GetX() - ch->GetX(), m_ch->GetY() - ch->GetY()) > 1000)
						return;

					DWORD vid = m_ch->GetVID();
					ScannerInfo * info = CNearbyScanner::instance().GetScannerInfo(vid); //Find the running event
					if (!info)
						return; //Already finished

					info->found(info->dwVID, ch->GetPlayerID()); //Exec callback
					CNearbyScanner::instance().EndScanner(vid); //Finish
					return;
				}
			}

			return;
		}
	};

	FindTarget f(pkChr);
	pkChr->GetSectree()->ForEachAround(f);

	return passes_per_sec;
}

void CNearbyScanner::BeginScanner(DWORD dwVID, void(*found) (DWORD, DWORD))
{
	sys_log(0, "StartScannerCreation: vid %lu", dwVID);

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().Find(dwVID);

	if (!pkChr)
	{
		sys_err("Cannot find character ptr by VID %lu", dwVID);
		return;
	}

	if (m_map_kListEvent.find(dwVID) != m_map_kListEvent.end())
	{
		//TODO: Support several scanners (with different actions of course).
		//Only requirement: different actions when triggered.
		sys_err("Attempted creation of duplicated sourrounding scanner for VID %lu", dwVID);
		return;
	}

	ScannerInfo* newInfo = AllocEventInfo<ScannerInfo>();
	newInfo->iID = ++m_iID;
	newInfo->dwVID = dwVID;
	newInfo->found = found;

	LPEVENT event = event_create(scanner_event, newInfo, PASSES_PER_SEC(1));

	if (event)
		m_map_kListEvent[dwVID].push_back(event);
}

void CNearbyScanner::EndScanner(DWORD dwVID)
{
	const auto it = m_map_kListEvent.find(dwVID);
	if (it == m_map_kListEvent.end())
		return;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end()) //Iterate over the list of events
	{
		LPEVENT pkEvent = *it2;
		ScannerInfo* info = dynamic_cast<ScannerInfo*>(pkEvent->info);

		if (!info)
		{
			sys_err("nullptr pointer when trying to end the ScannerInfo for vid ");
			++it2;
			continue;
		}

		event_cancel(&pkEvent);
		it2 = it->second.erase(it2);
	}
}

LPEVENT CNearbyScanner::GetScannerEvent(DWORD dwVID)
{
	const auto it = m_map_kListEvent.find(dwVID);
	if (it == m_map_kListEvent.end())
		return nullptr;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end()) //TODO: This should take some sort of index. Looping and returning always the first is kinda pointless, and is just here to support lists
	{
		LPEVENT pkEvent = *(it2++);
		ScannerInfo* info = dynamic_cast<ScannerInfo*>(pkEvent->info);

		if (!info)
		{
			sys_err("No scanner info when fetching event for vid %lu", dwVID);
			continue;
		}

		return pkEvent;
	}

	return nullptr;
}

ScannerInfo* CNearbyScanner::GetScannerInfo(DWORD dwVID)
{
	const auto it = m_map_kListEvent.find(dwVID);
	if (it == m_map_kListEvent.end())
		return nullptr;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end()) //TODO: take index to not always return the first info
	{
		LPEVENT pkEvent = *(it2++);
		ScannerInfo* info = dynamic_cast<ScannerInfo*>(pkEvent->info);

		if (!info)
		{
			sys_err("No scanner info when fetching info for vid %lu", dwVID);
			continue;
		}

		return info;
	}

	return nullptr;
}

void CNearbyScanner::Die(DWORD dwVID)
{
	const auto it = m_map_kListEvent.find(dwVID);

	if (it == m_map_kListEvent.end())
		return;

	std::list<LPEVENT>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
		event_cancel(&(*(it2++)));

	m_map_kListEvent.erase(it);
}