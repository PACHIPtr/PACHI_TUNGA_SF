import ui
import chat
import uiCommon
import wndMgr
import constInfo
import mouseModule
import item
import player
import net
import uiPickMoney as uiPickMoney
import uiToolTip
import localeInfo
import app

ROOT = "d:/ymir work/ui/game/mailbox/"
SLOTS_MAX = 9

class MailBox(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.popup = uiCommon.PopupDialog()
		self.questionDialog = None
		self.list_mail = []
		self.SLOT_ACTUAL = 1
		self.itemDataDict = {}
		self.PAGES_FUNC = {}
		for i in xrange(0,2):
			self.PAGES_FUNC[i] = False

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/mailbox.py")
		except:
			import exception
			exception.Abort("MailBox.LoadWindow.LoadObject")

		try:
			self.board = self.GetChild("post_list_window")
			self.prev_button = self.GetChild("prev_button")
			self.next_button = self.GetChild("next_button")
			self.post_all_delete_button = self.GetChild("post_all_delete_button")
			self.post_all_receive_button = self.GetChild("post_all_receive_button")
			self.post_write_button = self.GetChild("post_write_button")

		except:
			import exception
			exception.Abort("MailBox.LoadWindow")

		self.prev_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_RIGHT')
		self.next_button.SetEvent(self.__OnClickArrow, 'MOSTBOUGHT_LEFT')
		self.post_all_delete_button.SetEvent(self.__DeleteMails)
		self.post_all_receive_button.SetEvent(self.__RecibeMails)
		self.post_write_button.SetEvent(self.__SendMail)

		self.GetChild("board").SetCloseEvent(self.Close)

		self.ListMail = MailBoxList()
		self.ListMail.Open(self.board,5,7)
		self.ListMail.Show()

		self._gui_mailbox_send = MailBoxSend()
		self._gui_mailbox_send.LoadWindow()

	def Open(self):
		self.Show()

	def Destroy(self):
		self.Clear()

	def Clear(self):
		self.list_mail = []
		self.itemDataDict = {}

	if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
		if app.ENABLE_WON_MAIL_BOX_SYSTEM:
			def MailList(self,id,name,title,descrip,item,count,transmutation,gold,won,time,check,check_view):
				self.list_mail.append([int(id),name,title,descrip,int(item),int(count),int(gold),int(time),int(check),int(check_view),int(transmutation),int(won)])
		else:
			def MailList(self,id,name,title,descrip,item,count,transmutation,gold,time,check,check_view):
				self.list_mail.append([int(id),name,title,descrip,int(item),int(count),int(gold),int(time),int(check),int(check_view),int(transmutation)])
	else:
		if app.ENABLE_WON_MAIL_BOX_SYSTEM:
			def MailList(self,id,name,title,descrip,item,count,gold,won,time,check,check_view):
				self.list_mail.append([int(id),name,title,descrip,int(item),int(count),int(gold),int(time),int(check),int(check_view),int(won)])
		else:
			def MailList(self,id,name,title,descrip,item,count,gold,time,check,check_view):
				self.list_mail.append([int(id),name,title,descrip,int(item),int(count),int(gold),int(time),int(check),int(check_view)])

	def MailList2(self,id):
		emptySocketList = []
		emptyAttrList = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			emptySocketList.append(0)
		emptyAttrList = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			emptyAttrList.append((0, 0))
		self.itemDataDict[id] = (emptySocketList,emptyAttrList)

	def MailListAttrs(self,id,attr,type,value):
		self.itemDataDict[id][1][attr] = (type, value)

	def MailListSockets(self,id,socket,value):
		self.itemDataDict[id][0][socket] = value

	def LoadList(self):
		self.ListMail.Clear()

		for a in xrange(min(SLOTS_MAX , len(self.list_mail) - self.SLOT_ACTUAL * SLOTS_MAX  + SLOTS_MAX )):
			list_m = self.list_mail[a + (self.SLOT_ACTUAL - 1)*SLOTS_MAX]

			if list_m[4] != 0:
				attr = self.itemDataDict[list_m[0]][1]
				socket = self.itemDataDict[list_m[0]][0]
			else:
				attr = 0
				socket = 0

			SLOT_ACTIVE = a

			self.ListMail.SetList(SLOT_ACTIVE,list_m,attr,socket)

		if self.SLOT_ACTUAL > 1:
			self.PAGES_FUNC[1] = True
		else:
			self.PAGES_FUNC[1] = False

		if self.SLOT_ACTUAL *SLOTS_MAX >= len(self.list_mail):
			self.PAGES_FUNC[0] = False
		else:
			self.PAGES_FUNC[0] = True

	def __OnClickArrow(self, func):
		if func == 'MOSTBOUGHT_LEFT' and self.PAGES_FUNC[0]:
			self.SLOT_ACTUAL += 1
		elif func == 'MOSTBOUGHT_RIGHT' and self.PAGES_FUNC[1]:
			self.SLOT_ACTUAL -= 1
		self.LoadList()

	def CheckListDelele(self,id_mail):
		if len(self.list_mail) > 0:
			for i in xrange(0,len(self.list_mail)):
				if int(self.list_mail[i][0]) == id_mail:
					return i
		return -1

	def __SendMail(self):
		self._gui_mailbox_send.Open()

	def SendMailCheckName(self):
		self._gui_mailbox_send.CheckNameReturn(True)

	def FunctionDelete(self, index):
		id_index_list = self.CheckListDelele(index)
		del self.list_mail[id_index_list]

	def DeleteMails(self,id):
		self.FunctionDelete(id)

	def loadMailsDelete(self):
		if len(self.list_mail) <= self.SLOT_ACTUAL * SLOTS_MAX:
			self.SLOT_ACTUAL = 1
		self.LoadList()
		self.ListMail.ClosePanel()

	def AceptMails(self,index):
		id_index_list = self.CheckListDelele(index)
		self.list_mail[id_index_list][8] = 0

	def loadMailsAcept(self):
		self.LoadList()
		self.ListMail.ClosePanel()

	def CheckView(self,index):

		id_index_list = self.CheckListDelele(index)
		self.list_mail[id_index_list][9] = 0
		self.LoadList()
		
	def SendMailCheck(self):
		self._gui_mailbox_send.SendMailCheck()

	def CheckMails(self):
		if len(self.list_mail) < 1:
			return False
		return True

	def CheckAceptMails(self):
		for i in xrange(0,len(self.list_mail)):
			if self.list_mail[i][8] == 1:
				return True
		return False

	def __DeleteMails(self):
		if self.CheckMails() == False:
			self.popup.SetText("Silinecek mesaj bulunamadı.")
			self.popup.Open()
			return

		if self.CheckAceptMails() == True:
			questionDialog = uiCommon.QuestionDialog2()
			questionDialog.SetText1("Mesajı silmek istiyor musun?")
			questionDialog.SetText2("Mesajı geri getiremeyeceksin.")
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDeleteMails))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseDialog))
			questionDialog.Open()

			self.questionDialog = questionDialog

		else:
			self.OnDeleteMails(1)

	def OnDeleteMails(self,arg=0):
		net.MailBoxDeleteAllMail()
		if arg != 1:
			self.questionDialog.Close()

	def OnCloseDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None

	def __RecibeMails(self):
		if self.CheckAceptMails() == False:
			self.popup.SetText("Kabul Edilmiş bir mesaj bulunamadı.")
			self.popup.Open()
			return
			
		if self.CheckAceptMails() == True:
			net.MailBoxAcceptAllMail()

	def Close(self):
		net.MailBoxClose()
		self.Clear()
		self._gui_mailbox_send.Close()
		self.ListMail.ClosePanel()
		wndMgr.Hide(self.hWnd)

