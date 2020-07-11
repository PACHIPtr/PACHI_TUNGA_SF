import dbg,grp,time
import ui,uiCommon,localeInfo
import wndMgr

class _ui:
	###################################################################################################################
	# BOARD
	###################################################################################################################
	def Board(self, parent, moveable, x, y, width, heigh, center = False, IsShow = True):
		tmpBoard = ui.Board()

		if parent:
			tmpBoard.SetParent(parent)

		if moveable:
			tmpBoard.AddFlag('movable')
			tmpBoard.AddFlag('float')

		tmpBoard.SetSize(width, heigh)
		tmpBoard.SetPosition(x, y)
		if center:
			tmpBoard.SetCenterPosition()

		if IsShow:
			tmpBoard.Show()
		return tmpBoard

	def ThinBoard(self, parent, moveable, x, y, width, heigh, center = False, IsShow = True):
		tmpThinboard = ui.ThinBoard()

		if parent:
			tmpThinboard.SetParent(parent)

		if moveable:
			tmpThinboard.AddFlag('movable')
			tmpThinboard.AddFlag('float')

		tmpThinboard.SetSize(width, heigh)
		tmpThinboard.SetPosition(x, y)
		if center:
			tmpThinboard.SetCenterPosition()

		if IsShow:
			tmpThinboard.Show()
		return tmpThinboard

	def BoardWithTitleBar(self, parent, moveable, x, y, width, heigh, title, closeevent, center = False, IsShow = True):
		tmpTitleBoard = ui.BoardWithTitleBar()

		if parent:
			tmpTitleBoard.SetParent(parent)

		if moveable:
			tmpTitleBoard.AddFlag('movable')
			tmpTitleBoard.AddFlag('float')

		tmpTitleBoard.SetSize(width, heigh)
		tmpTitleBoard.SetPosition(x, y)
		tmpTitleBoard.SetTitleName(title)
		tmpTitleBoard.SetCloseEvent(closeevent)
		if center:
			tmpTitleBoard.SetCenterPosition()

		if IsShow:
			tmpTitleBoard.Show()
		return tmpTitleBoard

	###################################################################################################################
	# BUTTON
	###################################################################################################################
	def BasicButton(self, parent, buttonName, x, y, func):
		tmpButton = ui.Button()

		if parent:
			tmpButton.SetParent(parent)

		tmpButton.SetPosition(x, y)
		tmpButton.SetUpVisual('d:/ymir work/ui/public/middle_button_01.sub')
		tmpButton.SetOverVisual('d:/ymir work/ui/public/middle_button_02.sub')
		tmpButton.SetDownVisual('d:/ymir work/ui/public/middle_button_03.sub')

		tmpButton.SetText(buttonName)

		tmpButton.SetEvent(func)
		tmpButton.Show()
		return tmpButton

	def Button(self, parent, buttonName, tooltipText, x, y, func, UpVisual, OverVisual, DownVisual):
		tmpButtonFormatted = ui.Button()

		if parent:
			tmpButtonFormatted.SetParent(parent)

		tmpButtonFormatted.SetPosition(x, y)
		tmpButtonFormatted.SetUpVisual(UpVisual)
		tmpButtonFormatted.SetOverVisual(OverVisual)
		tmpButtonFormatted.SetDownVisual(DownVisual)

		tmpButtonFormatted.SetText(buttonName)
		tmpButtonFormatted.SetToolTipText(tooltipText)

		tmpButtonFormatted.SetEvent(func)
		tmpButtonFormatted.Show()
		return tmpButtonFormatted

	def RadioButton(self, parent, buttonName, tooltipText, x, y, func, UpVisual, OverVisual, DownVisual):
		tmpRadioButton = ui.RadioButton()

		if parent:
			tmpRadioButton.SetParent(parent)

		tmpRadioButton.SetPosition(x, y)

		tmpRadioButton.SetEvent(func)
		tmpRadioButton.SetUpVisual(UpVisual)
		tmpRadioButton.SetOverVisual(OverVisual)
		tmpRadioButton.SetDownVisual(DownVisual)

		tmpRadioButton.SetText(buttonName)
		tmpRadioButton.SetToolTipText(tooltipText)

		tmpRadioButton.SetEvent(func)
		tmpRadioButton.Show()
		return tmpRadioButton

	def ToggleButton(self, parent, text, tooltiptext, x, y, funcUp, funcDown, UpVisual, OverVisual, DownVisual):
		tmpToggleButton = ui.ToggleButton()

		if parent:
			tmpToggleButton.SetParent(parent)

		tmpToggleButton.SetPosition(x, y)
		tmpToggleButton.SetUpVisual(UpVisual)
		tmpToggleButton.SetOverVisual(OverVisual)
		tmpToggleButton.SetDownVisual(DownVisual)

		tmpToggleButton.SetText(text)
		tmpToggleButton.SetToolTipText(tooltiptext)

		tmpToggleButton.SetToggleUpEvent(funcUp)
		tmpToggleButton.SetToggleDownEvent(funcDown)
		tmpToggleButton.Show()
		return tmpToggleButton

	def DragButton(self, parent, func, UpVisual, OverVisual, DownVisual):
		tmpDragButton = ui.DragButton()

		tmpDragButton.SetParent(parent)
		tmpDragButton.SetMoveEvent(func)

		tmpDragButton.SetUpVisual(UpVisual)
		tmpDragButton.SetOverVisual(OverVisual)
		tmpDragButton.SetDownVisual(DownVisual)

		tmpDragButton.Show()
		return tmpDragButton

	def ResizeableButton(self, parent, x, y, width, heigh, text, tooltiptext, event):
		tmpResizeableButton = ResizeableButton()

		if parent:
			tmpResizeableButton.SetParent(parent)

		tmpResizeableButton.SetPosition(x, y)
		tmpResizeableButton.SetSize(width, heigh)

		tmpResizeableButton.SetText(text)
		tmpResizeableButton.SetToolTipText(tooltiptext)

		tmpResizeableButton.SetEvent(event)

		tmpResizeableButton.Show()
		return tmpResizeableButton

	###################################################################################################################
	# GENERAL
	###################################################################################################################
	def RGB(self, r, g, b):
		return (r*255, g*255, b*255)

	def Box(self, parent, x, y, width, heigh, color):
		tmpBox = ui.Box()

		if parent:
			tmpBox.SetParent(parent)
		if color:
			tmpBox.SetColor(color)

		tmpBox.SetPosition(x, y)
		tmpBox.SetSize(width, heigh)

		tmpBox.Show()
		return tmpBox

	def Line(self, parent, x, y, width, heigh, color):
		tmpLine = ui.Line()

		if parent:
			tmpLine.SetParent(parent)
		if color:
			tmpLine.SetColor(color)

		tmpLine.SetPosition(x, y)
		tmpLine.SetSize(width, heigh)

		tmpLine.Show()
		return tmpLine

	def TextLine(self, parent, text, x, y):
		tmpText = ui.TextLine()

		if parent:
			tmpText.SetParent(parent)

		tmpText.SetPosition(x, y)
		tmpText.SetText(text)

		tmpText.Show()
		return tmpText

	def LargeTextLine(self, parent, textlineText, x, y, color):
		tmpLargeTextLine = ui.TextLine()

		if parent:
			tmpLargeTextLine.SetParent(parent)

		tmpLargeTextLine.SetPosition(x, y)
		tmpLargeTextLine.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		tmpLargeTextLine.SetPackedFontColor(color)
		tmpLargeTextLine.SetText(textlineText)

		tmpLargeTextLine.Show()
		return tmpLargeTextLine

	def SlotWithText(self, parent, text, x, y, width, height):
		SlotBar = ui.SlotBar()

		if parent:
			SlotBar.SetParent(parent)

		SlotBar.SetSize(width, height)
		SlotBar.SetPosition(x, y)
		SlotBar.Show()

		textline = ui.TextLine()
		textline.SetParent(SlotBar)
		textline.SetPosition(5, 1)
		textline.SetText(text)
		textline.Show()

		return SlotBar, textline

	def EditLine(self, parent, text, x, y, width, heigh, max):
		tmpEditLine = ui.EditLine()

		if parent:
			tmpEditLine.SetParent(parent)

		tmpEditLine.SetSize(width, heigh)
		tmpEditLine.SetPosition(x, y)
		tmpEditLine.SetMax(max)
		tmpEditLine.SetLimitWidth(width)
		tmpEditLine.SetMultiLine()
		tmpEditLine.SetText(text)

		tmpEditLine.Show()
		return tmpEditLine

	def SlotWithEditline(self, parent, editlineText, x, y, width, heigh, max):
		SlotBar = ui.SlotBar()

		if parent:
			SlotBar.SetParent(parent)

		SlotBar.SetSize(width, heigh)
		SlotBar.SetPosition(x, y)
		SlotBar.Show()

		Value = ui.EditLine()
		Value.SetParent(SlotBar)
		Value.SetSize(width, heigh)
		Value.SetPosition(10, 1)
		Value.SetMax(max)
		Value.SetLimitWidth(width)
		Value.SetMultiLine()
		Value.SetText(editlineText)
		#Value.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
		Value.Show()

		return SlotBar, Value

	def NumberLine(self, parent, path, number, x, y):
		tmpNumberLine = ui.NumberLine()

		if parent:
			tmpNumberLine.SetParent(parent)

		tmpNumberLine.SetPosition(x, y)
		tmpNumberLine.SetPath(path)
		tmpNumberLine.SetNumber(number)

		tmpNumberLine.Show()
		return tmpNumberLine

	def ComboBox(self, parent, text, x, y, width):
		tmpCombo = ComboBox()

		if parent:
			tmpCombo.SetParent(parent)

		tmpCombo.SetPosition(x, y)
		tmpCombo.SetSize(width, 15)

		tmpCombo.SetCurrentItem(text)

		tmpCombo.Show()
		return tmpCombo

	def Gauge(self, parent, width, color, x, y):
		gauge = ui.Gauge()

		if parent:
			gauge.SetParent(parent)

		gauge.SetPosition(x, y)

		gauge.MakeGauge(width, color)
		gauge.Show()
		return gauge

	#self.tab_menu = _ui().TabControl(self.Board, 100, 100, ["tab1", "TAB1 TEXT"], ["tab2", "TAB2 TEXT"])
	def TabControl(self, parent, width, heigh, *tabv):
		self.tabs = []
		#{"TAB" : "tab_1", "TAB_NAME" : "Tab 1"},
		#{"TAB" : "tab_2", "TAB_NAME" : "Tab 2"}

		#*tabv (["tab_1", "Tab 1"], ["tab_2", "Tab 2"])
		for arg in tabv:
			tab_item = {"TAB" : arg[0], "TAB_NAME" : arg[1]}
			self.tabs.append(tab_item)

		x = -15
		self.tabPageList = {}
		self.tabList = {}
		for tabs in self.tabs:
			x += 80
			Tab = ui.RadioButton()
			Tab.SetParent(parent)
			Tab.SetPosition(x, 30)
			Tab.SetUpVisual("d:/ymir work/ui/game/windows/tab_button_large_01.sub")
			Tab.SetOverVisual("d:/ymir work/ui/game/windows/tab_button_large_02.sub")
			Tab.SetDownVisual("d:/ymir work/ui/game/windows/tab_button_large_03.sub")
			Tab.SetText(tabs["TAB_NAME"])
			Tab.SetEvent(lambda arg = tabs["TAB"]: self.TabEvent(arg))
			Tab.SetUp()
			Tab.Show()
			Page = ui.Box()
			Page.SetParent(parent)
			Page.SetPosition(8, 50)
			Page.SetSize(width-16, heigh-60)
			#Page.SetColor(grp.GenerateColor(0.0, 0.0, 0.5, 0.3))
			Page.Show()
			self.tabList[tabs["TAB"]] = Tab
			self.tabPageList[tabs["TAB"]] = Page

		if self.tabs:
			self.SetState(self.tabs[0]["TAB"])

		return self.tabPageList

	def TabEvent(self, arg):
		self.SetState(arg)

	def SetState(self, stateKey):
		for tabs in self.tabs:
			self.tabList[tabs["TAB"]].SetUp()
			self.tabPageList[tabs["TAB"]].Hide()

		self.tabList[stateKey].Down()
		self.tabPageList[stateKey].Show()

	###################################################################################################################
	# LIST
	###################################################################################################################

	def ListBox(self, parent, x, y, width, heigh):
		tmpListBox = ui.ListBox()

		if parent:
			tmpListBox.SetParent(parent)

		tmpListBox.SetPosition(x, y)
		tmpListBox.SetSize(width, heigh)

		tmpListBox.Show()
		return tmpListBox

	def ListBox2(self, parent, x, y, width, heigh):
		tmpListBox2 = ui.ListBox2()

		if parent:
			tmpListBox2.SetParent(parent)

		tmpListBox2.SetPosition(x, y)
		tmpListBox2.SetSize(width, heigh)

		tmpListBox2.Show()
		return tmpListBox2

	def ListBoxEx(self, parent, x, y, width, heigh):
		tmpListBoxEx = ui.ListBoxEx()

		if parent:
			tmpListBoxEx.SetParent(parent)

		tmpListBoxEx.SetPosition(x, y)
		if width and heigh:
			tmpListBoxEx.SetSize(width, heigh)

		tmpListBoxEx.Show()
		return tmpListBoxEx

	def ListBoxExFull(self, parent, x, y, width, heigh):
		bar = ui.Bar()
		if parent:
			bar.SetParent(parent)

		bar.SetPosition(x, y)
		bar.SetSize(width, heigh)
		bar.SetColor(0x77000000)
		bar.Show()

		ListBox = ui.ListBoxEx()
		ListBox.SetParent(bar)
		ListBox.SetPosition(0, 0)
		ListBox.SetSize(width, heigh)
		ListBox.Show()

		scroll = ui.ScrollBar()
		scroll.SetParent(ListBox)
		scroll.SetPosition(width-15, 0)
		scroll.SetScrollBarSize(heigh)
		scroll.Show()
		ListBox.SetScrollBar(scroll)

		return bar, ListBox, scroll

	def CheckBox(self, parent, text, x, y, upEvent, downEvent):
		tmpCheckBox = CheckBox(text)

		if parent:
			tmpCheckBox.SetParent(parent)

		tmpCheckBox.SetPosition(x, y)

		tmpCheckBox.eventUp   = upEvent
		tmpCheckBox.eventDown = downEvent

		tmpCheckBox.Show()
		return tmpCheckBox

	def NewListBox(self, parent, name, x, y, width, heigh, event, dropdown = 1):
		tmpNewListBox = NewListBox(parent,name,dropdown)
		if parent:
			tmpNewListBox.SetParent(parent)

		tmpNewListBox.SetPosition(x,y)
		tmpNewListBox.SetSize(width,heigh)

		tmpNewListBox.OnChange=event

		tmpNewListBox.Show()
		return tmpNewListBox

	def NewListBox2(self, parent, name, x, y, width, heigh, event):
		tmpNewListBox2 = NewListBox2()

		if parent:
			tmpNewListBox2.SetParent(parent)

		tmpNewListBox2.SetCurrentItem(name)
		tmpNewListBox2.SetSize(width, heigh)
		tmpNewListBox2.SetPosition(x, y)
		tmpNewListBox2.SetEvent(event)

		tmpNewListBox2.Show()
		return tmpNewListBox2

	###################################################################################################################
	# BAR
	###################################################################################################################
	def SlotBar(self, parent, x, y, width, heigh):
		tmpSlotBar = ui.SlotBar()

		if parent:
			tmpSlotBar.SetParent(parent)

		tmpSlotBar.SetSize(width, heigh)
		tmpSlotBar.SetPosition(x, y)

		tmpSlotBar.Show()
		return tmpSlotBar

	def Bar(self, parent, x, y, width, heigh):
		tmpBar = ui.Bar()

		if parent:
			tmpBar.SetParent(parent)

		tmpBar.SetPosition(x, y)
		tmpBar.SetSize(width, heigh)

		tmpBar.SetColor(0x77000000)

		tmpBar.Show()
		return tmpBar

	def Bar3D(self, parent, x, y, width, heigh, color):
		tmpBar3D = ui.Bar3D()

		if parent:
			tmpBar3D.SetParent(parent)
		if color:
			tmpBar3D.SetColor(color)

		tmpBar3D.SetPosition(x, y)
		tmpBar3D.SetSize(width, heigh)

		tmpBar3D.Show()
		return tmpBar3D

	def TitleBar(self, parent, x, y, width, color, event):
		tmpTitleBar = ui.TitleBar()

		if parent:
			tmpTitleBar.SetParent(parent)

		tmpTitleBar.SetPosition(x, y)
		tmpTitleBar.SetCloseEvent(event)

		tmpTitleBar.MakeTitleBar(width, color)
		tmpTitleBar.Show()
		return tmpTitleBar

	def HorizontalBar(self, parent, x, y, width):
		tmpHorizontalBar = ui.HorizontalBar()

		if parent:
			tmpHorizontalBar.SetParent(parent)

		tmpHorizontalBar.SetPosition(x, y)

		tmpHorizontalBar.Create(width)
		tmpHorizontalBar.Show()
		return tmpHorizontalBar

	def SliderBar(self, parent, x, y, width, heigh, event):
		tmpSliderBar = ui.SliderBar()

		if parent:
			tmpSliderBar.SetParent(parent)

		tmpSliderBar.SetPosition(x, y)
		tmpSliderBar.SetSize(width, heigh)
		tmpSliderBar.SetEvent(event)

		tmpSliderBar.Show()
		return tmpSliderBar

	def ProgressBar(self, parent, x, y, width, heigh, percentage):
		tmpProgressBar = ProgressBar()

		if parent:
			tmpProgressBar.SetParent(parent)

		tmpProgressBar.SetPosition(x, y)
		tmpProgressBar.SetSize(width, heigh)

		tmpProgressBar.SetPercentage(percentage)

		tmpProgressBar.Show()
		return tmpProgressBar

	def ScrollBar(self, parent, x, y, size):
		tmpScrollBar = ui.ScrollBar()

		if parent:
			tmpScrollBar.SetParent(parent)

		tmpScrollBar.SetPosition(x, y)
		tmpScrollBar.SetScrollBarSize(size)

		tmpScrollBar.Show()
		return tmpScrollBar

	###################################################################################################################
	# IMAGE
	###################################################################################################################
	def ImageBox(self, parent, image, x, y):
		tmpImage = ui.ImageBox()

		if parent:
			tmpImage.SetParent(parent)

		tmpImage.LoadImage(image)
		tmpImage.SetPosition(x, y)

		tmpImage.Show()
		return tmpImage

	def AniImageBox(self, parent, image, x, y):
		tmpAniImage = ui.AniImageBox()

		if parent:
			tmpAniImage.SetParent(parent)

		tmpAniImage.AppendImage(image)
		tmpAniImage.SetPosition(x, y)

		tmpAniImage.Show()
		return tmpAniImage

	def ExpandedImageBox(self, parent, image, x, y):
		tmpExpImageBox = ui.ExpandedImageBox()

		if parent:
			tmpExpImageBox.SetParent(parent)

		tmpExpImageBox.LoadImage(image)

		tmpExpImageBox.SetPosition(x,y)

		tmpExpImageBox.Show()
		return tmpExpImageBox

	###################################################################################################################
	# SLOT
	###################################################################################################################
	def SlotWindow(self, parent, x, y, width, heigh, emptyevent, selectItemEvent, itemInEvent, ItemOutEvent):
		tmpSlowWindow = ui.SlotWindow()

		if parent:
			tmpSlowWindow.SetParent(parent)

		tmpSlowWindow.SetSize(width, heigh)
		tmpSlowWindow.SetPosition(x, y)

		tmpSlowWindow.SetSelectEmptySlotEvent(emptyevent)
		tmpSlowWindow.SetSelectItemSlotEvent(selectItemEvent)
		tmpSlowWindow.SetOverInItemEvent(itemInEvent)
		tmpSlowWindow.SetOverOutItemEvent(ItemOutEvent)

		tmpSlowWindow.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)

		tmpSlowWindow.Show()
		return tmpSlowWindow

	def GridSlotWindow(self, parent, x, y, AddItemEvent, RemoveItemEvent, ItemInEvent, ItemOutEvent, SlotStartIndex, SlotXCount, SlotYCount, SlotXStep, SlotYStep, SlotXBlank, SlotYBlank):
		tmpGridSlotWindow = ui.GridSlotWindow()

		if parent:
			tmpGridSlotWindow.SetParent(parent)

		tmpGridSlotWindow.SetPosition(x, y)

		tmpGridSlotWindow.SetSelectEmptySlotEvent(ui.__mem_func__(AddItemEvent))
		tmpGridSlotWindow.SetSelectItemSlotEvent(ui.__mem_func__(RemoveItemEvent))
		tmpGridSlotWindow.SetOverInItemEvent(ui.__mem_func__(ItemInEvent))
		tmpGridSlotWindow.SetOverOutItemEvent(ui.__mem_func__(ItemOutEvent))

		tmpGridSlotWindow.ArrangeSlot(SlotStartIndex, SlotXCount, SlotYCount, SlotXStep, SlotYStep, SlotXBlank, SlotYBlank)
		tmpGridSlotWindow.RefreshSlot()
		tmpGridSlotWindow.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)

		tmpGridSlotWindow.Show()
		return tmpGridSlotWindow

	###################################################################################################################
	# COMMON
	###################################################################################################################
	def Popup(self, text, width, acceptevent):
		tmpPopup = uiCommon.PopupDialog()

		tmpPopup.SetText(str(text))
		if width: tmpPopup.SetWidth(width)
		if acceptevent: tmpPopup.SetAcceptEvent(acceptevent)

		tmpPopup.Open()
		return tmpPopup

	def PopupNotification(self, text, type = None):
		tmpPopupNotification = PopupNotificationMsg()

		if type:
			tmpPopupNotification.SetType(str(type))
		tmpPopupNotification.SetMsg(str(text))

		tmpPopupNotification.Show()
		return tmpPopupNotification

	def Input(self, text, length, acceptevent, cancelevent):
		tmpInput = uiCommon.InputDialog()

		tmpInput.SetTitle(str(text))
		tmpInput.SetMaxLength(length)

		if acceptevent: tmpInput.SetAcceptEvent(acceptevent)
		if cancelevent: tmpInput.SetCancelEvent(cancelevent)

		tmpInput.Open()
		return tmpInput

	def InputDescription(self, text, description,length, acceptevent, cancelevent):
		tmpInputDescription = uiCommon.InputDialogWithDescription()

		tmpInputDescription.SetTitle(str(text))
		tmpInputDescription.SetDescription(str(description))

		tmpInputDescription.SetMaxLength(length)

		if acceptevent: tmpInputDescription.SetAcceptEvent(acceptevent)
		if cancelevent: tmpInputDescription.SetCancelEvent(cancelevent)

		tmpInputDescription.Open()
		return tmpInputDescription

	def InputDescription2(self, text, description, description2, length, acceptevent, cancelevent):
		tmpInputDescription2 = uiCommon.InputDialogWithDescription2()

		tmpInputDescription2.SetTitle(str(text))
		tmpInputDescription2.SetDescription1(str(description))
		tmpInputDescription2.SetDescription2(str(description2))

		tmpInputDescription2.SetMaxLength(length)

		if acceptevent: tmpInputDescription2.SetAcceptEvent(acceptevent)
		if cancelevent: tmpInputDescription2.SetCancelEvent(cancelevent)

		tmpInputDescription2.Open()
		return tmpInputDescription2

	def Question(self, text, width, acceptevent, cancelevent):
		tmpQuestion = uiCommon.QuestionDialog()

		tmpQuestion.SetText(str(text))
		if width: tmpQuestion.SetWidth(width)

		if acceptevent: tmpQuestion.SAFE_SetAcceptEvent(acceptevent)
		if cancelevent: tmpQuestion.SAFE_SetCancelEvent(cancelevent)

		tmpQuestion.Open()
		return tmpQuestion

	def Question2(self, text, text2, width, acceptevent, cancelevent):
		tmpQuestion2 = uiCommon.QuestionDialog2()

		tmpQuestion2.SetText1(str(text))
		tmpQuestion2.SetText2(str(text2))

		if width: tmpQuestion2.SetWidth(width)

		if acceptevent: tmpQuestion2.SAFE_SetAcceptEvent(acceptevent)
		if cancelevent: tmpQuestion2.SAFE_SetCancelEvent(cancelevent)

		tmpQuestion2.Open()
		return tmpQuestion2

	def QuestionWithLimit(self, text, limit, width, acceptevent, cancelevent):
		tmpQuestionWithLimit = uiCommon.QuestionDialogWithTimeLimit()

		if width: tmpQuestionWithLimit.SetWidth(width)

		if acceptevent: tmpQuestionWithLimit.SAFE_SetAcceptEvent(acceptevent)
		if cancelevent: tmpQuestionWithLimit.SAFE_SetCancelEvent(cancelevent)

		tmpQuestionWithLimit.Open(str(text), limit)
		return tmpQuestionWithLimit

