import net
import app
import chrmgr
import event
import chat

import ui

class BiyologEkran(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0
		self.secim = 0
		self.seviye = 0
		self.secimbutonlari = []

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
			self.baslik = GetObject("titlename")
			self.af1 = GetObject("afv1")
			self.af2 = GetObject("afv2")
			self.af3 = GetObject("afv3")
			self.odulver = GetObject("odulver")
			self.secimbutonlari.append(GetObject("odul1"))
			self.secimbutonlari.append(GetObject("odul2"))
			self.secimbutonlari.append(GetObject("odul3"))
			self.secimbutonlari[0].SAFE_SetEvent(self.__odulsec,0)
			self.secimbutonlari[1].SAFE_SetEvent(self.__odulsec,1)	
			self.secimbutonlari[2].SAFE_SetEvent(self.__odulsec,2)			
			self.odulver.SAFE_SetEvent(self.vergitsin)	
		
		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/biyologekran.py")
		self.__Load_BindObject()
		self.SetCenterPosition()
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def vergitsin(self):
		net.SendChatPacket("/biyologver " + str(self.seviye) + " " + str(self.secim))
	
	def __odulsec(self, odul):
		if int(odul) == 0:
			self.secimbutonlari[0].Down()
			self.secimbutonlari[1].SetUp()
			self.secimbutonlari[2].SetUp()
			self.secim = 1
		elif int(odul) == 1:
			self.secimbutonlari[1].Down()
			self.secimbutonlari[0].SetUp()
			self.secimbutonlari[2].SetUp()
			self.secim = 2
		else:
			self.secimbutonlari[2].Down()
			self.secimbutonlari[1].SetUp()
			self.secimbutonlari[0].SetUp()
			self.secim = 3
		return

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def SetBaslik(self, baslik):
		self.baslik.SetText(str(baslik)+ " Görevi Ödülü")
		self.seviye = int(baslik)
		
	def SetOdul(self, aff1, affv1, aff2, affv2, aff3, affv3):
		self.af1.SetText("Max HP +"+str(affv1))
		self.af2.SetText("Savunma Deðeri +"+str(affv2))
		self.af3.SetText("Saldýrý Deðeri +"+str(affv3))

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
