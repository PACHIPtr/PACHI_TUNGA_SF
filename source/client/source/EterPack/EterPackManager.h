#pragma once

#include <windows.h>
#include <boost/unordered_map.hpp>
#include "../eterBase/Singleton.h"
#include "../eterBase/Stl.h"

#include "EterPack.h"

#ifdef ENABLE_FOX_FS
#include "foxfs/FoxFS_V2.h"
#endif

class CEterPackManager : public CSingleton<CEterPackManager>
{
	public:
		struct SCache
		{
			BYTE* m_abBufData;
			DWORD m_dwBufSize;
		};
	public:
#if defined(ENABLE_FOX_FS)
	enum ESearchModes
	{
		SEARCH_FILE,
		SEARCH_PACK
	};

	enum ESearchModes2
	{
		SEARCH_FILE_FIRST,
		SEARCH_PACK_FIRST
	};
#else
	enum ESearchModes
	{
		SEARCH_FILE_FIRST,
		SEARCH_PACK_FIRST
	};
#endif

		typedef std::list<CEterPack*> TEterPackList;
		typedef boost::unordered_map<std::string, CEterPack*, stringhash> TEterPackMap;

	public:
		CEterPackManager();
		virtual ~CEterPackManager();

		void SetCacheMode();
		void SetRelativePathMode();
		
		void LoadStaticCache(const char* c_szFileName);
		
		void SetSearchMode(bool bPackFirst);
		int GetSearchMode();

		//THEMIDA
		bool Get(CMappedFile & rMappedFile, const char * c_szFileName, LPCVOID * pData);
		
		//THEMIDA
		bool GetFromPack(CMappedFile & rMappedFile, const char * c_szFileName, LPCVOID * pData);
		
		//THEMIDA
		bool GetFromFile(CMappedFile & rMappedFile, const char * c_szFileName, LPCVOID * pData);
		bool isExist(const char * c_szFileName);
		bool isExistInPack(const char * c_szFileName);

		bool RegisterPack(const char * c_szName, const char * c_szDirectory, const BYTE* c_pbIV = NULL);		
		void RegisterRootPack(const char * c_szName);
		bool RegisterPackWhenPackMaking(const char * c_szName, const char * c_szDirectory, CEterPack* pPack);		



		const char * GetRootPackFileName();

	public:
		void ArrangeMemoryMappedPack();

	protected:
		int ConvertFileName(const char * c_szFileName, std::string & rstrFileName); // StringPath std::string ¹öÀü
		bool CompareName(const char * c_szDirectoryName, DWORD iLength, const char * c_szFileName);

		CEterPack* FindPack(const char* c_szPathName);

		SCache* __FindCache(DWORD dwFileNameHash);
		void	__ClearCacheMap();

	protected:
		bool					m_bTryRelativePath;
		bool					m_isCacheMode;
		int						m_iSearchMode;

		CEterFileDict			m_FileDict;
		CEterPack				m_RootPack;
		TEterPackList			m_PackList;
		TEterPackMap			m_PackMap;
		TEterPackMap			m_DirPackMap;		

		boost::unordered_map<DWORD, SCache> m_kMap_dwNameKey_kCache;

		CRITICAL_SECTION		m_csFinder;
#ifdef ENABLE_FOX_FS
		PFoxFS					m_pFoxFS;
#endif
};
