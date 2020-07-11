import net
import app
import event
import chat
import ui
import localeInfo
import uiScriptLocale
import constInfo
import time
import interfaceModule

favlist = None

class PopupDialog(ui.ScriptWindow):
	def __init__(self, parent):
		print "NEW POPUP WINDOW   ----------------------------------------------------------------------------"	
		ui.ScriptWindow.__init__(self)

		self.__Load()
		self.__Bind()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print "---------------------------------------------------------------------------- DELETE POPUP WINDOW"

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

		self.okButton.SetEvent(ui.__mem_func__(self.__OnOK))

	def Open(self, msg):
		self.textLine.SetText(msg)
		self.SetCenterPosition()
		self.Show()
		self.SetTop()

	def __OnOK(self):
		self.Hide()

class FavListSelectDialog(ui.ScriptWindow):
	def __init__(self):
		print "NEW MARK LIST WINDOW   ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.selectEvent = None
		self.isLoaded=0
		self.yenile = 0
		self.yenile2 = 0
		self.yenile3 = 0
		self.ilk = 0
		self.yuzdekac = 0
		self.sil_refresh = 0
		constInfo.fav_listim = []

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print "---------------------------------------------------------------------------- DELETE MARK LIST WINDOW"

	def Show(self):
		if self.isLoaded==0:
			self.isLoaded=1

			self.__Load()

		ui.ScriptWindow.Show(self)

	def Open(self):
		self.ilk = 0
		self.Show()
		self.SetCenterPosition()
		self.SetTop()

	def Close(self):
		self.popupDialog.Hide()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def SAFE_SetSelectEvent(self, event):
		self.selectEvent=ui.__mem_func__(event)

	def __Createfavlist(self):
		favlist=ui.ListBoxEx()
		favlist.SetParent(self)
		favlist.SetPosition(15, 50)
		favlist.Show()
		return favlist

	def __Load(self):
		self.popupDialog=PopupDialog(self)

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/favlistimwindow.py")
		except:
			import exception as exception
			exception.Abort("favlist.__Load")

		try:
			self.favlist=self.__Createfavlist()
			self.__ClearFileList()
			self.favlist.SetScrollBar(self.GetChild("ScrollBar"))

			self.board=self.GetChild("board")
			self.cancelButton=self.GetChild("cancel")
			self.refreshButton=self.GetChild("refresh")
			self.silButton=self.GetChild("sil")
			self.izleButton=self.GetChild("izle")
			self.yuzde=self.GetChild("yuzdelik")
			self.yuzde.Hide()
		except:
			import exception as exception
			exception.Abort("favlist.__Bind")

		self.refreshButton.SetEvent(ui.__mem_func__(self.__OnRefresh))
		self.silButton.SetEvent(ui.__mem_func__(self.__sil))
		self.izleButton.SetEvent(ui.__mem_func__(self.__izle))
		self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCancel))
		self.board.SetCloseEvent(ui.__mem_func__(self.__OnCancel))

		self.UpdateRect()
		self.__ClearFileList()
		net.SendYoutuberPanelPacket("listem", "\0", "\0")

		if len(constInfo.fav_listim) != 0:
			for i in xrange(0, len(constInfo.fav_listim)):
				self.favlist.AppendItem(markla(constInfo.fav_listim[i].split("#")[1] + " baþlýklý video : "+constInfo.fav_listim[i].split("#")[2]))

	def __izle(self):
		secc = self.favlist.GetSelectedItem()
		if secc:
			secileen = secc.GetText()
			secileenn = secileen.split(" baþlýklý video : ")[1]
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Link: %s" % (secileenn))
			constInfo.LINK_PLUS = str(secileenn)
			constInfo.video_izle=1

	def __sil(self):
		sec = self.favlist.GetSelectedItem()
		if sec:
			secilen = sec.GetText()
			secilenn = secilen.split("baþlýklý video : ")[0]
			net.SendYoutuberPanelPacket("sil", str(secilenn), "\0")
			chat.AppendChat(chat.CHAT_TYPE_INFO, "[%s] baþlýklý videon silindi!" % str(secilenn))
			net.SendYoutuberPanelPacket("listem", "\0", "\0")
			self.sil_refresh = 1
			self.yenile3 = app.GetTime()+1
		else:
			return

	def __PopupMessage(self, msg):
		self.popupDialog.Open(msg)

	def __OnCancel(self):
		self.Hide()

	def OnUpdate(self):
		if self.sil_refresh == 1:
			if self.yenile3 < app.GetTime():
				self.sil_refresh = 0
				self.__ClearFileList()
				for i in xrange(0, len(constInfo.fav_listim)):
					self.favlist.AppendItem(markla(constInfo.fav_listim[i].split("#")[1] + " baþlýklý video : "+constInfo.fav_listim[i].split("#")[2]))

		if self.ilk == 1:
			if self.yenile > app.GetTime():
				return
			else:
				self.__ClearFileList()
				net.SendYoutuberPanelPacket("listem", "\0", "\0")

				for i in xrange(0, len(constInfo.fav_listim)):
					self.favlist.AppendItem(markla(constInfo.fav_listim[i].split("#")[1] + " baþlýklý video : "+constInfo.fav_listim[i].split("#")[2]))

				if self.favlist.IsEmpty():
					self.__PopupMessage("Favori video listeniz boþ.")

				self.yenile = app.GetTime()+300
		else:
			if self.yuzdekac < 102:
				self.GetChild("ScrollBar").Hide()
				self.GetChild("yuzdelik").Show()
				self.GetChild("yuzdelik").SetFontName("Tahoma:25")
				self.GetChild("yuzdelik").SetText("%"+str(self.yuzdekac))
				self.yuzdekac += 1
			else:
				self.GetChild("yuzdelik").Hide()
				self.GetChild("ScrollBar").Show()
				self.ilk = 1
				self.yuzdekac = 0

	def __OnRefresh(self):
		if self.yenile2 > app.GetTime():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "10 saniyede bir listeyi güncelleyebilirsiniz. Kalan Süre : "+str(int(self.yenile2)-int(app.GetTime())))
		else:
			self.__ClearFileList()
			net.SendYoutuberPanelPacket("listem", "\0", "\0")

			for i in xrange(0, len(constInfo.fav_listim)):
				self.favlist.AppendItem(markla(constInfo.fav_listim[i].split("#")[1] + " baþlýklý video : "+constInfo.fav_listim[i].split("#")[2]))

			chat.AppendChat(chat.CHAT_TYPE_INFO, "Favori videolarým Yenilendi.")
			self.yenile2 = app.GetTime()+10

	def __ClearFileList(self):
		self.favlist.RemoveAllItems()

	def __AppendFile(self, fileName):
		self.favlist.AppendItem(markla(fileName))

