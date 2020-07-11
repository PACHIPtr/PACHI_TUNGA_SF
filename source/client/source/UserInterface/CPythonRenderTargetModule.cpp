#include "StdAfx.h"
#include "PythonApplication.h"


PyObject* renderTargetSelectModel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SelectModel(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetSetVisibility(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	bool isShow = false;
	if (!PyTuple_GetBoolean(poArgs, 1, &isShow))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetVisibility(isShow);

	return Py_BuildNone();
}

PyObject* renderTargetSetBackground(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	char * szPathName;
	if (!PyTuple_GetString(poArgs, 1, &szPathName))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->CreateBackground(
		szPathName, CPythonApplication::Instance().GetWidth(),
		CPythonApplication::Instance().GetHeight());
	return Py_BuildNone();
}

PyObject* renderTargetSetScale(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();
	
	float xScale;
	if (!PyTuple_GetFloat (poArgs, 1, &xScale))
		return Py_BadArgument();
	float yScale;
	if (!PyTuple_GetFloat (poArgs, 2, &yScale))
		return Py_BadArgument();
	float zScale;
	if (!PyTuple_GetFloat (poArgs, 3, &zScale))
		return Py_BadArgument();
	
	CRenderTargetManager::Instance().GetRenderTarget(index)->SetScale(zScale, yScale, xScale);
	return Py_BuildNone();
}

PyObject* renderTargetChangeArmor(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();
	
	DWORD vnum = 0;
	if (!PyTuple_GetByte(poArgs, 1, &index))
		return Py_BadArgument();
	
	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeArmor(vnum);
	return Py_BuildNone();
}

PyObject* renderTargetChangeWeapon(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();
	
	DWORD vnum = 0;
	if (!PyTuple_GetByte(poArgs, 1, &index))
		return Py_BadArgument();
	
	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeWeapon(vnum);
	return Py_BuildNone();
}

PyObject* renderTargetChangeHair(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();
	
	DWORD vnum = 0;
	if (!PyTuple_GetByte(poArgs, 1, &index))
		return Py_BadArgument();
	
	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeHair(vnum);
	return Py_BuildNone();
}

PyObject* renderTargetChangeSash(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();
	
	DWORD vnum = 0;
	if (!PyTuple_GetByte(poArgs, 1, &index))
		return Py_BadArgument();
	
	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeSash(vnum);
	return Py_BuildNone();
}

void initRenderTarget() {
	static PyMethodDef s_methods[] =
	{
		{ "SelectModel", renderTargetSelectModel, METH_VARARGS },
		{ "SetVisibility", renderTargetSetVisibility, METH_VARARGS },
		{ "SetBackground", renderTargetSetBackground, METH_VARARGS },
		{ "SetScale",	renderTargetSetScale,	METH_VARARGS },
		{ "ChangeArmor",	renderTargetChangeArmor,	METH_VARARGS },
		{ "ChangeWeapon",	renderTargetChangeWeapon,	METH_VARARGS },
		{ "ChangeHair",		renderTargetChangeHair,		METH_VARARGS },
		{ "ChangeSash",		renderTargetChangeSash,		METH_VARARGS },

		{nullptr, nullptr, 0 },
	};

	PyObject* poModule = Py_InitModule("renderTarget", s_methods);

}