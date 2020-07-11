#include "StdAfx.h"
#include "InstanceBase.h"
#include "PythonBackground.h"
#include "PythonNonPlayer.h"
#include "PythonPlayer.h"
#include "PythonCharacterManager.h"
#include "AbstractPlayer.h"
#include "AbstractApplication.h"
#include "packet.h"
#include "Locale_inc.h"
#include "PythonSystem.h"

#include "../eterlib/StateManager.h"
#include "../gamelib/ItemManager.h"
#include "../gameLib/RaceManager.h"

#include "VnumHelper.h"

BOOL HAIR_COLOR_ENABLE=FALSE;
BOOL USE_ARMOR_SPECULAR=FALSE;
BOOL RIDE_HORSE_ENABLE=TRUE;
const float c_fDefaultRotationSpeed = 1200.0f;
const float c_fDefaultHorseRotationSpeed = 300.0f;


bool IsWall(unsigned race)
{
	switch (race)
	{
		case 14201:
		case 14202:
		case 14203:
		case 14204:
			return true;
			break;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////


CInstanceBase::SHORSE::SHORSE()
{
	__Initialize();
}

CInstanceBase::SHORSE::~SHORSE()
{
	assert(m_pkActor==nullptr);
}

void CInstanceBase::SHORSE::__Initialize()
{
	m_isMounting=false;
	m_pkActor=nullptr;
}

void CInstanceBase::SHORSE::SetAttackSpeed(UINT uAtkSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetAttackSpeed(uAtkSpd/100.0f);	
}

void CInstanceBase::SHORSE::SetMoveSpeed(UINT uMovSpd)
{	
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetMoveSpeed(uMovSpd/100.0f);
}

void CInstanceBase::SHORSE::Create(const TPixelPosition& c_rkPPos, UINT eRace, UINT eHitEffect)
{
	assert(nullptr==m_pkActor && "CInstanceBase::SHORSE::Create - ALREADY MOUNT");

	m_pkActor=new CActorInstance;

	CActorInstance& rkActor=GetActorRef();
	rkActor.SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());
	if (!rkActor.SetRace(eRace))
	{
		delete m_pkActor;
		m_pkActor=nullptr;
		return;
	}

	rkActor.SetShape(0);
	rkActor.SetBattleHitEffect(eHitEffect);
	rkActor.SetAlphaValue(0.0f);
	rkActor.BlendAlphaValue(1.0f, 0.5f);
	rkActor.SetMoveSpeed(1.0f);
	rkActor.SetAttackSpeed(1.0f);
	rkActor.SetMotionMode(CRaceMotionData::MODE_GENERAL);
	rkActor.Stop();
	rkActor.RefreshActorInstance();

	rkActor.SetCurPixelPosition(c_rkPPos);

	m_isMounting=true;
}

void CInstanceBase::SHORSE::Destroy()
{
	if (m_pkActor)
	{
		m_pkActor->Destroy();
		delete m_pkActor;	
	}	

	__Initialize();
}

CActorInstance& CInstanceBase::SHORSE::GetActorRef()
{
	assert(nullptr!=m_pkActor && "CInstanceBase::SHORSE::GetActorRef");
	return *m_pkActor;
}

CActorInstance* CInstanceBase::SHORSE::GetActorPtr()
{
	return m_pkActor;
}

UINT CInstanceBase::SHORSE::GetLevel()
{
	if (m_pkActor)
	{
		DWORD mount = m_pkActor->GetRace();
		switch (mount)
		{
			case 20101:
			case 20102:
			case 20103:
				return 1;
			case 20104:
			case 20105:
			case 20106:
				return 2;
			case 20107:
			case 20108:
			case 20109:
			case 20110: // #0000673: [M2EU] 새로운 탈것 타고 공격 안됨 
			case 20111: // #0000673: [M2EU] 새로운 탈것 타고 공격 안됨 
			case 20112: // #0000673: [M2EU] 새로운 탈것 타고 공격 안됨 
			case 20113: // #0000673: [M2EU] 새로운 탈것 타고 공격 안됨 
			case 20114:
			case 20115:
			case 20116:
			case 20117:
			case 20118:
			case 20120:
			case 20121:
			case 20122:
			case 20123:
			case 20124:
			case 20125:
			case 20226:
			case 20227:
			case 20228:
			case 20253:
			case 20254:
			case 20255:
			case 20256:
			case 20233:
			case 20234:
			case 20235:
			case 20236:
			case 20237:
			case 20238:
			case 20222:
			case 20530:
			case 20531:
			case 20532:
			case 20533:
			case 20534:
			case 20535:
			case 20536:
			case 20537:
			case 30240:
			case 30241:
			case 30242:
			case 30243:
			case 30244:
			case 20538:
			case 20539:
			case 20540:
			case 20541:
			case 20542:
			case 20543:
			case 20544:
			case 20545:
			case 20546:
			case 20547:
			case 20548:
			case 20549:
			case 20550:
			case 20551:
			case 20552:
			case 20553:
			case 20558:
			case 20559:
			case 20259:
			case 20260:
				return 3;
			case 20119: // 라마단 이벤트용 흑마는 스킬불가, 공격가능한 레벨2로 설정
			case 20219: // 할로윈 이벤트용 흑마는 스킬불가, 공격가능한 레벨2로 설정 (=라마단 흑마 클론)
			case 20220:
			case 20221:
			case 20229:
			case 20230:
			case 20231:
			case 20232:
			case 20265:
				return 2;
		}

		// 마운트 확장 시스템용 특수 처리 (20201 ~ 20212 대역을 사용하고 순서대로 4개씩 나눠서 초급, 중급, 고급임)
		//	-- 탈것 레벨을 클라에서 측정하고 공격/스킬 사용가능 여부도 클라에서 처리하는 것 자체에 문제가 있는 듯.. [hyo]
		{
			// 중급 탈것은 레벨2 (공격 가능, 스킬 불가)
			if ((20205 <= mount &&  20208 >= mount) ||
				(20214 == mount) || (20217 == mount)			// 난폭한 전갑순순록, 난폭한 전갑암순록
				)
				return 2;

			// 고급 탈것은 레벨3 (공격 가능, 스킬 가능)
			if ((20209 <= mount &&  20212 >= mount) || 
				(20215 == mount) || (20218 == mount) ||	(20119 == mount) ||		// 용맹한 전갑순순록, 용맹한 전갑암순록
				(20219 <= mount &&  20224 >= mount) ||
				(20225 <= mount &&  20227 >= mount) ||
				(20230 <= mount &&  20299 >= mount))
				return 3;
		}
	}
	return 0;
}

bool CInstanceBase::SHORSE::IsNewMount()
{
	if (!m_pkActor)
		return false;
	DWORD mount = m_pkActor->GetRace();

	if ((20205 <= mount &&  20208 >= mount) ||
		(20214 == mount) || (20217 == mount)			// 난폭한 전갑순순록, 난폭한 전갑암순록
		)
		return true;

	// 고급 탈것
	if ((20209 <= mount &&  20212 >= mount) || 
		(20215 == mount) || (20218 == mount) ||	(20119 == mount) ||		// 용맹한 전갑순순록, 용맹한 전갑암순록
		(20219 <= mount &&  20224 >= mount) ||
		(20225 <= mount &&  20227 >= mount) ||
		(20230 <= mount &&  20241 >= mount))
		return true;

	return false;
}

bool CInstanceBase::SHORSE::IsManuManni()
{
	if (!m_pkActor)
		return false;
	
	DWORD mount = m_pkActor->GetRace();
	if (mount == 20246 || mount == 20247)
		return true;
	
	return false;
}

bool CInstanceBase::SHORSE::CanUseSkill()
{
	// 마상스킬은 말의 레벨이 3 이상이어야만 함.
	if (IsMounting())
		return 2 < GetLevel();

	return true;
}

bool CInstanceBase::SHORSE::CanAttack()
{
	if (IsMounting())
		if (GetLevel()<=1)
			return false;

	return true;
}
			
bool CInstanceBase::SHORSE::IsMounting()
{
	return m_isMounting;
}

void CInstanceBase::SHORSE::Deform()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.INSTANCEBASE_Deform();
}

void CInstanceBase::SHORSE::Render()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor=GetActorRef();
	rkActor.Render();
}

void CInstanceBase::__AttachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->AttachModelInstance(CRaceData::PART_MAIN, "saddle", m_GraphicThingInstance, CRaceData::PART_MAIN);
}

void CInstanceBase::__DetachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->DetachModelInstance(CRaceData::PART_MAIN, m_GraphicThingInstance, CRaceData::PART_MAIN);
}

//////////////////////////////////////////////////////////////////////////////////////

void CInstanceBase::BlockMovement()
{
	m_GraphicThingInstance.BlockMovement();
}

bool CInstanceBase::IsBlockObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.IsBlockObject(c_rkBGObj);
}

bool CInstanceBase::AvoidObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.AvoidObject(c_rkBGObj);
}

///////////////////////////////////////////////////////////////////////////////////

bool __ArmorVnumToShape(int iVnum, DWORD * pdwShape)
{
	*pdwShape = iVnum;

	/////////////////////////////////////////

	if (0 == iVnum || 1 == iVnum)
		return false;

	if (!USE_ARMOR_SPECULAR)
		return false;

	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iVnum, &pItemData))
		return false;

	enum
	{
		SHAPE_VALUE_SLOT_INDEX = 3,
	};

	*pdwShape = pItemData->GetValue(SHAPE_VALUE_SLOT_INDEX);

	return true;
}

DWORD __GetHairVnum(DWORD dwShape)
{
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Hair)), &pItemData))
		return dwShape;	
	
	return pItemData->GetIndex();
}

// 2004.07.05.myevan.궁신탄영 끼이는 문제
class CActorInstanceBackground : public IBackground
{
	public:
		CActorInstanceBackground() {}
		virtual ~CActorInstanceBackground() {}
		bool IsBlock(int x, int y)
		{
			CPythonBackground& rkBG=CPythonBackground::Instance();
			return rkBG.isAttrOn(x, y, CTerrainImpl::ATTRIBUTE_BLOCK);
		}
};

static CActorInstanceBackground gs_kActorInstBG;

bool CInstanceBase::LessRenderOrder(CInstanceBase* pkInst)
{
	int nMainAlpha=(__GetAlphaValue() < 1.0f) ? 1 : 0;
	int nTestAlpha=(pkInst->__GetAlphaValue() < 1.0f) ? 1 : 0;
	if (nMainAlpha < nTestAlpha)
		return true;
	if (nMainAlpha > nTestAlpha)
		return false;

	if (GetRace()<pkInst->GetRace())
		return true;
	if (GetRace()>pkInst->GetRace())
		return false;

	if (GetShape()<pkInst->GetShape())
		return true;

	if (GetShape()>pkInst->GetShape())
		return false;

	UINT uLeftLODLevel=__LessRenderOrder_GetLODLevel();
	UINT uRightLODLevel=pkInst->__LessRenderOrder_GetLODLevel();
	if (uLeftLODLevel<uRightLODLevel)
		return true;
	if (uLeftLODLevel>uRightLODLevel)
		return false;

	if (m_awPart[CRaceData::PART_WEAPON]<pkInst->m_awPart[CRaceData::PART_WEAPON])
		return true;

	return false;
}

UINT CInstanceBase::__LessRenderOrder_GetLODLevel()
{
	CGrannyLODController* pLODCtrl=m_GraphicThingInstance.GetLODControllerPointer(0);
	if (!pLODCtrl)
		return 0;

	return pLODCtrl->GetLODLevel();
}

bool CInstanceBase::__Background_GetWaterHeight(const TPixelPosition& c_rkPPos, float* pfHeight)
{
	long lHeight;
	if (!CPythonBackground::Instance().GetWaterHeight(int(c_rkPPos.x), int(c_rkPPos.y), &lHeight))
		return false;

	*pfHeight = float(lHeight);

	return true;
}

bool CInstanceBase::__Background_IsWaterPixelPosition(const TPixelPosition& c_rkPPos)
{
	return CPythonBackground::Instance().isAttrOn(c_rkPPos.x, c_rkPPos.y, CTerrainImpl::ATTRIBUTE_WATER);
}

const float PC_DUST_RANGE = 2000.0f;
const float NPC_DUST_RANGE = 1000.0f;

DWORD CInstanceBase::ms_dwUpdateCounter=0;
DWORD CInstanceBase::ms_dwRenderCounter=0;
DWORD CInstanceBase::ms_dwDeformCounter=0;

CDynamicPool<CInstanceBase> CInstanceBase::ms_kPool;

bool CInstanceBase::__IsInDustRange()
{
	if (!__IsExistMainInstance())
		return false;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);

	if (IsPC())
	{
		if (fDistance<=PC_DUST_RANGE)
			return true;
	}

	if (fDistance<=NPC_DUST_RANGE)
		return true;

	return false;
}

void CInstanceBase::__EnableSkipCollision()
{
	if (__IsMainInstance())
	{
		TraceError ("CInstanceBase::__EnableSkipCollision - You should not skip your own collisions!!");
		return;
	}
	m_GraphicThingInstance.EnableSkipCollision();
}

void CInstanceBase::__DisableSkipCollision()
{
	m_GraphicThingInstance.DisableSkipCollision();
}

DWORD CInstanceBase::__GetShadowMapColor(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetShadowMapColor(x, y);
}

float CInstanceBase::__GetBackgroundHeight(float x, float y)
{
	CPythonBackground& rkBG=CPythonBackground::Instance();
	return rkBG.GetHeight(x, y);
}

#ifdef __MOVIE_MODE__

BOOL CInstanceBase::IsMovieMode()
{
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;

	return false;
}

#endif

BOOL CInstanceBase::IsInvisibility()
{
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;

	return false;
}

BOOL CInstanceBase::IsParalysis()
{
	return m_GraphicThingInstance.IsParalysis();
}

BOOL CInstanceBase::IsGameMaster()
{
	if (m_kAffectFlagContainer.IsSet(AFFECT_YMIR))
		return true;
	return false;
}

#ifdef ENABLE_YOUTUBER_SYSTEM
BOOL CInstanceBase::IsYoutuber()
{
	if (m_kAffectFlagContainer.IsSet(49))
		return true;
	return false;
}

