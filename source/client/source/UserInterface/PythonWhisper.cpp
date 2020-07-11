#include "StdAfx.h"

#ifdef ENABLE_WHISPER_RENEWAL
#include "PythonWhisper.h"
#include "PythonNetworkStream.h"

PyObject * AddName(PyObject * poSelf, PyObject * poArgs)
{
	char* Name;
	if (!PyTuple_GetString(poArgs, 0, &Name))
		return Py_BadArgument();
	CPythonNetworkStream::Instance().SendWhisperPacket(Name, "|?whisper_renewal>|");
	CPythonWhisper::Instance().AddName(Name, CPythonWhisper::ME);
	return Py_BuildNone();
}

PyObject * DeleteName(PyObject * poSelf, PyObject * poArgs)
{
	char* Name;
	if (!PyTuple_GetString(poArgs, 0, &Name))
		return Py_BadArgument();
	CPythonNetworkStream::Instance().SendWhisperPacket(Name, "|?whisper_renewal<|");
	CPythonWhisper::Instance().DeleteName(Name, CPythonWhisper::ME);
	return Py_BuildNone();
}

PyObject * CheckName(PyObject * poSelf, PyObject * poArgs)
{
	char* Name;
	if (!PyTuple_GetString(poArgs, 0, &Name))
		return Py_BadArgument();
	return Py_BuildValue("i", CPythonWhisper::Instance().CheckName(Name, CPythonWhisper::TARGET));
}

PyObject * IsSended(PyObject * poSelf, PyObject * poArgs)
{
	char* Name;
	if (!PyTuple_GetString(poArgs, 0, &Name))
		return Py_BadArgument();
	return Py_BuildValue("i", CPythonWhisper::Instance().CheckName(Name, CPythonWhisper::ME));
}

void initWhisper()
{
	static PyMethodDef functions[] = {
		{"Add", AddName, METH_VARARGS}, // to target
		{"Remove", DeleteName, METH_VARARGS}, // to target
		{"CheckName", CheckName, METH_VARARGS}, // my list
		{"IsSended", IsSended, METH_VARARGS},
		{NULL, NULL, NULL},
	};
	
	PyObject* pModule = Py_InitModule("whisper", functions);
}
#endif
