#include "PythonrootlibManager.h"
#include <python/Python.h>

#ifdef __USE_CYTHON__
struct rootlib_SMethodDef
{
	char* func_name;
	void (*func)();
};

PyMODINIT_FUNC initcolorInfo();
PyMODINIT_FUNC initconsoleModule();
PyMODINIT_FUNC initconstInfo();
PyMODINIT_FUNC initcpyshield();
PyMODINIT_FUNC initdebugInfo();
PyMODINIT_FUNC initdragon_soul_refine_settings();
PyMODINIT_FUNC initeigenmods();
PyMODINIT_FUNC initemotion();
PyMODINIT_FUNC initexception();
PyMODINIT_FUNC initgame();
PyMODINIT_FUNC initgrid();
PyMODINIT_FUNC initinterfaceModule();
PyMODINIT_FUNC initintroCreate();
PyMODINIT_FUNC initintroEmpire();
PyMODINIT_FUNC initintroLoading();
PyMODINIT_FUNC initintroLogin();
PyMODINIT_FUNC initintrologin_ex();
PyMODINIT_FUNC initintrologin_old();
PyMODINIT_FUNC initintroLogo();
PyMODINIT_FUNC initintroSelect();
PyMODINIT_FUNC initlocaleInfo();
PyMODINIT_FUNC initmain();
PyMODINIT_FUNC initmouseModule();
PyMODINIT_FUNC initmusicInfo();
PyMODINIT_FUNC initnetworkModule();
PyMODINIT_FUNC initnew_introcreate();
PyMODINIT_FUNC initnew_introselect();
PyMODINIT_FUNC initplayerSettingModule();
PyMODINIT_FUNC initPrototype();
PyMODINIT_FUNC initrare_settings();
PyMODINIT_FUNC initselect_assassinskills();
PyMODINIT_FUNC initselect_shamanskills();
PyMODINIT_FUNC initselect_suraskills();
PyMODINIT_FUNC initselect_warriorskills();
PyMODINIT_FUNC initselect_wolfmanskills();
PyMODINIT_FUNC initserverCommandParser();
PyMODINIT_FUNC initserverInfo();
PyMODINIT_FUNC initstringCommander();
PyMODINIT_FUNC initsvsideoi();
PyMODINIT_FUNC initswitchbot();
PyMODINIT_FUNC inittest_affect();
PyMODINIT_FUNC inittranslate();
PyMODINIT_FUNC initui();
PyMODINIT_FUNC initui12zi();
PyMODINIT_FUNC inituiAffectShower();
PyMODINIT_FUNC inituiAttachMetin();
PyMODINIT_FUNC inituiaura();
PyMODINIT_FUNC inituiautohunt();
PyMODINIT_FUNC inituiaverage_price();
PyMODINIT_FUNC inituibiowindow();
PyMODINIT_FUNC inituibiyolog();
PyMODINIT_FUNC inituibkoku();
PyMODINIT_FUNC inituibosstracking();
PyMODINIT_FUNC inituicards();
PyMODINIT_FUNC inituichangelook();
PyMODINIT_FUNC inituiCharacter();
PyMODINIT_FUNC inituicharacterdetails();
PyMODINIT_FUNC inituiChat();
PyMODINIT_FUNC inituichequeticket();
PyMODINIT_FUNC inituichestdrop();
PyMODINIT_FUNC inituicombatzone();
PyMODINIT_FUNC inituiCommon();
PyMODINIT_FUNC inituiCube();
PyMODINIT_FUNC inituicube_ex();
PyMODINIT_FUNC inituiDragonSoul();
PyMODINIT_FUNC inituidungeoninfo();
PyMODINIT_FUNC inituiEquipmentDialog();
PyMODINIT_FUNC inituievent();
PyMODINIT_FUNC inituiEx();
PyMODINIT_FUNC inituiExchange();
PyMODINIT_FUNC inituiGameButton();
PyMODINIT_FUNC inituiGameOption();
PyMODINIT_FUNC inituigameoption_old();
PyMODINIT_FUNC inituigemshop();
PyMODINIT_FUNC inituigift();
PyMODINIT_FUNC inituiGuild();
PyMODINIT_FUNC inituiHelp();
PyMODINIT_FUNC inituiInventory();
PyMODINIT_FUNC inituiitemcombination();
PyMODINIT_FUNC inituiitempreview();
PyMODINIT_FUNC inituikeyboard();
PyMODINIT_FUNC inituikeyboardsettings();
PyMODINIT_FUNC inituiluckybox();
PyMODINIT_FUNC inituimailbox();
PyMODINIT_FUNC inituiMapNameShower();
PyMODINIT_FUNC inituimeleylairranking();
PyMODINIT_FUNC inituiMessenger();
PyMODINIT_FUNC inituiminigame();
PyMODINIT_FUNC inituiminigameattendance();
PyMODINIT_FUNC inituiminigamecatchking();
PyMODINIT_FUNC inituiminigamefishevent();
PyMODINIT_FUNC inituiMiniMap();
PyMODINIT_FUNC inituinewplayergauge();
PyMODINIT_FUNC inituinewshop();
PyMODINIT_FUNC inituinpcekran();
PyMODINIT_FUNC inituiOption();
PyMODINIT_FUNC inituiParty();
PyMODINIT_FUNC inituipetfeed();
PyMODINIT_FUNC inituipetincubatrice();
PyMODINIT_FUNC inituipetsystem();
PyMODINIT_FUNC inituiPhaseCurtain();
PyMODINIT_FUNC inituiPickMoney();
PyMODINIT_FUNC inituiPlayerGauge();
PyMODINIT_FUNC inituiPointReset();
PyMODINIT_FUNC inituiPrivateShopBuilder();
PyMODINIT_FUNC inituiprivateshopsearch();
PyMODINIT_FUNC inituiQuest();
PyMODINIT_FUNC inituirarityrefine();
PyMODINIT_FUNC inituiredgemshop();
PyMODINIT_FUNC inituiRefine();
PyMODINIT_FUNC inituiRestart();
PyMODINIT_FUNC inituiruhtasi();
PyMODINIT_FUNC inituiSafebox();
PyMODINIT_FUNC inituisash();
PyMODINIT_FUNC inituiScriptLocale();
PyMODINIT_FUNC inituiselectitem();
PyMODINIT_FUNC inituiselectitemex();
PyMODINIT_FUNC inituiSelectMusic();
PyMODINIT_FUNC inituiShop();
PyMODINIT_FUNC inituisupportshaman();
PyMODINIT_FUNC inituiSystem();
PyMODINIT_FUNC inituiSystemOption();
PyMODINIT_FUNC inituiTarget();
PyMODINIT_FUNC inituiTaskBar();
PyMODINIT_FUNC inituiTip();
PyMODINIT_FUNC inituiToolTip();
PyMODINIT_FUNC inituiUploadMark();
PyMODINIT_FUNC inituiutils();
PyMODINIT_FUNC inituiWeb();
PyMODINIT_FUNC inituiWhisper();
PyMODINIT_FUNC inituiyoutube();
PyMODINIT_FUNC inituiyoutubefavlist();
PyMODINIT_FUNC inituiyoutuber();
PyMODINIT_FUNC initui_wrapper();

