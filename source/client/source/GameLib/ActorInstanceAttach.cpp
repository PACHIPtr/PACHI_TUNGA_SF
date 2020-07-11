#include "StdAfx.h"
#include "../EffectLib/EffectManager.h"
#include "../UserInterface/Locale_inc.h"
#include "ActorInstance.h"
#include "ItemData.h"
#include "ItemManager.h"
#include "RaceData.h"
#include "WeaponTrace.h"
#include "RaceManager.h"

BOOL USE_WEAPON_SPECULAR = TRUE;


DWORD CActorInstance::AttachSmokeEffect(DWORD eSmoke)
{
	if (!m_pkCurRaceData)
		return 0;

	DWORD dwSmokeEffectID=m_pkCurRaceData->GetSmokeEffectID(eSmoke);

	return AttachEffectByID(0, m_pkCurRaceData->GetSmokeBone().c_str(), dwSmokeEffectID); 
}

bool CActorInstance::__IsLeftHandWeapon(DWORD type)
{
	if (CItemData::WEAPON_DAGGER == type || (CItemData::WEAPON_FAN == type && __IsMountingHorse()))
		return true;
	else if (CItemData::WEAPON_BOW == type)
		return true;
	else if (CItemData::WEAPON_CLAW == type)
		return true;	
	else
		return false;
}

bool CActorInstance::__IsRightHandWeapon(DWORD type)
{
	if (CItemData::WEAPON_DAGGER == type || (CItemData::WEAPON_FAN == type && __IsMountingHorse()))
		return true;
	else if (CItemData::WEAPON_BOW == type)
		return false;
	else if (CItemData::WEAPON_CLAW == type)
		return true;	
	else 
		return true;
}

bool CActorInstance::__IsWeaponTrace(DWORD weaponType)
{
	switch(weaponType)
	{
	case CItemData::WEAPON_BELL:
	case CItemData::WEAPON_FAN:
	case CItemData::WEAPON_BOW:	
		return false;
	default:
		return true;

	}
}

void CActorInstance::AttachWeapon(DWORD dwItemIndex, DWORD dwParentPartIndex, DWORD dwPartIndex)
{
	if (dwPartIndex>=CRaceData::PART_MAX_NUM)
		return;
	
	m_adwPartItemID[dwPartIndex]=dwItemIndex;

	
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwItemIndex, &pItemData))
	{
		RegisterModelThing(dwPartIndex, nullptr);
		SetModelInstance(dwPartIndex, dwPartIndex, 0);

		RegisterModelThing(CRaceData::PART_WEAPON_LEFT, nullptr);
		SetModelInstance(CRaceData::PART_WEAPON_LEFT, CRaceData::PART_WEAPON_LEFT, 0);

		RefreshActorInstance();
		return;
	}
	
	__DestroyWeaponTrace();
	
	DWORD dwWeaponType = pItemData->GetWeaponType();

	if (m_eRace == 8)
	{
		char* szAttachingBoneName = "equip_right_weapon";

		if (dwWeaponType != CItemData::WEAPON_CLAW && pItemData->GetSubType() != CItemData::COSTUME_WEAPON)
			szAttachingBoneName = "equip_right";
		m_pkCurRaceData->ChangeAttachingBoneName(CRaceData::PART_WEAPON, szAttachingBoneName);
	}
	__DestroyWeaponTrace();
	if (pItemData->GetType() != CItemData::ITEM_TYPE_COSTUME)
	{
		if (__IsRightHandWeapon(dwWeaponType))
			AttachWeapon(dwParentPartIndex, CRaceData::PART_WEAPON, pItemData);
		
		if (__IsLeftHandWeapon(dwWeaponType))
			AttachWeapon(dwParentPartIndex, CRaceData::PART_WEAPON_LEFT, pItemData);
	}
	else
	{
		DWORD typeDec = pItemData->GetValue(3);
		if (__IsRightHandWeapon(typeDec))
			AttachWeapon(dwParentPartIndex, CRaceData::PART_WEAPON, pItemData);
		
		if (__IsLeftHandWeapon(typeDec))
			AttachWeapon(dwParentPartIndex, CRaceData::PART_WEAPON_LEFT, pItemData);
	}
}

BOOL CActorInstance::GetAttachingBoneName(DWORD dwPartIndex, const char ** c_pszBoneName)
{
	return m_pkCurRaceData->GetAttachingBoneName(dwPartIndex, c_pszBoneName);
}

