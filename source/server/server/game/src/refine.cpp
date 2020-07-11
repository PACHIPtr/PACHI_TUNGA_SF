#include "stdafx.h"
#include "refine.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"

CRefineManager::CRefineManager()
{
}

CRefineManager::~CRefineManager()
{
}

bool CRefineManager::Initialize(TRefineTable* table, int size)
{
#ifdef ENABLE_ADVANCED_RELOAD_SYSTEM
	if (!m_map_RefineRecipe.empty())
		m_map_RefineRecipe.clear();
#endif
	for (int i = 0; i < size; ++i, ++table)
	{
		sys_log(0, "REFINE %d prob %d cost %d", table->id, table->prob, table->cost);
		m_map_RefineRecipe.insert(std::make_pair(table->id, *table));
	}

	sys_log(0, "REFINE: COUNT %d", m_map_RefineRecipe.size());
	return true;
}

const TRefineTable* CRefineManager::GetRefineRecipe(DWORD vnum)
{
	if (vnum == 0)
		return nullptr;

	auto it = m_map_RefineRecipe.find(vnum);
	sys_log(0, "REFINE: FIND %u %s", vnum, it == m_map_RefineRecipe.end() ? "FALSE" : "TRUE");

	if (it == m_map_RefineRecipe.end())
	{
		return nullptr;
	}

	return &it->second;
}