#ifndef FOXFS_H
#define FOXFS_H

#if defined(_MSC_VER)
	#define FOXFS_MSVC
#else
	#define FOXFS_GNUC
#endif

#if defined(FOXFS_DYNAMIC)
	#if defined(FOXFS_BUILDING)
		#if defined(FOXFS_MSVC)
			#define FOXFS_EXPORT __declspec(dllexport)
		#else
			#define FOXFS_EXPORT
		#endif
	#else
		#if defined(FOXFS_MSVC)
			#define FOXFS_EXPORT __declspec(dllimport)
		#else
			#define FOXFS_EXPORT
		#endif
	#endif
#else
	#define FOXFS_EXPORT
#endif

#if defined(FOXFS_MSVC)
	#define FOXFS_API __fastcall
#else
	#define FOXFS_API __attribute__((fastcall))
#endif

typedef void* PFoxFS;

#if defined(__cplusplus)
extern "C"
{
#endif

	PFoxFS FOXFS_EXPORT FOXFS_API FoxFS_Create();
	void FOXFS_EXPORT FOXFS_API FoxFS_Destroy(PFoxFS manager);
	
	int FOXFS_EXPORT FOXFS_API FoxFS_SetKeyServerA(PFoxFS manager, const char* hostname, unsigned short port);
	int FOXFS_EXPORT FOXFS_API FoxFS_SetKeyServerW(PFoxFS manager, const wchar_t* hostname, unsigned short port);
	int FOXFS_EXPORT FOXFS_API FoxFS_LoadA(PFoxFS manager, const char* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_LoadW(PFoxFS manager, const wchar_t* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_UnloadA(PFoxFS manager, const char* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_UnloadW(PFoxFS manager, const wchar_t* filename);
	
	unsigned int FOXFS_EXPORT FOXFS_API FoxFS_SizeA(PFoxFS manager, const char* filename);
	unsigned int FOXFS_EXPORT FOXFS_API FoxFS_SizeW(PFoxFS manager, const wchar_t* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_ExistsA(PFoxFS manager, const char* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_ExistsW(PFoxFS manager, const wchar_t* filename);
	int FOXFS_EXPORT FOXFS_API FoxFS_GetA(PFoxFS manager, const char* filename, void* buffer, unsigned int maxsize, unsigned int* outsize);
	int FOXFS_EXPORT FOXFS_API FoxFS_GetW(PFoxFS manager, const wchar_t* filename, void* buffer, unsigned int maxsize, unsigned int* outsize);

#if defined(__cplusplus)
}
#endif

#endif