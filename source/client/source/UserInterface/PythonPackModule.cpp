#include "StdAfx.h"
#include "../eterPack/EterPackManager.h"
#include "../eterBase/tea.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif


//#include <eterCrypt.h>

PyObject * packExist(PyObject * poSelf, PyObject * poArgs)
{
	char * strFileName;

	if (!PyTuple_GetString(poArgs, 0, &strFileName))
		return Py_BuildException();

	return Py_BuildValue("i", CEterPackManager::Instance().isExist(strFileName)?1:0);
}

PyObject * packGet(PyObject * poSelf, PyObject * poArgs)
{
	char * strFileName;
	if (!PyTuple_GetString(poArgs, 0, &strFileName))
		return Py_BuildException();
	
	int iEncKey;
	if (!PyTuple_GetInteger(poArgs, 1, &iEncKey))
		return Py_BuildException();
	
	if (iEncKey != 1120091120)
		return Py_BuildValue("s","TAKTIK GELISTIR CAHIL CUHELA UNPACK BURDANMI YAPILIR");

	// 파이썬에서 읽어드리는 패킹 파일은 python 파일과 txt 파일에 한정한다
	const char* pcExt = strrchr(strFileName, '.');
	if (pcExt) // 확장자가 있고
	{
		CMappedFile file;
		const void * pData = nullptr;

		if (CEterPackManager::Instance().Get(file,strFileName,&pData))
			return Py_BuildValue("s#",pData, file.Size());
	}

	return Py_BuildException();
}

void initpack()
{
	static PyMethodDef s_methods[] =
	{
		{ "Exist",		packExist,		METH_VARARGS },
		{ "Get",		packGet,		METH_VARARGS },
		{ nullptr, nullptr },		
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	Py_InitModule(GetModuleName(PACK_MODULE).c_str(), s_methods);
#else
	Py_InitModule("pack", s_methods);
#endif
}