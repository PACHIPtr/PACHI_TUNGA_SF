#pragma once

#ifdef ENABLE_ACCE_SYSTEM
#include "Packet.h"

class CPythonSash : public CSingleton<CPythonSash>
{
	public:
		DWORD	dwPrice;
		typedef std::vector<TSashMaterial> TSashMaterials;
	
	public:
		CPythonSash();
		virtual ~CPythonSash();
		
		void	Clear();
		void	AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos);
		void	AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs);
		void	RemoveMaterial(DWORD dwRefPrice, BYTE bPos);
		DWORD	GetPrice() {return dwPrice;}
		bool	GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell);
		void	GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);
	
	protected:
		TSashResult	m_vSashResult;
		TSashMaterials	m_vSashMaterials;
};
#endif
