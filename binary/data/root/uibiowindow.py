import app
import chr
import chrmgr
import player
import net
import pack
import background
import chat
import textTail
import event
import effect
import systemSetting
import quest
import guild
import skill
import messenger
import exchange
import ime
import item
import wndMgr, uiCommon, ui, time, playersettingmodule, localeInfo, snd, mouseModule, constInfo, uiScriptLocale, interfaceModule, dbg, uiToolTip

#################################################
#################################################

class BiologWnd(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.Initialize()
		self.LoadWindow()

	def Initialize(self):
		self.wndBioSlot = None
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/biowindow.py")
		except:
			import exception
			exception.Abort("BiologWnd.LoadWindow.LoadObject")
		try:
			self.titleBar = self.GetChild("titlebar")
			wndBioSlot = self.GetChild("BeltInventorySlot")
			self.counttext = self.GetChild("count")
			self.sure = self.GetChild("time")
			self.verbutton = self.GetChild("biobutton")

		except:
			import exception
			exception.Abort("BiologWnd.LoadWindow.BindObject")

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.verbutton.SetEvent(ui.__mem_func__(self.vergitsin))
		wndBioSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem2))
		wndBioSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem2))
		self.wndBioSlot = wndBioSlot

	def Destroy(self):
		self.ClearDictionary()

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
		
	def vergitsin(self):
		net.SendChatPacket("/biyologver")
	
	def SetBiyolog(self, bioitem, verilen, toplam, kalansure):
		constInfo.gerekli = 0
		constInfo.kalan = 0
		constInfo.verilen = 0
		constInfo.toplam = 0
	
		constInfo.gerekli = int(bioitem)
		constInfo.kalan = int(kalansure)
		constInfo.verilen = int(verilen)
		constInfo.toplam = int(toplam)
		self.RefreshSlot()

	def OverInItem2(self):
		itemIndex = int(constInfo.gerekli)
		item.SelectItem(itemIndex)
		
		taslarr = [player.GetItemMetinSocket(999, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
		efsunlarr = [player.GetItemAttribute(999, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

		self.tooltipItem.ClearToolTip()
		self.tooltipItem.AddRefineItemData(itemIndex, taslarr, efsunlarr)
		self.tooltipItem.Show()

	def OverOutItem2(self):
		self.tooltipItem.ClearToolTip()
		self.tooltipItem.Hide()

	def RefreshSlot(self):
		self.counttext.SetText(str(constInfo.verilen)+"/"+str(constInfo.toplam))
		getItemVNum= constInfo.gerekli

		if getItemVNum > 0:
			self.wndBioSlot.SetItemSlot(999, getItemVNum)
		else:	
			self.wndBioSlot.ClearSlot(999)
			return
		self.wndBioSlot.RefreshSlot()		
		
		
	def OnUpdate(self):
		self.RefreshSlot()
		import time
		current_milli_time = int(app.GetGlobalTimeStamp())
		if int(constInfo.kalan) == 0:
			self.sure.SetText("Verilebilir!")
		elif (int(constInfo.kalan)-current_milli_time <= 0):
			self.sure.SetText("Verilebilir!")
		else:
			self.sure.SetText(str(localeInfo.SecondToDHM(int(constInfo.kalan)-current_milli_time)))
			
		
	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True
