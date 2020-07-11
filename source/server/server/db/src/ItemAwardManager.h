// vim:ts=8 sw=4
#ifndef __INC_ITEM_AWARD_H
#define __INC_ITEM_AWARD_H
#include <map>
#include <set>
#include "Peer.h"

#ifdef ENABLE_EXTEND_ITEM_AWARD
enum
{
	ITEM_ATTRIBUTE_NONE = -1,
	ITEM_SKILL_VNUM = 50300,
	ITEM_SKILLFORGET_VNUM = 70037,
};
#endif

typedef struct SItemAward
{
	DWORD	dwID;
	char	szLogin[LOGIN_MAX_LEN + 1];
	DWORD	dwVnum;
	DWORD	dwCount;
	DWORD	dwSocket0;
	DWORD	dwSocket1;
	DWORD	dwSocket2;
	DWORD	dwSocket3;
	char	szWhy[ITEM_AWARD_WHY_MAX_LEN + 1];
	bool	bTaken;
	bool	bMall;
#ifdef ENABLE_EXTEND_ITEM_AWARD
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#endif
} TItemAward;

class ItemAwardManager : public singleton<ItemAwardManager>
{
public:
	ItemAwardManager();
	virtual ~ItemAwardManager();

	void				RequestLoad();
	void				Load(SQLMsg* pMsg);
	std::set<TItemAward*>* GetByLogin(const char* c_pszLogin);

	void				Taken(DWORD dwAwardID, DWORD dwItemID);
	
#ifdef ENABLE_EXTEND_ITEM_AWARD
	void 				CheckItemAddonType(TItemAward & pkItemAward, const TItemTable & pkItemTable);
	void				CheckItemCount(TItemAward & pkItemAward, const TItemTable & pkItemTable);
	void				CheckItemBlend(TItemAward & pkItemAward, const TItemTable & pkItemTable);
	void				CheckItemSkillBook(TItemAward & pkItemAward, const std::vector<TSkillTable> vec_skillTable);
#endif
#ifdef USE_ITEM_AWARD_CHECK_ATTRIBUTES
	void				CheckItemAttributes(TItemAward & pkItemAward, const TItemTable & pkItemTable, const std::vector<TItemAttrTable> vec_itemAttrTable);
	int8_t				GetItemAttributeSetIndex(const uint8_t bItemType, const uint8_t bItemSubType) const;
#endif
	
	// gift notify
	std::map<DWORD, TItemAward*>& GetMapAward();
	std::map<std::string, std::set<TItemAward*> >& GetMapkSetAwardByLogin();
private:
	// ID, ItemAward pair
	std::map<DWORD, TItemAward*>			m_map_award;
	// PID, ItemAward pair
	std::map<std::string, std::set<TItemAward*> >	m_map_kSetAwardByLogin;
};

#endif
