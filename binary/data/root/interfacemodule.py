##
## Interface
##
import app
import player
import net
import systemSetting
import chat
import guild
import miniMap
import event
import item

import constInfo
import wndMgr
import uiTaskbar
import uiCharacter
import uiInventory
import uiDragonSoul
import uiChat
import uiMessenger
import uiCards
import ui
import uiHelp
import uiWhisper
import uiPointReset
import uiShop
import uiExchange
import uiSystem
import uiRestart
import uiToolTip
import uiMiniMap
import uiParty
import uiSafebox
import uiGuild
import uiQuest
import uiPrivateShopBuilder
import uiCommon
import uiRefine
import uiEquipmentDialog
import uiGameButton
import uiTip
import uiCube
import uibiowindow

# ACCESSORY_REFINE_ADD_METIN_STONE
import uiSelectItem
# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
import uiScriptLocale
import localeInfo
if app.ENABLE_EVENT_INFORMATION_SYSTEM:
	import uiEvent

if app.ENABLE_MELEY_LAIR_DUNGEON:
	import uimeleylairranking
	
if app.ENABLE_CHANGELOOK_SYSTEM:
	import uichangelook	
	
if app.ENABLE_SHOP_SEARCH_SYSTEM:
	import uiPrivateShopSearch
		
if app.ENABLE_ACCE_SYSTEM:
	import uisash
	
if app.ENABLE_ITEM_COMBINATION_SYSTEM:
	import uiitemcombination
	
if app.ENABLE_12ZI:
	import ui12zi

if app.ENABLE_YOUTUBER_SYSTEM:
	import uiyoutuber
	
if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
	import uiChestDrop
	
if app.ENABLE_GEM_SYSTEM:
	import uiGemShop
	import uiRedGemShop
	import uiselectitemex
	
if app.ENABLE_MAIL_BOX_SYSTEM:
	import uimailbox
	
if app.ENABLE_CUBE_EX_RENEWAL:
	import uicube_ex
	
if app.ENABLE_CHEQUE_COUPON_SYSTEM:
	import uiChequeTicket
	
if app.ENABLE_AURA_SYSTEM:
	import uiaura

if app.ENABLE_AUTO_HUNTING_SYSTEM:
	import uiautohunt
	
if app.ENABLE_SPECIAL_GACHA_SYSTEM:
	import uiLuckyBox

