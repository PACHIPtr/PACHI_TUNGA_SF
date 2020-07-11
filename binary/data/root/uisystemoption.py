import net
import app
import player
import chrmgr
import systemSetting
import chat
import background
import ui
import snd
import localeInfo
import constInfo
import musicInfo
import uiSelectMusic
import math
import dbg
import os
import uiPrivateShopBuilder

MUSIC_FILENAME_MAX_LEN = 25

blockMode = 0

class PopupDialog(ui.ScriptWindow):
	def __init__(self, parent):
		print "PopupDialog::PopupDialog()"
		ui.ScriptWindow.__init__(self)

		self.__Load()
		self.__Bind()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print "PopupDialog::~PopupDialog()"

	def __Load(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")
		except:
			import exception as exception
			exception.Abort("PopupDialog.__Load")

	def __Bind(self):
		try:
			self.textLine=self.GetChild("message")
			self.okButton=self.GetChild("accept")
		except:
			import exception as exception
			exception.Abort("PopupDialog.__Bind")

		self.okButton.SAFE_SetEvent(self.__OnOK)

	def Open(self, msg):
		self.textLine.SetText(msg)
		self.SetCenterPosition()
		self.Show()
		self.SetTop()

	def __OnOK(self):
		self.Hide()

class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		if app.ENABLE_FOG_FIX:
			self.RefreshFogMode()
		self.RefreshNightOption()
		self.RefreshTextures()
		self.RefreshSash()
		self.RefreshCostume()
		self.RefreshCostumeW()
		self.RefreshCostumeH()
		self.RefreshLevel()
		self.RefreshHighlight()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE SYSTEM OPTION DIALOG"

	def __Initialize(self):
		self.tilingMode = 0
		self.titleBar = 0
		self.changeMusicButton = 0
		self.selectMusicFile = 0
		self.ctrlMusicVolume = 0
		self.ctrlSoundVolume = 0
		self.musicListDlg = 0
		self.tilingApplyButton = 0
		self.cameraModeButtonList = []
		self.fogModeButtonList = []
		self.tilingModeButtonList = []
		self.ctrlShadowQuality = 0
		self.nightModeOptionButtonList = []
		self.ctrlShopNamesRange = 0
		self.texturesOptionButtonList = []
		self.snowModeButton = []
		self.fonttypeButtonList = []
		if app.ENABLE_GRAPHIC_MASK:
			self.graphicMaskButtonList = []
		self.viewSashButtonList = []
		self.viewCostumeButtonList = []
		self.viewCostumeWButtonList = []
		self.viewCostumeHButtonList = []
		self.viewLevelButtonList = []
		self.highlightButtonList = []
		## Renewal ##
		self.option_page = None
		
	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY SYSTEM OPTION DIALOG"

	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception as exception
			exception.Abort("System.OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			# name #
			self.ctrlMusicVolumeName = GetObject("music_name")
			self.ctrlSoundVolumeName = GetObject("sound_name")
			self.ctrlShadowQualityName = GetObject("shadow_mode")
			self.ctrlShopNamesRangeName = GetObject("shop_name_range")
			self.changeMusicButtonName = GetObject("bgm_file")
			self.cameraModeButtonName = GetObject("camera_mode")
			self.fogModeButtonName = GetObject("fog_mode")
			self.tilingModeButtonName = GetObject("tiling_mode")
			self.nightModeOptionButtonName = GetObject("night_mode")
			self.texturesOptionButtonName = GetObject("textures_mode")
			self.snowModeName = GetObject("snow_mode")
			self.fonttypeButtonName = GetObject("font_type_name")
			self.viewCostumeButtonName = GetObject("viewcostume_on_off")
			self.viewCostumeWButtonName = GetObject("viewcostumew_on_off")
			self.viewCostumeHButtonName = GetObject("viewcostumeh_on_off")
			self.viewSashButtonName = GetObject("sash_on_off")
			self.graphicMaskButtonName = GetObject("background_text")
			self.viewLevelButtonName = GetObject("level_on_off")
			self.highlightButtonName = GetObject("highlight_on_off")
			# endname #
			self.selectMusicFile = GetObject("bgm_file")
			self.changeMusicButton = GetObject("bgm_button")
			self.ctrlMusicVolume = GetObject("music_volume_controller")
			self.ctrlSoundVolume = GetObject("sound_volume_controller")			
			self.cameraModeButtonList.append(GetObject("camera_short"))
			self.cameraModeButtonList.append(GetObject("camera_long"))
			if app.ENABLE_FOG_FIX:
				self.fogModeButtonList.append(GetObject("fog_on"))
				self.fogModeButtonList.append(GetObject("fog_off"))
			else:
				self.fogModeButtonList.append(GetObject("fog_level0"))
				self.fogModeButtonList.append(GetObject("fog_level1"))
				self.fogModeButtonList.append(GetObject("fog_level2"))
			self.tilingModeButtonList.append(GetObject("tiling_cpu"))
			self.tilingModeButtonList.append(GetObject("tiling_gpu"))
			self.tilingApplyButton=GetObject("tiling_apply")
			self.ctrlShadowQuality = GetObject("shadow_bar")
			self.nightModeOptionButtonList.append(GetObject("night_on"))
			self.nightModeOptionButtonList.append(GetObject("night_off"))
			self.ctrlShopNamesRange = GetObject("salestext_range_controller")
			self.texturesOptionButtonList.append(GetObject("texture_classic"))
			self.texturesOptionButtonList.append(GetObject("texture_snow"))
			self.texturesOptionButtonList.append(GetObject("texture_desert"))
			self.snowModeButton.append(GetObject("snow_mode_on"))
			self.snowModeButton.append(GetObject("snow_mode_off"))
			self.fonttypeButtonList.append(GetObject("font_type_arial"))
			self.fonttypeButtonList.append(GetObject("font_type_tahoma"))
			self.viewSashButtonList.append(GetObject("sash_on_button"))
			self.viewSashButtonList.append(GetObject("sash_off_button"))	
			self.viewCostumeButtonList.append(GetObject("viewcostume_on_button"))
			self.viewCostumeButtonList.append(GetObject("viewcostume_off_button"))
			self.viewCostumeWButtonList.append(GetObject("viewcostumew_on_button"))
			self.viewCostumeWButtonList.append(GetObject("viewcostumew_off_button"))
			self.viewCostumeHButtonList.append(GetObject("viewcostumeh_on_button"))
			self.viewCostumeHButtonList.append(GetObject("viewcostumeh_off_button"))
			self.viewLevelButtonList.append(GetObject("viewlevel_on_button"))
			self.viewLevelButtonList.append(GetObject("viewlevel_off_button"))
			self.highlightButtonList.append(GetObject("highlight_on_button"))
			self.highlightButtonList.append(GetObject("highlight_off_button"))
			if app.ENABLE_GRAPHIC_MASK:
				for graphicMaskName in ('object', 'cloud', 'water', 'tree'):
					self.graphicMaskButtonList.append(GetObject('background_{:s}'.format(graphicMaskName)))
					
			self.NextPage = GetObject("NextPage")
			self.PrevPage = GetObject("PrevPage")
		except:
			import exception as exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/systemoptiondialog.py")
		self.__Load_BindObject()
		
		self.SetCenterPosition()
		
		self.popupDialog = PopupDialog(self)
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.ctrlMusicVolume.SetSliderPos(float(systemSetting.GetMusicVolume()))
		self.ctrlMusicVolume.SetEvent(ui.__mem_func__(self.OnChangeMusicVolume))

		self.ctrlSoundVolume.SetSliderPos(float(systemSetting.GetSoundVolume()) / 5.0)
		self.ctrlSoundVolume.SetEvent(ui.__mem_func__(self.OnChangeSoundVolume))

		self.ctrlShadowQuality.SetSliderPos(float(systemSetting.GetShadowLevel()) / 5.0)
		self.ctrlShadowQuality.SetEvent(ui.__mem_func__(self.OnChangeShadowQuality))

		self.changeMusicButton.SAFE_SetEvent(self.__OnClickChangeMusicButton)

		self.cameraModeButtonList[0].SAFE_SetEvent(self.__OnClickCameraModeShortButton)
		self.cameraModeButtonList[1].SAFE_SetEvent(self.__OnClickCameraModeLongButton)

		if app.ENABLE_FOG_FIX:
			self.fogModeButtonList[0].SAFE_SetEvent(self.__OnClickFogModeOn)
			self.fogModeButtonList[1].SAFE_SetEvent(self.__OnClickFogModeOff)
		else:
			self.fogModeButtonList[0].SAFE_SetEvent(self.__OnClickFogModeLevel0Button)
			self.fogModeButtonList[1].SAFE_SetEvent(self.__OnClickFogModeLevel1Button)
			self.fogModeButtonList[2].SAFE_SetEvent(self.__OnClickFogModeLevel2Button)

		self.tilingModeButtonList[0].SAFE_SetEvent(self.__OnClickTilingModeCPUButton)
		self.tilingModeButtonList[1].SAFE_SetEvent(self.__OnClickTilingModeGPUButton)
		
		self.nightModeOptionButtonList[0].SAFE_SetEvent(self.__OnClickNightModeOptionEnableButton)
		self.nightModeOptionButtonList[1].SAFE_SetEvent(self.__OnClickNightModeOptionDisableButton)
		
		self.texturesOptionButtonList[0].SAFE_SetEvent(self.__OnClickChangeTextureClassic)
		self.texturesOptionButtonList[1].SAFE_SetEvent(self.__OnClickChangeTextureSnow)
		self.texturesOptionButtonList[2].SAFE_SetEvent(self.__OnClickChangeTextureDesert)
		
		self.snowModeButton[0].SAFE_SetEvent(self.__OnClickEnableSnowFall)
		self.snowModeButton[1].SAFE_SetEvent(self.__OnClickDisableSnowFall)
		
		self.fonttypeButtonList[0].SAFE_SetEvent(self.__OnClickArialButton)
		self.fonttypeButtonList[1].SAFE_SetEvent(self.__OnClickTahomaButton)
		
		self.viewCostumeButtonList[0].SAFE_SetEvent(self.__OnClickViewCostumeOnButton)
		self.viewCostumeButtonList[1].SAFE_SetEvent(self.__OnClickViewCostumeOffButton)
		
		self.viewCostumeWButtonList[0].SAFE_SetEvent(self.__OnClickViewCostumeWOnButton)
		self.viewCostumeWButtonList[1].SAFE_SetEvent(self.__OnClickViewCostumeWOffButton)
		
		self.viewCostumeHButtonList[0].SAFE_SetEvent(self.__OnClickViewCostumeHOnButton)
		self.viewCostumeHButtonList[1].SAFE_SetEvent(self.__OnClickViewCostumeHOffButton)
		
		self.viewSashButtonList[0].SAFE_SetEvent(self.__OnClickViewSashOnButton)
		self.viewSashButtonList[1].SAFE_SetEvent(self.__OnClickViewSashOffButton)
		
		self.viewLevelButtonList[0].SAFE_SetEvent(self.__OnClickViewLevelOnButton)
		self.viewLevelButtonList[1].SAFE_SetEvent(self.__OnClickViewLevelOffButton)
		
		self.highlightButtonList[0].SAFE_SetEvent(self.__OnClickHighlightOnButton)
		self.highlightButtonList[1].SAFE_SetEvent(self.__OnClickHighlightOffButton)
				
		self.tilingApplyButton.SAFE_SetEvent(self.__OnClickTilingApplyButton)

		self.__SetCurTilingMode()

		if not app.ENABLE_FOG_FIX:
			self.__ClickRadioButton(self.fogModeButtonList, constInfo.GET_FOG_LEVEL_INDEX())
		self.__ClickRadioButton(self.cameraModeButtonList, constInfo.GET_CAMERA_MAX_DISTANCE_INDEX())
		self.__ClickRadioButton(self.snowModeButton, not systemSetting.IsEnableSnowFall())
		
		if os.path.exists("gameoption.cfg"):
			fd = open( "gameoption.cfg" , "r")
			fonttype = fd.readline()
			if fonttype == "0":
				self.__ClickRadioButton(self.fonttypeButtonList, 0)
			elif fonttype == "1":
				self.__ClickRadioButton(self.fonttypeButtonList, 1)
		else:
			self.__ClickRadioButton(self.fonttypeButtonList, 0)

		if musicInfo.fieldMusic==musicInfo.METIN2THEMA:
			self.selectMusicFile.SetText(uiSelectMusic.DEFAULT_THEMA)
		else:
			self.selectMusicFile.SetText(musicInfo.fieldMusic[:MUSIC_FILENAME_MAX_LEN])
			
		self.ctrlShopNamesRange.SetSliderPos(float(systemSetting.GetShopNamesRange()))
		self.ctrlShopNamesRange.SetEvent(ui.__mem_func__(self.OnChangeShopNamesRange))
		
		if app.ENABLE_GRAPHIC_MASK:
			for (tabKey, tabButton) in enumerate(self.graphicMaskButtonList):
				tabButton.SetToggleUpEvent(lambda arg=tabKey: self.__OnClickGraphicMaskButton(arg, False))
				tabButton.SetToggleDownEvent(lambda arg=tabKey: self.__OnClickGraphicMaskButton(arg, True))

			self.__RefreshGraphicMask()
			
		self.NextPage.SAFE_SetEvent(self.GrPageNext)
		self.PrevPage.SAFE_SetEvent(self.GrPagePrev)
		
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
			self.ctrlMusicVolumeName.Show()
			self.ctrlMusicVolume.Show()
			
			self.ctrlSoundVolumeName.Show()
			self.ctrlSoundVolume.Show()
			
			self.ctrlShadowQualityName.Show()
			self.ctrlShadowQuality.Show()
			
			self.ctrlShopNamesRangeName.Show()
			self.ctrlShopNamesRange.Show()
			
			self.changeMusicButtonName.Show()
			self.changeMusicButton.Show()
			
			self.cameraModeButtonName.Show()
			self.cameraModeButtonList[0].Show()
			self.cameraModeButtonList[1].Show()
			
			self.fogModeButtonName.Show()
			self.fogModeButtonList[0].Show()
			self.fogModeButtonList[1].Show()
			
			self.tilingModeButtonName.Show()
			self.tilingModeButtonList[0].Show()
			self.tilingModeButtonList[1].Show()
			self.tilingApplyButton.Show()
			
			self.nightModeOptionButtonName.Show()
			self.nightModeOptionButtonList[0].Show()
			self.nightModeOptionButtonList[1].Show()
			
			## ##
			self.texturesOptionButtonName.Hide()
			self.texturesOptionButtonList[0].Hide()
			self.texturesOptionButtonList[1].Hide()
			self.texturesOptionButtonList[2].Hide()
			
			self.snowModeName.Hide()
			self.snowModeButton[0].Hide()
			self.snowModeButton[1].Hide()
			
			self.fonttypeButtonName.Hide()
			self.fonttypeButtonList[0].Hide()
			self.fonttypeButtonList[1].Hide()
			
			self.viewCostumeButtonName.Hide()
			self.viewCostumeButtonList[0].Hide()
			self.viewCostumeButtonList[1].Hide()
			
			self.viewCostumeWButtonName.Hide()
			self.viewCostumeWButtonList[0].Hide()
			self.viewCostumeWButtonList[1].Hide()
			
			self.viewCostumeHButtonName.Hide()
			self.viewCostumeHButtonList[0].Hide()
			self.viewCostumeHButtonList[1].Hide()
			
			self.viewSashButtonName.Hide()
			self.viewSashButtonList[0].Hide()
			self.viewSashButtonList[1].Hide()
			
			self.graphicMaskButtonName.Hide()
			self.graphicMaskButtonList[0].Hide()
			self.graphicMaskButtonList[1].Hide()
			self.graphicMaskButtonList[2].Hide()
			self.graphicMaskButtonList[3].Hide()
			
			self.viewLevelButtonName.Hide()
			self.viewLevelButtonList[0].Hide()
			self.viewLevelButtonList[1].Hide()
			
			self.highlightButtonName.Hide()
			self.highlightButtonList[0].Hide()
			self.highlightButtonList[1].Hide()
		else:
			self.ctrlMusicVolumeName.Hide()
			self.ctrlMusicVolume.Hide()
			
			self.ctrlSoundVolumeName.Hide()
			self.ctrlSoundVolume.Hide()
			
			self.ctrlShadowQualityName.Hide()
			self.ctrlShadowQuality.Hide()
			
			self.ctrlShopNamesRangeName.Hide()
			self.ctrlShopNamesRange.Hide()
			
			self.changeMusicButtonName.Hide()
			self.changeMusicButton.Hide()
			
			self.cameraModeButtonName.Hide()
			self.cameraModeButtonList[0].Hide()
			self.cameraModeButtonList[1].Hide()
			
			self.fogModeButtonName.Hide()
			self.fogModeButtonList[0].Hide()
			self.fogModeButtonList[1].Hide()
			
			self.tilingModeButtonName.Hide()
			self.tilingModeButtonList[0].Hide()
			self.tilingModeButtonList[1].Hide()
			self.tilingApplyButton.Hide()
			
			self.nightModeOptionButtonName.Hide()
			self.nightModeOptionButtonList[0].Hide()
			self.nightModeOptionButtonList[1].Hide()
			
			## ##
			self.texturesOptionButtonName.Show()
			self.texturesOptionButtonList[0].Show()
			self.texturesOptionButtonList[1].Show()
			self.texturesOptionButtonList[2].Show()
			
			self.snowModeName.Show()
			self.snowModeButton[0].Show()
			self.snowModeButton[1].Show()
			
			self.fonttypeButtonName.Show()
			self.fonttypeButtonList[0].Show()
			self.fonttypeButtonList[1].Show()
			
			self.viewCostumeButtonName.Show()
			self.viewCostumeButtonList[0].Show()
			self.viewCostumeButtonList[1].Show()
			
			self.viewCostumeWButtonName.Show()
			self.viewCostumeWButtonList[0].Show()
			self.viewCostumeWButtonList[1].Show()
			
			self.viewCostumeHButtonName.Show()
			self.viewCostumeHButtonList[0].Show()
			self.viewCostumeHButtonList[1].Show()
			
			self.viewSashButtonName.Show()
			self.viewSashButtonList[0].Show()
			self.viewSashButtonList[1].Show()
			
			self.graphicMaskButtonName.Show()
			self.graphicMaskButtonList[0].Show()
			self.graphicMaskButtonList[1].Show()
			self.graphicMaskButtonList[2].Show()
			self.graphicMaskButtonList[3].Show()
			
			self.viewLevelButtonName.Show()
			self.viewLevelButtonList[0].Show()
			self.viewLevelButtonList[1].Show()
			
			self.highlightButtonName.Show()
			self.highlightButtonList[0].Show()
			self.highlightButtonList[1].Show()

	def __OnClickTilingModeCPUButton(self):
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_1)
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_2)
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_CPU_TILING_3)
		self.__SetTilingMode(0)

	def __OnClickTilingModeGPUButton(self):
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_1)
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_2)
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_GPU_TILING_3)
		self.__SetTilingMode(1)

	def __OnClickTilingApplyButton(self):
		self.__NotifyChatLine(localeInfo.SYSTEM_OPTION_TILING_EXIT)
		if 0==self.tilingMode:
			background.EnableSoftwareTiling(1)
		else:
			background.EnableSoftwareTiling(0)

		net.ExitGame()

	def __OnClickChangeMusicButton(self):
		if not self.musicListDlg:
			
			self.musicListDlg=uiSelectMusic.FileListDialog()
			self.musicListDlg.SAFE_SetSelectEvent(self.__OnChangeMusic)

		self.musicListDlg.Open()

		
	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()


	def __SetTilingMode(self, index):
		self.__ClickRadioButton(self.tilingModeButtonList, index)
		self.tilingMode=index

	def __SetCameraMode(self, index):
		constInfo.SET_CAMERA_MAX_DISTANCE_INDEX(index)
		self.__ClickRadioButton(self.cameraModeButtonList, index)

	def __SetFogLevel(self, index):
		constInfo.SET_FOG_LEVEL_INDEX(index)
		self.__ClickRadioButton(self.fogModeButtonList, index)

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
		
	if app.ENABLE_GRAPHIC_MASK:
		def __RefreshGraphicMask(self):
			for (tabKey, tabButton) in enumerate(self.graphicMaskButtonList):
				if systemSetting.GetGraphicMaskPart(tabKey+1):
					tabButton.Down()
				else:
					tabButton.SetUp()

			background.LoadGraphicMaskSettings()
	
		def __OnClickGraphicMaskButton(self, bPart, bFlag):
			systemSetting.SetGraphicMaskPart(bPart+1, bFlag)
			self.__RefreshGraphicMask()

	def __OnChangeMusic(self, fileName):
		self.selectMusicFile.SetText(fileName[:MUSIC_FILENAME_MAX_LEN])

		if musicInfo.fieldMusic != "":
			snd.FadeOutMusic("BGM/"+ musicInfo.fieldMusic)

		if fileName==uiSelectMusic.DEFAULT_THEMA:
			musicInfo.fieldMusic=musicInfo.METIN2THEMA
		else:
			musicInfo.fieldMusic=fileName

		musicInfo.SaveLastPlayFieldMusic()
		
		if musicInfo.fieldMusic != "":
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	def OnChangeMusicVolume(self):
		pos = self.ctrlMusicVolume.GetSliderPos()
		snd.SetMusicVolume(pos * net.GetFieldMusicVolume())
		systemSetting.SetMusicVolume(pos)

	def OnChangeSoundVolume(self):
		pos = self.ctrlSoundVolume.GetSliderPos()
		snd.SetSoundVolumef(pos)
		systemSetting.SetSoundVolumef(pos)

	def OnChangeShadowQuality(self):
		pos = self.ctrlShadowQuality.GetSliderPos()
		systemSetting.SetShadowLevel(int(pos / 0.2))
		
	if app.ENABLE_FOG_FIX:
		def __OnClickFogModeOn(self):
			systemSetting.SetFogMode(True)
			background.SetEnvironmentFog(True)
			self.RefreshFogMode()
			systemSetting.SaveConfig()
			
		def __OnClickFogModeOff(self):
			systemSetting.SetFogMode(False)
			background.SetEnvironmentFog(False)
			self.RefreshFogMode()
			systemSetting.SaveConfig()
			
		def RefreshFogMode(self):
			if systemSetting.IsFogMode():
				self.fogModeButtonList[0].Down()
				self.fogModeButtonList[1].SetUp()
			else:
				self.fogModeButtonList[0].SetUp()
				self.fogModeButtonList[1].Down()
				
	def __OnClickNightModeOptionEnableButton(self):
		systemSetting.SetNightMode(True)
		background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
		background.SetEnvironmentData(1)
		self.RefreshNightOption()
		
	def __OnClickNightModeOptionDisableButton(self):
		systemSetting.SetNightMode(False)
		background.SetEnvironmentData(0)
		self.RefreshNightOption()
		
	def RefreshNightOption(self):
		if systemSetting.GetNightMode():
			self.nightModeOptionButtonList[0].Down()
			self.nightModeOptionButtonList[1].SetUp()
		else:
			self.nightModeOptionButtonList[0].SetUp()
			self.nightModeOptionButtonList[1].Down()
			
	if app.ENABLE_SHOPNAMES_RANGE:
		def OnChangeShopNamesRange(self):
			pos = self.ctrlShopNamesRange.GetSliderPos()
			systemSetting.SetShopNamesRange(pos)
			if systemSetting.IsShowSalesText():
				uiPrivateShopBuilder.UpdateADBoard()
		
	def __OnClickChangeTextureClassic(self):
		systemSetting.SetSnowTexturesMode(False)
		systemSetting.SetDesertTexturesMode(False)
		self.RefreshTextures()
		if background.GetCurrentMapName():
			classic_maps = [
				"metin2_map_a1",
				"metin2_map_b1",
				"metin2_map_c1"
			]
			classic_map_textures = {
				"metin2_map_a1" : "textureset\metin2_a1.txt",
				"metin2_map_b1" : "textureset\metin2_b1.txt",
				"metin2_map_c1" : "textureset\metin2_c1.txt", }
			if str(background.GetCurrentMapName()) in classic_maps:
				background.TextureChange(classic_map_textures[str(background.GetCurrentMapName())])

	def __OnClickChangeTextureSnow(self):
		systemSetting.SetSnowTexturesMode(True)
		systemSetting.SetDesertTexturesMode(False)
		self.RefreshTextures()
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

	def __OnClickChangeTextureDesert(self):
		systemSetting.SetSnowTexturesMode(False)
		systemSetting.SetDesertTexturesMode(True)
		self.RefreshTextures()
		if background.GetCurrentMapName():
			desert_maps = [
				"metin2_map_a1",
				"metin2_map_b1",
				"metin2_map_c1"
			]
			desert_maps_textures = {
				"metin2_map_a1" : "textureset\metin2_a1_desert.txt",
				"metin2_map_b1" : "textureset\metin2_b1_desert.txt",
				"metin2_map_c1" : "textureset\metin2_c1_desert.txt", }
			if str(background.GetCurrentMapName()) in desert_maps:
				background.TextureChange(desert_maps_textures[str(background.GetCurrentMapName())])		
	
	def RefreshTextures(self):
		if systemSetting.IsSnowTexturesMode():
			self.texturesOptionButtonList[0].SetUp()
			self.texturesOptionButtonList[1].Down()
			self.texturesOptionButtonList[2].SetUp()
		elif systemSetting.IsDesertTexturesMode():
			self.texturesOptionButtonList[0].SetUp()
			self.texturesOptionButtonList[1].SetUp()
			self.texturesOptionButtonList[2].Down()
		else:
			self.texturesOptionButtonList[0].Down()
			self.texturesOptionButtonList[1].SetUp()
			self.texturesOptionButtonList[2].SetUp()
			
	def __OnClickEnableSnowFall(self):
		systemSetting.SetEnableSnowFall(True)
		self.__ClickRadioButton(self.snowModeButton, 0)

	def __OnClickDisableSnowFall(self):
		systemSetting.SetEnableSnowFall(False)
		self.__ClickRadioButton(self.snowModeButton, 1)
		
	def __OnClickArialButton(self):
		if os.path.exists("gameoption.cfg"):
			os.remove("gameoption.cfg")
		f = open( "gameoption.cfg", "w")
		f.write("0")
		self.popupDialog.Open(localeInfo.LANG_RESTART_FOR_CHANGE)
		self.__ClickRadioButton(self.fonttypeButtonList, 0)
		
	def __OnClickTahomaButton(self):
		if os.path.exists("gameoption.cfg"):
			os.remove("gameoption.cfg")
		f = open( "gameoption.cfg", "w")
		f.write("1")
		self.popupDialog.Open(localeInfo.LANG_RESTART_FOR_CHANGE)
		self.__ClickRadioButton(self.fonttypeButtonList, 1)
		
	def __OnClickViewCostumeOnButton(self):
		net.SendChatPacket("/costume_config 0")
		constInfo.CONFIG_HIDE_COSTUME = 0
		self.RefreshCostume()
		
	def __OnClickViewCostumeOffButton(self):
		net.SendChatPacket("/costume_config 1")
		constInfo.CONFIG_HIDE_COSTUME = 1
		self.RefreshCostume()
	
	def __OnClickViewCostumeWOnButton(self):
		net.SendChatPacket("/costume_w_config 0")
		constInfo.CONFIG_HIDE_COSTUME_W = 0
		self.RefreshCostumeW()
		
	def __OnClickViewCostumeWOffButton(self):
		net.SendChatPacket("/costume_w_config 1")
		constInfo.CONFIG_HIDE_COSTUME_W = 1
		self.RefreshCostumeW()
		
	def __OnClickViewCostumeHOnButton(self):
		net.SendChatPacket("/costume_h_config 0")
		constInfo.CONFIG_HIDE_COSTUME_H = 0
		self.RefreshCostumeH()
		
	def __OnClickViewCostumeHOffButton(self):
		net.SendChatPacket("/costume_h_config 1")
		constInfo.CONFIG_HIDE_COSTUME_H = 1
		self.RefreshCostumeH()
		
	def __OnClickViewSashOnButton(self):
		net.SendChatPacket("/sash_config 0")
		constInfo.CONFIG_HIDE_SASH = 0
		self.RefreshSash()
		
	def __OnClickViewSashOffButton(self):
		net.SendChatPacket("/sash_config 1")
		constInfo.CONFIG_HIDE_SASH = 1
		self.RefreshSash()
		
	def __OnClickViewLevelOnButton(self):
		net.SendChatPacket("/level_view_config 0")
		constInfo.CONFIG_HIDE_LEVEL = 0
		self.RefreshLevel()
		
	def __OnClickViewLevelOffButton(self):
		net.SendChatPacket("/level_view_config 1")
		constInfo.CONFIG_HIDE_LEVEL = 1
		self.RefreshLevel()
		
	def __OnClickHighlightOnButton(self):
		systemSetting.SetItemHighlight(True)
		self.RefreshHighlight()
		
	def __OnClickHighlightOffButton(self):
		systemSetting.SetItemHighlight(False)
		self.RefreshHighlight()
		
	def RefreshSash(self):
		if constInfo.CONFIG_HIDE_SASH == 0:
			self.viewSashButtonList[0].Down()
			self.viewSashButtonList[1].SetUp()
		else:
			self.viewSashButtonList[0].SetUp()
			self.viewSashButtonList[1].Down()
		
	def RefreshCostume(self):
		if constInfo.CONFIG_HIDE_COSTUME == 0:
			self.viewCostumeButtonList[0].Down()
			self.viewCostumeButtonList[1].SetUp()
		else:
			self.viewCostumeButtonList[0].SetUp()
			self.viewCostumeButtonList[1].Down()
			
	def RefreshCostumeW(self):
		if constInfo.CONFIG_HIDE_COSTUME_W == 0:
			self.viewCostumeWButtonList[0].Down()
			self.viewCostumeWButtonList[1].SetUp()
		else:
			self.viewCostumeWButtonList[0].SetUp()
			self.viewCostumeWButtonList[1].Down()
		
	def RefreshCostumeH(self):
		if constInfo.CONFIG_HIDE_COSTUME_H == 0:
			self.viewCostumeHButtonList[0].Down()
			self.viewCostumeHButtonList[1].SetUp()
		else:
			self.viewCostumeHButtonList[0].SetUp()
			self.viewCostumeHButtonList[1].Down()
		
	def RefreshLevel(self):
		if constInfo.CONFIG_HIDE_LEVEL == 0:
			self.viewLevelButtonList[0].Down()
			self.viewLevelButtonList[1].SetUp()
		else:
			self.viewLevelButtonList[0].SetUp()
			self.viewLevelButtonList[1].Down()
			
	def RefreshHighlight(self):
		if systemSetting.IsItemHighlight() == True:
			self.highlightButtonList[0].Down()
			self.highlightButtonList[1].SetUp()
		else:
			self.highlightButtonList[0].SetUp()
			self.highlightButtonList[1].Down()

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
	
	def Show(self):
		ui.ScriptWindow.Show(self)
		self.option_page = 1
		self.ChangeGrPage()

	def Close(self):
		self.__SetCurTilingMode()
		self.Hide()

	def __SetCurTilingMode(self):
		if background.IsSoftwareTiling():
			self.__SetTilingMode(0)
		else:
			self.__SetTilingMode(1)	

	def __NotifyChatLine(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, text)

