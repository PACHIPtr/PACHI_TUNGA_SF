#include "StdAfx.h"
#include "PythonSkillPet.h"

#include "../EterBase/Poly/Poly.h"
#include "../EterPack/EterPackManager.h"
#include "InstanceBase.h"
#include "PythonPlayer.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

bool CPythonSkillPet::RegisterSkillPet(const char * c_szFileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, c_szFileName, &pvData))
		return false;

	CMemoryTextFileLoader textFileLoader;
	textFileLoader.Bind(kFile.Size(), pvData);
	
	CTokenVector TokenVector;
	for (DWORD i = 0; i < textFileLoader.GetLineCount()-1; ++i)
	{
		if (!textFileLoader.SplitLine(i, &TokenVector, "\t"))
			return false;
		//TraceError("%s ---- %s | %s | %s", c_szFileName, TokenVector[0].c_str(),TokenVector[1].c_str(),TokenVector[2].c_str());
		//TraceError("SkillPetDesc %d %s %s %s %s \n", TokenVector.size(), TokenVector[0], TokenVector[1], TokenVecto[2], TokenVector[3]);
		if (DESCPET_TOKEN_TYPE_MAX_NUM > TokenVector.size())
		{
			//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", -1);
			return false;
		}
		//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", 3);
		DWORD iSkillIndex = atoi(TokenVector[DESCPET_TOKEN_TYPE_VNUM].c_str());
		if (iSkillIndex == 0)
		{
			//TraceError("SkillPetDesc.line(%d).NO_INDEX_ERROR\n", i + 1);
			continue;
		}

		
		
		//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", 4);
		
		/*
		std::map<DWORD, TSkillDataPet>::iterator it = m_SkillDataPetMap.find(1);
		if (m_SkillDataPetMap.end() == it)
		{
			TraceError("Arrivato alla fine");
			continue;
		}
		*/
		//TraceError("Ciao: %s",m_SkillDataPetMap[1].strIconFileName);
		
		m_SkillDataPetMap.insert(make_pair(iSkillIndex, TSkillDataPet()));
		
		
		
		
		
		//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", 5);
		TSkillDataPet & rSkillData = m_SkillDataPetMap[iSkillIndex];

		//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", 6);
		// Vnum
		rSkillData.dwSkillIndex = iSkillIndex;		
		//TraceError("SkillPetDesc.line(%d).NO_VNUM_ERROR\n", 7);
		// Name
		rSkillData.strName = TokenVector[DESCPET_TOKEN_TYPE_NAME];
		//Icon Name		
		char szIconFileNameHeader[64+1];
		if (iSkillIndex == 19)
			_snprintf(szIconFileNameHeader, sizeof(szIconFileNameHeader), "%sskill/pet/Metin_Avcisi.png", g_strImagePath.c_str());
		else if (iSkillIndex == 20)
			_snprintf(szIconFileNameHeader, sizeof(szIconFileNameHeader), "%sskill/pet/Metin_Avcisi.png", g_strImagePath.c_str());
		else
			_snprintf(szIconFileNameHeader, sizeof(szIconFileNameHeader), "%sskill/pet/%s.sub", g_strImagePath.c_str(), TokenVector[DESCPET_TOKEN_TYPE_ICON_NAME].c_str());

		rSkillData.strIconFileName = szIconFileNameHeader;

		//Type
		std::map<std::string, DWORD>::iterator itor = m_SkillPetTypeIndexMap.begin();
		for(; itor != m_SkillPetTypeIndexMap.end(); ++itor){
			if (TokenVector[DESCPET_TOKEN_TYPE].compare(itor->first) == 0)
					rSkillData.byType = itor->second;
		}
		rSkillData.strDescription = TokenVector[DESCPET_TOKEN_TYPE_DESCRIPTION];
		rSkillData.dwskilldelay = atoi(TokenVector[DESCPET_TOKEN_TYPE_DELAY].c_str());

		
		rSkillData.pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szIconFileNameHeader);			
		
	}

	return true;
}

void CPythonSkillPet::__RegisterNormalIconImage(TSkillDataPet & rData, const char * c_szHeader, const char * c_szImageName)
{
	std::string strFileName = "";
	strFileName += c_szHeader;
	strFileName += c_szImageName;
	strFileName += ".sub";
	rData.pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(strFileName.c_str());
	
}
void CPythonSkillPet::Destroy()
{
	
	m_SkillDataPetMap.clear();	
}