BOOL CInstanceBase::IsTwitcher()
{
	if (m_kAffectFlagContainer.IsSet(50))
		return true;
	return false;
}
#endif

BOOL CInstanceBase::IsSameEmpire(CInstanceBase& rkInstDst)
{
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (IsCombatZoneMap() || rkInstDst.IsCombatZoneMap())
		return FALSE;
#endif

	if (0 == rkInstDst.m_dwEmpireID)
		return TRUE;

	if (IsGameMaster())
		return TRUE;

	if (rkInstDst.IsGameMaster())
		return TRUE;

	if (rkInstDst.m_dwEmpireID==m_dwEmpireID)
		return TRUE;
	
	return FALSE;
}

DWORD CInstanceBase::GetEmpireID()
{
	return m_dwEmpireID;
}

DWORD CInstanceBase::GetGuildID()
{
	return m_dwGuildID;
}

#if defined(WJ_SHOW_MOB_INFO)
DWORD CInstanceBase::GetAIFlag()
{
	return m_dwAIFlag;
}
#endif

#ifdef ENABLE_ALIGNMENT_SYSTEM
long long CInstanceBase::GetAlignment()
{
	return m_sAlignment;
}
#endif

UINT CInstanceBase::GetAlignmentGrade()
{
#ifdef ENABLE_ALIGNMENT_SYSTEM
	if (m_sAlignment >= 250000)
		return 0;
	else if (m_sAlignment >= 100000)
		return 1;
	else if (m_sAlignment >= 50000)
		return 2;
	else if (m_sAlignment >= 12000)
		return 3;
	else if (m_sAlignment >= 8000)
		return 4;
	else if (m_sAlignment >= 4000)
		return 5;
	else if (m_sAlignment >= 1000)
		return 6;
	else if (m_sAlignment >= 0)
		return 7;
	else if (m_sAlignment > -4000)
		return 8;
	else if (m_sAlignment > -8000)
		return 9;
	else if (m_sAlignment > -12000)
		return 10;

	return 11;
#else
	if (m_sAlignment >= 12000)
		return 0;
	else if (m_sAlignment >= 8000)
		return 1;
	else if (m_sAlignment >= 4000)
		return 2;
	else if (m_sAlignment >= 1000)
		return 3;
	else if (m_sAlignment >= 0)
		return 4;
	else if (m_sAlignment > -4000)
		return 5;
	else if (m_sAlignment > -8000)
		return 6;
	else if (m_sAlignment > -12000)
		return 7;

	return 8;
#endif
}

int CInstanceBase::GetAlignmentType()
{
#ifdef ENABLE_ALIGNMENT_SYSTEM
	switch (GetAlignmentGrade())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		{
			return ALIGNMENT_TYPE_WHITE;
			break;
		}

		case 8:
		case 9:
		case 10:
		case 11:
		{
			return ALIGNMENT_TYPE_DARK;
			break;
		}
	}

	return ALIGNMENT_TYPE_NORMAL;
#else
	switch (GetAlignmentGrade())
	{
	case 0:
	case 1:
	case 2:
	case 3:
	{
			  return ALIGNMENT_TYPE_WHITE;
			  break;
	}

	case 5:
	case 6:
	case 7:
	case 8:
	{
			  return ALIGNMENT_TYPE_DARK;
			  break;
	}
	}

	return ALIGNMENT_TYPE_NORMAL;
#endif
}

BYTE CInstanceBase::GetPKMode()
{
	return m_byPKMode;
}

bool CInstanceBase::IsKiller()
{
	return m_isKiller;
}

bool CInstanceBase::IsPartyMember()
{
	return m_isPartyMember;
}

BOOL CInstanceBase::IsInSafe()
{
	const TPixelPosition& c_rkPPosCur=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	if (CPythonBackground::Instance().isAttrOn(c_rkPPosCur.x, c_rkPPosCur.y, CTerrainImpl::ATTRIBUTE_BANPK))
		return TRUE;

	return FALSE;
}

float CInstanceBase::CalculateDistanceSq3d(const TPixelPosition& c_rkPPosDst)
{
	const TPixelPosition& c_rkPPosSrc=m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	return SPixelPosition_CalculateDistanceSq3d(c_rkPPosSrc, c_rkPPosDst);
}

void CInstanceBase::OnSelected()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachSelectEffect();
}

void CInstanceBase::OnUnselected()
{
	__DetachSelectEffect();
}

void CInstanceBase::OnTargeted()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachTargetEffect();
}

void CInstanceBase::OnUntargeted()
{
	__DetachTargetEffect();	
}

void CInstanceBase::DestroySystem()
{
	ms_kPool.Clear();
}

void CInstanceBase::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);

	memset(ms_adwCRCAffectEffect, 0, sizeof(ms_adwCRCAffectEffect));

	ms_fDustGap=250.0f;
	ms_fHorseDustGap=500.0f;
}

CInstanceBase* CInstanceBase::New()
{
	return ms_kPool.Construct();
}

void CInstanceBase::Delete(CInstanceBase* pkInst)
{
	pkInst->Destroy();
	ms_kPool.Destruct(pkInst);
}

void CInstanceBase::SetMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();

	DWORD dwVID=GetVirtualID();
	rkChrMgr.SetMainInstance(dwVID);

	m_GraphicThingInstance.SetMainInstance();
}

CInstanceBase* CInstanceBase::__GetMainInstancePtr()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetMainInstancePtr();
}

void CInstanceBase::__ClearMainInstance()
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	rkChrMgr.ClearMainInstance();
}

/* 실제 플레이어 캐릭터인지 조사.*/
bool CInstanceBase::__IsMainInstance()
{
	if (this==__GetMainInstancePtr())
		return true;

	return false;
}

bool CInstanceBase::__IsExistMainInstance()
{
	if(__GetMainInstancePtr())
		return true;
	else
		return false;
}

bool CInstanceBase::__MainCanSeeHiddenThing()
{
	return false;
//	CInstanceBase * pInstance = __GetMainInstancePtr();
//	return pInstance->IsAffect(AFFECT_GAMJI);
}

float CInstanceBase::__GetBowRange()
{
	float fRange = 2500.0f - 100.0f;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		fRange += float(rPlayer.GetStatus(POINT_BOW_DISTANCE));
	}

	return fRange;
}

CInstanceBase* CInstanceBase::__FindInstancePtr(DWORD dwVID)
{
	CPythonCharacterManager& rkChrMgr=CPythonCharacterManager::Instance();
	return rkChrMgr.GetInstancePtr(dwVID);
}

bool CInstanceBase::__FindRaceType(DWORD dwRace, BYTE* pbType)
{
	CPythonNonPlayer& rkNonPlayer=CPythonNonPlayer::Instance();
	return rkNonPlayer.GetInstanceType(dwRace, pbType);
}

void CInstanceBase::SetBodyColor (DWORD dwRace, DWORD dwBodyColor)
{
	SetModulateRenderMode();
	SetAddColor (dwBodyColor);
}

bool CInstanceBase::Create(const SCreateData& c_rkCreateData)
{
	IAbstractApplication::GetSingleton().SkipRenderBuffering(300);
	SetInstanceType(c_rkCreateData.m_bType);

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (!SetRace(c_rkCreateData.m_dwRace))
		return false;

	if(c_rkCreateData.m_dwRace == 34041 || c_rkCreateData.m_dwRace == 34045 || c_rkCreateData.m_dwRace == 34049 || c_rkCreateData.m_dwRace == 34053 || c_rkCreateData.m_dwRace == 34057 || c_rkCreateData.m_dwRace == 34061 || c_rkCreateData.m_dwRace == 34200 || c_rkCreateData.m_dwRace == 34202 || c_rkCreateData.m_dwRace == 31204 || c_rkCreateData.m_dwRace == 34120 || c_rkCreateData.m_dwRace == 34121 || c_rkCreateData.m_dwRace == 34122 || c_rkCreateData.m_dwRace == 34123 || c_rkCreateData.m_dwRace == 34124 || c_rkCreateData.m_dwRace == 34125 || c_rkCreateData.m_dwRace == 34138 || c_rkCreateData.m_dwRace == 34139)
	{
		if(c_rkCreateData.m_dwLevel >= 81)
			SetRace(c_rkCreateData.m_dwRace+1);
	}
#else
	if (!SetRace(c_rkCreateData.m_dwRace))
		return false;
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	int i=0;
	if(c_rkCreateData.m_dwRace == 34077)
	{
		SetInstanceType(CActorInstance::TYPE_PC);
		SetRace(3);
		i=1;
	}
#endif

	SetVirtualID(c_rkCreateData.m_dwVID);

	if (c_rkCreateData.m_isMain)
		SetMainInstance();

	if (IsGuildWall())
	{
		unsigned center_x;
		unsigned center_y;

		c_rkCreateData.m_kAffectFlags.ConvertToPosition(&center_x, &center_y);
		
		float center_z = __GetBackgroundHeight(center_x, center_y);
		NEW_SetPixelPosition(TPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY), center_z));
	}
	else
	{
		SCRIPT_SetPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY));
	}	

	if (0 != c_rkCreateData.m_dwMountVnum)
		MountHorse(c_rkCreateData.m_dwMountVnum);

	SetArmor(c_rkCreateData.m_dwArmor);

#ifdef ENABLE_BUFFI_SYSTEM
	if (IsPC() || i == 1 )
#else
	if (IsPC())
#endif
	{
		SetHair(c_rkCreateData.m_dwHair);
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
		SetWeapon(c_rkCreateData.m_dwWeapon, c_rkCreateData.m_dwWeaponRareLv);
#else
		SetWeapon(c_rkCreateData.m_dwWeapon);
#endif
#ifdef ENABLE_QUIVER_SYSTEM
		SetArrow(c_rkCreateData.m_dwArrow);
#endif
#ifdef ENABLE_ACCE_SYSTEM
		SetSash(c_rkCreateData.m_dwSash);
#endif
#ifdef ENABLE_AURA_SYSTEM
		SetAura(c_rkCreateData.m_dwAura);
#endif
	}
	
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	if (IsPC())
	{
		SetCombatZoneRank(c_rkCreateData.combat_zone_rank);
		SetCombatZonePoints(c_rkCreateData.combat_zone_points);
	}
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	if (i==1)
		SetSupportShaman(c_rkCreateData.is_support_shaman);
#endif

	__Create_SetName(c_rkCreateData);
	

	m_dwLevel = c_rkCreateData.m_dwLevel;
#if defined(WJ_SHOW_MOB_INFO)
	m_dwAIFlag = c_rkCreateData.m_dwAIFlag;
#endif
	m_dwGuildID = c_rkCreateData.m_dwGuildID;
	m_dwEmpireID = c_rkCreateData.m_dwEmpireID;
		
#ifdef ENABLE_GUILD_LEADER_SYSTEM
	m_bMemberType = c_rkCreateData.m_bMemberType;
#endif

	SetVirtualNumber(c_rkCreateData.m_dwRace);
	SetRotation(c_rkCreateData.m_fRot);
#ifdef ENABLE_GROWTH_PET_SYSTEM
	SetLevelText(c_rkCreateData.m_dwLevel);
#endif
	SetAlignment(c_rkCreateData.m_sAlignment);
	SetPKMode(c_rkCreateData.m_byPKMode);

	SetMoveSpeed(c_rkCreateData.m_dwMovSpd);
	SetAttackSpeed(c_rkCreateData.m_dwAtkSpd);
	
	if (!IsPC())
	{	
#ifdef ENABLE_GROWTH_PET_SYSTEM
		if (IsNewPet())
		{
			float scale = c_rkCreateData.m_dwLevel * 0.006f + 0.78f;
			if (c_rkCreateData.m_dwLevel == 120)
				scale = 2.0f;
			m_GraphicThingInstance.SetScale(scale, scale, scale, true);
		}
		else
		{
#endif
#ifdef ENABLE_MOB_SCALE_SYSTEM
			float fx,fy,fz = 1.0f;
			if (CPythonNonPlayer::Instance().GetScale(c_rkCreateData.m_dwRace, fx, fy, fz))
			{
				m_GraphicThingInstance.SetScale(fx, fy, fz, true);
			}
#endif			
#ifdef ENABLE_GROWTH_PET_SYSTEM
		}
#endif
	}
	
	if (i == 1)
	{
		m_GraphicThingInstance.SetScale(0.5, 0.5, 0.5, true);
	}

	// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
	if (!IsWearingDress())
	{
		// NOTE : 반드시 Affect 셋팅 윗쪽에 있어야 함
		m_GraphicThingInstance.SetAlphaValue(0.0f);
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 0.5f);
	}

	if (!IsGuildWall())
	{
		SetAffectFlagContainer(c_rkCreateData.m_kAffectFlags);
	}	

	// NOTE : 반드시 Affect 셋팅 후에 해야 함
	AttachTextTail();
	RefreshTextTail();

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_SPAWN) 
	{
		if (IsAffect(AFFECT_SPAWN))
			__AttachEffect(EFFECT_SPAWN_APPEAR);

		if (IsPC())
		{
			Refresh(CRaceMotionData::NAME_WAIT, true);
		}
		else
		{
			Refresh(CRaceMotionData::NAME_SPAWN, false);
		}
	}
	else
	{
		Refresh(CRaceMotionData::NAME_WAIT, true);
	}

	__AttachEmpireEffect(c_rkCreateData.m_dwEmpireID);

	RegisterBoundingSphere();
	
#ifdef ENABLE_BOSS_EFFECT_SYSTEM
		if (m_dwRace == 191 || m_dwRace == 192 || m_dwRace == 193 || m_dwRace == 194 || m_dwRace == 491 || m_dwRace == 492 || m_dwRace == 493 || m_dwRace == 494 || m_dwRace ==  531 || m_dwRace == 532 || m_dwRace == 533 || m_dwRace == 534 || m_dwRace == 591 || m_dwRace == 691 || m_dwRace == 791 || m_dwRace == 792 || m_dwRace == 1091 || m_dwRace == 1092 || m_dwRace == 1093 || m_dwRace == 1192 || m_dwRace == 1304 || m_dwRace == 1901 || m_dwRace == 2091 || m_dwRace == 2206 || m_dwRace == 2307 || m_dwRace == 2306 || m_dwRace == 2491 || m_dwRace == 2492 || m_dwRace == 2494 || m_dwRace == 2495 || m_dwRace == 2597 || m_dwRace == 2598 || m_dwRace == 3190 || m_dwRace == 3191 || m_dwRace == 3290 || m_dwRace == 3291 || m_dwRace == 3390 || m_dwRace == 3391 || m_dwRace == 3490 || m_dwRace == 3491 || m_dwRace == 3590 || m_dwRace == 3591 || m_dwRace == 3690 || m_dwRace == 3691 || m_dwRace == 3790 || m_dwRace == 3791 || m_dwRace == 3890 || m_dwRace == 3891 || m_dwRace == 6390 || m_dwRace == 6391 || m_dwRace == 3090 || m_dwRace == 3091 || m_dwRace == 2191 || m_dwRace == 6421 || m_dwRace == 4204 || m_dwRace == 4209 || m_dwRace == 4210 || m_dwRace == 3596 || m_dwRace == 1095 || m_dwRace == 1191)
			__AttachEffectBoss();
