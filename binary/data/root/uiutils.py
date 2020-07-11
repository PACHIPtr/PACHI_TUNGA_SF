import net
import app
import player
import item
import chat
import ui
import uiScriptLocale
import time
import snd
import localeInfo
import mouseModule
import constInfo
import dbg
import os
import grp
import uiCommon
import uiToolTip
class Edit(ui.EditLine):
	def __init__(self,main = "",ml = 99):
		ui.EditLine.__init__(self)
		self.SetText(main)
		self.main = main
		self.SetMax(ml)
		self.SetUserMax(ml)
	def GetText(self):
		res = ui.EditLine.GetText(self)
		if res == "":
			return "0"
		else:
			return res
			
	def __del__(self):
		ui.EditLine.__del__(self)
	def OnSetFocus(self):
		ui.EditLine.OnSetFocus(self)
		if ui.EditLine.GetText(self) == self.main:
			self.SetText("")
	def OnKillFocus(self):
		ui.EditLine.OnKillFocus(self)
		if ui.EditLine.GetText(self) == "":
			self.SetText(self.main)	
class Edit2(ui.EditLine):
	def __init__(self,parent,text,x,y,width,height,number=False,slot=2,max = 12):
		ui.EditLine.__init__(self)
		self.imageSlot=ui.MakeImageBox(parent, "d:/ymir work/ui/public/Parameter_Slot_0"+str(slot)+".sub", x,y)
		self.SetText(text)
		self.main = text
		self.SetMax(max)
		self.SetUserMax(max)
		self.SetParent(self.imageSlot)
		if number:
			self.SetNumberMode()
		
		self.SetSize(width,height)
		self.SetPosition(2,2)
		self.Show()
	def GetText(self):
		res = ui.EditLine.GetText(self)
		if res == "":
			return ""
		else:
			return res
			
	def __del__(self):
		ui.EditLine.__del__(self)
	#def OnSetFocus(self):
		#ui.EditLine.OnSetFocus(self)
		#if ui.EditLine.GetText(self) == self.main:
	#		self.SetText("")
	#def OnKillFocus(self):
	#	ui.EditLine.OnKillFocus(self)
	#	if ui.EditLine.GetText(self) == "":
	#		self.SetText(self.main)

