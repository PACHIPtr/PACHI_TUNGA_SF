#include "stdafx.h"
#include "constants.h"
#include "fishing.h"
#include "locale_service.h"

#include "item_manager.h"

#include "config.h"
#include "packet.h"

#include "sectree_manager.h"
#include "char.h"
#include "char_manager.h"

#include "log.h"

#include "questmanager.h"
#include "buffer_manager.h"
#include "desc_client.h"
#include "locale_service.h"

#include "affect.h"
#include "unique_item.h"

namespace fishing
{
	enum
	{
		MAX_FISH = 37,
		NUM_USE_RESULT_COUNT = 10, // 1 : DEAD 2 : BONE 3 ~ 12 : rest
		FISH_BONE_VNUM = 27799,
		SHELLFISH_VNUM = 27987,
		EARTHWORM_VNUM = 27801,
		MAX_PROB = 4,
	};

	enum EUsedFishing
	{
		USED_NONE,
		USED_SHELLFISH,
		USED_WATER_STONE,
		USED_TREASURE_MAP,
		USED_EARTHWARM,
	};

	int aFishingTime[5][31] =
	{
		{   0,   0,   0,   0,   0,   2,   4,   8,  12,  16,  20,  22,  25,  30,  50,  80,  50,  30,  25,  22,  20,  16,  12,   8,   4,   2,   0,   0,   0,   0,   0 },
		{   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   8,  12,  16,  20,  22,  25,  30,  50,  80,  50,  30,  25,  22,  20 },
		{  20,  22,  25,  30,  50,  80,  50,  30,  25,  22,  20,  16,  12,   8,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 },
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
		{  20,  20,  20,  20,  20,  22,  24,  28,  32,  36,  40,  42,  45,  50,  70, 100,  70,  50,  45,  42,  40,  36,  32,  28,  24,  22,  20,  20,  20,  20,  20 },
	};

	struct SFishInfo
	{
		char name[64];

		DWORD vnum;
		DWORD dead_vnum;
		DWORD grill_vnum;

		int prob[MAX_PROB];
		int difficulty;

		int time_type;
		int length_range[3]; // MIN MAX EXTRA_MAX : 99% MIN~MAX, 1% MAX~EXTRA_MAX

		int used_table[NUM_USE_RESULT_COUNT];
		// 6000 2000 1000 500 300 100 50 30 10 5 4 1
	};

	bool operator < (const SFishInfo& lhs, const SFishInfo& rhs)
	{
		return lhs.vnum < rhs.vnum;
	}

	int g_prob_sum[MAX_PROB];
	int g_prob_accumulate[MAX_PROB][MAX_FISH];

	SFishInfo fish_info[MAX_FISH] = { { "\0", }, };

