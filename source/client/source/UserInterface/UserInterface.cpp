#include "StdAfx.h"
#include "PythonApplication.h"
#include "resource.h"
#include "../eterbase/error.h"
#include <windowsx.h>
#include <windows.h>
#include <assert.h>
#include <tlhelp32.h>
#include <imagehlp.h>
#include <psapi.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commdlg.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <Shlwapi.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "../eterPack/EterPackManager.h"
#include "../eterLib/Util.h"
#include "../CWebBrowser/CWebBrowser.h"
#include "../eterBase/CPostIt.h"


#ifdef ENABLE_HWID_BAN_SYSTEM
#include "HWIDManager.h"
#endif

#include "PythonDynamicModuleNames.h"

#ifdef ENABLE_VFS
	#include <XORstr.h>
	#include <array>
	#include <memory>
#endif

extern "C" {  
extern int _fltused;  
volatile int _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;  
};  

bool __IS_TEST_SERVER_MODE__=false;

#ifdef __USE_CYTHON__
// don't include these two files .h .cpp if you're implementing cython via .pyd
#include "PythonrootlibManager.h"
#endif

extern bool SetDefaultCodePage(DWORD codePage);

static const char * sc_apszPythonLibraryFilenames[] =
{
	"UserDict.py",
	"__future__.py",
	"copy_reg.py",
	"linecache.py",
	"ntpath.py",
	"os.py",
	"site.py",
	"stat.py",
	"string.py",
	"traceback.py",
	"types.py",
	"\n",
};

char gs_szErrorString[512] = "";

void ApplicationSetErrorString(const char* szErrorString)
{
	strcpy(gs_szErrorString, szErrorString);
}

bool CheckPythonLibraryFilenames()
{
	for (int i = 0; *sc_apszPythonLibraryFilenames[i] != '\n'; ++i)
	{
		std::string stFilename = "lib\\";
		stFilename += sc_apszPythonLibraryFilenames[i];

		if (_access(stFilename.c_str(), 0) != 0)
		{
			return false;
		}

		MoveFile(stFilename.c_str(), stFilename.c_str());
	}

	return true;
}

struct ApplicationStringTable 
{
	HINSTANCE m_hInstance;
	std::map<DWORD, std::string> m_kMap_dwID_stLocale;
} gs_kAppStrTable;

void ApplicationStringTable_Initialize(HINSTANCE hInstance)
{
	gs_kAppStrTable.m_hInstance=hInstance;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey)
{
	char szBuffer[512];
	char szIniFileName[256];
	char szLocale[256];

	::GetCurrentDirectory(sizeof(szIniFileName), szIniFileName);
	if(szIniFileName[lstrlen(szIniFileName)-1] != '\\')
		strcat(szIniFileName, "\\");
	strcat(szIniFileName, "metin2client.dat");

	strcpy(szLocale, LocaleService_GetLocalePath());
	if(strnicmp(szLocale, "locale/", strlen("locale/")) == 0)
		strcpy(szLocale, LocaleService_GetLocalePath() + strlen("locale/"));
	::GetPrivateProfileString(szLocale, szKey, nullptr, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	if(szBuffer[0] == '\0')
		::GetPrivateProfileString("en", szKey, nullptr, szBuffer, sizeof(szBuffer)-1, szIniFileName);
	if(szBuffer[0] == '\0')
		strcpy(szBuffer, szKey);

	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID)
{
	char szBuffer[512];

	LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer)-1);
	std::string& rstLocale=gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale=szBuffer;

	return rstLocale;
}

const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey)
{
	return ApplicationStringTable_GetString(dwID, szKey).c_str();
}

const char* ApplicationStringTable_GetStringz(DWORD dwID)
{
	return ApplicationStringTable_GetString(dwID).c_str();
}

////////////////////////////////////////////

int Setup(LPSTR lpCmdLine); // Internal function forward

