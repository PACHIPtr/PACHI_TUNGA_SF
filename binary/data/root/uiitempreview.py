import dbg
import player
import item
import net
import snd
import ui
import uiToolTip
import localeInfo
import chat

class PreviewGui(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadScript()
		self.seller_vid = 0
		self.shopPos = 0

	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/itempreviewgui.py")

		except:
			import exception
			exception.Abort("AttachStoneDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.TitleName = self.GetChild("TitleName")
			self.Question = self.GetChild("Question")
			self.accept = self.GetChild("AcceptButton")
			self.cancel = self.GetChild("CancelButton")
			self.accept.SetEvent(ui.__mem_func__(self.Accept))
			self.cancel.SetEvent(ui.__mem_func__(self.Close))
		except:
			import exception
			exception.Abort("AttachStoneDialog.__LoadScript.BindObject")

		oldToolTip = uiToolTip.ItemToolTip()
		oldToolTip.SetParent(self)
		oldToolTip.SetPosition(15, 38)
		oldToolTip.SetFollow(False)
		oldToolTip.Show()
		self.oldToolTip = oldToolTip
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.TitleName.SetText("Nesne Ön Ýzleme")
		self.accept.SetText("Göster")
		self.cancel.SetText("Kapat")
		self.Question.SetText("Eþya satýn almayý onaylýyor musun?")

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.board = 0
		self.titleBar = 0
		self.toolTip = 0
		self.seller_vid = 0
		self.shopPos = 0
		
	def Open(self, seller, seller_vid, item_vnum, item_count, socket_list, attr_list, transmutation, price, price2, shopPos):
		self.seller_vid = seller_vid
		self.shopPos = shopPos
		self.oldToolTip.ClearToolTip()
		self.oldToolTip.SetDefaultFontName(localeInfo.UI_DEF_FONT_LARGE)
		self.oldToolTip.AutoAppendTextLine("|cFFCD853F%s|r" % seller)
		self.oldToolTip.AppendSpace(3)
		self.oldToolTip.AddItemData(item_vnum, socket_list, attr_list, 0, 0, 1, 0, -1, transmutation)
		self.oldToolTip.AppendSellInfoText()
		if int(price2) > 0:
			self.oldToolTip.AppendSellingChequePrice(price2)	
		if int(price) > 0:
			self.oldToolTip.AppendPrice(int(price))

		self.oldToolTip.AppendSpace(3)
		self.oldToolTip.AppendTextLine("|cFF008B8B%s Adet|r" % (int(item_count)))

		self.UpdateDialog()
		self.SetTop()
		self.SetCenterPosition()
		self.Show()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True

	def UpdateDialog(self):
		newWidth = self.oldToolTip.GetWidth() + 10 + 20
		newHeight = self.oldToolTip.GetHeight() + 98

		self.board.SetSize(newWidth, newHeight)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def Accept(self):
		net.SendPrivateShopSerchBuyItem(self.seller_vid, self.shopPos)
		self.Close()

	def Close(self):
		self.Hide()
