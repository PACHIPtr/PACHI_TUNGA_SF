import player
import net
import app
import miniMap
import chat
import background
import systemSetting
import combatzone
import ui
import uiScriptLocale
import wndMgr
import localeInfo
import colorinfo as colorinfo
import constInfo
import time
import snd
import time
import re
import os
import serverInfo as serverInfo
import uiToolTip
import grp
import uiCommon

class MapTextToolTip(ui.Window):
	def __init__(self):			
		ui.Window.__init__(self)

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetHorizontalAlignCenter()
		textLine.SetOutline()
		textLine.SetHorizontalAlignRight()
		textLine.Show()
		self.textLine = textLine

	def __del__(self):			
		ui.Window.__del__(self)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetTooltipPosition(self, PosX, PosY):
		self.textLine.SetPosition(PosX - 5, PosY)

	def SetTextColor(self, TextColor):
		self.textLine.SetPackedFontColor(TextColor)

	def GetTextSize(self):
		return self.textLine.GetTextSize()

class AtlasWindow(ui.ScriptWindow):

	class AtlasRenderer(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("not_pick")

		def OnUpdate(self):
			miniMap.UpdateAtlas()

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			fx = float(x)
			fy = float(y)
			miniMap.RenderAtlas(fx, fy)

		def HideAtlas(self):
			miniMap.HideAtlas()

		def ShowAtlas(self):
			miniMap.ShowAtlas()

	def __init__(self):
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Hide()
		self.infoGuildMark = ui.MarkBox()
		self.infoGuildMark.Hide()
		self.AtlasMainWindow = None
		self.mapName = ""
		self.board = 0

		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMapName(self, mapName):
		if 949==app.GetDefaultCodePage():
			try:
				self.board.SetTitleName(localeInfo.MINIMAP_ZONE_NAME_DICT[mapName])
			except:
				pass

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AtlasWindow.py")
		except:
			import exception as exception
			exception.Abort("AtlasWindow.LoadWindow.LoadScript")

		try:
			self.board = self.GetChild("board")

		except:
			import exception as exception
			exception.Abort("AtlasWindow.LoadWindow.BindObject")

		self.AtlasMainWindow = self.AtlasRenderer()
		self.board.SetCloseEvent(self.Hide)
		self.AtlasMainWindow.SetParent(self.board)
		self.AtlasMainWindow.SetPosition(7, 30)
		self.tooltipInfo.SetParent(self.board)
		self.infoGuildMark.SetParent(self.board)
		self.SetPosition(wndMgr.GetScreenWidth() - 136 - 256 - 10 - 165, 78)
		self.Hide()

		miniMap.RegisterAtlasWindow(self)

	def Destroy(self):
		miniMap.UnregisterAtlasWindow()
		self.ClearDictionary()
		self.AtlasMainWindow = None
		self.tooltipAtlasClose = 0
		self.tooltipInfo = None
		self.infoGuildMark = None
		self.board = None

	def OnUpdate(self):

		if not self.tooltipInfo:
			return

		if not self.infoGuildMark:
			return

		self.infoGuildMark.Hide()
		self.tooltipInfo.Hide()

		if False == self.board.IsIn():
			return

		(mouseX, mouseY) = wndMgr.GetMousePosition()
		(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)

		if False == bFind:
			return

		if "empty_guild_area" == sName:
			sName = localeInfo.GUILD_EMPTY_AREA

		self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
			
		(x, y) = self.GetGlobalPosition()
		self.tooltipInfo.SetTooltipPosition(mouseX - x, mouseY - y)
		self.tooltipInfo.SetTextColor(dwTextColor)
		self.tooltipInfo.Show()
		self.tooltipInfo.SetTop()

		if 0 != dwGuildID:
			textWidth, textHeight = self.tooltipInfo.GetTextSize()
			self.infoGuildMark.SetIndex(dwGuildID)
			self.infoGuildMark.SetPosition(mouseX - x - textWidth - 18 - 5, mouseY - y)
			self.infoGuildMark.Show()

	def Hide(self):
		if self.AtlasMainWindow:
			self.AtlasMainWindow.HideAtlas()
			self.AtlasMainWindow.Hide()
		ui.ScriptWindow.Hide(self)

	def Show(self):
		if self.AtlasMainWindow:
			(bGet, iSizeX, iSizeY) = miniMap.GetAtlasSize()
			if bGet:
				self.SetSize(iSizeX + 15, iSizeY + 38)
				self.board.SetSize(iSizeX + 15, iSizeY + 38)
				#self.AtlasMainWindow.SetSize(iSizeX, iSizeY)
				self.AtlasMainWindow.ShowAtlas()
				self.AtlasMainWindow.Show()
		ui.ScriptWindow.Show(self)

	def SetCenterPositionAdjust(self, x, y):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)

	def OnPressEscapeKey(self):
		self.Hide()
		return True

