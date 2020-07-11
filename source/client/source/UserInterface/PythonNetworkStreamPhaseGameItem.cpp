#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "PythonItem.h"
#include "PythonShop.h"
#include "PythonExchange.h"
#include "PythonSafeBox.h"
#include "PythonCharacterManager.h"
#include "PythonSystem.h"
#include "PythonPlayer.h"
#include "PythonPrivateShopSearch.h"
#include "AbstractPlayer.h"

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
bool CPythonNetworkStream::SendSafeBoxMoneyPacket(BYTE byState, long long llMoney)
{
	TPacketCGSafeboxMoney kSafeboxMoney;
	kSafeboxMoney.bHeader = HEADER_CG_SAFEBOX_MONEY;
	kSafeboxMoney.bState = byState;
	kSafeboxMoney.llMoney = llMoney;
	if (!Send(sizeof(kSafeboxMoney), &kSafeboxMoney))
		return false;

	return true;
}

bool CPythonNetworkStream::SendSafeBoxChequePacket(BYTE byState, WORD wCheque)
{
	TPacketCGSafeboxCheque kSafeboxCheque;
	kSafeboxCheque.bHeader = HEADER_CG_SAFEBOX_CHEQUE;
	kSafeboxCheque.bState = byState;
	kSafeboxCheque.wCheque = wCheque;
	if (!Send(sizeof(kSafeboxCheque), &kSafeboxCheque))
		return false;

	return true;
}
#endif

bool CPythonNetworkStream::SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos)
{
	__PlayInventoryItemDropSound(InventoryPos);

	TPacketCGSafeboxCheckin kSafeboxCheckin;
	kSafeboxCheckin.bHeader = HEADER_CG_SAFEBOX_CHECKIN;
	kSafeboxCheckin.ItemPos = InventoryPos;
	kSafeboxCheckin.bSafePos = bySafeBoxPos;
	if (!Send(sizeof(kSafeboxCheckin), &kSafeboxCheckin))
		return false;

	return true;
}

bool CPythonNetworkStream::SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos)
{
	__PlaySafeBoxItemDropSound(bySafeBoxPos);

	TPacketCGSafeboxCheckout kSafeboxCheckout;
	kSafeboxCheckout.bHeader = HEADER_CG_SAFEBOX_CHECKOUT;
	kSafeboxCheckout.bSafePos = bySafeBoxPos;
	kSafeboxCheckout.ItemPos = InventoryPos;
	if (!Send(sizeof(kSafeboxCheckout), &kSafeboxCheckout))
		return false;

	return true;
}

bool CPythonNetworkStream::SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, BYTE byCount)
{
	__PlaySafeBoxItemDropSound(bySourcePos);

	TPacketCGItemMove kItemMove;
	kItemMove.header = HEADER_CG_SAFEBOX_ITEM_MOVE;
	kItemMove.pos = TItemPos(INVENTORY, bySourcePos);
	kItemMove.num = byCount;
	kItemMove.change_pos = TItemPos(INVENTORY, byTargetPos);
	if (!Send(sizeof(kItemMove), &kItemMove))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvSafeBoxSetPacket()
{
	TPacketGCItemSet2 kItemSet;
	if (!Recv(sizeof(kItemSet), &kItemSet))
		return false;

	TItemData kItemData;
	kItemData.vnum	= kItemSet.vnum;
	kItemData.count = kItemSet.count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	kItemData.transmutation = kItemSet.transmutation;
#endif
	kItemData.flags = kItemSet.flags;
	kItemData.anti_flags = kItemSet.anti_flags;
	for (int isocket=0; isocket<ITEM_SOCKET_SLOT_MAX_NUM; ++isocket)
		kItemData.alSockets[isocket] = kItemSet.alSockets[isocket];
	for (int iattr=0; iattr<ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++iattr)
		kItemData.aAttr[iattr] = kItemSet.aAttr[iattr];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	kItemData.sealbind = kItemSet.sealbind;
#endif
	kItemData.is_basic = false;
	CPythonSafeBox::Instance().SetItemData(kItemSet.Cell.cell, kItemData);

	__RefreshSafeboxWindow();

	return true;
}

bool CPythonNetworkStream::RecvSafeBoxDelPacket()
{
	TPacketGCItemDel kItemDel;
	if (!Recv(sizeof(kItemDel), &kItemDel))
		return false;

	CPythonSafeBox::Instance().DelItemData(kItemDel.pos);

	__RefreshSafeboxWindow();

	return true;
}

bool CPythonNetworkStream::RecvSafeBoxWrongPasswordPacket()
{
	TPacketGCSafeboxWrongPassword kSafeboxWrongPassword;

	if (!Recv(sizeof(kSafeboxWrongPassword), &kSafeboxWrongPassword))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnSafeBoxError", Py_BuildValue("()"));

	return true;
}

bool CPythonNetworkStream::RecvSafeBoxSizePacket()
{
	TPacketGCSafeboxSize kSafeBoxSize;
	if (!Recv(sizeof(kSafeBoxSize), &kSafeBoxSize))
		return false;

	CPythonSafeBox::Instance().OpenSafeBox(kSafeBoxSize.bSize);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenSafeboxWindow", Py_BuildValue("(i)", kSafeBoxSize.bSize));
	return true;
}

#ifdef ENABLE_SAFEBOX_MONEY_SYSTEM
bool CPythonNetworkStream::RecvSafeBoxMoneyChangePacket()
{
	TPacketGCSafeboxMoneyChange kMoneyChange;
	if (!Recv(sizeof(kMoneyChange), &kMoneyChange))
		return false;

	CPythonSafeBox::Instance().SetMoney(kMoneyChange.llMoney);
	CPythonSafeBox::Instance().SetCheque(kMoneyChange.wCheque);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSafeboxMoney", Py_BuildValue("()"));
	return true;
}
#endif

//////////////////////////////////////////////////////////////////////////
// Mall
bool CPythonNetworkStream::SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos)
{
	__PlayMallItemDropSound(byMallPos);

	TPacketCGMallCheckout kMallCheckoutPacket;
	kMallCheckoutPacket.bHeader = HEADER_CG_MALL_CHECKOUT;
	kMallCheckoutPacket.bMallPos = byMallPos;
	kMallCheckoutPacket.ItemPos = InventoryPos;
	if (!Send(sizeof(kMallCheckoutPacket), &kMallCheckoutPacket))
		return false;

	return true;
}

