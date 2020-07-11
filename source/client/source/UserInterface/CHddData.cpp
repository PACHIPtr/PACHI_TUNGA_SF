#include "CHddData.h"

#include <functional>
#include <algorithm>

#include <sstream>

CHddData::CHddData()
{
	bool done = false;

	done = ReadPhysicalDriveInNTWithAdminRights();

	if (!done)
		done = ReadIdeDriveAsScsiDriveInNT();

	if (!done)
		done = ReadPhysicalDriveInNTWithZeroRights();

	if (!done)
		done = ReadPhysicalDriveInNTUsingSmart();
}

CHddData::~CHddData()
{
}

bool CHddData::ReadPhysicalDriveInNTWithAdminRights()
{
	bool done = false;
	int drive = 0; // If you want to iterate over all devices feel free to readd the for loop up to 16;

	HANDLE hPhysicalDriveIOCTL = 0;
	char driveName[256];

	sprintf_s(driveName, sizeof(driveName), "\\\\.\\PhysicalDrive%d", drive);

	hPhysicalDriveIOCTL = CreateFile(driveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr); //  Windows NT, Windows 2000, must have admin rights

	if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		return done;

	GETVERSIONOUTPARAMS VersionParams;
	DWORD               cbBytesReturned = 0;

	memset((void*)&VersionParams, 0, sizeof(VersionParams));
	if (!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION, nullptr, 0, &VersionParams, sizeof(VersionParams), &cbBytesReturned, nullptr)) // Get the version, etc of PhysicalDrive IOCTL
		return done;

	if (VersionParams.bIDEDeviceMap <= 0)
		return done;

	BYTE             bIDCmd = 0;
	SENDCMDINPARAMS  scip;

	bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
	IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

	memset(&scip, 0, sizeof(scip));
	memset(IdOutCmd, 0, sizeof(IdOutCmd));

	if (!DoIDENTIFY(hPhysicalDriveIOCTL, &scip, (PSENDCMDOUTPARAMS)&IdOutCmd, (BYTE)bIDCmd, (BYTE)drive, &cbBytesReturned))
		return done;

	DWORD diskdata[256];
	int ijk = 0;
	USHORT *pIdSector = (USHORT *)((PSENDCMDOUTPARAMS)IdOutCmd)->bBuffer;

	for (ijk = 0; ijk < 256; ijk++)
		diskdata[ijk] = pIdSector[ijk];

	SetDiskData(drive, diskdata);

	done = true;

	CloseHandle(hPhysicalDriveIOCTL);

	return done;
}

bool CHddData::ReadPhysicalDriveInNTUsingSmart()
{
	bool done = false;
	int drive = 0; // If you want to iterate over all devices feel free to readd the for loop up to 16;

	HANDLE hPhysicalDriveIOCTL = 0;
	char driveName[256];

	sprintf_s(driveName, sizeof(driveName), "\\\\.\\PhysicalDrive%d", drive);

	hPhysicalDriveIOCTL = CreateFile(driveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

	if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		return done;

	GETVERSIONINPARAMS GetVersionParams;
	DWORD cbBytesReturned = 0;

	memset((void*)& GetVersionParams, 0, sizeof(GetVersionParams)); 		// Get the version, etc of PhysicalDrive IOCTL

	if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_GET_VERSION, nullptr, 0, &GetVersionParams, sizeof(GETVERSIONINPARAMS), &cbBytesReturned, nullptr))
		return done;

	ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
	PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc(CommandSize);
	Command->irDriveRegs.bCommandReg = ID_CMD;
	DWORD BytesReturned = 0;

	if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS), Command, CommandSize, &BytesReturned, nullptr))
		return done;

	// Print the IDENTIFY data
	DWORD diskdata[256];
	USHORT *pIdSector = (USHORT *)(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

	for (int ijk = 0; ijk < 256; ijk++)
		diskdata[ijk] = pIdSector[ijk];

	SetDiskData(drive, diskdata);
	done = true;

	// Done
	CloseHandle(hPhysicalDriveIOCTL);
	free(Command);

	return done;
}

