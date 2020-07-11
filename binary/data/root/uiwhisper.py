import net
import app
import player
import chr
import chat
import ime
import grp
import ui
import localeInfo
import dbg
if app.ENABLE_WHISPER_RENEWAL:
	import whisper
	import time
	b_name = localeInfo.WHISPER_RENEWAL_TEXT

class WhisperButton(ui.Button):
	def __init__(self):
		ui.Button.__init__(self, "TOP_MOST")

	def __del__(self):
		ui.Button.__del__(self)

	def SetToolTipText(self, text, x=0, y = 32):
		ui.Button.SetToolTipText(self, text, x, y)
		self.ToolTipText.Show()

	def SetToolTipTextWithColor(self, text, color, x=0, y = 32):
		ui.Button.SetToolTipText(self, text, x, y)
		self.ToolTipText.SetPackedFontColor(color)
		self.ToolTipText.Show()

	def ShowToolTip(self):
		if 0 != self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if 0 != self.ToolTipText:
			self.ToolTipText.Show()

class WhisperDialog(ui.ScriptWindow):

	class TextRenderer(ui.Window):
		def SetTargetName(self, targetName):
			self.targetName = targetName

		def OnRender(self):
			(x, y, width, height) = self.GetRect()
			grp.RenderBar(x, y, width, height)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)
			chat.RenderBox(self.targetName, x, y)

	class ResizeButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)

		def __del__(self):
			ui.DragButton.__del__(self)

		def OnMouseOverIn(self):
			app.SetCursor(app.HVSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

	def __init__(self, eventMinimize, eventClose):
		print "NEW WHISPER DIALOG  ----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		self.targetName = ""
		self.eventMinimize = eventMinimize
		self.eventClose = eventClose
		self.eventAcceptTarget = None
		
	def __del__(self):
		print "---------------------------------------------------------------------------- DELETE WHISPER DIALOG"
		ui.ScriptWindow.__del__(self)		

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/WhisperDialog.py")			
		except:
			import exception as exception
			exception.Abort("WhisperDialog.LoadDialog.LoadScript")

		try:
			GetObject = self.GetChild
			self.scrollBar = GetObject("scrollbar")
			self.titleNameEdit = GetObject("titlename_edit")
			self.titleName = GetObject("titlename")
			self.closeButton = GetObject("closebutton")
			self.chatLine = GetObject("chatline")
			self.minimizeButton = GetObject("minimizebutton")
			self.sendButton = GetObject("sendbutton")
			self.board = GetObject("board")
			self.editBar = GetObject("editbar")
			self.gamemasterMark = GetObject("gamemastermark")
			self.acceptButton = GetObject("acceptbutton")
			if app.ENABLE_WHISPER_RENEWAL:
				self.typing = GetObject("typing")			
		except:
			import exception as exception
			exception.Abort("DialogWindow.LoadDialog.BindObject")

		if app.ENABLE_WHISPER_RENEWAL:
			self.SetTimer()
			self.typing.SetText(b_name)

		self.titleNameEdit.SetText("")
		self.gamemasterMark.Hide()
		self.minimizeButton.SetEvent(ui.__mem_func__(self.Minimize))
		#self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
		self.scrollBar.SetPos(1.0)
		self.scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.chatLine.SetReturnEvent(ui.__mem_func__(self.SendWhisper))
		self.chatLine.SetEscapeEvent(ui.__mem_func__(self.Minimize))
		self.chatLine.SetMultiLine()
		#self.chatLine.OnIMEKeyDown = ui.__mem_func__(self.OnIMEKeyDown)
		#self.SetClickEvent(self.OnMouseLeftButtonDown)
		
		self.sendButton.SetEvent(ui.__mem_func__(self.SendWhisper))
		self.titleNameEdit.SetReturnEvent(ui.__mem_func__(self.AcceptTarget))
		self.titleNameEdit.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.AcceptTarget))

		self.textRenderer = self.TextRenderer()
		self.textRenderer.SetParent(self)
		self.textRenderer.SetPosition(20, 28)
		self.textRenderer.SetTargetName("")
		self.textRenderer.Show()
		
		self.titleName.SetText("")
		self.closeButton.SetEvent(ui.__mem_func__(self.Close))
		self.resizeButton = self.ResizeButton()
		self.resizeButton.SetParent(self)
		self.resizeButton.SetSize(20, 20)
		self.resizeButton.SetPosition(410, 280)
		self.resizeButton.SetMoveEvent(ui.__mem_func__(self.ResizeWhisperDialog))
		self.resizeButton.Show()
		
		self.ResizeWhisperDialog()
		
		#self.ignoreButton.SetToggleDownEvent(ui.__mem_func__(self.IgnoreTarget))
		#self.ignoreButton.SetToggleUpEvent(ui.__mem_func__(self.IgnoreTarget))
		#self.reportViolentWhisperButton.SetEvent(ui.__mem_func__(self.ReportViolentWhisper))

	def Destroy(self):
		self.eventMinimize = None
		self.eventClose = None
		self.eventAcceptTarget = None

		self.ClearDictionary()
		self.scrollBar.Destroy()
		self.titleNameEdit = None
		#self.titleNameEditBar = None
		self.acceptButton = None
		self.scrollBar = None
		self.chatLine = None
		self.sendButton = None
		self.minimizeButton = None
		self.textRenderer = None
		self.board = None
		self.editBar = None
		self.resizeButton = None
		self.acceptButton = None
		self.closeButton = None
		self.titleName = None
		#self.ignoreButton = None
		#self.reportViolentWhisperButton = None

	def ResizeWhisperDialog(self):
		(xPos, yPos) = self.resizeButton.GetLocalPosition()
		if xPos < 280:
			self.resizeButton.SetPosition(280, yPos)
			return
		if yPos < 150:
			self.resizeButton.SetPosition(xPos, 150)
			return
		self.SetWhisperDialogSize(xPos + 20, yPos + 20)

	def SetWhisperDialogSize(self, width, height):
		try:
			max = int((width - 90) / 6) * 3 - 6

			self.board.SetSize(width, height)
			self.scrollBar.SetPosition(width - 25, 35)
			self.scrollBar.SetScrollBarSize(height - 100)
			self.scrollBar.SetPos(1.0)
			self.editBar.SetSize(width - 18, 50)
			self.chatLine.SetSize(width - 90, 40)
			self.chatLine.SetLimitWidth(width-90)
			self.SetSize(width, height)

			if 0 != self.targetName:
				chat.SetBoxSize(self.targetName, width - 60, height - 110)	

			chat.SetBoxPosition(self.targetName, (width - 60)+ 10, height - 110)
			
			self.textRenderer.SetPosition(20, 28)
			self.scrollBar.SetPosition(width - 25, 35)
			self.editBar.SetPosition(10, height - 60)
			self.sendButton.SetPosition(width - 80, 10)
			self.minimizeButton.SetPosition(width - 42, 12)
			self.closeButton.SetPosition(width - 24, 11)

			self.SetChatLineMax(max)
		except:
			import exception as exception
			exception.Abort("WhisperDialog.SetWhisperDialogSize.BindObject")

	def SetChatLineMax(self, max):
		self.chatLine.SetMax(max)
		from grpText import GetSplitingTextLine
		text = self.chatLine.GetText()
		if text:
			self.chatLine.SetText(GetSplitingTextLine(text, max, 0))

	def OpenWithTarget(self, targetName):
		if "$" in targetName:
			return
		chat.CreateFloatingBox(targetName)	
		self.titleName.SetText(targetName)
		chat.SetBoxSize(targetName, self.GetWidth() - 60, self.GetHeight() - 90)
			
		# self.ignoreButton.Hide()
		# if app.IsDevStage():
			# self.reportViolentWhisperButton.Show()
		# else:
			# self.reportViolentWhisperButton.Hide()
				
		self.chatLine.SetFocus()
		self.targetName = targetName
		self.textRenderer.SetTargetName(targetName)
		self.gamemasterMark.Hide()
		self.minimizeButton.Show()
		self.titleNameEdit.Hide()
		self.acceptButton.Hide()

	def OpenWithoutTarget(self, event):
		self.eventAcceptTarget = event
		self.minimizeButton.Hide()
		self.gamemasterMark.Hide()
		self.titleNameEdit.SetText("")
		self.titleNameEdit.SetFocus()
		self.targetName = 0
		self.titleNameEdit.Show()
		self.acceptButton.Show()
		self.titleName.SetText("")
		self.minimizeButton.Hide()
		self.gamemasterMark.Hide()
		if app.ENABLE_WHISPER_RENEWAL:
			self.typing.Hide()
		#self.ignoreButton.Hide()
		#self.reportViolentWhisperButton.Hide()

	def SetGameMasterLook(self):
		self.gamemasterMark.Show()

	def Minimize(self):
		self.titleNameEdit.KillFocus()
		self.chatLine.KillFocus()
		self.Hide()
		if app.ENABLE_WHISPER_RENEWAL:
			if whisper.IsSended(str(self.targetName)) and 0 != self.targetName:
				whisper.Remove(self.targetName)
		if None != self.eventMinimize:
			self.eventMinimize(self.targetName)

	def Close(self):
		chat.ClearBox(self.targetName)
		if app.ENABLE_WHISPER_RENEWAL:
			if whisper.IsSended(self.targetName) and 0 != self.targetName:
				whisper.Remove(self.targetName)
		self.titleNameEdit.KillFocus()
		self.chatLine.KillFocus()
		self.Hide()

		if None != self.eventClose:
			self.eventClose(self.targetName)

	def ReportViolentWhisper(self):
		net.SendChatPacket("/reportviolentwhisper " + self.targetName)

	def IgnoreTarget(self):
		net.SendChatPacket("/ignore " + self.targetName)

	def AcceptTarget(self):
		name = self.titleNameEdit.GetText()
		if len(name) <= 0:
			self.Close()
			return

		if None != self.eventAcceptTarget:
			self.titleNameEdit.KillFocus()
			self.eventAcceptTarget(name)

	def OnScroll(self):
		chat.SetBoxPosition(self.targetName, self.scrollBar.GetPos())
		
	def SendWhisper(self):
		text = self.chatLine.GetText()
		textLength = len(text)
		
		if self.targetName == "<Sistem>":
			return

		if textLength > 0:
			if net.IsInsultIn(text):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHAT_INSULT_STRING)
				return
				
			net.SendWhisperPacket(self.targetName, text)
			self.chatLine.SetText("")
			chat.AppendToBox(chat.WHISPER_TYPE_CHAT, self.targetName, player.GetName() + " : " + text)
				
	def OnTop(self):
		if self.chatLine:
			self.chatLine.SetFocus()
		
	def BindInterface(self, interface):
		self.interface = interface
		
	def OnMouseLeftButtonDown(self):
		hyperlink = ui.GetHyperlink()
		if hyperlink:
			if app.IsPressed(app.DIK_LALT):
				link = chat.GetLinkFromHyperlink(hyperlink)
				ime.PasteString(link)
			else:
				self.interface.MakeHyperlinkTooltip(hyperlink)

	def OnRunMouseWheel(self, nLen):
		if nLen > 0:
			self.scrollBar.OnUp()
		else:
			self.scrollBar.OnDown()

	if app.ENABLE_WHISPER_RENEWAL:
		def SetTimer(self):
			self.endTime = time.clock() + 0.4
		def OnUpdate(self):
			if 0 != self.targetName:
				if len(self.chatLine.GetText()) > 0 and not whisper.IsSended(self.targetName):
					whisper.Add(self.targetName)
				elif not len(self.chatLine.GetText()) > 0 and whisper.IsSended(self.targetName):
					whisper.Remove(self.targetName)
				if whisper.CheckName(self.targetName):
					self.typing.Show()
					lastTime = max(0, self.endTime - time.clock())
					if 0 == lastTime:
						if len(self.typing.GetText()) - len(b_name) < 3:
							self.typing.SetText(self.typing.GetText() + ".")
						else:
							self.typing.SetText(b_name)	
						self.SetTimer()
				else:
					self.typing.Hide()
					