#endif


	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_DEAD)
		m_GraphicThingInstance.DieEnd();

	SetStateFlags(c_rkCreateData.m_dwStateFlags);

	m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);

	if (!IsPC())
	{
		DWORD dwBodyColor = CPythonNonPlayer::Instance().GetMonsterColor(c_rkCreateData.m_dwRace);
		if (0 != dwBodyColor)
		{
			SetModulateRenderMode();
			SetAddColor(dwBodyColor);
		}
	}

	__AttachHorseSaddle();

	// 길드 심볼을 위한 임시 코드, 적정 위치를 찾는 중
	const int c_iGuildSymbolRace = 14200;
	if (c_iGuildSymbolRace == GetRace())
	{
		std::string strFileName = GetGuildSymbolFileName(m_dwGuildID);
		if (IsFile(strFileName.c_str()))
			m_GraphicThingInstance.ChangeMaterial(strFileName.c_str());
	}

	return true;
}

void CInstanceBase::__Create_SetName(const SCreateData& c_rkCreateData)
{
	if (IsGoto())
	{
		SetNameString("", 0);
		return;
	}
	if (IsWarp())
	{
		__Create_SetWarpName(c_rkCreateData);
		return;
	}

	SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
}

void CInstanceBase::__Create_SetWarpName(const SCreateData& c_rkCreateData)
{
	const char * c_szName;
	if (CPythonNonPlayer::Instance().GetName(c_rkCreateData.m_dwRace, &c_szName))
	{
		std::string strName = c_szName;
		int iFindingPos = strName.find_first_of(" ", 0);
		if (iFindingPos > 0)
		{
			strName.resize(iFindingPos);
		}
		SetNameString(strName.c_str(), strName.length());
	}
	else
	{
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
	}
}

void CInstanceBase::SetNameString(const char* c_szName, int len)
{
	m_stName.assign(c_szName, len);
}


bool CInstanceBase::SetRace(DWORD eRace)
{
	m_dwRace = eRace;

	if (!m_GraphicThingInstance.SetRace(eRace))
		return false;

	if (!__FindRaceType(m_dwRace, &m_eRaceType))
		m_eRaceType=CActorInstance::TYPE_PC;

	return true;
}

BOOL CInstanceBase::__IsChangableWeapon(int iWeaponID)
{	
	// 드레스 입고 있을때는 부케외의 장비는 나오지 않게..
	if (IsWearingDress())
	{
		const int c_iBouquets[] =
		{
			50201,	// Bouquet for Assassin
			50202,	// Bouquet for Shaman
			50203,
			50204,
			0, // #0000545: [M2CN] 웨딩 드레스와 장비 착용 문제
		};

		for (int i = 0; c_iBouquets[i] != 0; ++i)
			if (iWeaponID == c_iBouquets[i])
				return true;

		return false;
	}
	else
		return true;
}

BOOL CInstanceBase::IsWearingDress()
{
	const int c_iWeddingDressShape = 201;
	return c_iWeddingDressShape == m_eShape;
}

BOOL CInstanceBase::IsHoldingPickAxe()
{
	const int c_iPickAxeStart = 29101;
	const int c_iPickAxeEnd = 29110;
	return m_awPart[CRaceData::PART_WEAPON] >= c_iPickAxeStart && m_awPart[CRaceData::PART_WEAPON] <= c_iPickAxeEnd;
}

BOOL CInstanceBase::IsNewMount()
{
	return m_kHorse.IsNewMount();
}

BOOL CInstanceBase::IsMountingHorse()
{
	return m_kHorse.IsMounting();
}

void CInstanceBase::MountHorse(UINT eRace)
{
	m_kHorse.Destroy();
	m_kHorse.Create(m_GraphicThingInstance.NEW_GetCurPixelPositionRef(), eRace, ms_adwCRCAffectEffect[EFFECT_HIT]);

	SetMotionMode(CRaceMotionData::MODE_HORSE);	
	SetRotationSpeed(c_fDefaultHorseRotationSpeed);

	m_GraphicThingInstance.MountHorse(m_kHorse.GetActorPtr());
	m_GraphicThingInstance.Stop();
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::DismountHorse()
{
	m_kHorse.Destroy();
}

void CInstanceBase::GetInfo(std::string* pstInfo)
{
	char szInfo[256];
	sprintf(szInfo, "Inst - UC %d, RC %d Pool - %d ", 
		ms_dwUpdateCounter, 
		ms_dwRenderCounter,
		ms_kPool.GetCapacity()
	);

	pstInfo->append(szInfo);
}

void CInstanceBase::ResetPerformanceCounter()
{
	ms_dwUpdateCounter=0;
	ms_dwRenderCounter=0;
	ms_dwDeformCounter=0;
}

bool CInstanceBase::NEW_IsLastPixelPosition()
{
	return m_GraphicThingInstance.IsPushing();
}

const TPixelPosition& CInstanceBase::NEW_GetLastPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetLastPixelPositionRef();
}

void CInstanceBase::NEW_SetDstPixelPositionZ(FLOAT z)
{
	m_GraphicThingInstance.NEW_SetDstPixelPositionZ(z);
}

void CInstanceBase::NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	m_GraphicThingInstance.NEW_SetDstPixelPosition(c_rkPPosDst);
}

void CInstanceBase::NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosSrc)
{
	m_GraphicThingInstance.NEW_SetSrcPixelPosition(c_rkPPosSrc);
}

const TPixelPosition& CInstanceBase::NEW_GetCurPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetCurPixelPositionRef();	
}

const TPixelPosition& CInstanceBase::NEW_GetDstPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetDstPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetSrcPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetSrcPixelPositionRef();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void CInstanceBase::OnSyncing()
{
	m_GraphicThingInstance.__OnSyncing();
}

void CInstanceBase::OnWaiting()
{
	m_GraphicThingInstance.__OnWaiting();
}

void CInstanceBase::OnMoving()
{
	m_GraphicThingInstance.__OnMoving();
}

void CInstanceBase::ChangeGuild(DWORD dwGuildID)
{
	m_dwGuildID = dwGuildID;

	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}

DWORD CInstanceBase::GetPart(CRaceData::EParts part)
{
	assert(part >= 0 && part < CRaceData::PART_MAX_NUM);
	return m_awPart[part];
}

DWORD CInstanceBase::GetShape()
{
	return m_eShape;
}

bool CInstanceBase::CanAct()
{
	return m_GraphicThingInstance.CanAct();
}

bool CInstanceBase::CanMove()
{
	return m_GraphicThingInstance.CanMove();
}

bool CInstanceBase::CanUseSkill()
{
	if (IsPoly())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	if (!m_kHorse.CanUseSkill())
		return false;

	if (!m_GraphicThingInstance.CanUseSkill())
		return false;

	return true;
}

bool CInstanceBase::CanAttack()
{
	if (!m_kHorse.CanAttack())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;
	
	return m_GraphicThingInstance.CanAttack();
}



bool CInstanceBase::CanFishing()
{
	return m_GraphicThingInstance.CanFishing();
}


BOOL CInstanceBase::IsBowMode()
{
	return m_GraphicThingInstance.IsBowMode();
}

BOOL CInstanceBase::IsHandMode()
{
	return m_GraphicThingInstance.IsHandMode();
}

BOOL CInstanceBase::IsFishingMode()
{
	if (CRaceMotionData::MODE_FISHING == m_GraphicThingInstance.GetMotionMode())
		return true;

	return false;
}

BOOL CInstanceBase::IsFishing()
{
	return m_GraphicThingInstance.IsFishing();
}

BOOL CInstanceBase::IsDead()
{
	return m_GraphicThingInstance.IsDead();
}

BOOL CInstanceBase::IsStun()
{
	return m_GraphicThingInstance.IsStun();
}

BOOL CInstanceBase::IsSleep()
{
	return m_GraphicThingInstance.IsSleep();
}


BOOL CInstanceBase::__IsSyncing()
{
	return m_GraphicThingInstance.__IsSyncing();
}



void CInstanceBase::NEW_SetOwner(DWORD dwVIDOwner)
{
	m_GraphicThingInstance.SetOwner(dwVIDOwner);
}

float CInstanceBase::GetLocalTime()
{
	return m_GraphicThingInstance.GetLocalTime();
}


void CInstanceBase::PushUDPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
}

DWORD	ELTimer_GetServerFrameMSec();

void CInstanceBase::PushTCPStateExpanded(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg, UINT uTargetVID)
{
	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+100;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	kCmdNew.m_uTargetVID = uTargetVID;
	m_kQue_kCmdNew.push_back(kCmdNew);
}

void CInstanceBase::PushTCPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{	
	if (__IsMainInstance())
	{
		TraceError ("CInstanceBase::PushTCPState You can't send move packets to yourself!");
		return;
	}

	int nNetworkGap=ELTimer_GetServerFrameMSec()-dwCmdTime;
	
	m_nAverageNetworkGap=(m_nAverageNetworkGap*70+nNetworkGap*30)/100;
	
	/*
	if (m_dwBaseCmdTime == 0)
	{
		m_dwBaseChkTime = ELTimer_GetFrameMSec()-nNetworkGap;
		m_dwBaseCmdTime = dwCmdTime;

		Tracenf("VID[%d] 네트웍갭 [%d]", GetVirtualID(), nNetworkGap);
	}
	*/

	//m_dwBaseChkTime-m_dwBaseCmdTime+ELTimer_GetServerMSec();

	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime+m_nAverageNetworkGap;//m_dwBaseChkTime + (dwCmdTime - m_dwBaseCmdTime);// + nNetworkGap;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	m_kQue_kCmdNew.push_back(kCmdNew);

	//int nApplyGap=kCmdNew.m_dwChkTime-ELTimer_GetServerFrameMSec();

	//if (nApplyGap<-500 || nApplyGap>500)
	//	Tracenf("VID[%d] NAME[%s] 네트웍갭 [cur:%d ave:%d] 작동시간 (%d)", GetVirtualID(), GetNameString(), nNetworkGap, m_nAverageNetworkGap, nApplyGap);
}

/*
CInstanceBase::TStateQueue::iterator CInstanceBase::FindSameState(TStateQueue& rkQuekStt, DWORD dwCmdTime, UINT eFunc, UINT uArg)
{
	TStateQueue::iterator i=rkQuekStt.begin();
	while (rkQuekStt.end()!=i)
	{
		SState& rkSttEach=*i;
		if (rkSttEach.m_dwCmdTime==dwCmdTime)
			if (rkSttEach.m_eFunc==eFunc)
				if (rkSttEach.m_uArg==uArg)
					break;
		++i;
	}

	return i;
}
*/

BOOL CInstanceBase::__CanProcessNetworkStatePacket()
{
	if (m_GraphicThingInstance.IsDead())
		return FALSE;
	if (m_GraphicThingInstance.IsKnockDown())
		return FALSE;
	if (m_GraphicThingInstance.IsUsingSkill())
		if (!m_GraphicThingInstance.CanCancelSkill())
			return FALSE;

	return TRUE;
}

