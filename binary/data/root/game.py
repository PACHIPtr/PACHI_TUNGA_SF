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
import os
import dbg
import grp
import uipetsystem
import snd
import wndMgr
import fly
import localeInfo
import constInfo
import ui
import uiCommon
import uiPhaseCurtain
import uiMapNameShower
import uiAffectShower
import uiPlayerGauge
import uiCharacter
import uiTarget
import uiTaskbar
# PRIVATE_SHOP_PRICE_LIST
import uiPrivateShopBuilder
# END_OF_PRIVATE_SHOP_PRICE_LIST
import time
import serverInfo
import mouseModule
import consoleModule
import localeInfo

import playersettingmodule
import interfaceModule

import musicInfo
import debugInfo
import stringCommander
import uibiowindow

from _weakref import proxy


# SCREENSHOT_CWDSAVE
SCREENSHOT_CWDSAVE = True
SCREENSHOT_DIR = None
svsidedia = None
svsidedi_cp =  ""
from svsideoi import SvsideDialog
import binascii

import uiGuild

import uiruhtasi
import uibkoku
import uibiyolog

if app.ENABLE_HEALTH_BOARD_SYSTEM:
	import uiNewPlayerGauge
	
cameraDistance = 1550.0
cameraPitch = 27.0
cameraRotation = 0.0
cameraHeight = 100.0

testAlignment = 0

import uisupportshaman

if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
	import uikeyboardsettings
	
import uibosstracking
import uirarityrefine

if app.ENABLE_AVERAGE_ITEM_PRICE:
	import uiaverage_price

