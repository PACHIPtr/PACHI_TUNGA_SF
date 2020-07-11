import ui
import wndMgr
import mouseModule
import player
import skill
import app
import item
import net
import os
import uiToolTip
import uiScriptLocale
import constInfo
import chr

class AutoHunt(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.used_timers = [1 for j in xrange(18)]
		self.lastUpdate = 0
		self.is_on = 0
		self.attack_on = 0
		self.skills_on = 0
		self.potions_on = 0
		self.first_open = 0
		self.LoadWindow()
		self.LoadSettings()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/autowindow.py")
		except:
			import exception
			exception.Abort("autowindow.LoadDialog.LoadScript")

		try:
			GetObject=self.GetChild
			self.board = GetObject("board")
			self.Auto_Active_Skill_Slot_Table = GetObject("Auto_Active_Skill_Slot_Table")
			self.AutoSkillClearButton = GetObject("AutoSkillClearButton")
			self.AutoPositionClearButton = GetObject("AutoPositionClearButton")
			self.AutoAllClearButton = GetObject("AutoAllClearButton")
			self.AutoAttackOffButton = GetObject("AutoAttackOffButton")
			self.AutoAttackOnButton = GetObject("AutoAttackOnButton")
			self.AutoSkillOffButton = GetObject("AutoSkillOffButton")
			self.AutoSkillOnButton = GetObject("AutoSkillOnButton")
			self.AutoPositionlOffButton = GetObject("AutoPositionlOffButton")
			self.AutoPositionlOnButton = GetObject("AutoPositionlOnButton")
			self.AutoStartOffButton = GetObject("AutoStartOffButton")
			self.AutoStartOnButton = GetObject("AutoStartOnButton")
			self.AutoToolTIpButton = GetObject("AutoToolTIpButton")
			self.editlines = []
			for i in xrange(16):
				self.editlines.append(GetObject("editline%s" % (str(i))))
		except:
			import exception
			exception.Abort("autowindow.LoadDialog.BindObject")
		self.AutoToolTIpButton.SetToolTipWindow(self.__CreateGameTypeToolTip(uiScriptLocale.AUTO_TOOLTIP_TITLE))

		self.board.SetCloseEvent(ui.__mem_func__(self.Close))


		self.Auto_Active_Skill_Slot_Table.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectSlot))
		self.Auto_Active_Skill_Slot_Table.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		
		self.AutoSkillClearButton.SetEvent(ui.__mem_func__(self.ClearSkillSlots))
		self.AutoPositionClearButton.SetEvent(ui.__mem_func__(self.ClearItemSlots))
		self.AutoAllClearButton.SetEvent(ui.__mem_func__(self.ClearAllSlots))
		
		self.AutoStartOffButton.SetEvent(ui.__mem_func__(self.SetBotState), 0)
		self.AutoStartOnButton.SetEvent(ui.__mem_func__(self.SetBotState), 1)

		self.AutoAttackOffButton.SetEvent(ui.__mem_func__(self.SetAttackState), 0)
		self.AutoAttackOnButton.SetEvent(ui.__mem_func__(self.SetAttackState), 1)

		self.AutoSkillOffButton.SetEvent(ui.__mem_func__(self.SetSkillState), 0)
		self.AutoSkillOnButton.SetEvent(ui.__mem_func__(self.SetSkillState), 1)

		self.AutoPositionlOffButton.SetEvent(ui.__mem_func__(self.SetPotionsState), 0)
		self.AutoPositionlOnButton.SetEvent(ui.__mem_func__(self.SetPotionsState), 1)

		self.slots_items = []
		for i in xrange(17):
			self.slots_items.append([])
			self.slots_items[i].append(-1)
			self.slots_items[i].append(-1)
		
		self.AutoAttackOffButton.Down()
		self.AutoAttackOffButton.Disable()
		self.AutoStartOffButton.Down()
		self.AutoStartOffButton.Disable()
		self.AutoSkillOffButton.Down()
		self.AutoSkillOffButton.Disable()
		self.AutoPositionlOffButton.Down()
		self.AutoPositionlOffButton.Disable()
		self.RefreshSlots()

	def AutohuntRemoveSkillSlot(self, slot):
		for i in xrange(8):
			if self.slots_items[i][0] == slot:
				self.slots_items[i][0] = -1
				self.slots_items[i][1] = 0
				self.Auto_Active_Skill_Slot_Table.ClearSlot(i)
				self.editlines[i].SetText("")

	def AutohuntRemoveItemSlot(self, slot):
		for i in range(9, 17):
			if self.slots_items[i][0] == slot:
				self.slots_items[i][0] = -1
				self.slots_items[i][1] = 0
				self.Auto_Active_Skill_Slot_Table.ClearSlot(i)
				self.editlines[i-1].SetText("")

	def __CreateGameTypeToolTip(self, title):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)
		toolTip.AppendSpace(5)
		toolTip.AutoAppendTextLine(uiScriptLocale.AUTO_TOOLTIP_LINE1)
		toolTip.AutoAppendTextLine(uiScriptLocale.AUTO_TOOLTIP_LINE2)
		toolTip.AutoAppendTextLine(uiScriptLocale.AUTO_TOOLTIP_LINE3)
		toolTip.AutoAppendTextLine(uiScriptLocale.AUTO_TOOLTIP_LINE4)
		toolTip.AutoAppendTextLine(uiScriptLocale.AUTO_TOOLTIP_LINE5)

		toolTip.AlignHorizonalCenter()
		return toolTip

	def SelectSlot(self, slotIndex):
		if TRUE == mouseModule.mouseController.isAttached():
			self.AddSlot(slotIndex)

	def SelectItemSlot(self, slotIndex):
		if slotIndex >= 0 and slotIndex <= 7:
			srcSlotIndex = self.slots_items[slotIndex][0]
			selectedSkillIndex = player.GetSkillIndex(srcSlotIndex)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_AUTOHUNT_SKILL, srcSlotIndex, selectedSkillIndex)
		else:
			srcSlotIndex = self.slots_items[slotIndex][0]
			itemIndex = player.GetItemIndex(self.slots_items[slotIndex][0])
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_AUTOHUNT_ITEM, srcSlotIndex, itemIndex)

	def AddSlot(self, localSlotIndex):
		AttachedSlotType = mouseModule.mouseController.GetAttachedType()
		AttachedSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
		AttachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
		if AttachedSlotType == player.SLOT_TYPE_SKILL and localSlotIndex >= 0 and localSlotIndex <= 7:
			self.AddSkillSlot(localSlotIndex, AttachedSlotNumber)

		if AttachedSlotType == player.SLOT_TYPE_INVENTORY and localSlotIndex >= 9 and localSlotIndex <= 16:
			itemIndex = player.GetItemIndex(AttachedSlotNumber)
			item.SelectItem(itemIndex)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType == 3 and (itemSubType == 0 or itemSubType == 7 or itemSubType == 8 or itemSubType == 11 or itemSubType == 10):
				if itemSubType == 7:
					time = item.GetValue(1)
				elif itemSubType == 8:
					time = item.GetValue(3)
				else:
					time = 50
				self.AddItemSlot(localSlotIndex, AttachedSlotNumber, time)

		mouseModule.mouseController.DeattachObject()

	def RefreshSlots(self):
		for i in xrange(8):
			skillIndex = player.GetSkillIndex(self.slots_items[i][0])
			skillGrade = player.GetSkillGrade(self.slots_items[i][0])
			skillLevel = player.GetSkillLevel(self.slots_items[i][0])

			self.Auto_Active_Skill_Slot_Table.SetSkillSlotNew(i, skillIndex, skillGrade, skillLevel)
			self.Auto_Active_Skill_Slot_Table.SetSlotCountNew(i, skillGrade, skillLevel)

		for i in xrange(9, 17):
			itemIndex = player.GetItemIndex(self.slots_items[i][0])
			if itemIndex == 0:
				self.slots_items[i][0] = -1
				self.slots_items[i][1] = 0
				self.Auto_Active_Skill_Slot_Table.ClearSlot(i)
				self.editlines[i-1].SetText("")
			itemCount = player.GetItemCount(self.slots_items[i][0])
			self.Auto_Active_Skill_Slot_Table.SetItemSlot(i, itemIndex, itemCount)

	def AddSkillSlot(self, slotIndex, skillPos, timer = -1):
		if timer == -1:
			skillNextPercentage = player.GetSkillCurrentEfficientPercentage(skillPos)
			skillIndex = player.GetSkillIndex(skillPos)
			timer = skill.GetSkillCoolTime(skillIndex, skillNextPercentage)
		for i in xrange(8):
			if self.slots_items[i][0] == skillPos:
				self.slots_items[i][0] = -1
				self.editlines[i].SetText("")
		if self.slots_items[slotIndex][0] == -1:
			self.editlines[slotIndex].SetText(str(timer))
		self.slots_items[slotIndex][0] = skillPos
		self.RefreshSlots()

	def AddItemSlot(self, slotIndex, itemPos, time):
		for i in xrange(9, 17):
			if self.slots_items[i][0] == itemPos:
				self.slots_items[i][0] = -1
				self.editlines[i-1].SetText("")
		if self.slots_items[slotIndex][0] == -1:
			self.editlines[slotIndex-1].SetText(str(time))
		self.slots_items[slotIndex][0] = itemPos
		self.RefreshSlots()

	def ClearSkillSlots(self):
		for i in xrange(8):
			self.slots_items[i][0] = -1
			self.editlines[i].SetText("")
		self.RefreshSlots()

	def ClearItemSlots(self):
		for i in xrange(9, 17):
			self.slots_items[i][0] = -1
			self.editlines[i-1].SetText("")
		self.RefreshSlots()

	def ClearAllSlots(self):
		self.ClearSkillSlots()
		self.ClearItemSlots()

	def SaveSettings(self):
		if not os.path.exists("mark/userdata"):
			os.makedirs("mark/userdata")
		hs = open("mark/userdata/"+str(player.GetName()),"w")
		for i in xrange(16):
			if i > 7 and self.editlines[i].GetText() != "":
				timer = int(self.editlines[i].GetText())
				value = self.slots_items[i+1][0]
			elif i < 8 and self.editlines[i].GetText() != "":
				timer = int(self.editlines[i].GetText())
				value = self.slots_items[i][0]
			else:
				timer = -1
				value = -1
			hs.write("%d\t%d\n" % (value, timer))
		hs.close()

	def LoadSettings(self):
		if os.path.exists("mark/userdata/"+str(player.GetName())):
			handle = app.OpenTextFile("mark/userdata/"+str(player.GetName()))
			for i in xrange(16):
				line = app.GetTextFileLine(handle, i)
				lin = line.split("\t")
				try:
					if int(lin[0]) != -1:
						if i < 8:
							self.AddSkillSlot(i, int(lin[0]), int(lin[1]))
						else:
							self.AddItemSlot(i+1, int(lin[0]), int(lin[1]))
				except:
					pass

	def PopupMessage(self,text):
		pop = uiCommon.PopupDialog()
		pop.SetText(text)
		pop.SetAcceptEvent(self.__OnClosePopupDialog)
		pop.Open()
		self.pop = pop
		self.pop.SetTop()

	def SetBotState(self, arg):
		if app.ENABLE_AFFECT_CHECK and player.CheckAffect(long(chr.NEW_AFFECT_AUTO_HUNT), 0) == False:
			self.PopupMessage(localeInfo.AUTO_HUNT_ITEM_NULL)
			return
		if arg == 1:
			self.AutoStartOnButton.Down()
			self.AutoStartOnButton.Disable()
			self.AutoStartOffButton.SetUp()
			self.AutoStartOffButton.Enable()
			#net.SendAutohunt(1)
			self.is_on = 1
		else:
			self.AutoStartOffButton.Down()
			self.AutoStartOffButton.Disable()
			self.AutoStartOnButton.SetUp()
			self.AutoStartOnButton.Enable()
			#net.SendAutohunt(0)
			self.is_on = 0

	def SetAttackState(self, arg):
		if app.ENABLE_AFFECT_CHECK and player.CheckAffect(long(chr.NEW_AFFECT_AUTO_HUNT), 0) == False:
			self.PopupMessage(localeInfo.AUTO_HUNT_ITEM_NULL)
			return
		if arg == 1:
			self.AutoAttackOnButton.Down()
			self.AutoAttackOnButton.Disable()
			self.AutoAttackOffButton.SetUp()
			self.AutoAttackOffButton.Enable()
			self.attack_on = 1
		else:
			self.AutoAttackOffButton.Down()
			self.AutoAttackOffButton.Disable()
			self.AutoAttackOnButton.SetUp()
			self.AutoAttackOnButton.Enable()
			self.attack_on = 0

	def SetSkillState(self, arg):
		if app.ENABLE_AFFECT_CHECK and player.CheckAffect(long(chr.NEW_AFFECT_AUTO_HUNT), 0) == False:
			self.PopupMessage(localeInfo.AUTO_HUNT_ITEM_NULL)
			return
		if arg == 1:
			self.AutoSkillOnButton.Down()
			self.AutoSkillOnButton.Disable()
			self.AutoSkillOffButton.SetUp()
			self.AutoSkillOffButton.Enable()
			self.skills_on = 1
		else:
			self.AutoSkillOffButton.Down()
			self.AutoSkillOffButton.Disable()
			self.AutoSkillOnButton.SetUp()
			self.AutoSkillOnButton.Enable()
			self.skills_on = 0

	def SetPotionsState(self, arg):
		if app.ENABLE_AFFECT_CHECK and player.CheckAffect(long(chr.NEW_AFFECT_AUTO_HUNT), 0) == False:
			self.PopupMessage(localeInfo.AUTO_HUNT_ITEM_NULL)
			return
		if arg == 1:
			self.AutoPositionlOnButton.Down()
			self.AutoPositionlOnButton.Disable()
			self.AutoPositionlOffButton.SetUp()
			self.AutoPositionlOffButton.Enable()
			self.potions_on = 1
		else:
			self.AutoPositionlOffButton.Down()
			self.AutoPositionlOffButton.Disable()
			self.AutoPositionlOnButton.SetUp()
			self.AutoPositionlOnButton.Enable()
			self.potions_on = 0

	def OnUpdate(self):
		if app.GetGlobalTime() - self.lastUpdate > 500:
			self.lastUpdate = app.GetGlobalTime()
			self.RefreshSlots()
			self.SaveSettings()
		if app.GetGlobalTime() - self.lastUpdate > 250:
			self.lastUpdate = app.GetGlobalTime()
			if self.is_on == 1 and self.attack_on == 1:
				player.UpdateAutoHunt(True)
		if self.is_on == 1 and self.skills_on == 1:
			for i in xrange(8):
				if self.slots_items[i][0] != -1:
					if app.GetTime() > self.used_timers[i] and self.editlines[i].GetText() != "":
						self.used_timers[i] = app.GetTime() + int(self.editlines[i].GetText())
						player.ClickSkillSlot(self.slots_items[i][0])
		if self.is_on == 1 and self.potions_on == 1:
			for i in xrange(9, 17):
				if self.slots_items[i][0] != -1:
					itemIndex = player.GetItemIndex(self.slots_items[i][0])
					item.SelectItem(itemIndex)
					itemSubType = item.GetItemSubType()
					if itemSubType == 7 or itemSubType == 8 or itemSubType == 10:
						if app.GetTime() > self.used_timers[i] and self.editlines[i-1].GetText() != "":
							self.used_timers[i] = app.GetTime() + int(self.editlines[i-1].GetText())
							net.SendItemUsePacket(self.slots_items[i][0])
					else:
						if item.GetValue(0) != 0 and (float(player.GetStatus(player.HP)) / float(player.GetStatus(player.MAX_HP)) * 100) < int(self.editlines[i-1].GetText()):
							net.SendItemUsePacket(self.slots_items[i][0])
						if item.GetValue(1) != 0 and (float(player.GetStatus(player.SP)) / float(player.GetStatus(player.MAX_SP)) * 100) < int(self.editlines[i-1].GetText()):
							net.SendItemUsePacket(self.slots_items[i][0])

	def Close(self):
		self.Hide()

	def Open(self):
		if self.first_open == 0:
			self.first_open = 1
			#else:
				#self.AutoSkillOffButton.Down()
				#self.AutoSkillOffButton.Disable()
				#self.AutoPositionlOffButton.Down()
				#self.AutoPositionlOffButton.Disable()
		self.Show()

	def Destroy(self):
		self.ClearDictionary()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