class ListBoxExItem(ui.ListBoxEx.Item):
	def __init__(self, text):
		ui.ListBoxEx.Item.__init__(self)
		self.canLoad=0
		self.text=text
		#self.textLine=self.__CreateTextLine(text[:75])
		self.textLine=self.__CreateTextLine(text)
	def __del__(self):
		ui.ListBoxEx.Item.__del__(self)
	def GetText(self):
		return self.text
	def SetSize(self, width, height):
		ui.ListBoxEx.Item.SetSize(self, 7*len(self.textLine.GetText()) + 4, height)
	def __CreateTextLine(self, text):
		textLine=ui.TextLine()
		textLine.SetParent(self)
		textLine.SetPosition(10, 4)
		textLine.SetText(text)
		textLine.Show()
		return textLine

class ComboBox(ui.ComboBox):
	def __init__(self):
		ui.ComboBox.__init__(self)
		self.value = []

	def __del__(self):
		ui.ComboBox.__del__(self)

	def InsertItem(self, index, name, value=None):
		self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()
		self.value.append(value)

	def OnSelectItem(self, index, name):
		self.SetCurrentItem(name)
		self.CloseListBox()
		self.event(index)

	def GetCurrentValue(self):
		return self.value[self.GetCurrentIndex()]

	def GetCurrentText(self):
		return self.textLine.GetText()

	def GetItemCount(self):
		return len(self.value)

