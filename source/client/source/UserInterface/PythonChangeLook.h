#pragma once

#ifdef ENABLE_CHANGELOOK_SYSTEM
#include "Packet.h"

class CPythonChangeLook : public CSingleton<CPythonChangeLook>
{
	public:
		DWORD	dwCost;
		typedef std::vector<TChangeLookMaterial> TChangeLookMaterials;
	
	public:
		CPythonChangeLook();
		virtual ~CPythonChangeLook();
		
		void	Clear();
		void	AddMaterial(BYTE bPos, TItemPos tPos);
		void	RemoveMaterial(BYTE bPos);
		void	RemoveAllMaterials();
		DWORD	GetCost() {return dwCost;}
		void	SetCost(DWORD dwCostR) {dwCost = dwCostR;}
		bool	GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell);
	
	protected:
		TChangeLookMaterials	m_vMaterials;
};
#endif
