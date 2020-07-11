import player
import net
import app
import item
import chat
import ime
import os
import ui
import mouseModule
import snd
import grp
import uiScriptLocale
import localeInfo
import constInfo
import wndMgr
import uiCommon

class PetFeedWindow(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.is_showed = False
		self.arryfeed = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
		self.arrysize = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		self.questionDialog = None
		if app.WJ_TRADABLE_ICON:
			self.lockedItems = {i:(-1,-1) for i in range(25)}
		self.interface = None
		self.__LoadWindow()
		

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def IsShow(self):
		return self.is_showed

	def Show(self):
		self.is_showed = True
		if app.WJ_TRADABLE_ICON:
			self.SetInven(self.interface.GetInventoryHandle())
			self.interface.SetOnTopWindow(player.ON_TOP_WND_PET_FEED)
			self.interface.RefreshMarkInventoryBag()
		ui.ScriptWindow.Show(self)

	def Close(self):
		for x in range(len(self.arryfeed)):
			self.arryfeed[x] = -1
			self.arrysize[x] = 0
			self.petslot.ClearSlot(x)
			self.petslot.RefreshSlot()
		self.Hide()
		self.is_showed = False
		if app.WJ_TRADABLE_ICON:
			if self.interface:
				for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if itemInvenPage == self.interface.GetInventoryHandle().GetInventoryPageIndex():
						self.interface.GetInventoryHandle().wndItem.SetCanMouseEventSlot(itemSlotPos)
				self.lockedItems = {i:(-1,-1) for i in range(25)}
				self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
				self.interface.RefreshMarkInventoryBag()
	
	if app.WJ_TRADABLE_ICON:
		def CantAddItem(self, slotPos):
			itemIndex = player.GetItemIndex(slotPos)
			
			if itemIndex != 0:
				itemIsAntiFlagBySlot = player.IsAntiFlagBySlot(player.INVENTORY, slotPos, item.ANTIFLAG_SELL)
				if itemIsAntiFlagBySlot:
					return True
			
			return False

		def OnTop(self):
			self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
			self.interface.RefreshMarkInventoryBag()
	
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PetFeedWindow.py")
		except:
			import exception as exception
			exception.Abort("PetFeedWindow.LoadWindow.LoadObject")
			
		try:
			self.petfeed = self.GetChild("board")
			self.closebtn = self.GetChild("PetFeed_TitleBar")
			self.petslot = self.GetChild("FeedItemSlot")
			self.feedbtn = self.GetChild("FeedButton")
			
			##PetSlot
			
			self.petslot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
			self.petslot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
			#self.petslot.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
			#self.petslot.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
			
			##Event secondari
			
			self.feedbtn.SetEvent(ui.__mem_func__(self.SendPetItem))
			self.closebtn.SetCloseEvent(self.Close)
			
			
		except:
			import exception as exception
			exception.Abort("PetFeedWindow.LoadWindow.BindObject")
			
	def OnCloseQuestionDialog(self):
		if self.questionDialog:
			self.questionDialog.Close()

		self.questionDialog = None
	
	def SelectEmptySlot(self, selectedSlotPos):
		#chat.AppendChat(chat.CHAT_TYPE_INFO, "Empty"+str(selectedSlotPos))		

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
			if attachedItemCount > 1:
				return
			if attachedItemCount == 1:
				attachedItemCount = 0
			item.SelectItem(attachedItemIndex)
			(width, height) = item.GetItemSize()
			#chat.AppendChat(chat.CHAT_TYPE_INFO, "Empty"+str(width)+"a"+str(height))			
			if player.SLOT_TYPE_INVENTORY == attachedSlotType and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, selectedSlotPos):
				itemCount = player.GetItemCount(attachedSlotPos)
				attachedCount = mouseModule.mouseController.GetAttachedItemCount()
				self.arryfeed[selectedSlotPos] = attachedSlotPos
				self.arrysize[selectedSlotPos] = height
				self.petslot.SetItemSlot(selectedSlotPos, attachedItemIndex, attachedItemCount)
				if app.WJ_TRADABLE_ICON:
					self.CantTradableItem(selectedSlotPos, attachedSlotPos)
					self.Refresh()
					
			mouseModule.mouseController.DeattachObject()
	
	def SelectItemSlot(self, itemSlotIndex):
		#chat.AppendChat(chat.CHAT_TYPE_INFO, "Select"+str(itemSlotIndex))
		self.arryfeed[itemSlotIndex] = -1
		self.arrysize[itemSlotIndex] = 0
		self.petslot.ClearSlot(itemSlotIndex)
		self.petslot.RefreshSlot()
		if app.WJ_TRADABLE_ICON:
			(itemInvenPage, itemSlotPos) = self.lockedItems[itemSlotIndex]
			if itemInvenPage == self.wndInventory.GetInventoryPageIndex():
				self.interface.GetInventoryHandle().wndItem.SetCanMouseEventSlot(itemSlotPos)

			self.lockedItems[itemSlotIndex] = (-1, -1)
			self.Refresh()
		
	def CanMoveItem(self, size, destindex):
		for x in range(0, len(self.arrysize)):
			if self.arrysize[x] > 0:
				for y in range(0, self.arrysize[x]):
					if x+(y*5) == destindex:
						return False
		return True
		
	def AddPetItemFromInventory(self, attachedSlotPos, itemIndex, itemCount):
		item.SelectItem(itemIndex)
		(width, height) = item.GetItemSize()
		if self.arryfeed[0] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 0):
			self.arryfeed[0] = attachedSlotPos
			self.arrysize[0] = height
			self.petslot.SetItemSlot(0, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(0, attachedSlotPos)
		elif self.arryfeed[1] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 1):
			self.arryfeed[1] = attachedSlotPos
			self.arrysize[1] = height
			self.petslot.SetItemSlot(1, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(1, attachedSlotPos)
		elif self.arryfeed[2] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 2):
			self.arryfeed[2] = attachedSlotPos
			self.arrysize[2] = height
			self.petslot.SetItemSlot(2, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(2, attachedSlotPos)
		elif self.arryfeed[3] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 3):
			self.arryfeed[3] = attachedSlotPos
			self.arrysize[3] = height
			self.petslot.SetItemSlot(3, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(3, attachedSlotPos)
		elif self.arryfeed[4] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 4):
			self.arryfeed[4] = attachedSlotPos
			self.arrysize[4] = height
			self.petslot.SetItemSlot(4, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(4, attachedSlotPos)
		elif self.arryfeed[5] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 5):
			self.arryfeed[5] = attachedSlotPos
			self.arrysize[5] = height
			self.petslot.SetItemSlot(5, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(5, attachedSlotPos)
		elif self.arryfeed[6] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 6):
			self.arryfeed[6] = attachedSlotPos
			self.arrysize[6] = height
			self.petslot.SetItemSlot(6, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(6, attachedSlotPos)
		elif self.arryfeed[7] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 7):
			self.arryfeed[7] = attachedSlotPos
			self.arrysize[7] = height
			self.petslot.SetItemSlot(7, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(7, attachedSlotPos)
		elif self.arryfeed[8] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 8):
			self.arryfeed[8] = attachedSlotPos
			self.arrysize[8] = height
			self.petslot.SetItemSlot(8, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(8, attachedSlotPos)
		elif self.arryfeed[9] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 9):
			self.arryfeed[9] = attachedSlotPos
			self.arrysize[9] = height
			self.petslot.SetItemSlot(9, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(9, attachedSlotPos)
		elif self.arryfeed[10] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 10):
			self.arryfeed[10] = attachedSlotPos
			self.arrysize[10] = height
			self.petslot.SetItemSlot(10, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(10, attachedSlotPos)
		elif self.arryfeed[11] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 11):
			self.arryfeed[11] = attachedSlotPos
			self.arrysize[11] = height
			self.petslot.SetItemSlot(11, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(11, attachedSlotPos)
		elif self.arryfeed[12] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 12):
			self.arryfeed[12] = attachedSlotPos
			self.arrysize[12] = height
			self.petslot.SetItemSlot(12, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(12, attachedSlotPos)
		elif self.arryfeed[13] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 13):
			self.arryfeed[13] = attachedSlotPos
			self.arrysize[13] = height
			self.petslot.SetItemSlot(13, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(13, attachedSlotPos)
		elif self.arryfeed[14] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 14):
			self.arryfeed[14] = attachedSlotPos
			self.arrysize[14] = height
			self.petslot.SetItemSlot(14, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(14, attachedSlotPos)
		elif self.arryfeed[15] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 15):
			self.arryfeed[15] = attachedSlotPos
			self.arrysize[15] = height
			self.petslot.SetItemSlot(15, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(15, attachedSlotPos)
		elif self.arryfeed[16] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 16):
			self.arryfeed[16] = attachedSlotPos
			self.arrysize[16] = height
			self.petslot.SetItemSlot(16, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(16, attachedSlotPos)
		elif self.arryfeed[17] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 17):
			self.arryfeed[17] = attachedSlotPos
			self.arrysize[17] = height
			self.petslot.SetItemSlot(17, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(17, attachedSlotPos)
		elif self.arryfeed[18] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 18):
			self.arryfeed[18] = attachedSlotPos
			self.arrysize[18] = height
			self.petslot.SetItemSlot(18, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(18, attachedSlotPos)
		elif self.arryfeed[19] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 19):
			self.arryfeed[19] = attachedSlotPos
			self.arrysize[19] = height
			self.petslot.SetItemSlot(19, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(19, attachedSlotPos)
		elif self.arryfeed[20] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 20):
			self.arryfeed[20] = attachedSlotPos
			self.arrysize[20] = height
			self.petslot.SetItemSlot(20, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(20, attachedSlotPos)
		elif self.arryfeed[21] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 21):
			self.arryfeed[21] = attachedSlotPos
			self.arrysize[21] = height
			self.petslot.SetItemSlot(21, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(21, attachedSlotPos)
		elif self.arryfeed[22] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 22):
			self.arryfeed[22] = attachedSlotPos
			self.arrysize[22] = height
			self.petslot.SetItemSlot(22, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(22, attachedSlotPos)
		elif self.arryfeed[23] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 23):
			self.arryfeed[23] = attachedSlotPos
			self.arrysize[23] = height
			self.petslot.SetItemSlot(23, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(23, attachedSlotPos)
		elif self.arryfeed[24] == -1 and not attachedSlotPos in self.arryfeed and self.CanMoveItem(height, 24):
			self.arryfeed[24] = attachedSlotPos
			self.arrysize[24] = height
			self.petslot.SetItemSlot(24, itemIndex, itemCount)
			if app.WJ_TRADABLE_ICON:
				self.CantTradableItem(24, attachedSlotPos)
	def BindInterface(self, interface):
		self.interface = interface
	
	#def UseItemSlot(self, slotIndex):
		#chat.AppendChat(chat.CHAT_TYPE_INFO, "Select"+str(slotIndex))
	
	def OnUpdate(self):
		if app.WJ_TRADABLE_ICON:
			self.RefreshLockedSlot()
	
	def SendPetItem(self):
		for i in range(len(self.arryfeed)):
			if self.arryfeed[i] != -1:
				#chat.AppendChat(chat.CHAT_TYPE_INFO, "Oggetto inviato in pos"+str(i))
				net.SendChatPacket("/cubepetadd add %d %d" % (i, self.arryfeed[i]))
		net.SendChatPacket("/feedcubepet %d" % (constInfo.FEEDWIND))
		for x in range(len(self.arryfeed)):
			self.arryfeed[x] = -1
			self.arrysize[x] = 0
			self.petslot.ClearSlot(x)
			self.petslot.RefreshSlot()
		
	def ConfirmPetItem(self):
		for i in range(len(self.arryfeed)):
			if self.arryfeed[i] != -1:
				#chat.AppendChat(chat.CHAT_TYPE_INFO, "Oggetto inviato in pos"+str(i))
				net.SendChatPacket("/cubepetadd add %d %d" % (i, self.arryfeed[i]))
		net.SendChatPacket("/feedcubepet %d" % (constInfo.FEEDWIND))
		for x in range(len(self.arryfeed)):
			self.arryfeed[x] = -1
			self.arrysize[x] = 0
			self.petslot.ClearSlot(x)
			self.petslot.RefreshSlot()
		self.OnCloseQuestionDialog()

	if app.WJ_TRADABLE_ICON:
		def CantTradableItem(self, destSlotIndex, srcSlotIndex):
			itemInvenPage = srcSlotIndex / player.INVENTORY_PAGE_SIZE
			localSlotPos = srcSlotIndex - (itemInvenPage * player.INVENTORY_PAGE_SIZE)
			self.lockedItems[destSlotIndex] = (itemInvenPage, localSlotPos)
			if self.interface.GetInventoryHandle().GetInventoryPageIndex() == itemInvenPage:
				self.interface.GetInventoryHandle().wndItem.SetCantMouseEventSlot(localSlotPos)

		def RefreshLockedSlot(self):
			if self.wndInventory:
				for privatePos, (itemInvenPage, itemSlotPos) in self.lockedItems.items():
					if self.interface.GetInventoryHandle().GetInventoryPageIndex() == itemInvenPage:
						self.interface.GetInventoryHandle().wndItem.SetCantMouseEventSlot(itemSlotPos)

				self.interface.GetInventoryHandle().wndItem.RefreshSlot()
				
		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)

		def OnTop(self):
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PET_FEED)
				self.interface.RefreshMarkInventoryBag()