bool PackInitialize(const char * c_pszFolder)
{
	if (_access(c_pszFolder, 0) != 0)
		return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";
	CTextFileLoader::SetCacheMode();

	CEterPackManager::Instance().SetCacheMode();

	CSoundData::SetPackMode();
	CEterPackManager::Instance().RegisterPack("pack/effect", "d:/ymir work/effect");
	CEterPackManager::Instance().RegisterPack("pack/environment", "d:/ymir work/environment");
	CEterPackManager::Instance().RegisterPack("pack/guild", "d:/ymir work/guild");
	CEterPackManager::Instance().RegisterPack("pack/icon", "icon");
	CEterPackManager::Instance().RegisterPack("pack/item", "d:/ymir work/item");
	CEterPackManager::Instance().RegisterPack("pack/locale", "locale/");
	CEterPackManager::Instance().RegisterPack("pack/main", "d:/ymir work");
	CEterPackManager::Instance().RegisterPack("pack/maps", "*");
	CEterPackManager::Instance().RegisterPack("pack/main_secondary", "*");
	CEterPackManager::Instance().RegisterPack("pack/monster", "d:/ymir work/monster");
	CEterPackManager::Instance().RegisterPack("pack/monster2", "d:/ymir work/monster2");
	CEterPackManager::Instance().RegisterPack("pack/npc", "d:/ymir work/npc");
	CEterPackManager::Instance().RegisterPack("pack/npc_mount", "d:/ymir work/npc_mount");
	CEterPackManager::Instance().RegisterPack("pack/npc_pet", "d:/ymir work/npc_pet");
	CEterPackManager::Instance().RegisterPack("pack/npc2", "d:/ymir work/npc2");
	CEterPackManager::Instance().RegisterPack("pack/pc", "d:/ymir work/pc");
	CEterPackManager::Instance().RegisterPack("pack/pc2", "d:/ymir work/pc2");
	CEterPackManager::Instance().RegisterPack("pack/pc3", "d:/ymir work/pc3");
	CEterPackManager::Instance().RegisterPack("pack/property", "property");
	CEterPackManager::Instance().RegisterPack("pack/sound", "sound");
	CEterPackManager::Instance().RegisterPack("pack/special", "d:/ymir work/special");
	CEterPackManager::Instance().RegisterPack("pack/terrain", "d:/ymir work/terrainmaps");
	CEterPackManager::Instance().RegisterPack("pack/textureset", "d:/ymir work");
	CEterPackManager::Instance().RegisterPack("pack/tree", "d:/ymir work/tree");
	CEterPackManager::Instance().RegisterPack("pack/ui", "d:/ymir work/ui");
	CEterPackManager::Instance().RegisterPack("pack/zone", "d:/ymir work/zone");
	CEterPackManager::Instance().RegisterPack("pack/update1", "d:/ymir work/");
	CEterPackManager::Instance().RegisterPack("pack/update2", "d:/ymir work/");
	CEterPackManager::Instance().RegisterPack("pack/update_model", "d:/ymir work/");
	CEterPackManager::Instance().RegisterPack("pack/update_model2", "d:/ymir work/");
	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());
	return true;
}

bool RunMainScript(CPythonLauncher& pyLauncher, const char* lpCmdLine)
{
	initPythonApi();
	initpack();
	initdbg();
	initime();
	initgrp();
	initgrpImage();
	initgrpText();
	initwndMgr();
	/////////////////////////////////////////////
	initapp();
	initsystem();
	initchr();
	initchrmgr();
	initPlayer();
	initItem();
	initNonPlayer();
	initTrade();
	initChat();
	initTextTail();
	initnet();
	initMiniMap();
	initProfiler();
	initEvent();
	initeffect();
	initfly();
	initsnd();
	initeventmgr();
	initshop();
	initskill();
#ifdef ENABLE_GROWTH_PET_SYSTEM
	initskillpet();
#endif
	initquest();
	initBackground();
	initMessenger();
#ifdef ENABLE_CHANGELOOK_SYSTEM
	initChangeLook();
#endif
#ifdef ENABLE_SHOP_SEARCH_SYSTEM
	initprivateShopSearch();
#endif
#ifdef ENABLE_EVENT_INFORMATION_SYSTEM
	initGameEvents();
#endif
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	initCombatZoneSystem();
#endif
#ifdef ENABLE_ACCE_SYSTEM
	initSash();
#endif
	initsafebox();
	initguild();
	initServerStateChecker();
	initRenderTarget();	
#ifdef ENABLE_WHISPER_RENEWAL
	initWhisper();
#endif
#ifdef ENABLE_AURA_SYSTEM
	initAura();
#endif
	initFeatures();
#ifdef __USE_CYTHON__
	// don't add this line if you're implementing cython via .pyd:
	initrootlibManager();
#endif

	NANOBEGIN

    PyObject * builtins = PyImport_ImportModule("__builtin__");
#ifdef _NDEBUG
	PyModule_AddIntConstant(builtins, "__DEBUG__", 0);
#else
	PyModule_AddIntConstant(builtins, "__DEBUG__", 1);
#endif
#ifdef __USE_CYTHON__
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 1);
#else
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 0);
#endif

	// RegisterCommandLine
	{
		std::string stRegisterCmdLine;

		const char * loginMark = "-cs";
		const char * loginMark_NonEncode = "-ncs";
		const char * seperator = " ";

		std::string stCmdLine;
		const int CmdSize = 3;
		vector<std::string> stVec;
		SplitLine(lpCmdLine,seperator,&stVec);
		if (CmdSize == stVec.size() && stVec[0]==loginMark)
		{
			char buf[MAX_PATH];	//TODO 아래 함수 string 형태로 수정
			base64_decode(stVec[2].c_str(),buf);
			stVec[2] = buf;
			string_join(seperator,stVec,&stCmdLine);
		}
		else if (CmdSize <= stVec.size() && stVec[0]==loginMark_NonEncode)
		{
			stVec[0] = loginMark;
			string_join(" ",stVec,&stCmdLine);
		}
		else
			stCmdLine = lpCmdLine;

		PyModule_AddStringConstant(builtins, "__COMMAND_LINE__", stCmdLine.c_str());
	}
	{
		vector<std::string> stVec;
		SplitLine(lpCmdLine," " ,&stVec);

		if (stVec.size() != 0 && "--pause-before-create-window" == stVec[0])
		{
			system("pause");
		}
		
		CHAR spitLine[] = { 'm', 'a', 'i', 'n', '.', 'p', 'y', 0x0 };

#ifdef __USE_CYTHON__
		if (!pyLauncher.RunLine("import rootlib\nrootlib.moduleImport('main')"))
#else
		if (!pyLauncher.RunFile(spitLine))
#endif
		{
			TraceError("RunMain Error");
			return false;
		}
	}

	NANOEND
	return true;
}

