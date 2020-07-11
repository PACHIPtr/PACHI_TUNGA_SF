#include "CMachineGuid.h"

CMachineGuid::~CMachineGuid()
{
}

CMachineGuid::CMachineGuid()
{
	HKEY hKey;
	LSTATUS res = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring temp;
		res = GetStringRegKey(hKey, L"MachineGuid", temp, L"");
		if (res == ERROR_SUCCESS)
		{
			m_MachineGUID.assign(temp.begin(), temp.end());
		}
	}
	RegCloseKey(hKey);
}

LONG CMachineGuid::GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, nullptr, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
		strValue = szBuffer;

	return nError;
}