class MailBoxList(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = False
		self.slot_active = []
		self.slot_time = []

		self.info_load = []
		self.info_attr = []
		self.info_socket = []

		self.index_select = -1

		self.mailbox_r = MailBoxRead()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self,parent):
		self.ListItems(parent)

	def ListItems(self,parent):
		self.bg_mail = {}
		self.bg_icon = {}
		self.bg_title ={}
		self.bg_time = {}
		

		for i in xrange(0,SLOTS_MAX):
			self.bg_mail[i] = ui.Button()
			self.bg_mail[i].SetParent(parent)
			self.bg_mail[i].SetPosition(0,0+37*i)
			self.bg_mail[i].SetUpVisual(ROOT+"post_default.sub")
			self.bg_mail[i].SetOverVisual(ROOT+"post_over.sub")
			self.bg_mail[i].SetDownVisual(ROOT+"post_default.sub")
			self.bg_mail[i].SetEvent(self.OpenRead,i)
			self.bg_mail[i].Show()

			self.bg_icon[i] = ui.ImageBox()
			self.bg_icon[i].AddFlag("not_pick")
			self.bg_icon[i].SetParent(self.bg_mail[i])
			self.bg_icon[i].SetPosition(2,2)
			self.bg_icon[i].LoadImage(ROOT+"mailbox_icon_empty.sub")
			self.bg_icon[i].Show()

			self.bg_title[i] = ui.TextLine()
			self.bg_title[i].SetParent(self.bg_mail[i])
			self.bg_title[i].SetPosition(40,9)
			self.bg_title[i].SetText("")
			self.bg_title[i].SetHorizontalAlignLeft()
			self.bg_title[i].Show()

			self.bg_time[i] = ui.TextLine()
			self.bg_time[i].SetParent(self.bg_mail[i])
			self.bg_time[i].SetPosition(230,9)
			self.bg_time[i].SetText("")
			self.bg_time[i].Show()

	def Open(self, parent, x, y):
		if False == self.isLoaded:
			self.LoadWindow(parent)

		self.SetParent(parent)
		self.SetPosition(x,y)
		self.Show()

	def SetList(self,slot,list,attr,socket):
		icon_mail = self.IconList(list[1],list[4],list[8],list[6])

		self.slot_active.append(slot)
		self.slot_time.append(list[7])
		self.bg_icon[slot].LoadImage(icon_mail)
		self.bg_title[slot].SetText(list[2])
		
		self.info_load.append(list)
		self.info_attr.append(attr)
		self.info_socket.append(socket)
		#self.bg_time[slot].SetText(self.SetTime(list[7]))
		

	def SetTime(self, time1):
		time = time1 - app.GetGlobalTimeStamp()

		horas = int(time / 3600) ##segundos a horas
		minutos = int((time - (horas * 3600)) / 60) #segundos a minutos
		dias = int(time/86400) ##segundos a horas

		if time < 60:
			return "[%d Segundos]"%time
		if dias > 0:   #Dias
			return "[%d Dias]"%dias
		if horas == 0: #Minutos
			return "[%d Minutos]"%minutos
		if horas > 0:  #Hora
			return "[%d Horas]"%horas

	#def OnUpdate(self):
		#if len(self.slot_active) > 0:
			#count = 0
			#for i in self.slot_active:
				#self.bg_time[count].SetText(self.SetTime(int(self.slot_time[count])))
				#count += 1

	def Clear(self):
		self.slot_active = []
		self.info_load = []
		self.info_attr = []
		self.info_socket = []
		self.slot_time = []
		for i in xrange(0,SLOTS_MAX):
			self.bg_title[i].SetText("")
			self.bg_time[i].SetText("")
			self.bg_icon[i].LoadImage(ROOT+"mailbox_icon_empty.sub")

	def IconList(self,nombre,item,check,gold):

		if nombre.find("[") != -1:
			return ROOT+"mailbox_icon_gm.sub"
			
		if item != 0 and check == 1 or gold > 0:
			return ROOT+"mailbox_icon_item.sub"
		else:
			return ROOT+"mailbox_icon_post.sub"	

	def CheckSlot(self,index):
		for i in self.slot_active:
			if i == index:
				return True
		return False

	def ClosePanel(self):
		self.mailbox_r.Close()

	def LoadBoardAcept(self):
		self.OpenRead(self.index_select)

	def OpenRead(self,index):
		if self.CheckSlot(index):
			self.index_select = index
			self.mailbox_r.ClearList()
			self.mailbox_r.ReadList(self.info_load,self.info_attr,self.info_socket,index)
			self.mailbox_r.Open()