BOOL CInstanceBase::__IsEnableTCPProcess(UINT eCurFunc)
{
	if (m_GraphicThingInstance.IsActEmotion())
	{
		return FALSE;
	}

	if (!m_bEnableTCPState)
	{
		if (FUNC_EMOTION != eCurFunc)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CInstanceBase::StateProcess()
{	
	while (1)
	{
		if (m_kQue_kCmdNew.empty())
			return;	

		DWORD dwDstChkTime = m_kQue_kCmdNew.front().m_dwChkTime;
		DWORD dwCurChkTime = ELTimer_GetServerFrameMSec();	

		if (dwCurChkTime < dwDstChkTime)
			return;

		SCommand kCmdTop = m_kQue_kCmdNew.front();
		m_kQue_kCmdNew.pop_front();	

		TPixelPosition kPPosDst = kCmdTop.m_kPPosDst;
		//DWORD dwCmdTime = kCmdTop.m_dwCmdTime;	
		FLOAT fRotDst = kCmdTop.m_fDstRot;
		UINT eFunc = kCmdTop.m_eFunc;
		UINT uArg = kCmdTop.m_uArg;
		UINT uVID = GetVirtualID();	
		UINT uTargetVID = kCmdTop.m_uTargetVID;

		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		/*
		if (IsPC())
			Tracenf("%d cmd: vid=%d[%s] func=%d arg=%d  curPos=(%f, %f) dstPos=(%f, %f) rot=%f (time %d)", 
			ELTimer_GetMSec(),
			uVID, m_stName.c_str(), eFunc, uArg, 
			kPPosCur.x, kPPosCur.y,
			kPPosDst.x, kPPosDst.y, fRotDst, dwCmdTime-m_dwBaseCmdTime);
		*/

		TPixelPosition kPPosDir = kPPosDst - kPPosCur;
		float fDirLen = (float)sqrt(kPPosDir.x * kPPosDir.x + kPPosDir.y * kPPosDir.y);

		//Tracenf("거리 %f", fDirLen);

		if (!__CanProcessNetworkStatePacket())
		{
			Lognf(0, "vid=%d 움직일 수 없는 상태라 스킵 IsDead=%d, IsKnockDown=%d", uVID, m_GraphicThingInstance.IsDead(), m_GraphicThingInstance.IsKnockDown());
			return;
		}

		if (!__IsEnableTCPProcess(eFunc))
		{
			return;
		}

		switch (eFunc)
		{
			case FUNC_WAIT:
			{
				//Tracenf("%s (%f, %f) -> (%f, %f) 남은거리 %f", GetNameString(), kPPosCur.x, kPPosCur.y, kPPosDst.x, kPPosDst.y, fDirLen);
				if (fDirLen > 1.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);

					__EnableSkipCollision();

					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_WAIT;

					if (!IsWalking())
						StartWalking();

					//Tracen("목표정지");
				}
				else
				{
					//Tracen("현재 정지");

					m_isGoing = FALSE;

					if (!IsWaiting())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					SetAdvancingRotation(fRotDst);
					SetRotation(fRotDst);
				}
				break;
			}

			case FUNC_MOVE:
			{
				//NEW_GetSrcPixelPositionRef() = kPPosCur;
				//NEW_GetDstPixelPositionRef() = kPPosDst;
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();
				//m_isSyncMov = TRUE;

				m_kMovAfterFunc.eFunc = FUNC_MOVE;

				if (!IsWalking())
				{
					//Tracen("걷고 있지 않아 걷기 시작");
					StartWalking();
				}
				else
				{
					//Tracen("이미 걷는중 ");
				}
				break;
			}

			case FUNC_COMBO:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot=fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_COMBO;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					//Tracen("대기 공격 정지");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					RunComboAttack(fRotDst, uArg);
				}
				break;
			}

			case FUNC_ATTACK:
			{
				if (fDirLen>=50.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();
					//m_isSyncMov = TRUE;

					m_kMovAfterFunc.eFunc = FUNC_ATTACK;

					if (!IsWalking())
						StartWalking();

					//Tracen("너무 멀어서 이동 후 공격");
				}
				else
				{
					//Tracen("노말 공격 정지");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					RunNormalAttack(fRotDst);

					//Tracen("가깝기 때문에 워프 공격");
				}
				break;
			}

			case FUNC_MOB_SKILL:
			{
				if (fDirLen >= 50.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_MOB_SKILL;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					BlendRotation(fRotDst);

					m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + uArg);
				}
				break;
			}

			case FUNC_EMOTION:
			{
				if (fDirLen>100.0f)
				{
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;

					if (__IsMainInstance())
						__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = FUNC_EMOTION;
					m_kMovAfterFunc.uArg = uArg;
					m_kMovAfterFunc.uArgExpanded = uTargetVID;
					m_kMovAfterFunc.kPosDst = kPPosDst;

					if (!IsWalking())
						StartWalking();
				}
				else
				{
					__ProcessFunctionEmotion(uArg, uTargetVID, kPPosDst);
				}
				break;
			}

			default:
			{
				if (eFunc & FUNC_SKILL)
				{
					if (fDirLen >= 50.0f)
					{
						//NEW_GetSrcPixelPositionRef() = kPPosCur;
						//NEW_GetDstPixelPositionRef() = kPPosDst;
						NEW_SetSrcPixelPosition(kPPosCur);
						NEW_SetDstPixelPosition(kPPosDst);
						m_fDstRot = fRotDst;
						m_isGoing = TRUE;
						//m_isSyncMov = TRUE;
						__EnableSkipCollision();

						m_kMovAfterFunc.eFunc = eFunc;
						m_kMovAfterFunc.uArg = uArg;

						if (!IsWalking())
							StartWalking();

						//Tracen("너무 멀어서 이동 후 공격");
					}
					else
					{
						//Tracen("스킬 정지");

						m_isGoing = FALSE;

						if (IsWalking())
							EndWalking();

						SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
						SetAdvancingRotation(fRotDst);
						SetRotation(fRotDst);

						NEW_UseSkill(0, eFunc & 0x7f, uArg&0x0f, (uArg>>4) ? true : false);
						//Tracen("가깝기 때문에 워프 공격");
					}
				}
				break;
			}
		}
	}
}


void CInstanceBase::MovementProcess()
{
	TPixelPosition kPPosCur;
	NEW_GetPixelPosition(&kPPosCur);

	// 렌더링 좌표계이므로 y를 -화해서 더한다.

	TPixelPosition kPPosNext;
	{
		const D3DXVECTOR3 & c_rkV3Mov = m_GraphicThingInstance.GetMovementVectorRef();

		kPPosNext.x = kPPosCur.x + (+c_rkV3Mov.x);
		kPPosNext.y = kPPosCur.y + (-c_rkV3Mov.y);
		kPPosNext.z = kPPosCur.z + (+c_rkV3Mov.z);
	}

	TPixelPosition kPPosDeltaSC = kPPosCur - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSN = kPPosNext - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSD = NEW_GetDstPixelPositionRef() - NEW_GetSrcPixelPositionRef();

	float fCurLen = sqrtf(kPPosDeltaSC.x * kPPosDeltaSC.x + kPPosDeltaSC.y * kPPosDeltaSC.y);
	float fNextLen = sqrtf(kPPosDeltaSN.x * kPPosDeltaSN.x + kPPosDeltaSN.y * kPPosDeltaSN.y);
	float fTotalLen = sqrtf(kPPosDeltaSD.x * kPPosDeltaSD.x + kPPosDeltaSD.y * kPPosDeltaSD.y);
	float fRestLen = fTotalLen - fCurLen;

	if (__IsMainInstance())
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			if (fRestLen<=0.0)
			{
				if (IsWalking())
					EndWalking();

				//Tracen("목표 도달 정지");

				m_isGoing = FALSE;

				BlockMovement();

				if (FUNC_EMOTION == m_kMovAfterFunc.eFunc)
				{
					DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
					DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
					__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
					return;
				}
			}
		}
	}
	else
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			// 만약 렌턴시가 늦어 너무 많이 이동했다면..
			if (fRestLen < -100.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);

				float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, NEW_GetDstPixelPositionRef());
				SetAdvancingRotation(fDstRot);
				//Tracenf("VID %d 오버 방향설정 (%f, %f) %f rest %f", GetVirtualID(), kPPosCur.x, kPPosCur.y, fDstRot, fRestLen);			

				// 이동중이라면 다음번에 멈추게 한다
				if (FUNC_MOVE == m_kMovAfterFunc.eFunc)
				{
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
				}
			}
			// 도착했다면...
			else if (fCurLen <= fTotalLen && fTotalLen <= fNextLen)
			{
				if (m_GraphicThingInstance.IsDead() || m_GraphicThingInstance.IsKnockDown())
				{
					__DisableSkipCollision();

					//Tracen("사망 상태라 동작 스킵");

					m_isGoing = FALSE;

					//Tracen("행동 불능 상태라 이후 동작 스킵");
				}
				else
				{
					switch (m_kMovAfterFunc.eFunc)
					{
						case FUNC_ATTACK:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							RunNormalAttack(m_fDstRot);
							break;
						}

						case FUNC_COMBO:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							RunComboAttack(m_fDstRot, m_kMovAfterFunc.uArg);
							break;
						}

						case FUNC_EMOTION:
						{
							m_isGoing = FALSE;
							m_kMovAfterFunc.eFunc = FUNC_WAIT;
							__DisableSkipCollision();
							BlockMovement();

							DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
							DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
							__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
							break;
						}

						case FUNC_MOVE:
						{
							break;
						}

						case FUNC_MOB_SKILL:
						{
							if (IsWalking())
								EndWalking();

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							SetRotation(m_fDstRot);

							m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + m_kMovAfterFunc.uArg);
							break;
						}

						default:
						{
							if (m_kMovAfterFunc.eFunc & FUNC_SKILL)
							{
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								NEW_UseSkill(0, m_kMovAfterFunc.eFunc & 0x7f, m_kMovAfterFunc.uArg&0x0f, (m_kMovAfterFunc.uArg>>4) ? true : false);
							}
							else
							{
								//Tracenf("VID %d 스킬 공격 (%f, %f) rot %f", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot);

								__DisableSkipCollision();
								m_isGoing = FALSE;

								BlockMovement();
								SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
								SetAdvancingRotation(m_fDstRot);
								BlendRotation(m_fDstRot);
								if (!IsWaiting())
								{
									EndWalking();
								}

								//Tracenf("VID %d 정지 (%f, %f) rot %f IsWalking %d", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot, IsWalking());
							}
							break;
						}
					}

				}
			}

		}
	}

	if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
	{
		float fRotation = m_GraphicThingInstance.GetRotation();
		float fAdvancingRotation = m_GraphicThingInstance.GetAdvancingRotation();
		int iDirection = GetRotatingDirection(fRotation, fAdvancingRotation);

		if (DEGREE_DIRECTION_SAME != m_iRotatingDirection)
		{
			if (DEGREE_DIRECTION_LEFT == iDirection)
			{
				fRotation = fmodf(fRotation + m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed(), 360.0f);
			}
			else if (DEGREE_DIRECTION_RIGHT == iDirection)
			{
				fRotation = fmodf(fRotation - m_fRotSpd*m_GraphicThingInstance.GetSecondElapsed() + 360.0f, 360.0f);
			}

			if (m_iRotatingDirection != GetRotatingDirection(fRotation, fAdvancingRotation))
			{
				m_iRotatingDirection = DEGREE_DIRECTION_SAME;
				fRotation = fAdvancingRotation;
			}

			m_GraphicThingInstance.SetRotation(fRotation);
		}

		if (__IsInDustRange())
		{ 
			float fDustDistance = NEW_GetDistanceFromDestPixelPosition(m_kPPosDust);
			if (IsMountingHorse())
			{
				if (fDustDistance > ms_fHorseDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_HORSE_DUST);
				}
			}
			else
			{
				if (fDustDistance > ms_fDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_DUST);
				}
			}
		}
	}
}

void CInstanceBase::__ProcessFunctionEmotion(DWORD dwMotionNumber, DWORD dwTargetVID, const TPixelPosition & c_rkPosDst)
{
	if (IsWalking())
		EndWalkingWithoutBlending();

	__EnableChangingTCPState();
	SCRIPT_SetPixelPosition(c_rkPosDst.x, c_rkPosDst.y);

	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwTargetVID);
	if (pTargetInstance)
	{
		pTargetInstance->__EnableChangingTCPState();

		if (pTargetInstance->IsWalking())
			pTargetInstance->EndWalkingWithoutBlending();

		WORD wMotionNumber1 = HIWORD(dwMotionNumber);
		WORD wMotionNumber2 = LOWORD(dwMotionNumber);

		int src_job = RaceToJob(GetRace());
		int dst_job = RaceToJob(pTargetInstance->GetRace());

		NEW_LookAtDestInstance(*pTargetInstance);
		m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber1 + dst_job);
		m_GraphicThingInstance.SetRotation(m_GraphicThingInstance.GetTargetRotation());
		m_GraphicThingInstance.SetAdvancingRotation(m_GraphicThingInstance.GetTargetRotation());

		pTargetInstance->NEW_LookAtDestInstance(*this);
		pTargetInstance->m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber2 + src_job);
		pTargetInstance->m_GraphicThingInstance.SetRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());
		pTargetInstance->m_GraphicThingInstance.SetAdvancingRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());

		if (pTargetInstance->__IsMainInstance())
		{
			IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
			rPlayer.EndEmotionProcess();
		}
	}

	if (__IsMainInstance())
	{
		IAbstractPlayer & rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.EndEmotionProcess();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Update & Deform & Render


void CInstanceBase::Update()
{
	++ms_dwUpdateCounter;	

	StateProcess();
	m_GraphicThingInstance.PhysicsProcess();
	m_GraphicThingInstance.RotationProcess();
	m_GraphicThingInstance.ComboProcess();
	m_GraphicThingInstance.AccumulationMovement();

	if (m_GraphicThingInstance.IsMovement())
	{
		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		DWORD dwCurTime=ELTimer_GetFrameMSec();
		//if (m_dwNextUpdateHeightTime<dwCurTime)
		{
			m_dwNextUpdateHeightTime=dwCurTime;
			kPPosCur.z = __GetBackgroundHeight(kPPosCur.x, kPPosCur.y);
			NEW_SetPixelPosition(kPPosCur);
		}

		// SetMaterialColor
		{
			DWORD dwMtrlColor=__GetShadowMapColor(kPPosCur.x, kPPosCur.y);
			m_GraphicThingInstance.SetMaterialColor(dwMtrlColor);
		}
	}

	m_GraphicThingInstance.UpdateAdvancingPointInstance();
	
	if (IsPet())
	{
		if (CPythonSystem::instance().IsHidePets())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}

	if (IsMount())
	{
		if (CPythonSystem::instance().IsHideMounts())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}
	
	if (IsSupportShaman())
	{
		if (CPythonSystem::instance().IsHideShamans())
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}			
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}			
		}
	}
	
	if (IsOfflineShop())
	{	
		if (CPythonSystem::Instance().GetShowOfflineShopFlag() == false)
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}						
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}				
		}
	}
	
	AttackProcess();
	MovementProcess();

	m_GraphicThingInstance.MotionProcess(IsPC());
	if (IsMountingHorse())
	{
		m_kHorse.m_pkActor->HORSE_MotionProcess(FALSE);
	}

	__ComboProcess();	
	
	ProcessDamage();

}

void CInstanceBase::Transform()
{
	if (__IsSyncing())
	{
		//OnSyncing();
	}
	else
	{
		if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
		{
			const D3DXVECTOR3& c_rv3Movment=m_GraphicThingInstance.GetMovementVectorRef();

			float len=(c_rv3Movment.x*c_rv3Movment.x)+(c_rv3Movment.y*c_rv3Movment.y);
			if (len>1.0f)
				OnMoving();
			else
				OnWaiting();	
		}	
	}

	m_GraphicThingInstance.INSTANCEBASE_Transform();
}


void CInstanceBase::Deform()
{
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (!__CanRender())
		return;

	++ms_dwDeformCounter;

	m_GraphicThingInstance.INSTANCEBASE_Deform();

	m_kHorse.Deform();
}

void CInstanceBase::RenderTrace()
{
	if (!__CanRender())
		return;

	m_GraphicThingInstance.RenderTrace();
}




