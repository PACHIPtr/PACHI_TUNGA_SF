import net
import app
import player
import dbg
import ui
import uiScriptLocale

###################################################################################################
## Restart
class RestartDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/restartdialog.py")
		except Exception, msg:
			import sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("RestartDialog.LoadDialog - %s:%s" % (type, msg))
			app.Abort()
			return 0

		try:
			if (app.ENABLE_BATTLE_ZONE_SYSTEM):
				self.board = self.GetChild("board")
			self.restartHereButton=self.GetChild("restart_here_button")
			self.restartTownButton=self.GetChild("restart_town_button")
		except:
			import sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("RestartDialog.LoadDialog - %s:%s" % (type, msg))
			app.Abort()
			return 0

		self.restartHereButton.SetEvent(ui.__mem_func__(self.RestartHere))
		self.restartTownButton.SetEvent(ui.__mem_func__(self.RestartTown))
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			restartCombatZoneButton = ui.Button()
			restartCombatZoneButton.SetParent(self.board)
			restartCombatZoneButton.SetPosition(10, 77)
			restartCombatZoneButton.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
			restartCombatZoneButton.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
			restartCombatZoneButton.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
			restartCombatZoneButton.SetText(uiScriptLocale.RESTART_IMMEDIATE)
			restartCombatZoneButton.SAFE_SetEvent(self.RestartCombatZone)
			restartCombatZoneButton.Hide()
			self.restartCombatZoneButton = restartCombatZoneButton	

		return 1

	def Destroy(self):
		self.restartHereButton=0
		self.restartTownButton=0
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			self.restartCombatZoneButton = 0
			self.board = 0
		self.ClearDictionary()

	def OpenDialog(self):
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			self.CheckWindowStyle()
		self.Show()

	def Close(self):
		self.Hide()
		return True

	def RestartHere(self):
		net.SendChatPacket("/restart_here")

	def RestartTown(self):
		net.SendChatPacket("/restart_town")
		
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def RestartCombatZone(self):
			net.SendChatPacket("/restart_combat_zone")

		def CheckWindowStyle(self):

			def CheckCombatZoneButton():
				if player.IsCombatZoneMap():
					self.restartCombatZoneButton.Show()
				else:
					self.restartCombatZoneButton.Hide()
				
			def GetSizeByMapLocation():
				return [[200, 88], [200, 113]][player.IsCombatZoneMap()]
		
			xSize, ySize = GetSizeByMapLocation()
			self.board.SetSize(xSize, ySize)
			self.SetSize(xSize, ySize)
			CheckCombatZoneButton()

	def OnPressExitKey(self):
		return True

	def OnPressEscapeKey(self):
		return True
