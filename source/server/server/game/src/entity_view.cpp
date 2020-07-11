#include "stdafx.h"

#include "utils.h"
#include "char.h"
#include "sectree_manager.h"
#include "config.h"

void CEntity::ViewCleanup()
{
	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = it->first;
		++it;

		entity->ViewRemove(this, false);
	}

	m_map_view.clear();
}

void CEntity::ViewReencode()
{
	if (m_bIsObserver)
		return;

	EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		EncodeRemovePacket(entity);
		if (!m_bIsObserver)
			EncodeInsertPacket(entity);

		if (!entity->m_bIsObserver)
			entity->EncodeInsertPacket(this);
	}
}

void CEntity::ViewInsert(LPENTITY entity, bool recursive)
{
	if (this == entity)
		return;

	ENTITY_MAP::iterator it = m_map_view.find(entity);

	if (m_map_view.end() != it)
	{
		it->second = m_iViewAge;
		return;
	}

	m_map_view.insert(ENTITY_MAP::value_type(entity, m_iViewAge));

	if (!entity->m_bIsObserver)
		entity->EncodeInsertPacket(this);

	if (recursive)
		entity->ViewInsert(this, false);
}

void CEntity::ViewRemove(LPENTITY entity, bool recursive)
{
	ENTITY_MAP::iterator it = m_map_view.find(entity);

	if (it == m_map_view.end())
		return;

	m_map_view.erase(it);

	if (!entity->m_bIsObserver)
		entity->EncodeRemovePacket(this);

	if (recursive)
		entity->ViewRemove(this, false);
}
#include "questmanager.h"
class CFuncViewInsert
{
private:
	int dwViewRange;

public:
	LPENTITY m_me;

	CFuncViewInsert(LPENTITY ent) :
		dwViewRange(VIEW_RANGE + VIEW_BONUS_RANGE),
		m_me(ent)
	{
	}

	void operator () (LPENTITY ent)
	{
		// zRþeÁ§C®°^ lC´N °ÍRþ °L¸®¸¦ °c»eÇDz© °L¸®°^ ¸Ö¸é Aß°^ÇDÁö lE´Â´U.
		if (!ent->IsType(ENTITY_OBJECT))
			if (DISTANCE_APPROX(ent->GetX() - m_me->GetX(), ent->GetY() - m_me->GetY()) > dwViewRange)
				return;
#ifdef SHOP_DISTANCE
		if (ent->IsType(ENTITY_CHARACTER) && m_me->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER chMe = (LPCHARACTER)m_me;
			LPCHARACTER chEnt = (LPCHARACTER)ent;
			DWORD ViewRange = quest::CQuestManager::instance().GetEventFlag("shop_dist");
			if (ViewRange > 0 && DISTANCE_APPROX(ent->GetX() - m_me->GetX(), ent->GetY() - m_me->GetY()) > ViewRange &&
				chMe->IsPC() && chEnt->IsNPC() && chEnt->GetRaceNum() == 30000)
				return;
		}
#endif
		// lÞ¸¦ ´ë»óz^ Aß°^
		m_me->ViewInsert(ent);

		// µN´U Äl¸ZLÍ¸é
		if (ent->IsType(ENTITY_CHARACTER) && m_me->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER chMe = (LPCHARACTER)m_me;
			LPCHARACTER chEnt = (LPCHARACTER)ent;

			// ´ë»óRE NPC¸é StateMachineR» L¸´U.
			if (chMe->IsPC() && !chEnt->IsPC() && !chEnt->IsWarp() && !chEnt->IsGoto())
				chEnt->StartStateMachine();
		}
	}
};

void CEntity::UpdateSectree()
{
	if (!GetSectree())
	{
		if (IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER tch = (LPCHARACTER)this;
			sys_err("nullptr sectree name: %s %d %d", tch->GetName(), GetX(), GetY());
		}

		return;
	}

	++m_iViewAge;

	CFuncViewInsert f(this); // ³ª¸¦ ¼½Æ®¸®¿¡ ÀÖ´Â »ç¶÷µé¿¡°Ô Ãß°¡
	GetSectree()->ForEachAround(f);

	ENTITY_MAP::iterator it, this_it;

	//
	// m_map_view¿¡¼­ ÇÊ¿ä ¾ø´Â ³à¼®µé Áö¿ì±â
	//
	if (m_bObserverModeChange)
	{
		if (m_bIsObserver)
		{
			it = m_map_view.begin();

			while (it != m_map_view.end())
			{
				this_it = it++;
				if (this_it->second < m_iViewAge)
				{
					LPENTITY ent = this_it->first;

					// ³ª·Î ºÎÅÍ »ó´ë¹æÀ» Áö¿î´Ù.
					ent->EncodeRemovePacket(this);
					m_map_view.erase(this_it);

					// »ó´ë·Î ºÎÅÍ ³ª¸¦ Áö¿î´Ù.
					ent->ViewRemove(this, false);
				}
				else
				{
					LPENTITY ent = this_it->first;

					// ³ª·Î ºÎÅÍ »ó´ë¹æÀ» Áö¿î´Ù.
					//ent->EncodeRemovePacket(this);
					//m_map_view.erase(this_it);

					// »ó´ë·Î ºÎÅÍ ³ª¸¦ Áö¿î´Ù.
					//ent->ViewRemove(this, false);
					EncodeRemovePacket(ent);
				}
			}
		}
		else
		{
			it = m_map_view.begin();

			while (it != m_map_view.end())
			{
				this_it = it++;

				if (this_it->second < m_iViewAge)
				{
					LPENTITY ent = this_it->first;

					// ³ª·Î ºÎÅÍ »ó´ë¹æÀ» Áö¿î´Ù.
					ent->EncodeRemovePacket(this);
					m_map_view.erase(this_it);

					// »ó´ë·Î ºÎÅÍ ³ª¸¦ Áö¿î´Ù.
					ent->ViewRemove(this, false);
				}
				else
				{
					LPENTITY ent = this_it->first;
					ent->EncodeInsertPacket(this);
					EncodeInsertPacket(ent);

					ent->ViewInsert(this, true);
				}
			}
		}

		m_bObserverModeChange = false;
	}
	else
	{
		if (!m_bIsObserver)
		{
			it = m_map_view.begin();

			while (it != m_map_view.end())
			{
				this_it = it++;

				if (this_it->second < m_iViewAge)
				{
					LPENTITY ent = this_it->first;

					// ³ª·Î ºÎÅÍ »ó´ë¹æÀ» Áö¿î´Ù.
					ent->EncodeRemovePacket(this);
					m_map_view.erase(this_it);

					// »ó´ë·Î ºÎÅÍ ³ª¸¦ Áö¿î´Ù.
					ent->ViewRemove(this, false);
				}
			}
		}
	}
}