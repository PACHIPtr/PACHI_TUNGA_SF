#pragma once

class CPythonAura : public CSingleton<CPythonAura>
{
	public:
		DWORD	dwPrice;
		typedef std::vector<TAuraMaterial> TAuraMaterials;
		
	public:
		CPythonAura();
		virtual ~CPythonAura();
		
		void	Clear();
		void	AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos);
		void	AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs);
		void	RemoveMaterial(DWORD dwRefPrice, BYTE bPos);
		DWORD	GetPrice() {return dwPrice;}
		bool	GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell);
		void	GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);
	
	protected:
		TAuraResult	m_vAuraResult;
		TAuraMaterials	m_vAuraMaterials;
};