void CInstanceBase::Render()
{
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (!__CanRender())
		return;

	++ms_dwRenderCounter;

	m_kHorse.Render();
	m_GraphicThingInstance.Render();	
	
	if (CActorInstance::IsDirLine())
	{	
		if (NEW_GetDstPixelPositionRef().x != 0.0f)
		{
			static CScreen s_kScreen;

			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_DIFFUSE);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);	
			STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);
			
			TPixelPosition px;
			m_GraphicThingInstance.GetPixelPosition(&px);
			D3DXVECTOR3 kD3DVt3Cur(px.x, px.y, px.z);
			//D3DXVECTOR3 kD3DVt3Cur(NEW_GetSrcPixelPositionRef().x, -NEW_GetSrcPixelPositionRef().y, NEW_GetSrcPixelPositionRef().z);
			D3DXVECTOR3 kD3DVt3Dest(NEW_GetDstPixelPositionRef().x, -NEW_GetDstPixelPositionRef().y, NEW_GetDstPixelPositionRef().z);

			//printf("%s %f\n", GetNameString(), kD3DVt3Cur.y - kD3DVt3Dest.y);
			//float fdx = NEW_GetDstPixelPositionRef().x - NEW_GetSrcPixelPositionRef().x;
			//float fdy = NEW_GetDstPixelPositionRef().y - NEW_GetSrcPixelPositionRef().y;

			s_kScreen.SetDiffuseColor(0.0f, 0.0f, 1.0f);
			s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, px.z, kD3DVt3Dest.x, kD3DVt3Dest.y, px.z);
			STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, TRUE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}	
}

void CInstanceBase::RenderToShadowMap()
{
	if (IsDoor())
		return;

	if (IsBuilding())
		return;

	if (!__CanRender())
		return;

	if (!__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	const float SHADOW_APPLY_DISTANCE = 2500.0f;

	float fDistance=NEW_GetDistanceFromDestInstance(*pkInstMain);
	if (fDistance>=SHADOW_APPLY_DISTANCE)
		return;

	m_GraphicThingInstance.RenderToShadowMap();	
}

void CInstanceBase::RenderCollision()
{
	m_GraphicThingInstance.RenderCollisionData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Setting & Getting Data

void CInstanceBase::SetVirtualID(DWORD dwVirtualID)
{
	m_GraphicThingInstance.SetVirtualID(dwVirtualID);		
}

void CInstanceBase::SetVirtualNumber(DWORD dwVirtualNumber)
{
	m_dwVirtualNumber = dwVirtualNumber;
}

void CInstanceBase::SetInstanceType(int iInstanceType)
{
	m_GraphicThingInstance.SetActorType(iInstanceType);
}

#ifdef ENABLE_ALIGNMENT_SYSTEM
void CInstanceBase::SetAlignment(long long sAlignment)
{
	m_sAlignment = sAlignment;
	RefreshTextTailTitle();
}
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
void CInstanceBase::SetLevelText(int sLevel)
{
	m_dwLevel = sLevel;
	UpdateTextTailLevel(sLevel);
}
#endif

void CInstanceBase::SetLevel(DWORD level)
{
	m_dwLevel = level;
	RefreshTextTail();
}

void CInstanceBase::SetPKMode(BYTE byPKMode)
{
	if (m_byPKMode == byPKMode)
		return;

	m_byPKMode = byPKMode;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		rPlayer.NotifyChangePKMode();
	}	
}

void CInstanceBase::SetKiller(bool bFlag)
{
	if (m_isKiller == bFlag)
		return;

	m_isKiller = bFlag;
	RefreshTextTail();
}

void CInstanceBase::SetPartyMemberFlag(bool bFlag)
{
	m_isPartyMember = bFlag;
}

void CInstanceBase::SetStateFlags(DWORD dwStateFlags)
{
	if (dwStateFlags & ADD_CHARACTER_STATE_KILLER)
		SetKiller(TRUE);
	else
		SetKiller(FALSE);

	if (dwStateFlags & ADD_CHARACTER_STATE_PARTY)
		SetPartyMemberFlag(TRUE);
	else
		SetPartyMemberFlag(FALSE);
}

void CInstanceBase::SetComboType(UINT uComboType)
{
	m_GraphicThingInstance.SetComboType(uComboType);
}

const char * CInstanceBase::GetNameString()
{
	return m_stName.c_str();
}

DWORD CInstanceBase::GetRace()
{
	return m_dwRace;
}

DWORD CInstanceBase::GetOriginalRace()
{
	return m_dwOriginalRace;
}

DWORD CInstanceBase::GetLevel()
{
	return m_dwLevel;
}

#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
bool CInstanceBase::IsCombatZoneMap()
{
	if (!strcmp(CPythonBackground::Instance().GetWarpMapName(), "metin2_map_battlefied"))
		return true;
	return false;
}

void CInstanceBase::SetCombatZonePoints(DWORD dwValue)
{
	combat_zone_points = dwValue;
}

DWORD CInstanceBase::GetCombatZonePoints()
{
	return combat_zone_points;
}

void CInstanceBase::SetCombatZoneRank(BYTE bValue)
{
	combat_zone_rank = bValue;
}

BYTE CInstanceBase::GetCombatZoneRank()
{
	return combat_zone_rank;
}
#endif

#ifdef ENABLE_BUFFI_SYSTEM
void CInstanceBase::SetSupportShaman(bool bTrue)
{
	is_support_shaman = bTrue;
}

bool CInstanceBase::IsSupportShaman()
{
	return is_support_shaman;
}
#endif

#ifdef ENABLE_GUILD_LEADER_SYSTEM
BYTE CInstanceBase::GetGuildMemberType()
{
	return m_bMemberType;
}
#endif

bool CInstanceBase::IsConflictAlignmentInstance(CInstanceBase& rkInstVictim)
{
	if (PK_MODE_PROTECT == rkInstVictim.GetPKMode())
		return false;

	switch (GetAlignmentType())
	{
		case ALIGNMENT_TYPE_NORMAL:
		case ALIGNMENT_TYPE_WHITE:
			if (ALIGNMENT_TYPE_DARK == rkInstVictim.GetAlignmentType())
				return true;
			break;
		case ALIGNMENT_TYPE_DARK:
			if (GetAlignmentType() != rkInstVictim.GetAlignmentType())
				return true;
			break;
	}

	return false;
}

void CInstanceBase::SetDuelMode(DWORD type)
{
	m_dwDuelMode = type;
}

DWORD CInstanceBase::GetDuelMode()
{
	return m_dwDuelMode;
}

bool CInstanceBase::IsAttackableInstance(CInstanceBase& rkInstVictim)
{	
	if (__IsMainInstance())
	{		
		CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
		if(rkPlayer.IsObserverMode())
			return false;
	}

	if (GetVirtualID() == rkInstVictim.GetVirtualID())
		return false;

	if (IsStone())
	{
		if (rkInstVictim.IsPC())
			return true;
	}
	else if (IsPC())
	{
		if (rkInstVictim.IsStone())
			return true;
		
		if (rkInstVictim.IsSupportShaman())
			return false;

		if (rkInstVictim.IsPC())
		{
			if (GetDuelMode())
			{
				switch(GetDuelMode())
				{
				case DUEL_CANNOTATTACK:
					return false;
				case DUEL_START:
					if(__FindDUELKey(GetVirtualID(),rkInstVictim.GetVirtualID()))
						return true;
					else
						return false;
				}
			}
			if (PK_MODE_GUILD == GetPKMode())
				if (GetGuildID() == rkInstVictim.GetGuildID())
					return false;
				
			if (rkInstVictim.IsKiller())
				if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
					return true;

			if (PK_MODE_PROTECT != GetPKMode())
			{
				if (PK_MODE_FREE == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							return true;
				}
				if (PK_MODE_GUILD == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							if (GetGuildID() != rkInstVictim.GetGuildID())
								return true;
				}
			}

			if (IsSameEmpire(rkInstVictim))
			{
				if (IsPVPInstance(rkInstVictim))
					return true;

				if (PK_MODE_REVENGE == GetPKMode())
					if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
						if (IsConflictAlignmentInstance(rkInstVictim))
							return true;
			}
			else
			{				
				return true;
			}
		}

		if (rkInstVictim.IsEnemy())
			return true;

		if (rkInstVictim.IsWoodenDoor())
			return true;
	}
	else if (IsEnemy())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsBuilding())
			return true;
		
		if (rkInstVictim.IsSupportShaman())
			return false;
	}
	else if (IsPoly())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsEnemy())
			return true;
		
		if (rkInstVictim.IsSupportShaman())
			return false;
	}
	return false;
}

bool CInstanceBase::IsTargetableInstance(CInstanceBase& rkInstVictim)
{
	return rkInstVictim.CanPickInstance();
}

// 2004. 07. 07. [levites] - 스킬 사용중 타겟이 바뀌는 문제 해결을 위한 코드
bool CInstanceBase::CanChangeTarget()
{
	return m_GraphicThingInstance.CanChangeTarget();
}

// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
bool CInstanceBase::CanPickInstance()
{
	if (!__IsInViewFrustum())
		return false;

	if (IsDoor())
	{
		if (IsDead())
			return false;
	}

	if (IsPC())
	{
		if (IsAffect(AFFECT_EUNHYEONG))
		{
			if (!__MainCanSeeHiddenThing())
				return false;
		}
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY))
			return false;
		if (IsAffect(AFFECT_INVISIBILITY))
			return false;
	}

	if (IsDead())
		return false;

	return true;
}

bool CInstanceBase::CanViewTargetHP(CInstanceBase& rkInstVictim)
{
	if (rkInstVictim.IsStone())
		return true;
	if (rkInstVictim.IsWoodenDoor())
		return true;
	if (rkInstVictim.IsEnemy())
		return true;
#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
	if (rkInstVictim.IsPC())
		return true;
#endif

	return false;
}

BOOL CInstanceBase::IsPoly()
{
	return m_GraphicThingInstance.IsPoly();
}

BOOL CInstanceBase::IsPC()
{
	return m_GraphicThingInstance.IsPC();
}

BOOL CInstanceBase::IsNPC()
{
	return m_GraphicThingInstance.IsNPC();
}

#ifdef ENABLE_GROWTH_PET_SYSTEM
BOOL CInstanceBase::IsNewPet()
{
	return m_GraphicThingInstance.IsNewPet();
}
#endif

BOOL CInstanceBase::IsEnemy()
{
	return m_GraphicThingInstance.IsEnemy();
}

BOOL CInstanceBase::IsStone()
{
	return m_GraphicThingInstance.IsStone();
}


BOOL CInstanceBase::IsGuildWall()	//IsBuilding 길드건물전체 IsGuildWall은 담장벽만(문은 제외)
{
	return IsWall(m_dwRace);		
}


BOOL CInstanceBase::IsResource()
{
	switch (m_dwVirtualNumber)
	{
		case 20047:
		case 20048:
		case 20049:
		case 20050:
		case 20051:
		case 20052:
		case 20053:
		case 20054:
		case 20055:
		case 20056:
		case 20057:
		case 20058:
		case 20059:
		case 30301:
		case 30302:
		case 30303:
		case 30304:
		case 30305:
			return TRUE;
	}

	return FALSE;
}

BOOL CInstanceBase::IsWarp()
{
	return m_GraphicThingInstance.IsWarp();
}

BOOL CInstanceBase::IsGoto()
{
	return m_GraphicThingInstance.IsGoto();
}

BOOL CInstanceBase::IsObject()
{
	return m_GraphicThingInstance.IsObject();
}

BOOL CInstanceBase::IsBuilding()
{
	return m_GraphicThingInstance.IsBuilding();
}

BOOL CInstanceBase::IsDoor()
{
	return m_GraphicThingInstance.IsDoor();
}

