#include "HWIDManager.h"
#include "CMachineGuid.h"
#include "CHddData.h"
#include "CCpuId.h"
#include "CMacAddr.h"

HWIDMANAGER::HWIDMANAGER()
{
	m_MachineGUID	= "";
	m_MacAddr		= "";
	m_HDDSerial		= "";
	m_HDDModel		= "";
	m_CpuId			= "";
		
	CMachineGuid	c_MachineGuid;
	CMacAddr		c_MacAddr;
	CHddData		c_HddData;
	CCpuId			c_CpuId;

	m_MachineGUID	= c_MachineGuid.getMachineGUID();
	m_MacAddr		= c_MacAddr.getMacAddr();
	m_HDDSerial		= c_HddData.getHDDSerialNumber();
	m_HDDModel		= c_HddData.getHDDModelNumber();
	m_CpuId			= c_CpuId.getCpuID();
}