class MailBoxRead(ui.ScriptWindow):

	ID = 0
	CHECK = -1
	CHECK_VIEW = 0
	GOLD = 0
	TRANS = 0
	ITEM = 0
	list_attr = []
	list_socket = []

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.questionDialog = None
		self.toolTip = uiToolTip.ItemToolTip()
		self.LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/postread.py")
		except:
			import exception
			exception.Abort("MailBoxRead.LoadWindow.LoadObject")

		try:
			self.from_text = self.GetChild("from_text")
			self.title_text = self.GetChild("title_text")
			self.message_line = self.GetChild("message_line")
			self.delete_button = self.GetChild("delete_button")
			self.recv_button = self.GetChild("recv_button")
			self.won_text = self.GetChild("won_text")
			self.yang_text = self.GetChild("yang_text")
			self.item_slot = self.GetChild("item_slot")

		except:
			import exception
			exception.Abort("MailBoxRead.LoadWindow")

		self.delete_button.SetEvent(self.__DeleteMail)
		self.recv_button.SetEvent(self.__RecibeMail)
		self.GetChild("board").SetCloseEvent(self.Close)

		self.item_slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.item_slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		self.message_line.SetLimitWidth(235)
		self.message_line.SetMultiLine()

	def Open(self):
		if self.IsShow() != True:
			self.Show()
		
		self.CheckView()

	def CheckView(self):
		if self.CHECK_VIEW == 1:
			net.MailBoxViewMail(self.ID)
			self.CHECK_VIEW = 0

	def ClearList(self):
		if self.IsShow():

			self.ID = 0
			self.CHECK = -1
			self.CHECK_VIEW = 0
			self.GOLD = 0
			self.TRANS = 0
			self.ITEM = 0
			self.list_attr = []
			self.list_socket = []
			self.item_slot.ClearSlot(0)
			self.won_text.SetText("0")
			self.yang_text.SetText("0")

	def ReadList(self,list,attr,socket,index):
		#ID - GOLD - CHECK - CHECK_VIEW

		self.ID 		= int(list[index][0])
		self.GOLD		= int(list[index][6])
		self.CHECK 		= int(list[index][8])
		self.CHECK_VIEW = int(list[index][9])
		self.ITEM 		= int(list[index][4])

		if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
			self.TRANS 		= int(list[index][10])
		

		self.list_attr.append(attr[index])
		self.list_socket.append(socket[index])

		self.from_text.SetText(list[index][1])
		self.title_text.SetText(list[index][2])
		self.message_line.SetText(list[index][3])

		if self.LoadIsItem(list[index][4]) and self.CheckMail() == False: #Muestra el item si no ha aceptado el mensaje
			self.item_slot.SetItemSlot(0, list[index][4], list[index][5])

			if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
				self.item_slot.SetCoverButton(0, "d:/ymir work/ui/game/quest/slot_button_00.sub", "d:/ymir work/ui/game/quest/slot_button_00.sub", "d:/ymir work/ui/game/quest/slot_button_00.sub", "icon/item/ingame_convert_mark.tga", False, False)
				
				if self.TRANS > 0:
					self.item_slot.DisableCoverButton(0)
				else:
					self.item_slot.EnableCoverButton(0)

		if self.CheckMail() == False and list[index][6] != 0: #Muestra el yang si no ha aceptado el mensaje
			self.yang_text.SetText(str(list[index][6]))

		if app.ENABLE_WON_MAIL_BOX_SYSTEM:
			if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
				if self.CheckMail() == False and list[index][11] != 0: #Muestra el won si no ha aceptado el mensaje
					self.won_text.SetText(str(list[index][11]))
			else:
				if self.CheckMail() == False and list[index][10] != 0: #Muestra el won si no ha aceptado el mensaje
					self.won_text.SetText(str(list[index][10]))

	def LoadIsItem(self,vnum):
		if vnum != 0:
			return True
		return False

	def CheckMail(self):
		if self.CHECK == 1: # 1 ACTIVATE // 0 VIEW CHECK
			return False
		return True

	def __DeleteMail(self):
		if self.CheckMail() == False:
			chat.AppendChat(1,"Böyle bir mesaj bulunamadı.")
			return

		net.MailBoxDeleteMail(self.ID)

	def __RecibeMail(self):
		if self.CheckMail() == False:
			if self.GOLD != 0:
				questionDialog = uiCommon.QuestionDialog()
				questionDialog.SetText("Kabul etmek istiyor musun? (%5 Yang vergi kesilecek)")
				questionDialog.SetAcceptEvent(ui.__mem_func__(self.RecibeMail))
				questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseDialog))
				questionDialog.Open()
				self.questionDialog = questionDialog
			else:
				self.RecibeMail()

	def RecibeMail(self):
		net.MailBoxAcceptMail(self.ID)
		self.OnCloseDialog()

	def OnCloseDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None

	def OverInItem(self):
		self.toolTip.ClearToolTip()

		if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
			if self.TRANS > 0:
				item.SelectItem(self.TRANS)
				nameTransmutation = item.GetItemName()
				if item.GetItemType() == item.ITEM_TYPE_COSTUME:
					if (item.GetItemSubType() == item.COSTUME_TYPE_BODY) or (item.GetItemSubType() == item.COSTUME_TYPE_HAIR):
						malefemale = ""
						if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
							malefemale = localeInfo.FOR_FEMALE

						if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
							malefemale = localeInfo.FOR_MALE
						nameTransmutation += " ( " + malefemale +  " )"

			self.toolTip.SetInventoryItem(self.ITEM,self.list_socket[0],self.list_attr[0])
		else:
			self.toolTip.SetInventoryItem(self.ITEM,self.list_socket[0],self.list_attr[0])

	def OverOutItem(self):
		self.toolTip.Hide()

	def Close(self):
		if self.IsShow():
			self.OnCloseDialog()
			self.ClearList()
			self.Hide()