class GameWindow(ui.ScriptWindow):
	def __init__(self, stream):
		ui.ScriptWindow.__init__(self, "GAME")
		self.SetWindowName("game")
		net.SetPhaseWindow(net.PHASE_WINDOW_GAME, self)
		player.SetGameWindow(self)
		
		self.uiNewShopCreate = None
		self.uiNewShop = None

		global svsidedia
		if svsidedia == None:
			svsidedia = SvsideDialog()
		svsidedia.Board.Hide()

		self.quickSlotPageIndex = 0
		self.lastPKModeSendedTime = 0
		
		self.pressNumber = None
		
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			import uicombatzone
			self.wndCombatZone = uicombatzone.CombatZoneWindow()

		self.guildWarQuestionDialog = None
		self.interface = None
		self.targetBoard = None
		self.console = None
		self.mapNameShower = None
		self.affectShower = None
		self.playerGauge = None
		self.biyoekran	= None
		if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
			self.dlgKeyboardSettings = None

		self.stream=stream
		self.interface = interfaceModule.Interface()
		self.interface.SetStream(self.stream)
		self.interface.SetGame(self)
		self.interface.MakeInterface()
		self.interface.ShowDefaultWindows()
		self.stream.isAutoSelect = 0

		self.curtain = uiPhaseCurtain.PhaseCurtain()
		self.curtain.speed = 0.03
		self.curtain.Hide()

		self.targetBoard = uiTarget.TargetBoard()
		self.targetBoard.SetWhisperEvent(ui.__mem_func__(self.interface.OpenWhisperDialog))
		self.targetBoard.Hide()
		
		self.petmain = uipetsystem.PetSystemMain()
		if (app.ENABLE_PET_ATTR_DETERMINE):
			self.petmain.SetItemToolTip(self.interface.tooltipItem)
		self.petmain.BindInterface(self.interface)
		self.petmini = uipetsystem.PetSystemMini()

		self.console = consoleModule.ConsoleWindow()
		self.console.BindGameClass(self)
		self.console.SetConsoleSize(wndMgr.GetScreenWidth(), 200)
		self.console.Hide()

		self.support_shaman = uisupportshaman.SupportShamanDialog()
		if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
			self.dlgKeyboardSettings = uikeyboardsettings.KeyChangeWindow(self)
		self.interface.dlgSystem.BindKeyboardWindow(self.dlgKeyboardSettings)
		self.mapNameShower = uiMapNameShower.MapNameShower()
		self.affectShower = uiAffectShower.AffectShower()
		
		self.ruhtasi = uiruhtasi.RuhTasi()
		self.ruhtasi.Hide()
		
		self.bkoku = uibkoku.BKOku()
		self.bkoku.Hide()
		
		self.biyoekran = uibiyolog.BiyologEkran()
		self.biyoekran.Hide()
		
		self.playerGauge = uiPlayerGauge.PlayerGauge(self)
		self.playerGauge.Hide()
		
		if app.ENABLE_AVERAGE_ITEM_PRICE:
			self.average_price = uiaverage_price.AveragePrice()
			self.average_price.Hide()
		
		if app.ENABLE_HEALTH_BOARD_SYSTEM:
			self.newPlayerGauge = uiNewPlayerGauge.NewPlayerGauge(self)
			self.newPlayerGauge.Hide()
			
		self.biowindow = uibiowindow.BiologWnd()
		self.biowindow.Hide()
					
		self.bosstracking = uibosstracking.BossTrackingInfoWindow()
		#self.bosstracking.LoadWindow()
		self.bosstracking.Hide()
		constInfo.BOSS_TRACKING = self.bosstracking
		
		self.wndRR = uirarityrefine.RefineRarityWindow()
		self.wndRR.SetItemToolTip(self.interface.tooltipItem)
		
		#wj 2014.1.2. ESCÅ°¸¦ ´©¸¦ ½Ã ¿ì¼±ÀûÀ¸·Î DropQuestionDialog¸¦ ²ôµµ·Ï ¸¸µé¾ú´Ù. ÇÏÁö¸¸ Ã³À½¿¡ itemDropQuestionDialog°¡ ¼±¾ðµÇ¾î ÀÖÁö ¾Ê¾Æ ERROR°¡ ¹ß»ýÇÏ¿© init¿¡¼­ ¼±¾ð°ú µ¿½Ã¿¡ ÃÊ±âÈ­ ½ÃÅ´.
		self.itemDropQuestionDialog = None		
		
		self.SetQuickSlotMode()
		
		self.__ServerCommand_Build()
		self.__ProcessPreservedServerCommand()
		
		constInfo.INTERFACE_MODULE = self.interface
		
	def __del__(self):
		player.SetGameWindow(0)
		net.ClearPhaseWindow(net.PHASE_WINDOW_GAME, self)
		ui.ScriptWindow.__del__(self)

	def Open(self):
		app.SetFrameSkip(1)

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		
		import uiNewShop
		self.uiNewShop = uiNewShop.ShopDialog()
		self.uiNewShop.Close()
		self.uiNewShopCreate = uiNewShop.ShopDialogCreate()
		self.uiNewShopCreate.Hide()
		
		self.quickSlotPageIndex = 0
		self.PickingCharacterIndex = -1
		self.PickingItemIndex = -1
		self.consoleEnable = False
		self.isShowDebugInfo = False
		self.ShowNameFlag = False

		self.enableXMasBoom = False
		self.startTimeXMasBoom = 0.0
		self.indexXMasBoom = 0
		self.enableXMasMuzik = False
		self.startTimeXMasMuzik = 0.0
		self.indexXMasMuzik = 0

		global cameraDistance, cameraPitch, cameraRotation, cameraHeight

		app.SetCamera(cameraDistance, cameraPitch, cameraRotation, cameraHeight)

		constInfo.SET_DEFAULT_CAMERA_MAX_DISTANCE()
		constInfo.SET_DEFAULT_CHRNAME_COLOR()
		constInfo.SET_DEFAULT_FOG_LEVEL()
		constInfo.SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE()
		constInfo.SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS()
		constInfo.SET_DEFAULT_USE_SKILL_EFFECT_ENABLE()

		# TWO_HANDED_WEAPON_ATTACK_SPEED_UP
		constInfo.SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE()
		# END_OF_TWO_HANDED_WEAPON_ATTACK_SPEED_UP
		
		constInfo.SYSTEMS_WINDOW_KAPAT=1

		event.SetLeftTimeString(localeInfo.UI_LEFT_TIME)

		textTail.EnablePKTitle(constInfo.PVPMODE_ENABLE)

		if constInfo.PVPMODE_TEST_ENABLE:
			self.testPKMode = ui.TextLine()
			self.testPKMode.SetFontName(localeInfo.UI_DEF_FONT)
			self.testPKMode.SetPosition(0, 15)
			self.testPKMode.SetWindowHorizontalAlignCenter()
			self.testPKMode.SetHorizontalAlignCenter()
			self.testPKMode.SetFeather()
			self.testPKMode.SetOutline()
			self.testPKMode.Show()

			self.testAlignment = ui.TextLine()
			self.testAlignment.SetFontName(localeInfo.UI_DEF_FONT)
			self.testAlignment.SetPosition(0, 35)
			self.testAlignment.SetWindowHorizontalAlignCenter()
			self.testAlignment.SetHorizontalAlignCenter()
			self.testAlignment.SetFeather()
			self.testAlignment.SetOutline()
			self.testAlignment.Show()

		self.__BuildKeyDict()
		if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
			self.dlgKeyboardSettings.BuildKeyDict()
		self.__BuildDebugInfo()

		# PRIVATE_SHOP_PRICE_LIST
		uiPrivateShopBuilder.Clear()
		# END_OF_PRIVATE_SHOP_PRICE_LIST

		# UNKNOWN_UPDATE
		exchange.InitTrading()
		# END_OF_UNKNOWN_UPDATE


		## Sound
		snd.SetMusicVolume(systemSetting.GetMusicVolume()*net.GetFieldMusicVolume())
		snd.SetSoundVolume(systemSetting.GetSoundVolume())

		netFieldMusicFileName = net.GetFieldMusicFileName()
		if netFieldMusicFileName:
			snd.FadeInMusic("BGM/" + netFieldMusicFileName)
		elif musicInfo.fieldMusic != "":						
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

		self.SetQuickSlotMode()
		self.SelectQuickPage(self.quickSlotPageIndex)

		self.SetFocus()
		self.Show()
		app.ShowCursor()

		net.SendEnterGamePacket()
				
		# START_GAME_ERROR_EXIT
		try:
			self.StartGame()
		except:
			import exception
			exception.Abort("GameWindow.Open")
		# END_OF_START_GAME_ERROR_EXIT
		
		# NPC°¡ Å¥ºê½Ã½ºÅÛÀ¸·Î ¸¸µé ¼ö ÀÖ´Â ¾ÆÀÌÅÛµéÀÇ ¸ñ·ÏÀ» Ä³½Ì
		# ex) cubeInformation[20383] = [ {"rewordVNUM": 72723, "rewordCount": 1, "materialInfo": "101,1&102,2", "price": 999 }, ... ]
		self.cubeInformation = {}
		self.currentCubeNPC = 0
		
		if app.ENABLE_FOG_FIX:
			if systemSetting.IsFogMode():
				background.SetEnvironmentFog(True)
			else:
				background.SetEnvironmentFog(False)		

		if systemSetting.GetNightMode():
			background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
			background.SetEnvironmentData(1)
		else:
			background.SetEnvironmentData(0)	

		if systemSetting.IsSnowTexturesMode():
			if background.GetCurrentMapName():
				snow_maps = [
					"metin2_map_a1",
					"metin2_map_b1",
					"metin2_map_c1"
				]
				snow_maps_textures = {
					"metin2_map_a1" : "textureset\metin2_a1_snow.txt",
					"metin2_map_b1" : "textureset\metin2_b1_snow.txt",
					"metin2_map_c1" : "textureset\metin2_c1_snow.txt", }
				if str(background.GetCurrentMapName()) in snow_maps:
					background.TextureChange(snow_maps_textures[str(background.GetCurrentMapName())])
		if systemSetting.IsDesertTexturesMode():
			if background.GetCurrentMapName():
				snow_maps = [
					"metin2_map_a1",
					"metin2_map_b1",
					"metin2_map_c1"
				]
				snow_maps_textures = {
					"metin2_map_a1" : "textureset\metin2_a1_desert.txt",
					"metin2_map_b1" : "textureset\metin2_b1_desert.txt",
					"metin2_map_c1" : "textureset\metin2_c1_desert.txt", }
				if str(background.GetCurrentMapName()) in snow_maps:
					background.TextureChange(snow_maps_textures[str(background.GetCurrentMapName())])	
								
	def Close(self):
		self.Hide()

		global cameraDistance, cameraPitch, cameraRotation, cameraHeight
		(cameraDistance, cameraPitch, cameraRotation, cameraHeight) = app.GetCamera()

		if musicInfo.fieldMusic != "":
			snd.FadeOutMusic("BGM/"+ musicInfo.fieldMusic)

		self.onPressKeyDict = None
		self.onClickKeyDict = None
		
		
		self.uiNewShop.Hide()
		self.uiNewShopCreate.Hide()
		uiPrivateShopBuilder.Clear()
		uiPrivateShopBuilder.ClearBoardDict()

		chat.Close()	
		snd.StopAllSound()
		grp.InitScreenEffect()
		chr.Destroy()
		textTail.Clear()
		quest.Clear()
		background.Destroy()
		guild.Destroy()
		messenger.Destroy()
		skill.ClearSkillData()
		wndMgr.Unlock()
		mouseModule.mouseController.DeattachObject()

		if self.guildWarQuestionDialog:
			self.guildWarQuestionDialog.Close()

		self.guildNameBoard = None
		self.partyRequestQuestionDialog = None
		self.warPlayerCheckQuestionDialog = None
		self.partyInviteQuestionDialog = None
		self.guildInviteQuestionDialog = None
		self.guildWarQuestionDialog = None
		self.messengerAddFriendQuestion = None

		if app.ENABLE_CAPTCHA_SYSTEM:
			self.captchatest = None

		# UNKNOWN_UPDATE
		self.itemDropQuestionDialog = None
		# END_OF_UNKNOWN_UPDATE
		
		# QUEST_CONFIRM
		self.confirmDialog = None
		# END_OF_QUEST_CONFIRM
		
		self.PrintCoord = None
		self.FrameRate = None
		self.Pitch = None
		self.Splat = None
		self.TextureNum = None
		self.ObjectNum = None
		self.ViewDistance = None
		self.PrintMousePos = None

		self.ClearDictionary()
		
		self.support_shaman.Close()
		
		self.petmain.Close()
		self.petmini.Close()

		self.playerGauge = None

		if app.ENABLE_HEALTH_BOARD_SYSTEM:
			self.newPlayerGauge = None
		self.mapNameShower = None
		self.affectShower = None
		
		constInfo.SKILL_SELECT_POINTER = 0
		
		#uiPrivateShopBuilder.Clear()
		
		if self.console:
			self.console.BindGameClass(0)
			self.console.Close()
			self.console=None
		
		if self.targetBoard:
			self.targetBoard.Destroy()
			self.targetBoard = None
			
		if self.ruhtasi:
			self.ruhtasi.Destroy()
			self.ruhtasi = None
						
		if self.bkoku:
			self.bkoku.Destroy()
			self.bkoku = None
			
		if self.biyoekran:
			self.biyoekran.Destroy()
			self.biyoekran = None
			
		if self.biowindow:
			self.biowindow.Destroy()
			self.biowindow = None
			
		if self.bosstracking:
			self.bosstracking.Destroy()
			self.bosstracking = None
			
		if self.interface:
			self.interface.HideAllWindows()
			self.interface.Close()
			self.interface=None
			
		if app.ENABLE_AVERAGE_ITEM_PRICE:
			if self.average_price:
				self.average_price.Destroy()
				self.average_price = None
				
		if (app.ENABLE_BATTLE_ZONE_SYSTEM):
			if self.wndCombatZone:
				self.wndCombatZone.Close()
				
		if self.wndRR:
			self.wndRR.Close()
			self.wndRR = None
				
		player.ClearSkillDict()
		player.ResetCameraRotation()

		self.KillFocus()
		app.HideCursor()

		print "---------------------------------------------------------------------------- CLOSE GAME WINDOW"

	def __BuildKeyDict(self):
		onPressKeyDict = {}

		onPressKeyDict[app.DIK_1]	= lambda : self.PressNumKey(1)
		onPressKeyDict[app.DIK_2]	= lambda : self.PressNumKey(2)
		onPressKeyDict[app.DIK_3]	= lambda : self.PressNumKey(3)
		onPressKeyDict[app.DIK_4]	= lambda : self.PressNumKey(4)
		onPressKeyDict[app.DIK_5]	= lambda : self.PressNumKey(5)
		onPressKeyDict[app.DIK_6]	= lambda : self.PressNumKey(6)
		onPressKeyDict[app.DIK_7]	= lambda : self.PressNumKey(7)
		onPressKeyDict[app.DIK_8]	= lambda : self.PressNumKey(8)
		onPressKeyDict[app.DIK_9]	= lambda : self.PressNumKey(9)
		onPressKeyDict[app.DIK_F1]	= lambda : self.PressQuickSlot(4)
		onPressKeyDict[app.DIK_F2]	= lambda : self.PressQuickSlot(5)
		onPressKeyDict[app.DIK_F3]	= lambda : self.PressQuickSlot(6)
		onPressKeyDict[app.DIK_F4]	= lambda : self.PressQuickSlot(7)

		onPressKeyDict[app.DIK_5]	= lambda : self.PressNumNewKey(5)
		onPressKeyDict[app.DIK_6]	= lambda : self.PressNumNewKey(6)
		onPressKeyDict[app.DIK_7]	= lambda : self.PressNumNewKey(7)
		onPressKeyDict[app.DIK_8]	= lambda : self.PressNumNewKey(8)	
		
		onPressKeyDict[app.DIK_F9]	= lambda : self.OpenSupportShamanGui()

		onPressKeyDict[app.DIK_LALT]		= lambda : self.ShowName()
		onPressKeyDict[app.DIK_LCONTROL]	= lambda : self.ShowMouseImage()
		onPressKeyDict[app.DIK_SYSRQ]		= lambda : self.SaveScreen()
		onPressKeyDict[app.DIK_SPACE]		= lambda : self.StartAttack()

		#Ä³¸¯ÅÍ ÀÌµ¿Å°
		onPressKeyDict[app.DIK_UP]			= lambda : self.MoveUp()
		onPressKeyDict[app.DIK_DOWN]		= lambda : self.MoveDown()
		onPressKeyDict[app.DIK_LEFT]		= lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_RIGHT]		= lambda : self.MoveRight()
		onPressKeyDict[app.DIK_W]			= lambda : self.MoveUp()
		onPressKeyDict[app.DIK_S]			= lambda : self.MoveDown()
		onPressKeyDict[app.DIK_A]			= lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_D]			= lambda : self.MoveRight()

		onPressKeyDict[app.DIK_E]			= lambda: app.RotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_R]			= lambda: app.ZoomCamera(app.CAMERA_TO_NEGATIVE)
		#onPressKeyDict[app.DIK_F]			= lambda: app.ZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_T]			= lambda: app.PitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_G]			= self.PressGKey
		onPressKeyDict[app.DIK_Q]			= self.PressQKey

		onPressKeyDict[app.DIK_NUMPAD9]		= lambda: app.MovieResetCamera()
		onPressKeyDict[app.DIK_NUMPAD4]		= lambda: app.MovieRotateCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD6]		= lambda: app.MovieRotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_PGUP]		= lambda: app.MovieZoomCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_PGDN]		= lambda: app.MovieZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_NUMPAD8]		= lambda: app.MoviePitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD2]		= lambda: app.MoviePitchCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_GRAVE]		= lambda : self.PickUpItemNew()
		onPressKeyDict[app.DIK_Z]			= lambda : self.PickUpItemNew()
		#onPressKeyDict[app.DIK_X]			= lambda: self.interface.ToggleBonusWindow()
		onPressKeyDict[app.DIK_C]			= lambda state = "STATUS": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_V]			= lambda state = "SKILL": self.interface.ToggleCharacterWindow(state)
		#onPressKeyDict[app.DIK_B]			= lambda state = "EMOTICON": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_N]			= lambda state = "QUEST": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_O]			= lambda : self.interface.ToggleDragonSoulWindowWithNoInfo()
		onPressKeyDict[app.DIK_I]			= lambda : self.interface.ToggleInventoryWindow()

		onPressKeyDict[app.DIK_M]			= lambda : self.interface.PressMKey()
		#onPressKeyDict[app.DIK_H]			= lambda : self.interface.OpenHelpWindow()
		onPressKeyDict[app.DIK_ADD]			= lambda : self.interface.MiniMapScaleUp()
		onPressKeyDict[app.DIK_SUBTRACT]	= lambda : self.interface.MiniMapScaleDown()
		onPressKeyDict[app.DIK_L]			= lambda : self.interface.ToggleChatLogWindow()
		onPressKeyDict[app.DIK_COMMA]		= lambda : self.ShowConsole()		# "`" key
		onPressKeyDict[app.DIK_LSHIFT]		= lambda : self.SetQuickPageMode()

		onPressKeyDict[app.DIK_J]			= lambda : self.PressJKey()
		onPressKeyDict[app.DIK_H]			= lambda : self.PressHKey()
		onPressKeyDict[app.DIK_B]			= lambda : self.PressBKey()
		onPressKeyDict[app.DIK_F]			= lambda : self.PressFKey()
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			onPressKeyDict[app.DIK_K]		= lambda : self.PressExtendedInventory()

		# CUBE_TEST
		#onPressKeyDict[app.DIK_K]			= lambda : self.interface.OpenCubeWindow()
		# CUBE_TEST_END

		self.onPressKeyDict = onPressKeyDict

		onClickKeyDict = {}
		onClickKeyDict[app.DIK_UP] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_DOWN] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_LEFT] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_RIGHT] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_SPACE] = lambda : self.EndAttack()

		onClickKeyDict[app.DIK_W] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_S] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_A] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_D] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_Q] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_E] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_R] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_F] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_T] = lambda: app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_G] = lambda: self.__ReleaseGKey()
		onClickKeyDict[app.DIK_NUMPAD4] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD6] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGUP] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGDN] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD8] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD2] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_LALT] = lambda: self.HideName()
		onClickKeyDict[app.DIK_LCONTROL] = lambda: self.HideMouseImage()
		onClickKeyDict[app.DIK_LSHIFT] = lambda: self.SetQuickSlotMode()
		onClickKeyDict[app.DIK_P] = lambda: self.OpenPetMainGui()
		onClickKeyDict[app.DIK_U] = lambda: self.recordac()
		#if constInfo.PVPMODE_ACCELKEY_ENABLE:
		#	onClickKeyDict[app.DIK_B] = lambda: self.ChangePKMode()

		self.onClickKeyDict=onClickKeyDict
		
	if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
		def PressExtendedInventory(self):
			if self.interface:
				self.interface.ToggleExtendedInventoryWindow()

	def PressNumKey(self,num):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			
			if num >= 1 and num <= 9:
				if (chrmgr.IsPossibleEmoticon(-1)):				
					chrmgr.SetEmoticon(-1,int(num)-1)
					net.SendEmoticon(int(num)-1)
		else:
			if num >= 1 and num <= 4:
				self.pressNumber(num-1)

	def ClickBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			return
		else:
			if constInfo.PVPMODE_ACCELKEY_ENABLE:
				self.ChangePKMode()


	def	PressJKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if player.IsMountingHorse():
				net.SendChatPacket("/unmount")
			else:
				if not uiPrivateShopBuilder.IsBuildingPrivateShop():
					if app.ENABLE_MOUNT_SYSTEM:
						for i in xrange(player.INVENTORY_PAGE_SIZE * 4):
							index = player.GetItemIndex(i)
							if index:
								item.SelectItem(index)
								subType = item.GetItemSubType()
								if subType == item.USE_MOUNT:
									allow = 0
									net.SendItemUsePacket(i)
									break
					else:
						for i in xrange(player.INVENTORY_PAGE_SIZE * 4):
							if player.GetItemIndex(i) in (52043, 52044, 52045, 71164, 71165, 71166, 71167, 71168, 52091, 52092, 52093, 52094, 52095, 52096, 52097, 52098, 71161, 71131, 52033, 52005, 52019, 71176, 71177):
								net.SendItemUsePacket(i)
								break
	def	PressHKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_ride")
		else:
			self.interface.OpenHelpWindow()

	def	PressBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_back")
		else:
			state = "EMOTICON"
			self.interface.ToggleCharacterWindow(state)

	def	PressFKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_feed")	
		else:
			app.ZoomCamera(app.CAMERA_TO_POSITIVE)

	def PressGKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/ride")
		else:
			if self.ShowNameFlag:
				self.interface.ToggleGuildWindow()
			else:
				app.PitchCamera(app.CAMERA_TO_POSITIVE)
				
	def	ReleaseGKey(self):
		app.PitchCamera(app.CAMERA_STOP)
		
	def PressQKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if 0==interfaceModule.IsQBHide:
				interfaceModule.IsQBHide = 1
				self.interface.HideAllQuestButton()
			else:
				interfaceModule.IsQBHide = 0
				self.interface.ShowAllQuestButton()
		else:
			app.RotateCamera(app.CAMERA_TO_NEGATIVE)

	def PressJKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if player.IsmountingHorse():
				net.SendChatPacket("/unmount")
			else:
				self.teleport.Open()
	
	def SetQuickSlotMode(self):
		self.pressNumber=ui.__mem_func__(self.PressQuickSlot)

	def SetQuickPageMode(self):
		self.pressNumber=ui.__mem_func__(self.SelectQuickPage)

	def PressQuickSlot(self, localSlotIndex):
		player.RequestUseLocalQuickSlot(localSlotIndex)			

	def SelectQuickPage(self, pageIndex):
		self.quickSlotPageIndex = pageIndex
		player.SetQuickPage(pageIndex)
		
	def PressNumNewKey(self, key):
		if key > 8:
			return
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
		
			if key >= 5 and key <= 8:
				if(chrmgr.IsPossibleEmoticon(- 1)):				
					chrmgr.SetEmoticon(- 1, int(key) - 1)
					net.SendEmoticon(int(key) - 1)
		else:
			self.pressNewNumber((key - 1) - 4)
			

	def ToggleDebugInfo(self):
		self.isShowDebugInfo = not self.isShowDebugInfo

		if self.isShowDebugInfo:
			self.PrintCoord.Show()
			self.FrameRate.Show()
			self.Pitch.Show()
			self.Splat.Show()
			self.TextureNum.Show()
			self.ObjectNum.Show()
			self.ViewDistance.Show()
			self.PrintMousePos.Show()
		else:
			self.PrintCoord.Hide()
			self.FrameRate.Hide()
			self.Pitch.Hide()
			self.Splat.Hide()
			self.TextureNum.Hide()
			self.ObjectNum.Hide()
			self.ViewDistance.Hide()
			self.PrintMousePos.Hide()

	def __BuildDebugInfo(self):
		## Character Position Coordinate
		self.PrintCoord = ui.TextLine()
		self.PrintCoord.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintCoord.SetPosition(wndMgr.GetScreenWidth() - 270, 0)
		
		## Frame Rate
		self.FrameRate = ui.TextLine()
		self.FrameRate.SetFontName(localeInfo.UI_DEF_FONT)
		self.FrameRate.SetPosition(wndMgr.GetScreenWidth() - 270, 20)

		## Camera Pitch
		self.Pitch = ui.TextLine()
		self.Pitch.SetFontName(localeInfo.UI_DEF_FONT)
		self.Pitch.SetPosition(wndMgr.GetScreenWidth() - 270, 40)

		## Splat
		self.Splat = ui.TextLine()
		self.Splat.SetFontName(localeInfo.UI_DEF_FONT)
		self.Splat.SetPosition(wndMgr.GetScreenWidth() - 270, 60)
		
		##
		self.PrintMousePos = ui.TextLine()
		self.PrintMousePos.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintMousePos.SetPosition(wndMgr.GetScreenWidth() - 270, 80)

		# TextureNum
		self.TextureNum = ui.TextLine()
		self.TextureNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.TextureNum.SetPosition(wndMgr.GetScreenWidth() - 270, 100)

		# ¿ÀºêÁ§Æ® ±×¸®´Â °³¼ö
		self.ObjectNum = ui.TextLine()
		self.ObjectNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.ObjectNum.SetPosition(wndMgr.GetScreenWidth() - 270, 120)

		# ½Ã¾ß°Å¸®
		self.ViewDistance = ui.TextLine()
		self.ViewDistance.SetFontName(localeInfo.UI_DEF_FONT)
		self.ViewDistance.SetPosition(0, 0)

	def __NotifyError(self, msg):
		chat.AppendChat(chat.CHAT_TYPE_INFO, msg)
		
	if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
		def PressNumberKey(self, key):
			self.pressNumber(key)
			
		def PressQuickSlot(self, key):
			player.RequestUseLocalQuickSlot(key)
			
		def SelectQuickPage(self, key):
			if app.IsPressed(app.DIK_LSHIFT):
				self.quickSlotPageIndex = key
				player.SetQuickPage(key)
			
		def SetEmoticon(self, key):
			if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
				if (chrmgr.IsPossibleEmoticon(-1)):				
					chrmgr.SetEmoticon(-1, key)
					net.SendEmoticon(key)
				
		def ShowNameNew(self):
			self.ShowNameFlag = True
			self.playerGauge.EnableShowAlways()

		def HideNameNew(self):
			self.ShowNameFlag = False
			self.playerGauge.DisableShowAlways()

		def PressExtraKey(self, key):
			if key == "RIDE_HORSE":
				if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
					net.SendChatPacket("/user_horse_ride")
				
			elif key == "RIDE_PEED":
				if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
					net.SendChatPacket("/user_horse_feed")
				
			elif key == "RIDE":
				if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
					net.SendChatPacket("/ride")
				else:
					if self.ShowNameFlag:
						self.interface.ToggleGuildWindow()
					else:
						app.PitchCamera(app.CAMERA_TO_POSITIVE)
										
			elif key == "RIDE_J":
				if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
					if player.IsMountingHorse():
						net.SendChatPacket("/unmount")
					else:
						if not uiPrivateShopBuilder.IsBuildingPrivateShop():
							if app.ENABLE_MOUNT_SYSTEM:
								for i in xrange(180):
									index = player.GetItemIndex(i)
									if index:
										item.SelectItem(index)
										subType = item.GetItemSubType()
										if subType == item.USE_MOUNT:
											net.SendItemUsePacket(i)
											break
							else:
								return
			
			elif key == "RIDE_BYE":
				net.SendChatPacket("/user_horse_back")
		
			elif key == "QUEST_SCROLL_ONOFF":
				if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
					if 0 == interfaceModule.IsQBHide:
						interfaceModule.IsQBHide = 1
						self.interface.HideAllQuestButton()
					else:
						interfaceModule.IsQBHide = 0
						self.interface.ShowAllQuestButton()
					
			elif key == "GUILD_WINDOW":
				if app.IsPressed(app.DIK_LALT):
					self.interface.ToggleGuildWindow()
					
			elif key == "MESSENGER_WINDOW":
				if app.IsPressed(app.DIK_LCONTROL):
					self.interface.ToggleMessenger()

			elif key == "SHOW_MINIMAP":
				if app.IsPressed(app.DIK_LSHIFT):
					if False == self.interface.wndMiniMap.isShowMiniMap():
						self.interface.wndMiniMap.ShowMiniMap()
						self.interface.wndMiniMap.SetTop()
					else:
						self.interface.wndMiniMap.HideMiniMap()
					
			elif key == "HIDE_PM":
				if 0 == interfaceModule.IsWisperHide:
					interfaceModule.IsWisperHide = 1
					self.interface.HideAllWhisperButton()
				else:
					interfaceModule.IsWisperHide = 0
					self.interface.ShowAllWhisperButton()
					
			elif key == "OPEN_WHISPER":
				self.interface.OpenWhisperDialogWithoutTarget()
				

	def ChangePKMode(self):

		if not app.IsPressed(app.DIK_LCONTROL):
			return

		if player.GetStatus(player.LEVEL)<constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__NotifyError(localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return

		curTime = app.GetTime()
		if curTime - self.lastPKModeSendedTime < constInfo.PVPMODE_ACCELKEY_DELAY:
			return

		self.lastPKModeSendedTime = curTime

		curPKMode = player.GetPKMode()
		nextPKMode = curPKMode + 1
		if nextPKMode == player.PK_MODE_PROTECT:
			if 0 == player.GetGuildID():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
				nextPKMode = 0
			else:
				nextPKMode = player.PK_MODE_GUILD

		elif nextPKMode == player.PK_MODE_MAX_NUM:
			nextPKMode = 0

		net.SendChatPacket("/PKMode " + str(nextPKMode))
		print "/PKMode " + str(nextPKMode)

	def OnChangePKMode(self):

		self.interface.OnChangePKMode()

		try:
			self.__NotifyError(localeInfo.OPTION_PVPMODE_MESSAGE_DICT[player.GetPKMode()])
		except KeyError:
			print "UNKNOWN PVPMode[%d]" % (player.GetPKMode())

		if constInfo.PVPMODE_TEST_ENABLE:
			curPKMode = player.GetPKMode()
			alignment, grade = chr.testGetPKData()
			self.pkModeNameDict = { 0 : "PEACE", 1 : "REVENGE", 2 : "FREE", 3 : "PROTECT", }
			self.testPKMode.SetText("Current PK Mode : " + self.pkModeNameDict.get(curPKMode, "UNKNOWN"))
			self.testAlignment.SetText("Current Alignment : " + str(alignment) + " (" + localeInfo.TITLE_NAME_LIST[grade] + ")")

	###############################################################################################
	###############################################################################################
	## Game Callback Functions

	# Start
	def StartGame(self):
		self.RefreshInventory()
		self.RefreshEquipment()
		self.RefreshCharacter()
		self.RefreshSkill()

	# Refresh
	def CheckGameButton(self):
		if self.interface:
			self.interface.CheckGameButton()

	def RefreshAlignment(self):
		self.interface.RefreshAlignment()

	def RefreshStatus(self):
		self.CheckGameButton()

		if self.interface:
			self.interface.RefreshStatus()

		if self.playerGauge:
			self.playerGauge.RefreshGauge()

	def RefreshStamina(self):
		self.interface.RefreshStamina()

	def RefreshSkill(self):
		self.CheckGameButton()
		if self.interface:
			self.interface.RefreshSkill()

	def RefreshQuest(self):
		self.interface.RefreshQuest()

	def RefreshMessenger(self):
		self.interface.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.interface.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.interface.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.interface.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.interface.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.interface.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.interface.RefreshGuildGradePage()

	def RefreshMobile(self):
		if self.interface:
			self.interface.RefreshMobile()

	def OnMobileAuthority(self):
		self.interface.OnMobileAuthority()

	def OnBlockMode(self, mode):
		self.interface.OnBlockMode(mode)

	def OpenQuestWindow(self, skin, idx):
		if constInfo.INPUT_IGNORE == 1:
			return
	
		self.interface.OpenQuestWindow(skin, idx)
		
	def HideAllQuestWindow(self):
		self.interface.HideAllQuestWindow()
		
	def GetInputOn(self):
		constInfo.INPUT_IGNORE = 1
 
	def GetInputOff(self):
		constInfo.INPUT_IGNORE = 0
 
	def AskGuildName(self):

		guildNameBoard = uiCommon.InputDialog()
		guildNameBoard.SetTitle(localeInfo.GUILD_NAME)
		guildNameBoard.SetAcceptEvent(ui.__mem_func__(self.ConfirmGuildName))
		guildNameBoard.SetCancelEvent(ui.__mem_func__(self.CancelGuildName))
		guildNameBoard.Open()

		self.guildNameBoard = guildNameBoard

	def ConfirmGuildName(self):
		guildName = self.guildNameBoard.GetText()
		if not guildName:
			return

		if net.IsInsultIn(guildName):
			self.PopupMessage(localeInfo.GUILD_CREATE_ERROR_INSULT_NAME)
			return

		net.SendAnswerMakeGuildPacket(guildName)
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	def CancelGuildName(self):
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	## Refine
	def PopupMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, 0, localeInfo.UI_OK)

	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type=0):
		self.interface.OpenRefineDialog(targetItemPos, nextGradeItemVnum, cost, prob, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.interface.AppendMaterialToRefineDialog(vnum, count)

	def RunUseSkillEvent(self, slotIndex, coolTime):
		self.interface.OnUseSkill(slotIndex, coolTime)

	def ClearAffects(self):
		self.affectShower.ClearAffects()

	def SetAffect(self, affect):
		self.affectShower.SetAffect(affect)

	def ResetAffect(self, affect):
		self.affectShower.ResetAffect(affect)

	# UNKNOWN_UPDATE
	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration):
		self.affectShower.BINARY_NEW_AddAffect(type, pointIdx, value, duration)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulActivate(type - chr.NEW_AFFECT_DRAGON_SOUL_DECK1)
		elif chr.NEW_AFFECT_DRAGON_SOUL_QUALIFIED == type:
			self.BINARY_DragonSoulGiveQuilification()

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		self.affectShower.BINARY_NEW_RemoveAffect(type, pointIdx)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulDeactivate()

	# END_OF_UNKNOWN_UPDATE

	def ActivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnActivateSkill(slotIndex)

	def DeactivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnDeactivateSkill(slotIndex)

	def RefreshEquipment(self):
		if self.interface:
			self.interface.RefreshInventory()

	def RefreshInventory(self):
		if self.interface:
			self.interface.RefreshInventory()

	def RefreshCharacter(self):
		if self.interface:
			self.interface.RefreshCharacter()

	def OnGameOver(self):
		self.CloseTargetBoard()
		self.OpenRestartDialog()

	def OpenRestartDialog(self):
		self.interface.OpenRestartDialog()

	def ChangeCurrentSkill(self, skillSlotNumber):
		self.interface.OnChangeCurrentSkill(skillSlotNumber)
		
	## TargetBoard
	def SetPCTargetBoard(self, vid, name):
		self.targetBoard.Open(vid, name)
		
		if app.IsPressed(app.DIK_LCONTROL):
			
			if not player.IsSameEmpire(vid):
				return

			if player.IsMainCharacterIndex(vid):
				return		
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(vid):
				return

			self.interface.OpenWhisperDialog(name)

	def RefreshTargetBoardByVID(self, vid):
		self.targetBoard.RefreshByVID(vid)

	def RefreshTargetBoardByName(self, name):
		self.targetBoard.RefreshByName(name)
		
	def __RefreshTargetBoard(self):
		self.targetBoard.Refresh()
		
	if app.ENABLE_VIEW_TARGET_MONSTER_HP:
		def SetHPTargetBoard(self, vid, hpPercentage, iMinHP, iMaxHP, bElement):
			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)
			
			self.targetBoard.SetHP(hpPercentage, iMinHP, iMaxHP)
			self.targetBoard.SetElementImage(bElement)
			self.targetBoard.Show()
	else:
		def SetHPTargetBoard(self, vid, hpPercentage, bElement):
			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)
			
			self.targetBoard.SetHP(hpPercentage)
			self.targetBoard.SetElementImage(bElement)
			self.targetBoard.Show()
			
	def SetElementImage(self, bElement):
		if self.targetBoard:
			self.targetBoard.SetElementImage(bElement)

	def CloseTargetBoardIfDifferent(self, vid):
		if vid != self.targetBoard.GetTargetVID():
			self.targetBoard.Close()

	def CloseTargetBoard(self):
		self.targetBoard.Close()

	## View Equipment
	def OpenEquipmentDialog(self, vid):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			self.interface.OpenEquipmentDialog(vid)

	def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count, sealbind, transmutation):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			self.interface.SetEquipmentDialogItem(vid, slotIndex, vnum, count, sealbind, transmutation)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			self.interface.SetEquipmentDialogSocket(vid, slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			self.interface.SetEquipmentDialogAttr(vid, slotIndex, attrIndex, type, value)
		
	def ViewEquipRequest(self, vid):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			vid = int(vid)
			eqRequestQuestionDialog = uiCommon.QuestionDialog()
			eqRequestQuestionDialog.SetText(chr.GetNameByVID(vid) + ' giydiklerini g\xf6rmek istiyor?')
			eqRequestQuestionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
			eqRequestQuestionDialog.SetCancelText(localeInfo.UI_DENY)
			eqRequestQuestionDialog.SetAcceptEvent(lambda arg = True: self.AnswerViewEquipRequest(arg))
			eqRequestQuestionDialog.SetCancelEvent(lambda arg = False: self.AnswerViewEquipRequest(arg))
			eqRequestQuestionDialog.Open()
			eqRequestQuestionDialog.vid = vid
			self.eqRequestQuestionDialog = eqRequestQuestionDialog

	def AnswerViewEquipRequest(self, answer):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			if not self.eqRequestQuestionDialog:
				return
			else:
				vid = self.eqRequestQuestionDialog.vid
				if answer:
					net.SendChatPacket('/view_equip_accept ' + str(vid))
				else:
					net.SendChatPacket('/view_equip_deny ' + str(vid))
				self.eqRequestQuestionDialog.Close()
				self.eqRequestQuestionDialog = None
				return

	def ViewEquipRequestDenied(self):
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			self.PopupMessage('Z\xfdrh g\xf6r\xfcnt\xfcleme iste\xf0i reddedildi.')
			
	# SHOW_LOCAL_MAP_NAME
	def ShowMapName(self, mapName, x, y):

		if self.mapNameShower:
			self.mapNameShower.ShowMapName(mapName, x, y)

		if self.interface:
			self.interface.SetMapName(mapName)
	# END_OF_SHOW_LOCAL_MAP_NAME	

	def BINARY_OpenAtlasWindow(self):
		self.interface.BINARY_OpenAtlasWindow()

	## Chat
	def OnRecvWhisper(self, mode, name, line):
			
		if mode == chat.WHISPER_TYPE_GM:
			self.interface.RegisterGameMasterName(name)
		else:
			pass
			
		chat.AppendToBox(mode, name, line)
		self.interface.RecvWhisper(name)
		
	def OnRecvWhisperSystemMessage(self, mode, name, line):
		chat.AppendToBox(chat.WHISPER_TYPE_SYSTEM, name, line)
		self.interface.RecvWhisper(name)

	def OnRecvWhisperError(self, mode, name, line):
		if localeInfo.WHISPER_ERROR.has_key(mode):
			chat.AppendToBox(chat.WHISPER_TYPE_SYSTEM, name, localeInfo.WHISPER_ERROR[mode](name))
		else:
			chat.AppendToBox(chat.WHISPER_TYPE_SYSTEM, name, "Whisper Unknown Error(mode=%d, name=%s)" % (mode, name))
		self.interface.RecvWhisper(name)

	def RecvWhisper(self, name):
		self.interface.RecvWhisper(name)

	def OnPickMoney(self, money):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_PICK_MONEY % (localeInfo.NumberToString(money)))
	if app.ENABLE_CHEQUE_SYSTEM:
		def OnPickCheque(self, cheque):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHEQUE_SYSTEM_PICK_WON % (localeInfo.NumberToString(cheque)))
			
	def OnPickGem(self, gem):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_PICK_GEM % (localeInfo.NumberToString(gem)))
			
	def OnPickYearRoundPoint(self, year_round_point):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_PICK_YEAR_ROUND_POINT % (localeInfo.NumberToString(year_round_point)))
			
	def OnShopError(self, type):
		try:
			self.PopupMessage(localeInfo.SHOP_ERROR_DICT[type])
		except KeyError:
			self.PopupMessage(localeInfo.SHOP_ERROR_UNKNOWN % (type))

	def OnSafeBoxError(self):
		self.PopupMessage(localeInfo.SAFEBOX_ERROR)
		self.interface.InitSafeboxPassword()

	def OnFishingSuccess(self, isFish, fishName):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_SUCCESS(isFish, fishName), 2000)

	# ADD_FISHING_MESSAGE
	def OnFishingNotifyUnknown(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_UNKNOWN)

	def OnFishingWrongPlace(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_WRONG_PLACE)
	# END_OF_ADD_FISHING_MESSAGE

	def OnFishingNotify(self, isFish, fishName):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_NOTIFY(isFish, fishName))

	def OnFishingFailure(self):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_FAILURE, 2000)

	def OnCannotPickItem(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_PICK_ITEM)

	# MINING
	def OnCannotMining(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_MINING)
	# END_OF_MINING

	def OnCannotUseSkill(self, vid, type):
		if localeInfo.USE_SKILL_ERROR_TAIL_DICT.has_key(type):
			textTail.RegisterInfoTail(vid, localeInfo.USE_SKILL_ERROR_TAIL_DICT[type])

		if localeInfo.USE_SKILL_ERROR_CHAT_DICT.has_key(type):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_SKILL_ERROR_CHAT_DICT[type])

	def	OnCannotShotError(self, vid, type):
		textTail.RegisterInfoTail(vid, localeInfo.SHOT_ERROR_TAIL_DICT.get(type, localeInfo.SHOT_ERROR_UNKNOWN % (type)))

	## PointReset
	def StartPointReset(self):
		self.interface.OpenPointResetDialog()

	## Shop
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def StartShop(self, vid, points, curLimit, maxLimit, count):
			if(vid == constInfo.MARKED_SHOP_VID):
				background.DeletePrivateShopPos()
				constInfo.MARKED_SHOP_VID = 0
			self.interface.OpenShopDialog(vid, points, curLimit, maxLimit, count)
	else:
		def StartShop(self, vid):
			if(vid == constInfo.MARKED_SHOP_VID):
				background.DeletePrivateShopPos()
				constInfo.MARKED_SHOP_VID = 0
			self.interface.OpenShopDialog(vid)

	def EndShop(self):
		self.interface.CloseShopDialog()

	def RefreshShop(self):
		self.interface.RefreshShopDialog()

	def SetShopSellingPrice(self, Price):
		pass

	## Exchange
	def StartExchange(self):
		self.interface.StartExchange()

	def EndExchange(self):
		self.interface.EndExchange()

	def RefreshExchange(self):
		self.interface.RefreshExchange()

	## Party
	def RecvPartyInviteQuestion(self, leaderVID, leaderName):
		partyInviteQuestionDialog = uiCommon.QuestionDialogWithTimeLimit3()
		partyInviteQuestionDialog.SetText1(leaderName + localeInfo.PARTY_DO_YOU_JOIN)
		partyInviteQuestionDialog.SetTimeOverMsg(localeInfo.PARTY_ANSWER_TIMEOVER)
		partyInviteQuestionDialog.SetTimeOverEvent(self.AnswerPartyInvite, False)
		partyInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerPartyInvite(arg))
		partyInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerPartyInvite(arg))
		partyInviteQuestionDialog.Open(10)
		partyInviteQuestionDialog.partyLeaderVID = leaderVID
		self.partyInviteQuestionDialog = partyInviteQuestionDialog

	def AnswerPartyInvite(self, answer):

		if not self.partyInviteQuestionDialog:
			return

		partyLeaderVID = self.partyInviteQuestionDialog.partyLeaderVID

		distance = player.GetCharacterDistance(partyLeaderVID)
		if distance < 0.0 or distance > 5000:
			answer = False

		net.SendPartyInviteAnswerPacket(partyLeaderVID, answer)

		self.partyInviteQuestionDialog.Close()
		self.partyInviteQuestionDialog = None

	if app.ENABLE_PARTY_UPDATE:
		def AddPartyMember(self, pid, name, mapIdx, channel):
			self.interface.AddPartyMember(pid, name, mapIdx, channel)
	else:
		def AddPartyMember(self, pid, name):
			self.interface.AddPartyMember(pid, name)

	def UpdatePartyMemberInfo(self, pid):
		self.interface.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.interface.RemovePartyMember(pid)
		self.__RefreshTargetBoard()

	if app.ENABLE_PARTY_UPDATE:
		def LinkPartyMember(self, pid, vid, mapIdx, channel):
			self.interface.LinkPartyMember(pid, vid, mapIdx, channel)
	else:
		def LinkPartyMember(self, pid, vid):
			self.interface.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.interface.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.interface.UnlinkAllPartyMember()

	def ExitParty(self):
		self.interface.ExitParty()
		self.RefreshTargetBoardByVID(self.targetBoard.GetTargetVID())

	def ChangePartyParameter(self, distributionMode):
		self.interface.ChangePartyParameter(distributionMode)

	## Messenger
	def OnMessengerAddFriendQuestion(self, name):
		messengerAddFriendQuestion = uiCommon.QuestionDialogWithTimeLimit3()
		messengerAddFriendQuestion.SetText1(localeInfo.MESSENGER_DO_YOU_ACCEPT_ADD_FRIEND % (name))
		messengerAddFriendQuestion.SetTimeOverMsg(localeInfo.MESSENGER_ADD_FRIEND_ANSWER_TIMEOVER)
		messengerAddFriendQuestion.SetTimeOverEvent(self.OnDenyAddFriend)
		messengerAddFriendQuestion.SetAcceptEvent(ui.__mem_func__(self.OnAcceptAddFriend))
		messengerAddFriendQuestion.SetCancelEvent(ui.__mem_func__(self.OnDenyAddFriend))
		messengerAddFriendQuestion.Open(10)
		messengerAddFriendQuestion.name = name
		self.messengerAddFriendQuestion = messengerAddFriendQuestion

	def OnAcceptAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth y " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnDenyAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth n " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnCloseAddFriendQuestionDialog(self):
		self.messengerAddFriendQuestion.Close()
		self.messengerAddFriendQuestion = None
		return True

	## SafeBox
	def OpenSafeboxWindow(self, size):
		self.interface.OpenSafeboxWindow(size)

	def RefreshSafebox(self):
		self.interface.RefreshSafebox()

	def RefreshSafeboxMoney(self):
		self.interface.RefreshSafeboxMoney()

	# ITEM_MALL
	def OpenMallWindow(self, size):
		self.interface.OpenMallWindow(size)

	def RefreshMall(self):
		self.interface.RefreshMall()
	# END_OF_ITEM_MALL

	## Guild
	def RecvGuildInviteQuestion(self, guildID, guildName):
		guildInviteQuestionDialog = uiCommon.QuestionDialog()
		guildInviteQuestionDialog.SetText(guildName + localeInfo.GUILD_DO_YOU_JOIN)
		guildInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.Open()
		guildInviteQuestionDialog.guildID = guildID
		self.guildInviteQuestionDialog = guildInviteQuestionDialog

	def AnswerGuildInvite(self, answer):

		if not self.guildInviteQuestionDialog:
			return

		guildLeaderVID = self.guildInviteQuestionDialog.guildID
		net.SendGuildInviteAnswerPacket(guildLeaderVID, answer)

		self.guildInviteQuestionDialog.Close()
		self.guildInviteQuestionDialog = None

	
	def DeleteGuild(self):
		self.interface.DeleteGuild()

	## Clock
	def ShowClock(self, second):
		self.interface.ShowClock(second)

	def HideClock(self):
		self.interface.HideClock()

	## Emotion
	def BINARY_ActEmotion(self, emotionIndex):
		if self.interface.wndCharacter:
			self.interface.wndCharacter.ActEmotion(emotionIndex)

	###############################################################################################
	###############################################################################################
	## Keyboard Functions

	def CheckFocus(self):
		if False == self.IsFocus():
			if True == self.interface.IsOpenChat():
				self.interface.ToggleChat()

			self.SetFocus()

	def SaveScreen(self):
		print "save screen"

		# SCREENSHOT_CWDSAVE
		if SCREENSHOT_CWDSAVE:
			if not os.path.exists(os.getcwd()+os.sep+"screenshot"):
				os.mkdir(os.getcwd()+os.sep+"screenshot")

			(succeeded, name) = grp.SaveScreenShotToPath(os.getcwd()+os.sep+"screenshot"+os.sep)
		elif SCREENSHOT_DIR:
			(succeeded, name) = grp.SaveScreenShot(SCREENSHOT_DIR)
		else:
			(succeeded, name) = grp.SaveScreenShot()
		# END_OF_SCREENSHOT_CWDSAVE

		if succeeded:
			pass
			"""
			chat.AppendChat(chat.CHAT_TYPE_INFO, name + localeInfo.SCREENSHOT_SAVE1)
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE2)
			"""
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE_FAILURE)

	def ShowConsole(self):
		pass

	def ShowName(self):
		self.ShowNameFlag = True
		self.playerGauge.EnableShowAlways()
		player.SetQuickPage(self.quickSlotPageIndex+1)

	# ADD_ALWAYS_SHOW_NAME
	def __IsShowName(self):
		if systemSetting.IsAlwaysShowName() == 0:
			return True

		if self.ShowNameFlag:
			return True

		return False

	def __IsShowNameItem(self):
		if systemSetting.IsAlwaysShowName() == 1:
			return True

		return False
	# END_OF_ADD_ALWAYS_SHOW_NAME
	
	def HideName(self):
		self.ShowNameFlag = False
		self.playerGauge.DisableShowAlways()
		player.SetQuickPage(self.quickSlotPageIndex)

	def ShowMouseImage(self):
		self.interface.ShowMouseImage()

	def HideMouseImage(self):
		self.interface.HideMouseImage()

	def StartAttack(self):
		player.SetAttackKeyState(True)

	def EndAttack(self):
		player.SetAttackKeyState(False)

	def MoveUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, True)

	def MoveDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, True)

	def MoveLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, True)

	def MoveRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, True)

	def StopUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, False)

	def StopDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, False)

	def StopLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, False)

	def StopRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, False)

	def PickUpItem(self):
		player.PickCloseItem()

	def PickUpItemNew(self):
		player.PickCloseItemVector()

	###############################################################################################
	###############################################################################################
	## Event Handler

	def OnKeyDown(self, key):
		if self.interface.wndWeb and self.interface.wndWeb.IsShow():
			return

		if key == app.DIK_ESC:
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
			
		if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
			if self.interface:
				self.dlgKeyboardSettings.OnKeyDown(key)

		try:
			if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
				pass
			else:
				self.onPressKeyDict[key]()
		except KeyError:
			pass
		except:
			raise

		return True

	def OnKeyUp(self, key):
		if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
			if self.interface:
				self.dlgKeyboardSettings.OnKeyUp(key)
		try:
			if app.ENABLE_KEYBOARD_SETTINGS_SYSTEM:
				pass
			else:
				self.onClickKeyDict[key]()
		except KeyError:
			pass
		except:
			raise
		return True

	def OnMouseLeftButtonDown(self):
		if self.interface.BUILD_OnMouseLeftButtonDown():
			return

		if mouseModule.mouseController.isAttached():
			self.CheckFocus()
		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				return
			else:
				self.CheckFocus()
				player.SetMouseState(player.MBT_LEFT, player.MBS_PRESS);

		return True

	def OnMouseLeftButtonUp(self):

		if self.interface.BUILD_OnMouseLeftButtonUp():
			return

		if mouseModule.mouseController.isAttached():

			attachedType = mouseModule.mouseController.GetAttachedType()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
			attachedItemSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()

			## QuickSlot
			if player.SLOT_TYPE_QUICK_SLOT == attachedType:
				player.RequestDeleteGlobalQuickSlot(attachedItemSlotPos)
		
			if app.ENABLE_AUTO_HUNTING_SYSTEM and player.SLOT_TYPE_AUTOHUNT_ITEM == attachedType:
				self.interface.AutohuntRemoveItemSlot(attachedItemSlotPos)

			if app.ENABLE_AUTO_HUNTING_SYSTEM and player.SLOT_TYPE_AUTOHUNT_SKILL == attachedType:
				self.interface.AutohuntRemoveSkillSlot(attachedItemSlotPos)
		
			## Inventory
			elif player.SLOT_TYPE_INVENTORY == attachedType or player.SLOT_TYPE_SKILL_BOOK_INVENTORY == attachedType or player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY == attachedType or player.SLOT_TYPE_STONE_INVENTORY == attachedType or player.SLOT_TYPE_CHEST_INVENTORY == attachedType or player.SLOT_TYPE_ATTR_INVENTORY == attachedType or player.SLOT_TYPE_FLOWER_INVENTORY == attachedType:

				if player.ITEM_MONEY == attachedItemIndex:
					self.__PutMoney(attachedType, attachedItemCount, self.PickingCharacterIndex)
				else:
					self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)

			## DragonSoul
			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)
			
			mouseModule.mouseController.DeattachObject()

		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				if app.IsPressed(app.DIK_LALT):
					link = chat.GetLinkFromHyperlink(hyperlink)
					ime.PasteString(link)
				else:
					self.interface.MakeHyperlinkTooltip(hyperlink)
				return
			else:
				player.SetMouseState(player.MBT_LEFT, player.MBS_CLICK)

		#player.EndMouseWalking()
		return True

	def __PutItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, dstChrID):
		if player.SLOT_TYPE_INVENTORY == attachedType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType or player.SLOT_TYPE_SKILL_BOOK_INVENTORY == attachedType or player.SLOT_TYPE_UPGRADE_ITEMS_INVENTORY == attachedType or player.SLOT_TYPE_STONE_INVENTORY == attachedType or player.SLOT_TYPE_CHEST_INVENTORY == attachedType or player.SLOT_TYPE_ATTR_INVENTORY == attachedType or player.SLOT_TYPE_FLOWER_INVENTORY:
			attachedInvenType = player.SlotTypeToInvenType(attachedType)
			if True == chr.HasInstance(self.PickingCharacterIndex) and player.GetMainCharacterIndex() != dstChrID:
				if player.IsEquipmentSlot(attachedItemSlotPos):
					self.stream.popupWindow.Close()
					self.stream.popupWindow.Open(localeInfo.EXCHANGE_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)
				else:
					if chr.IsNPC(dstChrID):
						net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
						constInfo.AUTO_REFINE_TYPE = 2
						constInfo.AUTO_REFINE_DATA["NPC"][0] = dstChrID
						constInfo.AUTO_REFINE_DATA["NPC"][1] = attachedInvenType
						constInfo.AUTO_REFINE_DATA["NPC"][2] = attachedItemSlotPos
						constInfo.AUTO_REFINE_DATA["NPC"][3] = attachedItemCount
					else:
						if app.ENABLE_MELEY_LAIR_DUNGEON:
							if chr.IsStone(dstChrID):
								net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
							else:
								net.SendExchangeStartPacket(dstChrID)
								net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
						else:
							net.SendExchangeStartPacket(dstChrID)
							net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
			else:
				self.interface.DeleteItem(attachedItemSlotPos, attachedInvenType)

	def __PutMoney(self, attachedType, attachedMoney, dstChrID):
		if True == chr.HasInstance(dstChrID) and player.GetMainCharacterIndex() != dstChrID:
			net.SendExchangeStartPacket(dstChrID)
			net.SendExchangeElkAddPacket(attachedMoney)

	def OnMouseRightButtonDown(self):

		self.CheckFocus()

		if True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()

		else:
			player.SetMouseState(player.MBT_RIGHT, player.MBS_PRESS)

		return True

	def OnMouseRightButtonUp(self):
		if True == mouseModule.mouseController.isAttached():
			return True

		player.SetMouseState(player.MBT_RIGHT, player.MBS_CLICK)
		return True

	def OnMouseMiddleButtonDown(self):
		player.SetMouseMiddleButtonState(player.MBS_PRESS)

	def OnMouseMiddleButtonUp(self):
		player.SetMouseMiddleButtonState(player.MBS_CLICK)
		
	def CMDBiyologPenceresiAc(self):
		constInfo.OPEN_BIO_WND = 1
		
	def OpenBiologWindow(self):
		self.biowindow.Show()

	def OnUpdate(self):	
		app.UpdateGame()

		if 1 == constInfo.OPEN_BIO_WND:
			constInfo.OPEN_BIO_WND = 0
			self.OpenBiologWindow()

		if self.mapNameShower.IsShow():
			self.mapNameShower.Update()

		if self.isShowDebugInfo:
			self.UpdateDebugInfo()

		if self.enableXMasBoom:
			self.__XMasBoom_Update()

		if 1 == constInfo.OTO_TOPLAMA_NEW:
			self.PickUpItemNew()

		self.interface.BUILD_OnUpdate()
			
		if app.GetRandom(1, 200) == 100:
			app.SetTitle(localeInfo.TITLE_APPEND[app.GetRandom(0, len(localeInfo.TITLE_APPEND) - 1)])
			
		if systemSetting.IsAutoPickUp():
			self.PickUpItem()
					
		if constInfo.video_izle==1:
			self.__video_izle()
			constInfo.video_izle=0
			#self.PetToplama(169999)
		
	def UpdateDebugInfo(self):
		#
		# Ä³¸¯ÅÍ ÁÂÇ¥ ¹× FPS Ãâ·Â
		(x, y, z) = player.GetMainCharacterPosition()
		nUpdateTime = app.GetUpdateTime()
		nUpdateFPS = app.GetUpdateFPS()
		nRenderFPS = app.GetRenderFPS()
		nFaceCount = app.GetFaceCount()
		fFaceSpeed = app.GetFaceSpeed()
		nST=background.GetRenderShadowTime()
		(fAveRT, nCurRT) =  app.GetRenderTime()
		(iNum, fFogStart, fFogEnd, fFarCilp) = background.GetDistanceSetInfo()
		(iPatch, iSplat, fSplatRatio, sTextureNum) = background.GetRenderedSplatNum()
		if iPatch == 0:
			iPatch = 1

		#(dwRenderedThing, dwRenderedCRC) = background.GetRenderedGraphicThingInstanceNum()

		self.PrintCoord.SetText("Coordinate: %.2f %.2f %.2f ATM: %d" % (x, y, z, app.GetAvailableTextureMemory()/(1024*1024)))
		xMouse, yMouse = wndMgr.GetMousePosition()
		self.PrintMousePos.SetText("MousePosition: %d %d" % (xMouse, yMouse))			

		self.FrameRate.SetText("UFPS: %3d UT: %3d FS %.2f" % (nUpdateFPS, nUpdateTime, fFaceSpeed))

		if fAveRT>1.0:
			self.Pitch.SetText("RFPS: %3d RT:%.2f(%3d) FC: %d(%.2f) " % (nRenderFPS, fAveRT, nCurRT, nFaceCount, nFaceCount/fAveRT))

		self.Splat.SetText("PATCH: %d SPLAT: %d BAD(%.2f)" % (iPatch, iSplat, fSplatRatio))
		#self.Pitch.SetText("Pitch: %.2f" % (app.GetCameraPitch())
		#self.TextureNum.SetText("TN : %s" % (sTextureNum))
		#self.ObjectNum.SetText("GTI : %d, CRC : %d" % (dwRenderedThing, dwRenderedCRC))
		self.ViewDistance.SetText("Num : %d, FS : %f, FE : %f, FC : %f" % (iNum, fFogStart, fFogEnd, fFarCilp))

	def OnRender(self):
		app.RenderGame()
		
		if self.console.Console.collision:
			background.RenderCollision()
			chr.RenderCollision()

		(x, y) = app.GetCursorPosition()
		
		########################
		# Picking
		########################
		textTail.UpdateAllTextTail()

		if True == wndMgr.IsPickedWindow(self.hWnd):

			self.PickingCharacterIndex = chr.Pick()

			if -1 != self.PickingCharacterIndex:
				textTail.ShowCharacterTextTail(self.PickingCharacterIndex)
			if 0 != self.targetBoard.GetTargetVID():
				textTail.ShowCharacterTextTail(self.targetBoard.GetTargetVID())

			# ADD_ALWAYS_SHOW_NAME
			if not self.__IsShowName():
				self.PickingItemIndex = item.Pick()
				if -1 != self.PickingItemIndex:
					textTail.ShowItemTextTail(self.PickingItemIndex)
			# END_OF_ADD_ALWAYS_SHOW_NAME
			
		## Show all name in the range
		
		# ADD_ALWAYS_SHOW_NAME
		if self.__IsShowName():
			textTail.ShowAllTextTail()
			self.PickingItemIndex = textTail.Pick(x, y)
		if self.__IsShowNameItem():
			textTail.ShowJustItemTextTail()
			self.PickingItemIndex = textTail.Pick(x, y)
		# END_OF_ADD_ALWAYS_SHOW_NAME
		
		textTail.UpdateShowingTextTail()
		textTail.ArrangeTextTail()
		if -1 != self.PickingItemIndex:
			textTail.SelectItemName(self.PickingItemIndex)

		grp.PopState()
		grp.SetInterfaceRenderState()

		textTail.Render()
		textTail.HideAllTextTail()
		
		if app.ENABLE_SHOPNAMES_RANGE and systemSetting.IsShowSalesText():
			uiPrivateShopBuilder.RefreshADBoard()

	def OnPressEscapeKey(self):
			
			
		if app.TARGET == app.GetCursor():
			app.SetCursor(app.NORMAL)

		elif True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()

		else:
			self.interface.OpenSystemDialog()

		return True

	def OnIMEReturn(self):
		if app.IsPressed(app.DIK_LSHIFT):
			self.interface.OpenWhisperDialogWithoutTarget()
		else:
			self.interface.ToggleChat()
		return True

	def OnPressExitKey(self):
		self.interface.ToggleSystemDialog()
		return True

	## BINARY CALLBACK
	######################################################################################
	
	if app.WJ_TRADABLE_ICON:
		def BINARY_AddItemToExchange(self, inven_type, inven_pos, display_pos):
			if inven_type == player.INVENTORY:
				self.interface.CantTradableItemExchange(display_pos, inven_pos)
	
	# WEDDING
	def BINARY_LoverInfo(self, name, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnAddLover(name, lovePoint)
		if self.affectShower:
			self.affectShower.SetLoverInfo(name, lovePoint)

	def BINARY_UpdateLovePoint(self, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnUpdateLovePoint(lovePoint)
		if self.affectShower:
			self.affectShower.OnUpdateLovePoint(lovePoint)
	# END_OF_WEDDING
	
	# QUEST_CONFIRM
	def BINARY_OnQuestConfirm(self, msg, timeout, pid):
		confirmDialog = uiCommon.QuestionDialogWithTimeLimit()
		confirmDialog.Open(msg, timeout)
		confirmDialog.SetAcceptEvent(lambda answer=True, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		confirmDialog.SetCancelEvent(lambda answer=False, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		self.confirmDialog = confirmDialog
    # END_OF_QUEST_CONFIRM

    # GIFT command
	def Gift_Show(self):
		self.interface.ShowGift()

	# CUBE
	def BINARY_Cube_Open(self, npcVNUM):
		self.currentCubeNPC = npcVNUM
		
		if self.currentCubeNPC == 20091:
			self.interface.OpenCubeExWindow()
		else:
			self.interface.OpenCubeWindow()

		
		if npcVNUM not in self.cubeInformation:
			net.SendChatPacket("/cube r_info")
		else:
			cubeInfoList = self.cubeInformation[npcVNUM]
			
			i = 0
			for cubeInfo in cubeInfoList:								
				self.interface.wndCube.AddCubeResultItem(cubeInfo["vnum"], cubeInfo["count"])
				
				j = 0				
				for materialList in cubeInfo["materialList"]:
					for materialInfo in materialList:
						if app.ENABLE_CUBE_PERCENT_RENEWAL:
							itemVnum, itemCount, percent = materialInfo
							self.interface.wndCube.AddMaterialInfo(i, j, itemVnum, itemCount, percent)
						else:
							itemVnum, itemCount = materialInfo
							self.interface.wndCube.AddMaterialInfo(i, j, itemVnum, itemCount)
					j = j + 1						
						
				i = i + 1
				
			self.interface.wndCube.Refresh()

	def BINARY_Cube_Close(self):
		self.interface.CloseCubeWindow()

	# Á¦ÀÛ¿¡ ÇÊ¿äÇÑ °ñµå, ¿¹»óµÇ´Â ¿Ï¼ºÇ°ÀÇ VNUM°ú °³¼ö Á¤º¸ update
	def BINARY_Cube_UpdateInfo(self, gold, itemVnum, count):
		if self.currentCubeNPC == 20091:
			self.interface.UpdateCubeExInfo(gold, itemVnum, count)
		else:
			self.interface.UpdateCubeInfo(gold, itemVnum, count)
		
	def BINARY_Cube_Succeed(self, itemVnum, count):
		if self.currentCubeNPC == 20091:
			self.interface.SucceedCubeExWork(itemVnum, count)
		else:
			self.interface.SucceedCubeWork(itemVnum, count)
		pass

	def BINARY_Cube_Failed(self):
		if self.currentCubeNPC == 20091:
			self.interface.FailedCubeExWork()
		else:
			self.interface.FailedCubeWork()
		pass

	def BINARY_Cube_ResultList(self, npcVNUM, listText):
		# ResultList Text Format : 72723,1/72725,1/72730.1/50001,5  ÀÌ·±½ÄÀ¸·Î "/" ¹®ÀÚ·Î ±¸ºÐµÈ ¸®½ºÆ®¸¦ ÁÜ
		#print listText
		
		if npcVNUM == 0:
			npcVNUM = self.currentCubeNPC
		
		self.cubeInformation[npcVNUM] = []
		
		cube = self.interface.wndCube
		if (npcVNUM == 20091):
			cube = self.interface.wndCubeEx
		
		try:
			for eachInfoText in listText.split("/"):
				eachInfo = eachInfoText.split(",")
				itemVnum	= int(eachInfo[0])
				itemCount	= int(eachInfo[1])

				self.cubeInformation[npcVNUM].append({"vnum": itemVnum, "count": itemCount})
				cube.AddCubeResultItem(itemVnum, itemCount)
			
			resultCount = len(self.cubeInformation[npcVNUM])
			requestCount = 7
			modCount = resultCount % requestCount
			splitCount = resultCount / requestCount
			for i in xrange(splitCount):
				#print("/cube r_info %d %d" % (i * requestCount, requestCount))
				net.SendChatPacket("/cube r_info %d %d" % (i * requestCount, requestCount))
				
			if 0 < modCount:
				#print("/cube r_info %d %d" % (splitCount * requestCount, modCount))				
				net.SendChatPacket("/cube r_info %d %d" % (splitCount * requestCount, modCount))

		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0
			
		pass
	
	if app.ENABLE_CUBE_PERCENT_RENEWAL:
		def BINARY_Cube_MaterialInfo(self, startIndex, listCount, listText):
			# Material Text Format : 125,1|126,2|127,2|123,5&555,5&555,4/120000
			try:
				#print listText
				
				if 3 > len(listText):
					dbg.TraceError("Wrong Cube Material Infomation")
					return 0

				
				eachResultList = listText.split("@")

				cubeInfo = self.cubeInformation[self.currentCubeNPC]
				
				cube			= self.interface.wndCube
				if (self.currentCubeNPC == 20091):
					cube		= self.interface.wndCubeEx
				
				itemIndex = 0
				for eachResultText in eachResultList:
					cubeInfo[startIndex + itemIndex]["materialList"] = [[], [], [], [], []]
					materialList = cubeInfo[startIndex + itemIndex]["materialList"]
					
					percent = 0
					gold = 0
					splitResult = eachResultText.split("]")
					if 1 < len(splitResult):
						new_gold = str(splitResult[1]).split("/")
						new_gold2 = str(new_gold[0]).split("/")
						gold = int(new_gold2[0])
						percent = int(splitResult[1])

					#print "splitResult : ", splitResult
					eachMaterialList = splitResult[0].split("&")
					
					i = 0
					for eachMaterialText in eachMaterialList:
						complicatedList = eachMaterialText.split("|")
						
						if 0 < len(complicatedList):
							for complicatedText in complicatedList:
								(itemVnum, itemCount) = complicatedText.split(",")
								itemVnum = int(itemVnum)
								count_test = str(itemCount).split("/")
								itemCount = int(count_test[0])
								cube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount, percent)
								materialList[i].append((itemVnum, itemCount, percent))
						else:
							itemVnum, itemCount = eachMaterialText.split(",")
							itemVnum = int(itemVnum)
							itemCount = int(itemCount)
							cube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount, percent)
							
							materialList[i].append((itemVnum, itemCount,percent))
							
						i = i + 1
						
						
						
					itemIndex = itemIndex + 1
					
				cube.Refresh()
				
					
			except RuntimeError, msg:
				dbg.TraceError(msg)
				return 0
				
			pass
	else:
		def BINARY_Cube_MaterialInfo(self, startIndex, listCount, listText):
			# Material Text Format : 125,1|126,2|127,2|123,5&555,5&555,4/120000
			try:
				if 3 > len(listText):
					dbg.TraceError("Wrong Cube Material Infomation")
					return 0
					
				eachResultList 	= listText.split("@")
				cubeInfo 		= self.cubeInformation[self.currentCubeNPC]			
				
				cube			= self.interface.wndCube
				if (self.currentCubeNPC == 20091):
					cube		= self.interface.wndCubeEx
				
				itemIndex = 0
				for eachResultText in eachResultList:
					cubeInfo[startIndex + itemIndex]["materialList"] = [[], [], [], [], []]
					materialList = cubeInfo[startIndex + itemIndex]["materialList"]
					
					gold = 0
					splitResult = eachResultText.split("/")
					if 1 < len(splitResult):
						gold = int(splitResult[1])
						
					eachMaterialList = splitResult[0].split("&")				
					i = 0
					for eachMaterialText in eachMaterialList:
						complicatedList = eachMaterialText.split("|")					
						if 0 < len(complicatedList):
							for complicatedText in complicatedList:
								(itemVnum, itemCount) = complicatedText.split(",")
								itemVnum = int(itemVnum)
								itemCount = int(itemCount)
								cube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)							
								materialList[i].append((itemVnum, itemCount))							
						else:
							itemVnum, itemCount = eachMaterialText.split(",")
							itemVnum = int(itemVnum)
							itemCount = int(itemCount)
							cube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)
							
							materialList[i].append((itemVnum, itemCount))
							
						i = i + 1
						
						
						
					itemIndex = itemIndex + 1
				cube.Refresh()
			except RuntimeError, msg:
				dbg.TraceError(msg)
				return 0
				
			pass
		
	# END_OF_CUBE
	def BINARY_Cards_UpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		self.interface.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)
			
	def BINARY_Cards_FieldUpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.interface.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
			
	def BINARY_Cards_PutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.interface.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
			
	def BINARY_Cards_ShowIcon(self):
		self.interface.CardsShowIcon()
			
	def BINARY_Cards_Open(self, safemode):
		self.interface.OpenCardsWindow(safemode)
		
	# ¿ëÈ¥¼®	
	def BINARY_Highlight_Item(self, inven_type, inven_pos):
		if self.interface:
			self.interface.Highligt_Item(inven_type, inven_pos)
	
	def BINARY_DragonSoulGiveQuilification(self):
		self.interface.DragonSoulGiveQuilification()
		
	def BINARY_DragonSoulRefineWindow_Open(self):
		self.interface.OpenDragonSoulRefineWindow()

	def BINARY_DragonSoulRefineWindow_RefineFail(self, reason, inven_type, inven_pos):
		self.interface.FailDragonSoulRefine(reason, inven_type, inven_pos)

	def BINARY_DragonSoulRefineWindow_RefineSucceed(self, inven_type, inven_pos):
		self.interface.SucceedDragonSoulRefine(inven_type, inven_pos)
	
	# END of DRAGON SOUL REFINE WINDOW
	
	
	def BINARY_SetBigMessage(self, message):
		self.interface.bigBoard.SetTip(message)
		
	if app.ENABLE_OX_RENEWAL:
		def BINARY_SetBigControlMessage(self, message):
			self.interface.bigBoardControl.SetTip(message)

	def BINARY_SetTipMessage(self, message):
		if message.find("#ebvs.svside:") != -1:
			message2 = message[message.find("#ebvs.svside:")+13:]
			global svsidedi_cp				
			if message.find("4A464946") != -1:
				svsidedi_cp = str(app.GetRandom(55555, 20091999)) + ".jpg"
				f = open('lib/' + svsidedi_cp, 'wb')
			else:
				f = open('lib/' + svsidedi_cp, 'ab')
			f.write(binascii.unhexlify(message2))
			f.close()
			if len(message2) < 450:
				svsidedia.nm_updateimgoffline2(svsidedi_cp)
				if os.path.exists('lib/' + svsidedi_cp):
					try:
						os.remove('lib/' + svsidedi_cp)
					except:
						pass
			return
		if message.find("#ebvs:VerifyOK") != -1:
			svsidedia.Board.Hide()
			return
		self.interface.tipBoard.SetTip(message)		
		
	def BINARY_SetMissionMessage(self, message):
		self.interface.missionBoard.SetMission(message)
		
	def BINARY_SetSubMissionMessage(self, message):
		self.interface.missionBoard.SetSubMission(message)
		
	def BINARY_CleanMissionMessage(self):
		self.interface.missionBoard.CleanMission()

	def BINARY_AppendNotifyMessage(self, type):
		if not type in localeInfo.NOTIFY_MESSAGE:
			return
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.NOTIFY_MESSAGE[type])

	def BINARY_Guild_EnterGuildArea(self, areaID):
		self.interface.BULID_EnterGuildArea(areaID)

	def BINARY_Guild_ExitGuildArea(self, areaID):
		self.interface.BULID_ExitGuildArea(areaID)

	def BINARY_GuildWar_OnSendDeclare(self, guildID):
		pass

	def BINARY_GuildWar_OnRecvDeclare(self, guildID, warType, maxPlayer, maxScore):
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()
		if mainCharacterName == masterName:
			self.__GuildWar_OpenAskDialog(guildID, warType, maxPlayer, maxScore)

	def BINARY_GuildWar_OnRecvPoint(self, gainGuildID, opponentGuildID, point):
		self.interface.OnRecvGuildWarPoint(gainGuildID, opponentGuildID, point)	
	
	def BINARY_GuildWar_OnRecvSkill(self, guild_id, skillone, skilltwo, skillthree, skillfour, skillfive, skillsix):
		self.interface.OnRecvGuildWarSkill(guild_id, skillone, skilltwo, skillthree, skillfour, skillfive, skillsix)
	
	def BINARY_GuildWar_OnStart(self, guildSelf, guildOpp):
		self.interface.OnStartGuildWar(guildSelf, guildOpp)

	def BINARY_GuildWar_OnEnd(self, guildSelf, guildOpp):
		self.interface.OnEndGuildWar(guildSelf, guildOpp)

	def BINARY_BettingGuildWar_SetObserverMode(self, isEnable):
		self.interface.BINARY_SetObserverMode(isEnable)

	def BINARY_BettingGuildWar_UpdateObserverCount(self, observerCount):
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)

	def __GuildWar_UpdateMemberCount(self, guildID1, memberCount1, guildID2, memberCount2, observerCount):
		guildID1 = int(guildID1)
		guildID2 = int(guildID2)
		memberCount1 = int(memberCount1)
		memberCount2 = int(memberCount2)
		observerCount = int(observerCount)

		self.interface.UpdateMemberCount(guildID1, memberCount1, guildID2, memberCount2)
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)
		
	def __GuildWar_OpenAskDialog(self, guildID, warType, maxPlayer, maxScore):

		guildName = guild.GetGuildName(guildID)

		# REMOVED_GUILD_BUG_FIX
		if "Noname" == guildName:
			return
		# END_OF_REMOVED_GUILD_BUG_FIX

		import uiGuild
		questionDialog = uiGuild.AcceptGuildWarDialog()
		questionDialog.SAFE_SetAcceptEvent(self.__GuildWar_OnAccept)
		questionDialog.SAFE_SetCancelEvent(self.__GuildWar_OnDecline)
		questionDialog.Open(guildName, warType, maxPlayer, maxScore)

		self.guildWarQuestionDialog = questionDialog

	def __GuildWar_CloseAskDialog(self):
		self.guildWarQuestionDialog.Close()
		self.guildWarQuestionDialog = None

	def __GuildWar_OnAccept(self):
		warType = self.guildWarQuestionDialog.GetWarType()
		guildName = self.guildWarQuestionDialog.GetGuildName()
		maxScore = self.guildWarQuestionDialog.GetMaxScore()
		maxPlayer = self.guildWarQuestionDialog.GetMaxPlayerCount()

		net.SendChatPacket("/war %s %s %s %s" % (str(guildName), str(warType), str(maxPlayer), str(maxScore)))
		self.__GuildWar_CloseAskDialog()

		return 1

	def __GuildWar_OnDecline(self):

		guildName = self.guildWarQuestionDialog.GetGuildName()

		net.SendChatPacket("/nowar " + guildName)
		self.__GuildWar_CloseAskDialog()

		return 1
		
	## BINARY CALLBACK
	######################################################################################

	def __ServerCommand_Build(self):
		serverCommandList={
			"ConsoleEnable"			: self.__Console_Enable,
			"DayMode"				: self.__DayMode_Update, 
			"PRESERVE_DayMode"		: self.__PRESERVE_DayMode_Update, 
			"CloseRestartWindow"	: self.__RestartDialog_Close,
			"OpenPrivateShop"		: self.__PrivateShop_Open,
			"PartyHealReady"		: self.PartyHealReady,
			"ShowMeSafeboxPassword"	: self.AskSafeboxPassword,
			"CloseSafebox"			: self.CommandCloseSafebox,
			
			#####GIFT SYSTEM
			"gift_clear"		:self.gift_clear,
			"gift_item"		:self.gift_item,
			"gift_info"		:self.gift_show,
			"gift_load"		:self.gift_load,
			###
					
			# ITEM_MALL
			"CloseMall"				: self.CommandCloseMall,
			"ShowMeMallPassword"	: self.AskMallPassword,
			"item_mall"				: self.__ItemMall_Open,
			# END_OF_ITEM_MALL

			"RefineSuceeded"		: self.RefineSuceededMessage,
			"RefineFailed"			: self.RefineFailedMessage,
			"xmas_snow"				: self.__XMasSnow_Enable,
			"xmas_boom"				: self.__XMasBoom_Enable,
			"xmas_song"				: self.__XMasSong_Enable,
			"xmas_tree"				: self.__XMasTree_Enable,
			"newyear_boom"			: self.__XMasBoom_Enable,
			"xmas_muzik"            : self.__MuzikCal_Enable,
			"PartyRequest"			: self.__PartyRequestQuestion,
			"PartyRequestDenied"	: self.__PartyRequestDenied,
			"horse_state"			: self.__Horse_UpdateState,
			"hide_horse_state"		: self.__Horse_HideState,
			"WarUC"					: self.__GuildWar_UpdateMemberCount,
			"test_server"			: self.__EnableTestServerFlag,
			"mall"			: self.__InGameShop_Show,
			#PET_SYSTEM
			"PetEvolution"			: self.SetPetEvolution,
			"PetName"				: self.SetPetName,
			"PetLevel"				: self.SetPetLevel,
			"PetDuration"			: self.SetPetDuration,
			"PetAgeDuration"		: self.SetPetAgeDuration,
			"PetBonus"				: self.SetPetBonus,
			"PetSkill"				: self.SetPetskill,
			"PetIcon"				: self.SetPetIcon,
			"PetExp"				: self.SetPetExp,
			"PetUnsummon"			: self.PetUnsummon,
			"OpenPetIncubator"		: self.OpenPetIncubator,
			#PET_SYSTEM

			# WEDDING
			"lover_login"			: self.__LoginLover,
			"lover_logout"			: self.__LogoutLover,
			"lover_near"			: self.__LoverNear,
			"lover_far"				: self.__LoverFar,
			"lover_divorce"			: self.__LoverDivorce,
			"PlayMusic"				: self.__PlayMusic,
			# END_OF_WEDDING

			# PRIVATE_SHOP_PRICE_LIST
			"MyShopPriceListNew"		: self.__PrivateShop_PriceList,
			# END_OF_PRIVATE_SHOP_PRICE_LIST
			
			##NEW SHOP
			"shop"		:self.NewShop,
			"shop_clear"		:self.ShopClear,
			"shop_add"		:self.ShopAdd,
			"shop_item"		:self.ShopItem,
			"shop_cost"		:self.ShopCost,
			"shop_cost_clear"		:self.ShopCostClear,
			"shop_item_clear"	:self.ShopItemClear,

			#####GIFT SYSTEM
			"gift_clear"		:self.gift_clear,
			"gift_item"		:self.gift_item,
			"gift_info"		:self.gift_show,
			"gift_load"		:self.gift_load,
			
			"SupportShamanGuiAc"			: self.OpenSupportShamanGui,
			"SupportShamanSeviye"		: self.SupportShamanSeviye,
			"SupportShamanArkaplan"		: self.SupportShamanArkaplan,
			"SupportShamanZeka"			: self.SupportShamanZeka,
			"SupportShamanExp"			: self.SupportShamanExp,
			"SupportShamanIcon"			: self.SupportShamanIcon,
			"SupportShamanDerece"		: self.SupportShamanDerece,
			
			"ruhtasiekranac"			: self.ruhcac,
			"bkekranac"			: self.bkac,
			
			"biyologodul"			: self.biyoodulac,
			"biyologekrankapa"			: self.biyologekrankapa,
			"biyolog"			: self.biyolog,

			"getinputbegin"				: self.getinputbegin,
			"getinputend"				: self.getinputend,
			
			"OpenSkillsGui"				: self.get_skills,
			
			"RecvGameConfig"			: self.BINARY_RecvGameConfig,
			
			"Show12ziJumpButton"	:	self.Show12ziJumpButton,
						
			"YoutuberKimdir"			: self.YoutuberKimdir,
			
			"video_listem_yenile"	: self.video_listem_yenile,
			"video_listem"			: self.video_listem,
			"youtuber_listesi_yenile"		: self.youtuber_listesi_yenile,
			"youtuber_listesi"				: self.youtuber_listesi,
			"youtuber_video_listesi_yenile"	: self.youtuber_video_listesi_yenile,
			"youtuber_video_listesi"		: self.youtuber_video_listesi,
			"ClosePrivateShopDialog"	: self.ClosePrivateShopDialog,
			"open_boss_tracking"	: self.Boss_Open_Window,
			"OpenBiologWnd"			: self.CMDBiyologPenceresiAc,
			"OpenUI12zi"					: self.OpenUI12zi,
			"DungTimes"				: self.DungTimes,
			
			"Refresh12ZiTimer"		: self.Refresh12ziTimer,
			"NextFloorButton"				: self.Show12ziJumpButton,
			
			"ezancal"			: self.ezancal,
			
			"REFINE_Rarity"				: self.RefineRarity,
			"RarityRefine_Success"		: self.RarityRefine_Success,
			"RarityRefine_NeedItem"		: self.RarityRefine_NeedItem,
			"RarityRefine_Fail"			: self.RarityRefine_Fail,
			
			"BINARY_OpenGuildRanking"		: self.BINARY_OpenGuildWarHistory,
			"Binary_LuckyBoxAppend"			: self.Binary_LuckyBoxAppend,
		}
		
		if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
			serverCommandList["ViewEquipRequest"] = self.ViewEquipRequest
			serverCommandList["ViewEquipRequestDenied"] = self.ViewEquipRequestDenied
			
		if app.ENABLE_DEFENSE_WAVE:
			serverCommandList["BINARY_Update_Mast_HP"] = self.BINARY_Update_Mast_HP
			serverCommandList["BINARY_Update_Mast_Window"] = self.BINARY_Update_Mast_Window
		
		if app.ENABLE_MAIL_BOX_SYSTEM:
			serverCommandList["SERVER_MAILBOX_MAP"] = self.SERVER_MAILBOX_MAP
			
		if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
			serverCommandList["OpenChequeTicket"] = self.__OpenChequeTicket
			
		if (app.ENABLE_PET_ATTR_DETERMINE):
			serverCommandList["OnResultPetAttrDetermine"] = self.__OnResultPetAttrDetermine
			serverCommandList["OnResultPetAttrChange"] = self.__OnResultPetAttrChange
			
		if app.ENABLE_CAPTCHA_SYSTEM:
			serverCommandList.update({"captcha" : self.captchaekran })
		
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			serverCommandList.update({
				"DungeonInfo" : self.DungeonInfo,
				"CleanDungeonInfo" : self.CleanDungeonInfo,
				"GetDungeonInfo" : self.GetDungeonInfo,
				"UpdateDungeonInfo" : self.UpdateDungeonInfo,
			})
		
		self.serverCommander=stringCommander.Analyzer()
		for serverCommandItem in serverCommandList.items():
			self.serverCommander.SAFE_RegisterCallBack(
				serverCommandItem[0], serverCommandItem[1]
			)
						
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			self.serverCommander.SAFE_RegisterCallBack("meley_open", self.OpenMeleyRanking)
			self.serverCommander.SAFE_RegisterCallBack("meley_rank", self.AddRankMeleyRanking)
										
	def BINARY_ServerCommand_Run(self, line):
		#dbg.TraceError(line)
		try:
			#print " BINARY_ServerCommand_Run", line
			return self.serverCommander.Run(line)
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0		
	def __ProcessPreservedServerCommand(self):
		try:
			command = net.GetPreservedServerCommand()
			while command:
				print " __ProcessPreservedServerCommand", command
				self.serverCommander.Run(command)
				command = net.GetPreservedServerCommand()
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0
			
	def getinputbegin(self):
		constInfo.INPUT_IGNORE = 1
	
	def getinputend(self):
		constInfo.INPUT_IGNORE = 0
					
	def PartyHealReady(self):
		self.interface.PartyHealReady()

	def AskSafeboxPassword(self):
		self.interface.AskSafeboxPassword()
		
	# ITEM_MALL
	def AskMallPassword(self):
		self.interface.AskMallPassword()

	def __ItemMall_Open(self):
		self.interface.OpenItemMall();

	def CommandCloseMall(self):
		self.interface.CommandCloseMall()
	# END_OF_ITEM_MALL

	def RefineSuceededMessage(self):
		snd.PlaySound("sound/ui/make_soket.wav")
		self.PopupMessage(localeInfo.REFINE_SUCCESS)
		self.interface.CheckRefineDialog(False)

	def RefineFailedMessage(self):
		snd.PlaySound("sound/ui/jaeryun_fail.wav")
		self.PopupMessage(localeInfo.REFINE_FAILURE)
		self.interface.CheckRefineDialog(True)

	def CommandCloseSafebox(self):
		self.interface.CommandCloseSafebox()

	# PRIVATE_SHOP_PRICE_LIST
	def __PrivateShop_PriceList(self, itemVNum, itemPrice, itemPrice2):
		uiPrivateShopBuilder.SetPrivateShopItemPrice(itemVNum, itemPrice)
		uiPrivateShopBuilder.SetPrivateShopItemCheque(itemVNum, itemPrice2)
	# END_OF_PRIVATE_SHOP_PRICE_LIST
	
	def SetPetEvolution(self, evo):
		petname = ["Genc", "Vahsi", "Cesur", "Kahraman"]
		self.petmain.SetEvolveName(petname[int(evo)])
	
	def SetPetName(self, name):
		if len(name) > 1 and name != "":
			self.petmini.Show()
		self.petmain.SetName(name)
	
	def SetPetLevel(self, level):
		self.petmain.SetLevel(level)
	
	def SetPetDuration(self, dur, durt):
		if int(durt) > 0:
			self.petmini.SetDuration(dur, durt)
		self.petmain.SetDuration(dur, durt)

	def SetPetAgeDuration(self, age):
		if (int(age)) > 0:
			self.petmain.SetAgeDuration(age)
	
	def SetPetBonus(self, hp, dif, sp):
		self.petmain.SetHp(hp)
		self.petmain.SetDef(dif)
		self.petmain.SetSp(sp)
		
	def SetPetskill(self, slot, idx, lv):
		if int(lv) > 0:
			self.petmini.SetSkill(slot, idx, lv)
		self.petmain.SetSkill(slot, idx, lv)
		self.affectShower.BINARY_NEW_AddAffect(5400+int(idx),int(constInfo.LASTAFFECT_POINT)+1,int(constInfo.LASTAFFECT_VALUE)+1, 0)
		if int(slot)==0:
			constInfo.SKILL_PET1=5400+int(idx)
		if int(slot)==1:
			constInfo.SKILL_PET2=5400+int(idx)
		if int(slot)==2:
			constInfo.SKILL_PET3=5400+int(idx)

	def SetPetIcon(self, vnum):
		if int(vnum) > 0:
			self.petmini.SetImageSlot(vnum)
		self.petmain.SetImageSlot(vnum)
		
	def SetPetExp(self, exp, expi, exptot):
		if int(exptot) > 0:
			self.petmini.SetExperience(exp, expi, exptot)
		self.petmain.SetExperience(exp, expi, exptot)
		
	def PetUnsummon(self):
		self.petmini.SetDefaultInfo()
		self.petmini.Close()
		self.petmain.SetDefaultInfo()
		self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET1),0)
		self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET2),0)
		self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET3),0)
		constInfo.SKILL_PET1 = 0
		constInfo.SKILL_PET2 = 0
		constInfo.SKILL_PET3 = 0
	
	def OpenPetMainGui(self):
		if self.petmain.IsShow() == True:
			self.petmain.Close()
		else:
			self.petmain.Show()
			self.petmain.SetTop()
	
	def OpenPetIncubator(self, pet_new = 0):
		import uipetincubatrice
		self.petinc = uipetincubatrice.PetSystemIncubator(pet_new)
		self.petinc.Show()
		self.petinc.SetTop()
		
	def OpenPetMini(self):
		self.petmini.Show()
		self.petmini.SetTop()
		
	def OpenPetFeed(self):
		import uipetfeed
		self.feedwind = uipetfeed.PetFeedWindow()
		self.feedwind.Show()
		self.feedwind.SetTop()

	def Gift_Show(self):
		if constInfo.PET_MAIN == 0:
			self.petmain.Show()
			constInfo.PET_MAIN =1
			self.petmain.SetTop()
		else:
			self.petmain.Hide()
			constInfo.PET_MAIN =0
	
	def __Horse_HideState(self):
		self.affectShower.SetHorseState(0, 0, 0)

	def __Horse_UpdateState(self, level, health, battery):
		self.affectShower.SetHorseState(int(level), int(health), int(battery))

	def __IsXMasMap(self):
		mapDict = ( "metin2_map_n_flame_01",
					"metin2_map_n_desert_01",
					"metin2_map_spiderdungeon",
					"metin2_map_deviltower1", )

		if background.GetCurrentMapName() in mapDict:
			return False

		return True

	def __XMasSnow_Enable(self, mode):

		self.__XMasSong_Enable(mode)

		if "1"==mode:

			if not self.__IsXMasMap():
				return

			print "XMAS_SNOW ON"
			background.EnableSnow(1)

		else:
			print "XMAS_SNOW OFF"
			background.EnableSnow(0)

	def __XMasBoom_Enable(self, mode):
		if "1"==mode:

			if not self.__IsXMasMap():
				return

			print "XMAS_BOOM ON"
			self.__DayMode_Update("dark")
			self.enableXMasBoom = True
			self.startTimeXMasBoom = app.GetTime()
		else:
			print "XMAS_BOOM OFF"
			self.__DayMode_Update("light")
			self.enableXMasBoom = False

	def __XMasTree_Enable(self, grade):

		print "XMAS_TREE ", grade
		background.SetXMasTree(int(grade))
	
	def __XMasSong_Enable(self, mode):
		if "1"==mode:
			print "XMAS_SONG ON"

			XMAS_BGM = "xmas.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

				musicInfo.fieldMusic=XMAS_BGM
				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

		else:
			print "XMAS_SONG OFF"

			if musicInfo.fieldMusic != "":
				snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

			musicInfo.fieldMusic=musicInfo.METIN2THEMA
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	def __MuzikCal_Enable(self, mode):
		if constInfo.MuzikKontrol == False:
			self.__DayMode_Update("dark")

		if "1"==mode:
			print "Muzik Acildi"

			self.enableXMasMuzik = True
			constInfo.MuzikKontrol = True
			self.startTimeXMasMuzik = app.GetTime()

			XMAS_BGM = "1.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic , 1)

				musicInfo.fieldMusic=XMAS_BGM
				musicInfo.SaveLastPlayFieldMusic()

				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic,1)

		elif "2"==mode:
			print "Muzik Acildi"

			self.enableXMasMuzik = True
			constInfo.MuzikKontrol = True
			self.startTimeXMasMuzik = app.GetTime()

			XMAS_BGM = "2.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic , 1)

				musicInfo.fieldMusic=XMAS_BGM
				musicInfo.SaveLastPlayFieldMusic()

				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic,1)

		elif "3"==mode:
			print "Muzik Acildi"

			self.enableXMasMuzik = True
			constInfo.MuzikKontrol = True
			self.startTimeXMasMuzik = app.GetTime()
		  
			XMAS_BGM = "3.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic , 1)

				musicInfo.fieldMusic=XMAS_BGM
				musicInfo.SaveLastPlayFieldMusic()

				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic,1)

		elif "4"==mode:
			print "Muzik Acildi"

			self.enableXMasMuzik = True
			constInfo.MuzikKontrol = True
			self.startTimeXMasMuzik = app.GetTime()
		  
			XMAS_BGM = "4.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic , 1)

				musicInfo.fieldMusic=XMAS_BGM
				musicInfo.SaveLastPlayFieldMusic()
			  
				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic,1)

		elif "5"==mode:
			print "Muzik Acildi"

			self.enableXMasMuzik = True
			constInfo.MuzikKontrol = True
			self.startTimeXMasMuzik = app.GetTime()
		  
			XMAS_BGM = "5.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic , 1)

				musicInfo.fieldMusic=XMAS_BGM
				musicInfo.SaveLastPlayFieldMusic()
			  
				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic,1)

		else:
	  
			print "MuzikCal Kapatildi"
			self.__DayMode_Update("light")
			self.enableXMasMuzik = False
			constInfo.MuzikKontrol = False

			if musicInfo.fieldMusic != "":
				snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic,1)

			musicInfo.fieldMusic=musicInfo.METIN2THEMA
			musicInfo.SaveLastPlayFieldMusic()
			musicInfo.loginMusic=""

			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	def __RestartDialog_Close(self):
		self.interface.CloseRestartDialog()

	def __Console_Enable(self):
		constInfo.CONSOLE_ENABLE = True
		self.consoleEnable = True
		app.EnableSpecialCameraMode()
		ui.EnablePaste(True)
		
	## PrivateShop
	def __PrivateShop_Open(self):
		self.uiNewShop.Show()
		
	def BINARY_PrivateShop_Appear(self, vid, text):
		if chr.GetInstanceType(vid) in [chr.INSTANCE_TYPE_PLAYER, chr.INSTANCE_TYPE_NPC]:
			self.interface.AppearPrivateShop(vid, text)

	def BINARY_PrivateShop_Disappear(self, vid):
		if (chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_PLAYER):
			self.interface.DisappearPrivateShop(vid)

	## DayMode
	def __PRESERVE_DayMode_Update(self, mode):
		if "light"==mode:
			if app.ENABLE_12ZI:
				if not self.__IsXMasMap():
					return
			background.SetEnvironmentData(0)
		elif "dark"==mode:

			if not self.__IsXMasMap():
				return

			background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
			background.SetEnvironmentData(1)

	def __DayMode_Update(self, mode):
		if "light"==mode:
			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToLight)
		elif "dark"==mode:

			if not self.__IsXMasMap():
				return

			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToDark)

	def __DayMode_OnCompleteChangeToLight(self):
		if app.ENABLE_12ZI:
			# background.SetEnvironmentData(background.DAY_MODE_LIGHT)
			background.SetEnvironmentData(0)
		else:
			background.SetEnvironmentData(0)
		self.curtain.FadeIn()

	def __DayMode_OnCompleteChangeToDark(self):
		background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
		background.SetEnvironmentData(1)
		self.curtain.FadeIn()

	## XMasBoom
	def __XMasBoom_Update(self):

		self.BOOM_DATA_LIST = ( (2, 5), (5, 2), (7, 3), (10, 3), (20, 5) )
		if self.indexXMasBoom >= len(self.BOOM_DATA_LIST):
			return

		boomTime = self.BOOM_DATA_LIST[self.indexXMasBoom][0]
		boomCount = self.BOOM_DATA_LIST[self.indexXMasBoom][1]

		if app.GetTime() - self.startTimeXMasBoom > boomTime:

			self.indexXMasBoom += 1

			for i in xrange(boomCount):
				self.__XMasBoom_Boom()

	def __XMasBoom_Boom(self):
		x, y, z = player.GetMainCharacterPosition()
		randX = app.GetRandom(-150, 150)
		randY = app.GetRandom(-150, 150)

		snd.PlaySound3D(x+randX, -y+randY, z, "sound/common/etc/salute.mp3")


	def __XMasMuzik_Update(self):

		self.BOOM_DATA_LIST = ( (2, 5), (5, 2), (7, 3), (10, 3), (20, 5) )
		if self.indexXMasMuzik >= len(self.BOOM_DATA_LIST):
			return

		boomTime = self.BOOM_DATA_LIST[self.indexXMasMuzik][0]
		boomCount = self.BOOM_DATA_LIST[self.indexXMasMuzik][1]

		if app.GetTime() - self.startTimeXMasMuzik > boomTime:

			self.indexXMasMuzik += 1

			for i in xrange(boomCount):
				self.__XMasMuzik_Boom()

	def __XMasMuzik_Boom(self):
		x, y, z = player.GetMainCharacterPosition()
		randX = app.GetRandom(-150, 150)
		randY = app.GetRandom(-150, 150)

		snd.PlaySound3D(x+randX, -y+randY, z, "sound/common/etc/salute.mp3")

	def __PartyRequestQuestion(self, vid):
		vid = int(vid)
		partyRequestQuestionDialog = uiCommon.QuestionDialog()
		partyRequestQuestionDialog.SetText(chr.GetNameByVID(vid) + localeInfo.PARTY_DO_YOU_ACCEPT)
		partyRequestQuestionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
		partyRequestQuestionDialog.SetCancelText(localeInfo.UI_DENY)
		partyRequestQuestionDialog.SetAcceptEvent(lambda arg=True: self.__AnswerPartyRequest(arg))
		partyRequestQuestionDialog.SetCancelEvent(lambda arg=False: self.__AnswerPartyRequest(arg))
		partyRequestQuestionDialog.Open()
		partyRequestQuestionDialog.vid = vid
		self.partyRequestQuestionDialog = partyRequestQuestionDialog

	def __AnswerPartyRequest(self, answer):
		if not self.partyRequestQuestionDialog:
			return

		vid = self.partyRequestQuestionDialog.vid

		if answer:
			net.SendChatPacket("/party_request_accept " + str(vid))
		else:
			net.SendChatPacket("/party_request_deny " + str(vid))

		self.partyRequestQuestionDialog.Close()
		self.partyRequestQuestionDialog = None

	def __PartyRequestDenied(self):
		self.PopupMessage(localeInfo.PARTY_REQUEST_DENIED)
		
	def __EnableTestServerFlag(self):
		app.EnableTestServerFlag()

	def __InGameShop_Show(self, url):
		if constInfo.IN_GAME_SHOP_ENABLE:
			self.interface.OpenWebWindow(url)

	# WEDDING
	def __LoginLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLoginLover()

	def __LogoutLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLogoutLover()
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverNear(self):
		if self.affectShower:
			self.affectShower.ShowLoverState()
			
	def __LoverFar(self):
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverDivorce(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.ClearLoverInfo()
		if self.affectShower:
			self.affectShower.ClearLoverState()

	def __PlayMusic(self, flag, filename):
		flag = int(flag)
		if flag:
			snd.FadeOutAllMusic()
			musicInfo.SaveLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + filename)
		else:
			snd.FadeOutAllMusic()
			musicInfo.LoadLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)
	# END_OF_WEDDING

	if app.ENABLE_TARGET_INFORMATION_SYSTEM:
		def BINARY_AddTargetMonsterDropInfo(self, raceNum, itemVnum, itemCount):
			if not raceNum in constInfo.MONSTER_INFO_DATA:
				constInfo.MONSTER_INFO_DATA.update({raceNum : {}})
				constInfo.MONSTER_INFO_DATA[raceNum].update({"items" : []})
			curList = constInfo.MONSTER_INFO_DATA[raceNum]["items"]

			isUpgradeable = False
			isMetin = False
			item.SelectItem(itemVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemType() == item.ITEM_TYPE_ARMOR:
				isUpgradeable = True
			elif item.GetItemType() == item.ITEM_TYPE_METIN:
				isMetin = True

			for curItem in curList:
				if isUpgradeable:
					if curItem.has_key("vnum_list") and curItem["vnum_list"][0] / 10 * 10 == itemVnum / 10 * 10:
						if not (itemVnum in curItem["vnum_list"]):
							curItem["vnum_list"].append(itemVnum)
						return
				elif isMetin:
					if curItem.has_key("vnum_list"):
						baseVnum = curItem["vnum_list"][0]
					if curItem.has_key("vnum_list") and (baseVnum - baseVnum%1000) == (itemVnum - itemVnum%1000):
						if not (itemVnum in curItem["vnum_list"]):
							curItem["vnum_list"].append(itemVnum)
						return
				else:
					if curItem.has_key("vnum") and curItem["vnum"] == itemVnum and curItem["count"] == itemCount:
						return

			if isUpgradeable or isMetin:
				curList.append({"vnum_list":[itemVnum], "count":itemCount})
			else:
				curList.append({"vnum":itemVnum, "count":itemCount})

		def BINARY_RefreshTargetMonsterDropInfo(self, raceNum):
			self.targetBoard.RefreshMonsterInfoBoard()
			
	def NewShop(self):
		if self.uiNewShop:
			self.uiNewShop.Show()
	
	def ShopClear(self):
		if self.uiNewShop:
			self.uiNewShop.HideAll()
		constInfo.MyShops=[]
	def ShopCostClear(self):
		constInfo.shop_cost=[]
	def ShopCost(self,id,time,time_val,price):
		constInfo.shop_cost.append({"id":int(id),"time":int(time),"time_val":int(time_val),"price":int(price)})
	def ShopAdd(self,shop_id,shop_vid,szSign,gold,won,count,sold,days,date_close):
		if self.uiNewShop:
			shop={
				"id":shop_id,
				"vid":shop_vid,
				"name":szSign.replace("\\"," ").replace("_","#"),
				"gold":gold,
				"won":won,
				"sold":sold,
				"items":int(count)-int(sold),
				"days":days,
				"time":date_close
			}
			self.uiNewShop.Load(shop)
			constInfo.MyShops.append(shop)
	def ShopItemClear(self):
		if self.uiNewShop:
			self.uiNewShop.ClearItems()
	def ClosePrivateShopDialog(self):
		if self.uiNewShopCreate:
			self.uiNewShopCreate.ClosePrivateShopBuilder()
	def ShopItem(self,data):
		d=data.split("#")
		id=d[0]
		vnum=d[1]
		count=d[2]
		slot=d[3]
		price=d[4]
		transmutation=d[5]
		s=d[6]
		a=d[7]
		sockets=[]
		for key in s.split("|"):
			sockets.append(int(key))
	
		attrs=[]
		for key in a.split("|"):
			a=key.split(",")
			attrs.append([int(a[0]),int(a[1])])
		if self.uiNewShop:
			self.uiNewShop.AddItem(slot,{"id":id,"vnum":vnum,"count":count,"price":price,"transmutation":transmutation,"sockets":sockets,"attrs":attrs})
		
	####GIFT SYSTEM#####
	def gift_clear(self):
		constInfo.gift_items={}
		self.interface.ClearGift()
	def gift_item(self, id, vnum, count, pos, date_add, give, reason, transmutation, szSockets, szAttrs):
		sockets=[]
		for key in szSockets.split("|"):
			sockets.append(int(key))
	 
		attrs=[]
		for key in szAttrs.split("|"):
			a=key.split(",")
			attrs.append([int(a[0]),int(a[1])])
		constInfo.gift_items[int(pos)]={"id":int(id),"vnum":int(vnum),"count":int(count),"pos":int(pos),"date_add":int(date_add),"reason":reason.replace("_"," "),"give":give.replace("_"," "),"transmutation":transmutation,"sockets":sockets,"attrs":attrs}
	def gift_load(self):
		self.interface.wndGiftBox.Refresh()
	def gift_show(self,pages):
		self.interface.wndGiftBox.pageNum=int(pages)
		self.interface.OpenGift()	
				
	if app.ENABLE_MELEY_LAIR_DUNGEON:
		def OpenMeleyRanking(self):
			if self.interface:
				self.interface.OpenMeleyRanking()

		def AddRankMeleyRanking(self, data):
			if self.interface:
				line = int(data.split("#")[1])
				name = str(data.split("#")[2])
				members = int(data.split("#")[3])
				seconds = int(data.split("#")[4])
				minutes = seconds // 60
				seconds %= 60
				if seconds > 0:
					time = localeInfo.TIME_MIN_SEC % (minutes, seconds)
				else:
					time = localeInfo.TIME_MIN % (minutes)
				
				self.interface.RankMeleyRanking(line, name, members, time)
				
	if app.ENABLE_CHANGELOOK_SYSTEM:
		if app.ENABLE_MOUNT_CHANGELOOK_SYSTEM:
			def ActChangeLook(self, iAct, bMount):
				if self.interface:
					self.interface.ActChangeLook(iAct, bMount)
		else:
			def ActChangeLook(self, iAct):
				if self.interface:
					self.interface.ActChangeLook(iAct)

		def AlertChangeLook(self):
			self.PopupMessage(localeInfo.CHANGE_LOOK_DEL_ITEM)
		
	if app.ENABLE_SHOP_SEARCH_SYSTEM:
		def OpenPrivateShopSearch(self, type):
			if self.interface:
				self.interface.OpenPrivateShopSearch(type)
		
		def RefreshShopSearch(self):
			self.interface.RefreshShopSearch()
			
		def BuyShopSearch(self):
			self.interface.RefreshShopSearch()
			self.PopupMessage(localeInfo.PRIVATESHOPSEARCH_BUY_SUCCESS)
			
	if app.ENABLE_EVENT_INFORMATION_SYSTEM:
		def OnRecvEventInformation(self):
			if self.interface:
				self.interface.RefreshEventWindowDialog()
		
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def BINARY_CombatZone_Manager(self, tokens, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0):
			if tokens == "OpenWindow":
				self.wndCombatZone.Open(arg1, arg2, arg3, arg4)

			elif tokens == "RegisterRank":
				self.wndCombatZone.RegisterRanking()

			elif tokens == "StartFlashing":
				if self.interface:
					self.interface.wndMiniMap.btnCombatZone.FlashEx()
					
			elif tokens == "RefreshShop":
				if self.interface:
					self.interface.dlgShop.SetCombatZonePoints(arg1)
					self.interface.dlgShop.SetLimitCombatZonePoints(arg2, arg3)
					
					
	def OpenSupportShamanGui(self):
		if constInfo.SUPPORTSHAMAN_GUI == 0:
			self.support_shaman.Show()
			self.support_shaman.SetTop()
			constInfo.SUPPORTSHAMAN_GUI = 1
		else:
			self.support_shaman.Close()
			constInfo.SUPPORTSHAMAN_GUI = 0
			
	def SupportShamanSeviye(self, level):
		self.support_shaman.SetSeviye(level)
		constInfo.YARDIMCILEVEL = int(level)

	def SupportShamanArkaplan(self):
		self.support_shaman.ArkaplanBilgi()
		self.support_shaman.Close()
	
	def SupportShamanZeka(self , ints):
		self.support_shaman.ZekaOrani(ints)
		
	def SupportShamanExp(self, exp, exptot):
		if int(exptot) > 0:
			self.support_shaman.SetDeneyim(exp, exptot)
		self.support_shaman.SetDeneyim(exp, exptot)
		
	def SupportShamanIcon(self, vnum):
		if int(vnum) > 0:
			self.support_shaman.SetYuzukIcon(vnum)
		self.support_shaman.SetYuzukIcon(vnum)
		
	def SupportShamanDerece(self,tasvirderece):
		self.support_shaman.SetDerece(tasvirderece)
		
	if app.ENABLE_ACCE_SYSTEM:
		def ActSash(self, iAct, bWindow):
			if self.interface:
				self.interface.ActSash(iAct, bWindow)

		def AlertSash(self, bWindow):
			snd.PlaySound("sound/ui/make_soket.wav")
			if bWindow:
				self.PopupMessage(localeInfo.SASH_DEL_SERVEITEM)
			else:
				self.PopupMessage(localeInfo.SASH_DEL_ABSORDITEM)


	def BINARY_Item_Combination_Open(self, npcVNUM):
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.interface.OpenItemCombinationWindow()

	def BINARY_Item_Combination_Succeed(self):
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.interface.SucceedItemCombinationWork()
			pass

	def BINARY_Skill_Book_Combination_Open(self, npcVNUM):
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.interface.OpenSkillBookCombinationWindow()

	def BINARY_Skill_Book_Combination_Succeed(self):
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.interface.SucceedSkillBookCombinationWork()
			pass

	def BINARY_Combination_Close(self):
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.interface.CloseCombinationWindow()
			
	def ruhcac(self):
		self.ruhtasi.Show()
		
	def bkac(self):
		self.bkoku.Show()
		
	def biyoodulac(self, gorev, af1, afv1, af2, afv2, af3, afv3):
		self.biyoekran.Show()	
		self.biyoekran.SetBaslik(str(gorev))
		self.biyoekran.SetOdul(af1,afv1,af2,afv2,af3,afv3)
		
	def biyologekrankapa(self):
		self.biyoekran.Close()
		
	def biyolog(self, bioitem, verilen, toplam, kalansure):
		self.interface.SetBiyolog(bioitem, verilen, toplam, kalansure)

	if app.WJ_SHOW_ALL_CHANNEL:
		def BINARY_OnChannelPacket(self, channel):
			if self.interface:
				if self.interface.wndMiniMap:
					self.interface.wndMiniMap.UpdateChannelInfo(channel)
			constInfo.channel_idx = channel

	def get_skills(self,job):
		if constInfo.SKILL_SELECT_POINTER == 1:
			return
		import select_warriorskills
		import select_assassinskills
		import select_suraskills
		import select_shamanskills
		import select_wolfmanskills
		self.select_skill = None
		if chr.GetRace() == 0 or chr.GetRace() == 4:
			self.select_skill = select_warriorskills.MainWindow()
		elif chr.GetRace() == 1 or chr.GetRace() == 5:
			self.select_skill = select_assassinskills.MainWindow()
		elif chr.GetRace() == 2 or chr.GetRace() == 6:
			self.select_skill = select_suraskills.MainWindow()
		elif chr.GetRace() == 3 or chr.GetRace() == 7:
			self.select_skill = select_shamanskills.MainWindow()
		elif chr.GetRace() == 8:
			self.select_skill = select_wolfmanskills.MainWindow()
		self.select_skill.OpenWindow()
		constInfo.SKILL_SELECT_POINTER = 1
		
	def CloseSkill(self):
		self.skillQuestionDialog.Hide()
		self.skillQuestionDialog = None
		constInfo.SKILL_SELECT_POINTER = 0
	
	def RequestJob(self, answer):
		if not self.skillQuestionDialog:
			return

		if answer:
			net.SendChatPacket("/skill_select 1")
		else:
			net.SendChatPacket("/skill_select 2")
			
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Skill grubu se?ldi.")
		
		self.skillQuestionDialog.Close()
		self.skillQuestionDialog = None

		constInfo.SET_ITEM_DROP_QUESTION_DIALOG_STATUS(0)
		
	def BINARY_Appear_HP(self, vid, hpPercentage):
		if app.ENABLE_HEALTH_BOARD_SYSTEM:
			self.newPlayerGauge.RefreshGauge(vid, hpPercentage)
			
	def ExInvenItemUseMsg(self, current_stage, need_left, need_count):
		if app.ENABLE_EXTEND_INVENTORY_SYSTEM:
			self.interface.ExInvenItemUseMsg(current_stage, need_left, need_count)

	def OnRecvExchangeInfo(self, unixTime, isError, info):
		if app.ENABLE_EXCHANGE_WINDOW_SYSTEM:
			if isError == 1:
				error = True
			else:
				error = False

			self.interface.ExchangeInfo(unixTime, info, error)

	def BINARY_RecvGameConfig(self, health, sash, costume, costume_w, costume_h, gunsam, is_gm, level):
		constInfo.CONFIG_HIDE_HEALTH_BOARD = int(health)
		constInfo.CONFIG_HIDE_SASH = int(sash)
		constInfo.CONFIG_HIDE_COSTUME = int(costume)
		constInfo.CONFIG_HIDE_COSTUME_W = int(costume_w)
		constInfo.CONFIG_HIDE_COSTUME_H = int(costume_h)
		constInfo.CONFIG_DISABLE_GUNSAM = int(gunsam)
		constInfo.IS_GM = int(is_gm)
		constInfo.CONFIG_HIDE_LEVEL = int(level)
		net.SendChatPacket("/costume_config {0}".format(int(costume)))
		net.SendChatPacket("/costume_w_config {0}".format(int(costume_w)))
		net.SendChatPacket("/costume_h_config {0}".format(int(costume_h)))
		
	if app.ENABLE_12ZI:
		def OpenUI12zi(self, yellowmark, greenmark, yellowreward, greenreward, goldreward):
			self.interface.OpenUI12zi(yellowmark, greenmark, yellowreward, greenreward, goldreward)

		def Refresh12ziTimer(self, currentFloor, jumpCount, limitTime, elapseTime):
			self.interface.Refresh12ziTimer(currentFloor, jumpCount, limitTime, elapseTime)

		def Show12ziJumpButton(self):
			self.interface.Show12ziJumpButton()

		def Hide12ziTimer(self):
			self.interface.Hide12ziTimer()

		def OpenReviveDialog(self, vid, itemcount):
			self.targetBoard.OpenReviveDialog(vid, itemcount);

		def NotEnoughPrism(self, itemcount):
			self.PopupMessage(localeInfo.NOT_ENOUGH_PRISM % (itemcount))

		def RefreshShopItemToolTip(self):
			self.interface.RefreshShopItemToolTip()
				
	def WarpTest(self):
		background.Destroy()
		background.Initialize()
		background.WarpTest(4742, 9544);
		background.RegisterEnvironmentData(0, "d:/ymir work/environment/b3.msenv")
		background.SetEnvironmentData(0)
		background.SetShadowLevel(background.SHADOW_ALL)		

	def video_listem_yenile(self):
		constInfo.fav_listim = []

	def youtuber_listesi_yenile(self):
		constInfo.youtuber_list = []

	def youtuber_video_listesi_yenile(self):
		constInfo.youtuber_video_list = []

	def video_listem(self, baslik, link):
		constInfo.fav_listim.append("#"+baslik+"#"+link)

	def youtuber_listesi(self, kim):
		constInfo.youtuber_list.append("#"+kim)

	def youtuber_video_listesi(self, baslik, link):
		constInfo.youtuber_video_list.append("#"+baslik+"#"+link)

	def __video_izle(self):
		val = int(len(constInfo.LINK_PLUS))
		if val < 3:
			chat.AppendChat(5, localeInfo.YOUTUBER_LINK_LEN_NOT_ENOUGH)
			return

		url = "http://ruya2.com/videos.php?youtube=%s" % str(constInfo.LINK_PLUS)
		self.interface.OpenYoutubeWebWindow(url)

	def __YoutuberListele(self):
		import uiYoutubeFavList
		self.youtuberlistdialog=uiYoutubeFavList.YoutuberListSelectDialog()
		self.youtuberlistdialog.Open()
		
	def YoutuberKimdir(self):
		pass
		
	if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
		def BINARY_AddChestDropInfo(self, chestVnum, pageIndex, slotIndex, itemVnum, itemCount):
			if self.interface:
				self.interface.AddChestDropInfo(chestVnum, pageIndex, slotIndex, itemVnum, itemCount)
						
		def BINARY_RefreshChestDropInfo(self, chestVnum):
			if self.interface:
				self.interface.RefreshChestDropInfo(chestVnum)
				
	if app.ENABLE_FISH_JIGSAW_EVENT:
		def MiniGameFishEvent(self, isEnable, lasUseCount):
			if self.interface:
				self.interface.SetFishEventStatus(isEnable)
				self.interface.MiniGameFishCount(lasUseCount)

		def MiniGameFishUse(self, shape, useCount):
			self.interface.MiniGameFishUse(shape, useCount)
			
		def MiniGameFishAdd(self, pos, shape):
			self.interface.MiniGameFishAdd(pos, shape)
			
		def MiniGameFishReward(self, vnum):
			self.interface.MiniGameFishReward(vnum)	

	def OpenTestDialog(self):
		if self.interface:
			self.interface.OpenmMailBox()
			
	if app.ENABLE_12ZI:
		def BINARY_NextBeadUpdateTime(self, time):
			if self.interface:
				self.interface.NextBeadUpdateTime(time)
				
	def BINARY_OpenKeyboardSettings(self):
		if self.dlgKeyboardSettings.IsShow():
			self.dlgKeyboardSettings.Close()
		else:
			self.dlgKeyboardSettings.Open()
		
	def BINARY_SetMissionMessage(self, message):
		self.interface.missionBoard.SetMission(message)
		
	def BINARY_SetSubMissionMessage(self, message):
		self.interface.missionBoard.SetSubMission(message)
		
	def BINARY_CleanMissionMessage(self):
		self.interface.missionBoard.CleanMission()
		
	if app.ENABLE_GEM_SYSTEM:
		if app.ENABLE_BOSS_GEM_SYSTEM:
			def OpenGemShop(self, gemShopType):
				if self.interface:
					self.interface.OpenGemShop(gemShopType)
					
			def CloseGemShop(self):
				if self.interface:
					self.interface.CloseGemShop()
					
			def RefreshGemShop(self, gemShopType):
				if self.interface:
					self.interface.RefreshGemShop(gemShopType)
		else:
			def OpenGemShop(self):
				if self.interface:
					self.interface.OpenGemShop()
					
			def CloseGemShop(self):
				if self.interface:
					self.interface.CloseGemShop()
					
			def RefreshGemShop(self):
				if self.interface:
					self.interface.RefreshGemShop()
				
	if app.ENABLE_ATTENDANCE_EVENT:
		def MiniGameAttendanceEvent(self, isEnable):
			if self.interface:
				self.interface.SetAttendanceEventStatus(isEnable)

		def MiniGameAttendanceSetData(self, type, value):
			self.interface.MiniGameAttendanceSetData(type, value)
			
		def RefreshHitCount(self, vid):
			if vid == self.targetBoard.GetTargetVID():
				self.targetBoard.RefreshHitCount(vid)
				
	def BossTrackingOpenPacket(self):
		net.SendChatPacket("/open_boss_tracking")
		
	def Boss_Open_Window(self):
		self.bosstracking.Open()
		
	def BINARY_Boss_Tracking(self, kill_time, start_time, channel, mob_vnum):
		self.bosstracking.SetData(kill_time, start_time, channel, mob_vnum, 0)

	if app.ENABLE_MINI_GAME:
		if app.ENABLE_MINI_GAME_CATCH_KING:					
			def MiniGameCatchKingEventStart(self, bigScore):
				if self.interface:
					self.interface.MiniGameCatchKingEventStart(bigScore)
					
			def MiniGameCatchKingSetHandCard(self, cardNumber):
				if self.interface:
					self.interface.MiniGameCatchKingSetHandCard(cardNumber)

			def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
				if self.interface:
					self.interface.MiniGameCatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
					
			def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
				if self.interface:
					self.interface.MiniGameCatchKingSetEndCard(cardPos, cardNumber)
					
			def MiniGameCatchKingReward(self, rewardCode):
				if self.interface:
					self.interface.MiniGameCatchKingReward(rewardCode)
					
	def DungTimes(self, a, b, c, d, e, f, g, h):
		self.interface.DungTimes(a, b, c, d, e, f, g, h)
	
	if app.ENABLE_DEFENSE_WAVE:
		def BINARY_Update_Mast_HP(self, hp, max):
			self.interface.BINARY_Update_Mast_HP(hp, max)

		def BINARY_Update_Mast_Window(self, i):
			self.interface.BINARY_Update_Mast_Window(int(i))
			
	if app.ENABLE_MAIL_BOX_SYSTEM:
		def BINARY_MAILBOX_OPEN(self):
			self.interface.OpenMailBox()

		if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
			if app.ENABLE_WON_MAIL_BOX_SYSTEM:
				def BINARY_MAILBOX_RECEIVE_INFO(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,won,time,check_acept,check_view):
					self.interface.LoadMailBox(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,won,time,check_acept,check_view)
			else:
				def BINARY_MAILBOX_RECEIVE_INFO(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,time,check_acept,check_view):
					self.interface.LoadMailBox(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,time,check_acept,check_view)
		else:
			if app.ENABLE_WON_MAIL_BOX_SYSTEM:
				def BINARY_MAILBOX_RECEIVE_INFO(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,won,time,check_acept,check_view):
					self.interface.LoadMailBox(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,won,time,check_acept,check_view)
			else:
				def BINARY_MAILBOX_RECEIVE_INFO(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,time,check_acept,check_view):
					self.interface.LoadMailBox(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,time,check_acept,check_view)

		def BINARY_MAILBOX_CHECK_NAME_SUCCESSFUL(self):
			self.interface.MailBoxCheckName()

		def BINARY_MAILBOX_SEND_MAIL_SUCCESSFUL(self):
			self.interface.MailBoxSendMail()

		def BINARY_MAILBOX_ACCEPT_SUCCESSFUL(self, id_mail):
			self.interface.MailBoxAceptMails(id_mail)

		def BINARY_MAILBOX_DELETE_SUCCESSFUL(self, id_mail):
			self.interface.MailBoxDeleteMails(id_mail)

		def BINARY_MAILBOX_CHECK_VIEW_SUCCESSFULL(self, id_mail):
			self.interface.MailBoxCheckView(id_mail)

		def BINARY_MAILBOX_SEND_LOADING_ACCEPT(self):
			self.interface.MailBoxAceptMailsLoad()

		def BINARY_MAILBOX_SEND_LOADING_DELETE(self):
			self.interface.MailBoxDeleteMailsLoad()

		def BINARY_MAILBOX_RECEIVE_ITEM_SOCKET(self,id_mail,index,sockets):
			self.interface.MailBoxSocketsItems(id_mail,index,sockets)

		def BINARY_MAILBOX_RECEIVE_ITEM_ATTR(self,id_mail,index,attr_type,attr_value):
			self.interface.MailBoxAttrItems(id_mail,index,attr_type,attr_value)

		def SERVER_MAILBOX_MAP(self,c_a,m_a,m_s):
			self.interface.wndMiniMap.MailBoxValues(int(c_a),int(m_a),int(m_s))
			
	def Refresh12ziTimer(self, currentFloor, jumpCount, limitTime, elapseTime):
		if self.interface:
			self.interface.Refresh12ziTimer(currentFloor, jumpCount, limitTime, elapseTime)
			
	if app.ENABLE_REFINE_MSG_ADD:
		def BINARY_RefineFailedTypeMessage(self, type):
			REFINE_FAILURE_DICT = {
				player.REFINE_FAIL_GRADE_DOWN: localeInfo.REFINE_FAILURE_GRADE_DOWN,
				player.REFINE_FAIL_DEL_ITEM:   localeInfo.REFINE_FAILURE_DEL_ITEM,
				player.REFINE_FAIL_KEEP_GRADE: localeInfo.REFINE_FAILURE_KEEP_GRADE,
				player.REFINE_FAIL_MAX:        localeInfo.REFINE_FAILURE
			}
			
			snd.PlaySound("sound/ui/jaeryun_fail.wav")
			self.PopupMessage(REFINE_FAILURE_DICT.get(type, player.REFINE_FAIL_MAX))
			self.interface.CheckRefineDialog(True)

	if app.ENABLE_12ZI:
		def BINARY_SetEnvironment(self, idx):
			self.indexEnv = idx
			self.curtain.SAFE_FadeOut(self.__SetEnvironment)

		def __SetEnvironment(self):
			background.SetEnvironmentData(self.indexEnv)
			self.curtain.FadeIn()
			
	if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
		def __OpenChequeTicket(self, itemPos):
			self.interface.OpenChequeTicket(int(itemPos))
			
	if (app.ENABLE_PET_ATTR_DETERMINE):
		def __OnResultPetAttrDetermine(self, pet_type):
			self.petmain.OnResultPetAttrDetermine(int(pet_type))
			
		def __OnResultPetAttrChange(self, pet_type):
			self.petmain.OnResultPetAttrChange(int(pet_type))
			
	def ezancal(self):
		snd.PlaySound("d:/ymir work/ezan.mp3")
		
	if app.ENABLE_CAPTCHA_SYSTEM:
		def captchaekran(self, sayi1, sayi2, sayi3, sayi4, sayi5):
			captchatest = uiCommon.CaptchaEkran()
			captchatest.SetText1("|cffffff00Bot Engel Sistemi")
			captchatest.SetText3("Oyuna devam edebilmek için üstteki kodu asagiya yaz.")
			captchatest.SetTimeOverMsg("Soruyu zamaninda cevaplamadigin için oyundan atiliyorsun.")
			captchatest.SetTimeOverEvent(self.captchacevap, "kapat")
			captchatest.SetAcceptEvent(lambda arg=True: self.captchacevap(arg))
			captchatest.SetCancelEvent(lambda arg=False: self.captchacevap(arg))
			captchatest.Open(15, sayi1, sayi2, sayi3, sayi4, sayi5)
			self.captchatest = captchatest

		def captchacevap(self, answer):

			if not self.captchatest:
				return
		
			if answer == False:
				self.captchatest.Temizle()
				# chat.AppendChat(1, "Temizledim bro.")
			elif answer == True:
				# chat.AppendChat(1, "Girilen sifre : " + str(self.captchatest.GetInput()))
				net.SendChatPacket("/captcha " + str(self.captchatest.GetInput()))
				self.captchatest.Close()
				self.captchatest = None
			else:
				net.SendChatPacket("/captcha 0")
				self.captchatest.Close()
				self.captchatest = None
			
	if app.ENABLE_AURA_SYSTEM:
		def ActAura(self, iAct, bWindow):
			if self.interface:
				self.interface.ActAura(iAct, bWindow)

		def AlertAura(self, bWindow):
			snd.PlaySound("sound/ui/make_soket.wav")
			# if bWindow:
				# self.PopupMessage(localeInfo.AURA_DEL_SERVEITEM)
			# else:
				# self.PopupMessage(localeInfo.AURA_DEL_ABSORDITEM)

	def RefineRarity(self):
		self.wndRR.Open()
		
	def RarityRefine_Success(self):
		self.PopupMessage(localeInfo.RARITY_REFINE_SUCCESS)
		self.wndRR.SuccessMessage()

	def RarityRefine_NeedItem(self, code):
		self.wndRR.NeedItem(code)

	def RarityRefine_Fail(self):
		self.PopupMessage(localeInfo.RARITY_REFINE_FAIL)
		self.wndRR.Failed()
		
	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def DungeonInfo(self, questindex):
			import constInfo
			constInfo.dungeonData["quest_index"] = questindex

		def CleanDungeonInfo(self):
			import constInfo
			constInfo.dungeonInfo = []

		def CleanDungeonRanking(self):
			import constInfo
			constInfo.dungeonRanking["ranking_list"] = []

		def GetDungeonInfo(self, cmd):
			import constInfo
			cmd = cmd.split("#")

			if cmd[0] == "INPUT":
				constInfo.INPUT_IGNORE = int(cmd[1])

			elif cmd[0] == "CMD":
				net.SendQuestInputStringPacket(constInfo.dungeonData["quest_cmd"])
				constInfo.dungeonData["quest_cmd"] = ""

			else:
				pass

		def UpdateDungeonInfo(self, type, organization, levelLimit, partyMembers, mapName, mapIndex, mapCoordX, mapCoordY, cooldown, duration, entranceMapName, strengthBonusName, resistanceBonusName, itemVnum,reptime):
			type = int(type)
			organization = int(organization)
			levelLimit = int(levelLimit)
			partyMembers = int(partyMembers)
			mapName = str(mapName).replace("_", " ")
			mapIndex = int(mapIndex)
			mapCoordX = int(mapCoordX)
			mapCoordY = int(mapCoordY)
			cooldown = int(cooldown)
			duration = int(duration)
			entranceMapName = str(entranceMapName).replace("_", " ")
			strengthBonusName = str(strengthBonusName).replace("_", " ")
			resistanceBonusName = str(resistanceBonusName).replace("_", " ")
			itemVnum = int(itemVnum)
			reptimex = int(reptime)

			import constInfo
			constInfo.dungeonInfo.append(\
				{
					"type" : type,\
					"organization" : organization,\
					"level_limit" : levelLimit,\
					"party_members" : partyMembers,\
					"map" : mapName,\
					"map_index" : mapIndex,\
					"map_coord_x" : mapCoordX,\
					"map_coord_y" : mapCoordY,\
					"cooldown" : cooldown,\
					"duration" : duration,\
					"entrance_map" : entranceMapName,\
					"strength_bonus" : strengthBonusName,\
					"resistance_bonus" : resistanceBonusName,\
					"item_vnum" : itemVnum,
					"reptime" : reptimex,
				},
			)

		def UpdateDungeonRanking(self, name, level, pointType):
			name = str(name)
			level = int(level)
			pointType = int(pointType)

			import constInfo
			constInfo.dungeonRanking["ranking_list"].append([name, level, pointType],)

		def OpenDungeonRanking(self):
			import uiDungeonInfo
			self.DungeonRank = uiDungeonInfo.DungeonRank()
			self.DungeonRank.Open()
	
	if app.ENABLE_AVERAGE_ITEM_PRICE:
		def AverageSearchOpen(self):
			self.average_price.Show()
			
		def BINARY_OnRecvAverageItemPrice(self, vnum, price, won):
			self.average_price.SetData(vnum, price, won)
	
	if app.ENABLE_GUILD_RANKING_SYSTEM:
		def BINARY_OnRecvGuildRank(self, name, master, win, loss):
			self.interface.SetGuildRankedData(name, master, win, loss)
			
		def BINARY_OpenGuildWarHistory(self):
			self.interface.OpenGuildWarHistory()
			
	if app.ENABLE_SPECIAL_GACHA_SYSTEM:
		def Binary_LuckyBoxAppend(self, itemVnum, itemCount, needMoney):
			if self.interface:
				if self.interface.wndLuckyBox:
					if self.interface.wndLuckyBox.IsShow():
						self.interface.wndLuckyBox.SetData(itemVnum, itemCount, needMoney)
					else:
						self.interface.wndLuckyBox.Open()
						self.interface.wndLuckyBox.SetData(itemVnum, itemCount, needMoney)
						