BOOL CInstanceBase::IsWoodenDoor()
{
	if (m_GraphicThingInstance.IsDoor())
	{
		int vnum = GetVirtualNumber();
		if (vnum == 13000) // 나무문
			return true;
		else if (vnum >= 30111 && vnum <= 30119) // 사귀문
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

BOOL CInstanceBase::IsStoneDoor()
{
	return m_GraphicThingInstance.IsDoor() && 13001 == GetVirtualNumber();
}

BOOL CInstanceBase::IsFlag()
{
	if (GetRace() == 20035)
		return TRUE;
	if (GetRace() == 20036)
		return TRUE;
	if (GetRace() == 20037)
		return TRUE;
	if (GetRace() == 20143)
		return TRUE;

	return FALSE;
}

BOOL CInstanceBase::IsPet()
{
	if (GetRace() >= 34001 && GetRace() <= 34999)
		return true;
	if (GetRace() >= 20523 && GetRace() <= 20529)
		return true;
	return false;
}

BOOL CInstanceBase::IsMount()
{
	if (GetRace() >= 20110 && GetRace() <= 20299)
		return true;
	if (GetRace() >= 20530 && GetRace() <= 20537)
		return true;
	return false;
}

BOOL CInstanceBase::IsOfflineShop()
{
	if (GetRace() >= 30000 && GetRace() <= 30014)
		return true;
	return false;
}

BOOL CInstanceBase::IsForceVisible()
{
	if (IsAffect(AFFECT_SHOW_ALWAYS))
		return TRUE;

	if (IsObject() || IsBuilding() || IsDoor() )
		return TRUE;

	return FALSE;
}

int	CInstanceBase::GetInstanceType()
{
	return m_GraphicThingInstance.GetActorType();
}

DWORD CInstanceBase::GetVirtualID()
{
	return m_GraphicThingInstance.GetVirtualID();
}

DWORD CInstanceBase::GetVirtualNumber()
{
	return m_dwVirtualNumber;
}

// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
bool CInstanceBase::__IsInViewFrustum()
{
	return m_GraphicThingInstance.isShow();
}

bool CInstanceBase::__CanRender()
{
	if (IsAlwaysRender())
	{
		return true;
	}
	
	if (!__IsInViewFrustum())
		return false;
	if (IsAffect(AFFECT_INVISIBILITY))
		return false;

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic Control

bool CInstanceBase::IntersectBoundingBox()
{
	float u, v, t;
	return m_GraphicThingInstance.Intersect(&u, &v, &t);
}

bool CInstanceBase::IntersectDefendingSphere()
{
	return m_GraphicThingInstance.IntersectDefendingSphere();
}

float CInstanceBase::GetDistance(CInstanceBase * pkTargetInst)
{
	TPixelPosition TargetPixelPosition;
	pkTargetInst->m_GraphicThingInstance.GetPixelPosition(&TargetPixelPosition);
	return GetDistance(TargetPixelPosition);
}

float CInstanceBase::GetDistance(const TPixelPosition & c_rPixelPosition)
{
	TPixelPosition PixelPosition;
	m_GraphicThingInstance.GetPixelPosition(&PixelPosition);

	float fdx = PixelPosition.x - c_rPixelPosition.x;
	float fdy = PixelPosition.y - c_rPixelPosition.y;

	return sqrtf((fdx*fdx) + (fdy*fdy));
}

CActorInstance& CInstanceBase::GetGraphicThingInstanceRef()
{
	return m_GraphicThingInstance;
}

CActorInstance* CInstanceBase::GetGraphicThingInstancePtr()
{
	return &m_GraphicThingInstance;
}

void CInstanceBase::RefreshActorInstance()
{
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::Refresh(DWORD dwMotIndex, bool isLoop)
{
	RefreshState(dwMotIndex, isLoop);
}

void CInstanceBase::RestoreRenderMode()
{
	m_GraphicThingInstance.RestoreRenderMode();
}

void CInstanceBase::SetAddRenderMode()
{
	m_GraphicThingInstance.SetAddRenderMode();
}

void CInstanceBase::SetModulateRenderMode()
{
	m_GraphicThingInstance.SetModulateRenderMode();
}

void CInstanceBase::SetRenderMode(int iRenderMode)
{
	m_GraphicThingInstance.SetRenderMode(iRenderMode);
}

void CInstanceBase::SetAddColor(const D3DXCOLOR & c_rColor)
{
	m_GraphicThingInstance.SetAddColor(c_rColor);
}

void CInstanceBase::__SetBlendRenderingMode()
{
	m_GraphicThingInstance.SetBlendRenderMode();
}

void CInstanceBase::__SetAlphaValue(float fAlpha)
{
	m_GraphicThingInstance.SetAlphaValue(fAlpha);
}

float CInstanceBase::__GetAlphaValue()
{
	return m_GraphicThingInstance.GetAlphaValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Part

void CInstanceBase::SetHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false)
		return;

	float fSpecularPower = 0.0f;
	/*if (CItemManager::Instance().GetItemDataPointer(__GetHairVnum(eHair), &pItemData))
		fSpecularPower=pItemData->GetSpecularPowerf();*/
	
	m_awPart[CRaceData::PART_HAIR] = eHair;
	m_GraphicThingInstance.SetHair(eHair, fSpecularPower);
}

void CInstanceBase::ChangeHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC()==false)
		return;

	if (GetPart(CRaceData::PART_HAIR)==eHair)
		return;
	
	SetHair(eHair);

	//int type = m_GraphicThingInstance.GetMotionMode();

	RefreshState(CRaceMotionData::NAME_WAIT, true);
	//RefreshState(type, true);
}

#ifdef ENABLE_EFFECT_STONE_SYSTEM
void CInstanceBase::SetArmor(DWORD dwArmor, DWORD dwStoneEffect)
#else
void CInstanceBase::SetArmor(DWORD dwArmor)
#endif
{
	DWORD dwShape;
	if (__ArmorVnumToShape(dwArmor, &dwShape))
	{
		CItemData * pItemData;
		if (CItemManager::Instance().GetItemDataPointer(dwArmor, &pItemData))
		{
			float fSpecularPower=pItemData->GetSpecularPowerf();
			SetShape(dwShape, fSpecularPower);
#ifdef ENABLE_EFFECT_STONE_SYSTEM
			__GetRefinedEffect(pItemData, m_dwWeaponRareLv, dwStoneEffect);
#else
			__GetRefinedEffect(pItemData, m_dwWeaponRareLv);
#endif
#ifdef ENABLE_SHINING_SYSTEM
			__GetShiningEffect(pItemData);
#endif
			return;
		}
		else
		{
			__ClearArmorRefineEffect();
#ifdef ENABLE_SHINING_SYSTEM
			__ClearArmorShiningEffect();
#endif
	}
		}

	SetShape(dwArmor);
}

void CInstanceBase::SetShape(DWORD eShape, float fSpecular)
{
	if (IsPoly())
	{
		m_GraphicThingInstance.SetShape(0);	
	}
	else
	{
		m_GraphicThingInstance.SetShape(eShape, fSpecular);		
	}

	m_eShape = eShape;
}

#ifdef ENABLE_AURA_SYSTEM
bool CInstanceBase::SetAura(DWORD eAura)
{
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return false;

	//m_GraphicThingInstance.ChangePart(CRaceData::PART_AURA, eAura);
	if (!eAura)
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_awPart[CRaceData::PART_AURA] = 0;
		return true;
	}

	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(eAura, &pItemData))
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_awPart[CRaceData::PART_AURA] = 0;
		return true;
	}
	
	ClearAuraEffect();

	BYTE byRace = (BYTE)GetRace();
	BYTE byJob = (BYTE)RaceToJob(byRace);
	BYTE bySex = (BYTE)RaceToSex(byRace);

	byJob += 1;
	if (bySex == 0)
		byJob += 5;

	//m_GraphicThingInstance.AttachAura(pItemData);

	float fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ;
	if (pItemData->GetItemScale(byJob, fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ))
	{
		if (m_kHorse.IsMounting() && byJob != NRaceData::JOB_WOLFMAN)
			fPositionZ += 15.0f;
		if (m_kHorse.IsManuManni())
			fPositionZ += 10.0f;

		// D3DXVECTOR3 * sendPosition;
		// sendPosition->x = fPositionX;
		// sendPosition->y = fPositionY;
		// sendPosition->z = fPositionZ;

		m_auraRefineEffect = m_GraphicThingInstance.AttachEffectByID(NULL, "Bip01 Spine2", pItemData->GetAuraEffectID(), NULL);
		m_awPart[CRaceData::PART_AURA] = eAura;
		//__EffectContainer_AttachEffect(m_auraRefineEffect);
	}
	
	return true;
}

void CInstanceBase::ChangeAura(DWORD eAura)
{
	if (!IsPC())
		return;
	
	SetAura(eAura);
}

void CInstanceBase::ClearAuraEffect()
{
	if (!m_auraRefineEffect)
		return;
	
	__DetachEffect(m_auraRefineEffect);
	m_auraRefineEffect = 0;
}
#endif

#ifdef ENABLE_ACCE_SYSTEM
void CInstanceBase::SetSash(DWORD dwSash)
{
	if (!IsPC())
		return;
	
	if (IsPoly())
		return;

	if (dwSash == 0) {
		m_awPart[CRaceData::PART_SASH] = 0;
		m_GraphicThingInstance.AttachSash(0, 0.0f);
		ClearSashEffect();
		return;
	}
	
	dwSash += 85000;
	ClearSashEffect();
	
	float fSpecular = 65.0f;
	if (dwSash > 87000)
    {
        dwSash -= 2000;
        fSpecular += 35;
        
        m_dwSashEffect = EFFECT_REFINED + EFFECT_SASH;
        __EffectContainer_AttachEffect(m_dwSashEffect);
    }
	
	//fSpecular /= 100.0f;
	m_awPart[CRaceData::PART_SASH] = dwSash;
	
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwSash, &pItemData))
		return;
	
	m_GraphicThingInstance.AttachSash(pItemData, fSpecular);
#ifdef ENABLE_OBJ_SCALLING
	DWORD dwRace = GetRace(), dwPos = RaceToJob(dwRace), dwSex = RaceToSex(dwRace);
	dwPos += 1;
	if (dwSex == 0)
		dwPos += 5;
	
	float fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ;
	if (pItemData->GetItemScale(dwPos, fScaleX, fScaleY, fScaleZ, fPositionX, fPositionY, fPositionZ))
	{
		m_GraphicThingInstance.SetScale(fScaleX, fScaleY, fScaleZ, true);
		if (m_kHorse.IsMounting())
		{
			fPositionZ += 10.0f;
			if (m_kHorse.IsManuManni())
				fPositionZ += 10.0f;
		}
		
		m_GraphicThingInstance.SetScalePosition(fPositionX, fPositionY, fPositionZ);
	}
#endif
}

void CInstanceBase::ChangeSash(DWORD dwSash)
{
	if (!IsPC())
		return;
	
	SetSash(dwSash);
}

void CInstanceBase::ClearSashEffect()
{
	if (!m_dwSashEffect)
		return;
	
	__EffectContainer_DetachEffect(m_dwSashEffect);
	m_dwSashEffect = 0;
}
#endif

DWORD CInstanceBase::GetWeaponType()
{
	DWORD dwWeapon = GetPart(CRaceData::PART_WEAPON);
	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwWeapon, &pItemData))
		return CItemData::WEAPON_NONE;
	
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	if (pItemData->GetType() == CItemData::ITEM_TYPE_COSTUME)
		return pItemData->GetValue(3);
#endif
	return pItemData->GetWeaponType();
}

/*
void CInstanceBase::SetParts(const WORD * c_pParts)
{
	if (IsPoly())
		return;

	if (__IsShapeAnimalWear())
		return;

	UINT eWeapon=c_pParts[CRaceData::PART_WEAPON];

	if (__IsChangableWeapon(eWeapon) == false)
			eWeapon = 0;

	if (eWeapon != m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
	{
		m_GraphicThingInstance.AttachPart(CRaceData::PART_MAIN, CRaceData::PART_WEAPON, eWeapon);
		m_awPart[CRaceData::PART_WEAPON] = eWeapon;
	}

	__AttachHorseSaddle();
}
*/

void CInstanceBase::__ClearWeaponRefineEffect()
{
	if (m_swordRefineEffectRight)
	{
		__DetachEffect(m_swordRefineEffectRight);
		m_swordRefineEffectRight = 0;
	}
	if (m_swordRefineEffectLeft)
	{
		__DetachEffect(m_swordRefineEffectLeft);
		m_swordRefineEffectLeft = 0;
	}
}

void CInstanceBase::__ClearArmorRefineEffect()
{
	if (m_armorRefineEffect)
	{
		__DetachEffect(m_armorRefineEffect);
		m_armorRefineEffect = 0;
	}
}

#ifdef ENABLE_WEAPON_RARITY_SYSTEM
DWORD CInstanceBase::GetRareLevel(DWORD iPoints)
{
	if (iPoints >= 10 && iPoints < 25)
		return 1;
	else if (iPoints > 25 && iPoints <= 50)
		return 2;
	else if (iPoints > 50 && iPoints <= 75)
		return 3;
	else if (iPoints > 75)
		return 4;

	return 0;
}

DWORD CInstanceBase::__DetermineRarityLevel(DWORD dwType, DWORD dwWeaponRarePts, bool isDagger)
{
	switch (dwType)
	{
		case CItemData::WEAPON_TWO_HANDED:
		case CItemData::WEAPON_SWORD:
			switch (GetRareLevel(dwWeaponRarePts)) // real, instead of pts
			{
				case 1:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;

				case 2:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL2;

				case 3:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL3;

				case 4:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL4;

				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;
			}

			break;

		case CItemData::WEAPON_DAGGER:
			switch (GetRareLevel(dwWeaponRarePts))
			{
				case 1:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL1;

				case 2:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL2;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL2;

				case 3:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL3;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL3;

				case 4:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL4;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL4;

				default:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_FAN:
			switch (GetRareLevel(dwWeaponRarePts))
			{
				case 1:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
				
				case 2:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL2;
				
				case 3:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL3;
				
				case 4:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL4;
				
				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_BELL:
			switch (GetRareLevel(dwWeaponRarePts))
			{
				case 1:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
				
				case 2:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL2;
				
				case 3:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL3;
				
				case 4:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL4;
				
				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_BOW:
			switch (GetRareLevel(dwWeaponRarePts))
			{
				case 1:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL1;

				case 2:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL2;

				case 3:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL3;

				case 4:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL4;

				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_CLAW:
			switch (GetRareLevel(dwWeaponRarePts))
			{
				case 1:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL1;

				case 2:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL2;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL2;

				case 3:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL3;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL3;

				case 4:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL4;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL4;

				default:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL1;
			}
			break;

		default:
			break;
	}

	return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;
}
#endif

#ifdef ENABLE_EFFECT_STONE_SYSTEM
DWORD CInstanceBase::GetAddedEffectStone(DWORD dwEffectStone)
{
	if (dwEffectStone == 87001) return EFFECT_REFINED+EFFECT_STONE_ARMOR_1;
	else if (dwEffectStone == 87002) return EFFECT_REFINED+EFFECT_STONE_ARMOR_2;
	else if (dwEffectStone == 87003) return EFFECT_REFINED+EFFECT_STONE_ARMOR_3;
	else if (dwEffectStone == 87004) return EFFECT_REFINED+EFFECT_STONE_ARMOR_4;
	else if (dwEffectStone == 87005) return EFFECT_REFINED+EFFECT_STONE_ARMOR_5;
	else if (dwEffectStone == 87006) return EFFECT_REFINED+EFFECT_STONE_ARMOR_6;
}

DWORD CInstanceBase::GetAddedEffectStoneWeapon(DWORD dwType, DWORD dwEffectStone, bool isDagger)
{
	switch (dwType)
	{
		case CItemData::WEAPON_TWO_HANDED:
		case CItemData::WEAPON_SWORD:
			switch (dwEffectStone) // real, instead of pts
			{
				case 87101:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;

				case 87102:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL2;

				case 87103:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL3;

				case 87104:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL4;

				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;
			}

			break;

		case CItemData::WEAPON_DAGGER:
			switch (dwEffectStone)
			{
				case 87101:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL1;

				case 87102:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL2;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL2;

				case 87103:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL3;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL3;

				case 87104:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL4;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL4;

				default:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LDAGGER_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DAGGER_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_FAN:
			switch (dwEffectStone)
			{
				case 87101:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
				
				case 87102:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL2;
				
				case 87103:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL3;
				
				case 87104:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL4;
				
				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_BELL:
			switch (dwEffectStone)
			{
				case 87101:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
				
				case 87102:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL2;
				
				case 87103:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL3;
				
				case 87104:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL4;
				
				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_FAN_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_BOW:
			switch (dwEffectStone)
			{
				case 87101:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL1;

				case 87102:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL2;

				case 87103:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL3;

				case 87104:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL4;

				default:
					return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_BOW_SPECIAL1;
			}
			break;

		case CItemData::WEAPON_CLAW:
			switch (dwEffectStone)
			{
				case 87101:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL1;

				case 87102:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL2;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL2;

				case 87103:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL3;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL3;

				case 87104:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL4;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL4;

				default:
					if (isDagger)
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_LCLAW_SPECIAL1;
					else
						return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_DCLAW_SPECIAL1;
			}
			break;

		default:
			break;
	}

	return EFFECT_REFINED + EFFECT_SPECIAL_REFINED_SWORD_SPECIAL1;
}

UINT CInstanceBase::__GetRefinedEffect(CItemData* pItem, DWORD dwWeaponRarePts, DWORD dwEffectStone)
#else
UINT CInstanceBase::__GetRefinedEffect(CItemData* pItem, DWORD dwWeaponRarePts)
#endif
{
	DWORD refine = 0;
	CItemData* pkWeapon;
	DWORD vnum = pItem->GetIndex();
	if (CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Equipment_Weapon)), &pkWeapon))
	{
		refine = max(pkWeapon->GetRefine() + pkWeapon->GetSocketCount(), CItemData::ITEM_SOCKET_MAX_NUM) - CItemData::ITEM_SOCKET_MAX_NUM;
		if (CItemVnumHelper::IsBlackDragonWeaponSword(vnum) == true || CItemVnumHelper::IsBlackDragonWeaponDagger(vnum) == true || CItemVnumHelper::IsBlackDragonWeaponFan(vnum) == true || CItemVnumHelper::IsBlackDragonWeaponBell(vnum) == true || CItemVnumHelper::IsBlackDragonWeaponBow(vnum) == true || CItemVnumHelper::IsBlackDragonWeaponTwoHanded(vnum) == true)
			refine = 9;
	}
	switch (pItem->GetType())
	{
		case CItemData::ITEM_TYPE_WEAPON:
			__ClearWeaponRefineEffect();		
			if (refine < 7)	//현재 제련도 7 이상만 이펙트가 있습니다.
				return 0;
			switch(pItem->GetSubType())
			{				
				case CItemData::WEAPON_SWORD:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponSword(vnum) == true)
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_SWORD;
					else
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED7+refine-7;
					break;
					
				case CItemData::WEAPON_DAGGER:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						m_swordRefineEffectLeft = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone, true);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						m_swordRefineEffectLeft = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts, true);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponDagger(vnum) == true)
					{
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_DAGGER;
						m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_REFINED_7TH_DAGGER_LEFT;
					}
					else
					{
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
						m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7_LEFT+refine-7;
					}
					break;

				case CItemData::WEAPON_FAN:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponFan(vnum) == true)
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_FAN;
					else
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED7+refine-7;
					break;

				case CItemData::WEAPON_ARROW:
#ifdef ENABLE_QUIVER_SYSTEM
				case CItemData::WEAPON_QUIVER:
#endif
				case CItemData::WEAPON_BELL:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponBell(vnum) == true)
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_FAN;
					else
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED7+refine-7;
					break;

				case CItemData::WEAPON_BOW:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponBow(vnum) == true)
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_BOW;
					else
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_BOW_REFINED7+refine-7;
					break;
				
				case CItemData::WEAPON_CLAW:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						m_swordRefineEffectLeft = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone, true);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						m_swordRefineEffectLeft = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts, true);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponClaw(vnum) == true)
					{
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_CLAW;
						m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_REFINED_7TH_CLAW_LEFT;
					}
					else
					{
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
						m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7_LEFT+refine-7;
					}
					break;
				default:
