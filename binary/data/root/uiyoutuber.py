import net
import app
import chat
import systemSetting
import ui
import uiYoutubeFavList

class PopupDialog(ui.ScriptWindow):
	def __init__(self, parent):
		ui.ScriptWindow.__init__(self)
		self.__Load()
		self.__Bind()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __Load(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")
		except:
			import exception as exception
			exception.Abort("PopupDialog.__Load")

	def __Bind(self):
		try:
			self.textLine=self.GetChild("message")
			self.okButton=self.GetChild("accept")
		except:
			import exception as exception
			exception.Abort("PopupDialog.__Bind")
		self.okButton.SAFE_SetEvent(self.__OnOK)

	def Open(self, msg):
		self.textLine.SetText(msg)
		self.SetCenterPosition()
		self.Show()
		self.SetTop()

	def __OnOK(self):
		self.Hide()

class YoutubeWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadWindow()
		self.interface = None
		self.BoolFaveri = False

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Close(self):
		self.Hide()

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/youtuber.py")
		except:
			import exception as exception
			exception.Abort("YoutubeWindow.LoadWindow")

		try:
			self.petfeed = self.GetChild("board")
			self.closebtn = self.GetChild("TitleYoutube")
			self.TitleName = self.GetChild("TitleName")

			self.FavoriButton = self.GetChild("Favori")
			self.okButton = self.GetChild("okButton")
			self.viewButton = self.GetChild("viewButton")
			self.FavList = self.GetChild("FavList")
			self.editline = self.GetChild("InputValue2")
			self.editline2 = self.GetChild("InputValue")
			self.button3 = self.GetChild("button3")

			self.FavoriButton.SetToggleUpEvent(self.FavoriListUp)
			self.FavoriButton.SetToggleDownEvent(self.FavoriListDown)

			self.okButton.SAFE_SetEvent(self.Tamamla)
			self.viewButton.SAFE_SetEvent(self.Onizleme)
			self.FavList.SAFE_SetEvent(self.FavListeBak)
			self.button3.SAFE_SetEvent(self.Close)
			self.editline.OnPressEscapeKey = self.Close
			self.editline2.OnPressEscapeKey = self.Close
		except:
			import exception as exception
			exception.Abort("PetItemWindow.LoadWindow")

		self.popupDialog=PopupDialog(self)
		self.SetTop()
		self.SetCenterPosition()

	def Show(self):
		ui.ScriptWindow.Show(self)
		self.SetTop()
		self.editline2.SetFocus()
		self.BoolFaveri = False
		self.FavoriButton.SetUp()

	def BindInterfaceClass(self, interface):
		self.interface = interface

	def FavoriListUp(self):
		self.BoolFaveri = False
		self.FavoriButton.SetUp()

	def FavoriListDown(self):
		self.BoolFaveri = True
		self.FavoriButton.Down()

	def Tamamla(self):
		if (self.interface):
			val = int(len(self.editline.GetText()))
			url=str(self.editline.GetText())
			if val < 3:
				chat.AppendChat(5, "Link çok kýsa")
				return

			if val > 25:
				chat.AppendChat(5, "Link çok büyük")
				return

			val2 = int(len(self.editline2.GetText()))
			baslik=str(self.editline2.GetText())

			if val2 < 3:
				chat.AppendChat(5, "Baþlýk çok kýsa")
				return

			if val2 > 25:
				chat.AppendChat(5, "Baþlýk çok uzun (Max 25 Karakter)")
				return

			if url.lower().find('%') != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Linkte % iþareti kullanamazsýnýz!")
				return

			if url.lower().find("'") != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Linkte ' iþareti kullanamazsýnýz!")
				return

			if url.lower().find(' ') != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Linkte boþluk iþareti kullanamazsýnýz!")
				return

			if baslik.lower().find("'") != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Baþlýkta % iþareti kullanamazsýnýz!")
				return

			if baslik.lower().find('%') != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Baþlýkta ' iþareti kullanamazsýnýz!")
				return

			if baslik.lower().find(" ") != -1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Baþlýkta boþluk iþareti kullanamazsýnýz!")
				return

		self.interface.YeniLink()
		text = "  Videoyu izlemek için %s'a/e týklayýn!" % (baslik)
		you = '|cffed475c[YOU'
		tube = '|cffffffffTUBE]'
		ayir = ' : '
		net.SendChatPacket(str(you)+str(tube)+str(ayir)+'|cffffc700|Hyoutube:'+str(url)+'|h['+str(baslik)+']|h|r'+text[1:], chat.CHAT_TYPE_SHOUT)

		if self.BoolFaveri == True:
			net.SendYoutuberPanelPacket("olustur", baslik, url)

		self.Close()

	def Onizleme(self):
		if (self.interface):
			val = int(len(self.editline.GetText()))
			if val > 1:
				url = "http://ruya2.com/videos.php?youtube=%s" % str(self.editline.GetText())
				self.interface.OpenYoutubeWebWindow(url)
			else:
				chat.AppendChat(5, "Link çok kýsa")

	def FavListeBak(self):
		self.favlistdialog=uiYoutubeFavList.FavListSelectDialog()
		self.favlistdialog.Open()
		self.Close()

	def __PopupMessage(self, msg):
		self.popupDialog.Open(msg)