bool CPythonNetworkStream::RecvMallOpenPacket()
{
	TPacketGCMallOpen kMallOpen;
	if (!Recv(sizeof(kMallOpen), &kMallOpen))
		return false;

	CPythonSafeBox::Instance().OpenMall(kMallOpen.bSize);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenMallWindow", Py_BuildValue("(i)", kMallOpen.bSize));

	return true;
}
bool CPythonNetworkStream::RecvMallItemSetPacket()
{
	TPacketGCItemSet2 kItemSet;
	if (!Recv(sizeof(kItemSet), &kItemSet))
		return false;

	TItemData kItemData;
	kItemData.vnum = kItemSet.vnum;
	kItemData.count = kItemSet.count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	kItemData.transmutation = kItemSet.transmutation;
#endif
	kItemData.flags = kItemSet.flags;
	kItemData.anti_flags = kItemSet.anti_flags;
	for (int isocket=0; isocket<ITEM_SOCKET_SLOT_MAX_NUM; ++isocket)
		kItemData.alSockets[isocket] = kItemSet.alSockets[isocket];
	for (int iattr=0; iattr<ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++iattr)
		kItemData.aAttr[iattr] = kItemSet.aAttr[iattr];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	kItemData.sealbind = kItemSet.sealbind;
#endif
	kItemData.is_basic = false;

	CPythonSafeBox::Instance().SetMallItemData(kItemSet.Cell.cell, kItemData);

	__RefreshMallWindow();

	return true;
}
bool CPythonNetworkStream::RecvMallItemDelPacket()
{
	TPacketGCItemDel kItemDel;
	if (!Recv(sizeof(kItemDel), &kItemDel))
		return false;

	CPythonSafeBox::Instance().DelMallItemData(kItemDel.pos);

	__RefreshMallWindow();
	Tracef(" >> CPythonNetworkStream::RecvMallItemDelPacket\n");

	return true;
}
// Mall
//////////////////////////////////////////////////////////////////////////