class ListBoxEx2(ui.ListBoxEx):
	def __init__(self):
		ui.ListBoxEx.__init__(self)
		self.listbox_items = []
		self.listbox_array = []
		self.listbox_array_val = []

	def __del__(self):
		ui.ListBoxEx.__del__(self)

	def RemoveSelectedItem(self):
		itemCount = 0
		selItem=self.GetSelectedItem()
		if selItem:
			selected = selItem.GetText()

			for i in self.listbox_items:
				if i["item_name"] == selected:
					self.listbox_items.pop(itemCount)
					break
				itemCount += 1

			self.RemoveItem(selItem)

			self.RemoveAllItems()
			for item in self.listbox_items:
				self.AppendItem(Item(item["item_name"]))

	def AddItem(self, item_name):
		self.AppendItem(Item(item_name))
		listbox_data = {"item_name" : item_name}
		self.listbox_items.append(listbox_data)

	def AddItem2(self, text, value=None):
		self.AppendItem(Item(text))
		self.listbox_array.append(text)
		self.listbox_array_val.append(value)

	def RemoveSelectedItem2(self):
		selItem = self.GetSelectedItem()
		if selItem:
			sel = self.GetItemIndex(selItem)
			self.listbox_array.pop(sel)
			self.listbox_array_val.pop(sel)
			self.RemoveAllItems()
			for n in self.listbox_array:
				self.AppendItem(Item(n))

	def GetSelectedText2(self):
		selItem = self.GetSelectedItem()
		if selItem:
			sel = self.GetItemIndex(selItem)
			return self.listbox_array[sel]

	def GetSelectedValue2(self):
		selItem = self.GetSelectedItem()
		if selItem:
			sel = self.GetItemIndex(selItem)
			return self.listbox_array_val[sel]

	def GetItemCount(self):
		return len(self.itemList)