class YoutuberListSelectDialog(ui.ScriptWindow):
	def __init__(self):
		print "NEW MARK LIST WINDOW   ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.selectEvent = None
		self.isLoaded=0
		self.yenile = 0
		self.yenile2 = 0
		self.ilk = 0
		self.yuzdekac = 0
		constInfo.youtuber_list = []

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print "---------------------------------------------------------------------------- DELETE MARK LIST WINDOW"

	def Show(self):
		if self.isLoaded==0:
			self.isLoaded=1

			self.__Load()

		ui.ScriptWindow.Show(self)

	def Open(self):
		self.ilk = 0
		self.Show()
		self.SetCenterPosition()
		self.SetTop()

	def Close(self):
		self.popupDialog.Hide()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def SAFE_SetSelectEvent(self, event):
		self.selectEvent=ui.__mem_func__(event)

	def __Createyoutuberlist(self):
		youtuberlist=ui.ListBoxEx()
		youtuberlist.SetParent(self)
		youtuberlist.SetPosition(15, 50)
		youtuberlist.Show()
		return youtuberlist

	def __Load(self):
		self.popupDialog=PopupDialog(self)

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/youtuberlistwindow.py")
		except:
			import exception as exception
			exception.Abort("youtuberlist.__Load")

		try:
			self.youtuberlist=self.__Createyoutuberlist()
			self.__ClearFileList()
			self.youtuberlist.SetScrollBar(self.GetChild("ScrollBar"))

			self.board=self.GetChild("board")
			self.cancelButton=self.GetChild("cancel")
			self.refreshButton=self.GetChild("refresh")
			self.izleButton=self.GetChild("izle")
			self.yuzde=self.GetChild("yuzdelik")
			self.yuzde.Hide()
		except:
			import exception as exception
			exception.Abort("youtuberlist.__Bind")

		self.refreshButton.SetEvent(ui.__mem_func__(self.__OnRefresh))
		self.izleButton.SetEvent(ui.__mem_func__(self.__izle))
		self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCancel))
		self.board.SetCloseEvent(ui.__mem_func__(self.__OnCancel))

		self.UpdateRect()
		self.__ClearFileList()
		net.SendYoutubeVideoPacket("youtuber", "\0")

		if len(constInfo.youtuber_list) != 0:
			for i in xrange(0, len(constInfo.youtuber_list)):
				self.youtuberlist.AppendItem(markla(constInfo.youtuber_list[i].split("#")[1] + " isimli youtuber."))

	def __izle(self):
		secc = self.youtuberlist.GetSelectedItem()
		if secc:
			secileen = secc.GetText()
			secileenn = secileen.split(" isimli youtuber.")[0]
			self.youtuberlistdialog = YoutuberVideoSelectDialog()
			self.youtuberlistdialog.Open(secileenn)
			self.Close()

	def __PopupMessage(self, msg):
		self.popupDialog.Open(msg)

	def __OnCancel(self):
		self.Hide()

	def OnUpdate(self):
		if self.ilk == 1:
			if self.yenile > app.GetTime():
				return
			else:
				self.__ClearFileList()
				net.SendYoutubeVideoPacket("youtuber", "\0")

				for i in xrange(0, len(constInfo.youtuber_list)):
					self.youtuberlist.AppendItem(markla(constInfo.youtuber_list[i].split("#")[1] + " isimli youtuber."))

				if self.youtuberlist.IsEmpty():
					self.__PopupMessage("Youtuber listesi boþ.")

				self.yenile = app.GetTime()+300
		else:
			if self.yuzdekac < 102:
				self.GetChild("ScrollBar").Hide()
				self.GetChild("yuzdelik").Show()
				self.GetChild("yuzdelik").SetFontName("Tahoma:25")
				self.GetChild("yuzdelik").SetText("%"+str(self.yuzdekac))
				self.yuzdekac += 1
			else:
				self.GetChild("yuzdelik").Hide()
				self.GetChild("ScrollBar").Show()
				self.ilk = 1
				self.yuzdekac = 0

	def __OnRefresh(self):
		if self.yenile2 > app.GetTime():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "10 saniyede bir listeyi güncelleyebilirsiniz. Kalan Süre : "+str(int(self.yenile2)-int(app.GetTime())))
		else:
			self.__ClearFileList()
			net.SendYoutubeVideoPacket("youtuber", "\0")

			for i in xrange(0, len(constInfo.youtuber_list)):
				self.youtuberlist.AppendItem(markla(constInfo.youtuber_list[i].split("#")[1] + " isimli youtuber."))

			chat.AppendChat(chat.CHAT_TYPE_INFO, "Youtuber listesi Yenilendi.")
			self.yenile2 = app.GetTime()+10

	def __ClearFileList(self):
		self.youtuberlist.RemoveAllItems()

	def __AppendFile(self, fileName):
		self.youtuberlist.AppendItem(markla(fileName))