void CActorInstance::AttachWeapon(DWORD dwParentPartIndex, DWORD dwPartIndex, CItemData * pItemData)
{
	if (!pItemData)
		return;
	
	const char * szBoneName;
	if (!GetAttachingBoneName(dwPartIndex, &szBoneName))
		return;
	
	if (CRaceData::PART_WEAPON_LEFT == dwPartIndex)
	{
		RegisterModelThing(dwPartIndex, pItemData->GetSubModelThing());
	}
	else
	{
		RegisterModelThing(dwPartIndex, pItemData->GetModelThing());
	}
	
	for (DWORD i = 0; i < pItemData->GetLODModelThingCount(); ++i)
	{
		CGraphicThing * pThing;
		if (!pItemData->GetLODModelThingPointer(i, &pThing))
			continue;
		
		RegisterLODThing(dwPartIndex, pThing);
	}
	if (IsPC() && (this->GetRace() == 8))
	{

		DWORD dwItemVnum = pItemData->GetIndex();
		if ((dwItemVnum > 29100 && dwItemVnum <= 29111) || pItemData->GetType() == CItemData::ITEM_TYPE_ROD)
		{

			szBoneName = "equip_right";
		}
	}
	SetModelInstance(dwPartIndex, dwPartIndex, 0);
	AttachModelInstance(dwParentPartIndex, szBoneName, dwPartIndex);
	if (USE_WEAPON_SPECULAR)
	{
		SMaterialData kMaterialData;
		kMaterialData.pImage = nullptr;
		kMaterialData.isSpecularEnable = TRUE;
		kMaterialData.fSpecularPower = pItemData->GetSpecularPowerf();
		kMaterialData.bSphereMapIndex = 1;
		SetMaterialData(dwPartIndex, nullptr, kMaterialData);
	}
	
	if (pItemData->GetType() != CItemData::ITEM_TYPE_COSTUME)
	{
		if (__IsWeaponTrace(pItemData->GetWeaponType()))
		{
			CWeaponTrace * pWeaponTrace = CWeaponTrace::New();
			pWeaponTrace->SetWeaponInstance(this, dwPartIndex, szBoneName);
			m_WeaponTraceVector.push_back(pWeaponTrace);
		}
	}
	else
	{
		DWORD typeDec = pItemData->GetValue(3);
		if (__IsWeaponTrace(typeDec))
		{
			CWeaponTrace * pWeaponTrace = CWeaponTrace::New();
			pWeaponTrace->SetWeaponInstance(this, dwPartIndex, szBoneName);
			m_WeaponTraceVector.push_back(pWeaponTrace);
		}
	}
}

void  CActorInstance::DettachEffect(DWORD dwEID)
{
	std::list<TAttachingEffect>::iterator i = m_AttachingEffectList.begin();

	while (i != m_AttachingEffectList.end())
	{
		TAttachingEffect & rkAttEft = (*i);

		if (rkAttEft.dwEffectIndex == dwEID)
		{
			i = m_AttachingEffectList.erase(i);
			CEffectManager::Instance().DestroyEffectInstance(dwEID);
		}
		else
		{
			++i;
		}
	}
}

DWORD CActorInstance::AttachEffectByName(DWORD dwParentPartIndex, const char * c_pszBoneName, const char * c_pszEffectName)
{
	std::string str;
	DWORD dwCRC;
	StringPath(c_pszEffectName, str);
	dwCRC = GetCaseCRC32(str.c_str(), str.length());

	return AttachEffectByID(dwParentPartIndex, c_pszBoneName, dwCRC);
}

DWORD CActorInstance::AttachEffectByID(DWORD dwParentPartIndex, const char * c_pszBoneName, DWORD dwEffectID, const D3DXVECTOR3 * c_pv3Position)
{
	TAttachingEffect ae;
	ae.iLifeType = EFFECT_LIFE_INFINITE;
	ae.dwEndTime = 0;
	ae.dwModelIndex = dwParentPartIndex;
	ae.dwEffectIndex = CEffectManager::Instance().GetEmptyIndex();
	ae.isAttaching = TRUE;
	if (c_pv3Position)
	{
		D3DXMatrixTranslation(&ae.matTranslation, c_pv3Position->x, c_pv3Position->y, c_pv3Position->z);
	}
	else
	{
		D3DXMatrixIdentity(&ae.matTranslation);
	}
	CEffectManager& rkEftMgr=CEffectManager::Instance();
	rkEftMgr.CreateEffectInstance(ae.dwEffectIndex, dwEffectID);

	if (c_pszBoneName)
	{
		int iBoneIndex;
		
		if (!FindBoneIndex(dwParentPartIndex,c_pszBoneName, &iBoneIndex))
		{
			ae.iBoneIndex = -1;
			//Tracef("Cannot get Bone Index\n");
			//assert(false && "Cannot get Bone Index");
		}
		else
		{
			ae.iBoneIndex = iBoneIndex;
		}
	}
	else
	{
		ae.iBoneIndex = -1;
	}

	m_AttachingEffectList.push_back(ae);

	return ae.dwEffectIndex;
}

