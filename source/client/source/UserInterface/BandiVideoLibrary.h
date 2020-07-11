////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// BandiVideoLibrary 2.1
/// 
/// Copyright(C) 2008-2011 BandiSoft.com All rights reserved.
///
/// Visit http://www.bandisoft.com for more information.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.h"
#include <bandivid/bandivid.h>



#ifdef _WIN64
#	ifdef _DEBUG
#		define BANDIVIDEO_DLL_FILE_NAME		_T("bdvid64_d.dll")
#	else
#		define BANDIVIDEO_DLL_FILE_NAME		_T("bdvid64.dll")
#	endif
#	define BANDIVIDEO_RELEASE_DLL_FILE_NAME	_T("bdvid64.dll")
#else
#	ifdef _DEBUG
#		define BANDIVIDEO_DLL_FILE_NAME		_T("bdvid32_d.dll")
#	else
#		define BANDIVIDEO_DLL_FILE_NAME		_T("bdvid32.dll")
#	endif
#	define BANDIVIDEO_RELEASE_DLL_FILE_NAME	_T("bdvid32.dll")
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// utility class for load library and create instance for video library
////////////////////////////////////////////////////////////////////////////////////////////////////
class CBandiVideoLibrary : public IBandiVideo2
{
public :
	CBandiVideoLibrary();
	~CBandiVideoLibrary();

	HRESULT Create(LPCTSTR dll_path_name, IBandiVideoFileReader *reader = nullptr, IBandiVideoSound *sound = nullptr);
	void	Destroy();
	BOOL	IsCreated();

public :		
	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** p) { return E_NOTIMPL; }
	ULONG   STDMETHODCALLTYPE AddRef(void)	{return 0;}
	ULONG   STDMETHODCALLTYPE Release(void)	{return 0;}

	// IBandiVideo2
	HRESULT		Verify(LPCSTR szID, LPCSTR szKey);		// 인증하기			
	INT			GetVersion();							// 버전 정보		

	HRESULT		Open(LPCWSTR path_name, BOOL async = FALSE);
	HRESULT		Open(LPCSTR path_name, BOOL async = FALSE);
	HRESULT		Close();
	HRESULT		Play();
	HRESULT		Stop();
	HRESULT		Seek(INT64 value, BVL_SEEK_FLAG seek_flag);
	HRESULT		Pause(BOOL pause);

	BOOL		IsNextFrame();
	HRESULT		GetFrame(BVL_FRAME &frame, BOOL can_skip);
	HRESULT		GetVideoInfo(BVL_VIDEO_INFO &info);
	HRESULT		GetStatus(BVL_STATUS &status);

	HRESULT		SetVolume(INT volume);				// 0 ~ 255 

private :
	HMODULE					m_dll;
	LPCREATEBANDIVIDEO2		m_pCreateBandiVideo2;

public:
	IBandiVideo2*			m_bvl;
};
