#include "CCpuId.h"

#include <intrin.h>

CCpuId::CCpuId()
{
	int registers[4];
	char cpuIDHex[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	char* part1 = &cpuIDHex[0];
	char* part2 = &cpuIDHex[8];

	__cpuid(registers, 1);

	sprintf_s(part1, 9, "%08X", registers[3]); 	//reverse byte order (endiannes) MartPwnS
	sprintf_s(part2, 9, "%08X", registers[0]);

	m_CpuID = std::string(cpuIDHex);
}

CCpuId::~CCpuId(){
}