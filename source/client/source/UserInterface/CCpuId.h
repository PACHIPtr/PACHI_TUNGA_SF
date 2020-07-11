#ifndef _CCpuId_H____
#define _CCpuId_H____

#include <string>

class CCpuId
{
public:
	CCpuId();
	~CCpuId();

	std::string getCpuID() { return m_CpuID; };

private:
	std::string m_CpuID;
};

#endif //CCpuId