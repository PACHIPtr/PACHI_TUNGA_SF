import app
import net
import combatzone
import chat
import event
import ui
import wndMgr
import uiToolTip
import math
import uiScriptLocale

def Py_FormatTime(time):
	m, s = divmod(time, 60)
	h, m = divmod(m, 60)
	return "%02d:%02d:%02d" % (h, m, s)

class CombatZoneWindow(ui.ScriptWindow):
	DESCRIPTION_TEXT_FILE_NAME = "locale/tr/desc_battle_field.txt"
	ROOT_PATH = "d:/ymir work/ui/public/battle/"
	M_RANK, M_NAME, M_EMPIRE, M_POINTS = 0, 1, 2, 3

	EMPIRE_IMAGE_DICT = { 
		net.EMPIRE_A : ROOT_PATH + "empire_shinsu.sub",
		net.EMPIRE_B : ROOT_PATH + "empire_chunjo.sub",
		net.EMPIRE_C : ROOT_PATH + "empire_jinno.sub" 
	}
	
	class CombatZoneDescriptionBox(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.SetWindowName("CombatZoneWindowDescriptionBox")
			self.descIndex = 0
		def __del__(self):
			ui.Window.__del__(self)
		def SetIndex(self, index):
			self.descIndex = index
		def OnRender(self):
			event.RenderEventSet(self.descIndex)
			
	class CombatZoneDataDaysWindow(ui.Window):
		DAY_MAX_NUM = 7
		H_START, M_START, H_END, M_END = 0, 1, 2, 3
		NAME_OF_DAYS_LIST = [
			uiScriptLocale.BATTLE_FIELD_MONDAY,
			uiScriptLocale.BATTLE_FIELD_TUESDAY,
			uiScriptLocale.BATTLE_FIELD_WEDNESDAY,
			uiScriptLocale.BATTLE_FIELD_THURSDAY,
			uiScriptLocale.BATTLE_FIELD_FRIDAY,
			uiScriptLocale.BATTLE_FIELD_SATURDAY,
			uiScriptLocale.BATTLE_FIELD_SUNDAY
		]
	
		def __init__(self, parentClass = None):
			ui.Window.__init__(self)
			self.SetSize(200, 140)
			self.AddFlag("not_pick")
			self.SetParent(parentClass)
			self.descTextLine = {}
			self.InitializeObject()

		def __del__(self):
			ui.Window.__del__(self)
			
		def InitializeObject(self):
			yPos = 26
			for day in xrange(self.DAY_MAX_NUM):
				self.descTextLine[day] = ui.TextLine()
				self.descTextLine[day].SetParent(self)
				self.descTextLine[day].SetPosition(10, yPos)
				self.descTextLine[day].SetHorizontalAlignCenter()
				self.descTextLine[day].SetVerticalAlignCenter()
				self.descTextLine[day].SetWindowHorizontalAlignCenter()
				self.descTextLine[day].Show()
				yPos += 15
				
				m_vec_daysTabs = combatzone.GetDay(day)
				self.descTextLine[day].SetText(uiScriptLocale.BATTLE_FIELD_EACH_DAY % (self.NAME_OF_DAYS_LIST[day], m_vec_daysTabs[self.H_START]+2, m_vec_daysTabs[self.M_START], m_vec_daysTabs[self.H_END]+2, m_vec_daysTabs[self.M_END]))

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.InitializeObject()
		self.Initialize()
		
	def InitializeObject(self):
		self.descIndex = 0
		self.timeElapsed = 0
		self.scrollPos = 0
		self.descriptionBox = 0
		self.canParticipate = False
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()
		
	def IsDown(self):
		return self.canParticipate

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Initialize(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/combatzonewindow.py")
		except:
			import exception
			exception.Abort("CombatZoneWindow.Initialize.LoadObject")
		try:
			GetObject = self.GetChild

			self.main = {
				'rankingData' : {
					'memberSlot' : {},
					'memberRank' : {},
					'memberName' : {},
					'memberEmpire' : {},
					'memberPoints' : {}
				},

				'window' : {
					'selectedPage' : {
						combatzone.COMBAT_ZONE_TYPE_RANKING_WEEKLY : GetObject("tab_01"),
						combatzone.COMBAT_ZONE_TYPE_RANKING_ALL : GetObject("tab_02"),	

						'btnSelectWeekly' : GetObject("tab_button_01"),
						'btnSelectTotal' : GetObject("tab_button_02"),
					},
					
					'event' : {
						'e_titleBar' : GetObject("titlebar").SetCloseEvent(ui.__mem_func__(self.Close)),
						'e_PotionRequest' : [
							GetObject("itemSlotWindow").SetOverInItemEvent(ui.__mem_func__(self.OverInItem)), 
							GetObject("itemSlotWindow").SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem)), 
							GetObject("itemSlotWindow").SetItemSlot(0, combatzone.COMBAT_ZONE_ICON_SLOT, combatzone.COMBAT_ZONE_ICON_SLOT_COUNT),
							GetObject("btnRequest").SetEvent(ui.__mem_func__(self.OnClickToggleRequestPotion))
						]
					},

					'infoTimeRemaining' : GetObject("timeRemaining"),					
					'infoMonstersKilled' : GetObject("monstersKilled"),
					'infoPoints' : GetObject("my_point"),
					
					'infoBoard' : GetObject("text_board"),
					'list' : GetObject("list"),
					'notice' : GetObject("notice"),
					'btnEnter' : GetObject("enter_button"),
					'scrollBar' : GetObject("scrollbar")
				}
			}

			yPos = 24
			parent = self.main['window']['list']
			for i in xrange(combatzone.COMBAT_ZONE_MAX_ROWS_RANKING):
				self.main['rankingData']['memberSlot'][i] = ui.ImageBox()
				self.main['rankingData']['memberSlot'][i].AddFlag("not_pick")
				self.main['rankingData']['memberSlot'][i].SetParent(parent)
				self.main['rankingData']['memberSlot'][i].SetPosition(2, yPos)
				self.main['rankingData']['memberSlot'][i].LoadImage(self.ROOT_PATH + "rank_list.sub")
			
				self.main['rankingData']['memberRank'][i] = ui.TextLine()
				self.main['rankingData']['memberRank'][i].SetParent(parent)
				self.main['rankingData']['memberRank'][i].SetPosition(33, yPos + 2)
				self.main['rankingData']['memberRank'][i].Show()
				
				self.main['rankingData']['memberName'][i] = ui.TextLine()
				self.main['rankingData']['memberName'][i].SetParent(parent)
				self.main['rankingData']['memberName'][i].SetPosition(126, yPos + 2)
				self.main['rankingData']['memberName'][i].SetHorizontalAlignCenter()
				self.main['rankingData']['memberName'][i].Show()

				self.main['rankingData']['memberEmpire'][i] = ui.ImageBox()
				self.main['rankingData']['memberEmpire'][i].AddFlag("not_pick")
				self.main['rankingData']['memberEmpire'][i].SetParent(parent)
				self.main['rankingData']['memberEmpire'][i].SetPosition(206, yPos + 4)
				self.main['rankingData']['memberEmpire'][i].Show()

				self.main['rankingData']['memberPoints'][i] = ui.TextLine()
				self.main['rankingData']['memberPoints'][i].SetParent(parent)
				self.main['rankingData']['memberPoints'][i].SetPosition(261, yPos + 2)
				self.main['rankingData']['memberPoints'][i].SetHorizontalAlignCenter()
				self.main['rankingData']['memberPoints'][i].Show()
				yPos += 24
		except:
			import exception
			exception.Abort("CombatZoneWindow.Initialize.BindObject")
			
		self.main['window']['selectedPage']['btnSelectWeekly'].SAFE_SetEvent(self.OnClickToggleRanking, combatzone.COMBAT_ZONE_TYPE_RANKING_WEEKLY)
		self.main['window']['selectedPage']['btnSelectTotal'].SAFE_SetEvent(self.OnClickToggleRanking, combatzone.COMBAT_ZONE_TYPE_RANKING_ALL)
		self.main['window']['btnEnter'].SAFE_SetEvent(self.OnClickToggleParticipate)
		self.main['window']['scrollBar'].SetScrollEvent(ui.__mem_func__(self.OnScrollToggleDescription))
		
		self.SetCenterPosition()
		self.Hide()
		
	def OnClickToggleRequestPotion(self):
		combatzone.SendCombatZoneRequestActionPacket(combatzone.COMBAT_ZONE_ACTION_REQUEST_POTION, combatzone.COMBAT_ZONE_EMPTY_DATA)
		
	def OverInItem(self):
		self.tooltipItem.SetItemToolTip(combatzone.COMBAT_ZONE_ICON_SLOT)

	def OverOutItem(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()	
		
	def GetSelectedRank(self, selectedPage):
		self.main['window']['selectedPage'][selectedPage].Show()			
		self.main['window']['selectedPage'][[combatzone.COMBAT_ZONE_TYPE_RANKING_WEEKLY, combatzone.COMBAT_ZONE_TYPE_RANKING_ALL][selectedPage is combatzone.COMBAT_ZONE_TYPE_RANKING_WEEKLY]].Hide()			
		
		for btn in [self.main['window']['selectedPage']['btnSelectWeekly'], self.main['window']['selectedPage']['btnSelectTotal']]:
			btn.SetUp()
			
	def OnClickToggleParticipate(self):
		combatzone.SendCombatZoneRequestActionPacket(combatzone.COMBAT_ZONE_ACTION_PARTICIPATE, combatzone.COMBAT_ZONE_EMPTY_DATA)
		
	def OnUpdate(self):
		iTimeRemaining = self.timeElapsed - app.GetGlobalTimeStamp()
		if (iTimeRemaining > 0):
			self.main['window']['infoTimeRemaining'].SetText(Py_FormatTime(iTimeRemaining))
		
		(xposEventSet, yposEventSet) = self.main['window']['infoBoard'].GetGlobalPosition()
		event.UpdateEventSet(self.descIndex, xposEventSet + 7, - (yposEventSet + 7 - (int(self.scrollPos) * 16)))
		if self.descriptionBox:
			self.descriptionBox.SetIndex(self.descIndex)
		
	def Open(self, points, iTimeElapsed, iCurMobsKills, iMaxMobKills):
		self.timeElapsed = iTimeElapsed + app.GetGlobalTimeStamp()
		
		self.main['window']['notice'].SetText([uiScriptLocale.BATTLE_FIELD_OPERATION_OFFLINE, uiScriptLocale.BATTLE_FIELD_OPERATION_ONLINE][combatzone.IsRunning()])
		self.main['window']['infoPoints'].SetText(uiScriptLocale.BATTLE_FIELD_POINTS % points)
		self.main['window']['infoMonstersKilled'].SetText(uiScriptLocale.BATTLE_FIELD_MONSTERS_LIMIT % (iCurMobsKills, iMaxMobKills))
		self.GetSelectedRank(combatzone.COMBAT_ZONE_TYPE_RANKING_WEEKLY)	
		
		self.SetDescriptionEvent()
		self.CreateDescriptionBox()
		self.main['window']['scrollBar'].SetPos(0.0)
		
		self.classDataDays = self.CombatZoneDataDaysWindow(self)
		self.classDataDays.SetPosition(- 10, 30)
		self.classDataDays.Show()

	def OnClickToggleRanking(self, selectedPage):
		combatzone.SendCombatZoneRequestActionPacket(combatzone.COMBAT_ZONE_ACTION_CHANGE_PAGE_RANKING, selectedPage)
		self.GetSelectedRank(selectedPage)

	def RegisterRanking(self):
	
		def SetPadders(key, row, tokens):
			self.main['rankingData'][key][row].SetText(str(tokens))
			self.main['rankingData'][key][row].Show()
			SetMemberSlot(row)

		def SetMemberEmpire(row, tokens):
			if tokens:
				self.main['rankingData']['memberEmpire'][row].LoadImage(self.EMPIRE_IMAGE_DICT[tokens])
				self.main['rankingData']['memberEmpire'][row].Show()
			
		def SetMemberSlot(row):
			self.main['rankingData']['memberSlot'][row].Show()

		def SetMemberRank(row, tokens):
			rowIndex = tokens + 1
			if (row == combatzone.COMBAT_ZONE_MAX_ROWS_RANKING - 1):
				rowIndex -= 1
				
			SetPadders('memberRank', row, rowIndex)
			
		def SetMemberName(row, tokens):
			SetPadders('memberName', row, tokens)
			
		def SetMemberPoints(row, tokens):
			SetPadders('memberPoints', row, tokens)

		for row in xrange(combatzone.COMBAT_ZONE_MAX_ROWS_RANKING):

			m_vec_rankTabs = combatzone.GetRankings(row)
			memberRank, memberName, memberEmpire, memberPoints = m_vec_rankTabs[self.M_RANK], m_vec_rankTabs[self.M_NAME], m_vec_rankTabs[self.M_EMPIRE], m_vec_rankTabs[self.M_POINTS]

			if memberEmpire == combatzone.COMBAT_ZONE_EMPTY_DATA:
				for memberEmpty in [self.main['rankingData']['memberSlot'][row], self.main['rankingData']['memberRank'][row], self.main['rankingData']['memberName'][row], self.main['rankingData']['memberEmpire'][row], self.main['rankingData']['memberPoints'][row]]:
					memberEmpty.Hide()
			else:
				SetMemberRank(row, memberRank)
				SetMemberName(row, memberName)
				SetMemberEmpire(row, memberEmpire)
				SetMemberPoints(row, memberPoints)
		self.Show()
		
	def Close(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = 0
		self.descriptionBox = 0
		self.scrollPos = 0
		self.Hide()
		
	def OnScrollToggleDescription(self):
		pos = self.main['window']['scrollBar'].GetPos()
		self.scrollPos = math.floor(float(pos) / float(float(1) / float(event.GetLineCount(self.descIndex) - 12)) + 0.001)
		event.SetVisibleStartLine(self.descIndex, int(self.scrollPos))
		event.Skip(self.descIndex)

	def SetDescriptionEvent(self):
		event.ClearEventSet(self.descIndex)
		self.descIndex = event.RegisterEventSet(self.DESCRIPTION_TEXT_FILE_NAME)
		event.SetRestrictedCount(self.descIndex, 30)
		event.SetVisibleLineCount(self.descIndex, 12)

	def CreateDescriptionBox(self):
		self.descriptionBox = self.CombatZoneDescriptionBox()
		self.descriptionBox.SetParent(self.main['window']['infoBoard'])
		self.descriptionBox.Show()

	def Destroy(self):
		self.ClearDictionary()
		self.descriptionBox = None
		self.scrollPos = None