#ifdef ENABLE_EFFECT_STONE_SYSTEM
					if (dwEffectStone > 0)
					{
						m_swordRefineEffectRight = GetAddedEffectStoneWeapon(pItem->GetSubType(), dwEffectStone);
						break;
					}
#endif
#ifdef ENABLE_WEAPON_RARITY_SYSTEM
					if (pItem->IsFlag(CItemData::ITEM_FLAG_RARE_ABILITY) && dwWeaponRarePts >= 10)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetSubType(), dwWeaponRarePts);
						break;
					}
#endif
					if (CItemVnumHelper::IsBlackDragonWeaponTwoHanded(vnum) == true)
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_REFINED_7TH_SWORD;
					else
						m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED7+refine-7;
			}
				
			if (m_swordRefineEffectRight)
				m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
			if (m_swordRefineEffectLeft)
				m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
			break;
		case CItemData::ITEM_TYPE_ARMOR:
			__ClearArmorRefineEffect();

		// °ⓒ¿E Æ?E­ AIÆaÆ®
		if (pItem->GetSubType() == CItemData::ARMOR_BODY)
		{
#ifdef ENABLE_EFFECT_STONE_SYSTEM
			if (dwEffectStone > 0)
			{
				m_armorRefineEffect = GetAddedEffectStone(dwEffectStone);
				__AttachEffect(m_armorRefineEffect);
				break;
			}
#endif
			if (CItemVnumHelper::IsSetaouSteelArmor(vnum) == true)
			{
				__AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL);
				__AttachEffect(EFFECT_REFINED+EFFECT_BODYARMOR_SPECIAL2);
			}
			
			if (CItemVnumHelper::IsJotunArmors(vnum) == true)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL3);
				break;
			}
			
			if (CItemVnumHelper::IsZodiacArmors(vnum) == true)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL3);
				break;				
			}
			
			if (CItemVnumHelper::IsKyanitArmors(vnum) == true)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL3);
				break;							
			}
			
			if (CItemVnumHelper::IsBlackDragonArmor(vnum) == true)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL4);
				break;
			}
			
			m_armorRefineEffect = EFFECT_REFINED+EFFECT_BODYARMOR_REFINED7+refine-7;
			__AttachEffect(m_armorRefineEffect);
		}
		break;
		case CItemData::ITEM_TYPE_COSTUME:
	
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
		if (pItem->GetSubType() == CItemData::COSTUME_WEAPON)
		{
			__ClearWeaponRefineEffect();

#ifdef USE_WEAPON_COSTUME_WITH_EFFECT
			switch(pItem->GetValue(3))
			{
				case CItemData::WEAPON_SWORD:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED7+refine-7;
					break;
					
				case CItemData::WEAPON_DAGGER:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						m_swordRefineEffectLeft = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts, true);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
					m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7_LEFT+refine-7;
					break;

				case CItemData::WEAPON_FAN:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_FANBELL_REFINED7+refine-7;
					break;

				case CItemData::WEAPON_ARROW:
#ifdef ENABLE_QUIVER_SYSTEM
				case CItemData::WEAPON_QUIVER:
#endif
				case CItemData::WEAPON_BELL:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
					break;

				case CItemData::WEAPON_BOW:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_BOW_REFINED7+refine-7;
					break;
				
				case CItemData::WEAPON_CLAW:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						m_swordRefineEffectLeft = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts, true);
						break;
					}
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7+refine-7;
					m_swordRefineEffectLeft = EFFECT_REFINED+EFFECT_SMALLSWORD_REFINED7_LEFT+refine-7;
					break;
				default:
					if (dwWeaponRarePts >= 150)
					{
						m_swordRefineEffectRight = __DetermineRarityLevel(pItem->GetValue(3), dwWeaponRarePts);
						break;
					}
					
					m_swordRefineEffectRight = EFFECT_REFINED+EFFECT_SWORD_REFINED7+refine-7;
			}
				
			if (m_swordRefineEffectRight)
				m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);
			if (m_swordRefineEffectLeft)
				m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
#endif //USE_WEAPON_COSTUME_WITH_EFFECT
		}
		break;
#endif
	}
	return 0;
}

#ifdef ENABLE_QUIVER_SYSTEM
bool CInstanceBase::SetArrow(DWORD eArrow)
{
	if (IsPoly())
		return false;
	if (__IsShapeAnimalWear())
		return false;

	if (eArrow)
	{
		CItemData * pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(eArrow, &pItemData))
			return false;

		if (pItemData->GetType() == CItemData::ITEM_TYPE_WEAPON)
		{
			if (pItemData->GetSubType() == CItemData::WEAPON_ARROW)
			{
				m_GraphicThingInstance.SetQuiverEquipped(false);
				m_GraphicThingInstance.SetQuiverEffectID(0);
				return true;
			}

			if (pItemData->GetSubType() == CItemData::WEAPON_QUIVER)
			{
				m_GraphicThingInstance.SetQuiverEquipped(true);
				m_GraphicThingInstance.SetQuiverEffectID(pItemData->GetValue(0));
				return true;
			}
		}
	}

	m_GraphicThingInstance.SetQuiverEquipped(false);
	m_GraphicThingInstance.SetQuiverEffectID(0);
	return false;
}
#endif

#ifdef ENABLE_EFFECT_STONE_SYSTEM
bool CInstanceBase::SetWeapon(DWORD eWeapon, DWORD dwWeaponRareLv, DWORD dwWeaponStoneEffect)
#else
bool CInstanceBase::SetWeapon(DWORD eWeapon, DWORD dwWeaponRareLv)
#endif
{
	if (IsPoly())
		return false;
	
	if (__IsShapeAnimalWear())
		return false;
	
	if (__IsChangableWeapon(eWeapon) == false)
		eWeapon = 0;

	m_GraphicThingInstance.AttachWeapon(eWeapon);
	m_awPart[CRaceData::PART_WEAPON] = eWeapon;
	
	//Weapon Effect
	CItemData * pItemData;
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{
#ifdef ENABLE_EFFECT_STONE_SYSTEM
		__GetRefinedEffect(pItemData, dwWeaponRareLv, dwWeaponStoneEffect);
#else
		__GetRefinedEffect(pItemData, dwWeaponRareLv);
#endif
#ifdef ENABLE_SHINING_SYSTEM
		__GetShiningEffect(pItemData);
#endif
	}
	else
	{
		__ClearWeaponRefineEffect();
#ifdef ENABLE_SHINING_SYSTEM
		__ClearWeaponShiningEffect();
#endif
	}
	return true;
}

#ifdef ENABLE_EFFECT_STONE_SYSTEM
void CInstanceBase::ChangeWeapon(DWORD eWeapon, DWORD dwWeaponRarity, DWORD dwWeaponStoneEffect)
#else
void CInstanceBase::ChangeWeapon(DWORD eWeapon, DWORD dwWeaponRarity)
#endif
{
	CItemData * pItemData;
			
	if (eWeapon == m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
		return;
	
	if (!CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
	{
		if (!m_kHorse.IsMounting())
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
		else
			SetMotionMode(CRaceMotionData::MODE_HORSE);
	}
	
#ifdef ENABLE_EFFECT_STONE_SYSTEM
	if (SetWeapon(eWeapon, dwWeaponRarity, dwWeaponStoneEffect))
#else
	if (SetWeapon(eWeapon, dwWeaponRarity))
#endif
		RefreshState(CRaceMotionData::NAME_WAIT, true);
}

#ifdef ENABLE_EFFECT_STONE_SYSTEM
bool CInstanceBase::ChangeArmor(DWORD dwArmor, DWORD dwStoneEffect)
#else
bool CInstanceBase::ChangeArmor(DWORD dwArmor)
#endif
{
	DWORD eShape;
	__ArmorVnumToShape(dwArmor, &eShape);

	if (GetShape()==eShape)
		return false;

	CAffectFlagContainer kAffectFlagContainer;
	kAffectFlagContainer.CopyInstance(m_kAffectFlagContainer);

	DWORD dwVID = GetVirtualID();
	DWORD dwRace = GetRace();
	DWORD eHair = GetPart(CRaceData::PART_HAIR);
#ifdef ENABLE_ACCE_SYSTEM
	DWORD dwSash = GetPart(CRaceData::PART_SASH);
#endif
	DWORD eWeapon = GetPart(CRaceData::PART_WEAPON);
#ifdef ENABLE_AURA_SYSTEM
	DWORD eAura = GetPart(CRaceData::PART_AURA);
#endif
	float fRot = GetRotation();
	float fAdvRot = GetAdvancingRotation();

	if (IsWalking())
		EndWalking();

	// 2004.07.25.myevan.이펙트 안 붙는 문제
	//////////////////////////////////////////////////////
	__ClearAffects();
	//////////////////////////////////////////////////////

	if (!SetRace(dwRace))
	{
		TraceError("CPythonCharacterManager::ChangeArmor - SetRace VID[%d] Race[%d] ERROR", dwVID, dwRace);
		return false;
	}

#ifdef ENABLE_EFFECT_STONE_SYSTEM
	SetArmor(dwArmor, dwStoneEffect);
#else
	SetArmor(dwArmor);
#endif
	SetHair(eHair);
#ifdef ENABLE_ACCE_SYSTEM
	SetSash(dwSash);
#endif
#ifdef ENABLE_AURA_SYSTEM
	SetAura(eAura);
#endif

	SetRotation(fRot);
	SetAdvancingRotation(fAdvRot);

	__AttachHorseSaddle();

	RefreshState(CRaceMotionData::NAME_WAIT, TRUE);

	// 2004.07.25.myevan.이펙트 안 붙는 문제
	/////////////////////////////////////////////////
	SetAffectFlagContainer(kAffectFlagContainer);
	/////////////////////////////////////////////////

	CActorInstance::IEventHandler& rkEventHandler=GetEventHandlerRef();
	rkEventHandler.OnChangeShape();

	return true;
}

bool CInstanceBase::__IsShapeAnimalWear()
{
	if (100 == GetShape() ||
		101 == GetShape() ||
		102 == GetShape() ||
		103 == GetShape())
		return true;

	return false;
}

DWORD CInstanceBase::__GetRaceType()
{
	return m_eRaceType;
}

void CInstanceBase::RefreshState(DWORD dwMotIndex, bool isLoop)
{
	DWORD dwPartItemID = m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON);

	BYTE byItemType = 0xff;
	BYTE bySubType = 0xff;

	CItemManager & rkItemMgr = CItemManager::Instance();
	CItemData * pItemData;
	if (rkItemMgr.GetItemDataPointer(dwPartItemID, &pItemData))
	{
		byItemType = pItemData->GetType();
		bySubType = pItemData->GetWeaponType();
	}

	if (IsPoly())
	{
		SetMotionMode(CRaceMotionData::MODE_GENERAL);
	}
	else if (IsWearingDress())
	{
		SetMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);
	}
	else if (IsHoldingPickAxe())
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
		}
	}
	else if (CItemData::ITEM_TYPE_ROD == byItemType)
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_FISHING);
		}
	}
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
	else if (byItemType == CItemData::ITEM_TYPE_COSTUME)
	{
		switch (pItemData->GetValue(3))
		{
			case CItemData::WEAPON_SWORD:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
				else
					SetMotionMode(CRaceMotionData::MODE_ONEHAND_SWORD);
				break;
			case CItemData::WEAPON_DAGGER:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD);
				else
					SetMotionMode(CRaceMotionData::MODE_DUALHAND_SWORD);
				break;
			case CItemData::WEAPON_BOW:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW);
				else
					SetMotionMode(CRaceMotionData::MODE_BOW);
				break;
			case CItemData::WEAPON_TWO_HANDED:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD);
				else
					SetMotionMode(CRaceMotionData::MODE_TWOHAND_SWORD);
				break;
			case CItemData::WEAPON_BELL:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_BELL);
				else
					SetMotionMode(CRaceMotionData::MODE_BELL);
				break;
			case CItemData::WEAPON_FAN:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_FAN);
				else
					SetMotionMode(CRaceMotionData::MODE_FAN);
				break;
			case CItemData::WEAPON_CLAW:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE_CLAW);
				else
					SetMotionMode(CRaceMotionData::MODE_CLAW);
				break;
			default:
				if (m_kHorse.IsMounting())
					SetMotionMode(CRaceMotionData::MODE_HORSE);
				else
					SetMotionMode(CRaceMotionData::MODE_GENERAL);
			break;
		}
	}
