#pragma once

#include "ItemData.h"

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,						
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};
		
#ifdef ENABLE_ACCE_SYSTEM
		enum EItemScaleColumn
		{
			ITEMSCALE_VNUM,
			ITEMSCALE_JOB,
			ITEMSCALE_SEX,
			ITEMSCALE_SCALE_X,
			ITEMSCALE_SCALE_Y,
			ITEMSCALE_SCALE_Z,
			ITEMSCALE_POSITION_X,
			ITEMSCALE_POSITION_Y,
			ITEMSCALE_POSITION_Z,
			ITEMSCALE_NUM,
		};
#endif

	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;
		
		typedef struct SBlendInfo {
			DWORD	dwVnum;
			BYTE	bApplyType;
			std::vector<int>	vec_iApplyValue;
			std::vector<int>	vec_iApplyDuration;
		} TBlendInfo;
		typedef std::map<DWORD, TBlendInfo> TBlendMap;

	public:
		CItemManager();
		virtual ~CItemManager();
		
		void			Destroy();
		TItemMap		GetItems() const { return m_ItemMap; }

		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();

		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);
		BOOL			GetBlendInfoPointer(DWORD dwItemID, TBlendInfo ** ppBlendInfo);
		
		BYTE			GetAttrTypeByName(const std::string& stAttrType);
		/////
		bool			LoadItemDesc(const char* c_szFileName);
		bool			LoadItemList(const char* c_szFileName);
		bool			LoadItemTable(const char* c_szFileName);
		bool			LoadItemBlend(const char* c_szFileName);
		CItemData *		MakeItemData(DWORD dwIndex);
#ifdef ENABLE_ACCE_SYSTEM
		bool	LoadItemScale(const char* c_szFileName);
#endif
#ifdef ENABLE_SHINING_SYSTEM
		bool	LoadShiningTable(const char* szShiningTable);
#endif
		
	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;
		TBlendMap m_BlendMap;
};