rootlib_SMethodDef rootlib_init_methods[] =
{
	{ "colorInfo", initcolorInfo },
	{ "consoleModule", initconsoleModule },
	{ "constInfo", initconstInfo },
	{ "cpyshield", initcpyshield },
	{ "debugInfo", initdebugInfo },
	{ "dragon_soul_refine_settings", initdragon_soul_refine_settings },
	{ "eigenmods", initeigenmods },
	{ "emotion", initemotion },
	{ "exception", initexception },
	{ "game", initgame },
	{ "grid", initgrid },
	{ "interfaceModule", initinterfaceModule },
	{ "introCreate", initintroCreate },
	{ "introEmpire", initintroEmpire },
	{ "introLoading", initintroLoading },
	{ "introLogin", initintroLogin },
	{ "intrologin_ex", initintrologin_ex },
	{ "intrologin_old", initintrologin_old },
	{ "introLogo", initintroLogo },
	{ "introSelect", initintroSelect },
	{ "localeInfo", initlocaleInfo },
	{ "main", initmain },
	{ "mouseModule", initmouseModule },
	{ "musicInfo", initmusicInfo },
	{ "networkModule", initnetworkModule },
	{ "new_introcreate", initnew_introcreate },
	{ "new_introselect", initnew_introselect },
	{ "playerSettingModule", initplayerSettingModule },
	{ "Prototype", initPrototype },
	{ "rare_settings", initrare_settings },
	{ "select_assassinskills", initselect_assassinskills },
	{ "select_shamanskills", initselect_shamanskills },
	{ "select_suraskills", initselect_suraskills },
	{ "select_warriorskills", initselect_warriorskills },
	{ "select_wolfmanskills", initselect_wolfmanskills },
	{ "serverCommandParser", initserverCommandParser },
	{ "serverInfo", initserverInfo },
	{ "stringCommander", initstringCommander },
	{ "svsideoi", initsvsideoi },
	{ "switchbot", initswitchbot },
	{ "test_affect", inittest_affect },
	{ "translate", inittranslate },
	{ "ui", initui },
	{ "ui12zi", initui12zi },
	{ "uiAffectShower", inituiAffectShower },
	{ "uiAttachMetin", inituiAttachMetin },
	{ "uiaura", inituiaura },
	{ "uiautohunt", inituiautohunt },
	{ "uiaverage_price", inituiaverage_price },
	{ "uibiowindow", inituibiowindow },
	{ "uibiyolog", inituibiyolog },
	{ "uibkoku", inituibkoku },
	{ "uibosstracking", inituibosstracking },
	{ "uicards", inituicards },
	{ "uichangelook", inituichangelook },
	{ "uiCharacter", inituiCharacter },
	{ "uicharacterdetails", inituicharacterdetails },
	{ "uiChat", inituiChat },
	{ "uichequeticket", inituichequeticket },
	{ "uichestdrop", inituichestdrop },
	{ "uicombatzone", inituicombatzone },
	{ "uiCommon", inituiCommon },
	{ "uiCube", inituiCube },
	{ "uicube_ex", inituicube_ex },
	{ "uiDragonSoul", inituiDragonSoul },
	{ "uidungeoninfo", inituidungeoninfo },
	{ "uiEquipmentDialog", inituiEquipmentDialog },
	{ "uievent", inituievent },
	{ "uiEx", inituiEx },
	{ "uiExchange", inituiExchange },
	{ "uiGameButton", inituiGameButton },
	{ "uiGameOption", inituiGameOption },
	{ "uigameoption_old", inituigameoption_old },
	{ "uigemshop", inituigemshop },
	{ "uigift", inituigift },
	{ "uiGuild", inituiGuild },
	{ "uiHelp", inituiHelp },
	{ "uiInventory", inituiInventory },
	{ "uiitemcombination", inituiitemcombination },
	{ "uiitempreview", inituiitempreview },
	{ "uikeyboard", inituikeyboard },
	{ "uikeyboardsettings", inituikeyboardsettings },
	{ "uiluckybox", inituiluckybox },
	{ "uimailbox", inituimailbox },
	{ "uiMapNameShower", inituiMapNameShower },
	{ "uimeleylairranking", inituimeleylairranking },
	{ "uiMessenger", inituiMessenger },
	{ "uiminigame", inituiminigame },
	{ "uiminigameattendance", inituiminigameattendance },
	{ "uiminigamecatchking", inituiminigamecatchking },
	{ "uiminigamefishevent", inituiminigamefishevent },
	{ "uiMiniMap", inituiMiniMap },
	{ "uinewplayergauge", inituinewplayergauge },
	{ "uinewshop", inituinewshop },
	{ "uinpcekran", inituinpcekran },
	{ "uiOption", inituiOption },
	{ "uiParty", inituiParty },
	{ "uipetfeed", inituipetfeed },
	{ "uipetincubatrice", inituipetincubatrice },
	{ "uipetsystem", inituipetsystem },
	{ "uiPhaseCurtain", inituiPhaseCurtain },
	{ "uiPickMoney", inituiPickMoney },
	{ "uiPlayerGauge", inituiPlayerGauge },
	{ "uiPointReset", inituiPointReset },
	{ "uiPrivateShopBuilder", inituiPrivateShopBuilder },
	{ "uiprivateshopsearch", inituiprivateshopsearch },
	{ "uiQuest", inituiQuest },
	{ "uirarityrefine", inituirarityrefine },
	{ "uiredgemshop", inituiredgemshop },
	{ "uiRefine", inituiRefine },
	{ "uiRestart", inituiRestart },
	{ "uiruhtasi", inituiruhtasi },
	{ "uiSafebox", inituiSafebox },
	{ "uisash", inituisash },
	{ "uiScriptLocale", inituiScriptLocale },
	{ "uiselectitem", inituiselectitem },
	{ "uiselectitemex", inituiselectitemex },
	{ "uiSelectMusic", inituiSelectMusic },
	{ "uiShop", inituiShop },
	{ "uisupportshaman", inituisupportshaman },
	{ "uiSystem", inituiSystem },
	{ "uiSystemOption", inituiSystemOption },
	{ "uiTarget", inituiTarget },
	{ "uiTaskBar", inituiTaskBar },
	{ "uiTip", inituiTip },
	{ "uiToolTip", inituiToolTip },
	{ "uiUploadMark", inituiUploadMark },
	{ "uiutils", inituiutils },
	{ "uiWeb", inituiWeb },
	{ "uiWhisper", inituiWhisper },
	{ "uiyoutube", inituiyoutube },
	{ "uiyoutubefavlist", inituiyoutubefavlist },
	{ "uiyoutuber", inituiyoutuber },
	{ "ui_wrapper", initui_wrapper },
	{ NULL, NULL },
};