bool CHddData::ReadPhysicalDriveInNTWithZeroRights()
{
	bool done = false;
	int drive = 0; // If you want to iterate over all devices feel free to readd the for loop up to 16;

	HANDLE hPhysicalDriveIOCTL = 0;

	//  Try to get a handle to PhysicalDrive IOCTL, report failure and exit if can't.
	char driveName[256];

	sprintf_s(driveName, sizeof(driveName), "\\\\.\\PhysicalDrive%d", drive);

	//  Windows NT, Windows 2000, Windows XP - admin rights not required
	hPhysicalDriveIOCTL = CreateFile(driveName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		return done;

	char local_buffer[10000];
	STORAGE_PROPERTY_QUERY query;
	DWORD cbBytesReturned = 0;

	memset((void *)& query, 0, sizeof(query));
	query.PropertyId = StorageDeviceProperty;
	query.QueryType = PropertyStandardQuery;

	memset(local_buffer, 0, sizeof(local_buffer));

	if (!DeviceIoControl(hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query), &local_buffer[0], sizeof(local_buffer), &cbBytesReturned, nullptr))
		return done;

	STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *)& local_buffer;
	char serialNumber[1000];
	char modelNumber[1000];

	int swapNeeded = 0;

	// Im not sure if this is actually needed / possible but it seemed to fix some issue.

	unsigned int i = 1;
	char *c = (char*)&i;
	if (*c)
		swapNeeded = 0; //little endian
	else
		swapNeeded = 1; //big endian

	flipAndCodeBytes(local_buffer, descrip->SerialNumberOffset, swapNeeded, serialNumber);
	flipAndCodeBytes(local_buffer, descrip->ProductIdOffset, swapNeeded, modelNumber);

	if (0 == m_HDDSerialNumber[0] && (isalnum(serialNumber[0]) || isalnum(serialNumber[19])))
	{
		std::string s_SerialNumber(serialNumber);
		std::string s_ModelNumber(modelNumber);

		SetDiskData(s_ModelNumber, s_SerialNumber);
		done = true;
	}

	CloseHandle(hPhysicalDriveIOCTL);

	return done;
}

// DoIDENTIFY FUNCTION: Send an IDENTIFY command to the drive bDriveNum = 0-3 bIDCmd = IDE_ATA_IDENTIFY or IDE_ATAPI_IDENTIFY
bool CHddData::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum, PDWORD lpcbBytesReturned) {
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE; // Set up data structures for IDENTIFY command.
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg = 1;
	pSCIP->irDriveRegs.bCylLowReg = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4); 	// Compute the drive number.

	pSCIP->irDriveRegs.bCommandReg = bIDCmd; // The command can either be IDE identify or ATAPI identify.
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return (DeviceIoControl(hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA, (LPVOID)pSCIP, sizeof(SENDCMDINPARAMS) - 1, (LPVOID)pSCOP, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, lpcbBytesReturned, nullptr) != 0);
}

bool CHddData::ReadIdeDriveAsScsiDriveInNT()
{
	bool done = false;
	int controller = 0;

	for (controller = 0; controller < 16; controller++) //Required loop for this detection method dont remove <MartPwnS>
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   driveName[256];

		sprintf_s(driveName, sizeof(driveName), "\\\\.\\Scsi%d:", controller); 	//  Try to get a handle to PhysicalDrive IOCTL, report failure and exit if can't.

		hScsiDriveIOCTL = CreateFile(driveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

		if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
			continue;

		int drive = 0;

		char buffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH];
		SRB_IO_CONTROL *p = (SRB_IO_CONTROL *)buffer;
		SENDCMDINPARAMS *pin = (SENDCMDINPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
		DWORD dummy;

		memset(buffer, 0, sizeof(buffer));
		p->HeaderLength = sizeof(SRB_IO_CONTROL);
		p->Timeout = 10000;
		p->Length = SENDIDLENGTH;
		p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
		strncpy_s((char *)p->Signature, 8 + 1, "SCSIDISK", 8 + 1);

		pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
		pin->bDriveNumber = drive;

		if (!DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, buffer, sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDINPARAMS) - 1, buffer, sizeof(SRB_IO_CONTROL) + SENDIDLENGTH, &dummy, nullptr))
			continue;

		SENDCMDOUTPARAMS *pOut = (SENDCMDOUTPARAMS *)(buffer + sizeof(SRB_IO_CONTROL));
		IDSECTOR *pId = (IDSECTOR *)(pOut->bBuffer);
		if (!pId->sModelNumber[0])
			continue;

		DWORD diskdata[256];
		int ijk = 0;
		USHORT *pIdSector = (USHORT *)pId;

		for (ijk = 0; ijk < 256; ijk++)
			diskdata[ijk] = pIdSector[ijk];

		SetDiskData(controller * 2 + drive, diskdata);

		done = true;

		CloseHandle(hScsiDriveIOCTL);
		break;
	}

	return done;
}