void CActorInstance::RefreshActorInstance()
{
	if (!m_pkCurRaceData)
	{
		TraceError("void CActorInstance::RefreshActorInstance() - m_pkCurRaceData=nullptr");
		return;
	}

	// This is Temporary place before making the weapon detection system
	// Setup Collison Detection Data
	m_BodyPointInstanceList.clear();
	//m_AttackingPointInstanceList.clear();
	m_DefendingPointInstanceList.clear();

	// Base
	for (DWORD i = 0; i < m_pkCurRaceData->GetAttachingDataCount(); ++i)
	{
		const NRaceData::TAttachingData * c_pAttachingData;

		if (!m_pkCurRaceData->GetAttachingDataPointer(i, &c_pAttachingData))
			continue;

		switch (c_pAttachingData->dwType)
		{
			case NRaceData::ATTACHING_DATA_TYPE_COLLISION_DATA:
			{
				const NRaceData::TCollisionData * c_pCollisionData = c_pAttachingData->pCollisionData;

				TCollisionPointInstance PointInstance;
				if (NRaceData::COLLISION_TYPE_ATTACKING == c_pCollisionData->iCollisionType)
					continue;

				if (!CreateCollisionInstancePiece(CRaceData::PART_MAIN, c_pAttachingData, &PointInstance))
					continue;

				switch (c_pCollisionData->iCollisionType)
				{
					case NRaceData::COLLISION_TYPE_ATTACKING:
						//m_AttackingPointInstanceList.push_back(PointInstance);
						break;
					case NRaceData::COLLISION_TYPE_DEFENDING:
						m_DefendingPointInstanceList.push_back(PointInstance);
						break;
					case NRaceData::COLLISION_TYPE_BODY:
						m_BodyPointInstanceList.push_back(PointInstance);
						break;
				}
			}
			break;

			case NRaceData::ATTACHING_DATA_TYPE_EFFECT:
//				if (!m_bEffectInitialized)
//				{
//					DWORD dwCRC;
//					StringPath(c_pAttachingData->pEffectData->strFileName);
//					dwCRC = GetCaseCRC32(c_pAttachingData->pEffectData->strFileName.c_str(),c_pAttachingData->pEffectData->strFileName.length());
//
//					TAttachingEffect ae;
//					ae.iLifeType = EFFECT_LIFE_INFINITE;
//					ae.dwEndTime = 0;
//					ae.dwModelIndex = 0;
//					ae.dwEffectIndex = CEffectManager::Instance().GetEmptyIndex();
//					ae.isAttaching = TRUE;
//					CEffectManager::Instance().CreateEffectInstance(ae.dwEffectIndex, dwCRC);
//
//					if (c_pAttachingData->isAttaching)
//					{
//						int iBoneIndex;
//						if (!FindBoneIndex(0,c_pAttachingData->strAttachingBoneName.c_str(), &iBoneIndex))
//						{
//							Tracef("Cannot get Bone Index\n");
//							assert(false/*Cannot get Bone Index*/);
//						}
//
//						ae.iBoneIndex = iBoneIndex;
//					}
//					else
//					{
//						ae.iBoneIndex = -1;
//					}
//
//					m_AttachingEffectList.push_back(ae);
//				}

				if (c_pAttachingData->isAttaching)
				{
					AttachEffectByName(0, c_pAttachingData->strAttachingBoneName.c_str(), c_pAttachingData->pEffectData->strFileName.c_str());
				}
				else
				{
					AttachEffectByName(0, 0, c_pAttachingData->pEffectData->strFileName.c_str());
				}
				break;

			case NRaceData::ATTACHING_DATA_TYPE_OBJECT:
				break;

			default:
				assert(false/*NOT_IMPLEMENTED*/);
				break;
		}
	}

	for (DWORD j = 0; j < CRaceData::PART_MAX_NUM; ++j)
	{
		if (0 == m_adwPartItemID[j])
			continue;

		CItemData * pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(m_adwPartItemID[j], &pItemData))
			return;

		for (DWORD k = 0; k < pItemData->GetAttachingDataCount(); ++k)
		{
			const NRaceData::TAttachingData * c_pAttachingData;

			if (!pItemData->GetAttachingDataPointer(k, &c_pAttachingData))
				continue;

			switch(c_pAttachingData->dwType)
			{
				case NRaceData::ATTACHING_DATA_TYPE_COLLISION_DATA:
					{
						const NRaceData::TCollisionData * c_pCollisionData = c_pAttachingData->pCollisionData;
						
						// FIXME : 첫번째 인자는 Part의 번호다.
						//         Base는 무조건 0인가? - [levites]
						TCollisionPointInstance PointInstance;
						if (NRaceData::COLLISION_TYPE_ATTACKING == c_pCollisionData->iCollisionType)
							continue;
						if (!CreateCollisionInstancePiece(j, c_pAttachingData, &PointInstance))
							continue;
						
						switch (c_pCollisionData->iCollisionType)
						{
						case NRaceData::COLLISION_TYPE_ATTACKING:
							//m_AttackingPointInstanceList.push_back(PointInstance);
							break;
						case NRaceData::COLLISION_TYPE_DEFENDING:
							m_DefendingPointInstanceList.push_back(PointInstance);
							break;
						case NRaceData::COLLISION_TYPE_BODY:
							m_BodyPointInstanceList.push_back(PointInstance);
							break;
						}
					}
					break;

				case NRaceData::ATTACHING_DATA_TYPE_EFFECT:
					if (!m_bEffectInitialized)
					{
						DWORD dwCRC;
						StringPath(c_pAttachingData->pEffectData->strFileName);
						dwCRC = GetCaseCRC32(c_pAttachingData->pEffectData->strFileName.c_str(),c_pAttachingData->pEffectData->strFileName.length());

						TAttachingEffect ae;
						ae.iLifeType = EFFECT_LIFE_INFINITE;
						ae.dwEndTime = 0;
						ae.dwModelIndex = j;
						ae.dwEffectIndex = CEffectManager::Instance().GetEmptyIndex();
						ae.isAttaching = TRUE;
						CEffectManager::Instance().CreateEffectInstance(ae.dwEffectIndex, dwCRC);

						int iBoneIndex;
						if (!FindBoneIndex(j,c_pAttachingData->strAttachingBoneName.c_str(), &iBoneIndex))
						{
							Tracef("Cannot get Bone Index\n");
							assert(false/*Cannot get Bone Index*/);
						}
						Tracef("Creating %p %d %d\n", this, j,k);

						ae.iBoneIndex = iBoneIndex;

						m_AttachingEffectList.push_back(ae);
					}
					break;

				case NRaceData::ATTACHING_DATA_TYPE_OBJECT:
					break;

				default:
					assert(false/*NOT_IMPLEMENTED*/);
					break;
			}
		}
	}

	m_bEffectInitialized = true;
}

