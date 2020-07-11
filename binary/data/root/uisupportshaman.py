import net
import app
import player
import item
import chr
import shop
import chat
import snd
import wndMgr

import ui
import uiCommon
import uiToolTip
import mouseModule
import localeInfo
import constInfo

def checkdiv(n):
	x = str(n/10.0)
	if len(x) > 3:
		return str(x)[0:3]
	return str(x)

def pointop(n):
	t = int(n)
	if t / 10 < 1:
		return "0."+n
	else:		
		return n[0:len(n)-1]+"."+n[len(n)-1:]

class SupportShamanDialog(ui.ScriptWindow):
	class TextToolTip(ui.Window):
		def __init__(self, y):
			ui.Window.__init__(self, "TOP_MOST")

			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetHorizontalAlignLeft()
			textLine.SetOutline()
			textLine.Show()
			self.y = y
			self.textLine = textLine

		def __del__(self):
			ui.Window.__del__(self)

		def SetText(self, text):
			self.textLine.SetText(text)

		def OnRender(self):
			(mouseX, mouseY) = wndMgr.GetMousePosition()
			self.textLine.SetPosition(mouseX, mouseY - 60 + self.y)

	def __init__(self, vnum = 0):
		ui.ScriptWindow.__init__(self)
		self.vnum = vnum
		self.isLoaded = 0
		self.closeQuestionDialog = None
		self.LoadWindow()

	def __del__(self):	
		ui.ScriptWindow.__del__(self)
		
	def Show(self):
		self.SetCenterPosition()
		self.SetTop()
		ui.ScriptWindow.Show(self)
		
	def LoadWindow(self):
		if (self.isLoaded == 1):
			return
			
		self.isLoaded = 1
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/supportshaman.py")
		except:
			import exception as exception
			exception.Abort("SYardimcisiDialog.LoadWindow.LoadObject")
			
		try:
			self.board = self.GetChild("Board")
			self.yardimciderece = self.GetChild("YardimciDerece")
			self.yardimcilevel = self.GetChild("YardimciLevel")
			self.yuzukicon = self.GetChild("YuzukIcon")
			self.zekaorani = self.GetChild("ZekaOrani")
			self.yardimciexpa = self.GetChild("YardimciTecrube_01")
			self.yardimciexpb = self.GetChild("YardimciTecrube_02")
			self.yardimciexpc = self.GetChild("YardimciTecrube_03")
			self.yardimciexpd = self.GetChild("YardimciTecrube_04")
			self.yardimciexparkaplan = []
			self.yardimciexparkaplan.append(self.yardimciexpa)
			self.yardimciexparkaplan.append(self.yardimciexpb)
			self.yardimciexparkaplan.append(self.yardimciexpc)
			self.yardimciexparkaplan.append(self.yardimciexpd)
			self.yardimciexpboard = self.GetChild("YardimciTecrube_Arkaplan")
			self.tooltipexp = []
			for i in range(0,4):
				self.tooltipexp.append(self.TextToolTip(15*i))
				self.tooltipexp[i].Hide()

			for exp in self.yardimciexparkaplan:
				exp.SetSize(0, 0)

		except:
			import exception as exception
			exception.Abort("SYardimcisiDialog.LoadWindow.BindObject")
			
		self.ArkaplanBilgi()
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		self.yuzukicon.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))

	def SetYuzukIcon(self, vnum):
		self.yuzukicon.SetItemSlot(0, int(vnum), 0)
		self.yuzukicon.SetAlwaysRenderCoverButton(0, True)

	def SetDeneyim(self, expm, exptot):
		expm = int(expm)
		exptot = int(exptot)
		
		if exptot > 0:	
			totalexp = exptot
			totexpm = int(float(totalexp) / 100 * 100 )
			totexpi = totalexp - totexpm
			expmp =  float(expm) / totexpm * 100
		else:
			totalexp = 0
			totexpm = 0
			totexpi = 0
			expmp =  0
			
		
		curPoint = int(min(expm, totexpm))
		curPoint = int(max(expm, 0))
		maxPoint = int(max(totexpm, 0))
		
		maxPointi = int(max(totexpi, 0))

		quarterPoint = maxPoint / 4
		quarterPointi = maxPointi 
		FullCount = 0

		if 0 != quarterPoint:
			FullCount = min(4, curPoint / quarterPoint)

		for i in xrange(3):
			self.yardimciexparkaplan[i].Hide()
			
		self.yardimciexparkaplan[3].Hide()

		for i in xrange(FullCount):
			self.yardimciexparkaplan[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.yardimciexparkaplan[i].Show()

		if 0 != quarterPoint:
			if FullCount < 4:
				Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
				self.yardimciexparkaplan[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.yardimciexparkaplan[FullCount].Show()
			
		#####
		self.tooltipexp[0].SetText("Deneyim Puaný : %d/%d" % (expm, totexpm))
		self.tooltipexp[1].SetText("Deneyim Puaný : %.2f%%" % expmp)

	def ArkaplanBilgi(self):
		self.yardimciderece.SetText("")
		self.yardimcilevel.SetText("")
		self.yuzukicon.ClearSlot(0)
		self.SetDeneyim(0,0)
		self.zekaorani.SetText("")

	def BindInterfaceClass(self, interface):
		self.interface = interface
	
	def ZekaOrani(self, ints):
		self.zekaorani.SetText(ints +"%")

	def Destroy(self):
		self.ClearDictionary()
		self.interface = None
		
	def SetDerece(self, yardimciderece):
		self.yardimciderece.SetText(yardimciderece)

	def SetSeviye(self, level):
		self.yardimcilevel.SetText(level)

	def __OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			targetIndex = player.GetItemIndex(attachedSlotPos)
			if attachedSlotType != player.SLOT_TYPE_INVENTORY:
				return
				
			mouseModule.mouseController.DeattachObject()
				
			item.SelectItem(targetIndex)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			itemVnum = player.GetItemIndex(attachedSlotPos)

			self.yuzukicon.SetItemSlot(player.GetItemIndex(attachedSlotPos), player.GetItemCount(attachedSlotPos))

			for i in xrange(4):
				self.yuzukicon.ClearSlot(i)

	def OnUpdate(self):
		if True == self.yardimciexpboard.IsIn():
			for i in range(0,4):
				self.tooltipexp[i].Show()
		else:
			for i in range(0,4):
				self.tooltipexp[i].Hide()

	def Close(self):
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True