CPythonSkillPet::SSkillDataPet::SSkillDataPet()
{


	dwSkillIndex = 0;
	strName = "";
	strIconFileName = "";
	byType = 0;			
	strDescription = "";
	dwskilldelay = 0;
	pImage = nullptr;	
	
}

CPythonSkillPet::CPythonSkillPet()
{
	/*
	TSkillDataPet SkillDataPet;
	SkillDataPet.dwSkillIndex = 0;
	SkillDataPet.strName = "";
	SkillDataPet.strIconFileName = "";
	SkillDataPet.byType = 0;
	SkillDataPet.strDescription = "";
	SkillDataPet.dwskilldelay = 0;
	SkillDataPet.pImage = nullptr;
	
	m_SkillDataPetMap.insert(TSkillDataPetMap::value_type(0, SkillDataPet));
	*/
	petslot[0] = 0;
	petslot[1] = 0;
	petslot[2] = 0;

	m_SkillPetTypeIndexMap.insert(std::map<std::string, DWORD>::value_type("EMPTY", SKILLPET_TYPE_NONE));
	m_SkillPetTypeIndexMap.insert(std::map<std::string, DWORD>::value_type("PASSIVE", SKILLPET_TYPE_PASSIVE));
	m_SkillPetTypeIndexMap.insert(std::map<std::string, DWORD>::value_type("AUTO", SKILLPET_TYPE_AUTO));

}
CPythonSkillPet::~CPythonSkillPet()
{
}


void CPythonSkillPet:: SetSkillbySlot(int slot, int skillIndex)
{
	//TraceError("Setto lo slot %d --> skill %d", slot, skillIndex);
	petslot[slot] = skillIndex;
}

BOOL CPythonSkillPet::GetSkillData(DWORD dwSkillIndex, TSkillDataPet ** ppSkillData)
{
	TSkillDataPetMap::iterator it = m_SkillDataPetMap.find(dwSkillIndex);

	if (m_SkillDataPetMap.end() == it)
		return FALSE;

	*ppSkillData = &(it->second);
	return TRUE;
}

BOOL CPythonSkillPet::GetSkillIndex(int slot, int* skillIndex) 
{
	*skillIndex = petslot[slot];
	return TRUE;
}

PyObject * petskillSetSkillSlot(PyObject * poSelf, PyObject * poArgs)
{
	int slot;
	int skillIndex;

	if (!PyTuple_GetInteger(poArgs, 0, &slot))
		return Py_BadArgument();
	if (!PyTuple_GetInteger(poArgs, 1, &skillIndex))
		return Py_BadArgument();
	CPythonSkillPet::Instance().SetSkillbySlot(slot, skillIndex);
	return Py_BuildValue("i", 0);
}

PyObject * petskillGetIconImage(PyObject * poSelf, PyObject * poArgs)
{
	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Py_BadArgument();

	CPythonSkillPet::SSkillDataPet * c_pSkillData;
	if (!CPythonSkillPet::Instance().GetSkillData(iSkillIndex, &c_pSkillData))
		return Py_BuildValue("i", 0);	// 익셉션을 내는 대신 0을 리턴한다.

	return Py_BuildValue("i", c_pSkillData->pImage);
}

PyObject * petskillGetSkillbySlot(PyObject * poSelf, PyObject * poArgs)
{
	int slot;
	int skillIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &slot))
		return Py_BadArgument();

	if (slot > 2)
		return Py_BadArgument();

	if (!CPythonSkillPet::Instance().GetSkillIndex(slot, &skillIndex))
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", skillIndex);

}

PyObject * petskillGetEmptySkill(PyObject * poSelf, PyObject * poArgs)
{
	CGraphicImage * noskillbtn;
	char szIconFileNameHeader[64 + 1];
	_snprintf(szIconFileNameHeader, sizeof(szIconFileNameHeader), "d:/ymir work/ui/pet/skill_button/skill_enable_button.sub");
	noskillbtn = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szIconFileNameHeader);
	return Py_BuildValue("i", noskillbtn);
}

void initskillpet()
{
	static PyMethodDef s_methods[] =
	{
		///Variabili setting //
		{ "SetSkillSlot",					petskillSetSkillSlot,						METH_VARARGS },


		//Variabili Get //
		
		{ "GetIconImage",					petskillGetIconImage,						METH_VARARGS },
		{ "GetSkillbySlot",					petskillGetSkillbySlot ,					METH_VARARGS },
		{ "GetEmptySkill",					petskillGetEmptySkill,						METH_VARARGS },

		{ NULL,										NULL,										NULL },
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(SKILL_PET_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("skillpet", s_methods);
#endif
}