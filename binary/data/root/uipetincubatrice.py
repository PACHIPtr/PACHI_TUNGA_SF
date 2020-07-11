import player
import net
import app
import item
import chat
import ime
import chr
import os
import ui
import mouseModule
import snd
import grp
import uiScriptLocale
import localeInfo
import constInfo
import wndMgr
import uiCommon

class PetSystemIncubator(ui.ScriptWindow):
	
	def __init__(self, new_pet):
		ui.ScriptWindow.__init__(self)
		self.__LoadWindow()
		self.LoadPetIncubatorImg(new_pet)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
	
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PetHatchingWindow.py")
		except:
			import exception as exception
			exception.Abort("PetHatchingWindow.LoadWindow.LoadObject")
			
		try:
			self.board = self.GetChild("board")
			self.boardtitle = self.GetChild("PetHatching_TitleBar")
			
			self.petimg = self.GetChild("HatchingItemSlot")
			self.petname = self.GetChild("pet_name")
			self.HatchingButton = self.GetChild("HatchingButton")
			
			
			#Event
			self.boardtitle.SetCloseEvent(ui.__mem_func__(self.Close))
			self.HatchingButton.SetEvent(ui.__mem_func__(self.RequestHatching,))
			
			
		except:
			import exception as exception
			exception.Abort("PetHatchingWindow.LoadWindow.BindObject")
			
	def LoadPetIncubatorImg(self, new_pet):
		petarryname = ["Maymun Yumurtasý", "Örümcek Yumurtasý", "Razadör Yumurtasý", "Nemere Yumurtasý", "Mavi Ejderha Yumurtasý", "Kýzýl Ejderha Yumurtasý", "Azrail Yumurtasý", "Cellatçýk Yumurtasý", "Baashido Yumurtasý", "Nessie Yumurtasý"]
		petarryimg = [55701, 55702, 55703, 55704, 55705, 55706, 55707, 55708, 55709, 55410]
		chat.AppendChat(chat.CHAT_TYPE_INFO, "[Pet-Kuluçka] "+petarryname[int(new_pet)]+".")
		self.petimg.SetItemSlot(0,petarryimg[int(new_pet)], 0)
		self.petimg.SetAlwaysRenderCoverButton(0, True)
		#self.petimg.SetSlotBaseImage("icon/item/"+petarryimg[new_pet], 0, 0, 0, 0)
		
	def RequestHatching(self):       
		if self.petname.GetText() == "" or len(self.petname.GetText()) < item.PET_NAME_MIN_SIZE or len(self.petname.GetText()) > item.PET_NAME_MAX_SIZE:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PET_SYSTEM_CORRECT_PET_NAME)
			return     

		if player.GetElk() < item.PET_HATCHING_MONEY:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PET_SYSTEM_CORRECT_MONEY)
			return

		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.PET_SYSTEM_ACCEPT_NAME % (str(self.petname.GetText())))
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.__Yes))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.__No))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def __Yes(self):
		chr.RequestPetName(self.petname.GetText())
		self.questionDialog.Close()
		self.Close()

	def __No(self):
		self.questionDialog.Close()


			