IsQBHide = 0
class Interface(object):
	CHARACTER_STATUS_TAB = 1
	CHARACTER_SKILL_TAB = 2
	
	def __init__(self):
		systemSetting.SetInterfaceHandler(self)
		self.windowOpenPosition = 0
		if app.WJ_TRADABLE_ICON:
			self.onTopWindow = player.ON_TOP_WND_NONE
		self.dlgWhisperWithoutTarget = None
		self.inputDialog = None
		self.tipBoard = None
		self.bigBoard = None
		if app.ENABLE_OX_RENEWAL:
			self.bigBoardControl = None
		self.missionBoard = None
		self.wndGiftBox = None

		# ITEM_MALL
		self.mallPageDlg = None
		# END_OF_ITEM_MALL

		self.wndWeb = None
		if app.ENABLE_YOUTUBER_SYSTEM:
			self.wndYoutubeWeb = None
		self.wndTaskBar = None
		self.wndCharacter = None
		self.wndInventory = None
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndExtendedInventory = None
		self.wndExpandedTaskBar = None
		self.wndExpandedMoneyTaskbar = None
		self.wndDragonSoul = None
		self.wndDragonSoulRefine = None
		self.wndChat = None
		self.wndMessenger = None
		self.wndMiniMap = None
		self.wndGuild = None
		self.wndGuildBuilding = None
		if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
			self.dlgChestDrop = None
		if app.ENABLE_GEM_SYSTEM:
			self.dlgGemShop = None
			self.dlgRedGemShop = None
		self.listGMName = {}
		self.wndQuestWindow = {}
		self.wndQuestWindowNewKey = 0
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		self.wndBead = None
		if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
			self.wndImproveChequeTicket = None
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.wndDungeonInfo = None
		if app.ENABLE_GUILD_RANKING_SYSTEM:
			self.wndHistoryGuildWarsAndRanking = None
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			self.wndLuckyBox = None
		event.SetInterfaceWindow(self)

	def __del__(self):
		systemSetting.DestroyInterfaceHandler()
		event.SetInterfaceWindow(None)
		
	def SetStream(self, stream):
		self.stream = stream
		
	def SetGame(self, game):
		self.game = game
		
	def __OnClickGiftButton(self):
		if self.wndGameButton:
			if not self.wndGiftBox.IsShow():
				self.wndGiftBox.Open()
			else:
				self.wndGiftBox.Close()

	def ClearGift(self):
		if self.wndGameButton:
			self.wndGameButton.HideGiftButton()
		if self.wndGiftBox:
			self.wndGiftBox.Clear()
			self.wndGiftBox.Refresh()
	def OpenGift(self):
		if self.wndGameButton:
			self.wndGameButton.ShowGiftButton()		

	################################
	## Make Windows & Dialogs
	def __MakeUICurtain(self):
		wndUICurtain = ui.Bar("TOP_MOST")
		wndUICurtain.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		wndUICurtain.SetColor(0x77000000)
		wndUICurtain.Hide()
		self.wndUICurtain = wndUICurtain

	def __MakeMessengerWindow(self):
		self.wndMessenger = uiMessenger.MessengerWindow()

		from _weakref import proxy
		self.wndMessenger.SetWhisperButtonEvent(lambda n,i=proxy(self):i.OpenWhisperDialog(n))
		self.wndMessenger.SetGuildButtonEvent(ui.__mem_func__(self.ToggleGuildWindow))

	def __MakeGuildWindow(self):
		self.wndGuild = uiGuild.GuildWindow()

	def __MakeChatWindow(self):
		
		wndChat = uiChat.ChatWindow()
		
		wndChat.SetSize(wndChat.CHAT_WINDOW_WIDTH, 0)
		wndChat.SetPosition(wndMgr.GetScreenWidth()/2 - wndChat.CHAT_WINDOW_WIDTH/2, wndMgr.GetScreenHeight() - wndChat.EDIT_LINE_HEIGHT - 37)
		wndChat.SetHeight(200)
		wndChat.Refresh()
		wndChat.Show()

		self.wndChat = wndChat
		self.wndChat.BindInterface(self)
		self.wndChat.SetSendWhisperEvent(ui.__mem_func__(self.OpenWhisperDialogWithoutTarget))
		self.wndChat.SetOpenChatLogEvent(ui.__mem_func__(self.ToggleChatLogWindow))

	def __MakeTaskBar(self):
		wndTaskBar = uiTaskbar.TaskBar()
		wndTaskBar.LoadWindow()
		self.wndTaskBar = wndTaskBar
		self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_CHARACTER, ui.__mem_func__(self.ToggleCharacterWindowStatusPage))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_INVENTORY, ui.__mem_func__(self.ToggleInventoryWindow))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_MESSENGER, ui.__mem_func__(self.ToggleMessenger))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_SYSTEM, ui.__mem_func__(self.ToggleSystemDialog))	
		if uiTaskbar.TaskBar.IS_EXPANDED:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_EXPAND, ui.__mem_func__(self.ToggleExpandedButton))
			self.wndExpandedTaskBar = uiTaskbar.ExpandedTaskBar()
			self.wndExpandedTaskBar.LoadWindow()
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskbar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, ui.__mem_func__(self.ToggleDragonSoulWindow))
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskbar.ExpandedTaskBar.BUTTON_PET_GUI, ui.__mem_func__(self.TogglePetMain))
			if app.ENABLE_AUTO_HUNTING_SYSTEM:
				self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskbar.ExpandedTaskBar.BUTTON_AUTO_HUNT, ui.__mem_func__(self.OpenAutohuntWindow))
		else:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_CHAT, ui.__mem_func__(self.ToggleChat))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskbar.TaskBar.BUTTON_EXPAND_MONEY, ui.__mem_func__(self.ToggleExpandedMoneyButton))
		self.wndExpandedMoneyTaskbar = uiTaskbar.ExpandedMoneyTaskBar()
		self.wndExpandedMoneyTaskbar.LoadWindow()
		
		self.wndEnergyBar = None
		if app.ENABLE_ENERGY_SYSTEM:
			wndEnergyBar = uiTaskbar.EnergyBar()
			wndEnergyBar.LoadWindow()
			self.wndEnergyBar = wndEnergyBar

		import uiGift
		wndGiftBox=uiGift.GiftDialog()
		wndGiftBox.Hide()
		self.wndGiftBox=wndGiftBox

	def __MakeParty(self):
		wndParty = uiParty.PartyWindow()
		wndParty.Hide()
		self.wndParty = wndParty
		
	def __MakeGameButtonWindow(self):
		wndGameButton = uiGameButton.GameButtonWindow()
		wndGameButton.SetTop()
		wndGameButton.Show()
		wndGameButton.SetButtonEvent("STATUS", ui.__mem_func__(self.__OnClickStatusPlusButton))
		wndGameButton.SetButtonEvent("SKILL", ui.__mem_func__(self.__OnClickSkillPlusButton))
		wndGameButton.SetButtonEvent("QUEST", ui.__mem_func__(self.__OnClickQuestButton))
		wndGameButton.SetButtonEvent("HELP", ui.__mem_func__(self.__OnClickHelpButton))
		wndGameButton.SetButtonEvent("BUILD", ui.__mem_func__(self.__OnClickBuildButton))
		wndGameButton.SetButtonEvent("GIFT", ui.__mem_func__(self.__OnClickGiftButton))

		self.wndGameButton = wndGameButton

	def __IsChatOpen(self):
		return True
		
	def __MakeWindows(self):
		wndCharacter = uiCharacter.CharacterWindow()
		wndInventory = uiInventory.InventoryWindow()
		wndInventory.BindInterfaceClass(self)
		wndInventory.BindGameClass(self.game)
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			wndExtendedInventory = uiInventory.ExtendedInventoryWindow()
			wndExtendedInventory.BindInterfaceClass(self)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			wndDragonSoul = uiDragonSoul.DragonSoulWindow()	
			wndDragonSoulRefine = uiDragonSoul.DragonSoulRefineWindow()
		else:
			wndDragonSoul = None
			wndDragonSoulRefine = None
 
		wndMiniMap = uiMiniMap.MiniMap()
		wndSafebox = uiSafebox.SafeboxWindow()
		if app.WJ_TRADABLE_ICON:
			wndSafebox.BindInterface(self)
			
		if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
			self.wndImproveChequeTicket = uiChequeTicket.ChequeTicketWindow()
		
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			import uiDungeonInfo
			self.wndDungeonInfo = uiDungeonInfo.DungeonInfo()
			constInfo.DUNGEON_TRACKING = self.wndDungeonInfo
			
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			self.wndLuckyBox = uiLuckyBox.LuckyBoxWindow()
			self.wndLuckyBox.Hide()
		
		# ITEM_MALL
		wndMall = uiSafebox.MallWindow()
		self.wndMall = wndMall
		# END_OF_ITEM_MALL

		wndChatLog = uiChat.ChatLogWindow()
		wndChatLog.BindInterface(self)
	
		self.wndCharacter = wndCharacter
		self.wndInventory = wndInventory
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndExtendedInventory = wndExtendedInventory
		self.wndDragonSoul = wndDragonSoul
		self.wndDragonSoulRefine = wndDragonSoulRefine
		self.wndMiniMap = wndMiniMap
		self.wndSafebox = wndSafebox
		self.wndChatLog = wndChatLog
		
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			
		if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
			self.dlgChestDrop = uiChestDrop.ChestDropWindow()
			
		if app.ENABLE_GEM_SYSTEM:
			self.dlgGemShop = uiGemShop.GemShopWindow()
			self.dlgRedGemShop = uiRedGemShop.RedGemShopWindow()
			
		if app.ENABLE_GUILD_RANKING_SYSTEM:
			self.wndHistoryGuildWarsAndRanking = uiGuild.GuildWarHistoryAndRankingWindow()
			
	def __MakeDialogs(self):
		self.dlgExchange = uiExchange.ExchangeDialog()
		if app.WJ_TRADABLE_ICON:
			self.dlgExchange.BindInterface(self)
			self.dlgExchange.SetInven(self.wndInventory)
			self.wndInventory.BindWindow(self.dlgExchange)
		self.dlgExchange.LoadDialog()
		self.dlgExchange.SetCenterPosition()
		self.dlgExchange.Hide()
		
		self.dlgPointReset = uiPointReset.PointResetDialog()
		self.dlgPointReset.LoadDialog()
		self.dlgPointReset.Hide()

		self.dlgShop = uiShop.ShopDialog()
		if app.WJ_TRADABLE_ICON:
			self.dlgShop.BindInterface(self)
		self.dlgShop.LoadDialog()
		self.dlgShop.Hide()
		
		if app.ENABLE_YOUTUBER_SYSTEM:
			self.wndyoutuber = uiyoutuber.YoutubeWindow()
			self.wndyoutuber.Hide()
			self.wndyoutuber.BindInterfaceClass(self)
		
		self.dlgRestart = uiRestart.RestartDialog()
		self.dlgRestart.LoadDialog()
		self.dlgRestart.Hide()

		self.dlgSystem = uiSystem.SystemDialog(self.stream)
		self.dlgSystem.LoadDialog()
		self.dlgSystem.SetOpenHelpWindowEvent(ui.__mem_func__(self.OpenHelpWindow))
		self.dlgSystem.BindInterface(self)

		self.dlgSystem.Hide()

		self.dlgPassword = uiSafebox.PasswordDialog()
		self.dlgPassword.Hide()

		self.hyperlinkItemTooltip = uiToolTip.HyperlinkItemToolTip()
		self.hyperlinkItemTooltip.Hide()

		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()

		self.tooltipSkill = uiToolTip.SkillToolTip()
		self.tooltipSkill.Hide()

		self.privateShopBuilder = uiPrivateShopBuilder.PrivateShopBuilder()
		self.privateShopBuilder.Hide()

		self.dlgRefineNew = uiRefine.RefineDialogNew()
		if app.WJ_TRADABLE_ICON:
			self.dlgRefineNew.SetInven(self.wndInventory)
			self.wndInventory.BindWindow(self.dlgRefineNew)
		self.dlgRefineNew.Hide()
		
		if app.ENABLE_AUTO_HUNTING_SYSTEM:
			self.dlgAutohunt = uiautohunt.AutoHunt()
			self.dlgAutohunt.Hide()

	def __MakeHelpWindow(self):
		self.wndHelp = uiHelp.HelpWindow()
		self.wndHelp.LoadDialog()
		self.wndHelp.SetCloseEvent(ui.__mem_func__(self.CloseHelpWindow))
		self.wndHelp.Hide()

	def __MakeTipBoard(self):
		self.tipBoard = uiTip.TipBoard()
		self.tipBoard.Hide()

		self.bigBoard = uiTip.BigBoard()
		self.bigBoard.Hide()
		
		self.missionBoard = uiTip.MissionBoard()
		self.missionBoard.Hide()
		
		if app.ENABLE_OX_RENEWAL:
			self.bigBoardControl = uiTip.BigBoardControl()
			self.bigBoardControl.Hide()
		
	if app.ENABLE_MELEY_LAIR_DUNGEON:
		def __MakeMeleyRanking(self):
			self.wndMeleyRanking = uimeleylairranking.Window()
			self.wndMeleyRanking.LoadWindow()
			self.wndMeleyRanking.Hide()

	def __MakeBioWindow(self):
		self.wndBioWindow = uibiowindow.BiologWnd()
		self.wndBioWindow.LoadWindow()
		self.wndBioWindow.Hide()
			
	def __MakeWebWindow(self):
		if constInfo.IN_GAME_SHOP_ENABLE:
			import uiWeb
			self.wndWeb = uiWeb.WebWindow()
			self.wndWeb.LoadWindow()
			self.wndWeb.Hide()
					
	if app.ENABLE_CHANGELOOK_SYSTEM:
		def __MakeChangeLookWindow(self):
			self.wndChangeLook = uichangelook.Window(self.wndInventory, self)
			self.wndChangeLook.LoadWindow()
			self.wndChangeLook.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetChangeLookWindow(self.wndChangeLook)
			
	if app.ENABLE_SHOP_SEARCH_SYSTEM:
		def __MakePrivateShopSearchWindow(self):
			self.wndPrivateShopSearch = uiPrivateShopSearch.PrivateShopSearchDialog()
			self.wndPrivateShopSearch.LoadWindow()
			self.wndPrivateShopSearch.Hide()
			
	if app.ENABLE_EVENT_INFORMATION_SYSTEM:
		def __MakeEventWindow(self):
			self.wndEvent = uiEvent.EventWindow(self)
			self.wndEvent.LoadWindow()
			self.wndEvent.Hide()
			
	if app.ENABLE_ACCE_SYSTEM:
		def __MakeSashWindow(self):
			self.wndSashCombine = uisash.CombineWindow(self.wndInventory, self)
			self.wndSashCombine.LoadWindow()
			self.wndSashCombine.Hide()
			
			self.wndSashAbsorption = uisash.AbsorbWindow(self.wndInventory, self)
			self.wndSashAbsorption.LoadWindow()
			self.wndSashAbsorption.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetSashWindow(self.wndSashCombine, self.wndSashAbsorption)
				
	if app.ENABLE_AURA_SYSTEM:
		def __MakeAuraWindow(self):
			self.wndAuraRefine = uiaura.RefineWindow(self.wndInventory, self)
			self.wndAuraRefine.LoadWindow()
			self.wndAuraRefine.Hide()
	
			self.wndAuraAbsorption = uiaura.AbsorbWindow(self.wndInventory, self)
			self.wndAuraAbsorption.LoadWindow()
			self.wndAuraAbsorption.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetAuraWindow(self.wndAuraAbsorption, self.wndAuraRefine)
				
	if app.ENABLE_ITEM_COMBINATION_SYSTEM:
		def __MakeCombinationWindow(self):
			self.wndItemCombination = uiitemcombination.ItemCombinationWindow()
			self.wndItemCombination.BindInterface(self)
			self.wndItemCombination.LoadWindow()
			self.wndItemCombination.Hide()

			self.wndSkillBookCombination = uiitemcombination.SkillBookCombinationWindow()
			self.wndSkillBookCombination.BindInterface(self)
			self.wndSkillBookCombination.LoadWindow()
			self.wndSkillBookCombination.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetCombinationWindow(self.wndItemCombination, self.wndSkillBookCombination)
			if self.wndExtendedInventory:
				self.wndExtendedInventory.SetCombinationWindow(self.wndItemCombination, self.wndSkillBookCombination)
				
	if app.ENABLE_YOUTUBER_SYSTEM:
		def __MakeYoutubeWebWindow(self):
			import uiYoutube
			self.wndYoutubeWeb = uiYoutube.YoutubeWebWindow()
			self.wndYoutubeWeb.LoadWindow()
			self.wndYoutubeWeb.Hide()
				
	def __MakeCubeWindow(self):
		self.wndCube = uiCube.CubeWindow()
		self.wndCube.LoadWindow()
		self.wndCube.Hide()

	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()

	if app.ENABLE_MINIGAME_RUMI_EVENT:
		def __MakeCardsInfoWindow(self):
			self.wndCardsInfo = uiCards.CardsInfoWindow()
			self.wndCardsInfo.LoadWindow()
			self.wndCardsInfo.Hide()
			
		def __MakeCardsWindow(self):
			self.wndCards = uiCards.CardsWindow()
			self.wndCards.LoadWindow()
			self.wndCards.Hide()
			
		def __MakeCardsIconWindow(self):
			self.wndCardsIcon = uiCards.IngameWindow()
			self.wndCardsIcon.LoadWindow()
			self.wndCardsIcon.Hide()
			
	# ACCESSORY_REFINE_ADD_METIN_STONE
	def __MakeItemSelectWindow(self):
		self.wndItemSelect = uiSelectItem.SelectItemWindow()
		self.wndItemSelect.Hide()
		if app.ENABLE_GEM_SYSTEM:
			self.wndItemSelectEx = uiselectitemex.SelectItemWindow()
			self.wndItemSelectEx.Hide()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
	
	if app.ENABLE_MAIL_BOX_SYSTEM:
		def __MakeMailWindow(self):
			self.wndMailBox = uimailbox.MailBox()
			self.wndMailBox.LoadWindow()
			self.wndMailBox.Hide()
	
	def MakeInterface(self):
		self.__MakeTipBoard()
		self.__MakeMessengerWindow()
		self.__MakeGuildWindow()
		self.__MakeChatWindow()
		self.__MakeParty()
		self.__MakeWindows()
		self.__MakeDialogs()
		self.__MakeBioWindow()

		self.__MakeUICurtain()
		self.__MakeTaskBar()
		self.__MakeGameButtonWindow()
		self.__MakeHelpWindow()
		self.__MakeWebWindow()
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			self.__MakeMeleyRanking()
		self.__MakeCubeWindow()
		self.__MakeCubeResultWindow()
		if app.ENABLE_CUBE_EX_RENEWAL:
			self.__MakeCubeExWindow()
		if app.ENABLE_MINIGAME_RUMI_EVENT:
			self.__MakeCardsInfoWindow()
			self.__MakeCardsWindow()
			self.__MakeCardsIconWindow()
		if app.ENABLE_CHANGELOOK_SYSTEM:
			self.__MakeChangeLookWindow()
		if app.ENABLE_SHOP_SEARCH_SYSTEM:
			self.__MakePrivateShopSearchWindow()
		if app.ENABLE_EVENT_INFORMATION_SYSTEM:
			self.__MakeEventWindow()
		if app.ENABLE_ACCE_SYSTEM:
			self.__MakeSashWindow()
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.__MakeCombinationWindow()
		if app.ENABLE_YOUTUBER_SYSTEM:
			self.__MakeYoutubeWebWindow()
		if app.ENABLE_MAIL_BOX_SYSTEM:
			self.__MakeMailWindow()
		if app.ENABLE_AURA_SYSTEM:
			self.__MakeAuraWindow()
		import uiGift
		wndGiftBox=uiGift.GiftDialog()
		wndGiftBox.Hide()
		self.wndGiftBox=wndGiftBox
		
		
		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.__MakeItemSelectWindow()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
		
		if app.ENABLE_12ZI:
			self.__Make12ziTimerWindow()
			self.__MakeBeadWindow()
			self.__Make12ziRewardWindow()

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}

		self.wndInventory.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndExtendedInventory.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetItemToolTip(self.tooltipItem)
			self.wndDragonSoulRefine.SetItemToolTip(self.tooltipItem)

		self.wndSafebox.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_CHANGELOOK_SYSTEM:
			self.wndChangeLook.SetItemToolTip(self.tooltipItem)
		self.wndCube.SetItemToolTip(self.tooltipItem)
		self.wndCubeResult.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SHOP_SEARCH_SYSTEM:
			self.wndPrivateShopSearch.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_ACCE_SYSTEM:
			self.wndSashCombine.SetItemToolTip(self.tooltipItem)
			self.wndSashAbsorption.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			self.wndItemCombination.SetItemToolTip(self.tooltipItem)
			self.wndSkillBookCombination.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_EVENT_INFORMATION_SYSTEM:
			self.wndEvent.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_AURA_SYSTEM:
			self.wndAuraAbsorption.SetItemToolTip(self.tooltipItem)
			self.wndAuraRefine.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			self.wndLuckyBox.SetItemToolTip(self.tooltipItem)
			
		# ITEM_MALL
		self.wndMall.SetItemToolTip(self.tooltipItem)
		# END_OF_ITEM_MALL

		self.wndCharacter.SetSkillToolTip(self.tooltipSkill)
		self.wndTaskBar.SetItemToolTip(self.tooltipItem)
		self.wndTaskBar.SetSkillToolTip(self.tooltipSkill)
		self.wndGuild.SetSkillToolTip(self.tooltipSkill)

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.wndItemSelect.SetItemToolTip(self.tooltipItem)
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
		
		if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
			self.dlgChestDrop.SetItemToolTip(self.tooltipItem)

		self.dlgShop.SetItemToolTip(self.tooltipItem)
		self.dlgExchange.SetItemToolTip(self.tooltipItem)
		self.privateShopBuilder.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_GEM_SYSTEM:
			self.wndItemSelectEx.SetItemToolTip(self.tooltipItem)
			self.dlgGemShop.SetItemToolTip(self.tooltipItem)
			self.dlgRedGemShop.SetItemToolTip(self.tooltipItem)
		self.wndEvent.SetItemToolTip(self.tooltipItem)
		
		self.__InitWhisper()
		self.DRAGON_SOUL_IS_QUALIFIED = True

	def MakeHyperlinkTooltip(self, hyperlink):
		tokens = hyperlink.split(":")
		if tokens and len(tokens):
			type = tokens[0]
			if "item" == type:
				self.hyperlinkItemTooltip.SetHyperlinkItem(tokens)
			elif "whisper" == type:
				if player.GetName() == str(tokens[1]):
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.WHISPER_CANNOT_SEND_YOURSELF)
					return
				self.OpenWhisperDialog(str(tokens[1]))
			elif app.ENABLE_YOUTUBER_SYSTEM and "youtube" == type:
				url = "http://ruya2.com/videos.php?youtube=%s" % str(tokens[1])
				self.OpenYoutubeWebWindow(url)
				
	def __MakeBeadWindow(self):
		self.wndBead = ui12zi.BeadWindow()
		self.wndBead.Hide()
		
	def __Make12ziRewardWindow(self):
		self.wnd12ziReward = ui12zi.Reward12ziWindow()
		self.wnd12ziReward.SetItemToolTip(self.tooltipItem)
		self.wnd12ziReward.Hide()
		
	def __Make12ziTimerWindow(self):
		self.wnd12ziTimer = ui12zi.FloorLimitTimeWindow()
		self.wnd12ziTimer.Hide()

	## Make Windows & Dialogs
	################################
	
	def Close(self):

		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Destroy()
			del self.dlgWhisperWithoutTarget
		
		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Destroy()
			del self.dlgWhisperWithoutTarget

		if uiQuest.QuestDialog.__dict__.has_key("QuestCurtain"):
			uiQuest.QuestDialog.QuestCurtain.Close()

		if self.wndQuestWindow:
			for key, eachQuestWindow in self.wndQuestWindow.items():
				eachQuestWindow.nextCurtainMode = -1
				eachQuestWindow.OnPressEscapeKey()
				eachQuestWindow = None
		self.wndQuestWindow = {}

		if self.wndChat:
			self.wndChat.Destroy()

		if self.wndTaskBar:
			self.wndTaskBar.Destroy()
			
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Destroy()
			
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Destroy()
			
		if self.wndEnergyBar:
			self.wndEnergyBar.Destroy()

		if self.wndCharacter:
			self.wndCharacter.Hide()
			self.wndCharacter.Destroy()
			
		if self.wndInventory:
			self.wndInventory.Hide()
			self.wndInventory.Destroy()
			
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndExtendedInventory:
				self.wndExtendedInventory.Hide()
				self.wndExtendedInventory.Destroy()
			
		if self.wndDragonSoul:
			self.wndDragonSoul.Hide()
			self.wndDragonSoul.Destroy()
			
		if self.wndDragonSoulRefine:
			self.wndDragonSoulRefine.Destroy()
			
		if app.ENABLE_CHEQUE_COUPON_SYSTEM:
			if (self.wndImproveChequeTicket):
				del self.wndImproveChequeTicket

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				self.wndDungeonInfo.Destroy()
				
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			if self.wndLuckyBox:
				self.wndLuckyBox.Destroy()

		if self.dlgExchange:
			self.dlgExchange.Destroy()
		
		if app.ENABLE_YOUTUBER_SYSTEM:
			##-Youtube_Pencere-##
			if self.wndYoutubeWeb:
				self.wndYoutubeWeb.Destroy()
				self.wndYoutubeWeb = None
			##-Youtube_Pencere-##

			##-Youtuber-##
			if self.wndyoutuber:
				self.wndyoutuber.Destroy()
			##-Youtuber-##

		if self.dlgPointReset:
			self.dlgPointReset.Destroy()
			
		if self.wndGiftBox:
			self.wndGiftBox.Clear()
			self.wndGiftBox.Hide()
			self.wndGiftBox.Destroy()
		del self.wndGiftBox			
		if self.dlgShop:
			self.dlgShop.Destroy()

		if self.dlgRestart:
			self.dlgRestart.Destroy()

		if self.dlgSystem:
			self.dlgSystem.Destroy()

		if self.dlgPassword:
			self.dlgPassword.Destroy()

		if self.wndMiniMap:
			self.wndMiniMap.Destroy()
			self.wndMiniMap = None

		if self.wndSafebox:
			self.wndSafebox.Destroy()

		if self.wndWeb:
			self.wndWeb.Destroy()
			self.wndWeb = None

		if self.wndMall:
			self.wndMall.Destroy()

		if self.wndParty:
			self.wndParty.Destroy()

		if self.wndHelp:
			self.wndHelp.Destroy()
			
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			if self.wndMeleyRanking:
				self.wndMeleyRanking.Destroy()
			
		if app.ENABLE_CHANGELOOK_SYSTEM:
			if self.wndChangeLook:
				self.wndChangeLook.Destroy()
				
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			if self.wndItemCombination:
				self.wndItemCombination.Destroy()

			if self.wndSkillBookCombination:
				self.wndSkillBookCombination.Destroy()
			
		if self.wndCardsInfo:
			self.wndCardsInfo.Destroy()

		if self.wndCards:
			self.wndCards.Destroy()

		if self.wndCardsIcon:
			self.wndCardsIcon.Destroy()
			
		if self.wndBioWindow:
			self.wndBioWindow.Destroy()
			
		if app.ENABLE_SHOP_SEARCH_SYSTEM:
			if self.wndPrivateShopSearch:
				self.wndPrivateShopSearch.Destroy()	
				
		if app.ENABLE_EVENT_INFORMATION_SYSTEM:
			if self.wndEvent:
				self.wndEvent.Destroy()
				
		if app.ENABLE_ACCE_SYSTEM:
			if self.wndSashCombine:
				self.wndSashCombine.Destroy()
			
			if self.wndSashAbsorption:
				self.wndSashAbsorption.Destroy()
				
		if app.ENABLE_AURA_SYSTEM:
			if self.wndAuraAbsorption:
				self.wndAuraAbsorption.Destroy()

			if self.wndAuraRefine:
				self.wndAuraRefine.Destroy()
				
		if self.wndCube:
			self.wndCube.Destroy()
			
		if self.wndCubeResult:
			self.wndCubeResult.Destroy()
			
		if self.wndMessenger:
			self.wndMessenger.Destroy()
				
		if app.ENABLE_MAIL_BOX_SYSTEM:
			if self.wndMailBox:
				self.wndMailBox.Destroy()

		if self.wndGuild:
			self.wndGuild.Hide()
			self.wndGuild.Destroy()
			
		if self.privateShopBuilder:
			self.privateShopBuilder.Destroy()
			
		if self.dlgRefineNew:
			self.dlgRefineNew.Destroy()
			
		if app.ENABLE_AUTO_HUNTING_SYSTEM:
			if self.dlgAutohunt:
				self.dlgAutohunt.Destroy()
			
		if self.wndGuildBuilding:
			self.wndGuildBuilding.Destroy()

		if self.wndGameButton:
			self.wndGameButton.Destroy()

		# ITEM_MALL
		if self.mallPageDlg:
			self.mallPageDlg.Destroy()
		# END_OF_ITEM_MALL
		
		if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
			if self.dlgChestDrop:
				self.dlgChestDrop.Destroy()
				
		if app.ENABLE_GEM_SYSTEM:
			if self.dlgGemShop:
				self.dlgGemShop.Destroy()
				
			if self.dlgRedGemShop:
				self.dlgRedGemShop.Destroy()
				
			if self.wndItemSelectEx:
				self.wndItemSelectEx.Destroy()
		
		if app.ENABLE_GUILD_RANKING_SYSTEM:
			if (self.wndHistoryGuildWarsAndRanking):
				del self.wndHistoryGuildWarsAndRanking

		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Destroy()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
		
		if self.wndBead:
			self.wndBead.Hide()
			self.wndBead.Destroy()
			del self.wndBead
			
		if self.wnd12ziTimer:
			self.wnd12ziTimer.Hide()
			self.wnd12ziTimer.Destroy()
			del self.wnd12ziTimer
			
		if self.wnd12ziReward:
			self.wnd12ziReward.Hide()
			self.wnd12ziReward.Destroy()
			del self.wnd12ziReward
			
		self.wndChatLog.Destroy()
		
		for btn in self.questButtonList:
			btn.SetEvent(0)
		for btn in self.whisperButtonList:
			btn.SetEvent(0)
		for dlg in self.whisperDialogDict.itervalues():
			dlg.Destroy()
		for brd in self.guildScoreBoardDict.itervalues():
			brd.Destroy()
		for dlg in self.equipmentDialogDict.itervalues():
			dlg.Destroy()

		# ITEM_MALL
		del self.mallPageDlg
		# END_OF_ITEM_MALL

		del self.wndGuild
		del self.wndMessenger
		del self.wndUICurtain
		del self.wndChat
		del self.wndTaskBar
		if self.wndExpandedTaskBar:
			del self.wndExpandedTaskBar
		if self.wndExpandedMoneyTaskbar:
			del self.wndExpandedMoneyTaskbar
		del self.wndEnergyBar
		del self.wndCharacter
		del self.wndInventory
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndExtendedInventory:
				del self.wndExtendedInventory
		if self.wndDragonSoul:
			del self.wndDragonSoul
		if self.wndDragonSoulRefine:
			del self.wndDragonSoulRefine
		if app.ENABLE_YOUTUBER_SYSTEM:
			del self.wndyoutuber
		if app.ENABLE_CUBE_EX_RENEWAL:
			if self.wndCubeEx:
				del self.wndCubeEx
		del self.dlgExchange
		del self.dlgPointReset
		del self.dlgShop
		del self.dlgRestart
		del self.dlgSystem
		del self.dlgPassword
		del self.hyperlinkItemTooltip
		del self.tooltipItem
		del self.tooltipSkill
		del self.wndMiniMap
		del self.wndSafebox
		del self.wndMall
		del self.wndParty
		del self.wndHelp
		if app.ENABLE_MELEY_LAIR_DUNGEON:
			del self.wndMeleyRanking
		if app.ENABLE_CHANGELOOK_SYSTEM:
			del self.wndChangeLook
		if app.ENABLE_ITEM_COMBINATION_SYSTEM:
			del self.wndItemCombination
			del self.wndSkillBookCombination
		del self.wndCardsInfo
		del self.wndCards
		del self.wndCardsIcon
		del self.wndBioWindow
		if app.ENABLE_SHOP_SEARCH_SYSTEM:
			del self.wndPrivateShopSearch
		if app.ENABLE_EVENT_INFORMATION_SYSTEM:
			del self.wndEvent
		if app.ENABLE_ACCE_SYSTEM:
			del self.wndSashCombine
			del self.wndSashAbsorption
		if app.ENABLE_AURA_SYSTEM:
			del self.wndAuraAbsorption
			del self.wndAuraRefine
		del self.wndCube
		del self.wndCubeResult
		if app.ENABLE_MAIL_BOX_SYSTEM:
			del self.wndMailBox
		del self.privateShopBuilder
		del self.inputDialog
		del self.wndChatLog
		del self.dlgRefineNew
		if app.ENABLE_AUTO_HUNTING_SYSTEM:
			del self.dlgAutohunt
		del self.wndGuildBuilding
		del self.wndGameButton
		del self.tipBoard
		del self.bigBoard
		if app.ENABLE_OX_RENEWAL:
			del self.bigBoardControl
		del self.wndItemSelect
		del self.missionBoard
		if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
			if self.dlgChestDrop:
				del self.dlgChestDrop
		if app.ENABLE_GEM_SYSTEM:
			if self.wndItemSelectEx:
				del self.wndItemSelectEx
			if self.dlgGemShop:
				del self.dlgGemShop
			if self.dlgRedGemShop:
				del self.dlgRedGemShop
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				del self.wndDungeonInfo
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			if self.wndLuckyBox:
				del self.wndLuckyBox
		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		uiChat.DestroyChatInputSetWindow()

	## Self
	def SetOpenBoniSwitcherEvent(self, event):
		self.wndInventory.SetOpenBoniSwitcherEvent(event)

	## Skill
	def OnUseSkill(self, slotIndex, coolTime):
		self.wndCharacter.OnUseSkill(slotIndex, coolTime)
		self.wndTaskBar.OnUseSkill(slotIndex, coolTime)
		self.wndGuild.OnUseSkill(slotIndex, coolTime)

	def OnActivateSkill(self, slotIndex):
		self.wndCharacter.OnActivateSkill(slotIndex)
		self.wndTaskBar.OnActivateSkill(slotIndex)

	def OnDeactivateSkill(self, slotIndex):
		self.wndCharacter.OnDeactivateSkill(slotIndex)
		self.wndTaskBar.OnDeactivateSkill(slotIndex)

	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.wndTaskBar.OnChangeCurrentSkill(skillSlotNumber)

	def SelectMouseButtonEvent(self, dir, event):
		self.wndTaskBar.SelectMouseButtonEvent(dir, event)

	## Refresh
	def RefreshAlignment(self):
		self.wndCharacter.RefreshAlignment()

	def RefreshStatus(self):
		self.wndTaskBar.RefreshStatus()
		self.wndCharacter.RefreshStatus()
		self.wndInventory.RefreshStatus()
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndExtendedInventory.RefreshStatus()
		if self.wndEnergyBar:
			self.wndEnergyBar.RefreshStatus()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshStatus()
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.RefreshStatus()
		self.wndBead.SetBeadCount(0)

	def RefreshStamina(self):
		self.wndTaskBar.RefreshStamina()

	def RefreshSkill(self):
		self.wndCharacter.RefreshSkill()
		self.wndTaskBar.RefreshSkill()
		self.wndInventory.RefreshSkill()

	def RefreshInventory(self):
		self.wndTaskBar.RefreshQuickSlot()
		self.wndInventory.RefreshItemSlot()
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndExtendedInventory.RefreshItemSlot()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshItemSlot()

	def RefreshCharacter(self): ## Character 페이지의 얼굴, Inventory 페이지의 전신 그림 등의 Refresh
		self.wndCharacter.RefreshCharacter()
		self.wndTaskBar.RefreshQuickSlot()

	def RefreshQuest(self):
		self.wndCharacter.RefreshQuest()

	def RefreshSafebox(self):
		self.wndSafebox.RefreshSafebox()
		
	def DeleteItem(self, slotPos, invenType):
		self.wndInventory.DeleteItem(slotPos, invenType)

	# ITEM_MALL
	def RefreshMall(self):
		self.wndMall.RefreshMall()

	def OpenItemMall(self):
		if not self.mallPageDlg:
			self.mallPageDlg = uiShop.MallPageDialog()

		self.mallPageDlg.Open()
	# END_OF_ITEM_MALL

	def RefreshMessenger(self):
		self.wndMessenger.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.wndGuild.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.wndGuild.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.wndGuild.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.wndGuild.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.wndGuild.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.wndGuild.RefreshGuildGradePage()

	def DeleteGuild(self):
		self.wndMessenger.ClearGuildMember()
		self.wndGuild.DeleteGuild()

	def RefreshMobile(self):
		self.dlgSystem.RefreshMobile()

	def OnMobileAuthority(self):
		self.dlgSystem.OnMobileAuthority()
		
	def OnBlockMode(self, mode):
		self.dlgSystem.OnBlockMode(mode)

	## Calling Functions
	# PointReset
	def OpenPointResetDialog(self):
		self.dlgPointReset.Show()
		self.dlgPointReset.SetTop()

	def ClosePointResetDialog(self):
		self.dlgPointReset.Close()

	# Shop
	if (app.ENABLE_BATTLE_ZONE_SYSTEM):
		def OpenShopDialog(self, vid, points, curLimit, maxLimit, count):
			self.wndInventory.Show()
			if self.wndExpandedMoneyTaskbar:
				self.wndExpandedMoneyTaskbar.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid, points, curLimit, maxLimit, count)
			self.dlgShop.SetTop()
	else:
		def OpenShopDialog(self, vid):
			self.wndInventory.Show()
			if self.wndExpandedMoneyTaskbar:
				self.wndExpandedMoneyTaskbar.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid)
			self.dlgShop.SetTop()

	def CloseShopDialog(self):
		self.dlgShop.Close()

	def RefreshShopDialog(self):
		self.dlgShop.Refresh()
		
	if app.ENABLE_SHOW_CHEST_DROP_SYSTEM:
		def AddChestDropInfo(self, chestVnum, pageIndex, slotIndex, itemVnum, itemCount):
			self.dlgChestDrop.AddChestDropItem(int(chestVnum), int(pageIndex), int(slotIndex), int(itemVnum), int(itemCount))
			
		def RefreshChestDropInfo(self, chestVnum):
			self.dlgChestDrop.RefreshItems(chestVnum)
		
	## Quest
	def OpenCharacterWindowQuestPage(self):
		self.wndCharacter.Show()
		self.wndCharacter.SetState("QUEST")

	def OpenQuestWindow(self, skin, idx):
		wnds = ()

		q = uiQuest.QuestDialog(skin, idx)
		q.SetWindowName("QuestWindow" + str(idx))
		q.Show()
		if skin:
			q.Lock()
			wnds = self.__HideWindows()

			# UNKNOWN_UPDATE
			q.AddOnDoneEvent(lambda tmp_self, args=wnds: self.__ShowWindows(args))
			# END_OF_UNKNOWN_UPDATE

		if skin:
			q.AddOnCloseEvent(q.Unlock)
		q.AddOnCloseEvent(lambda key = self.wndQuestWindowNewKey:ui.__mem_func__(self.RemoveQuestDialog)(key))
		self.wndQuestWindow[self.wndQuestWindowNewKey] = q

		self.wndQuestWindowNewKey = self.wndQuestWindowNewKey + 1

		# END_OF_UNKNOWN_UPDATE
		
	def RemoveQuestDialog(self, key):
		del self.wndQuestWindow[key]
		
	def HideAllQuestWindow(self):
		tempList = []
		for i,v in self.wndQuestWindow.iteritems():
			tempList.append(v)

		for i in tempList:
			i.OnCancel()	

	## Exchange
	def StartExchange(self):
		self.dlgExchange.OpenDialog()
		self.dlgExchange.Refresh()

	def EndExchange(self):
		self.dlgExchange.CloseDialog()

	def RefreshExchange(self):
		self.dlgExchange.Refresh()
		
	if app.WJ_TRADABLE_ICON:
		def CantTradableItemExchange(self, dstSlotIndex, srcSlotIndex):
			self.dlgExchange.CantTradableItem(dstSlotIndex, srcSlotIndex)
			
	if app.ENABLE_EXCHANGE_WINDOW_SYSTEM:
		def ExchangeInfo(self, unixTime, info, isError):	
			pass
			#self.dlgExchange.AppendInformation(unixTime, info, isError)
			
	## Party
	if app.ENABLE_PARTY_UPDATE:
		def AddPartyMember(self, pid, name, mapIdx, channel):
			self.wndParty.AddPartyMember(pid, name, mapIdx, channel)
			self.__ArrangeQuestButton()
	else:
		def AddPartyMember(self, pid, name):
			self.wndParty.AddPartyMember(pid, name)
			self.__ArrangeQuestButton()

	def UpdatePartyMemberInfo(self, pid):
		self.wndParty.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.wndParty.RemovePartyMember(pid)

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	if app.ENABLE_PARTY_UPDATE:
		def LinkPartyMember(self, pid, vid, mapIdx, channel):
			self.wndParty.LinkPartyMember(pid, vid, mapIdx, channel)
	else:
		def LinkPartyMember(self, pid, vid):
			self.wndParty.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.wndParty.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.wndParty.UnlinkAllPartyMember()

	def ExitParty(self):
		self.wndParty.ExitParty()

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	def PartyHealReady(self):
		self.wndParty.PartyHealReady()

	def ChangePartyParameter(self, distributionMode):
		self.wndParty.ChangePartyParameter(distributionMode)

	## Safebox
	def AskSafeboxPassword(self):
		if self.wndSafebox.IsShow():
			return

		if self.dlgPassword.IsShow():
			self.dlgPassword.CloseDialog()
			return

		self.dlgPassword.SetSendMessage("/safebox_password ")
		#if self.dlgPassword.GetSafeboxPwd() != "":
		#	self.dlgPassword.OnAccept()
		#	return

		self.dlgPassword.SetTitle(localeInfo.PASSWORD_TITLE)
		self.dlgPassword.ShowDialog()

	def OpenSafeboxWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndSafebox.ShowWindow(size)

	def RefreshSafeboxMoney(self):
		self.wndSafebox.RefreshSafeboxMoney()

	def CommandCloseSafebox(self):
		self.wndSafebox.CommandCloseSafebox()
		
	def InitSafeboxPassword(self):
		self.dlgPassword.InitSafeboxPassword()

	# ITEM_MALL
	def AskMallPassword(self):
		if self.wndMall.IsShow():
			return

		if self.dlgPassword.IsShow():
			self.dlgPassword.CloseDialog()
			return

		self.dlgPassword.SetSendMessage("/mall_password ")
		#if self.dlgPassword.GetSafeboxPwd() != "":
			#self.dlgPassword.OnAccept()
			#return

		self.dlgPassword.SetTitle(localeInfo.MALL_PASSWORD_TITLE)
		self.dlgPassword.ShowDialog()

	def OpenMallWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndMall.ShowWindow(size)

	def CommandCloseMall(self):
		self.wndMall.CommandCloseMall()
	# END_OF_ITEM_MALL

	## Guild
	def OnStartGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnStartGuildWar(guildSelf, guildOpp)

		guildWarScoreBoard = uiGuild.GuildWarScoreBoard()
		guildWarScoreBoard.Open(guildSelf, guildOpp)
		guildWarScoreBoard.Show()
		self.guildScoreBoardDict[uiGuild.GetGVGKey(guildSelf, guildOpp)] = guildWarScoreBoard

	def OnEndGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnEndGuildWar(guildSelf, guildOpp)

		key = uiGuild.GetGVGKey(guildSelf, guildOpp)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].Destroy()
		del self.guildScoreBoardDict[key]

	# GUILDWAR_MEMBER_COUNT
	def UpdateMemberCount(self, gulidID1, memberCount1, guildID2, memberCount2):
		key = uiGuild.GetGVGKey(gulidID1, guildID2)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].UpdateMemberCount(gulidID1, memberCount1, guildID2, memberCount2)
	# END_OF_GUILDWAR_MEMBER_COUNT

	def OnRecvGuildWarPoint(self, gainGuildID, opponentGuildID, point):
		key = uiGuild.GetGVGKey(gainGuildID, opponentGuildID)
		if not self.guildScoreBoardDict.has_key(key):
			return
		
		import dbg
		guildBoard = self.guildScoreBoardDict[key]
		constInfo.GUILD_BOARD_KEY = key
		guildBoard.SetScore(gainGuildID, opponentGuildID, point)
		
	def OnRecvGuildWarSkill(self, guild_id, skillone, skilltwo, skillthree, skillfour, skillfive, skillsix):
		guildBoard = self.guildScoreBoardDict[constInfo.GUILD_BOARD_KEY]
		guildBoard.SetSkill(guild_id, skillone, skilltwo, skillthree, skillfour, skillfive, skillsix)
	## PK Mode
	def OnChangePKMode(self):
		self.wndCharacter.RefreshAlignment()
		self.dlgSystem.OnChangePKMode()

	## Refine
	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type):
		self.dlgRefineNew.Open(targetItemPos, nextGradeItemVnum, cost, prob, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.dlgRefineNew.AppendMaterial(vnum, count)
		
	## Show & Hide
	def ShowDefaultWindows(self):
		self.wndTaskBar.Show()
		self.wndMiniMap.Show()
		self.wndMiniMap.ShowMiniMap()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()
		if self.wndBead:
			self.wndBead.Show()

	def ShowAllWindows(self):
		self.wndTaskBar.Show()
		self.wndCharacter.Show()
		self.wndInventory.Show()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Show()
			self.wndDragonSoulRefine.Show()
		self.wndChat.Show()
		self.wndMiniMap.Show()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Show()
			self.wndExpandedMoneyTaskbar.SetTop()
		if self.wndBead:
			self.wndBead.Show()
		if self.wnd12ziTimer:
			self.wnd12ziTimer.Show()
		if self.wnd12ziReward:
			self.wnd12ziReward.Show()

	def HideAllWindows(self):
		if self.wndTaskBar:
			self.wndTaskBar.Hide()
		
		if self.wndEnergyBar:
			self.wndEnergyBar.Hide()

		if app.ENABLE_DETAILS_UI:
			if self.wndCharacter:
				self.wndCharacter.Close()
		else:
			if self.wndCharacter:
				self.wndCharacter.Hide()
			
		if self.wndInventory:
			self.wndInventory.Hide()
			
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndExtendedInventory:
				self.wndExtendedInventory.Hide()
				
		if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
			if (self.wndImproveChequeTicket):
				self.wndImproveChequeTicket.Hide()
			
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Hide()
			self.wndDragonSoulRefine.Hide()

		if self.wndChat:
			self.wndChat.Hide()

		if self.wndMiniMap:
			self.wndMiniMap.HideMiniMap()
			self.wndMiniMap.Hide()
			
		if self.wndMessenger:
			self.wndMessenger.Hide()

		if self.wndGuild:
			self.wndGuild.Hide()
			
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide()
 
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Hide()
			
		if self.wndBead:
			self.wndBead.Hide()
			
		if self.wnd12ziTimer:
			self.wnd12ziTimer.Hide()
			
		if self.wnd12ziReward:
			self.wnd12ziReward.Hide()
			
		if app.ENABLE_CUBE_EX_RENEWAL:
			if self.wndCubeEx:
				self.wndCubeEx.Hide()
				
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				self.wndDungeonInfo.Hide()
				
		if app.ENABLE_GUILD_RANKING_SYSTEM:
			if (self.wndHistoryGuildWarsAndRanking):
				self.wndHistoryGuildWarsAndRanking.Hide()
				
		if app.ENABLE_SPECIAL_GACHA_SYSTEM:
			if self.wndLuckyBox:
				self.wndLuckyBox.Hide()
			
	def SetBiyolog(self, bioitem, verilen, toplam, kalansure):
		self.wndBioWindow.SetBiyolog(bioitem, verilen, toplam, kalansure)
			
	def ShowMouseImage(self):
		self.wndTaskBar.ShowMouseImage()

	def HideMouseImage(self):
		self.wndTaskBar.HideMouseImage()
		
	def YeniLink(self):
		if False == self.wndChat.IsEditMode():
			self.wndChat.OpenChat()
		self.wndChat.SetChatFocus()

	def ToggleChat(self):
		if True == self.wndChat.IsEditMode():
			self.wndChat.CloseChat()
		else:
			# 웹페이지가 열렸을때는 채팅 입력이 안됨
			if self.wndWeb and self.wndWeb.IsShow():
				pass
			else:
				self.wndChat.OpenChat()

	def IsOpenChat(self):
		return self.wndChat.IsEditMode()

	def SetChatFocus(self):
		self.wndChat.SetChatFocus()
		
	def RefreshShopItemToolTip(self):
		if self.tooltipItem:
			self.tooltipItem.RefreshShopToolTip()

	def OpenRestartDialog(self):
		self.dlgRestart.OpenDialog()
		self.dlgRestart.SetTop()

	def CloseRestartDialog(self):
		self.dlgRestart.Close()

	def ToggleSystemDialog(self):
		if False == self.dlgSystem.IsShow():
			self.dlgSystem.OpenDialog()
			self.dlgSystem.SetTop()
		else:
			self.dlgSystem.Close()
			
	def OpenSystemDialog(self):
		self.dlgSystem.OpenDialog()
		self.dlgSystem.SetTop()

	def ToggleMessenger(self):
		if self.wndMessenger.IsShow():
			self.wndMessenger.Hide()
		else:
			self.wndMessenger.SetTop()
			self.wndMessenger.Show()

	def ToggleMiniMap(self):
		if app.IsPressed(app.DIK_LSHIFT) or app.IsPressed(app.DIK_RSHIFT):
			if False == self.wndMiniMap.isShowMiniMap():
				self.wndMiniMap.ShowMiniMap()
				self.wndMiniMap.SetTop()
			else:
				self.wndMiniMap.HideMiniMap()

		else:
			self.wndMiniMap.ToggleAtlasWindow()

	def PressMKey(self):
		if app.IsPressed(app.DIK_LALT) or app.IsPressed(app.DIK_RALT):
			self.ToggleMessenger()

		else:
			self.ToggleMiniMap()

	def SetMapName(self, mapName):
		self.wndMiniMap.SetMapName(mapName)

	def MiniMapScaleUp(self):
		self.wndMiniMap.ScaleUp()

	def MiniMapScaleDown(self):
		self.wndMiniMap.ScaleDown()

	def ToggleCharacterWindow(self, state):
		if False == player.IsObserverMode():
			if False == self.wndCharacter.IsShow():
				self.OpenCharacterWindowWithState(state)
			else:
				if state == self.wndCharacter.GetState():
					self.wndCharacter.OverOutItem()
					if app.ENABLE_DETAILS_UI:
						self.wndCharacter.Close()
					else:
						self.wndCharacter.Hide()
				else:
					self.wndCharacter.SetState(state)

	def OpenCharacterWindowWithState(self, state):
		if False == player.IsObserverMode():
			self.wndCharacter.SetState(state)
			self.wndCharacter.Show()
			self.wndCharacter.SetTop()

	def ToggleCharacterWindowStatusPage(self):
		self.ToggleCharacterWindow("STATUS")

	def TogglePetMain(self):
		net.SendChatPacket("/gift")		
				
	def ToggleInventoryWindow(self):
		if False == player.IsObserverMode():
			if False == self.wndInventory.IsShow():
				self.wndInventory.Show()
				self.wndInventory.SetTop()
				if not self.wndExpandedMoneyTaskbar.IsShow():
					self.wndExpandedMoneyTaskbar.Show()
				if constInfo.KInventorymode == 1:
					if not self.wndExtendedInventory.IsShow():
						self.wndExtendedInventory.Show()
			else:
				self.wndInventory.OverOutItem()
				self.wndInventory.Close()
				if self.wndExpandedMoneyTaskbar.IsShow():
					self.wndExpandedMoneyTaskbar.Hide()
				if constInfo.KInventorymode == 1:
					if self.wndExtendedInventory.IsShow():
						self.wndExtendedInventory.OverOutItem()
						self.wndExtendedInventory.Close()	
					
	if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
		def ToggleExtendedInventoryWindow(self):	
			if False == player.IsObserverMode():
				if self.wndExtendedInventory.IsShow():
					self.wndExtendedInventory.OverOutItem()
					self.wndExtendedInventory.Close()
				else:
					self.wndExtendedInventory.Show()
	
	def ToggleExpandedButton(self):
		if False == player.IsObserverMode():
			if False == self.wndExpandedTaskBar.IsShow():
				self.wndExpandedTaskBar.Show()
				self.wndExpandedTaskBar.SetTop()
			else:
				self.wndExpandedTaskBar.Close()
				
	def ToggleExpandedMoneyButton(self):
		if False == player.IsObserverMode():
			if False == self.wndExpandedMoneyTaskbar.IsShow():
				self.wndExpandedMoneyTaskbar.Show()
				self.wndExpandedMoneyTaskbar.SetTop()
			else:
				self.wndExpandedMoneyTaskbar.Close()
	
	# 용혼석
	def DragonSoulActivate(self, deck):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.ActivateDragonSoulByExtern(deck)

	def DragonSoulDeactivate(self):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.DeactivateDragonSoul()
		
	def Highligt_Item(self, inven_type, inven_pos):
		if not app.ENABLE_ITEM_HIGHLIGHT_SYSTEM:
			if player.DRAGON_SOUL_INVENTORY == inven_type:
				if app.ENABLE_DRAGON_SOUL_SYSTEM:
					self.wndDragonSoul.HighlightSlot(inven_pos)
		else:
			if inven_type == player.INVENTORY:
				self.wndInventory.HighlightSlot(inven_pos)
			elif inven_type == player.DRAGON_SOUL_INVENTORY:
				if app.ENABLE_DRAGON_SOUL_SYSTEM:
					self.wndDragonSoul.HighlightSlot(inven_pos)
			elif app.ENABLE_SPECIAL_INVENTORY_SYSTEM and (inven_type == player.SKILL_BOOK_INVENTORY or inven_type == player.UPGRADE_ITEMS_INVENTORY or inven_type == player.STONE_ITEMS_INVENTORY or inven_type == player.CHEST_ITEMS_INVENTORY or inven_type == player.ATTR_ITEMS_INVENTORY or inven_type == player.FLOWER_ITEMS_INVENTORY):
				self.wndExtendedInventory.HighlightSlot(inven_pos, inven_type)
			
	def DragonSoulGiveQuilification(self):
		self.DRAGON_SOUL_IS_QUALIFIED = True
		self.wndExpandedTaskBar.SetToolTipText(uiTaskbar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, uiScriptLocale.TASKBAR_DRAGON_SOUL)

	def ToggleDragonSoulWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
					else:
						try:
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
						except:
							self.wndPopupDialog = uiCommon.PopupDialog()
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
				else:
					self.wndDragonSoul.Close()
		
	def ToggleDragonSoulWindowWithNoInfo(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
				else:
					self.wndDragonSoul.Close()
					
	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def ToggleDungeonInfoWindow(self):
			if False == player.IsObserverMode():
				if False == self.wndDungeonInfo.IsShow():
					self.wndDungeonInfo.Open()
				else:
					self.wndDungeonInfo.Close()
				
	def FailDragonSoulRefine(self, reason, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineFail(reason, inven_type, inven_pos)
 
	def SucceedDragonSoulRefine(self, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineSucceed(inven_type, inven_pos)
 
	def OpenDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Show()
					if None != self.wndDragonSoul:
						if False == self.wndDragonSoul.IsShow():
							self.wndDragonSoul.Show()

	def CloseDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Close()
					
	if app.ENABLE_YOUTUBER_SYSTEM:
		def ToggleYoutubeWindow(self, url):
			import uiyoutube
			YoutubePanel = uiyoutube.WebWindow()
			YoutubePanel.Open(url)
			self.YoutubePanel = YoutubePanel
	
	def ToggleGuildWindow(self):
		if not self.wndGuild.IsShow():
			if self.wndGuild.CanOpen():
				self.wndGuild.Open()
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GUILD_YOU_DO_NOT_JOIN)
		else:
			self.wndGuild.OverOutItem()
			self.wndGuild.Hide()

	def ToggleChatLogWindow(self):
		if self.wndChatLog.IsShow():
			self.wndChatLog.Hide()
		else:
			self.wndChatLog.Show()

	def CheckGameButton(self):
		if self.wndGameButton:
			self.wndGameButton.CheckGameButton()

	def __OnClickStatusPlusButton(self):
		self.ToggleCharacterWindow("STATUS")

	def __OnClickSkillPlusButton(self):
		self.ToggleCharacterWindow("SKILL")

	def __OnClickQuestButton(self):
		self.ToggleCharacterWindow("QUEST")

	def __OnClickHelpButton(self):
		player.SetPlayTime(1)
		self.CheckGameButton()
		self.OpenHelpWindow()

	def __OnClickBuildButton(self):
		self.BUILD_OpenWindow()

	def OpenHelpWindow(self):
		self.wndUICurtain.Show()
		self.wndHelp.Open()

	def CloseHelpWindow(self):
		self.wndUICurtain.Hide()
		self.wndHelp.Close()
			
	if app.ENABLE_MELEY_LAIR_DUNGEON:
		def OpenMeleyRanking(self):
			self.wndMeleyRanking.Open()

		def RankMeleyRanking(self, line, name, members, time):
			self.wndMeleyRanking.AddRank(line, name, members, time)
			
	def OpenWebWindow(self, url):
		self.wndWeb.Open(url)

		# 웹페이지를 열면 채팅을 닫는다
		self.wndChat.CloseChat()
		
	if app.ENABLE_CHANGELOOK_SYSTEM:
		if app.ENABLE_MOUNT_CHANGELOOK_SYSTEM:
			def ActChangeLook(self, iAct, bMount):
				if iAct == 1:
					if not self.wndChangeLook.IsOpened():
						self.wndChangeLook.Open(bMount)
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
					
					self.wndInventory.RefreshBagSlotWindow()
				elif iAct == 2:
					if self.wndChangeLook.IsOpened():
						self.wndChangeLook.Close()
					
					self.wndInventory.RefreshBagSlotWindow()
				elif iAct == 3 or iAct == 4:
					if self.wndChangeLook.IsOpened():
						self.wndChangeLook.Refresh()
					
					self.wndInventory.RefreshBagSlotWindow()
		else:
			def ActChangeLook(self, iAct):
				if iAct == 1:
					if not self.wndChangeLook.IsOpened():
						self.wndChangeLook.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
					
					self.wndInventory.RefreshBagSlotWindow()
				elif iAct == 2:
					if self.wndChangeLook.IsOpened():
						self.wndChangeLook.Close()
					
					self.wndInventory.RefreshBagSlotWindow()
				elif iAct == 3 or iAct == 4:
					if self.wndChangeLook.IsOpened():
						self.wndChangeLook.Refresh()
					
					self.wndInventory.RefreshBagSlotWindow()

	if app.ENABLE_ITEM_COMBINATION_SYSTEM:
		def OpenItemCombinationWindow(self):
			self.wndItemCombination.Open()
			if self.wndInventory.IsShow() == False:
				self.wndInventory.Show()

		def SucceedItemCombinationWork(self):
			self.wndItemCombination.Clear()

		def OpenSkillBookCombinationWindow(self):
			self.wndSkillBookCombination.Open()
			if self.wndInventory.IsShow() == False:
				self.wndInventory.Show()

		def SucceedSkillBookCombinationWork(self):
			self.wndSkillBookCombination.Clear()

		def CloseCombinationWindow(self):
			if self.wndItemCombination.IsShow() == True:
				self.wndItemCombination.Close()

			elif self.wndSkillBookCombination.IsShow() == True:
				self.wndSkillBookCombination.Close()
		
	if app.ENABLE_YOUTUBER_SYSTEM:
		def OpenYoutubeWebWindow(self, url):
			self.wndYoutubeWeb.Open(url)
			self.wndChat.CloseChat()

		def YoutubeLink(self):
			self.wndyoutuber.Show()

		def YeniLink(self):
			if False == self.wndChat.IsEditMode():
				self.wndChat.OpenChat()
			self.wndChat.SetChatFocus()

	# show GIFT
	def ShowGift(self):
		self.wndTaskBar.ShowGift()

	def CloseWbWindow(self):
		self.wndWeb.Close()

	if app.ENABLE_YOUTUBER_SYSTEM:
		def CloseYoutubeWebWindow(self):
			self.wndYoutubeWeb.Close()
		
	def OpenCardsInfoWindow(self):
		self.wndCardsInfo.Open()
		
	def OpenCardsWindow(self, safemode):
		self.wndCards.Open(safemode)
		
	def UpdateCardsInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		self.wndCards.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)
		
	def UpdateCardsFieldInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsPutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsShowIcon(self):
		self.wndCardsIcon.Open()
		
	if app.ENABLE_SHOP_SEARCH_SYSTEM:
		def OpenPrivateShopSearch(self, type):
			self.wndPrivateShopSearch.Open(type)
			
		def RefreshShopSearch(self):
			self.wndPrivateShopSearch.RefreshMe()
			self.wndPrivateShopSearch.RefreshList()	
		
		def RefreshRequest(self):
			self.wndPrivateShopSearch.RefreshRequest()	
			
	if app.ENABLE_EVENT_INFORMATION_SYSTEM:
		def ShowEventWindowDialog(self):
			self.wndEvent.Refresh()
			self.wndEvent.OpenEventInfoDialog()
			
		def RefreshEventWindowDialog(self):
			self.wndEvent.Refresh()
			
	if app.ENABLE_ACCE_SYSTEM:
		def ActSash(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndSashCombine.IsOpened():
						self.wndSashCombine.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Close()
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Close()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Refresh(iAct)
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Refresh(iAct)
				
				self.wndInventory.RefreshBagSlotWindow()
				
	if app.ENABLE_AURA_SYSTEM:
		def ActAura(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Close()
				else:
					if self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Close()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Refresh(iAct)
				else:
					if self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Refresh(iAct)
				
				self.wndInventory.RefreshBagSlotWindow()
		
	def OpenCubeWindow(self):
		self.wndCube.Open()

		if False == self.wndInventory.IsShow():
			self.wndInventory.Show()	
				
	def UpdateCubeInfo(self, gold, itemVnum, count):
		self.wndCube.UpdateInfo(gold, itemVnum, count)

	def CloseCubeWindow(self):
		self.wndCube.Close()

	def FailedCubeWork(self):
		self.wndCube.Refresh()

	def SucceedCubeWork(self, itemVnum, count):
		self.wndCube.Clear()
		
		print "큐브 제작 성공! [%d:%d]" % (itemVnum, count)

		if 0: # 결과 메시지 출력은 생략 한다
			self.wndCubeResult.SetPosition(*self.wndCube.GetGlobalPosition())
			self.wndCubeResult.SetCubeResultItem(itemVnum, count)
			self.wndCubeResult.Open()
			self.wndCubeResult.SetTop()
			
	def __HideWindows(self):
		hideWindows = self.wndTaskBar,\
						self.wndCharacter,\
						self.wndInventory,\
						self.wndMiniMap,\
						self.wndGuild,\
						self.wndMessenger,\
						self.wndChat,\
						self.wndParty,\
						self.wndGameButton,

		if self.wndEnergyBar:
			hideWindows += self.wndEnergyBar,
			
		if self.wndExpandedTaskBar:
			hideWindows += self.wndExpandedTaskBar,
			
		if self.wndExpandedMoneyTaskbar:
			hideWindows += self.wndExpandedMoneyTaskbar,
			
		if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
			if (self.wndImproveChequeTicket):
				hideWindows += self.wndImproveChequeTicket,
			
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			hideWindows += self.wndDragonSoul,\
						self.wndDragonSoulRefine,
						
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndExtendedInventory:
				hideWindows += self.wndExtendedInventory,
				
		if app.ENABLE_GUILD_RANKING_SYSTEM:
			if (self.wndHistoryGuildWarsAndRanking):
				hideWindows += self.wndHistoryGuildWarsAndRanking,
				
		if self.wndBead:
			hideWindows += self.wndBead,
			
		if self.wnd12ziReward:
			hideWindows += self.wnd12ziReward,
			
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				hideWindows += self.wndDungeonInfo,
			
		hideWindows = filter(lambda x:x.IsShow(), hideWindows)
		map(lambda x:x.Hide(), hideWindows)
		import sys

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		if self.wndChat.IsEditMode():
			self.wndChat.CloseChat()

		return hideWindows

	def __ShowWindows(self, wnds):
		import sys
		map(lambda x:x.Show(), wnds)
		global IsQBHide
		if not IsQBHide:
			self.ShowAllQuestButton()
		else:
			self.HideAllQuestButton()

		self.ShowAllWhisperButton()

	def BINARY_OpenAtlasWindow(self):
		if self.wndMiniMap:
			self.wndMiniMap.ShowAtlas()

	def BINARY_SetObserverMode(self, flag):
		self.wndGameButton.SetObserverMode(flag)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def BINARY_OpenSelectItemWindow(self):
		self.wndItemSelect.Open()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
	
	def OpenInputNameDialogPet(self, slot):
		inputDialog = uiCommon.InputDialogName()
		inputDialog.SetTitle("Isim Degistir")
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.ChangePetName))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
		inputDialog.Open()
		inputDialog.slot = slot	
		self.inputDialog = inputDialog	
		
	def ChangePetName(self):
		net.SendChatPacket("/pet_change_name {0} {1}".format(str(self.inputDialog.slot), str(self.inputDialog.GetText())))
		self.ClosePrivateShopInputNameDialog()

	#####################################################################################
	### Private Shop ###

	def OpenPrivateShopInputNameDialog(self):
		#if player.IsInSafeArea():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CANNOT_OPEN_PRIVATE_SHOP_IN_SAFE_AREA)
		#	return
	
		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_NAME_DIALOG_TITLE)
		inputDialog.SetMaxLength(32)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenPrivateShopBuilder))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog
	
	def ClosePrivateShopInputNameDialog(self):
		self.inputDialog = None
		return True
	
	def OpenPrivateShopBuilder(self):
	
		if not self.inputDialog:
			return True
	
		if not len(self.inputDialog.GetText()):
			return True
	
		self.privateShopBuilder.Open(self.inputDialog.GetText())
		self.ClosePrivateShopInputNameDialog()
		return True

	def AppearPrivateShop(self, vid, text):
		board = uiPrivateShopBuilder.PrivateShopAdvertisementBoard()
		board.Open(vid, text)

		self.privateShopAdvertisementBoardDict[vid] = board

	def DisappearPrivateShop(self, vid):

		if not self.privateShopAdvertisementBoardDict.has_key(vid):
			return

		del self.privateShopAdvertisementBoardDict[vid]
		uiPrivateShopBuilder.DeleteADBoard(vid)
		
	#####################################################################################
	### Equipment ###

	if app.ENABLE_VIEW_EQUIPMENT_SYSTEM:
		def OpenEquipmentDialog(self, vid):
			if app.ENABLE_PVP_ADVANCED:
				if self.equipmentDialogDict.has_key(vid):
					self.equipmentDialogDict[vid].Destroy()
					self.CloseEquipmentDialog(vid)
				
			dlg = uiEquipmentDialog.EquipmentDialog()
			dlg.SetItemToolTip(self.tooltipItem)
			dlg.SetCloseEvent(ui.__mem_func__(self.CloseEquipmentDialog))
			dlg.Open(vid)

			self.equipmentDialogDict[vid] = dlg

		def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count, sealbind, transmutation):
			if not vid in self.equipmentDialogDict:
				return
			self.equipmentDialogDict[vid].SetEquipmentDialogItem(slotIndex, vnum, count, sealbind, transmutation)

		def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
			if not vid in self.equipmentDialogDict:
				return
			self.equipmentDialogDict[vid].SetEquipmentDialogSocket(slotIndex, socketIndex, value)

		def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
			if not vid in self.equipmentDialogDict:
				return
			self.equipmentDialogDict[vid].SetEquipmentDialogAttr(slotIndex, attrIndex, type, value)
			
		def CloseEquipmentDialog(self, vid):
			if not vid in self.equipmentDialogDict:
				return
				
			if app.ENABLE_PVP_ADVANCED:
				if self.equipmentDialogDict.has_key(vid):
					self.equipmentDialogDict[vid].Destroy()
				
			del self.equipmentDialogDict[vid]

	#####################################################################################

	#####################################################################################
	### Quest ###	
	def BINARY_ClearQuest(self, index):
		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)		
	
	def RecvQuest(self, index, name):
		# QUEST_LETTER_IMAGE
		self.BINARY_RecvQuest(index, name, "file", localeInfo.GetLetterImageName())
		# END_OF_QUEST_LETTER_IMAGE

	def BINARY_RecvQuest(self, index, name, iconType, iconName):

		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)

		btn = uiWhisper.WhisperButton()

		# QUEST_LETTER_IMAGE
		##!! 20061026.levites.퀘스트_이미지_교체
		if "item"==iconType:
			item.SelectItem(int(iconName))
			buttonImageFileName=item.GetIconImageFileName()
		else:
			buttonImageFileName=iconName

		if name[0] == '*':
			btn.SetUpVisual("locale/tr/icon/scroll_close_blue.tga")
			btn.SetOverVisual("locale/tr/icon/scroll_open_blue.tga")
			btn.SetDownVisual("locale/tr/icon/scroll_open_blue.tga")
			name = name[1:]
		elif name[0] == '~':
			btn.SetUpVisual("locale/tr/icon/scroll_close_golden.tga")
			btn.SetOverVisual("locale/tr/icon/scroll_open_golden.tga")
			btn.SetDownVisual("locale/tr/icon/scroll_open_golden.tga")
			name = name[1:]
		elif name[0] == '&':
			btn.SetUpVisual("locale/tr/icon/scroll_close_green.tga")
			btn.SetOverVisual("locale/tr/icon/scroll_open_green.tga")
			btn.SetDownVisual("locale/tr/icon/scroll_open_green.tga")
			name = name[1:]
		elif name[0] == '+':
			btn.SetUpVisual("locale/tr/icon/scroll_close_purple.tga")
			btn.SetOverVisual("locale/tr/icon/scroll_open_purple.tga")
			btn.SetDownVisual("locale/tr/icon/scroll_open_purple.tga")
			name = name[1:]
		else:
			btn.SetUpVisual(localeInfo.GetLetterCloseImageName())
			btn.SetOverVisual(localeInfo.GetLetterOpenImageName())
			btn.SetDownVisual(localeInfo.GetLetterOpenImageName())
		# END_OF_QUEST_LETTER_IMAGE

		btn.SetToolTipText(name, -20, 35)
		btn.ToolTipText.SetHorizontalAlignLeft()
			
		btn.SetEvent(ui.__mem_func__(self.__StartQuest), btn)
		btn.Show()

		btn.index = index
		btn.name = name

		self.questButtonList.insert(0, btn)
		self.__ArrangeQuestButton()

		#chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.QUEST_APPEND)

	def __ArrangeQuestButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		##!! 20061026.levites.퀘스트_위치_보정
		if self.wndParty.IsShow():
			xPos = 100 + 30
		else:
			xPos = 20

		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63

		count = 0
		for btn in self.questButtonList:

			btn.SetPosition(xPos + (int(count/yCount) * 100), yPos + (count%yCount * 63))
			count += 1
			global IsQBHide
			if IsQBHide:
				btn.Hide()
			else:
				btn.Show()

	def __StartQuest(self, btn):
		event.QuestButtonClick(btn.index)
		self.__DestroyQuestButton(btn)

	def __FindQuestButton(self, index):
		for btn in self.questButtonList:
			if btn.index == index:
				return btn

		return 0

	def __DestroyQuestButton(self, btn):
		btn.SetEvent(0)
		self.questButtonList.remove(btn)
		self.__ArrangeQuestButton()

	def HideAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Hide()

	def ShowAllQuestButton(self):		
		for btn in self.questButtonList:
			btn.Show()
	#####################################################################################

	#####################################################################################
	### Whisper ###

	def __InitWhisper(self):
		chat.InitFloatingBoxes(self)

	## 채팅창의 "메시지 보내기"를 눌렀을때 이름 없는 대화창을 여는 함수
	## 이름이 없기 때문에 기존의 WhisperDialogDict 와 별도로 관리된다.
	def OpenWhisperDialogWithoutTarget(self):
		if not self.dlgWhisperWithoutTarget:
			dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
			dlgWhisper.BindInterface(self)
			dlgWhisper.LoadDialog()
			dlgWhisper.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)
			dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
			dlgWhisper.Show()
			self.dlgWhisperWithoutTarget = dlgWhisper

			self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		else:
			self.dlgWhisperWithoutTarget.SetTop()
			self.dlgWhisperWithoutTarget.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)

	## 이름 없는 대화창에서 이름을 결정했을때 WhisperDialogDict에 창을 넣어주는 함수
	def RegisterTemporaryWhisperDialog(self, name):
		if not self.dlgWhisperWithoutTarget:
			return

		btn = self.__FindWhisperButton(name)
		if 0 != btn:
			self.__DestroyWhisperButton(btn)

		elif self.whisperDialogDict.has_key(name):
			oldDialog = self.whisperDialogDict[name]
			oldDialog.Destroy()
			del self.whisperDialogDict[name]

		self.whisperDialogDict[name] = self.dlgWhisperWithoutTarget
		self.dlgWhisperWithoutTarget.OpenWithTarget(name)
		self.dlgWhisperWithoutTarget = None
		self.__CheckGameMaster(name)

	def OpenWhisperDialog(self, name):
		if not self.whisperDialogDict.has_key(name):
			dlg = self.__MakeWhisperDialog(name)
			dlg.OpenWithTarget(name)
			dlg.chatLine.SetFocus()
			dlg.Show()

			self.__CheckGameMaster(name)
			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

	## 다른 캐릭터로부터 메세지를 받았을때 일단 버튼만 띄워 두는 함수			
	def RecvWhisper(self, name):
		if not self.whisperDialogDict.has_key(name):
			btn = self.__FindWhisperButton(name)
			if 0 == btn:
				btn = self.__MakeWhisperButton(name)
				btn.Flash()
				if app.ENABLE_FLASH_APPLICATION:
					app.FlashApplication()
				chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))

			else:
				if app.ENABLE_FLASH_APPLICATION:
					btn.Flash()
				app.FlashApplication()
		elif self.IsGameMasterName(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def MakeWhisperButton(self, name):
		self.__MakeWhisperButton(name)
		
	def ShowWhisperDialog(self, btn):
		try:
			self.__MakeWhisperDialog(btn.name)
			dlgWhisper = self.whisperDialogDict[btn.name]
			dlgWhisper.OpenWithTarget(btn.name)
			dlgWhisper.Show()
			self.__CheckGameMaster(btn.name)
		except:
			import dbg, sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("interface.ShowWhisperDialog - Failed to find key | %s:%s" % (type, msg))

		## 버튼 초기화
		self.__DestroyWhisperButton(btn)

	def MinimizeWhisperDialog(self, name):
		if 0 != name:
			self.__MakeWhisperButton(name)
		self.CloseWhisperDialog(name)
		
	def BilgileriEkle(self, name, empire, level, channel):
		try:
			dlgWhisper = self.whisperDialogDict[name]
			dlgWhisper.SetToolTipForBilgi(name, empire, level, channel)
		except:
			import dbg, sys
			(type, msg, tb)=sys.exc_info()
			dbg.TraceError("interface.BilgileriEkle - Failed to find key | %s:%s" % (type, msg))

	## WhisperDialog 창에서 닫기 명령을 수행했을때 호출되는 함수
	## 창을 지웁니다.
	def CloseWhisperDialog(self, name):

		if 0 == name:

			if self.dlgWhisperWithoutTarget:
				self.dlgWhisperWithoutTarget.Destroy()
				self.dlgWhisperWithoutTarget = None

			return

		try:
			dlgWhisper = self.whisperDialogDict[name]
			dlgWhisper.Destroy()
			del self.whisperDialogDict[name]
		except:
			import dbg
			dbg.TraceError("interface.CloseWhisperDialog - Failed to find key")

	## 버튼의 개수가 바뀌었을때 버튼을 재정렬 하는 함수
	def __ArrangeWhisperButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		xPos = screenWidth - 70
		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63
		#yCount = (screenHeight - 285) / 63

		count = 0
		for button in self.whisperButtonList:

			button.SetPosition(xPos + (int(count/yCount) * -50), yPos + (count%yCount * 63))
			count += 1

	## 이름으로 Whisper 버튼을 찾아 리턴해 주는 함수
	## 버튼은 딕셔너리로 하지 않는 것은 정렬 되어 버려 순서가 유지 되지 않으며
	## 이로 인해 ToolTip들이 다른 버튼들에 의해 가려지기 때문이다.
	def __FindWhisperButton(self, name):
		for button in self.whisperButtonList:
			if button.name == name:
				return button

		return 0

	## 창을 만듭니다.
	def __MakeWhisperDialog(self, name):
		dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
		dlgWhisper.BindInterface(self)
		dlgWhisper.LoadDialog()
		dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
		self.whisperDialogDict[name] = dlgWhisper

		self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		return dlgWhisper

	def __MakeWhisperButton(self, name):
		if "$" in name:
			return	
		whisperButton = uiWhisper.WhisperButton()
		whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		if self.IsGameMasterName(name):
			whisperButton.SetToolTipTextWithColor(name, 0xffffa200)
		else:
			whisperButton.SetToolTipText(name)
		whisperButton.ToolTipText.SetHorizontalAlignCenter()
		whisperButton.SetEvent(ui.__mem_func__(self.ShowWhisperDialog), whisperButton)
		whisperButton.Show()
		whisperButton.name = name
		self.whisperButtonList.insert(0, whisperButton)
		self.__ArrangeWhisperButton()
		return whisperButton

	def __DestroyWhisperButton(self, button):
		button.SetEvent(0)
		self.whisperButtonList.remove(button)
		self.__ArrangeWhisperButton()

	def HideAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Hide()

	def ShowAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Show()

	def __CheckGameMaster(self, name):
		if not self.listGMName.has_key(name):
			return
		if self.whisperDialogDict.has_key(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def RegisterGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return
		self.listGMName[name] = "GM"

	def IsGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return True
		else:
			return False

	#####################################################################################

	#####################################################################################
	### Guild Building ###

	def BUILD_OpenWindow(self):
		self.wndGuildBuilding = uiGuild.BuildGuildBuildingWindow()
		self.wndGuildBuilding.Open()
		self.wndGuildBuilding.wnds = self.__HideWindows()
		self.wndGuildBuilding.SetCloseEvent(ui.__mem_func__(self.BUILD_CloseWindow))

	def BUILD_CloseWindow(self):
		self.__ShowWindows(self.wndGuildBuilding.wnds)
		self.wndGuildBuilding = None

	def BUILD_OnUpdate(self):
		if not self.wndGuildBuilding:
			return

		if self.wndGuildBuilding.IsPositioningMode():
			import background
			x, y, z = background.GetPickingPoint()
			self.wndGuildBuilding.SetBuildingPosition(x, y, z)

	def BUILD_OnMouseLeftButtonDown(self):
		if not self.wndGuildBuilding:
			return

		# GUILD_BUILDING
		if self.wndGuildBuilding.IsPositioningMode():
			self.wndGuildBuilding.SettleCurrentPosition()
			return True
		elif self.wndGuildBuilding.IsPreviewMode():
			pass
		else:
			return True
		# END_OF_GUILD_BUILDING
		return False

	def BUILD_OnMouseLeftButtonUp(self):
		if not self.wndGuildBuilding:
			return

		if not self.wndGuildBuilding.IsPreviewMode():
			return True

		return False

	def BULID_EnterGuildArea(self, areaID):
		# GUILD_BUILDING
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()

		if mainCharacterName != masterName:
			return

		if areaID != player.GetGuildID():
			return
		# END_OF_GUILD_BUILDING

		self.wndGameButton.ShowBuildButton()

	def BULID_ExitGuildArea(self, areaID):
		self.wndGameButton.HideBuildButton()


	#####################################################################################

	def IsEditLineFocus(self):
		if self.ChatWindow.chatLine.IsFocus():
			return 1

		if self.ChatWindow.chatToLine.IsFocus():
			return 1

		return 0

	def EmptyFunction(self):
		pass
		
	def GetInventoryPageIndex(self):
		if self.wndInventory:
			return self.wndInventory.GetInventoryPageIndex()
		else:
			return -1
			
	def GetInventoryHandle(self):
		if self.wndInventory:
			return self.wndInventory
		else:
			return -1

	if app.WJ_TRADABLE_ICON:
		def SetOnTopWindow(self, onTopWnd):
			self.onTopWindow = onTopWnd

		def GetOnTopWindow(self):
			return self.onTopWindow
			
		def MarkUnusableInvenSlotOnTopWnd(self, onTopWnd, index):
			if player.ON_TOP_WND_SHOP == onTopWnd:
				if self.dlgShop.CantSellInvenItem(index):
					return True
			elif player.ON_TOP_WND_SAFEBOX == onTopWnd:
				if self.wndSafebox.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_PET_FEED == onTopWnd:
				if self.game.petmain.feedwind.CantAddItem(index):
					return True
			elif player.ON_TOP_WND_ACCE_COMBINE == onTopWnd:
				if self.wndSashCombine.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_ACCE_ABSORB == onTopWnd:
				if self.wndSashAbsorption.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_CHANGE_LOOK == onTopWnd:
				if self.wndChangeLook.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_COSTUME_COMBINATION == onTopWnd:
				if self.wndItemCombination.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_BOOK_COMBINATION == onTopWnd:
				if self.wndSkillBookCombination.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_AURA == onTopWnd:
				if self.wndAuraAbsorption.CantCheckInItem(index):
					return True
			elif player.ON_TOP_WND_AURA_REFINE == onTopWnd:
				if self.wndAuraRefine.CantCheckInItem(index):
					return True
					
			return False

		def RefreshMarkInventoryBag(self):
			if self.wndInventory:
				if self.wndInventory.IsShow():
					self.wndInventory.RefreshBagSlotWindow()
			
	if app.ENABLE_FAST_ATTACH_ITEMS_SYSTEM:
		def AttachItemFromSafebox(self, slotIndex, itemIndex):
			if self.wndInventory and self.wndInventory.IsShow():
				self.wndInventory.AttachItemFromSafebox(slotIndex, itemIndex)

			return True

		def AttachInvenItemToOtherWindowSlot(self, slotWindow, slotIndex):
			if self.wndSafebox and self.wndSafebox.IsShow():
				return self.wndSafebox.AttachItemFromInventory(slotWindow, slotIndex)

			#if self.dlgExchange and self.dlgExchange.IsShow():
			#	return self.dlgExchange.AttachItemFromInventory(slotWindow, slotIndex)

			return False
			
	if app.ENABLE_EXTEND_INVENTORY_SYSTEM:		
		def ExInvenItemUseMsg(self, current_stage, need_left, need_count):
			self.wndInventory.ExInvenItemUseMsg(current_stage, need_left, need_count)
			
	def SetBeadCount(self, value):
		if self.wndBead:
			self.wndBead.SetBeadCount(value)
		
	def NextBeadUpdateTime(self, value):
		if self.wndBead:
			self.wndBead.NextBeadUpdateTime(value)
			
	if app.ENABLE_FISH_JIGSAW_EVENT:
		def MiniGameFishUse(self, shape, useCount):
			if self.wndEvent:
				self.wndEvent.MiniGameFishUse(shape, useCount)
			
		def MiniGameFishAdd(self, pos, shape):
			if self.wndEvent:
				self.wndEvent.MiniGameFishAdd(pos, shape)
			
		def MiniGameFishReward(self, vnum):
			if self.wndEvent:
				self.wndEvent.MiniGameFishReward(vnum)
				
		def MiniGameFishCount(self, count):
			if self.wndEvent:
				self.wndEvent.MiniGameFishCount(count)

		def SetFishEventStatus(self, isEnable):
			if self.wndEvent:
				self.wndEvent.IntegrationMiniGame()

	if app.ENABLE_MINI_GAME:
		if app.ENABLE_MINI_GAME_CATCH_KING:					
			def MiniGameCatchKingEventStart(self, bigScore):
				if self.wndEvent:
					self.wndEvent.MiniGameCatchKingEventStart(bigScore)
					
			def MiniGameCatchKingSetHandCard(self, cardNumber):
				if self.wndEvent:
					self.wndEvent.MiniGameCatchKingSetHandCard(cardNumber)

			def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
				if self.wndEvent:
					self.wndEvent.MiniGameCatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)
					
			def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
				if self.wndEvent:
					self.wndEvent.MiniGameCatchKingSetEndCard(cardPos, cardNumber)
					
			def MiniGameCatchKingReward(self, rewardCode):
				if self.wndEvent:
					self.wndEvent.MiniGameCatchKingReward(rewardCode)

	if app.ENABLE_ATTENDANCE_EVENT:
		def MiniGameAttendanceSetData(self, type, value):
			if self.wndEvent:
				self.wndEvent.MiniGameAttendanceSetData(type, value)
				
		def SetAttendanceEventStatus(self, isEnable):
			if self.wndEvent:
				self.wndEvent.IntegrationMiniGame()
					
	def NextBeadUpdateTime(self, value):
		if self.wndBead:
			self.wndBead.NextBeadUpdateTime(value)

	if app.ENABLE_GEM_SYSTEM:
		if app.ENABLE_BOSS_GEM_SYSTEM:
			def OpenGemShop(self, gemShopType):
				if gemShopType == True:
					self.dlgRedGemShop.Open()
				else:
					self.dlgGemShop.Open()
				
			def CloseGemShop(self):
				self.dlgGemShop.Close()
					
			def RefreshGemShop(self, gemShopType):
				if gemShopType == True:
					self.dlgRedGemShop.RefreshGemShop()
				else:
					self.dlgGemShop.RefreshGemShop()
		else:
			def OpenGemShop(self):
				self.dlgGemShop.Open()
				
			def CloseGemShop(self):
				self.dlgGemShop.Close()
					
			def RefreshGemShop(self):
				self.dlgGemShop.RefreshGemShop()
			
		def BINARY_OpenSelectItemExWindow(self):
			self.wndItemSelectEx.Open()
			
	def CheckRefineDialog(self, isFail):
		self.dlgRefineNew.CheckRefine(isFail)
		
	def OpenUI12zi(self, yellowmark, greenmark, yellowreward, greenreward, goldreward):
		if self.wnd12ziReward == None:
			self.wnd12ziReward = ui12zi.Reward12ziWindow()
		self.wnd12ziReward.Open(int(yellowmark), int(greenmark), int(yellowreward), int(greenreward), int(goldreward))
			
	if app.ENABLE_DEFENSE_WAVE:
		def BINARY_Update_Mast_HP(self, hp, max):
			self.wndMiniMap.SetMastHP(hp, max)

		def BINARY_Update_Mast_Window(self, i):
			self.wndMiniMap.SetMastWindow(i)
			
	if app.ENABLE_MAIL_BOX_SYSTEM:
		def OpenMailBox(self):
			self.wndMailBox.LoadList()
			self.wndMailBox.Open()

		if app.ENABLE_TRANSMUTATION_MAIL_BOX_SYSTEM:
			if app.ENABLE_WON_MAIL_BOX_SYSTEM:
				def LoadMailBox(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,won,time,check_acept,check_view):
					self.wndMailBox.MailList(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,won,time,check_acept,check_view)
					self.wndMailBox.MailList2(id_mail)
			else:
				def LoadMailBox(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,time,check_acept,check_view):
					self.wndMailBox.MailList(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,transmutation,gold,time,check_acept,check_view)
					self.wndMailBox.MailList2(id_mail)
		else:
			if app.ENABLE_WON_MAIL_BOX_SYSTEM:
				def LoadMailBox(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,won,time,check_acept,check_view):
					self.wndMailBox.MailList(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,won,time,check_acept,check_view)
					self.wndMailBox.MailList2(id_mail)
			else:
				def LoadMailBox(self,id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,time,check_acept,check_view):
					self.wndMailBox.MailList(id_mail,name_send,titulo_mail,description_mail,item_vnum,item_count,gold,time,check_acept,check_view)
					self.wndMailBox.MailList2(id_mail)

		def MailBoxSocketsItems(self,id_mail,index,sockets):
			self.wndMailBox.MailListSockets(id_mail,index,sockets)

		def MailBoxAttrItems(self,id_mail,index,attr_type,attr_value):
			self.wndMailBox.MailListAttrs(id_mail,index,attr_type,attr_value)

		def MailBoxCheckName(self):
			self.wndMailBox.SendMailCheckName()

		def MailBoxSendMail(self):
			self.wndMailBox.SendMailCheck()

		def MailBoxDeleteMails(self,id_mail):
			self.wndMailBox.DeleteMails(id_mail)

		def MailBoxDeleteMailsLoad(self):
			self.wndMailBox.loadMailsDelete()

		def MailBoxAceptMails(self,id_mail):
			self.wndMailBox.AceptMails(id_mail)

		def MailBoxAceptMailsLoad(self):
			self.wndMailBox.loadMailsAcept()

		def MailBoxCheckView(self,id_mail):
			self.wndMailBox.CheckView(id_mail)
			
	if app.ENABLE_12ZI:
		def OpenUI12zi(self, yellowmark, greenmark, yellowreward, greenreward, goldreward):
			if self.wnd12ziReward == None:
				self.wnd12ziReward = ui12zi.Reward12ziWindow()
			self.wnd12ziReward.Open(yellowmark, greenmark, yellowreward, greenreward, goldreward)

		def Refresh12ziTimer(self, currentFloor, jumpCount, limitTime, elapseTime):
			if self.wndMiniMap:
				self.wndMiniMap.Hide()

			if self.wnd12ziTimer == None:
				self.wnd12ziTimer = ui12zi.FloorLimitTimeWindow()

			self.wnd12ziTimer.Refresh12ziTimer(currentFloor, jumpCount, limitTime, elapseTime)
			self.wnd12ziTimer.Open()

		def Show12ziJumpButton(self):
			self.wnd12ziTimer.Show12ziJumpButton()

		def Hide12ziTimer(self):
			self.wnd12ziTimer.Hide()

		def RefreshShopItemToolTip(self):
			if self.tooltipItem:
				self.tooltipItem.RefreshShopToolTip()

	if app.ENABLE_CUBE_EX_RENEWAL:
		def __MakeCubeExWindow(self):
			self.wndCubeEx = uicube_ex.CubeWindow()
			self.wndCubeEx.SetItemToolTip(self.tooltipItem)
			self.wndCubeEx.Hide()
			
		def OpenCubeExWindow(self):
			self.wndCubeEx.Open()
			
			if (self.wndInventory.IsShow() == False):
				self.wndInventory.Show()
			
		def UpdateCubeExInfo(self, gold, itemVnum, itemCount):
			self.wndCubeEx.UpdateInfo(gold, itemVnum, itemCount)
			
		def SucceedCubeExWork(self, itemVnum, itemCount):
			self.wndCubeEx.ClearSlots()
			
		def FailedCubeExWork(self):
			self.wndCubeEx.ClearSlots()
			
	if (app.ENABLE_CHEQUE_COUPON_SYSTEM):
		def OpenChequeTicket(self, itemPos):
			self.wndImproveChequeTicket.Open(itemPos)
			
	if app.ENABLE_AUTO_HUNTING_SYSTEM:
		def OpenAutohuntWindow(self):
			if self.dlgAutohunt.IsShow():
				self.dlgAutohunt.Close()
			else:
				self.dlgAutohunt.Open()

	def AutohuntRemoveSkillSlot(self, slot):
		self.dlgAutohunt.AutohuntRemoveSkillSlot(slot)

	def AutohuntRemoveItemSlot(self, slot):
		self.dlgAutohunt.AutohuntRemoveItemSlot(slot)
	
	if app.ENABLE_GUILD_RANKING_SYSTEM:
		def OpenGuildWarHistory(self):
			self.wndHistoryGuildWarsAndRanking.Open()
			
		def SetGuildRankedData(self, name, master, win, loss):
			self.wndHistoryGuildWarsAndRanking.SetDataGuildRanked( name, master, win, loss)