static PyObject* rootlib_isExist(PyObject *self, PyObject *args)
{
	char* func_name;

	if(!PyArg_ParseTuple(args, "s", &func_name))
		return NULL;

	for (int i = 0; NULL != rootlib_init_methods[i].func_name;i++)
	{
		if (0 == _stricmp(rootlib_init_methods[i].func_name, func_name))
		{
			return Py_BuildValue("i", 1);
		}
	}
	return Py_BuildValue("i", 0);
}

static PyObject* rootlib_moduleImport(PyObject *self, PyObject *args)
{
	char* func_name;

	if(!PyArg_ParseTuple(args, "s", &func_name))
		return NULL;

	for (int i = 0; NULL != rootlib_init_methods[i].func_name;i++)
	{
		if (0 == _stricmp(rootlib_init_methods[i].func_name, func_name))
		{
			rootlib_init_methods[i].func();
			if (PyErr_Occurred())
				return NULL;
			PyObject* m = PyDict_GetItemString(PyImport_GetModuleDict(), rootlib_init_methods[i].func_name);
			if (m == NULL) {
				PyErr_SetString(PyExc_SystemError,
					"dynamic module not initialized properly");
				return NULL;
			}
			Py_INCREF(m);
			return Py_BuildValue("S", m);
		}
	}
	return NULL;
}

static PyObject* rootlib_getList(PyObject *self, PyObject *args)
{
	int iTupleSize = 0;
	while (NULL != rootlib_init_methods[iTupleSize].func_name) {iTupleSize++;}

	PyObject* retTuple = PyTuple_New(iTupleSize);
	for (int i = 0; NULL != rootlib_init_methods[i].func_name; i++)
	{
		PyObject* retSubString = PyString_FromString(rootlib_init_methods[i].func_name);
		PyTuple_SetItem(retTuple, i, retSubString);
	}
	return retTuple;
}

void initrootlibManager()
{
	static struct PyMethodDef methods[] =
	{
		{"isExist", rootlib_isExist, METH_VARARGS},
		{"moduleImport", rootlib_moduleImport, METH_VARARGS},
		{"getList", rootlib_getList, METH_VARARGS},
		{NULL, NULL},
	};

	PyObject* m;
	m = Py_InitModule("rootlib", methods);
}
#endif
