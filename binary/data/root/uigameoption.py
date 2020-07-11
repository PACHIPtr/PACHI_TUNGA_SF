import app
import net
import player
import chrmgr
import systemSetting
import chat
import background
import ui
import snd
import localeInfo
import constInfo
import uiPrivateShopBuilder # ±Ë¡ÿ»£
import interfaceModule # ±Ë¡ÿ»£
import serverInfo
import os
blockMode = 0
viewChatMode = 0

MOBILE = False

class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()		
		self.RefreshViewChat()
		self.RefreshAlwaysShowName()
		self.RefreshShowDamage()
		self.RefreshShowSalesText()
		self.RefreshShowOfflineShop()
		self.RefreshShowCompanions()
		self.RefreshShowMobInfo()
		self.RefreshGoldLogStatus()
		self.__ChatFilterRefresh()
		self.RefreshHealthBoard()
		self.RefreshAutoPickUp()
		self.RefreshGunsam()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0
		self.nameColorModeButtonList = []
		self.viewTargetBoardButtonList = []
		self.pvpModeButtonDict = {}
		self.blockButtonList = []
		self.viewChatButtonList = []
		self.alwaysShowNameButtonList = []
		self.showDamageButtonList = []
		self.showsalesTextButtonList = []
		self.showOfflineShopButtonList = []
		self.showCompanionsButtonList = []
		self.showMobInfoButtonList = []
		self.showGoldLogButtonList = []
		self.chatFilterList = []
		self.viewHealthBoardButtonList = []
		self.enableAutoPickUpButtonList = []
		self.dlgKeyboardSettings = None
		self.gunsam_button_list = []
		## Renewal ##
		self.option_page = None
		
	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY GAME OPTION DIALOG"
	
	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			##Names
			self.nameColorModeButtonName = GetObject("name_color")
			self.viewTargetBoardButtonName = GetObject("target_board")
			self.pvpModeButtonName = GetObject("pvp_mode")
			self.blockButtonName = GetObject("block")
			self.viewChatName = GetObject("chat")
			self.alwaysShowNameButtonName = GetObject("always_show_name")
			self.showDamageButtonName = GetObject("effect_on_off")
			self.showsalesTextButtonName = GetObject("salestext_on_off")
			self.showOfflineShopButtonName = GetObject("show_offline_shop_name")
			self.showCompanionsButtonName = GetObject("show_companions")
			self.showMobInfoButtonName = GetObject("show_mob_info")
			self.showGoldLogButtonName = GetObject("gold_on_off")
			self.chatFilterName = GetObject("chat_filter")
			self.viewHealthBoardButtonName = GetObject("healthboard_on_off")
			self.enableAutoPickUpButtonName = GetObject("autopickup_on_off")
			self.keyboardText = GetObject("keyboardText")
			self.gunsam_button_text = GetObject("gunsam_mode_button_text")
			##EndNames
			self.nameColorModeButtonList.append(GetObject("name_color_normal"))
			self.nameColorModeButtonList.append(GetObject("name_color_empire"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_no_view"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_view"))
			self.pvpModeButtonDict[player.PK_MODE_PEACE] = GetObject("pvp_peace")
			self.pvpModeButtonDict[player.PK_MODE_REVENGE] = GetObject("pvp_revenge")
			self.pvpModeButtonDict[player.PK_MODE_GUILD] = GetObject("pvp_guild")
			self.pvpModeButtonDict[player.PK_MODE_FREE] = GetObject("pvp_free")
			self.blockButtonList.append(GetObject("block_exchange_button"))
			self.blockButtonList.append(GetObject("block_party_button"))
			self.blockButtonList.append(GetObject("block_guild_button"))
			self.blockButtonList.append(GetObject("block_whisper_button"))
			self.blockButtonList.append(GetObject("block_friend_button"))
			self.blockButtonList.append(GetObject("block_party_request_button"))
			self.blockButtonList.append(GetObject("block_view_equipment_button"))
			
			self.viewChatButtonList.append(GetObject("view_chat_on_button"))
			self.viewChatButtonList.append(GetObject("view_chat_off_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_all_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_item_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_off_button"))
			self.showDamageButtonList.append(GetObject("show_damage_on_button"))
			self.showDamageButtonList.append(GetObject("show_damage_off_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_on_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_off_button"))
			self.showOfflineShopButtonList.append(GetObject("show_offline_shop_on"))
			self.showOfflineShopButtonList.append(GetObject("show_offline_shop_off"))
			self.showCompanionsButtonList.append(GetObject("show_companions_pets_button"))
			self.showCompanionsButtonList.append(GetObject("show_companions_mounts_button"))
			self.showCompanionsButtonList.append(GetObject("show_companions_shaman_button"))
			self.showMobInfoButtonList.append(GetObject("show_mob_level_button"))
			self.showMobInfoButtonList.append(GetObject("show_mob_AI_flag_button"))
			self.showGoldLogButtonList.append(GetObject("gold_on_button"))
			self.showGoldLogButtonList.append(GetObject("gold_off_button"))
			self.chatFilterList.append(GetObject("chat_filter_dice"))
			self.viewHealthBoardButtonList.append(GetObject("healthboard_on_button"))
			self.viewHealthBoardButtonList.append(GetObject("healthboard_off_button"))
			self.enableAutoPickUpButtonList.append(GetObject("autopickup_speed_on_button"))
			self.enableAutoPickUpButtonList.append(GetObject("autopickup_default_on_button"))
			self.enableAutoPickUpButtonList.append(GetObject("autopickup_off_button"))
			self.keyboardButton = GetObject("keyboardButton")
			self.gunsam_button_list.append(GetObject("gunsam_button_on"))
			self.gunsam_button_list.append(GetObject("gunsam_button_off"))
			
			self.NextPage = GetObject("NextPage")
			self.PrevPage = GetObject("PrevPage")
		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")
			
	def __Load(self):
		self.__Load_LoadScript("uiscript/gameoptiondialog.py")
		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.nameColorModeButtonList[0].SAFE_SetEvent(self.__OnClickNameColorModeNormalButton)
		self.nameColorModeButtonList[1].SAFE_SetEvent(self.__OnClickNameColorModeEmpireButton)

		self.viewTargetBoardButtonList[0].SAFE_SetEvent(self.__OnClickTargetBoardViewButton)
		self.viewTargetBoardButtonList[1].SAFE_SetEvent(self.__OnClickTargetBoardNoViewButton)

		self.pvpModeButtonDict[player.PK_MODE_PEACE].SAFE_SetEvent(self.__OnClickPvPModePeaceButton)
		self.pvpModeButtonDict[player.PK_MODE_REVENGE].SAFE_SetEvent(self.__OnClickPvPModeRevengeButton)
		self.pvpModeButtonDict[player.PK_MODE_GUILD].SAFE_SetEvent(self.__OnClickPvPModeGuildButton)
		self.pvpModeButtonDict[player.PK_MODE_FREE].SAFE_SetEvent(self.__OnClickPvPModeFreeButton)

		self.blockButtonList[0].SetToggleUpEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleUpEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleUpEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleUpEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleUpEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleUpEvent(self.__OnClickBlockPartyRequest)
		self.blockButtonList[6].SetToggleUpEvent(self.__OnClickBlockViewEquipmentButton)

		self.blockButtonList[0].SetToggleDownEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleDownEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleDownEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleDownEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleDownEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleDownEvent(self.__OnClickBlockPartyRequest)
		self.blockButtonList[6].SetToggleDownEvent(self.__OnClickBlockViewEquipmentButton)

		self.viewChatButtonList[0].SAFE_SetEvent(self.__OnClickViewChatOnButton)
		self.viewChatButtonList[1].SAFE_SetEvent(self.__OnClickViewChatOffButton)

		self.alwaysShowNameButtonList[0].SAFE_SetEvent(self.__OnClickAlwaysShowNameOnButton)
		self.alwaysShowNameButtonList[1].SAFE_SetEvent(self.__OnClickAlwaysShowItemNameOnButton)
		self.alwaysShowNameButtonList[2].SAFE_SetEvent(self.__OnClickAlwaysShowNameOffButton)

		self.showDamageButtonList[0].SAFE_SetEvent(self.__OnClickShowDamageOnButton)
		self.showDamageButtonList[1].SAFE_SetEvent(self.__OnClickShowDamageOffButton)
		
		self.showsalesTextButtonList[0].SAFE_SetEvent(self.__OnClickSalesTextOnButton)
		self.showsalesTextButtonList[1].SAFE_SetEvent(self.__OnClickSalesTextOffButton)		
		
		self.showOfflineShopButtonList[0].SAFE_SetEvent(self.__OnClickOfflineShopOnButton)
		self.showOfflineShopButtonList[1].SAFE_SetEvent(self.__OnClickOfflineShopOffButton)
		
		#Show companions
		self.showCompanionsButtonList[0].SetToggleUpEvent(self.__OnClickHidePetsButton)
		self.showCompanionsButtonList[0].SetToggleDownEvent(self.__OnClickHidePetsButton)

		self.showCompanionsButtonList[1].SetToggleUpEvent(self.__OnClickHideMountsButton)
		self.showCompanionsButtonList[1].SetToggleDownEvent(self.__OnClickHideMountsButton)
		
		self.showCompanionsButtonList[2].SetToggleUpEvent(self.__OnClickHideShamansButton)
		self.showCompanionsButtonList[2].SetToggleDownEvent(self.__OnClickHideShamansButton)
		
		self.showMobInfoButtonList[0].SetToggleUpEvent(self.__OnClickShowMobLevelButton)
		self.showMobInfoButtonList[1].SetToggleUpEvent(self.__OnClickShowMobAIFlagButton)

		self.showMobInfoButtonList[0].SetToggleDownEvent(self.__OnClickShowMobLevelButton)
		self.showMobInfoButtonList[1].SetToggleDownEvent(self.__OnClickShowMobAIFlagButton)
		
		self.showGoldLogButtonList[0].SAFE_SetEvent(self.__OnClickShowGoldOnButton)
		self.showGoldLogButtonList[1].SAFE_SetEvent(self.__OnClickShowGoldOffButton)	
		
		self.chatFilterList[0].SetToggleUpEvent(self.__ChatFilterDiceEnable) #1
		self.chatFilterList[0].SetToggleDownEvent(self.__ChatFilterDiceDisable) #0

		self.viewHealthBoardButtonList[0].SAFE_SetEvent(self.__OnClickViewHealthBoardOnButton)
		self.viewHealthBoardButtonList[1].SAFE_SetEvent(self.__OnClickViewHealthBoardOffButton)
		
		self.enableAutoPickUpButtonList[0].SAFE_SetEvent(self.__OnClickEnableAutoPickUpButton)
		self.enableAutoPickUpButtonList[1].SAFE_SetEvent(self.__OnClickEnableAutoPickUpSpeedButton)
		self.enableAutoPickUpButtonList[2].SAFE_SetEvent(self.__OnClickDisableAutoPickUpButton)
				
		self.keyboardButton.SAFE_SetEvent(self.__OnClickKeyboardButton)
		
		self.gunsam_button_list[0].SAFE_SetEvent(self.gunsam_mode_on)
		self.gunsam_button_list[1].SAFE_SetEvent(self.gunsam_mode_off)
		
		self.NextPage.SAFE_SetEvent(self.GrPageNext)
		self.PrevPage.SAFE_SetEvent(self.GrPagePrev)
		
		self.__ClickRadioButton(self.nameColorModeButtonList, constInfo.GET_CHRNAME_COLOR_INDEX())
		self.__ClickRadioButton(self.viewTargetBoardButtonList, constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD())
		self.__SetPeacePKMode()
		
	def GrPageNext(self):
		if self.option_page == 2:
			return
			
		self.option_page = 2
		self.ChangeGrPage()

	def GrPagePrev(self):
		if self.option_page == 1:
			return
			
		self.option_page = 1
		self.ChangeGrPage()
		
	def ChangeGrPage(self):
		page = self.option_page
		if page == 1:
			self.nameColorModeButtonName.Show()
			self.nameColorModeButtonList[0].Show()
			self.nameColorModeButtonList[1].Show()
			
			self.viewTargetBoardButtonName.Show()
			self.viewTargetBoardButtonList[0].Show()
			self.viewTargetBoardButtonList[1].Show()
			
			self.pvpModeButtonName.Show()
			self.pvpModeButtonDict[player.PK_MODE_PEACE].Show()
			self.pvpModeButtonDict[player.PK_MODE_REVENGE].Show()
			self.pvpModeButtonDict[player.PK_MODE_GUILD].Show()
			self.pvpModeButtonDict[player.PK_MODE_FREE].Show()
			
			self.blockButtonName.Show()
			self.blockButtonList[0].Show()
			self.blockButtonList[1].Show()
			self.blockButtonList[2].Show()
			self.blockButtonList[3].Show()
			self.blockButtonList[4].Show()
			self.blockButtonList[5].Show()
			self.blockButtonList[6].Show()
			
			self.viewChatName.Show()
			self.viewChatButtonList[0].Show()
			self.viewChatButtonList[1].Show()
			
			self.alwaysShowNameButtonName.Show()
			self.alwaysShowNameButtonList[0].Show()
			self.alwaysShowNameButtonList[1].Show()
			self.alwaysShowNameButtonList[2].Show()
			
			self.showDamageButtonName.Show()
			self.showDamageButtonList[0].Show()
			self.showDamageButtonList[1].Show()
			
			self.showsalesTextButtonName.Show()
			self.showsalesTextButtonList[0].Show()
			self.showsalesTextButtonList[1].Show()
			
			self.showOfflineShopButtonName.Hide()
			self.showOfflineShopButtonList[0].Hide()
			self.showOfflineShopButtonList[1].Hide()
			
			self.showCompanionsButtonName.Hide()
			self.showCompanionsButtonList[0].Hide()
			self.showCompanionsButtonList[1].Hide()
			self.showCompanionsButtonList[2].Hide()
			
			self.showMobInfoButtonName.Hide()
			self.showMobInfoButtonList[0].Hide()
			self.showMobInfoButtonList[1].Hide()
			
			self.showGoldLogButtonName.Hide()
			self.showGoldLogButtonList[0].Hide()
			self.showGoldLogButtonList[1].Hide()
			
			self.chatFilterName.Hide()
			self.chatFilterList[0].Hide()
			
			self.viewHealthBoardButtonName.Hide()
			self.viewHealthBoardButtonList[0].Hide()
			self.viewHealthBoardButtonList[1].Hide()
			
			self.enableAutoPickUpButtonName.Hide()
			self.enableAutoPickUpButtonList[0].Hide()
			self.enableAutoPickUpButtonList[1].Hide()
			self.enableAutoPickUpButtonList[2].Hide()
			
			self.keyboardText.Hide()
			self.keyboardButton.Hide()
			
			self.gunsam_button_text.Hide()
			self.gunsam_button_list[0].Hide()
			self.gunsam_button_list[1].Hide()
		else:
			self.nameColorModeButtonName.Hide()
			self.nameColorModeButtonList[0].Hide()
			self.nameColorModeButtonList[1].Hide()
			
			self.viewTargetBoardButtonName.Hide()
			self.viewTargetBoardButtonList[0].Hide()
			self.viewTargetBoardButtonList[1].Hide()
			
			self.pvpModeButtonName.Hide()
			self.pvpModeButtonDict[player.PK_MODE_PEACE].Hide()
			self.pvpModeButtonDict[player.PK_MODE_REVENGE].Hide()
			self.pvpModeButtonDict[player.PK_MODE_GUILD].Hide()
			self.pvpModeButtonDict[player.PK_MODE_FREE].Hide()
			
			self.blockButtonName.Hide()
			self.blockButtonList[0].Hide()
			self.blockButtonList[1].Hide()
			self.blockButtonList[2].Hide()
			self.blockButtonList[3].Hide()
			self.blockButtonList[4].Hide()
			self.blockButtonList[5].Hide()
			self.blockButtonList[6].Hide()
			
			self.viewChatName.Hide()
			self.viewChatButtonList[0].Hide()
			self.viewChatButtonList[1].Hide()
			
			self.alwaysShowNameButtonName.Hide()
			self.alwaysShowNameButtonList[0].Hide()
			self.alwaysShowNameButtonList[1].Hide()
			self.alwaysShowNameButtonList[2].Hide()
			
			self.showDamageButtonName.Hide()
			self.showDamageButtonList[0].Hide()
			self.showDamageButtonList[1].Hide()
			
			self.showsalesTextButtonName.Hide()
			self.showsalesTextButtonList[0].Hide()
			self.showsalesTextButtonList[1].Hide()
			##
			self.showOfflineShopButtonName.Show()
			self.showOfflineShopButtonList[0].Show()
			self.showOfflineShopButtonList[1].Show()
			
			self.showCompanionsButtonName.Show()
			self.showCompanionsButtonList[0].Show()
			self.showCompanionsButtonList[1].Show()
			self.showCompanionsButtonList[2].Show()
			
			self.showMobInfoButtonName.Show()
			self.showMobInfoButtonList[0].Show()
			self.showMobInfoButtonList[1].Show()
			
			self.showGoldLogButtonName.Show()
			self.showGoldLogButtonList[0].Show()
			self.showGoldLogButtonList[1].Show()
			
			self.chatFilterName.Show()
			self.chatFilterList[0].Show()
			
			self.viewHealthBoardButtonName.Show()
			self.viewHealthBoardButtonList[0].Show()
			self.viewHealthBoardButtonList[1].Show()
			
			self.enableAutoPickUpButtonName.Show()
			self.enableAutoPickUpButtonList[0].Show()
			self.enableAutoPickUpButtonList[1].Show()
			self.enableAutoPickUpButtonList[2].Show()
			
			self.keyboardText.Show()
			self.keyboardButton.Show()
			
			self.gunsam_button_text.Show()
			self.gunsam_button_list[0].Show()
			self.gunsam_button_list[1].Show()
	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()

	def __SetNameColorMode(self, index):
		constInfo.SET_CHRNAME_COLOR_INDEX(index)
		self.__ClickRadioButton(self.nameColorModeButtonList, index)

	def __SetTargetBoardViewMode(self, flag):
		constInfo.SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(flag)
		self.__ClickRadioButton(self.viewTargetBoardButtonList, flag)

	def __OnClickNameColorModeNormalButton(self):
		self.__SetNameColorMode(0)

	def __OnClickNameColorModeEmpireButton(self):
		self.__SetNameColorMode(1)

	def __OnClickTargetBoardViewButton(self):
		self.__SetTargetBoardViewMode(0)

	def __OnClickTargetBoardNoViewButton(self):
		self.__SetTargetBoardViewMode(1)

	def __OnClickCameraModeShortButton(self):
		self.__SetCameraMode(0)

	def __OnClickCameraModeLongButton(self):
		self.__SetCameraMode(1)

	def __OnClickFogModeLevel0Button(self):
		self.__SetFogLevel(0)

	def __OnClickFogModeLevel1Button(self):
		self.__SetFogLevel(1)

	def __OnClickFogModeLevel2Button(self):
		self.__SetFogLevel(2)
		
	#show companions
	def __OnClickHidePetsButton(self):
		systemSetting.SetHidePets(not systemSetting.IsHidePets())
		self.RefreshShowCompanions()

	def __OnClickHideMountsButton(self):
		systemSetting.SetHideMounts(not systemSetting.IsHideMounts())
		self.RefreshShowCompanions()
	
	def __OnClickHideShamansButton(self):
		systemSetting.SetHideShamans(not systemSetting.IsHideShamans())
		self.RefreshShowCompanions()

	def __OnClickBlockExchangeButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_EXCHANGE))
	def __OnClickBlockPartyButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY))
	def __OnClickBlockViewEquipmentButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_EQUIPMENT_REQUEST))
	def __OnClickBlockGuildButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_GUILD))
	def __OnClickBlockWhisperButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_WHISPER))
	def __OnClickBlockFriendButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_FRIEND))
	def __OnClickBlockPartyRequest(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY_REQUEST))
		
	def __OnClickViewHealthBoardOnButton(self):
		net.SendChatPacket("/health_board_config 0")
		constInfo.CONFIG_HIDE_HEALTH_BOARD = 0
		self.RefreshHealthBoard()
		
	def __OnClickViewHealthBoardOffButton(self):
		net.SendChatPacket("/health_board_config 1")
		constInfo.CONFIG_HIDE_HEALTH_BOARD = 1
		self.RefreshHealthBoard()
		
	def __OnClickEnableAutoPickUpButton(self):
		constInfo.OTO_TOPLAMA_NEW = 1
		self.enableAutoPickUpButtonList[0].Down()
		self.enableAutoPickUpButtonList[1].SetUp()
		self.enableAutoPickUpButtonList[2].SetUp()
		import uiCommon
		self.wndPopupDialog = uiCommon.SupportInformation()
		self.wndPopupDialog.SetWidth(500,105+25)
		self.wndPopupDialog.SetText1(localeInfo.AUTO_ITEM_TEXT_1)
		self.wndPopupDialog.SetText2(localeInfo.AUTO_ITEM_TEXT_2)
		self.wndPopupDialog.SetAcceptEvent(ui.__mem_func__(self.CloseDialog))
		self.wndPopupDialog.Open(10)
	
	def __OnClickEnableAutoPickUpSpeedButton(self):
		systemSetting.SetAutoPickUp(True)
		constInfo.OTO_TOPLAMA_NEW = 0
		self.RefreshAutoPickUp()
	
	def __OnClickDisableAutoPickUpButton(self):
		systemSetting.SetAutoPickUp(False)
		#constInfo.OTO_TOPLAMA_NEW = 0
		constInfo.OTO_TOPLAMA_NEW = 0
		self.RefreshAutoPickUp()
				
	def gunsam_mode_on(self):
		net.SendChatPacket("/gunsam_config 0")
		constInfo.CONFIG_DISABLE_GUNSAM = 0
		self.RefreshGunsam()
		
	def gunsam_mode_off(self):
		net.SendChatPacket("/gunsam_config 1")
		constInfo.CONFIG_DISABLE_GUNSAM = 1
		self.RefreshGunsam()
		
	def __OnClickViewChatOnButton(self):
		global viewChatMode
		viewChatMode = 1
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()
	def __OnClickViewChatOffButton(self):
		global viewChatMode
		viewChatMode = 0
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()

	def __OnClickAlwaysShowNameOnButton(self):
		systemSetting.SetAlwaysShowNameFlag(0)
		self.RefreshAlwaysShowName()
		
	def __OnClickAlwaysShowItemNameOnButton(self):
		systemSetting.SetAlwaysShowNameFlag(1)
		self.RefreshAlwaysShowName()

	def __OnClickAlwaysShowNameOffButton(self):
		systemSetting.SetAlwaysShowNameFlag(2)
		self.RefreshAlwaysShowName()

	def __OnClickShowDamageOnButton(self):
		systemSetting.SetShowDamageFlag(True)
		self.RefreshShowDamage()

	def __OnClickShowDamageOffButton(self):
		systemSetting.SetShowDamageFlag(False)
		self.RefreshShowDamage()
		
	def __OnClickSalesTextOnButton(self):
		systemSetting.SetShowSalesTextFlag(True)
		self.RefreshShowSalesText()
		uiPrivateShopBuilder.UpdateADBoard()

	def __OnClickSalesTextOffButton(self):
		systemSetting.SetShowSalesTextFlag(False)
		self.RefreshShowSalesText()	
		
	def __OnClickOfflineShopOnButton(self):
		self.__OnClickSalesTextOnButton()
		systemSetting.SetShowOfflineShop(1)
		self.RefreshShowOfflineShop()
		uiPrivateShopBuilder.UpdateADBoard()
		
	def __OnClickOfflineShopWoOnButton(self):
		self.__OnClickSalesTextOffButton()
		systemSetting.SetShowOfflineShop(True)
		self.RefreshShowOfflineShop()
	
	def __OnClickOfflineShopOffButton(self):
		self.__OnClickSalesTextOffButton()
		systemSetting.SetShowOfflineShop(False)
		self.RefreshShowOfflineShop()
		
	def __OnClickShowMobLevelButton(self):
		if app.WJ_SHOW_MOB_INFO:
			if systemSetting.IsShowMobLevel():
				systemSetting.SetShowMobLevel(False)
			else:
				systemSetting.SetShowMobLevel(True)

			self.RefreshShowMobInfo()

	def __OnClickShowMobAIFlagButton(self):
		if app.WJ_SHOW_MOB_INFO:
			if systemSetting.IsShowMobAIFlag():
				systemSetting.SetShowMobAIFlag(False)
			else:
				systemSetting.SetShowMobAIFlag(True)

			self.RefreshShowMobInfo()
			
	def __OnClickShowGoldOnButton(self):
		systemSetting.SetShowMoneyLog(True)
		systemSetting.SaveConfig()
		self.RefreshGoldLogStatus()
		
	def __OnClickShowGoldOffButton(self):
		systemSetting.SetShowMoneyLog(False)
		systemSetting.SaveConfig()
		self.RefreshGoldLogStatus()
		
	def __CheckPvPProtectedLevelPlayer(self):	
		if player.GetStatus(player.LEVEL)<constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__SetPeacePKMode()
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return 1

		return 0

	def __SetPKMode(self, mode):
		for btn in self.pvpModeButtonDict.values():
			btn.SetUp()
		if self.pvpModeButtonDict.has_key(mode):
			self.pvpModeButtonDict[mode].Down()

	def __SetPeacePKMode(self):
		self.__SetPKMode(player.PK_MODE_PEACE)

	def __RefreshPVPButtonList(self):
		self.__SetPKMode(player.GetPKMode())
		
	if app.ENABLE_MELEY_LAIR_DUNGEON:
		def setMeleyMap(self):
			mapName = background.GetCurrentMapName()
			if mapName == "metin2_map_n_flame_dragon":
				if player.GetGuildID() != 0 and player.GetPKMode() != player.PK_MODE_GUILD:
					for btn in self.pvpModeButtonDict.values():
						btn.SetUp()
					
					net.SendChatPacket("/pkmode 4", chat.CHAT_TYPE_TALKING)
					self.pvpModeButtonDict[player.PK_MODE_GUILD].Down()

		def isMeleyMap(self, button):
			mapName = background.GetCurrentMapName()
			if mapName == "metin2_map_n_flame_dragon":
				if self.pvpModeButtonDict[button]:
					self.pvpModeButtonDict[button].SetUp()
				
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CANNOT_CHANGE_FIGHT_MODE)
				return 1
			
			return 0

	def __OnClickPvPModePeaceButton(self):
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			if self.isMeleyMap(player.PK_MODE_PEACE):
				return
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 0", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeRevengeButton(self):
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			if self.isMeleyMap(player.PK_MODE_REVENGE):
				return
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 1", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeFreeButton(self):
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			if self.isMeleyMap(player.PK_MODE_FREE):
				return
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 2", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeGuildButton(self):
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			if self.isMeleyMap(player.PK_MODE_GUILD):
				return
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if 0 == player.GetGuildID():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
			return

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 4", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def OnChangePKMode(self):
		self.__RefreshPVPButtonList()

	def __OnChangeMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_PHONE_NUMBER_TITLE)
		inputDialog.SetMaxLength(13)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobilePhoneNumber))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def __OnDeleteMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.MESSENGER_DO_YOU_DELETE_PHONE_NUMBER)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDeleteMobile))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnInputMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()

		if not text:
			return

		text.replace('-', '')
		net.SendChatPacket("/mobile " + text)
		self.OnCloseInputDialog()
		return True

	def OnInputMobileAuthorityCode(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()
		net.SendChatPacket("/mobile_auth " + text)
		self.OnCloseInputDialog()
		return True

	def OnDeleteMobile(self):
		global MOBILE
		if not MOBILE:
			return

		net.SendChatPacket("/mobile")
		self.OnCloseQuestionDialog()
		return True

	def OnCloseInputDialog(self):
		self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def RefreshMobile(self):
		global MOBILE
		if not MOBILE:
			return

		if player.HasMobilePhoneNumber():
			self.inputMobileButton.Hide()
			self.deleteMobileButton.Show()
		else:
			self.inputMobileButton.Show()
			self.deleteMobileButton.Hide()

	def OnMobileAuthority(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialogWithDescription()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_TITLE)
		inputDialog.SetDescription(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_DESCRIPTION)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobileAuthorityCode))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.SetMaxLength(4)
		inputDialog.SetBoardWidth(310)
		inputDialog.Open()
		self.inputDialog = inputDialog

	def RefreshBlock(self):
		global blockMode
		for i in xrange(len(self.blockButtonList)):
			if 0 != (blockMode & (1 << i)):
				self.blockButtonList[i].Down()
			else:
				self.blockButtonList[i].SetUp()
				
	def RefreshViewChat(self):
		if systemSetting.IsViewChat():
			self.viewChatButtonList[0].Down()
			self.viewChatButtonList[1].SetUp()
		else:
			self.viewChatButtonList[0].SetUp()
			self.viewChatButtonList[1].Down()

	def RefreshAlwaysShowName(self):
		if systemSetting.IsAlwaysShowName() == 0:
			self.alwaysShowNameButtonList[0].Down()
			self.alwaysShowNameButtonList[1].SetUp()
			self.alwaysShowNameButtonList[2].SetUp()
		elif systemSetting.IsAlwaysShowName() == 1:
			self.alwaysShowNameButtonList[0].SetUp()
			self.alwaysShowNameButtonList[1].Down()
			self.alwaysShowNameButtonList[2].SetUp()			
		else:
			self.alwaysShowNameButtonList[0].SetUp()
			self.alwaysShowNameButtonList[1].SetUp()
			self.alwaysShowNameButtonList[2].Down()

	def RefreshShowDamage(self):
		if systemSetting.IsShowDamage():
			self.showDamageButtonList[0].Down()
			self.showDamageButtonList[1].SetUp()
		else:
			self.showDamageButtonList[0].SetUp()
			self.showDamageButtonList[1].Down()
			
	def RefreshShowSalesText(self):
		if systemSetting.IsShowSalesText():
			self.showsalesTextButtonList[0].Down()
			self.showsalesTextButtonList[1].SetUp()
		else:
			self.showsalesTextButtonList[0].SetUp()
			self.showsalesTextButtonList[1].Down()
			
	def RefreshShowOfflineShop(self):
		if systemSetting.IsShowOfflineShop():
			self.showOfflineShopButtonList[0].Down()
			self.showOfflineShopButtonList[1].SetUp()	
		else:
			self.showOfflineShopButtonList[0].SetUp()
			self.showOfflineShopButtonList[1].Down()
			
	def RefreshShowCompanions(self):
		if systemSetting.IsHidePets():
			self.showCompanionsButtonList[0].Down()
		else:
			self.showCompanionsButtonList[0].SetUp()

		if systemSetting.IsHideMounts():
			self.showCompanionsButtonList[1].Down()
		else:
			self.showCompanionsButtonList[1].SetUp()
			
		if systemSetting.IsHideShamans():
			self.showCompanionsButtonList[2].Down()
		else:
			self.showCompanionsButtonList[2].SetUp()
			
	def RefreshShowMobInfo(self):
		if app.WJ_SHOW_MOB_INFO:
			if systemSetting.IsShowMobLevel():
				self.showMobInfoButtonList[0].Down()
			else:
				self.showMobInfoButtonList[0].SetUp()

			if systemSetting.IsShowMobAIFlag():
				self.showMobInfoButtonList[1].Down()
			else:
				self.showMobInfoButtonList[1].SetUp()
				
	def RefreshGoldLogStatus(self):
		if systemSetting.IsShowMoneyLog():
			self.showGoldLogButtonList[0].Down()
			self.showGoldLogButtonList[1].SetUp()
		else:
			self.showGoldLogButtonList[0].SetUp()
			self.showGoldLogButtonList[1].Down()
			
	def __ChatFilterDiceEnable(self) :
		systemSetting.SetShowDiceInfo(1)
		self.__ChatFilterRefresh()

	def __ChatFilterDiceDisable(self) :
		systemSetting.SetShowDiceInfo(0)
		self.__ChatFilterRefresh()
			
	def __ChatFilterRefresh(self) :
		if systemSetting.IsShowDiceInfo():
			self.chatFilterList[0].SetUp()
		else:
			self.chatFilterList[0].Down()
			
	def RefreshHealthBoard(self):
		if constInfo.CONFIG_HIDE_HEALTH_BOARD == 0:
			self.viewHealthBoardButtonList[0].Down()
			self.viewHealthBoardButtonList[1].SetUp()
		else:
			self.viewHealthBoardButtonList[0].SetUp()
			self.viewHealthBoardButtonList[1].Down()
			
	def RefreshAutoPickUp(self):
		if systemSetting.IsAutoPickUp():
			self.enableAutoPickUpButtonList[0].SetUp()
			self.enableAutoPickUpButtonList[1].Down()
			self.enableAutoPickUpButtonList[2].SetUp()
		else:
			self.enableAutoPickUpButtonList[0].SetUp()
			self.enableAutoPickUpButtonList[1].SetUp()
			self.enableAutoPickUpButtonList[2].Down()
			
	def RefreshGunsam(self):
		if constInfo.CONFIG_DISABLE_GUNSAM == 0:
			self.gunsam_button_list[0].Down()
			self.gunsam_button_list[1].SetUp()
		else:
			self.gunsam_button_list[0].SetUp()
			self.gunsam_button_list[1].Down()
			
	def OnBlockMode(self, mode):
		global blockMode
		blockMode = mode
		self.RefreshBlock()

	def Show(self):
		self.RefreshMobile()
		self.RefreshBlock()
		ui.ScriptWindow.Show(self)
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			self.setMeleyMap()
		self.option_page = 1
		self.ChangeGrPage()
			
	def BindKeyBoardSettings(self, dlgKeyboardSettings):
		self.dlgKeyboardSettings = dlgKeyboardSettings
		
	def __OnClickKeyboardButton(self):
		if self.dlgKeyboardSettings:
			if self.dlgKeyboardSettings.IsShow():
				self.dlgKeyboardSettings.Close()
			else:
				self.Close()
				self.dlgKeyboardSettings.Open()

	def Close(self):
		self.Hide()
