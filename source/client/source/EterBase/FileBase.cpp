#include "StdAfx.h"
#include "FileBase.h"

CFileBase::CFileBase() : m_hFile(nullptr), m_dwSize(0)
{
}

CFileBase::~CFileBase()
{
	Destroy();
}

char * CFileBase::GetFileName()
{
	return m_filename;
}

void CFileBase::Destroy()
{
	Close();
	m_dwSize = 0;
}

void CFileBase::Close()
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);	
		m_hFile = nullptr;
	}
}

BOOL CFileBase::Create(const char* filename, EFileMode mode)
{
	Destroy();

	strncpy(m_filename, filename, MAX_PATH);

	DWORD dwMode, dwShareMode = FILE_SHARE_READ;

	if (mode == FILEMODE_WRITE)
	{
		dwMode = GENERIC_READ | GENERIC_WRITE;
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	}
	else
		dwMode = GENERIC_READ;

	m_hFile = CreateFile(filename,					// name of the file
						 dwMode,					// desired access
						 dwShareMode,				// share mode
						 nullptr,						// security attributes
						 mode == FILEMODE_READ ? OPEN_EXISTING : OPEN_ALWAYS, // creation disposition
						 FILE_ATTRIBUTE_NORMAL,		// flags and attr
						 nullptr);						// template file

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_dwSize = GetFileSize(m_hFile, nullptr);
		m_mode = mode;
		return true;
	}

/*	char buf[256];
	GetCurrentDirectory(256, buf);
	DWORD dwErr = GetLastError();*/
	m_hFile = nullptr;
	return false;
}

DWORD CFileBase::Size()
{
	return (m_dwSize);
}

void CFileBase::SeekCur(DWORD size)
{
	SetFilePointer(m_hFile, size, nullptr, FILE_CURRENT);
}

void CFileBase::Seek(DWORD offset)
{
	if (offset > m_dwSize)
		offset = m_dwSize;

	SetFilePointer(m_hFile, offset, nullptr, FILE_BEGIN);
}

DWORD CFileBase::GetPosition()
{
	return SetFilePointer(m_hFile, 0, nullptr, FILE_CURRENT);
}

BOOL CFileBase::Write(const void* src, int bytes)
{
	DWORD dwUseless;
	BOOL ret = WriteFile(m_hFile, src, bytes, &dwUseless, nullptr);
	
	if (!ret)
		return false;

	m_dwSize = GetFileSize(m_hFile, nullptr);
	return true;
}

BOOL CFileBase::Read(void* dest, int bytes)
{
	DWORD dwUseless;
	return ReadFile(m_hFile, dest, bytes, &dwUseless, nullptr);
}

BOOL CFileBase::IsNull()
{
	return !m_hFile ? true : false;
}