class ResizeableButton(ui.Window):
	DARK_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)

	WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)

	def __init__(self, layer = "UI"):
		ui.Window.__init__(self, layer)

		self.eventFunc = None
		self.eventArgs = None

		self.ButtonText = None
		self.ToolTipText = None

		self.isOver = False
		self.isSelected = False

		self.width = 0
		self.height = 0
		self.BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)

	def __del__(self):
		ui.Window.__del__(self)

		self.eventFunc = None
		self.eventArgs = None

	def SetSize(self, width, height):
		ui.Window.SetSize(self, width, height)
		self.width = width
		self.height = height

	def SetEvent(self, func, *args):
		self.eventFunc = func
		self.eventArgs = args

	def SetTextColor(self, color):
		if not self.ButtonText:
			return
		self.ButtonText.SetPackedFontColor(color)

	def SetColor(self, color):
		self.BACKGROUND_COLOR = color

	def SetText(self, text):
		if not self.ButtonText:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	def SetToolTipText(self, text, x=0, y = -19):
		if not self.ToolTipText:
			toolTip=ui.createToolTipWindowDict["TEXT"]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)

	def SetToolTipWindow(self, toolTip):
		self.ToolTipText = toolTip
		self.ToolTipText.SetParentProxy(self)

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.SetTop()
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()

	def OnMouseLeftButtonDown(self):
		self.isSelected = True

	def OnMouseLeftButtonUp(self):
		self.isSelected = False
		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)

	def OnUpdate(self):
		if self.IsIn():
			self.isOver = True
			self.ShowToolTip()
		else:
			self.isOver = False
			self.HideToolTip()

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()

		widthRender = self.width
		heightRender = self.height
		grp.SetColor(self.BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(self.DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(self.HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

			if self.isSelected:
				grp.SetColor(self.WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

class ProgressBar(ui.Window):

	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	DARK_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)

	PROGRESS_COLOR = grp.GenerateColor(1.0, 0.2, 0.2, 0.4)

	def __init__(self, layer = "UI"):
		ui.Window.__init__(self, layer)

		self.width = 0
		self.height = 0
		self.percentage = 0

		self.textLine = None

	def __del__(self):
		ui.Window.__del__(self)

	def GetPercentage(self):
		if self.textLine:
			return self.textLine.GetText()[1:]

	def SetSize(self, width, height):
		ui.Window.SetSize(self, width, height)
		self.width = width
		self.height = height

	def SetPercentage(self, num):
		if num > 100:
			self.percentage = 100
		elif num < 0:
			self.percentage = 0
		else:
			self.percentage = num

		if not self.textLine:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.textLine = textLine

		self.textLine.SetText('%' + str("%.2f" % (self.percentage)))

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()

		widthRender = self.width
		heightRender = self.height
		grp.SetColor(self.BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(self.DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		grp.SetColor(self.PROGRESS_COLOR)
		grp.RenderBar(xRender + 2, yRender + 2, (self.width - 3) * self.percentage / 100, heightRender - 3)

class CheckBox(ui.Window):

	checked = 0
	eventUp  =None
	eventDown=None

	def __init__(self,cont = ""):
		ui.Window.__init__(self)
		self.BGImage = ui.ImageBox()
		self.BGImage.SetParent(self)
		self.BGImage.LoadImage("x.tga")
		self.BGImage.SetPosition(0, 0)
		self.BGImage.Show()

		self.BGImage2 = ui.ImageBox()
		self.BGImage2.SetParent(self)
		self.BGImage2.LoadImage("tick.tga") #secili
		self.BGImage2.SetPosition(0, 0)
		self.BGImage2.Hide()

		self.BG = ui.Bar("UI")
		self.BG.SetParent(self)
		self.BG.SetPosition(0,0)
		self.BG.SetSize(20,20)
		self.BG.SetColor(0xFFFFFF)
		self.BG.SetTop()
		self.BG.OnMouseLeftButtonUp = self.Toggle
		self.OnMouseLeftButtonUp = self.Toggle
		self.BG.Show()

		self.Title = ui.TextLine()
		self.Title.SetParent(self)
		self.Title.SetPosition(25,2)
		self.Title.SetText(cont)
		self.Title.Show()

		self.SetSize(25+self.Title.GetTextSize()[0]+5,20)

	def __del__(self):
		ui.ToggleButton.__del__(self)

	def Toggle(self):
		if self.checked == 1:
			self.OnToggleUp()
		else:
			self.OnToggleDown()

	def OnToggleUp(self):
		self.BGImage2.Hide()
		self.BGImage.Show()
		self.BG.Hide()
		self.BG.Show()

		self.checked = 0

		if self.eventUp:
			self.eventUp()

	def OnToggleDown(self):
		self.BGImage.Hide()
		self.BGImage2.Show()
		self.BG.Hide()
		self.BG.Show()

		self.checked = 1
		if self.eventDown:
			self.eventDown()

class NewListBox(ui.Window):
	dropped  = 0
	dropstat = 0
	last = 0
	lastS = 0
	maxh = 95
	OnChange = None
	class Item(ui.ListBoxEx.Item):
		def __init__(self,parent, text,value=0):
			ui.ListBoxEx.Item.__init__(self)

			self.textBox=ui.TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			self.textBox.Show()

			self.value = value
		def GetValue(self):
			return self.value
		def GetText(self):
			return self.textBox.GetText()
		def __del__(self):
			ui.ListBoxEx.Item.__del__(self)

	def __init__(self,parent,tt = "",down=1):
		ui.Window.__init__(self,"TOP_MOST")
		self.down = down
		self.SetParentProxy(parent)

		self.bg = ui.Bar("TOP_MOST")
		self.bg.SetParent(self)
		self.bg.SetPosition(0,0)
		self.bg.SetColor(0xc0000000)
		self.bg.OnMouseOverIn = self.bgMouseIn
		self.bg.OnMouseOverOut = self.bgMouseOut
		self.bg.OnMouseLeftButtonDown = self.ExpandMe
		self.bg.Show()

		self.act = ui.TextLine()
		self.act.SetParent(self.bg)
		self.act.SetPosition(4,2)
		self.act.SetText(tt)
		self.act.Show()
		self.GetText = self.act.GetText

		self.Drop = ui.Bar("TOP_MOST")
		self.Drop.SetParent(self.GetParentProxy())
		self.Drop.SetPosition(0,21)
		self.Drop.SetSize(150,0)
		self.Drop.SetColor(0xff0a0a0a)

		self.ScrollBar = ui.ThinScrollBar()
		self.ScrollBar.SetParent(self.Drop)
		self.ScrollBar.SetPosition(132,0)
		self.ScrollBar.SetScrollBarSize(0)

		self.DropList = ui.ListBoxEx()
		self.DropList.SetParent(self.Drop)
		self.DropList.itemHeight = 12
		self.DropList.itemStep = 13
		self.DropList.SetPosition(0,0)
		self.DropList.SetSize(132,13)
		self.DropList.SetScrollBar(self.ScrollBar)
		self.DropList.SetSelectEvent(self.SetTitle)
		self.DropList.SetViewItemCount(0)
		self.DropList.Show()

		if tt != "":
			self.AppendItemAndSelect(tt)

		self.selected = self.DropList.GetSelectedItem()

		self.SetSize(120,20)

	def __del__(self):
		del self.bg
		del self.Drop
		ui.Window.__del__(self)

	def Destroy(self):
		self.DropList = None
		self.ScrollBar = None
		self.Drop = None
		self.act = None
		self.bg = None

	c = 1
	def AppendItem(self,text,value=0):
		self.c+=1
		self.DropList.AppendItem(self.Item(self,text,value))
		self.maxh = min(95,13*self.c)
		if self.c > 7:
			self.ScrollBar.Show()

	def AppendItemAndSelect(self,text,value=0):
		self.DropList.AppendItem(self.Item(self,text,value))
		self.DropList.SelectIndex(len(self.DropList.itemList)-1)

	def SelectByAffectId(self,id):
		for x in self.DropList.itemList:
			if x.value == id:
				self.DropList.SelectItem(x)
				break

	def SetTitle(self,item):
		self.act.SetText(str(item.textBox.GetText()))
		self.last = self.DropList.basePos
		self.lastS = self.ScrollBar.GetPos()
		self.dropped = 0
		self.selected = item
		if self.OnChange:
			self.OnChange()

	def SetPosition(self,w,h):
		ui.Window.SetPosition(self,w,h)
		if self.down == 1:
			self.Drop.SetPosition(w,h+21)
		else:
			self.Drop.SetPosition(w,h-self.Drop.GetHeight())

	def SetSize(self,w,h):
		ui.Window.SetSize(self,w,h)
		self.bg.SetSize(w,h)
		self.Drop.SetSize(w,0)
		self.DropList.SetSize(w-18,self.maxh)
		for x in self.DropList.itemList:
			x.SetSize(w-18,12)
		self.ScrollBar.SetPosition(w-18,0)

	def ExpandMe(self):
		if self.dropped == 1:
			self.dropped = 0
		else:
			self.dropped = 1

	def OnUpdate(self):
		iter = 6
		if self.Drop.GetHeight() < 50:
			self.ScrollBar.Hide()
		else:
			self.ScrollBar.Show()

		if self.dropped == 0 and self.dropstat == 1:
			if self.Drop.GetHeight() <=0:
				self.dropstat = 0
				self.Drop.SetSize(self.Drop.GetWidth(),0)
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
				self.Drop.Hide()
			else:
				if self.Drop.GetHeight()-iter < 0:
					self.Drop.SetSize(self.Drop.GetWidth(),0)
				else:
					self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()-iter)
					(w,h) = self.GetLocalPosition()
					self.SetPosition(w,h)


				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		elif self.dropped == 1 and self.dropstat == 0:
			self.Drop.Show()
			self.SetTop()
			if self.Drop.GetHeight() >=self.maxh:
				self.Drop.SetSize(self.Drop.GetWidth(),self.maxh)
				self.ScrollBar.SetScrollBarSize(self.maxh)
				self.dropstat = 1
				self.DropList.SetViewItemCount(7)
				self.ScrollBar.SetPos(self.lastS)
			else:
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight()+iter)
				self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()+iter)
				(w,h) = self.GetLocalPosition()
				self.SetPosition(w,h)
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)

	## BG Hover
	def bgMouseIn(self):
		self.bg.SetColor(0xc00a0a0a)

	def bgMouseOut(self):
		self.bg.SetColor(0xc0000000)

	itemCount = 9
	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	DARK_COLOR = grp.GenerateColor(0.3, 0.3, 0.3, 1.0)
	DARK_COLOR2 = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
	BRIGHT_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)
	SELECT_COLOR = grp.GenerateColor(0.9, 0.03, 0.01, 0.4)
	WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)