def __RegisterMiniMapColor(type, rgb):
	miniMap.RegisterColor(type, rgb[0], rgb[1], rgb[2])

class MiniMap(ui.ScriptWindow):

	CANNOT_SEE_INFO_MAP_DICT = {
		"metin2_map_monkeydungeon" : False,
		"metin2_map_monkeydungeon_02" : False,
		"metin2_map_monkeydungeon_03" : False,
		"metin2_map_devilsCatacomb" : False,
		"metin2_12zi_stage" : False,
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__Initialize()

		miniMap.Create()
		miniMap.SetScale(2.0)
		
		if app.ENABLE_MAIL_BOX_SYSTEM:
			self.toolTip = uiToolTip.ToolTip(160)
			self.toolTip.HideToolTip()

		self.AtlasWindow = AtlasWindow()
		self.AtlasWindow.LoadWindow()
		self.AtlasWindow.Hide()

		self.tooltipMiniMapOpen = MapTextToolTip()
		self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
		self.tooltipMiniMapOpen.Show()
		self.tooltipMiniMapClose = MapTextToolTip()
		self.tooltipMiniMapClose.SetText(localeInfo.UI_CLOSE)
		self.tooltipMiniMapClose.Show()
		self.tooltipScaleUp = MapTextToolTip()
		self.tooltipScaleUp.SetText(localeInfo.MINIMAP_INC_SCALE)
		self.tooltipScaleUp.Show()
		self.tooltipScaleDown = MapTextToolTip()
		self.tooltipScaleDown.SetText(localeInfo.MINIMAP_DEC_SCALE)
		self.tooltipScaleDown.Show()
		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		self.tooltipAtlasOpen.Show()
		self.tooltipDungeonTracking = MapTextToolTip()
		self.tooltipDungeonTracking.SetText(localeInfo.MINIMAP_DUNGEON)
		self.tooltipDungeonTracking.Show()
		self.tooltipBossTracking = MapTextToolTip()
		self.tooltipBossTracking.SetText(localeInfo.MINIMAP_BOSS)
		self.tooltipBossTracking.Show()
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		if miniMap.IsAtlas():
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		else:
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_CAN_NOT_SHOW_AREAMAP)

		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		self.mapName = ""

		self.isLoaded = 0
		self.canSeeInfo = True
		

	def __del__(self):
		miniMap.Destroy()
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.positionInfo = 0
		self.observerCount = 0

		self.OpenWindow = 0
		self.CloseWindow = 0
		self.ScaleUpButton = 0
		self.ScaleDownButton = 0
		self.MiniMapHideButton = 0
		self.MiniMapShowButton = 0
		self.AtlasShowButton = 0
		
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			self.btnCombatZone = 0

		self.tooltipMiniMapOpen = 0
		self.tooltipMiniMapClose = 0
		self.tooltipScaleUp = 0
		self.tooltipScaleDown = 0
		self.tooltipAtlasOpen = 0
		self.tooltipDungeonTracking = 0
		self.tooltipBossTracking = 0
		self.tooltipInfo = None
		self.serverInfo = None
		self.generalInfo = None
		self.generalInfoTimer = 0
		self.pingTimer = 0
		self.newtooltipInfo = None
		if app.ENABLE_DEFENSE_WAVE:
			self.MastHp = 0
	
	def SetMapName(self, mapName):
		self.mapName=mapName
		self.AtlasWindow.SetMapName(mapName)
		#self.UpdatePingCount()


		if self.CANNOT_SEE_INFO_MAP_DICT.has_key(mapName):
			self.canSeeInfo = False
			self.HideMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP_CANNOT_SEE)
		else:
			self.canSeeInfo = True
			self.ShowMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
	

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/MiniMap.py")
		except:
			import exception as exception
			exception.Abort("MiniMap.LoadWindow.LoadScript")

		try:
			self.OpenWindow = self.GetChild("OpenWindow")
			self.MiniMapWindow = self.GetChild("MiniMapWindow")
			self.ScaleUpButton = self.GetChild("ScaleUpButton")
			self.ScaleDownButton = self.GetChild("ScaleDownButton")
			self.MiniMapHideButton = self.GetChild("MiniMapHideButton")
			self.AtlasShowButton = self.GetChild("AtlasShowButton")
			self.CloseWindow = self.GetChild("CloseWindow")
			self.MiniMapShowButton = self.GetChild("MiniMapShowButton")
			self.positionInfo = self.GetChild("PositionInfo")
			self.observerCount = self.GetChild("ObserverCount")
			self.serverInfo = self.GetChild("ServerInfo")
			if (app.ENABLE_BATTLE_ZONE_SYSTEM):
				self.btnCombatZone = self.GetChild("BattleButton")
			if app.ENABLE_DEFENSE_WAVE:
				self.MastHp = self.GetChild("MastHp")
				self.MastWindow = self.GetChild("MastWindow")
				self.MastHp.OnMouseOverIn = ui.__mem_func__(self.MastHp.ShowToolTip)
				self.MastHp.OnMouseOverOut = ui.__mem_func__(self.MastHp.HideToolTip)
				self.MastHp.SetShowToolTipEvent(self.MastHp.OnMouseOverIn)
				self.MastHp.SetHideToolTipEvent(self.MastHp.OnMouseOverOut)
			if app.ENABLE_MAIL_BOX_SYSTEM:
				self.MailBoxButton = self.GetChild("MailBoxButton")
				self.MailBoxEffect = self.GetChild("MailBoxEffect")
			self.l_button = self.GetChild("l_button")
			self.p_button = self.GetChild("p_button")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.Bind")

		if constInfo.MINIMAP_POSITIONINFO_ENABLE==0:
			self.positionInfo.Hide()

		self.serverInfo.SetText(net.GetServerInfo())
		self.generalInfo = ui.TextLine()
		self.generalInfo.SetFontName("Arial:17")
		self.generalInfo.SetPosition(0,87)
		self.generalInfo.SetWindowHorizontalAlignCenter()
		self.generalInfo.SetHorizontalAlignCenter()		
		self.generalInfo.SetFeather()
		self.generalInfo.SetOutline()
		self.ScaleUpButton.SetEvent(ui.__mem_func__(self.ScaleUp))
		self.ScaleDownButton.SetEvent(ui.__mem_func__(self.ScaleDown))
		self.MiniMapHideButton.SetEvent(ui.__mem_func__(self.HideMiniMap))
		self.MiniMapShowButton.SetEvent(ui.__mem_func__(self.ShowMiniMap))
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			self.btnCombatZone.SetEvent(ui.__mem_func__(self.OpenCombatZoneWindow))
			self.btnCombatZone.Down()
			
		if app.ENABLE_MAIL_BOX_SYSTEM:
			self.MailBoxButton.SetEvent(ui.__mem_func__(self.OnMouseOverIn), "mouse_over_in")
			self.MailBoxButton.SetEvent(ui.__mem_func__(self.OnMouseOverOut), "mouse_over_out")

		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

		(ButtonPosX, ButtonPosY) = self.MiniMapShowButton.GetGlobalPosition()
		self.tooltipMiniMapOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.MiniMapHideButton.GetGlobalPosition()
		self.tooltipMiniMapClose.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleUpButton.GetGlobalPosition()
		self.tooltipScaleUp.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleDownButton.GetGlobalPosition()
		self.tooltipScaleDown.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.AtlasShowButton.GetGlobalPosition()
		self.tooltipAtlasOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)
		
		(ButtonPosX, ButtonPosY) = self.l_button.GetGlobalPosition()
		self.tooltipDungeonTracking.SetTooltipPosition(ButtonPosX, ButtonPosY)
		
		(ButtonPosX, ButtonPosY) = self.p_button.GetGlobalPosition()
		self.tooltipBossTracking.SetTooltipPosition(ButtonPosX, ButtonPosY)		
		if app.ENABLE_DEFENSE_WAVE:
			self.MastHp.SetPercentage(5000000, 5000000)
			self.MastWindow.Hide()
		self.ShowMiniMap()
				
		self.l_button.SetEvent(ui.__mem_func__(self.__DungeonTracking))
		self.p_button.SetEvent(ui.__mem_func__(self.__BossTracking))
		
		self.tlInfo = uiToolTip.ItemToolTip()
		self.tlInfo.Hide()
	
	def Destroy(self):
		self.HideMiniMap()

		self.AtlasWindow.Hide()
		self.AtlasWindow.Destroy()
		self.AtlasWindow = None

		self.ClearDictionary()

		self.__Initialize()
		
	def UpdateObserverCount(self, observerCount):
		if observerCount>0:
			self.observerCount.Show()
		elif observerCount<=0:
			self.observerCount.Hide()
		self.observerCount.SetText(localeInfo.MINIMAP_OBSERVER_COUNT % observerCount)
		
	if app.ENABLE_DEFENSE_WAVE:
		def SetMastHP(self, hp):
			self.MastHp.SetPercentage(hp, 5000000)
			self.MastHp.SetToolTipText("HP:  %d /5000000" % hp)

		def SetMastWindow(self, i):
			if i:
				self.MastWindow.Show()
			else:
				self.MastWindow.Hide()
				
	def __BossTracking(self):
		self.bosstracking = constInfo.BOSS_TRACKING
		self.bosstracking.Open()
		
	def __DungeonTracking(self):
		self.dungeontracking = constInfo.DUNGEON_TRACKING
		self.dungeontracking.Open()
		
	# def __BonusButton(self):
		# self.EfsunBotDialog = uiCommon.QuestionDialog()
		# self.EfsunBotDialog.SetText("Hangi efsun botunu kullanmak istiyorsunuz?")
		# self.EfsunBotDialog.SetAcceptText("Normal")
		# self.EfsunBotDialog.SetCancelText("Yeþil")
		# self.EfsunBotDialog.SetAcceptEvent(lambda arg=True: self.OnEfsunBotDialog(1))
		# self.EfsunBotDialog.SetCancelEvent(lambda arg=False: self.OnEfsunBotDialog(2))
		# self.EfsunBotDialog.Open()
		
	# def OnEfsunBotDialog(self,gelen):
		# self.EfsunBotDialog.Close()
		# if gelen==1:
			# if not constInfo.eigenmod:
				# execfile('eigenmods.py',{})
				# constInfo.eigenmod = 1
		# else:
			# if self.switchbot.is_open == 1:
				# self.switchbot.Hide()	
			# else:
				# self.switchbot.Show()

	def OnUpdate(self):
		global bgk
		(x, y, z) = player.GetMainCharacterPosition()
		miniMap.Update(x, y)

		self.positionInfo.SetText("(%.0f, %.0f)" % (x/100, y/100))

		if self.tooltipInfo:
			if True == self.MiniMapWindow.IsIn():
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				(bFind, sName, iPosX, iPosY, dwTextColor) = miniMap.GetInfo(mouseX, mouseY)
				if bFind == 0:
					self.tooltipInfo.Hide()
				elif not self.canSeeInfo:
					self.tooltipInfo.SetText("%s(%s)" % (sName, localeInfo.UI_POS_UNKNOWN))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
				else:
					self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
			else:
				self.tooltipInfo.Hide()
			


		if True == self.MiniMapShowButton.IsIn():
			self.tooltipMiniMapOpen.Show()
		else:
			self.tooltipMiniMapOpen.Hide()

		if True == self.MiniMapHideButton.IsIn():
			self.tooltipMiniMapClose.Show()
		else:
			self.tooltipMiniMapClose.Hide()

		if True == self.ScaleUpButton.IsIn():
			self.tooltipScaleUp.Show()
		else:
			self.tooltipScaleUp.Hide()

		if True == self.ScaleDownButton.IsIn():
			self.tooltipScaleDown.Show()
		else:
			self.tooltipScaleDown.Hide()

		if True == self.AtlasShowButton.IsIn():
			self.tooltipAtlasOpen.Show()
		else:
			self.tooltipAtlasOpen.Hide()
			
		if True == self.l_button.IsIn():
			self.tooltipDungeonTracking.Show()
		else:
			self.tooltipDungeonTracking.Hide()
			
		if True == self.p_button.IsIn():
			self.tooltipBossTracking.Show()
		else:
			self.tooltipBossTracking.Hide()
		
	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		fx = float(x)
		fy = float(y)
		miniMap.Render(fx + 1.0, fy + 5.0)

	def Close(self):
		self.HideMiniMap()

	def HideMiniMap(self):
		miniMap.Hide()
		self.OpenWindow.Hide()
		self.CloseWindow.Show()

	def ShowMiniMap(self):
		if not self.canSeeInfo:
			return

		miniMap.Show()
		self.OpenWindow.Show()
		self.CloseWindow.Hide()

	def isShowMiniMap(self):
		return miniMap.isShow()
		
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def OnAskCombatZoneQuestionDialog(self):
			import uiCommon
			self.combatZoneLeaveQuestionDialog = uiCommon.QuestionDialog2()
			self.combatZoneLeaveQuestionDialog.SetText1(uiScriptLocale.EXIT_BATTLE_FIELD_COLLECTED_POINTS % (player.GetCombatZonePoints()))
			self.combatZoneLeaveQuestionDialog.SetText2(uiScriptLocale.EXIT_BATTLE_FIELD)
			self.combatZoneLeaveQuestionDialog.SetWidth(320)
			self.combatZoneLeaveQuestionDialog.SetAcceptEvent(lambda arg = True: self.OnToggleCombatZoneQuestionDialog(arg))
			self.combatZoneLeaveQuestionDialog.SetCancelEvent(lambda arg = False: self.OnToggleCombatZoneQuestionDialog(arg))
			self.combatZoneLeaveQuestionDialog.Open()
			
		def OnToggleCombatZoneQuestionDialog(self, answer):
			if not self.combatZoneLeaveQuestionDialog:
				return

			self.combatZoneLeaveQuestionDialog.Close()
			self.combatZoneLeaveQuestionDialog = None

			if not answer:
				return

			combatzone.SendCombatZoneRequestActionPacket(combatzone.COMBAT_ZONE_ACTION_LEAVE, combatzone.COMBAT_ZONE_EMPTY_DATA)
			return True
	
		def OpenCombatZoneWindow(self):
			if player.IsCombatZoneMap():
				self.OnAskCombatZoneQuestionDialog()
			else:
				combatzone.SendCombatZoneRequestActionPacket(combatzone.COMBAT_ZONE_ACTION_OPEN_RANKING, combatzone.COMBAT_ZONE_EMPTY_DATA)

	def ScaleUp(self):
		miniMap.ScaleUp()

	def __IsSpecialMap(self):
		Blocked_MAPS = [
			"season1/metin2_map_oxevent",
			"season2/metin2_map_guild_inside01",
			"season2/metin2_map_empirewar01",
			"season2/metin2_map_empirewar02",
			"season2/metin2_map_empirewar03",
			"metin2_map_dragon_timeattack_01",
			"metin2_map_dragon_timeattack_02",
			"metin2_map_dragon_timeattack_03",
			"metin2_map_skipia_dungeon_boss",
			"metin2_map_skipia_dungeon_boss2",
			"metin2_map_devilsCatacomb",
			"metin2_map_deviltower1",
			"metin2_map_t1",
			"metin2_map_t2",
			"metin2_map_t3",
			"metin2_map_t4",
			"metin2_map_t5",
			"metin2_map_wedding_01",
			"metin2_map_ring",
			"metin2_map_gemi",
			"gm_guild_build",
			"metin2_map_duel"
		]

		if str(background.GetCurrentMapName()) in Blocked_MAPS:
			return True

		return False

	def ScaleDown(self):
		miniMap.ScaleDown()

	def ShowAtlas(self):
		if not miniMap.IsAtlas():
			return
		if not self.AtlasWindow.IsShow():
			self.AtlasWindow.Show()

	def ToggleAtlasWindow(self):
		if not miniMap.IsAtlas():
			return
		if self.AtlasWindow.IsShow():
			self.AtlasWindow.Hide()
		else:
			self.AtlasWindow.Show()
			
	def UpdateChannelInfo(self, channel):
		srvName = serverInfo.SERVER_1
		if constInfo.SERVER_TYPE == 1:
			srvName = serverInfo.SERVER_1
		elif constInfo.SERVER_TYPE == 2:
			srvName = serverInfo.SERVER_2
		elif constInfo.SERVER_TYPE == 3:
			srvName = serverInfo.SERVER_3
		else:
			srvName = serverInfo.SERVER_1
		if channel == 99:
			serverInfoStr = srvName + ' - Ortak CH'
		else:
			serverInfoStr = srvName + ' - ' + str(channel) + '. CH'
		self.serverInfo.SetText(serverInfoStr)
		net.SetServerInfo(serverInfoStr)
		
	if app.ENABLE_MAIL_BOX_SYSTEM:
		def MailBoxValues(self,c_a,m_a,m_s):
			if c_a > 0:
				self.MailBoxEffect.Show()
				self.MailBoxButton.Show()
			else:
				self.MailBoxEffect.Hide()
				self.MailBoxButton.Hide()
				
			self.toolTip.ClearToolTip()
			self.toolTip.SetTitle("Gelen Kutusu %d"%c_a)
			self.toolTip.AppendTextLine("Eksiz: %d / Ekli: %d"%(m_s,m_a))

		def OnMouseOverIn(self):
			self.toolTip.ShowToolTip()
		
		def OnMouseOverOut(self):
			self.toolTip.HideToolTip()
