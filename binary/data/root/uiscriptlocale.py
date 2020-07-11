import app

OPTION_SHADOW = "SHADOW"

CODEPAGE = str(app.GetDefaultCodePage())

#CUBE_TITLE = "Cube Window"

def LoadLocaleFile(srcFileName, localeDict):
	localeDict["CUBE_INFO_TITLE"] = "Recipe"
	localeDict["CUBE_REQUIRE_MATERIAL"] = "Requirements"
	localeDict["CUBE_REQUIRE_MATERIAL_OR"] = "or"
	
	try:
		lines = pack_open(srcFileName,"r").readlines()
	except IOError:
		import dbg
		dbg.LogBox("LoadUIScriptLocaleError(%(srcFileName)s)" % locals())
		app.Abort()

	for line in lines:
		tokens = line[:-1].split("\t")
		
		if len(tokens) >= 2:
			localeDict[tokens[0]] = tokens[1]			
			
		else:
			print len(tokens), lines.index(line), line



name = app.GetLocalePath()

LOCALE_UISCRIPT_PATH = "d:/ymir work/ui/"
LOGIN_PATH = "d:/ymir work/ui/login/"
EMPIRE_PATH = "d:/ymir work/ui/empire/"
GUILD_PATH = "d:/ymir work/ui/guild/"
SELECT_PATH = "d:/ymir work/ui/select/"
WINDOWS_PATH = "d:/ymir work/ui/windows/"
MAPNAME_PATH = "d:/ymir work/ui/mapname/"

JOBDESC_WARRIOR_PATH = "%s/jobdesc_warrior.txt" % (name)
JOBDESC_ASSASSIN_PATH = "%s/jobdesc_assassin.txt" % (name)
JOBDESC_SURA_PATH = "%s/jobdesc_sura.txt" % (name)
JOBDESC_SHAMAN_PATH = "%s/jobdesc_shaman.txt" % (name)
JOBDESC_WOLFMAN_PATH = "%s/jobdesc_wolfman.txt" % (name)

EMPIREDESC_A = "%s/empiredesc_a.txt" % (name)
EMPIREDESC_B = "%s/empiredesc_b.txt" % (name)
EMPIREDESC_C = "%s/empiredesc_c.txt" % (name)
CARDS_DESC = "%s/mini_game_okey_desc.txt" % (name)
ATTENDANCE_DESC = "%s/attendance_desc.txt" % (name)
CATCH_KING_DESC = "%s/catchking_event_desc.txt" % (name)
CATCH_KING_SIMPLE = "%s/catchking_event_simple_desc.txt" % (name)
tipList = "%s/loading_tips.txt" % (name)

LOCALE_INTERFACE_FILE_NAME = "%s/locale_interface.txt" % (name)

LoadLocaleFile(LOCALE_INTERFACE_FILE_NAME, locals())