class NewListBox2(ui.ScriptWindow):
	class ModifiedListBox(ui.ListBox):
		def __init__(self):
			ui.ListBox.__init__(self)
			self.selectedLine = -1

		def GetSelectedItemText(self):
			return self.textDict.get(self.selectedLine, "")

		def GetSelectedLine(self):
			return self.selectedLine

		def ArrangeItem(self):
			self.SetSize(self.width, itemCount*self.stepSize)
			self._LocateItem()

		def SelectItem2(self, line):
			self.selectedLine = line

		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2

			if -1 != self.overLine:
				if self.overLine - self.basePos < self.showLineCount:
					grp.SetColor(HALF_WHITE_COLOR)
					grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)

			if -1 != self.selectedLine:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.x = 0
		self.y = 0
		self.width = 0
		self.height = 0
		self.isSelected = False
		self.isOver = False
		self.isListOpened = False
		self.event = lambda *arg: None
		self.openEvent = lambda *arg: None
		self.closeEvent = lambda *arg: None
		self.enable = True

		self.ToolTipText = None

		self.imgLeft = ui.ImageBox()
		self.imgLeft.AddFlag("not_pick")
		self.imgLeft.SetParent(self)
		self.imgLeft.LoadImage("d:/ymir work/ui/game/taskbar/quickslot_downbutton_01.sub")
		self.imgLeft.Show()

		self.textLine = ui.TextLine()
		self.textLine.SetText("Nesne")
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetPosition(0,2)
		self.textLine.SetOutline()
		self.textLine.Show()

		self.bar = ui.Bar3D()
		self.bar.SetParent(self)
		self.bar.Hide()

		self.listBox = self.ModifiedListBox()
		self.listBox.SetParent(self.bar)
		self.listBox.SetEvent(ui.__mem_func__(self.OnSelectItem))
		self.listBox.SetPickAlways()
		self.listBox.Hide()

		self.scrollBar = ui.SmallThinScrollBar()
		self.scrollBar.SetParent(self.bar)
		self.scrollBar.SetScrollEvent(ui.__mem_func__(self.__OnScroll))
		#self.scrollBar.SetPickAlways()
		self.scrollBar.Hide()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.textLine = None
		self.listBox = None

	def SetPosition(self, x, y):
		ui.ScriptWindow.SetPosition(self, x, y)
		self.x = x
		self.y = y
		self.__ArrangeListBox()

	def SetSize(self, width, height):
		ui.ScriptWindow.SetSize(self, width, height)
		self.width = width
		self.height = height
		self.textLine.UpdateRect()
		self.imgLeft.SetPosition(width-13,7)
		self.__ArrangeListBox()

	def SetViewItemCount(self, x):
		self.listBox.SetViewItemCount(x)