	void Initialize()
	{
		SFishInfo fish_info_bak[MAX_FISH];
		thecore_memcpy(fish_info_bak, fish_info, sizeof(fish_info));

		memset(fish_info, 0, sizeof(fish_info));

		// LOCALE_SERVICE
		const int FILE_NAME_LEN = 256;
		char szFishingFileName[FILE_NAME_LEN + 1];
		snprintf(szFishingFileName, sizeof(szFishingFileName),
			"%s/fishing.txt", LocaleService_GetBasePath().c_str());
		FILE* fp = fopen(szFishingFileName, "r");
		// END_OF_LOCALE_SERVICE

		if (*fish_info_bak[0].name)
			sys_err("Reloading fish table.");

		if (!fp)
		{
			SendLog("error! cannot open fishing.txt");

			// 백업에 이름이 있으면 리스토어 한다.
			if (*fish_info_bak[0].name)
			{
				thecore_memcpy(fish_info, fish_info_bak, sizeof(fish_info));
				sys_err("  restoring to backup");
			}
			return;
		}

		memset(fish_info, 0, sizeof(fish_info));

		char buf[512];
		int idx = 0;

		while (fgets(buf, 512, fp))
		{
			if (*buf == '#')
				continue;

			char* p = strrchr(buf, '\n');
			*p = '\0';

			const char* start = buf;
			const char* tab = strchr(start, '\t');

			if (!tab)
			{
				printf("Tab error on line: %s\n", buf);
				sys_err("error! parsing fishing.txt");

				if (*fish_info_bak[0].name)
				{
					thecore_memcpy(fish_info, fish_info_bak, sizeof(fish_info));
					sys_err("  restoring to backup");
				}
				break;
			}

			char szCol[256], szCol2[256];
			int iColCount = 0;

			do
			{
				strlcpy(szCol2, start, MIN(sizeof(szCol2), (tab - start) + 1));
				szCol2[tab - start] = '\0';

				trim_and_lower(szCol2, szCol, sizeof(szCol));

				if (!*szCol || *szCol == '\t')
					iColCount++;
				else
				{
					switch (iColCount++)
					{
					case 0: strlcpy(fish_info[idx].name, szCol, sizeof(fish_info[idx].name)); break;
					case 1: str_to_number(fish_info[idx].vnum, szCol); break;
					case 2: str_to_number(fish_info[idx].dead_vnum, szCol); break;
					case 3: str_to_number(fish_info[idx].grill_vnum, szCol); break;
					case 4: str_to_number(fish_info[idx].prob[0], szCol); break;
					case 5: str_to_number(fish_info[idx].prob[1], szCol); break;
					case 6: str_to_number(fish_info[idx].prob[2], szCol); break;
					case 7: str_to_number(fish_info[idx].prob[3], szCol); break;
					case 8: str_to_number(fish_info[idx].difficulty, szCol); break;
					case 9: str_to_number(fish_info[idx].time_type, szCol); break;
					case 10: str_to_number(fish_info[idx].length_range[0], szCol); break;
					case 11: str_to_number(fish_info[idx].length_range[1], szCol); break;
					case 12: str_to_number(fish_info[idx].length_range[2], szCol); break;
					case 13: // 0
					case 14: // 1
					case 15: // 2
					case 16: // 3
					case 17: // 4
					case 18: // 5
					case 19: // 6
					case 20: // 7
					case 21: // 8
					case 22: // 9
						str_to_number(fish_info[idx].used_table[iColCount - 1 - 12], szCol);
						break;
					}
				}

				start = tab + 1;
				tab = strchr(start, '\t');
			} while (tab);

			idx++;

			if (idx == MAX_FISH)
				break;
		}

		fclose(fp);
		/*
			for (int i = 0; i < MAX_FISH; ++i)
			{
				sys_log(0, "FISH: %-24s vnum %5lu prob %4d %4d %4d %4d len %d %d %d",
						fish_info[i].name,
						fish_info[i].vnum,
						fish_info[i].prob[0],
						fish_info[i].prob[1],
						fish_info[i].prob[2],
						fish_info[i].prob[3],
						fish_info[i].length_range[0],
						fish_info[i].length_range[1],
						fish_info[i].length_range[2]);
			}
		*/
		// 확률 계산
		for (int j = 0; j < MAX_PROB; ++j)
		{
			g_prob_accumulate[j][0] = fish_info[0].prob[j];

			for (int i = 1; i < MAX_FISH; ++i)
				g_prob_accumulate[j][i] = fish_info[i].prob[j] + g_prob_accumulate[j][i - 1];

			g_prob_sum[j] = g_prob_accumulate[j][MAX_FISH - 1];
			//sys_log(0, "FISH: prob table %d %d", j, g_prob_sum[j]);
		}
	}

	int DetermineFishByProbIndex(int prob_idx)
	{
		int rv = number(1, g_prob_sum[prob_idx]);
		int* p = std::lower_bound(g_prob_accumulate[prob_idx], g_prob_accumulate[prob_idx] + MAX_FISH, rv);
		int fish_idx = p - g_prob_accumulate[prob_idx];
		return fish_idx;
	}

	int GetProbIndexByMapIndex(int index)
	{
		if (index > 60)
			return -1;

		switch (index)
		{
		case 1:
		case 21:
		case 41:
			return 0;

		case 3:
		case 23:
		case 43:
			return 1;
		}

		return -1;
	}

	int DetermineFish(LPCHARACTER ch)
	{
		int map_idx = ch->GetMapIndex();
		int prob_idx = GetProbIndexByMapIndex(map_idx);

		if (prob_idx < 0)
			return 0;

		// ADD_PREMIUM
		if (ch->GetPremiumRemainSeconds(PREMIUM_FISH_MIND) > 0 ||
			ch->IsEquipUniqueGroup(UNIQUE_GROUP_FISH_MIND)) // 월간어심 착용시 고급 물고기 확률 상승
		{
			if (quest::CQuestManager::instance().GetEventFlag("manwoo") != 0)
				prob_idx = 3;
			else
				prob_idx = 2;
		}
		// END_OF_ADD_PREMIUM

		int adjust = 0;
		if (quest::CQuestManager::instance().GetEventFlag("fish_miss_pct") != 0)
		{
			int fish_pct_value = MINMAX(0, quest::CQuestManager::instance().GetEventFlag("fish_miss_pct"), 200);
			adjust = (100 - fish_pct_value) * fish_info[0].prob[prob_idx] / 100;
		}

		int rv = number(adjust + 1, g_prob_sum[prob_idx]);

		int* p = std::lower_bound(g_prob_accumulate[prob_idx], g_prob_accumulate[prob_idx] + MAX_FISH, rv);
		int fish_idx = p - g_prob_accumulate[prob_idx];

		// 중국에서는 금덩어리, 금열쇠, 은열쇠 나오지 않게 함
		{
			DWORD vnum = fish_info[fish_idx].vnum;

			if (vnum == 50008 || vnum == 50009 || vnum == 80008)
				return 0;
		}

		return (fish_idx);
	}