#endif
	else if (byItemType == CItemData::ITEM_TYPE_WEAPON)
	{
		if (m_kHorse.IsMounting())
		{
			switch (bySubType)
			{		
				case CItemData::WEAPON_SWORD:
					SetMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
					break;			
				case CItemData::WEAPON_TWO_HANDED:
					SetMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD);
					break;				
				case CItemData::WEAPON_DAGGER:
					SetMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD);
					break;
				case CItemData::WEAPON_FAN:
					SetMotionMode(CRaceMotionData::MODE_HORSE_FAN);
					break;
				case CItemData::WEAPON_BELL:
					SetMotionMode(CRaceMotionData::MODE_HORSE_BELL);
					break;
				case CItemData::WEAPON_BOW:
					SetMotionMode(CRaceMotionData::MODE_HORSE_BOW);
					break;
				case CItemData::WEAPON_CLAW:
					SetMotionMode(CRaceMotionData::MODE_HORSE_CLAW);
					break;

				case CItemData::WEAPON_ARROW:
				case CItemData::WEAPON_QUIVER:
					
				default:
					SetMotionMode(CRaceMotionData::MODE_HORSE);
					break;
			}
		}
		else
		{
			switch (bySubType)
			{		
				case CItemData::WEAPON_SWORD:
					SetMotionMode(CRaceMotionData::MODE_ONEHAND_SWORD);
					break;
				case CItemData::WEAPON_TWO_HANDED:
					SetMotionMode(CRaceMotionData::MODE_TWOHAND_SWORD);
					break;
				case CItemData::WEAPON_DAGGER:
					SetMotionMode(CRaceMotionData::MODE_DUALHAND_SWORD);
					break;
				case CItemData::WEAPON_BOW:
					SetMotionMode(CRaceMotionData::MODE_BOW);
					break;
				case CItemData::WEAPON_FAN:
					SetMotionMode(CRaceMotionData::MODE_FAN);
					break;
				case CItemData::WEAPON_BELL:
					SetMotionMode(CRaceMotionData::MODE_BELL);
					break;
				case CItemData::WEAPON_CLAW:
					SetMotionMode(CRaceMotionData::MODE_CLAW);
					break;
				case CItemData::WEAPON_ARROW:
#ifdef ENABLE_QUIVER_SYSTEM
				case CItemData::WEAPON_QUIVER:
#endif
				default:
					SetMotionMode(CRaceMotionData::MODE_GENERAL);
					break;
			}
		}
	}
	
	if (isLoop)
		m_GraphicThingInstance.InterceptLoopMotion(dwMotIndex);
	else
		m_GraphicThingInstance.InterceptOnceMotion(dwMotIndex);

	RefreshActorInstance();
}

void CInstanceBase::RegisterBoundingSphere()
{
	// Stone 일 경우 DeforomNoSkin 을 하면
	// 낙하하는 애니메이션 같은 경우 애니메이션이
	// 바운드 박스에 영향을 미쳐 컬링이 제대로 이루어지지 않는다.
	if (!IsStone())
	{
		m_GraphicThingInstance.DeformNoSkin();
	}

	m_GraphicThingInstance.RegisterBoundingSphere();
}

bool CInstanceBase::CreateDeviceObjects()
{
	return m_GraphicThingInstance.CreateDeviceObjects();
}

void CInstanceBase::DestroyDeviceObjects()
{
	m_GraphicThingInstance.DestroyDeviceObjects();
}

float CInstanceBase::GetBaseHeight()
{
	CActorInstance* pkHorse = m_kHorse.GetActorPtr();
	if (!m_kHorse.IsMounting() || !pkHorse)
		return 0.0f;
	else
		return 220.0f;
}

void CInstanceBase::Destroy()
{	
	DetachTextTail();
	
	DismountHorse();

	m_kQue_kCmdNew.clear();
	
	__EffectContainer_Destroy();
	__StoneSmoke_Destroy();

	if (__IsMainInstance())
		__ClearMainInstance();	
	
	m_GraphicThingInstance.Destroy();
	
	__Initialize();
}

void CInstanceBase::__InitializeRotationSpeed()
{
	SetRotationSpeed(c_fDefaultRotationSpeed);
}

void CInstanceBase::__Warrior_Initialize()
{
	m_kWarrior.m_dwGeomgyeongEffect=0;
}

#ifdef ENABLE_SHINING_SYSTEM
//This method checks if the item has shinings and will attach them.
//Weapons are heaving special checks for two sided weapons like daggers or fans while mounting.
//Every itemtype is supported. If no explicit logic is implemented it will be attached to "Bip01".
void CInstanceBase::__GetShiningEffect(CItemData* pItem)
{
	//Set this to true if you want to hide the refine effect if there's a shining for this item.
	bool removeRefineEffect = false;
	CItemData::TItemShiningTable shiningTable = pItem->GetItemShiningTable();
	if (pItem->GetType() == CItemData::ITEM_TYPE_WEAPON || (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME && pItem->GetSubType() == CItemData::COSTUME_WEAPON))
	{
		__ClearWeaponShiningEffect();
		if (shiningTable.Any() && removeRefineEffect)
		{
			__ClearWeaponRefineEffect();
		}
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (strcmp(shiningTable.szShinings[i], ""))
			{
				if (pItem->GetSubType() == CItemData::WEAPON_BOW)
				{
					__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON_LEFT");
				}
				else
				{
					//Check for double sided weapons or fan which is attached on both sides when mounted.
					bool twoSidedWeapon = pItem->GetSubType() == CItemData::WEAPON_DAGGER || (IsMountingHorse() && pItem->GetSubType() == CItemData::WEAPON_FAN) || pItem->GetSubType() == CItemData::WEAPON_CLAW;
					if (twoSidedWeapon)
					{
						__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON_LEFT");
					}
					__AttachWeaponShiningEffect(i, shiningTable.szShinings[i], "PART_WEAPON");
				}
			}
		}
	}
	else if ((pItem->GetType() == CItemData::ITEM_TYPE_ARMOR && pItem->GetSubType() == CItemData::ARMOR_BODY) || (pItem->GetType() == CItemData::ITEM_TYPE_COSTUME && pItem->GetSubType() == CItemData::COSTUME_BODY))
	{
		__ClearArmorShiningEffect();
		if (shiningTable.Any() && removeRefineEffect)
		{
			__ClearArmorRefineEffect();
		}
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (strcmp(shiningTable.szShinings[i], ""))
			{
				__AttachArmorShiningEffect(i, shiningTable.szShinings[i]);
			}
		}
	}
}
//Attaching the shining effect. 
//BoneName can be "PART_WEAPON" or "PART_WEAPON_LEFT" to get the attaching bone name dynamically.
//If boneName is not given "Bip01" is used as boneName.
void CInstanceBase::__AttachWeaponShiningEffect(int effectIndex, const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}
	if (effectIndex >= CItemData::ITEM_SHINING_MAX_COUNT)
	{
		return;
	}
	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);
	if (!strcmp(boneName, "PART_WEAPON"))
	{
		const char* c_szRightBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON, &c_szRightBoneName);
		if (strcmp(c_szRightBoneName, ""))
		{
			m_weaponShiningEffects[0][effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, c_szRightBoneName, effectFileName);
		}
	}
	else if (!strcmp(boneName, "PART_WEAPON_LEFT"))
	{
		const char* c_szLeftBoneName;
		m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON_LEFT, &c_szLeftBoneName);
		if (strcmp(c_szLeftBoneName, ""))
		{
			m_weaponShiningEffects[1][effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, c_szLeftBoneName, effectFileName);
		}
	}
	else
	{
		Tracef("Invalid partname for getting attaching bone name. %s - %s", effectFileName, boneName);
	}
}
//Attaching the armor shining effect. 
//If boneName is not given "Bip01" is used as boneName.
void CInstanceBase::__AttachArmorShiningEffect(int effectIndex, const char* effectFileName, const char* boneName)
{
	if (IsAffect(AFFECT_INVISIBILITY))
	{
		return;
	}
	if (effectIndex >= CItemData::ITEM_SHINING_MAX_COUNT)
	{
		return;
	}
	if (!strcmp(boneName, ""))
	{
		Tracef("Empty bone name for attaching armor shining. Effect Index: %i, EffectFileName: %s", effectIndex, effectFileName);
		return;
	}
	CEffectManager::Instance().RegisterEffect(effectFileName, false, false);
	m_armorShiningEffects[effectIndex] = m_GraphicThingInstance.AttachEffectByName(0, boneName, effectFileName);
}
//Clears all weapon shining effects. Left & Right if set.
void CInstanceBase::__ClearWeaponShiningEffect(bool detaching)
{
	if (detaching)
	{
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			if (m_weaponShiningEffects[0][i])
			{
				__DetachEffect(m_weaponShiningEffects[0][i]);
			}
			if (m_weaponShiningEffects[1][i])
			{
				__DetachEffect(m_weaponShiningEffects[1][i]);
			}
		}
	}
		memset(&m_weaponShiningEffects, 0, sizeof(m_weaponShiningEffects));
}
//Clears all armor shining effects.
void CInstanceBase::__ClearArmorShiningEffect(bool detaching)
{
	if (detaching)
	{
		for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
		{
			__DetachEffect(m_armorShiningEffects[i]);
		}
	}
	memset(&m_armorShiningEffects, 0, sizeof(m_armorShiningEffects));
}
#endif

void CInstanceBase::__Initialize()
{
	__Warrior_Initialize();
	__StoneSmoke_Inialize();
	__EffectContainer_Initialize();
	__InitializeRotationSpeed();

	SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());

	m_kAffectFlagContainer.Clear();

	m_dwLevel = 0;
	m_dwGuildID = 0;
	m_dwEmpireID = 0;

	m_eType = 0;
	m_eRaceType = 0;
	m_eShape = 0;
	m_dwRace = 0;
	m_dwOriginalRace = 0;
	m_dwVirtualNumber = 0;

	m_dwBaseCmdTime=0;
	m_dwBaseChkTime=0;
	m_dwSkipTime=0;

	m_GraphicThingInstance.Initialize();

	m_dwAdvActorVID=0;
	m_dwLastDmgActorVID=0;

	m_nAverageNetworkGap=0;
	m_dwNextUpdateHeightTime=0;

	// Moving by keyboard
	m_iRotatingDirection = DEGREE_DIRECTION_SAME;

	// Moving by mouse	
	m_isTextTail = FALSE;
	m_isGoing = FALSE;
	NEW_SetSrcPixelPosition(TPixelPosition(0, 0, 0));
	NEW_SetDstPixelPosition(TPixelPosition(0, 0, 0));

	m_kPPosDust = TPixelPosition(0, 0, 0);


	m_kQue_kCmdNew.clear();

	m_dwLastComboIndex = 0;

	m_swordRefineEffectRight = 0;
	m_swordRefineEffectLeft = 0;
	m_armorRefineEffect = 0;
#ifdef ENABLE_ACCE_SYSTEM
	m_dwSashEffect = 0;
#endif
#ifdef ENABLE_SHINING_SYSTEM
	__ClearWeaponShiningEffect(false);
	__ClearArmorShiningEffect(false);
#endif
#ifdef ENABLE_AURA_SYSTEM
	m_auraRefineEffect = 0;
#endif

	m_sAlignment = 0;
	m_byPKMode = 0;
	m_isKiller = false;
	m_isPartyMember = false;

	m_bEnableTCPState = TRUE;

	m_stName = "";
	
#if defined(ENABLE_BATTLE_ZONE_SYSTEM)
	combat_zone_rank = 0;
	combat_zone_points = 0;
#endif

#ifdef ENABLE_BUFFI_SYSTEM
	is_support_shaman = false;
#endif

#if defined(WJ_SHOW_MOB_INFO)
	m_dwAIFlag = 0;
#endif

	memset(m_awPart, 0, sizeof(m_awPart));
	memset(m_adwCRCAffectEffect, 0, sizeof(m_adwCRCAffectEffect));
	//memset(m_adwCRCEmoticonEffect, 0, sizeof(m_adwCRCEmoticonEffect));
	memset(&m_kMovAfterFunc, 0, sizeof(m_kMovAfterFunc));

	m_bDamageEffectType = false;
	m_dwDuelMode = DUEL_NONE;
	m_dwEmoticonTime = 0;
	m_IsAlwaysRender = false;
}

CInstanceBase::CInstanceBase()
{
	__Initialize();
}

CInstanceBase::~CInstanceBase()
{
	Destroy();
}

bool CInstanceBase::IsAlwaysRender()
{
	return m_IsAlwaysRender;
}

void CInstanceBase::SetAlwaysRender(bool val)
{
	m_IsAlwaysRender = val;
}

void CInstanceBase::GetBoundBox(D3DXVECTOR3 * vtMin, D3DXVECTOR3 * vtMax)
{
	m_GraphicThingInstance.GetBoundBox(vtMin, vtMax);
}