##
	def SetFormToolTipText(self, type, text, x, y):
		if not self.ToolTipText:
			toolTip = ui.createToolTipWindowDict[type]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)

	def SetToolTipWindow(self, toolTip):
		self.ToolTipText = toolTip
		self.ToolTipText.SetParentProxy(self)

	def SetToolTipText(self, text, x=0, y = -19):
		self.SetFormToolTipText("TEXT", text, x, y)

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()

	def __OnScroll(self):
		scrollLen = self.listBox.GetItemCount()-self.listBox.GetViewItemCount()
		if scrollLen < 0:
			scrollLen = 0
		self.listBox.SetBasePos(int(self.scrollBar.GetPos()*scrollLen))

	def SetViewBase(self, selectedLine):
		if not selectedLine:
			self.scrollBar.SetPos(0.0)
			return
		self.scrollBar.SetPos(selectedLine/float(self.lenList-2))

	def __ArrangeListBox(self):
		self.bar.SetPosition(0, self.height + 5)
		self.bar.SetSize(self.width, 4+itemCount*17)

		self.listBox.SetPosition(0, 1)
		self.listBox.SetWidth(self.width-13)

		self.scrollBar.SetPosition(self.width-14,3)
		self.scrollBar.SetScrollBarSize(itemCount*17)

	def Enable(self):
		self.enable = True

	def Disable(self):
		self.enable = False
		self.textLine.SetText("")
		self.CloseListBox()

	def SetEvent(self, event):
		self.event = event

	def ClearItem(self):
		self.CloseListBox()
		self.listBox.ClearItem()

	def InsertItem(self, name, index):
		self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()
		#self.lenList += 1

	def InsertList(self, list):
		for name, index in list:
			self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()
		self.lenList = len(list)

	def SetCurrentItem(self, text):
		self.textLine.SetText(text)

	def SelectItem(self, key):
		self.listBox.SelectItem(key)

	def SelectItem2(self, key):
		self.listBox.SelectItem2(key)

	def OnSelectItem(self, index, name):
		self.CloseListBox()
		self.SetCurrentItem(name)
		self.event = index

	def CloseListBox(self):
		self.isListOpened = False
		if self.closeEvent:
			self.closeEvent()
		ui.ScriptWindow.SetSize(self, self.width, self.height)
		self.bar.Hide()

	def OnMouseLeftButtonDown(self):
		if not self.enable:
			return

		self.isSelected = True

	def SelectTrue(self):
		self.isSelected = True

	def GetSelectedLine(self):
		return self.listBox.GetSelectedLine()

	def GetSelectedItem(self):
		return self.listBox.GetSelectedItem()

	def GetSelectedItemText(self):
		return self.listBox.GetSelectedItemText()

	def OnMouseLeftButtonUp(self):
		if not self.enable:
			return

		self.isSelected = False

		if self.isListOpened:
			self.CloseListBox()
		else:
			if self.listBox.GetItemCount() > 0:
				if self.openEvent:
					self.openEvent()

				self.isListOpened = True

				ui.ScriptWindow.SetSize(self, self.width, self.height+self.bar.GetHeight())
				self.bar.Show()

				self.listBox.ArrangeItem()
				self.listBox.LocateItem()
				self.listBox.Show()

				self.scrollBar.Show()

	def OnUpdate(self):
		if not self.enable:
			return

		if self.IsIn():
			self.isOver = True
			self.ShowToolTip()
		else:
			self.isOver = False
			self.HideToolTip()

	def OnRender(self):
		self.x, self.y = self.GetGlobalPosition()
		xRender = self.x
		yRender = self.y
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

		if self.isSelected:
			grp.SetColor(WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

	def IsIn(self):
		return ui.Window.IsIn(self)

class WaitingDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		pass
		#self.x=_ui().Window(None, wndMgr.GetScreenWidth()/2 - 125, wndMgr.GetScreenHeight()/2 - 52, 0, 0)

	def Open(self, waitTime):
		curTime = time.clock()
		self.endTime = curTime + waitTime

		self.Show()

	def Close(self):
		self.Hide()

	def Destroy(self):
		self.Hide()

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)

	def OnUpdate(self):
		lastTime = max(0, self.endTime - time.clock())
		if 0 == lastTime:
			self.Close()
			self.eventTimeOver()
		else:
			return

	def OnPressExitKey(self):
		self.Close()
		return True


