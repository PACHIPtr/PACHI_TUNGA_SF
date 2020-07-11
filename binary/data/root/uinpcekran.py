import net
import ui
import app
import chrmgr


class NPCEkran(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0

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
			
			self.silahci = GetObject("weapon_shop")
			self.silahci.SAFE_SetEvent(self.butonlar,1)

			self.zirhci = GetObject("armor_shop")
			self.zirhci.SAFE_SetEvent(self.butonlar,11)

			self.market = GetObject("shop")
			self.market.SAFE_SetEvent(self.butonlar,21)

			self.bio = GetObject("bio_shop")
			self.bio.SAFE_SetEvent(self.butonlar,26)

			self.balikci = GetObject("fish_shop")
			self.balikci.SAFE_SetEvent(self.butonlar,41)

			self.simyaci = GetObject("alchemy_shop")
			self.simyaci.SAFE_SetEvent(self.butonlar,51)

			self.seyyar = GetObject("wedding_shop")
			self.seyyar.SAFE_SetEvent(self.butonlar,441)

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("UIScript/npcekran.py")

		self.__Load_BindObject()

		#self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		
	def butonlar(self, npc):
		net.SendChatPacket("/open_range_npc " + str(npc))


	def OnPressEscapeKey(self):
		self.Close()
		return True


	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