	void FishingReact(LPCHARACTER ch)
	{
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_REACT;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingSuccess(LPCHARACTER ch)
	{
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_SUCCESS;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingFail(LPCHARACTER ch)
	{
		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_FAIL;
		p.info = ch->GetVID();
		ch->PacketAround(&p, sizeof(p));
	}

	void FishingPractice(LPCHARACTER ch)
	{
		LPITEM rod = ch->GetWear(WEAR_WEAPON);
		if (rod && rod->GetType() == ITEM_ROD)
		{
			// 최대 수련도가 아닌 경우 낚시대 수련
			if (rod->GetRefinedVnum() > 0 && rod->GetSocket(0) < rod->GetValue(2) && number(1, rod->GetValue(1)) == 1)
			{
				rod->SetSocket(0, rod->GetSocket(0) + 1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시대의 수련도가 증가하였습니다! (%d/%d)"), rod->GetSocket(0), rod->GetValue(2));
				if (rod->GetSocket(0) == rod->GetValue(2))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시대가 최대 수련도에 도달하였습니다."));
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("어부를 통해 다음 레벨의 낚시대로 업그레이드 할 수 있습니다."));
				}
			}
		}
		if (rod)
			rod->SetSocket(2, 0);
	}

	bool PredictFish(LPCHARACTER ch)
	{
		// ADD_PREMIUM
		// 어심환
		if (ch->FindAffect(AFFECT_FISH_MIND_PILL) ||
			ch->GetPremiumRemainSeconds(PREMIUM_FISH_MIND) > 0 ||
			ch->IsEquipUniqueGroup(UNIQUE_GROUP_FISH_MIND))
			return true;
		// END_OF_ADD_PREMIUM

		return false;
	}

	EVENTFUNC(fishing_event)
	{
		fishing_event_info* info = dynamic_cast<fishing_event_info*>(event->info);

		if (info == nullptr)
		{
			sys_err("fishing_event> <Factor> nullptr pointer");
			return 0;
		}

		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->pid);

		if (!ch)
			return 0;

		LPITEM rod = ch->GetWear(WEAR_WEAPON);

		if (!(rod && rod->GetType() == ITEM_ROD))
		{
			ch->m_pkFishingEvent = nullptr;
			return 0;
		}

		switch (info->step)
		{
		case 0:	// 흔들리기 또는 떡밥만 날아감
			++info->step;

			//info->ch->Motion(MOTION_FISHING_SIGN);
			info->hang_time = get_dword_time();
			info->fish_id = DetermineFish(ch);
			FishingReact(ch);

			if (PredictFish(ch))
			{
				TPacketGCFishing p;
				p.header = HEADER_GC_FISHING;
				p.subheader = FISHING_SUBHEADER_GC_FISH;
				p.info = fish_info[info->fish_id].vnum;
				ch->GetDesc()->Packet(&p, sizeof(TPacketGCFishing));
			}
			return (PASSES_PER_SEC(6));

		default:
			++info->step;

			if (info->step > 5)
				info->step = 5;

			ch->m_pkFishingEvent = nullptr;
			FishingFail(ch);
			rod->SetSocket(2, 0);
			return 0;
		}
	}

	LPEVENT CreateFishingEvent(LPCHARACTER ch)
	{
		fishing_event_info* info = AllocEventInfo<fishing_event_info>();
		info->pid = ch->GetPlayerID();
		info->step = 0;
		info->hang_time = 0;

		int time = number(10, 40);

		TPacketGCFishing p;
		p.header = HEADER_GC_FISHING;
		p.subheader = FISHING_SUBHEADER_GC_START;
		p.info = ch->GetVID();
		p.dir = (BYTE)(ch->GetRotation() / 5);
		ch->PacketAround(&p, sizeof(TPacketGCFishing));

		return event_create(fishing_event, info, PASSES_PER_SEC(time));
	}