bool Main(HINSTANCE hInstance, LPSTR lpCmdLine)
{
#ifdef LOCALE_SERVICE_YMIR
	extern bool g_isScreenShotKey;
	g_isScreenShotKey = true;
#endif

	DWORD dwRandSeed=time(nullptr)+DWORD(GetCurrentProcess());
	srandom(dwRandSeed);
	srand(random());

	SetLogLevel(1);


	if (_access("perf_game_update.txt", 0)==0)
	{
		DeleteFile("perf_game_update.txt");
	}

	if (_access("newpatch.exe", 0)==0)
	{		
		system("patchupdater.exe");
		return false;
	}
#ifndef __VTUNE__
	ilInit();
#endif
	if (!Setup(lpCmdLine))
		return false;

#ifdef _DEBUG
	OpenConsoleWindow();
	OpenLogFile(true); // true == uses syserr.txt and log.txt
#else
	OpenLogFile(false); // false == uses syserr.txt only
#endif

	static CLZO				lzo;
	static CEterPackManager	EterPackManager;
#ifdef ENABLE_HWID_BAN_SYSTEM
	static HWIDMANAGER		hwidManager;
#endif

	if (!PackInitialize("pack"))
	{
		LogBox("Pack Initialization failed. Check log.txt file..");
		return false;
	}

	CPythonApplication * app = new CPythonApplication;

	app->Initialize(hInstance);

	bool ret=false;
	{
		CPythonLauncher pyLauncher;

		if (pyLauncher.Create())
		{
			ret=RunMainScript(pyLauncher, lpCmdLine);	//게임 실행중엔 함수가 끝나지 않는다.
		}

		//ProcessScanner_ReleaseQuitEvent();
		
		//게임 종료시.
		app->Clear();

		timeEndPeriod(1);
		pyLauncher.Clear();
	}

	app->Destroy();
	delete app;
	
#ifdef ENABLE_CRASH_SENDER
	#ifndef _DEBUG
	RemoveEterException();
	#endif
#endif
	
	return ret;
}

HANDLE CreateMetin2GameMutex()
{
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength				= sizeof(sa);
	sa.lpSecurityDescriptor	= nullptr;
	sa.bInheritHandle		= FALSE;

	return CreateMutex(&sa, FALSE, "Metin2GameMutex");
}

void DestroyMetin2GameMutex(HANDLE hMutex)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		hMutex = nullptr;
	}
}

void __ErrorPythonLibraryIsNotExist()
{
	LogBoxf("FATAL ERROR!! Python Library file not exist!");
}

bool __IsTimeStampOption(LPSTR lpCmdLine)
{
	const char* TIMESTAMP = "/timestamp";
	return (strncmp(lpCmdLine, TIMESTAMP, strlen(TIMESTAMP))==0);
}

