#include "StdAfx.h"
#include "../eterLib/Profiler.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

PyObject * profilerPush(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	return Py_BuildNone();
}

PyObject * profilerPop(PyObject * poSelf, PyObject * poArgs)
{
	char * szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	return Py_BuildNone();
}

void initProfiler()
{
	static PyMethodDef s_methods[] =
	{
		{ "Push",				profilerPush,				METH_VARARGS },
		{ "Pop",				profilerPop,				METH_VARARGS },

		{ NULL,					NULL,						NULL		 },
	};
	
#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(PROFILER_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("profiler", s_methods);
#endif

}