	int GetFishingLevel(LPCHARACTER ch)
	{
		LPITEM rod = ch->GetWear(WEAR_WEAPON);

		if (!rod || rod->GetType() != ITEM_ROD)
			return 0;

		return rod->GetSocket(2) + rod->GetValue(0);
	}

	int Compute(DWORD fish_id, DWORD ms, DWORD * item, int level)
	{
		if (fish_id == 0)
			return -2;

		if (fish_id >= MAX_FISH)
		{
			sys_err("Wrong FISH ID : %d", fish_id);
			return -2;
		}

		if (ms > 6000)
			return -1;

		int time_step = MINMAX(0, ((ms + 99) / 200), 31 - 1);
		if (number(1, 100) <= aFishingTime[fish_info[fish_id].time_type][time_step])
		{
			if (number(1, fish_info[fish_id].difficulty) <= level)
			{
				*item = fish_info[fish_id].vnum;
				return 0;
			}
			return -3;
		}

		return -1;
	}

	int GetFishLength(int fish_id)
	{
		if (number(0, 99))
		{
			// 99% : normal size
			return (int)(fish_info[fish_id].length_range[0] +
				(fish_info[fish_id].length_range[1] - fish_info[fish_id].length_range[0])
				* (number(0, 2000) + number(0, 2000) + number(0, 2000) + number(0, 2000) + number(0, 2000)) / 10000);
		}
		else
		{
			// 1% : extra LARGE size
			return (int)(fish_info[fish_id].length_range[1] +
				(fish_info[fish_id].length_range[2] - fish_info[fish_id].length_range[1])
				* 2 * asin(number(0, 10000) / 10000.) / M_PI);
		}
	}

