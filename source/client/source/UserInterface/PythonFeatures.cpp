#include "StdAfx.h"
#include "PythonFeatures.h"

#ifdef PYTHON_DYNAMIC_MODULE_NAME
#include "PythonDynamicModuleNames.h"
#endif

CPythonFeatures::CPythonFeatures()
{
	for (int i = 0; i < FEATURE_MAX_NUM; ++i)
		m_bFeaturesEnabled[i] = false;
}

CPythonFeatures::~CPythonFeatures()
{
}

void CPythonFeatures::Initialize()
{
	for (int i = 0; i < FEATURE_MAX_NUM; ++i)
		m_bFeaturesEnabled[i] = true;
}

PyObject* featuresIsFeatureEnabled(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bFeatureID;
	if (!PyTuple_GetInteger(poArgs, 0, &bFeatureID))
		return Py_BuildNone();
	
	bool isActivate = CPythonFeatures::instance().IsFeatureEnabled(bFeatureID);
	
	return Py_BuildValue("i", isActivate == true ? 1 : 0);
}

void initFeatures()
{
	static PyMethodDef s_methods[] = 
	{
		{"IsFeatureEnabled",featuresIsFeatureEnabled,METH_VARARGS},
		{NULL, NULL, NULL},
	};

#ifdef PYTHON_DYNAMIC_MODULE_NAME
	PyObject* poModule = Py_InitModule(GetModuleName(FEATURES_MODULE).c_str(), s_methods);
#else
	PyObject* poModule = Py_InitModule("features", s_methods);
#endif
	
	/*******************************************************************************************/
	/*** Features ***/
	/*******************************************************************************************/	
	PyModule_AddIntConstant(poModule, "TEST_FEATURES",										CPythonFeatures::TEST_FEATURES);
	PyModule_AddIntConstant(poModule, "WOLFMAN_FEATURES",									CPythonFeatures::WOLFMAN_FEATURES);
	PyModule_AddIntConstant(poModule, "WEAPON_RARITY_FEATURES",								CPythonFeatures::WEAPON_RARITY_FEATURES);
	PyModule_AddIntConstant(poModule, "PRIVATESHOP_SEARCH_FEATURES",						CPythonFeatures::PRIVATESHOP_SEARCH_FEATURES);
	PyModule_AddIntConstant(poModule, "SKILL_SELECT_EX_FEATURES",							CPythonFeatures::SKILL_SELECT_EX_FEATURES);
	PyModule_AddIntConstant(poModule, "HAIR_SELECT_EX_FEATURES",							CPythonFeatures::HAIR_SELECT_EX_FEATURES);
	PyModule_AddIntConstant(poModule, "POINT_BOXES_FEATURES",								CPythonFeatures::POINT_BOXES_FEATURES);
	PyModule_AddIntConstant(poModule, "CHEQUE_FEATURES",									CPythonFeatures::CHEQUE_FEATURES);
	PyModule_AddIntConstant(poModule, "ACCE_FEATURES",										CPythonFeatures::ACCE_FEATURES);
	PyModule_AddIntConstant(poModule, "CHANGELOOK_FEATURES",								CPythonFeatures::CHANGELOOK_FEATURES);
	PyModule_AddIntConstant(poModule, "COMBINATION_FEATURES",								CPythonFeatures::COMBINATION_FEATURES);
	PyModule_AddIntConstant(poModule, "SPECIAL_INVENTORY_FEATURES",							CPythonFeatures::SPECIAL_INVENTORY_FEATURES);
	PyModule_AddIntConstant(poModule, "GEM_FEATURES",										CPythonFeatures::GEM_FEATURES);
	PyModule_AddIntConstant(poModule, "CHANNEL_CHANGE_FEATURES",							CPythonFeatures::CHANNEL_CHANGE_FEATURES);
	PyModule_AddIntConstant(poModule, "AURA_FEATURES",										CPythonFeatures::AURA_FEATURES);
	PyModule_AddIntConstant(poModule, "PET_FEATURES",										CPythonFeatures::PET_FEATURES);
	PyModule_AddIntConstant(poModule, "COSTUME_SET_FEATURES",								CPythonFeatures::COSTUME_SET_FEATURES);
	PyModule_AddIntConstant(poModule, "SAFEBOX_MONEY_FEATURES",								CPythonFeatures::SAFEBOX_MONEY_FEATURES);
	PyModule_AddIntConstant(poModule, "VALUE_PACK_FEATURES",								CPythonFeatures::VALUE_PACK_FEATURES);
	PyModule_AddIntConstant(poModule, "BOSS_TRACKING_FEATURES",								CPythonFeatures::BOSS_TRACKING_FEATURES);
	PyModule_AddIntConstant(poModule, "DC_COUPON_FEATURES",									CPythonFeatures::DC_COUPON_FEATURES);
	PyModule_AddIntConstant(poModule, "MAIL_BOX_FEATURES",									CPythonFeatures::MAIL_BOX_FEATURES);
	PyModule_AddIntConstant(poModule, "REMOVE_SKILL_AFFECT_FEATURES",						CPythonFeatures::REMOVE_SKILL_AFFECT_FEATURES);
	PyModule_AddIntConstant(poModule, "OPEN_GIFTBOX_FEATURES",								CPythonFeatures::OPEN_GIFTBOX_FEATURES);
	PyModule_AddIntConstant(poModule, "TRANSFER_MONEY_FEATURES",							CPythonFeatures::TRANSFER_MONEY_FEATURES);
	PyModule_AddIntConstant(poModule, "GUILD_RANK_FEATURES",								CPythonFeatures::GUILD_RANK_FEATURES);
	PyModule_AddIntConstant(poModule, "AVERAGE_PRICE_FEATURES",								CPythonFeatures::AVERAGE_PRICE_FEATURES);
	PyModule_AddIntConstant(poModule, "SHOP_MONEY_FEATURES",								CPythonFeatures::SHOP_MONEY_FEATURES);
	PyModule_AddIntConstant(poModule, "FEATURE_MAX_NUM",									CPythonFeatures::FEATURE_MAX_NUM);
	/*******************************************************************************************/
	/*** END ***/
	/*******************************************************************************************/
}