class PopupNotificationMsg(ui.Window):
	TITLE_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
	SPECIAL_TITLE_COLOR = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
	NORMAL_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)

	def __init__(self):
		ui.Window.__init__(self)
		self.BuildWindow()
		self.cikis_tamamla = 0
		self.kapatma_islemi = 0

	def __del__(self):
		ui.Window.__del__(self)

	def BuildWindow(self):
		self.Board = ui.ThinBoard()
		self.Board.SetSize(300, 50)
		self.Board.SetPosition(wndMgr.GetScreenWidth() - 310, wndMgr.GetScreenHeight())
		self.Board.Show()
		self.comp = _ui()

		self.text_title = self.comp.LargeTextLine(self.Board, "Notification warning", 45, 8, self.SPECIAL_TITLE_COLOR)
		self.textline = self.comp.TextLine(self.Board, "NO_TEXT", 45, 25)
		#self._ProgressBar = self.comp.ProgressBar(self.Board, 150, 30, 133, 11, 0)

		self.WaitPls = WaitingDialog()
		self.WaitPls.Open(5.0)
		self.WaitPls.SAFE_SetTimeOverEvent(self.Kapat)

	def OpenWindow(self):
		if self.Board.IsShow():
			self.Board.Hide()
		else:
			self.Board.Show()

	def Close(self):
		self.Board.Hide()

	def Kapat(self):
		if not self.cikis_tamamla:
			self.cikis_tamamla = 1

	def Sonlandir(self):
		self.Close()

	def OnUpdate(self):
		x, y = self.Board.GetGlobalPosition()

		if y != wndMgr.GetScreenHeight() - 90:
			self.Board.SetPosition(x, y - 1)

		if self.cikis_tamamla:
			if y != wndMgr.GetScreenHeight():
				self.Board.SetPosition(x, y + 1)
			else:
				if not self.kapatma_islemi:
					self.kapatma_islemi = 1
					self.Sonlandir()
		else:
			pass
			#self._ProgressBar.SetPercentage(float(self._ProgressBar.GetPercentage()) + 0.33)


	def SetMsg(self, arg):
		self.textline.SetText(arg.replace("_"," "))

	def SetType(self, type, add = ""):
		if type == 1:
			self.text_title.SetText("Bilgilendirme")
		elif type == 2:
			self.text_title.SetText("Biyalog Uyarisi")
			self.img = self.comp.ExpandedImage(self.Board, 8, 8, "icon\\item\\%s.tga" % (str(add)))


