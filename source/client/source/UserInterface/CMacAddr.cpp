#include "CMacAddr.h"

CMacAddr::CMacAddr()
{
	findValidMac();
}

CMacAddr::~CMacAddr()
{
	if (m_pAdapters) {
		delete[] m_pAdapters;
		m_pAdapters = nullptr;
	}
}

bool CMacAddr::IsPrimaryAdapter(DWORD dwIndex)
{
	bool bIsPrimaryAdapter = false;
	if (m_pAdapters)
	{
		CNetworkAdapter* pAdapt = nullptr;

		DWORD dwMinIndex = m_pAdapters->GetAdapterIndex();
		for (unsigned int i = 0; i < m_nCount; i++)
		{
			pAdapt = &m_pAdapters[i];
			if (pAdapt->GetAdapterIndex() < dwMinIndex)
			{
				dwMinIndex = pAdapt->GetAdapterIndex();
			}
		}

		if (dwIndex == dwMinIndex)
			bIsPrimaryAdapter = true;
	}

	return bIsPrimaryAdapter;
}

bool CMacAddr::initAdapters()
{
	bool	bRet = true;
	DWORD	dwErr = 0;
	ULONG	ulNeeded = 0;

	dwErr = EnumNetworkAdapters(m_pAdapters, 0, &ulNeeded);
	if (dwErr == ERROR_INSUFFICIENT_BUFFER) {
		m_nCount = ulNeeded / sizeof(CNetworkAdapter);
		m_pAdapters = new CNetworkAdapter[ulNeeded / sizeof(CNetworkAdapter)];
		dwErr = EnumNetworkAdapters(m_pAdapters, ulNeeded, &ulNeeded);
		if (!m_pAdapters) {
			bRet = false;
		}
	}
	else{
		bRet = false;
	}
	return bRet;
}

bool CMacAddr::findValidMac()
{
	initAdapters(); // Get me the required adapter list

	DWORD dwIndex;

	CNetworkAdapter* pAdapt = nullptr;

	if (m_pAdapters) {
		for (UINT i = 0; i < m_nCount; i++)
		{
			pAdapt = &m_pAdapters[i];

			dwIndex = pAdapt->GetAdapterIndex();

			if (IsPrimaryAdapter(dwIndex))
			{
				setMACData(pAdapt->GetAdapterAddress());
				return true;
			}
		}
	}
	return false;
}

void CMacAddr::setMACData(std::string input)
{
	m_macAddr = input;
}