void CActorInstance::SetWeaponTraceTexture(const char * szTextureName)
{
	std::vector<CWeaponTrace*>::iterator it;
	for (it = m_WeaponTraceVector.begin(); it != m_WeaponTraceVector.end(); ++it)
	{
		(*it)->SetTexture(szTextureName);
	}
}

void CActorInstance::UseTextureWeaponTrace()
{
	for_each(
			m_WeaponTraceVector.begin(),
			m_WeaponTraceVector.end(),
			std::void_mem_fun(&CWeaponTrace::UseTexture)
			);
}

void CActorInstance::UseAlphaWeaponTrace()
{
	for_each(
			m_WeaponTraceVector.begin(),
			m_WeaponTraceVector.end(),
			std::void_mem_fun(&CWeaponTrace::UseAlpha)
			);
}

void CActorInstance::UpdateAttachingInstances()
{
	CEffectManager& rkEftMgr=CEffectManager::Instance();

	std::list<TAttachingEffect>::iterator it;
	DWORD dwCurrentTime = CTimer::Instance().GetCurrentMillisecond();
	for (it = m_AttachingEffectList.begin(); it!= m_AttachingEffectList.end();)
	{
		if (EFFECT_LIFE_WITH_MOTION == it->iLifeType)
		{
			++it;
			continue;
		}

		if ((EFFECT_LIFE_NORMAL == it->iLifeType && it->dwEndTime < dwCurrentTime) ||
			!rkEftMgr.IsAliveEffect(it->dwEffectIndex))
		{
			rkEftMgr.DestroyEffectInstance(it->dwEffectIndex);
			it = m_AttachingEffectList.erase(it);
		}
		else
		{
			if (it->isAttaching)
			{
				rkEftMgr.SelectEffectInstance(it->dwEffectIndex);

				if (it->iBoneIndex == -1)
				{
					D3DXMATRIX matTransform;
					matTransform = it->matTranslation;
					matTransform *= m_worldMatrix;
					rkEftMgr.SetEffectInstanceGlobalMatrix(matTransform);
				}
				else
				{
					D3DXMATRIX * pBoneMat;
					if (GetBoneMatrix(it->dwModelIndex, it->iBoneIndex, &pBoneMat))
					{
						D3DXMATRIX matTransform;
						matTransform = *pBoneMat;
						matTransform *= it->matTranslation;
						matTransform *= m_worldMatrix;
						rkEftMgr.SetEffectInstanceGlobalMatrix(matTransform);
					}
					else
					{						
						//TraceError("GetBoneMatrix(modelIndex(%d), boneIndex(%d)).NOT_FOUND_BONE", 
						//	it->dwModelIndex, it->iBoneIndex);
					}
				}
			}

			++it;
		}
	}
}