void CHddData::SetDiskData(int drive, DWORD diskdata[256])
{
	std::string serialNumber;
	std::string modelNumber;

	ConvertToString(diskdata, 10, 19, serialNumber); 	//  copy the hard drive serial number to the buffer
	ConvertToString(diskdata, 27, 46, modelNumber); 	//  copy the hard drive model number to the buffer

	SetDiskData(modelNumber, serialNumber);
}

void CHddData::SetDiskData(std::string model, std::string serial)
{
	if (m_HDDSerialNumber.empty() && (isalnum(serial[0]) || isalnum(serial[19])) && !serial[0] == 0)
	{
		CleanWhitespaces(model);
		CleanWhitespaces(serial);

		m_HDDModelNumber = model;
		m_HDDSerialNumber = serial;
	}
}

void CHddData::ConvertToString(DWORD diskdata[256], int firstIndex, int lastIndex, std::string& buf) {
	std::stringstream ss;
	for (int index = firstIndex; index <= lastIndex; index++)
	{
		ss.put((char)(diskdata[index] / 256)); //  get high byte for 1st character
		ss.put((char)(diskdata[index] % 256)); //  get low byte for 2nd character
	}
	buf = ss.str();
	CleanWhitespaces(buf);
}

void CHddData::CleanWhitespaces(std::string& buf)
{
	buf.erase(std::remove(buf.begin(), buf.end(), ' '), buf.end()); // remove whitespaces from everywhere
}

char * CHddData::flipAndCodeBytes(const char * str, int pos, int flip, char * buf)
{
	int i;
	int j = 0;
	int k = 0;

	buf[0] = '\0';
	if (pos <= 0)
		return buf;

	if (!j)
	{
		char p = 0;

		// First try to gather all characters representing hex digits only.
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = tolower(str[i]);

			if (isspace(c))
				c = '0';

			++p;
			buf[k] <<= 4;

			if (c >= '0' && c <= '9')
				buf[k] |= (unsigned char)(c - '0');
			else if (c >= 'a' && c <= 'f')
				buf[k] |= (unsigned char)(c - 'a' + 10);
			else
			{
				j = 0;
				break;
			}

			if (p == 2)
			{
				if (buf[k] != '\0' && !isprint(buf[k]))
				{
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}
		}
	}

	if (!j)
	{
		// There are non-digit characters, gather them as is.
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = str[i];

			if (!isprint(c))
			{
				j = 0;
				break;
			}

			buf[k++] = c;
		}
	}

	if (!j)
	{
		// The characters are not there or are not printable.
		k = 0;
	}

	buf[k] = '\0';

	if (flip)
	{		// Flip adjacent characters
		for (j = 0; j < k; j += 2)
		{
			char t = buf[j];
			buf[j] = buf[j + 1];
			buf[j + 1] = t;
		}
	}

	// Trim any beginning and end space
	i = j = -1;
	for (k = 0; buf[k] != '\0'; ++k)
	{
		if (!isspace(buf[k]))
		{
			if (i < 0)
				i = k;
			j = k;
		}
	}

	if ((i >= 0) && (j >= 0))
	{
		for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
			buf[k - i] = buf[k];
		buf[k - i] = '\0';
	}

	return buf;
}