	void Take(fishing_event_info * info, LPCHARACTER ch)
	{
#ifdef ENABLE_FISHBOT_FIX
		if (info->step == 1)	// 고기가 걸린 상태면..
		{
			int32_t ms = static_cast<int32_t>((get_dword_time() - info->hang_time));
			uint32_t item_vnum = 0;
			int32_t ret = Compute(info->fish_id, ms, &item_vnum, GetFishingLevel(ch));

			if (!ch->GetLastFishTime())
				ch->SetLastFishTime(ms);
			else {
				if (ch->GetFishHackCount() == 3) {
					ch->ResetFishHackCount();
					LogManager::instance().HackLog("FISHBOT", ch);
					ch->GetDesc()->DelayedDisconnect(3);
				}
				else {
					if ((ms == ch->GetLastFishTime()) || (abs(ch->GetLastFishTime() - ms) < 70) || (abs(ms - ch->GetLastFishTime()) < 70))
						ch->UpdateFishHackCount();
					else
						ch->ResetFishHackCount();
				}
			}

			if (test_server)
				ch->ChatPacket(CHAT_TYPE_INFO, "/HACK-TEST/ Last: %d Now: %d HackCount: %d", ch->GetLastFishTime(), ms, ch->GetFishHackCount());
#else
		if (info->step == 1)	// 고기가 걸린 상태면..
		{
			long ms = (long)((get_dword_time() - info->hang_time));
			DWORD item_vnum = 0;
			int ret = Compute(info->fish_id, ms, &item_vnum, GetFishingLevel(ch));

			//if (test_server)
			//ch->ChatPacket(CHAT_TYPE_INFO, "%d ms", ms);
#endif

			switch (ret)
			{
			case -2: // 잡히지 않은 경우
			case -3: // 난이도 때문에 실패
			case -1: // 시간 확률 때문에 실패
				//ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("고기가 미끼만 빼먹고 잽싸게 도망칩니다."));
			{
				int map_idx = ch->GetMapIndex();
				int prob_idx = GetProbIndexByMapIndex(map_idx);

				LogManager::instance().FishLog(
					ch->GetPlayerID(),
					prob_idx,
					info->fish_id,
					GetFishingLevel(ch),
					ms);
			}
			FishingFail(ch);
			break;

			case 0:
				//ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("고기가 잡혔습니다! (%s)"), fish_info[info->fish_id].name);
				if (item_vnum)
				{
					FishingSuccess(ch);

#ifdef ENABLE_FISH_JIGSAW_EVENT
					if (quest::CQuestManager::instance().GetEventFlag("2018_fish_event"))
					{
						if (ch->IsEquipUniqueItem(UNIQUE_ITEM_FISH_MIND))
						{
							int r = number(1, 100);
							if (r <= 10)
								ch->AutoGiveItem(ITEM_FISH_EVENT_BOX_SPECIAL, 1, -1, false);
							else
								ch->AutoGiveItem(ITEM_FISH_EVENT_BOX, 5, -1, false);
						}
						else
						{
							ch->AutoGiveItem(ITEM_FISH_EVENT_BOX, 5, -1, false);
						}
					}
#endif

					TPacketGCFishing p;
					p.header = HEADER_GC_FISHING;
					p.subheader = FISHING_SUBHEADER_GC_FISH;
					p.info = item_vnum;
					ch->GetDesc()->Packet(&p, sizeof(TPacketGCFishing));

					LPITEM item = ch->AutoGiveItem(item_vnum, 1, -1, false);
					if (item)
					{
						item->SetSocket(0, GetFishLength(info->fish_id));
						if (test_server)
						{
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이번에 잡은 물고기의 길이는 %.2fcm"), item->GetSocket(0) / 100.f);
						}

						if (quest::CQuestManager::instance().GetEventFlag("fishevent") > 0 && (info->fish_id == 5 || info->fish_id == 6))
						{
							// 이벤트 중이므로 기록한다.

							TPacketGDHighscore p;
							p.dwPID = ch->GetPlayerID();
							p.lValue = item->GetSocket(0);

							if (info->fish_id == 5)
							{
								strlcpy(p.szBoard, LC_TEXT("낚시이벤트월척붕어"), sizeof(p.szBoard));
							}
							else if (info->fish_id == 6)
							{
								strlcpy(p.szBoard, LC_TEXT("낚시이벤트잉어"), sizeof(p.szBoard));
							}

							db_clientdesc->DBPacket(HEADER_GD_HIGHSCORE_REGISTER, 0, &p, sizeof(TPacketGDHighscore));
						}
					}

					int map_idx = ch->GetMapIndex();
					int prob_idx = GetProbIndexByMapIndex(map_idx);

					LogManager::instance().FishLog(
						ch->GetPlayerID(),
						prob_idx,
						info->fish_id,
						GetFishingLevel(ch),
						ms,
						true,
						item ? item->GetSocket(0) : 0);
				}
				else
				{
					int map_idx = ch->GetMapIndex();
					int prob_idx = GetProbIndexByMapIndex(map_idx);

					LogManager::instance().FishLog(
						ch->GetPlayerID(),
						prob_idx,
						info->fish_id,
						GetFishingLevel(ch),
						ms);
					FishingFail(ch);
				}
				break;
			}
		}
		else if (info->step > 1)
		{
			int map_idx = ch->GetMapIndex();
			int prob_idx = GetProbIndexByMapIndex(map_idx);

			LogManager::instance().FishLog(
				ch->GetPlayerID(),
				prob_idx,
				info->fish_id,
				GetFishingLevel(ch),
				7000);
			//ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("고기가 미끼만 빼먹고 잽싸게 도망칩니다."));
			FishingFail(ch);
		}
		else
		{
			TPacketGCFishing p;
			p.header = HEADER_GC_FISHING;
			p.subheader = FISHING_SUBHEADER_GC_STOP;
			p.info = ch->GetVID();
			ch->PacketAround(&p, sizeof(p));
		}

		if (info->step)
		{
			FishingPractice(ch);
		}
		//Motion(MOTION_FISHING_PULL);
	}

	void Simulation(int level, int count, int prob_idx, LPCHARACTER ch)
	{
		std::map<std::string, int> fished;
		int total_count = 0;

		for (int i = 0; i < count; ++i)
		{
			int fish_id = DetermineFishByProbIndex(prob_idx);
			DWORD item = 0;
			Compute(fish_id, (number(2000, 4000) + number(2000, 4000)) / 2, &item, level);

			if (item)
			{
				fished[fish_info[fish_id].name]++;
				total_count++;
			}
		}

		for (std::map<std::string, int>::iterator it = fished.begin(); it != fished.end(); ++it)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s : %d 마리"), it->first.c_str(), it->second);

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d 종류 %d 마리 낚음"), fished.size(), total_count);
	}

	void UseFish(LPCHARACTER ch, LPITEM item)
	{
		int idx = item->GetVnum() - fish_info[2].vnum + 2;

		// 피라미 사용불가, 살아있는게 아닌건 사용불가

		if (idx <= 1 || idx >= MAX_FISH)
			return;

		int r = number(1, 10000);

		item->SetCount(item->GetCount() - 1);

		if (r >= 4001)
		{
			// 죽은 물고기
			ch->AutoGiveItem(fish_info[idx].dead_vnum);
		}
		else if (r >= 2001)
		{
			// 생선뼈
			ch->AutoGiveItem(FISH_BONE_VNUM);
		}
		else
		{
			static int s_acc_prob[NUM_USE_RESULT_COUNT] =
			{
				1000,
				1500,
				1800,
				1900,
				1950,
				1980,
				1990,
				1995,
				1999,
				2000,
			};

			int u_index = std::lower_bound(s_acc_prob, s_acc_prob + NUM_USE_RESULT_COUNT, r) - s_acc_prob;

			switch (fish_info[idx].used_table[u_index])
			{
			case USED_TREASURE_MAP:	// 3
			case USED_NONE:		// 0
			case USED_WATER_STONE:	// 2
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("고기가 흔적도 없이 사라집니다."));
				break;

			case USED_SHELLFISH:	// 1
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("배 속에서 조개가 나왔습니다."));
				ch->AutoGiveItem(SHELLFISH_VNUM);
				break;

			case USED_EARTHWARM:	// 4
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("배 속에서 지렁이가 나왔습니다."));
				ch->AutoGiveItem(EARTHWORM_VNUM);
				break;

			default:
				ch->AutoGiveItem(fish_info[idx].used_table[u_index]);
				break;
			}
		}
	}

	void Grill(LPCHARACTER ch, LPITEM item)
	{
		/*if (item->GetVnum() < fish_info[3].vnum)
		  return;
		  int idx = item->GetVnum()-fish_info[3].vnum+3;
		  if (idx>=MAX_FISH)
		  idx = item->GetVnum()-fish_info[3].dead_vnum+3;
		  if (idx>=MAX_FISH)
		  return;*/
		int idx = -1;
		DWORD vnum = item->GetVnum();
		if (vnum >= 27803 && vnum <= 27830)
			idx = vnum - 27800;
		if (vnum >= 27833 && vnum <= 27860)
			idx = vnum - 27830;
		if (idx == -1)
			return;

		int count = item->GetCount();

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s를 구웠습니다."), item->GetName());
		item->SetCount(0);
		ch->AutoGiveItem(fish_info[idx].grill_vnum, count);
	}

	bool RefinableRod(LPITEM rod)
	{
		if (rod->GetType() != ITEM_ROD)
			return false;

		if (rod->IsEquipped())
			return false;

		return (rod->GetSocket(0) == rod->GetValue(2));
	}

	int RealRefineRod(LPCHARACTER ch, LPITEM item)
	{
		if (!ch || !item)
			return 2;

		// REFINE_ROD_HACK_BUG_FIX
		if (!RefinableRod(item))
		{
			sys_err("REFINE_ROD_HACK pid(%u) item(%s:%d)", ch->GetPlayerID(), item->GetName(), item->GetID());

			LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), -1, 1, "ROD_HACK");

			return 2;
		}
		// END_OF_REFINE_ROD_HACK_BUG_FIX

		LPITEM rod = item;

		int iAdv = rod->GetValue(0) / 10;

		if (number(1, 100) <= rod->GetValue(3))
		{
			LogManager::instance().RefineLog(ch->GetPlayerID(), rod->GetName(), rod->GetID(), iAdv, 1, "ROD");

			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(rod->GetRefinedVnum(), 1);

			if (pkNewItem)
			{
				UINT bCell = rod->GetCell();

				// 낚시대 개량 성공
				ITEM_MANAGER::instance().RemoveItem(rod, "REMOVE (REFINE FISH_ROD)");
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, bCell));
				LogManager::instance().ItemLog(ch, pkNewItem, "REFINE FISH_ROD SUCCESS", pkNewItem->GetName());
				return 1;
			}

			// 낚시대 개량 실패
			return 2;
		}
		else
		{
			LogManager::instance().RefineLog(ch->GetPlayerID(), rod->GetName(), rod->GetID(), iAdv, 0, "ROD");

			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(rod->GetValue(4), 1);
			if (pkNewItem)
			{
				UINT bCell = rod->GetCell();

				// 낚시대 개량에 성공
				ITEM_MANAGER::instance().RemoveItem(rod, "REMOVE (REFINE FISH_ROD)");
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, bCell));
				LogManager::instance().ItemLog(ch, pkNewItem, "REFINE FISH_ROD FAIL", pkNewItem->GetName());
				return 0;
			}
			return 2;
		}
	}
}