class MailBoxSend(ui.ScriptWindow):

	SLOT_ITEM_INV = -1
	YANG_SEND = 0 
	WON_SEND = 0
	NAME_SAVE = ""
	COMPROBACION = False

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.questionDialog = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/postwrite.py")
		except:
			import exception
			exception.Abort("MailBoxSend.LoadWindow.LoadObject")

		try:
			self.post_friend_scrollbar = self.GetChild("post_friend_scrollbar")
			self.post_friend_scrollbar.Hide()

			self.bg_window = self.GetChild("bg_window")

			self.to_editline = self.GetChild("to_editline")
			self.title_editline = self.GetChild("title_editline")

			self.message_line = self.GetChild("message_line")
			self.msg_count_text = self.GetChild("msg_count_text")
			self.post_friend_select_button = self.GetChild("post_friend_select_button")
			self.item_slot = self.GetChild("item_slot")

			self.yang_text = self.GetChild("yang_text")

			self.won_text = self.GetChild("won_text")

			self.button_money = self.GetChild("button_money")
			self.button_money.SetEvent(self.OpenPickMoneyDialog)

			if app.ENABLE_WON_MAIL_BOX_SYSTEM:
				self.button_won = self.GetChild("button_won")
				self.button_won.SetEvent(self.OpenPickMoneyDialog)

			self.GetChild("board").SetCloseEvent(self.Close)

			self.GetChild("mouse_over_image").Hide()

			self.post_send_button = self.GetChild("post_send_button")
			self.post_close_button = self.GetChild("post_close_button")

			self.post_write_confirm_button = self.GetChild("post_write_confirm_button")

		except:
			import exception
			exception.Abort("MailBoxSend.LoadWindow")

		self.post_friend_select_button.SetEvent(self.__OpenBoardFriends)
		self.item_slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.item_slot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))

		dlgPickMoney = uiPickMoney.PickMoneyDialog()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
		dlgPickMoney.SetTitleName("Ticaret")
		dlgPickMoney.SetMax(10)
		dlgPickMoney.Hide()
		self.dlgPickMoney = dlgPickMoney

		self.post_send_button.SetEvent(self.__SendMail)
		self.post_close_button.SetEvent(self.Close)

		self.post_write_confirm_button.SetEvent(ui.__mem_func__(self.__NameConfirmMail), "mouse_click")

		self._i_b = ui.MakeImageBox(self.bg_window,"d:/ymir work/ui/game/mailbox/post_write_confirm_default.sub",216,2)
		self._i_b.Hide()

		self.DD = DropDown(self)
		self.DD.SetToMail=self.SetToMail
		self.DD.SetPosition(45,27)
		self.DD.SetSize(117,120)

	def OpenPickMoneyDialog(self):
		if app.ENABLE_WON_MAIL_BOX_SYSTEM:
			self.dlgPickMoney.Open(player.GetElk(),player.GetCheque())
		else:
			self.dlgPickMoney.Open(player.GetElk())

	if app.ENABLE_WON_MAIL_BOX_SYSTEM:
		def OnPickMoney(self, money, cheque):
			self.YANG_SEND = int(money)
			self.WON_SEND = int(cheque)
			self.yang_text.SetText(str(money))
			self.won_text.SetText(str(cheque))
	else:
		def OnPickMoney(self, money):
			self.YANG_SEND = int(money)
			self.yang_text.SetText(str(money))

	def Open(self):
		if self.IsShow():
			self.DD.Clear()
			self.Hide()
		else:
			self.DD.AddList(constInfo.FRIENDS_LIST_MAIL)
			self.Show()

	def __OpenBoardFriends(self):
		self.DD.ExpandDrop()

	def __OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()

			mouseModule.mouseController.DeattachObject()
			if player.SLOT_TYPE_INVENTORY != attachedSlotType:
				return

			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
			count = player.GetItemCount(attachedInvenType, attachedSlotPos)
				
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)

			if item.IsAntiFlag(item.ANTIFLAG_GIVE):
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Bu eşyayı posta kutusuna ekleyemezsin.")
				return

		self.SLOT_ITEM_INV=attachedSlotPos
		self.item_slot.ClearSlot(selectedSlotPos)
		self.item_slot.SetItemSlot(selectedSlotPos, player.GetItemIndex(attachedSlotPos), player.GetItemCount(attachedSlotPos))

		if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
			itemTransmutation = player.GetItemTransmutation(attachedSlotPos)
			self.item_slot.SetCoverButton(selectedSlotPos, "d:/ymir work/ui/game/quest/slot_button_00.sub", "d:/ymir work/ui/game/quest/slot_button_00.sub", "d:/ymir work/ui/game/quest/slot_button_00.sub", "icon/item/ingame_convert_mark.tga", False, False)
			
			if itemTransmutation > 0:
				self.item_slot.DisableCoverButton(selectedSlotPos)
			else:
				self.item_slot.EnableCoverButton(selectedSlotPos)

	def __OnSelectItemSlot(self, selectedSlotPos):
		self.SLOT_ITEM_INV = -1
		self.item_slot.ClearSlot(selectedSlotPos)

	def __OnLimpiar(self):
		return

	def GetNumberText(self,number):
		if len(self.message_line.GetText()) <= 0:
			return 000
		return 000+number

	def SetToMail(self, to=""):
		self.to_editline.SetText(to)

	def ReturnValues(self,type):
		Values = {
			"nombre" : self.to_editline.GetText(),
			"asunto" : self.title_editline.GetText(),
			"descrip" : self.message_line.GetText(),
			"slot_item" : self.SLOT_ITEM_INV,
		}
		return Values[type]

	def SetValue(self,type,valor):
		Values = {
			"nombre" : self.to_editline.SetText(valor),
			"asunto" : self.title_editline.SetText(valor),
			"descrip" : self.message_line.SetText(valor),
		}

	def __SendMail(self):

		nombre = self.ReturnValues("nombre")
		asunto = self.ReturnValues("asunto")
		descrip = self.ReturnValues("descrip")
		slot_item = self.ReturnValues("slot_item")

		if self.COMPROBACION == False: #Check Nombre
			self.ChatW("Butona tıklayacak kullanıcı adını doğrulayın.")
			return
		if len(asunto) == 0:
			self.ChatW("Başlık eksik gibi görünüyor.")
			return
		elif len(descrip) == 0:
			self.ChatW("Açıklama eksik gibi görünüyor.")
			return

		if asunto.find("'") != -1 or asunto.find('"') != -1:
			self.ChatW("Başlık yanlış gibi görünüyor.")
			return

		if descrip.find("'") != -1 or descrip.find('"') != -1:
			self.ChatW("Açıklama yanlış gibi görünüyor.")
			return

		questionDialog = uiCommon.QuestionDialog2()
		questionDialog.SetText1("Mesajı Göndermek istiyor musun: %s"%self.ReturnValues("nombre"))
		questionDialog.SetText2("Ücret: 1000 yang.")

		if app.ENABLE_WON_MAIL_BOX_SYSTEM:
			questionDialog.SetAcceptEvent(lambda arg1=nombre,arg2=asunto,arg3=descrip,arg4=slot_item,arg5=self.YANG_SEND,arg6=self.WON_SEND: self.OnSendMail(arg1, arg2, arg3, arg4, arg5,arg6))
		else:
			questionDialog.SetAcceptEvent(lambda arg1=nombre,arg2=asunto,arg3=descrip,arg4=slot_item,arg5=self.YANG_SEND: self.OnSendMail(arg1, arg2, arg3, arg4, arg5))

		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseDialog))
		questionDialog.Open()

		self.questionDialog = questionDialog

	if app.ENABLE_WON_MAIL_BOX_SYSTEM:
		def OnSendMail(self,nombre,asunto,descrip,slot_item,yang,won):
			net.MailBoxMail(nombre,asunto,descrip,int(slot_item),int(yang),int(won))
			self.OnCloseDialog()
	else:
		def OnSendMail(self,nombre,asunto,descrip,slot_item,yang):
			net.MailBoxMail(nombre,asunto,descrip,int(slot_item),int(yang))
			self.OnCloseDialog()

	def OnCloseDialog(self):
		if not self.questionDialog:
			return

		self.questionDialog.Close()
		self.questionDialog = None

	def ButtonConfirmFunct(self,func):
		if func == True:
			self.post_write_confirm_button.Hide()
			self._i_b.Show()
		else:
			self.post_write_confirm_button.Show()
			self._i_b.Hide()

		self.COMPROBACION = func

	def CheckNameReturn(self, check):
		self.ButtonConfirmFunct(check)
		self.NAME_SAVE = self.ReturnValues("nombre")

	def SendMailCheck(self):
		self.Close()
	
	def __NameConfirmMail(self):
		if len(self.ReturnValues("nombre")) == 0:
			self.ChatW("Ad Doğrulanamadı.")
			return
		net.MailBoxNameCheck(self.ReturnValues("nombre"))

	def OnUpdate(self):
		if self.msg_count_text:
			self.msg_count_text.SetText("%03d / 100" %int(self.GetNumberText(len(self.message_line.GetText()))))

		if self.NAME_SAVE != self.ReturnValues("nombre"):
			self.ButtonConfirmFunct(False)

	def ClearInfo(self):
		self.SLOT_ITEM_INV = -1
		self.YANG_SEND = 0 
		self.WON_SEND = 0
		self.NAME_SAVE = ""
		self.COMPROBACION = False

		self.SetValue("nombre","")
		self.SetValue("asunto","")
		self.SetValue("descrip","")

		self.DD.Clear()

		self.ButtonConfirmFunct(False)

		self.item_slot.ClearSlot(0)

		self.yang_text.SetText("0")

		self.won_text.SetText("0")

		self.dlgPickMoney.Close()

	def ChatW(self,text):
		chat.AppendChat(1,text)

	def Close(self):
		if self.IsShow():
			self.OnCloseDialog()
			self.ClearInfo()
			self.Hide()