void CActorInstance::ShowAllAttachingEffect()
{
	std::list<TAttachingEffect>::iterator it;
	for(it = m_AttachingEffectList.begin(); it!= m_AttachingEffectList.end();++it)
	{
		CEffectManager::Instance().SelectEffectInstance(it->dwEffectIndex);
		CEffectManager::Instance().ShowEffect();
	}
}

void CActorInstance::SetActiveAllAttachingEffect()
{
	std::list<TAttachingEffect>::iterator it;
	for (it = m_AttachingEffectList.begin(); it != m_AttachingEffectList.end(); ++it)
	{
		CEffectManager::Instance().ActiveEffectInstance(it->dwEffectIndex);
	}
}

void CActorInstance::SetDeactiveAllAttachingEffect()
{
	std::list<TAttachingEffect>::iterator it;
	for (it = m_AttachingEffectList.begin(); it != m_AttachingEffectList.end(); ++it)
	{
		CEffectManager::Instance().DeactiveEffectInstance(it->dwEffectIndex);
	}
}

void CActorInstance::HideAllAttachingEffect()
{
	std::list<TAttachingEffect>::iterator it;
	for(it = m_AttachingEffectList.begin(); it!= m_AttachingEffectList.end();++it)
	{
		CEffectManager::Instance().SelectEffectInstance(it->dwEffectIndex);
		CEffectManager::Instance().HideEffect();
	}
}

void CActorInstance::__ClearAttachingEffect()
{
	m_bEffectInitialized = false;

	std::list<TAttachingEffect>::iterator it;
	for(it = m_AttachingEffectList.begin(); it!= m_AttachingEffectList.end();++it)
	{
		CEffectManager::Instance().DestroyEffectInstance(it->dwEffectIndex);
	}
	m_AttachingEffectList.clear();
}

#ifdef ENABLE_ACCE_SYSTEM
void CActorInstance::AttachSash(CItemData * pItemData, float fSpecular)
{
	if (!pItemData)
	{
		RegisterModelThing(CRaceData::PART_SASH, nullptr);
		SetModelInstance(CRaceData::PART_SASH, CRaceData::PART_SASH, 0);
		RefreshActorInstance();
		return;
	}
	
	RegisterModelThing(CRaceData::PART_SASH, pItemData->GetModelThing());
	SetModelInstance(CRaceData::PART_SASH, CRaceData::PART_SASH, 0);
	AttachModelInstance(CRaceData::PART_MAIN, "Bip01 Spine2", CRaceData::PART_SASH);
	
	if (fSpecular > 0.0f)
	{
		SMaterialData kMaterialData;
		kMaterialData.pImage = nullptr;
		kMaterialData.isSpecularEnable = TRUE;
		kMaterialData.fSpecularPower = fSpecular;
		kMaterialData.bSphereMapIndex = 1;
		SetMaterialData(CRaceData::PART_SASH, nullptr, kMaterialData);
	}
}
#endif

#ifdef ENABLE_AURA_SYSTEM
void CActorInstance::AttachAura(CItemData * pItemData)
{
	if (!pItemData)
	{
		RegisterModelThing(CRaceData::PART_AURA, nullptr);
		SetModelInstance(CRaceData::PART_AURA, CRaceData::PART_AURA, 0);
		RefreshActorInstance();
		return;
	}
	
	RegisterModelThing(CRaceData::PART_AURA, pItemData->GetModelThing());
	SetModelInstance(CRaceData::PART_AURA, CRaceData::PART_AURA, 0);
	AttachModelInstance(CRaceData::PART_MAIN, "Bip01 Spine2", CRaceData::PART_AURA);
}
#endif

#ifdef ENABLE_QUIVER_SYSTEM
void CActorInstance::SetQuiverEquipped(bool bEquipped)
{
	m_bIsQuiverEquipped = bEquipped;
}
void CActorInstance::SetQuiverEffectID(DWORD dwEffectID)
{
	m_dwQuiverEffectID = dwEffectID;
}
#endif