#include "stdafx.h"
#include "questmanager.h"
#include "sectree_manager.h"
#include "char.h"

namespace quest
{
	struct FWarpToHome
	{
		void operator() (LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER)ent;

				if (ch->IsPC() == true && ch->IsGM() != true)
				{
					if (((ch->GetX() >= 764503 && ch->GetX() <= 772362) && (ch->GetY() >= 22807 && ch->GetY() <= 26499)) == false)
					{
						ch->GoHome();
					}
				}
			}
		};
	};

	ALUA(dance_event_go_home)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(115);

		if (pSecMap != nullptr)
		{
			FWarpToHome f;
			pSecMap->for_each(f);
		}

		return 0;
	}

	void RegisterDanceEventFunctionTable()
	{
		luaL_reg dance_event_functions[] =
		{
			{"gohome", dance_event_go_home},
			{nullptr, nullptr}
		};

		CQuestManager::instance().AddLuaFunctionTable("dance_event", dance_event_functions);
	}
}