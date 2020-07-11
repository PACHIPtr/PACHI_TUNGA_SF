#include "stdafx.h"
#include "Locale_inc.h"

#pragma comment(lib,	"speedtree.lib"		)
#pragma comment(lib,	"python.lib"		)
#pragma comment(lib,	"granny.lib"		)
#ifdef _DEBUG
#pragma comment(lib,	"cryptlib_debug.lib"		)
#else
#pragma comment(lib,	"cryptlib.lib"		)
#endif


#pragma comment( lib, "dinput8.lib" )
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "ddraw.lib" )
#pragma comment( lib, "dmoguids.lib" )

#pragma comment( lib, "devil.lib" )
#pragma comment( lib, "mss32.lib" )

#pragma comment( lib, "version.lib" )
#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "imm32.lib" )
#pragma comment( lib, "oldnames.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "strmiids.lib" )

#pragma comment( lib, "jpeg.lib" )
#pragma comment( lib, "lzo2.lib" )
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib")

#ifdef ENABLE_FOX_FS
#pragma comment( lib, "lz4.lib" )
#pragma comment( lib, "xxhash.lib" )
#ifdef _DEBUG
#pragma comment( lib, "FoxFS_d.lib" )
#else
#pragma comment( lib, "FoxFS.lib" )
#endif
#endif

#ifdef ENABLE_CRASH_SENDER
#pragma comment( lib, "CrashRpt1403.lib")
#pragma comment( lib, "CrashRptProbe1403.lib")
#endif

#ifdef __USE_CYTHON__
#ifdef _DEBUG
	#pragma comment (lib, "rootlib_d.lib")
#else
	#pragma comment (lib, "rootlib.lib")
#endif
#endif