class ChooseDialog(ui.BoardWithTitleBar):
	def __init__(self):
		ui.BoardWithTitleBar.__init__(self)

		self.ItemList = []
		self.ItemValueList = []

		self.GuiFunc()

	def __del__(self):
		ui.BoardWithTitleBar.__del__(self)

	def BuildWindow(self):
		self.SetSize(230, 342)
		self.AddFlag('movable')
		self.AddFlag('float')
		self.SetTitleName('Choose')
		self.SetCloseEvent(self.Close)
		self.comp = _ui()

		self.OKButton = self.comp.BasicButton(self, 'OK', 54, 309, self.SetAcceptEvent)
		self.CancelButton = self.comp.BasicButton(self, 'Cancel', 120, 309, self.SetCancelEvent)
		self.bar_listBox, self.list_listBox, self.scroll_listBox = self.comp.ListBoxExFull(self, 12, 34, 206, 268)
		self.list_listBox.SetViewItemCount(13)

	def AddItem(self, text, value=0):
		self.list_listBox.AppendItem(Item(text))
		self.ItemList.append(text)
		self.ItemValueList.append(value)

	def RemoveSelectedItem(self):
		selItem = self.list_listBox.GetSelectedItem()
		if selItem:
			sel = self.list_listBox.GetItemIndex(selItem)
			self.ItemList.pop(sel)
			self.ItemValueList.pop(sel)
			self.list_listBox.RemoveAllItems()
			for n in self.ItemList:
				self.list_listBox.AppendItem(Item(n))

	def GetSelectedText(self):
		selItem = self.list_listBox.GetSelectedItem()
		if selItem:
			sel = self.list_listBox.GetItemIndex(selItem)
			return self.ItemList[sel]

	def GetSelectedValue(self):
		selItem = self.list_listBox.GetSelectedItem()
		if selItem:
			sel = self.list_listBox.GetItemIndex(selItem)
			return self.ItemValueList[sel]

	def SetAcceptEvent(self, event):
		self.OKButton.SetEvent(event)

	def SetCancelEvent(self, event):
		self.CancelButton.SetEvent(event)

	def SetAcceptText(self, text):
		self.OKButton.SetText(text)

	def SetCancelText(self, text):
		self.CancelButton.SetText(text)

	def SetTitleText(self, text):
		self.SetTitleName(text)

	def SetBoardPosition(self, x, y):
		self.SetPosition(x, y)

	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True