class YoutuberVideoSelectDialog(ui.ScriptWindow):
	def __init__(self):
		print "NEW MARK LIST WINDOW   ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.selectEvent = None
		self.isLoaded=0
		self.yenile = 0
		self.yenile2 = 0
		self.ilk = 0
		self.kisi = None
		self.yuzdekac = 0
		constInfo.youtuber_video_list = []

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print "---------------------------------------------------------------------------- DELETE MARK LIST WINDOW"

	def Show(self):
		if self.isLoaded==0:
			self.isLoaded=1

			self.__Load()

		ui.ScriptWindow.Show(self)

	def Open(self, kisi):
		self.ilk = 0
		self.kisi = kisi
		self.Show()
		self.SetCenterPosition()
		self.SetTop()

	def Close(self):
		self.popupDialog.Hide()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def SAFE_SetSelectEvent(self, event):
		self.selectEvent=ui.__mem_func__(event)

	def __Createyoutubervideolist(self):
		youtubervideolist=ui.ListBoxEx()
		youtubervideolist.SetParent(self)
		youtubervideolist.SetPosition(15, 50)
		youtubervideolist.Show()
		return youtubervideolist

	def __Load(self):
		self.popupDialog=PopupDialog(self)

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/youtubervideolistwindow.py")
		except:
			import exception as exception
			exception.Abort("youtubervideolist.__Load")

		try:
			self.youtubervideolist=self.__Createyoutubervideolist()
			self.__ClearFileList()
			self.youtubervideolist.SetScrollBar(self.GetChild("ScrollBar"))

			self.board=self.GetChild("board")
			self.cancelButton=self.GetChild("cancel")
			self.refreshButton=self.GetChild("refresh")
			self.izleButton=self.GetChild("izle")
			self.yuzde=self.GetChild("yuzdelik")
			self.yuzde.Hide()
		except:
			import exception as exception
			exception.Abort("youtubervideolist.__Bind")

		self.refreshButton.SetEvent(ui.__mem_func__(self.__OnRefresh))
		self.izleButton.SetEvent(ui.__mem_func__(self.__izle))
		self.cancelButton.SetEvent(ui.__mem_func__(self.__OnCancel))
		self.board.SetCloseEvent(ui.__mem_func__(self.__OnCancel))

		self.UpdateRect()
		self.__ClearFileList()
		net.SendYoutubeVideoPacket("video", self.kisi)

		if len(constInfo.youtuber_video_list) != 0:
			for i in xrange(0, len(constInfo.youtuber_video_list)):
				self.youtubervideolist.AppendItem(markla(constInfo.youtuber_video_list[i].split("#")[1] + " baþlýklý video : "+constInfo.youtuber_video_list[i].split("#")[2]))

	def __izle(self):
		secc = self.youtubervideolist.GetSelectedItem()
		if secc:
			secileen = secc.GetText()
			secileenn = secileen.split(" baþlýklý video : ")[1]
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Link: %s" % (secileenn))
			constInfo.LINK_PLUS = str(secileenn)
			constInfo.video_izle=1

	def __PopupMessage(self, msg):
		self.popupDialog.Open(msg)

	def __OnCancel(self):
		self.Hide()

	def OnUpdate(self):
		if self.ilk == 1:
			if self.yenile > app.GetTime():
				return
			else:
				self.__ClearFileList()
				net.SendYoutubeVideoPacket("video", self.kisi)

				for i in xrange(0, len(constInfo.youtuber_video_list)):
					self.youtubervideolist.AppendItem(markla(constInfo.youtuber_video_list[i].split("#")[1] + " baþlýklý video : "+constInfo.youtuber_video_list[i].split("#")[2]))

				if self.youtubervideolist.IsEmpty():
					self.__PopupMessage("Youtuburun hiç videosu yok.")

				self.yenile = app.GetTime()+300
		else:
			if self.yuzdekac < 102:
				self.GetChild("ScrollBar").Hide()
				self.GetChild("yuzdelik").Show()
				self.GetChild("yuzdelik").SetFontName("Tahoma:25")
				self.GetChild("yuzdelik").SetText("%"+str(self.yuzdekac))
				self.yuzdekac += 1
			else:
				self.GetChild("yuzdelik").Hide()
				self.GetChild("ScrollBar").Show()
				self.ilk = 1
				self.yuzdekac = 0

	def __OnRefresh(self):
		if self.yenile2 > app.GetTime():
			chat.AppendChat(chat.CHAT_TYPE_INFO, "10 saniyede bir listeyi güncelleyebilirsiniz. Kalan Süre : "+str(int(self.yenile2)-int(app.GetTime())))
		else:
			self.__ClearFileList()
			net.SendYoutubeVideoPacket("video", self.kisi)

			for i in xrange(0, len(constInfo.youtuber_video_list)):
				self.youtubervideolist.AppendItem(markla(constInfo.youtuber_video_list[i].split("#")[1] + " baþlýklý video : "+constInfo.youtuber_video_list[i].split("#")[2]))

			chat.AppendChat(chat.CHAT_TYPE_INFO, "Youtuburun video listesi Yenilendi.")
			self.yenile2 = app.GetTime()+10

	def __ClearFileList(self):
		self.youtubervideolist.RemoveAllItems()

	def __AppendFile(self, fileName):
		self.youtubervideolist.AppendItem(markla(fileName))

class markla(ui.ListBoxEx.Item):
	def __init__(self, fileName):
		ui.ListBoxEx.Item.__init__(self)
		self.imgWidth=0
		self.imgHeight=0
		self.canLoad=0
		self.textLine=self.__CreateTextLine(fileName)
		self.imgBox=self.__CreateImageBox()

	def __del__(self):
		ui.ListBoxEx.Item.__del__(self)

	def GetText(self):
		return self.textLine.GetText()

	def SetSize(self, width, height):
		ui.ListBoxEx.Item.SetSize(self, 20 + 6*len(self.textLine.GetText()) + 4, height)

	def __CreateTextLine(self, fileName):
		textLine=ui.TextLine()
		textLine.SetParent(self)
		textLine.SetPosition(20, 0)
		textLine.SetText(fileName)
		textLine.Show()
		return textLine

	def __CreateImageBox(self):
		imgBox=ui.ImageBox()
		imgBox.AddFlag("not_pick")
		imgBox.SetParent(self)
		imgBox.SetPosition(-1, 0)
		imgBox.LoadImage("d:/ymir work/ui/youtube_logo.tga")
		imgBox.Show()
		return imgBox