// Item
// Recieve
bool CPythonNetworkStream::RecvItemSetPacket()
{
	TPacketGCItemSet packet_item_set;

	if (!Recv(sizeof(TPacketGCItemSet), &packet_item_set))
		return false;

	TItemData kItemData;
	kItemData.vnum	= packet_item_set.vnum;
	kItemData.count	= packet_item_set.count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	kItemData.transmutation = packet_item_set.transmutation;
#endif
	kItemData.flags = 0;
	for (int i=0; i<ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		kItemData.alSockets[i]=packet_item_set.alSockets[i];
	for (int j=0; j<ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		kItemData.aAttr[j]=packet_item_set.aAttr[j];

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	
	rkPlayer.SetItemData(packet_item_set.Cell, kItemData);
	
	__RefreshInventoryWindow();
	return true;
}

bool CPythonNetworkStream::RecvItemSetPacket2()
{
	TPacketGCItemSet2 packet_item_set;

	if (!Recv(sizeof(TPacketGCItemSet2), &packet_item_set))
		return false;

	TItemData kItemData;
	kItemData.vnum = packet_item_set.vnum;
	kItemData.count = packet_item_set.count;
#ifdef ENABLE_CHANGELOOK_SYSTEM
	kItemData.transmutation = packet_item_set.transmutation;
#endif
	kItemData.flags = packet_item_set.flags;
	kItemData.anti_flags = packet_item_set.anti_flags;

	for (int i = 0; i<ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		kItemData.alSockets[i] = packet_item_set.alSockets[i];
	for (int j = 0; j<ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		kItemData.aAttr[j] = packet_item_set.aAttr[j];
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	kItemData.sealbind = packet_item_set.sealbind;
#endif
#ifdef ENABLE_BASIC_ITEM_SYSTEM
	kItemData.is_basic = packet_item_set.is_basic;
#else
	kItemData.is_basic = false;
#endif
	
	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetItemData(packet_item_set.Cell, kItemData);

	if (packet_item_set.highlight)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_Highlight_Item", Py_BuildValue("(ii)", packet_item_set.Cell.window_type, packet_item_set.Cell.cell));

	__RefreshInventoryWindow();
	return true;
}

bool CPythonNetworkStream::RecvItemUsePacket()
{
	TPacketGCItemUse packet_item_use;

	if (!Recv(sizeof(TPacketGCItemUse), &packet_item_use))
		return false;

	__RefreshInventoryWindow();
	return true;
}

bool CPythonNetworkStream::RecvItemUpdatePacket()
{
	TPacketGCItemUpdate packet_item_update;

	if (!Recv(sizeof(TPacketGCItemUpdate), &packet_item_update))
		return false;

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	rkPlayer.SetItemCount(packet_item_update.Cell, packet_item_update.count);
#ifdef ENABLE_CHANGELOOK_SYSTEM
	rkPlayer.SetItemTransmutation(packet_item_update.Cell, packet_item_update.transmutation);
#endif
#ifdef ENABLE_BASIC_ITEM_SYSTEM
	rkPlayer.SetBasicItem(packet_item_update.Cell, packet_item_update.is_basic);
#endif
	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
		rkPlayer.SetItemMetinSocket(packet_item_update.Cell, i, packet_item_update.alSockets[i]);
	for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
		rkPlayer.SetItemAttribute(packet_item_update.Cell, j, packet_item_update.aAttr[j].bType, packet_item_update.aAttr[j].sValue);
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	rkPlayer.SetItemUnbindTime(packet_item_update.Cell, packet_item_update.sealbind);
#endif

	__RefreshInventoryWindow();
	return true;
}

bool CPythonNetworkStream::RecvItemGroundAddPacket()
{
	TPacketGCItemGroundAdd packet_item_ground_add;

	if (!Recv(sizeof(TPacketGCItemGroundAdd), &packet_item_ground_add))
		return false;

	__GlobalPositionToLocalPosition(packet_item_ground_add.lX, packet_item_ground_add.lY);

	CPythonItem::Instance().CreateItem(packet_item_ground_add.dwVID, 
									   packet_item_ground_add.dwVnum,
									   packet_item_ground_add.alSockets[0],
									   packet_item_ground_add.alSockets[1],
									   packet_item_ground_add.alSockets[2],
									   packet_item_ground_add.count,
									   packet_item_ground_add.lX,
									   packet_item_ground_add.lY,
									   packet_item_ground_add.lZ);
	return true;
}


bool CPythonNetworkStream::RecvItemOwnership()
{
	TPacketGCItemOwnership p;

	if (!Recv(sizeof(TPacketGCItemOwnership), &p))
		return false;

	CPythonItem::Instance().SetOwnership(p.dwVID, p.szName);
	return true;
}

bool CPythonNetworkStream::RecvItemGroundDelPacket()
{
	TPacketGCItemGroundDel	packet_item_ground_del;

	if (!Recv(sizeof(TPacketGCItemGroundDel), &packet_item_ground_del))
		return false;

	CPythonItem::Instance().DeleteItem(packet_item_ground_del.vid);
	return true;
}

bool CPythonNetworkStream::RecvQuickSlotAddPacket()
{
	TPacketGCQuickSlotAdd packet_quick_slot_add;

	if (!Recv(sizeof(TPacketGCQuickSlotAdd), &packet_quick_slot_add))
		return false;

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	rkPlayer.AddQuickSlot(packet_quick_slot_add.pos, packet_quick_slot_add.slot.Type, packet_quick_slot_add.slot.Position);

	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::RecvQuickSlotDelPacket()
{
	TPacketGCQuickSlotDel packet_quick_slot_del;

	if (!Recv(sizeof(TPacketGCQuickSlotDel), &packet_quick_slot_del))
		return false;

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	rkPlayer.DeleteQuickSlot(packet_quick_slot_del.pos);

	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::RecvQuickSlotMovePacket()
{
	TPacketGCQuickSlotSwap packet_quick_slot_swap;

	if (!Recv(sizeof(TPacketGCQuickSlotSwap), &packet_quick_slot_swap))
		return false;

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	rkPlayer.MoveQuickSlot(packet_quick_slot_swap.pos, packet_quick_slot_swap.change_pos);

	__RefreshInventoryWindow();

	return true;
}



bool CPythonNetworkStream::SendShopEndPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop packet_shop;
	packet_shop.header = HEADER_CG_SHOP;
	packet_shop.subheader = SHOP_SUBHEADER_CG_END;

	if (!Send(sizeof(packet_shop), &packet_shop))
	{
		Tracef("SendShopEndPacket Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendShopBuyPacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;
	
	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_BUY;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	BYTE bCount=1;
	if (!Send(sizeof(BYTE), &bCount))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	if (!Send(sizeof(BYTE), &bPos))
	{
		Tracef("SendShopBuyPacket Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendShopSellPacket(BYTE bySlot)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_SELL;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(BYTE), &bySlot))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendShopSellPacketNew(UINT wSlot, BYTE byCount, BYTE byType)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGShop PacketShop;
	PacketShop.header = HEADER_CG_SHOP;
	PacketShop.subheader = SHOP_SUBHEADER_CG_SELL2;

	if (!Send(sizeof(TPacketCGShop), &PacketShop))
	{
		Tracef("SendShopSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(UINT), &wSlot))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(BYTE), &byCount))
	{
		Tracef("SendShopAddSellPacket Error\n");
		return false;
	}
	if (!Send(sizeof(BYTE), &byType))
	{
		Tracef("SendShopAddSellPacket Errorn");
		return false;
	}

	Tracef(" SendShopSellPacketNew(wSlot=%d, byCount=%d, byType=%d)n", wSlot, byCount, byType);

	return true;
}

// Send
bool CPythonNetworkStream::SendItemUsePacket(TItemPos pos)
{
	if (!__CanActMainInstance())
		return true;

	if (__IsEquipItemInSlot(pos))
	{
		if (CPythonExchange::Instance().isTrading())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_EXCHANGE"));
			return true;
		}

		if (CPythonShop::Instance().IsOpen())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_SHOP"));
			return true;
		}

		if (__IsPlayerAttacking())
			return true;
	}

	__PlayInventoryItemUseSound(pos);

	TPacketCGItemUse itemUsePacket;
	itemUsePacket.header = HEADER_CG_ITEM_USE;
	itemUsePacket.pos = pos;

	if (!Send(sizeof(TPacketCGItemUse), &itemUsePacket))
	{
		Tracen("SendItemUsePacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos)
{
	if (!__CanActMainInstance())
		return true;	

	TPacketCGItemUseToItem itemUseToItemPacket;
	itemUseToItemPacket.header = HEADER_CG_ITEM_USE_TO_ITEM;
	itemUseToItemPacket.source_pos = source_pos;
	itemUseToItemPacket.target_pos = target_pos;

	if (!Send(sizeof(TPacketCGItemUseToItem), &itemUseToItemPacket))
	{
		Tracen("SendItemUseToItemPacket Error");
		return false;
	}

#ifdef _DEBUG
	Tracef(" << SendItemUseToItemPacket(src=%d, dst=%d)\n", source_pos, target_pos);
#endif

	return true;
}

#ifndef ENABLE_DROP_DIALOG_EX_SYSTEM
bool CPythonNetworkStream::SendItemDropPacket(TItemPos pos, DWORD elk)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemDrop itemDropPacket;
	itemDropPacket.header = HEADER_CG_ITEM_DROP;
	itemDropPacket.pos = pos;
	itemDropPacket.elk = elk;

	if (!Send(sizeof(TPacketCGItemDrop), &itemDropPacket))
	{
		Tracen("SendItemDropPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD count)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemDrop2 itemDropPacket;
	itemDropPacket.header = HEADER_CG_ITEM_DROP2;
	itemDropPacket.pos = pos;
	itemDropPacket.gold = elk;
	itemDropPacket.count = count;

	if (!Send(sizeof(itemDropPacket), &itemDropPacket))
	{
		Tracen("SendItemDropPacket Error");
		return false;
	}

	return true;
}
#else
bool CPythonNetworkStream::SendItemDeletePacket(TItemPos item_pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemDelete itemDeletePacket;
	itemDeletePacket.header = HEADER_CG_ITEM_DELETE;
	itemDeletePacket.item_pos = item_pos;

	if (!Send(sizeof(TPacketCGItemDelete), &itemDeletePacket))
	{
		Tracen("SendItemDeletePacket Error");
		return false;
	}

#ifdef _DEBUG
	Tracef(" << SendItemDeletePacket(item_pos=%d)\n", item_pos);
#endif

	return true;
}

bool CPythonNetworkStream::SendItemSellPacket(TItemPos item_pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemSell itemSellPacket;
	itemSellPacket.header = HEADER_CG_ITEM_SELL;
	itemSellPacket.item_pos = item_pos;

	if (!Send(sizeof(TPacketCGItemSell), &itemSellPacket))
	{
		Tracen("SendItemSellPacket Error");
		return false;
	}

#ifdef _DEBUG
	Tracef(" << SendItemSellPacket(item_pos=%d)\n", item_pos);
#endif

	return true;
}
#endif

bool CPythonNetworkStream::__IsEquipItemInSlot(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	return rkPlayer.IsEquipItemInSlot(uSlotPos);
}

void CPythonNetworkStream::__PlayInventoryItemUseSound(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	DWORD dwItemID=rkPlayer.GetItemIndex(uSlotPos);

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.PlayUseSound(dwItemID);
}

void CPythonNetworkStream::__PlayInventoryItemDropSound(TItemPos uSlotPos)
{
	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	DWORD dwItemID=rkPlayer.GetItemIndex(uSlotPos);

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

//void CPythonNetworkStream::__PlayShopItemDropSound(UINT uSlotPos)
//{
//	DWORD dwItemID;
//	CPythonShop& rkShop=CPythonShop::Instance();
//	if (!rkShop.GetSlotItemID(uSlotPos, &dwItemID))
//		return;
//	
//	CPythonItem& rkItem=CPythonItem::Instance();
//	rkItem.PlayDropSound(dwItemID);
//}

void CPythonNetworkStream::__PlaySafeBoxItemDropSound(UINT uSlotPos)
{
	DWORD dwItemID;
	CPythonSafeBox& rkSafeBox=CPythonSafeBox::Instance();
	if (!rkSafeBox.GetSlotItemID(uSlotPos, &dwItemID))
		return;

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

void CPythonNetworkStream::__PlayMallItemDropSound(UINT uSlotPos)
{
	DWORD dwItemID;
	CPythonSafeBox& rkSafeBox=CPythonSafeBox::Instance();
	if (!rkSafeBox.GetSlotMallItemID(uSlotPos, &dwItemID))
		return;

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.PlayDropSound(dwItemID);
}

bool CPythonNetworkStream::SendItemMovePacket(TItemPos pos, TItemPos change_pos, BYTE num)
{	
	if (!__CanActMainInstance())
		return true;
	
	if (__IsEquipItemInSlot(pos))
	{
		if (CPythonExchange::Instance().isTrading())
		{
			if (pos.IsEquipCell() || change_pos.IsEquipCell())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_EXCHANGE"));
				return true;
			}
		}

		if (CPythonShop::Instance().IsOpen())
		{
			if (pos.IsEquipCell() || change_pos.IsEquipCell())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_AppendNotifyMessage", Py_BuildValue("(s)", "CANNOT_EQUIP_SHOP"));
				return true;
			}
		}

		if (__IsPlayerAttacking())
			return true;
	}

	__PlayInventoryItemDropSound(pos);

	TPacketCGItemMove	itemMovePacket;
	itemMovePacket.header = HEADER_CG_ITEM_MOVE;
	itemMovePacket.pos = pos;
	itemMovePacket.change_pos = change_pos;
	itemMovePacket.num = num;

	if (!Send(sizeof(TPacketCGItemMove), &itemMovePacket))
	{
		Tracen("SendItemMovePacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendItemPickUpPacket(DWORD vid)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGItemPickUp	itemPickUpPacket;
	itemPickUpPacket.header = HEADER_CG_ITEM_PICKUP;
	itemPickUpPacket.vid = vid;

	if (!Send(sizeof(TPacketCGItemPickUp), &itemPickUpPacket))
	{
		Tracen("SendItemPickUpPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendQuickSlotAddPacket(UINT wpos, BYTE type, UINT pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotAdd quickSlotAddPacket;

	quickSlotAddPacket.header		= HEADER_CG_QUICKSLOT_ADD;
	quickSlotAddPacket.pos			= wpos;
	quickSlotAddPacket.slot.Type	= type;
	quickSlotAddPacket.slot.Position = pos;

	if (!Send(sizeof(TPacketCGQuickSlotAdd), &quickSlotAddPacket))
	{
		Tracen("SendQuickSlotAddPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendQuickSlotDelPacket(UINT pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotDel quickSlotDelPacket;

	quickSlotDelPacket.header = HEADER_CG_QUICKSLOT_DEL;
	quickSlotDelPacket.pos = pos;

	if (!Send(sizeof(TPacketCGQuickSlotDel), &quickSlotDelPacket))
	{
		Tracen("SendQuickSlotDelPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendQuickSlotMovePacket(UINT pos, UINT change_pos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGQuickSlotSwap quickSlotSwapPacket;

	quickSlotSwapPacket.header = HEADER_CG_QUICKSLOT_SWAP;
	quickSlotSwapPacket.pos = pos;
	quickSlotSwapPacket.change_pos = change_pos;

	if (!Send(sizeof(TPacketCGQuickSlotSwap), &quickSlotSwapPacket))
	{
		Tracen("SendQuickSlotSwapPacket Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvSpecialEffect()
{
	TPacketGCSpecialEffect kSpecialEffect;
	if (!Recv(sizeof(kSpecialEffect), &kSpecialEffect))
		return false;

	DWORD effect = -1;
	bool bPlayPotionSound = false;	//포션을 먹을 경우는 포션 사운드를 출력하자.!!
	bool bAttachEffect = true;		//캐리터에 붙는 어태치 이펙트와 일반 이펙트 구분.!!
	switch (kSpecialEffect.type)
	{
		case SE_HPUP_RED:
			effect = CInstanceBase::EFFECT_HPUP_RED;
			bPlayPotionSound = true;
			break;
		case SE_SPUP_BLUE:
			effect = CInstanceBase::EFFECT_SPUP_BLUE;
			bPlayPotionSound = true;
			break;
		case SE_SPEEDUP_GREEN:
			effect = CInstanceBase::EFFECT_SPEEDUP_GREEN;
			bPlayPotionSound = true;
			break;
		case SE_DXUP_PURPLE:
			effect = CInstanceBase::EFFECT_DXUP_PURPLE;
			bPlayPotionSound = true;
			break;
		case SE_CRITICAL:
			effect = CInstanceBase::EFFECT_CRITICAL;
			break;
		case SE_PENETRATE:
			effect = CInstanceBase::EFFECT_PENETRATE;
			break;
		case SE_BLOCK:
			effect = CInstanceBase::EFFECT_BLOCK;
			break;
		case SE_DODGE:
			effect = CInstanceBase::EFFECT_DODGE;
			break;
		case SE_CHINA_FIREWORK:
			effect = CInstanceBase::EFFECT_FIRECRACKER;
			bAttachEffect = false;
			break;
		case SE_SPIN_TOP:
			effect = CInstanceBase::EFFECT_SPIN_TOP;
			bAttachEffect = false;
			break;
		case SE_SUCCESS :
			effect = CInstanceBase::EFFECT_SUCCESS ;
			bAttachEffect = false ;
			break ;
		case SE_FAIL :
			effect = CInstanceBase::EFFECT_FAIL ;
			break ;
		case SE_LEVELUP_ON_14_FOR_GERMANY:	//레벨업 14일때 ( 독일전용 )
			effect = CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY;
			bAttachEffect = false ;
			break;
		case SE_LEVELUP_UNDER_15_FOR_GERMANY: //레벨업 15일때 ( 독일전용 )
			effect = CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY;
			bAttachEffect = false ;
			break;
		case SE_PERCENT_DAMAGE1:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE1;
			break;
		case SE_PERCENT_DAMAGE2:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE2;
			break;
		case SE_PERCENT_DAMAGE3:
			effect = CInstanceBase::EFFECT_PERCENT_DAMAGE3;
			break;
		case SE_AUTO_HPUP:
			effect = CInstanceBase::EFFECT_AUTO_HPUP;
			break;
		case SE_AUTO_SPUP:
			effect = CInstanceBase::EFFECT_AUTO_SPUP;
			break;
		case SE_EQUIP_RAMADAN_RING:
			effect = CInstanceBase::EFFECT_RAMADAN_RING_EQUIP;
			break;
		case SE_EQUIP_HALLOWEEN_CANDY:
			effect = CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP;
			break;
		case SE_EQUIP_HAPPINESS_RING:
 			effect = CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP;
			break;
		case SE_EQUIP_LOVE_PENDANT:
			effect = CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP;
			break;
#ifdef ENABLE_VERSION_162_ENABLED
		case SE_EFFECT_HEALER:
			effect = CInstanceBase::EFFECT_HEALER;
			break;
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
		case SE_COMBAT_ZONE_POTION:
			effect = CInstanceBase::EFFECT_COMBAT_ZONE_POTION;
			break;	
#endif
#ifdef ENABLE_ACCE_SYSTEM
		case SE_EFFECT_SASH_SUCCEDED:
			effect = CInstanceBase::EFFECT_SASH_SUCCEDED;
			break;
		case SE_EFFECT_SASH_EQUIP:
			effect = CInstanceBase::EFFECT_SASH_EQUIP;
			break;
#endif
		// NEW_EFFECTS
		case SE_PVP_WIN:
			effect = CInstanceBase::EFFECT_PVP_WIN;
			break;
		case SE_PVP_OPEN1:
			effect = CInstanceBase::EFFECT_PVP_OPEN1;
			break;
		case SE_PVP_OPEN2:
			effect = CInstanceBase::EFFECT_PVP_OPEN2;
			break;
		case SE_PVP_BEGIN1:
			effect = CInstanceBase::EFFECT_PVP_BEGIN1;
			break;
		case SE_PVP_BEGIN2:
			effect = CInstanceBase::EFFECT_PVP_BEGIN2;
			break;
		case SE_FR_SUCCESS:
			effect = CInstanceBase::EFFECT_FR_SUCCESS;
			break;
		case SE_FR_FAIL:
			effect = CInstanceBase::EFFECT_FR_FAIL;
			break;
		// NEW_EFFECTS
#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
		case SE_SKILL_DAMAGE_ZONE:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE;
			break;

		case SE_SKILL_DAMAGE_ZONE_BUYUK:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_BUYUK;
			break;

		case SE_SKILL_DAMAGE_ZONE_ORTA:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_ORTA;
			break;

		case SE_SKILL_DAMAGE_ZONE_KUCUK:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_KUCUK;
			break;

		case SE_SKILL_SAFE_ZONE:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE;
			break;

		case SE_SKILL_SAFE_ZONE_BUYUK:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_BUYUK;
			break;

		case SE_SKILL_SAFE_ZONE_ORTA:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_ORTA;
			break;

		case SE_SKILL_SAFE_ZONE_KUCUK:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_KUCUK;
			break;

		case SE_METEOR:
			effect = CInstanceBase::EFFECT_METEOR;
			break;

		case SE_BEAD_RAIN:
			effect = CInstanceBase::EFFECT_BEAD_RAIN;
			break;

		case SE_FALL_ROCK:
			effect = CInstanceBase::EFFECT_FALL_ROCK;
			break;

		case SE_ARROW_RAIN:
			effect = CInstanceBase::EFFECT_ARROW_RAIN;
			break;

		case SE_HORSE_DROP:
			effect = CInstanceBase::EFFECT_HORSE_DROP;
			break;

		case SE_EGG_DROP:
			effect = CInstanceBase::EFFECT_EGG_DROP;
			break;

		case SE_DEAPO_BOOM:
			effect = CInstanceBase::EFFECT_DEAPO_BOOM;
			break;
#endif
		default:
			TraceError ("%d is not a special effect number. TPacketGCSpecialEffect", kSpecialEffect.type);
			break;
	}

	if (bPlayPotionSound)
	{		
		IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
		if(rkPlayer.IsMainCharacterIndex(kSpecialEffect.vid))
		{
			CPythonItem& rkItem=CPythonItem::Instance();
			rkItem.PlayUsePotionSound();
		}
	}

	if (-1 != effect)
	{
		CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecialEffect.vid);
		if (pInstance)
		{
			if(bAttachEffect)
				pInstance->AttachSpecialEffect(effect);
			else
				pInstance->CreateSpecialEffect(effect);
		}
	}

	return true;
}

#ifdef ENABLE_ZODIAC_TEMPLE_SYSTEM
bool CPythonNetworkStream::RecvSpecialZodiacEffect()
{
	TPacketGCSpecialZodiacEffect kSpecialZodiacEffect;
	if (!Recv(sizeof(kSpecialZodiacEffect), &kSpecialZodiacEffect))
		return false;

	DWORD effect = -1;
	switch (kSpecialZodiacEffect.type)
	{
		case SE_SKILL_DAMAGE_ZONE:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE;
			break;

		case SE_SKILL_DAMAGE_ZONE_BUYUK:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_BUYUK;
			break;

		case SE_SKILL_DAMAGE_ZONE_ORTA:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_ORTA;
			break;

		case SE_SKILL_DAMAGE_ZONE_KUCUK:
			effect = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_KUCUK;
			break;

		case SE_SKILL_SAFE_ZONE:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE;
			break;

		case SE_SKILL_SAFE_ZONE_BUYUK:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_BUYUK;
			break;

		case SE_SKILL_SAFE_ZONE_ORTA:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_ORTA;
			break;

		case SE_SKILL_SAFE_ZONE_KUCUK:
			effect = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_KUCUK;
			break;

		case SE_METEOR:
			effect = CInstanceBase::EFFECT_METEOR;
			break;

		case SE_BEAD_RAIN:
			effect = CInstanceBase::EFFECT_BEAD_RAIN;
			break;

		case SE_FALL_ROCK:
			effect = CInstanceBase::EFFECT_FALL_ROCK;
			break;

		case SE_ARROW_RAIN:
			effect = CInstanceBase::EFFECT_ARROW_RAIN;
			break;

		case SE_HORSE_DROP:
			effect = CInstanceBase::EFFECT_HORSE_DROP;
			break;

		case SE_EGG_DROP:
			effect = CInstanceBase::EFFECT_EGG_DROP;
			break;

		default:
			TraceError("TPacketGCSpecialZodiacEffect.type::Cannot packet:: %d", kSpecialZodiacEffect.type);
			break;
	}

	if (-1 != effect)
	{
		CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecialZodiacEffect.vid);
		if (pInstance)
		{
			pInstance->AttachSpecialZodiacEffect(effect, kSpecialZodiacEffect.x, kSpecialZodiacEffect.y);
		}
	}

	DWORD effect2 = -1;
	switch (kSpecialZodiacEffect.type2)
	{
		case SE_SKILL_DAMAGE_ZONE:
			effect2 = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE;
			break;

		case SE_SKILL_DAMAGE_ZONE_BUYUK:
			effect2 = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_BUYUK;
			break;

		case SE_SKILL_DAMAGE_ZONE_ORTA:
			effect2 = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_ORTA;
			break;

		case SE_SKILL_DAMAGE_ZONE_KUCUK:
			effect2 = CInstanceBase::EFFECT_SKILL_DAMAGE_ZONE_KUCUK;
			break;

		case SE_SKILL_SAFE_ZONE:
			effect2 = CInstanceBase::EFFECT_SKILL_SAFE_ZONE;
			break;

		case SE_SKILL_SAFE_ZONE_BUYUK:
			effect2 = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_BUYUK;
			break;

		case SE_SKILL_SAFE_ZONE_ORTA:
			effect2 = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_ORTA;
			break;

		case SE_SKILL_SAFE_ZONE_KUCUK:
			effect2 = CInstanceBase::EFFECT_SKILL_SAFE_ZONE_KUCUK;
			break;

		case SE_METEOR:
			effect2 = CInstanceBase::EFFECT_METEOR;
			break;

		case SE_BEAD_RAIN:
			effect2 = CInstanceBase::EFFECT_BEAD_RAIN;
			break;

		case SE_FALL_ROCK:
			effect2 = CInstanceBase::EFFECT_FALL_ROCK;
			break;

		case SE_ARROW_RAIN:
			effect2 = CInstanceBase::EFFECT_ARROW_RAIN;
			break;

		case SE_HORSE_DROP:
			effect2 = CInstanceBase::EFFECT_HORSE_DROP;
			break;

		case SE_EGG_DROP:
			effect2 = CInstanceBase::EFFECT_EGG_DROP;
			break;
	}

	if (-1 != effect2)
	{
		CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecialZodiacEffect.vid);
		if (pInstance)
		{
			pInstance->AttachSpecialZodiacEffect(effect2, kSpecialZodiacEffect.x, kSpecialZodiacEffect.y);
		}
	}

	return true;
}
#endif

bool CPythonNetworkStream::RecvSpecificEffect()
{
	TPacketGCSpecificEffect kSpecificEffect;
	if (!Recv(sizeof(kSpecificEffect), &kSpecificEffect))
		return false;

	CInstanceBase * pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kSpecificEffect.vid);
	//EFFECT_TEMP
	if (pInstance)
	{
		CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", kSpecificEffect.effect_file, false);
		pInstance->AttachSpecialEffect(CInstanceBase::EFFECT_TEMP);
	}

	return true;
}

bool CPythonNetworkStream::RecvDragonSoulRefine()
{
	TPacketGCDragonSoulRefine kDragonSoul;

	if (!Recv(sizeof(kDragonSoul), &kDragonSoul))
		return false;
	
	
	switch (kDragonSoul.bSubType)
	{
	case DS_SUB_HEADER_OPEN:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_Open", Py_BuildValue("()"));
		break;
	case DS_SUB_HEADER_REFINE_FAIL:
	case DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE:
	case DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL:
	case DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY:
	case DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL:
	case DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_RefineFail", Py_BuildValue("(iii)", 
			kDragonSoul.bSubType, kDragonSoul.Pos.window_type, kDragonSoul.Pos.cell));
		break;
	case DS_SUB_HEADER_REFINE_SUCCEED:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_DragonSoulRefineWindow_RefineSucceed", 
				Py_BuildValue("(ii)", kDragonSoul.Pos.window_type, kDragonSoul.Pos.cell));
		break;
	}

	return true;
}

#ifdef ENABLE_SHOP_SEARCH_SYSTEM
bool CPythonNetworkStream::SendPrivateShopSearchInfo (int32_t Race, int32_t ItemCat, int32_t SubCat, int32_t MinLevel, int32_t MaxLevel, int32_t MinRefine, int32_t MaxRefine, uint64_t MinGold, uint64_t MaxGold, uint64_t MinCheque, uint64_t MaxCheque)
{

	if (!__CanActMainInstance())
	{
		return true;
	}

	TPacketCGShopSearch ShopSearchPacket;
	ShopSearchPacket.header = HEADER_CG_SHOP_SEARCH;
	ShopSearchPacket.Race = Race;
	ShopSearchPacket.ItemCat = ItemCat;
	ShopSearchPacket.SubCat = SubCat;
	ShopSearchPacket.MinLevel = MinLevel;
	ShopSearchPacket.MaxLevel = MaxLevel;
	ShopSearchPacket.MinRefine = MinRefine;
	ShopSearchPacket.MaxRefine = MaxRefine;
	ShopSearchPacket.MinGold = MinGold;
	ShopSearchPacket.MaxGold = MaxGold;
	ShopSearchPacket.MinCheque = MinCheque;
	ShopSearchPacket.MaxCheque = MaxCheque;

	if (!Send (sizeof (ShopSearchPacket), &ShopSearchPacket))
	{
		Tracen ("SendPrivateShopSearchInfo Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendPrivateShopSearchInfoSub (int32_t Race, int32_t ItemCat, int32_t SubCat, int32_t MinLevel, int32_t MaxLevel, int32_t MinRefine, int32_t MaxRefine, uint64_t MinGold, uint64_t MaxGold, char* ItemName, uint64_t MinCheque, uint64_t MaxCheque)
{

	if (!__CanActMainInstance())
	{
		return true;
	}

	TPacketCGShopSearch ShopSearchPacket;
	ShopSearchPacket.header = HEADER_CG_SHOP_SEARCH_SUB;
	ShopSearchPacket.Race = Race;
	ShopSearchPacket.ItemCat = ItemCat;
	ShopSearchPacket.SubCat = SubCat;
	ShopSearchPacket.MinLevel = MinLevel;
	ShopSearchPacket.MaxLevel = MaxLevel;
	ShopSearchPacket.MinRefine = MinRefine;
	ShopSearchPacket.MaxRefine = MaxRefine;
	ShopSearchPacket.MinGold = MinGold;
	ShopSearchPacket.MaxGold = MaxGold;
	strncpy_s (ShopSearchPacket.ItemName, ItemName, sizeof (ShopSearchPacket.ItemName) - 1);
	ShopSearchPacket.MinCheque = MinCheque;
	ShopSearchPacket.MaxCheque = MaxCheque;

	if (!Send (sizeof (ShopSearchPacket), &ShopSearchPacket))
	{
		Tracen ("SendPrivateShopSearchInfoSub Error");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendPrivateShopSerchBuyItem (int32_t shopVid, BYTE shopItemPos)
{
	if (!__CanActMainInstance())
	{
		return true;
	}

	TPacketCGShopSearchBuy PacketShopSearchBuy;
	PacketShopSearchBuy.header = HEADER_CG_SHOP_SEARCH_BUY;
	PacketShopSearchBuy.shopVid = shopVid;
	PacketShopSearchBuy.shopItemPos = shopItemPos;

	if (!Send (sizeof (TPacketCGShopSearchBuy), &PacketShopSearchBuy))
	{
		Tracef ("SendPrivateShopSerchBuyItem Error\n");
		return false;
	}

	Tracef ("SendPrivateShopSerchBuyItem: ShopVid: %d  Pos %d", shopVid, shopItemPos);

	return true;
}

bool CPythonNetworkStream::RecvShopSearchSet()
{
	TPacketGCShopSearchItemSet packet_item_set;

	if (!Recv (sizeof (TPacketGCShopSearchItemSet), &packet_item_set))
	{
		return false;
	}
	CPythonPrivateShopSearch::TSearchItemData searchItem;
	searchItem.vnum = packet_item_set.vnum;
	searchItem.count = packet_item_set.count;
	searchItem.flags = packet_item_set.flags;
	searchItem.anti_flags = packet_item_set.anti_flags;

	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
	{
		searchItem.alSockets[i] = packet_item_set.alSockets[i];
	}
	for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
	{
		searchItem.aAttr[j] = packet_item_set.aAttr[j];
	}

	searchItem.transmutation = packet_item_set.transmutation;
	
	searchItem.vid = packet_item_set.vid;
	searchItem.price = packet_item_set.price;
	searchItem.price2 = packet_item_set.price2;
	searchItem.Cell = packet_item_set.Cell;
	
	strncpy_s (searchItem.name, packet_item_set.name, sizeof (searchItem.name) - 1);

	CPythonPrivateShopSearch::Instance().AddItemData (packet_item_set.vid, packet_item_set.price, packet_item_set.price2, searchItem);

	__RefreshShopSearchWindow();
	return true;
}
#endif

#ifdef ENABLE_FISH_JIGSAW_EVENT
bool CPythonNetworkStream::SendFishBoxUse(BYTE bWindow, WORD wCell)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGFishEvent packetFishEvent;
	packetFishEvent.bHeader = HEADER_CG_FISH_EVENT_SEND;
	packetFishEvent.bSubheader = FISH_EVENT_SUBHEADER_BOX_USE;

	if (!Send(sizeof(TPacketCGFishEvent), &packetFishEvent))
	{
		Tracef("SendFishEventPacket Error\n");
		return false;
	}
	
	if (!Send(sizeof(bWindow), &bWindow))
	{
		Tracef("SendFishBoxUse Error\n");
		return false;
	}
	
	if (!Send(sizeof(wCell), &wCell))
	{
		Tracef("SendFishBoxUse Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::SendFishShapeAdd(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGFishEvent packetFishEvent;
	packetFishEvent.bHeader = HEADER_CG_FISH_EVENT_SEND;
	packetFishEvent.bSubheader = FISH_EVENT_SUBHEADER_SHAPE_ADD;

	if (!Send(sizeof(TPacketCGFishEvent), &packetFishEvent))
	{
		Tracef("SendFishEventPacket Error\n");
		return false;
	}
	
	if (!Send(sizeof(BYTE), &bPos))
	{
		Tracef("SendFishShapeAdd Error\n");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvFishEventInfo()
{
	TPacketGCFishEventInfo fishEventPacket;

	if (!Recv(sizeof(fishEventPacket), &fishEventPacket))
		return false;

	switch (fishEventPacket.bSubheader)
	{
		case FISH_EVENT_SUBHEADER_BOX_USE:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameFishUse", Py_BuildValue("(ii)", fishEventPacket.dwFirstArg, fishEventPacket.dwSecondArg));
			break;
			
		case FISH_EVENT_SUBHEADER_SHAPE_ADD:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameFishAdd", Py_BuildValue("(ii)", fishEventPacket.dwFirstArg, fishEventPacket.dwSecondArg));
			break;
			
		case FISH_EVENT_SUBHEADER_GC_REWARD:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameFishReward", Py_BuildValue("(i)", fishEventPacket.dwFirstArg));
			break;
			
		case FISH_EVENT_SUBHEADER_GC_ENABLE:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameFishEvent", Py_BuildValue("(ii)", fishEventPacket.dwFirstArg, fishEventPacket.dwSecondArg));
			break;
	};

	return true;
}
#endif

#ifdef ENABLE_GEM_SYSTEM
bool CPythonNetworkStream::RecvGemShopOpen()
{
	TPacketGCGemShopOpen kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;
	
	CPythonPlayer::Instance().ClearGemShopItemVector();
	for (int i = 0; i < 27; ++i)
	{
		TGemShopItem GemItem;
		GemItem.slotIndex = kPacket.shopItems[i].slotIndex;
		GemItem.status = kPacket.shopItems[i].status;
		
		GemItem.dwVnum = kPacket.shopItems[i].dwVnum;
		GemItem.bCount = kPacket.shopItems[i].bCount;
		GemItem.dwPrice = kPacket.shopItems[i].dwPrice;

#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (kPacket.gemShopType == true)
			CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem, true);
		else
			CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem, false);
#else
		CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem);
#endif
		
		Tracenf("GELEN VERI: %d %d %d %d %d %d", GemItem.slotIndex, GemItem.status, GemItem.dwVnum, GemItem.bCount, GemItem.dwPrice, kPacket.gemShopType);
	}
	
#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (kPacket.gemShopType == true)
		CPythonPlayer::Instance().SetGemBossShopRefreshTime(kPacket.nextRefreshTime);
	else
		CPythonPlayer::Instance().SetGemShopRefreshTime(kPacket.nextRefreshTime);
#else
	CPythonPlayer::Instance().SetGemShopRefreshTime(kPacket.nextRefreshTime);
#endif

#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (kPacket.gemShopType == true)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenGemShop", Py_BuildValue("(i)", 1));
	else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenGemShop", Py_BuildValue("(i)", 0));
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenGemShop", Py_BuildValue("()"));
#endif

	return true;
}

bool CPythonNetworkStream::RecvGemShopRefresh()
{
	TPacketGCGemShopRefresh kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;
	
	CPythonPlayer::Instance().ClearGemShopItemVector();
	for (int i = 0; i < 27; ++i)
	{
		TGemShopItem GemItem;
		GemItem.slotIndex = kPacket.shopItems[i].slotIndex;
		GemItem.status = kPacket.shopItems[i].status;
		
		GemItem.dwVnum = kPacket.shopItems[i].dwVnum;
		GemItem.bCount = kPacket.shopItems[i].bCount;
		GemItem.dwPrice = kPacket.shopItems[i].dwPrice;

#ifdef ENABLE_BOSS_GEM_SYSTEM
		if (kPacket.gemShopType == true)
			CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem, true);
		else
			CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem, false);
#else
		CPythonPlayer::Instance().SetGemShopItemData(kPacket.shopItems[i].slotIndex, GemItem);
#endif
	}
	
#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (kPacket.gemShopType == true)
		CPythonPlayer::Instance().SetGemBossShopRefreshTime(kPacket.nextRefreshTime);
	else
		CPythonPlayer::Instance().SetGemShopRefreshTime(kPacket.nextRefreshTime);
#else
	CPythonPlayer::Instance().SetGemShopRefreshTime(kPacket.nextRefreshTime);
#endif

#ifdef ENABLE_BOSS_GEM_SYSTEM
	if (kPacket.gemShopType == true)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGemShop", Py_BuildValue("(i)", 1));
	else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGemShop", Py_BuildValue("(i)", 0));
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGemShop", Py_BuildValue("()"));
#endif

	return true;
}

bool CPythonNetworkStream::SendGemShopBuy(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;
	
	TPacketCGGemShop PacketGemShop;
	PacketGemShop.header = HEADER_CG_GEM_SHOP;
	PacketGemShop.subheader = GEM_SHOP_SUBHEADER_CG_BUY;
	
	if (!Send(sizeof(TPacketCGShop), &PacketGemShop))
	{
		Tracef("SendGemShopBuyPacket Error\n");
		return false;
	}

	if (!Send(sizeof(BYTE), &bPos))
	{
		Tracef("SendGemShopBuyPacket Error\n");
		return false;
	}
	
	return true;
}

bool CPythonNetworkStream::SendGemShopAdd(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;
	
	TPacketCGGemShop PacketGemShop;
	PacketGemShop.header = HEADER_CG_GEM_SHOP;
	PacketGemShop.subheader = GEM_SHOP_SUBHEADER_CG_ADD;
	
	if (!Send(sizeof(TPacketCGShop), &PacketGemShop))
	{
		Tracef("SendGemShopAddPacket Error\n");
		return false;
	}

	if (!Send(sizeof(BYTE), &bPos))
	{
		Tracef("SendGemShopAddPacket Error\n");
		return false;
	}
	
	return true;
}

bool CPythonNetworkStream::SendGemShopRefresh()
{
	if (!__CanActMainInstance())
		return true;
	
	TPacketCGGemShop PacketGemShop;
	PacketGemShop.header = HEADER_CG_GEM_SHOP;
	PacketGemShop.subheader = GEM_SHOP_SUBHEADER_CG_REFRESH;
	
	if (!Send(sizeof(TPacketCGShop), &PacketGemShop))
	{
		Tracef("SendGemShopAddPacket Error\n");
		return false;
	}
	
	return true;
}
#endif

#ifdef ENABLE_MAIL_BOX_SYSTEM

bool CPythonNetworkStream::RecvMailBox()
{
	TPacketGCMailBoxReceive KMailBox;

	if (!Recv(sizeof(KMailBox), &KMailBox))
		return false;

	switch (KMailBox.bSubType)
	{
		case MAILBOX_SUB_HEADER_OPEN:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_OPEN", Py_BuildValue("()"));
			break;

		case MAILBOX_SUB_HEADER_RECEIVE_INFO:
			#ifdef USE_TRANSMUTATION_SYSTEM_MAILBOX
				#ifdef USE_WON_SYSTEM_MAILBOX
					//Won and Transmutacion
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_INFO", Py_BuildValue("(isssiiiiiiii)",
						KMailBox.id_mail , KMailBox.nombre_pj,  KMailBox.asunto,  KMailBox.descrip,  KMailBox.item_vnum,  KMailBox.item_count, KMailBox.transmutation,  KMailBox.gold, KMailBox.won, KMailBox.time, KMailBox.check_acept, KMailBox.check_view
					));
				#else
					//Transmutacion
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_INFO", Py_BuildValue("(isssiiiiiii)",
						KMailBox.id_mail , KMailBox.nombre_pj,  KMailBox.asunto,  KMailBox.descrip,  KMailBox.item_vnum,  KMailBox.item_count, KMailBox.transmutation,  KMailBox.gold, KMailBox.time, KMailBox.check_acept, KMailBox.check_view
					));
				#endif
			#else
				#ifdef USE_WON_SYSTEM_MAILBOX
					//Won
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_INFO", Py_BuildValue("(isssiiiiiii)",
						KMailBox.id_mail , KMailBox.nombre_pj,  KMailBox.asunto,  KMailBox.descrip,  KMailBox.item_vnum,  KMailBox.item_count, KMailBox.gold, KMailBox.won, KMailBox.time, KMailBox.check_acept, KMailBox.check_view
					));
				#else
					//None
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_INFO", Py_BuildValue("(isssiiiiii)",
						KMailBox.id_mail , KMailBox.nombre_pj,  KMailBox.asunto,  KMailBox.descrip,  KMailBox.item_vnum,  KMailBox.item_count, KMailBox.gold, KMailBox.time, KMailBox.check_acept, KMailBox.check_view
					));

				#endif
			#endif

			if (KMailBox.item_vnum != 0){
				
				for (int j = 0; j < ITEM_SOCKET_SLOT_MAX_NUM; ++j)
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_ITEM_SOCKET", Py_BuildValue("(iii)", KMailBox.id_mail, j, KMailBox.alSockets[j]));

				for (int k = 0; k < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++k)
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_RECEIVE_ITEM_ATTR", Py_BuildValue("(iiii)",KMailBox.id_mail,k,KMailBox.aAttr[k].bType,KMailBox.aAttr[k].sValue));
				
			}
			
			break;

		case MAILBOX_SUB_HEADER_CHECK_NAME_SUCCESSFUL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_CHECK_NAME_SUCCESSFUL", Py_BuildValue("()"));
			break;

		case MAILBOX_SUB_HEADER_SEND_MAIL_SUCCESSFUL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_SEND_MAIL_SUCCESSFUL", Py_BuildValue("()"));
			break;

		case MAILBOX_SUB_HEADER_CHECK_ACCEPT_SUCCESSFUL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_ACCEPT_SUCCESSFUL", Py_BuildValue("(i)",KMailBox.id_mail));
			break;

		case MAILBOX_SUB_HEADER_CHECK_DELETE_SUCCESSFUL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_DELETE_SUCCESSFUL", Py_BuildValue("(i)",KMailBox.id_mail));
			break;

		case MAILBOX_SUB_HEADER_LOADING_ACCEPT:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_SEND_LOADING_ACCEPT", Py_BuildValue("()"));
			break;

		case MAILBOX_SUB_HEADER_LOADING_DELETE:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_SEND_LOADING_DELETE", Py_BuildValue("()"));
			break;

		case MAILBOX_SUB_HEADER_CHECK_VIEW_SUCCESSFUL:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_MAILBOX_CHECK_VIEW_SUCCESSFULL", Py_BuildValue("(i)",KMailBox.id_mail));
			break;


	}

	return true;
}
#endif