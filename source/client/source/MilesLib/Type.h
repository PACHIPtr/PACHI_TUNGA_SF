#pragma once

#include <vector>

namespace NSound
{
	extern std::string strResult;

	typedef struct SSoundData
	{
		float fTime;
		std::string strSoundFileName;
		float fSoundVolume;
	} TSoundData;
	typedef struct SSoundInstance
	{
		DWORD dwFrame;
		std::string strSoundFileName;
		float fSoundVolume;
	} TSoundInstance;
	typedef std::vector<TSoundData> TSoundDataVector;
	typedef std::vector<TSoundInstance> TSoundInstanceVector;

	bool			LoadSoundInformationPiece(const char * c_szFileName, TSoundDataVector & rSoundDataVector, const char * c_szPathHeader = nullptr);
	bool			SaveSoundInformationPiece(const char * c_szFileName, TSoundDataVector & rSoundDataVector);
	void			DataToInstance(const TSoundDataVector & c_rSoundDataVector, TSoundInstanceVector * pSoundInstanceVector);

	const char *	GetResultString();
	void			SetResultString(const char * c_pszStr);
};