class DropDown(ui.Window):
	drop = False
	SetToMail = None

	def __init__(self,parent):
		ui.Window.__init__(self,"TOP_MOST")
		self.SetParentProxy(parent)

		self.bg = ui.SlotBar()
		self.bg.SetParent(self)
		self.bg.SetSize(0,0)
		self.bg.SetPosition(0,0)
		self.bg.Show()

		self.list = ui.ListBox()
		self.list.SetParent(self.bg)
		self.list.SetPosition(0,0)
		self.list.SetTextCenterAlign(False)
		self.list.SetEvent(ui.__mem_func__(self.__SelectNameList))
		self.list.Show()

	def __del__(self): 
		ui.Window.__del__(self)

	def SetSize(self,w,h):
		ui.Window.SetSize(self,w+41,h+30)
		self.bg.SetSize(w,h)
		self.list.SetSize(self.bg.GetWidth(), self.bg.GetHeight()+30)

	def SetPosition(self,w,h):
		ui.Window.SetPosition(self,w,h)
		self.bg.SetPosition(w,h)


	def ExpandDrop(self):
		if self.drop == True:
			self.drop = False
			self.Hide()
		else:
			self.drop = True
			self.Show()

	def AddList(self,list):
		count = 0
		for i in list:
			self.list.InsertItem(count, list[count])
			count += 1

	def Clear(self):
		self.list.ClearItem()

	def GetName(self):
		return self.list.GetSelectedItemText()

	def __SelectNameList(self):
		self.ExpandDrop()
		if self.SetToMail:
			self.SetToMail(self.GetName())
