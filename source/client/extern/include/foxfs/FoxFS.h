#ifndef __M2VFS__H__
#define __M2VFS__H__

#ifdef M2VFS_DYNAMIC
#ifdef BUILD_M2VFS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif

#define M2VFS_API __stdcall

typedef void* PM2VFS;

#ifdef __cplusplus
extern "C"
{
#endif
PM2VFS DLL_EXPORT M2VFS_API M2VFS_Create();
void   DLL_EXPORT M2VFS_API M2VFS_Destroy(PM2VFS hM2VFS);

BOOL   DLL_EXPORT M2VFS_API M2VFS_SetKeyServerA(PM2VFS hM2VFS, LPCSTR lpszHostName, WORD wPort);
BOOL   DLL_EXPORT M2VFS_API M2VFS_SetKeyServerW(PM2VFS hM2VFS, LPCWSTR lpszHostName, WORD wPort);
BOOL   DLL_EXPORT M2VFS_API M2VFS_LoadA(PM2VFS hM2VFS, LPCSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_LoadW(PM2VFS hM2VFS, LPCWSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_UnloadA(PM2VFS hM2VFS, LPCSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_UnloadW(PM2VFS hM2VFS, LPCWSTR lpszFileName);

DWORD  DLL_EXPORT M2VFS_API M2VFS_SizeA(PM2VFS hM2VFS, LPCSTR lpszFileName);
DWORD  DLL_EXPORT M2VFS_API M2VFS_SizeW(PM2VFS hM2VFS, LPCWSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_ExistsA(PM2VFS hM2VFS, LPCSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_ExistsW(PM2VFS hM2VFS, LPCWSTR lpszFileName);
BOOL   DLL_EXPORT M2VFS_API M2VFS_GetA(PM2VFS hM2VFS, LPCSTR lpszFileName, void* pBuffer, DWORD dwMaxSize, LPDWORD lpdwOutSize);
BOOL   DLL_EXPORT M2VFS_API M2VFS_GetW(PM2VFS hM2VFS, LPCWSTR lpszFileName, void* pBuffer, DWORD dwMaxSize, LPDWORD lpdwOutSize);
#ifdef __cplusplus
}
#endif

#endif // __M2VFS__H__
