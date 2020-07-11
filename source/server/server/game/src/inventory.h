#ifndef __HEADER_INVENTORY__
#define __HEADER_INVENTORY__

#include "typedef.h"

class CInventory
{
	public:
		enum class EStatus 
		{
			ERROR_UNDEFINED = -1,
			SUCCESS = 0,
			ERROR_NO_SPACE = 1,
		};
		CInventory(LPCHARACTER owner) { m_owner = owner; };
		EStatus Add(LPITEM &item);
		LPITEM GetItemAtPos(const WORD& wCell, BYTE bWindowType = 0) const;
		LPITEM FindSameItem(LPITEM item, int fromPos, int untilPos, BYTE bWindowType = 0) const;

	private:
		void TryStack(int fromInventoryPos, int untilInventoryPos, LPITEM item, LPITEM &match, short &bCount, BYTE bWindowType = INVENTORY);
		LPCHARACTER m_owner;
};

#endif