void __PrintTimeStamp()
{
#ifdef	_DEBUG
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST DEBUG VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	
#else
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST VERSION %s  ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);			
#endif			
}

bool __IsLocaleOption(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--locale") == 0);
}

bool __IsLocaleVersion(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--perforce-revision") == 0);
}

#ifdef URIEL_AC_VERIFY
extern "C" __declspec(dllexport) void urielac();
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef URIEL_AC_VERIFY
	urielac();
#endif

#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 110247 ); 
#endif

	SetEterExceptionHandler();

	ApplicationStringTable_Initialize(hInstance);

	//Protection();
	LoadConfig("loca.cfg");
	SetDefaultCodePage(GetCodePage());	

	bool bQuit = false;
	int nArgc = 0;
	PCHAR* szArgv = CommandLineToArgv( lpCmdLine, &nArgc );

	WebBrowser_Startup(hInstance);

	if (!CheckPythonLibraryFilenames())
	{
		__ErrorPythonLibraryIsNotExist();
		goto Clean;
	}

	Main(hInstance, lpCmdLine);

	WebBrowser_Cleanup();

	::CoUninitialize();

	if(gs_szErrorString[0])
		MessageBox(nullptr, gs_szErrorString, ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);

Clean:
	SAFE_FREE_GLOBAL(szArgv);
	
#ifndef _DEBUG
	RemoveEterException();
#endif

	return 0;
}

#if GrannyProductMinorVersion==4 || GrannyProductMinorVersion==7
static void GrannyError(granny_log_message_type Type,
						granny_log_message_origin Origin,
						char const *Error,
						void *UserData)
{
	TraceError("GRANNY: %s", Error);
}
#elif GrannyProductMinorVersion==9
static void GrannyError(granny_log_message_type Type,
						granny_log_message_origin Origin,
						char const*  File,
						granny_int32x Line,
						char const *Error,
						void *UserData)
{
	//Origin==GrannyFileReadingLogMessage for granny run-time tag& revision warning (Type==GrannyWarningLogMessage)
	//Origin==GrannyControlLogMessage for miss track_group on static models as weapons warning (Type==GrannyWarningLogMessage)
	//Origin==GrannyMeshBindingLogMessage for miss bone ToSkeleton on new ymir models error (Type==GrannyErrorLogMessage)
	// if (Type == GrannyWarningLogMessage)
	if (Origin==GrannyFileReadingLogMessage || Origin==GrannyControlLogMessage || Origin==GrannyMeshBindingLogMessage)
		return;
	TraceError("GRANNY: %s(%d): ERROR: %s --- [%d] %s --- [%d] %s", File, Line, Error, Type, GrannyGetLogMessageTypeString(Type), Origin, GrannyGetLogMessageOriginString(Origin));
}
#elif GrannyProductMinorVersion==11
static void GrannyError(granny_log_message_type Type,
						granny_log_message_origin Origin,
						char const*  File,
						granny_int32x Line,
						char const *Error,
						void *UserData)
{
	//Origin==GrannyFileReadingLogMessage for granny run-time tag& revision warning (Type==GrannyWarningLogMessage)
	//Origin==GrannyControlLogMessage for miss track_group on static models as weapons warning (Type==GrannyWarningLogMessage)
	//Origin==GrannyMeshBindingLogMessage for miss bone ToSkeleton on new ymir models error (Type==GrannyErrorLogMessage)
	// if (Type == GrannyWarningLogMessage)
	if (Origin==GrannyFileReadingLogMessage || Origin==GrannyControlLogMessage || Origin==GrannyMeshBindingLogMessage)
		return;
	TraceError("GRANNY: %s(%d): ERROR: %s --- [%d] %s --- [%d] %s", File, Line, Error, Type, GrannyGetLogMessageTypeString(Type), Origin, GrannyGetLogMessageOriginString(Origin));
}
#else
#error "unknown granny version"
#endif

int Setup(LPSTR lpCmdLine)
{
	/* 
	 *	타이머 정밀도를 올린다.
	 */
	TIMECAPS tc; 
	UINT wTimerRes; 

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		return 0;

	wTimerRes = MINMAX(tc.wPeriodMin, 1, tc.wPeriodMax); 
	timeBeginPeriod(wTimerRes); 

	/*
	 *	그래니 에러 핸들링
	 */

	//granny_log_callback Callback;
    //Callback.Function = GrannyError;
    //Callback.UserData = 0;
    //GrannySetLogCallback(&Callback);
	return 1;
}

