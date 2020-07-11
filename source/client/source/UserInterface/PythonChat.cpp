#include "StdAfx.h"
#include "PythonChat.h"

#include "AbstractApplication.h"
#include "PythonCharacterManager.h"
#include "../eterBase/Timer.h"
#include <boost/range/adaptor/reversed.hpp>

int CPythonChat::TChatSet::ms_iChatModeSize = CHAT_TYPE_MAX_NUM;

const float c_fStartDisappearingTime = 5.0f;
const int c_iMaxLineCount = 5;

///////////////////////////////////////////////////////////////////////////////////////////////////

CDynamicPool<CPythonChat::SChatLine> CPythonChat::SChatLine::ms_kPool;

void CPythonChat::SetChatColor(UINT eType, UINT r, UINT g, UINT b)
{
	if (eType>=CHAT_TYPE_MAX_NUM)
		return;

	DWORD dwColor=(0xff000000)|(r<<16)|(g<<8)|(b);
	m_akD3DXClrChat[eType]=D3DXCOLOR(dwColor);	
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CPythonChat::SChatLine* CPythonChat::SChatLine::New()
{
	return ms_kPool.Construct();
}

void CPythonChat::SChatLine::Delete(CPythonChat::SChatLine* pkChatLine)
{
	pkChatLine->Instance.Destroy();
	ms_kPool.Destruct(pkChatLine);
}

void CPythonChat::SChatLine::DestroySystem()
{
	ms_kPool.Destroy();	
}

void CPythonChat::SChatLine::SetColor(DWORD dwID, DWORD dwColor)
{
	assert(dwID < CHAT_LINE_COLOR_ARRAY_MAX_NUM);

	if (dwID >= CHAT_LINE_COLOR_ARRAY_MAX_NUM)
		return;

	aColor[dwID] = dwColor;
}

void CPythonChat::SChatLine::SetColorAll(DWORD dwColor)
{
	for (int i = 0; i < CHAT_LINE_COLOR_ARRAY_MAX_NUM; ++i)
		aColor[i] = dwColor;
}

D3DXCOLOR & CPythonChat::SChatLine::GetColorRef(DWORD dwID)
{
	assert(dwID < CHAT_LINE_COLOR_ARRAY_MAX_NUM);

	if (dwID >= CHAT_LINE_COLOR_ARRAY_MAX_NUM)
	{
		static D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
		return color;
	}

	return aColor[dwID];
}

CPythonChat::SChatLine::SChatLine()
{
	for (int i = 0; i < CHAT_LINE_COLOR_ARRAY_MAX_NUM; ++i)
		aColor[i] = 0xff0000ff;
}
CPythonChat::SChatLine::~SChatLine() 
{
	Instance.Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int CPythonChat::CreateChatSet(DWORD dwID)
{
	m_ChatSetMap.emplace (dwID, TChatSet());
	return dwID;
}

void CPythonChat::UpdateViewMode(DWORD dwID)
{
	IAbstractApplication& rApp=IAbstractApplication::GetSingleton();

	float fcurTime = rApp.GetGlobalTime();
	//float felapsedTime = rApp.GetGlobalElapsedTime();

	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	TChatLineList * pLineList = &(pChatSet->m_ShowingChatLineList);
	int iLineIndex = pLineList->size();
	int iHeight = -(int(pLineList->size()+1) * pChatSet->m_iStep);

	TChatLineList::iterator itor;
	for (itor = pLineList->begin(); itor != pLineList->end();)
	{
		TChatLine * pChatLine = (*itor);

		D3DXCOLOR & rColor = pChatLine->GetColorRef(dwID);

		float fElapsedTime = (fcurTime - pChatLine->fAppendedTime);
		if (fElapsedTime >= c_fStartDisappearingTime || iLineIndex >= c_iMaxLineCount)
		{
			rColor.a -= rColor.a / 10.0f;

			if (rColor.a <= 0.1f)
			{
				itor = pLineList->erase(itor);
			}
			else
			{
				++itor;
			}
		}
		else
		{
			++itor;
		}

		/////

		iHeight += pChatSet->m_iStep;
		--iLineIndex;

		pChatLine->Instance.SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(rColor);
		pChatLine->Instance.Update();
		if (pChatLine->ImageInstance && pChatLine->iType == CHAT_TYPE_SHOUT)
			pChatLine->ImageInstance->SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight + 2);
	}
}

void CPythonChat::UpdateEditMode(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	const int c_iAlphaLine = max(0, GetVisibleLineCount(dwID) - GetEditableLineCount(dwID) + 2);

	int iLineIndex = 0;
	float fAlpha = 0.0f;
	float fAlphaStep = 0.0f;

	if (c_iAlphaLine > 0)
		fAlphaStep = 1.0f / float(c_iAlphaLine);

	TChatLineList * pLineList = &(pChatSet->m_ShowingChatLineList);
	int iHeight = -(int(pLineList->size()+1) * pChatSet->m_iStep);

	for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
	{
		TChatLine * pChatLine = (*itor);

		D3DXCOLOR & rColor = pChatLine->GetColorRef(dwID);

		if (iLineIndex < c_iAlphaLine)
		{
			rColor.a += (fAlpha - rColor.a) / 10.0f;
			fAlpha = fMIN(fAlpha+fAlphaStep, 1.0f);
		}
		else
		{
			rColor.a = fMIN(rColor.a+0.05f, 1.0f);
		}

		iHeight += pChatSet->m_iStep;
		pChatLine->Instance.SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(rColor);
		pChatLine->Instance.Update();
		if (pChatLine->ImageInstance && pChatLine->iType == CHAT_TYPE_SHOUT)
			pChatLine->ImageInstance->SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight + 2);
	}
}

void CPythonChat::UpdateLogMode(DWORD dwID)
{
	auto pChatSet = GetChatSetPtr (dwID);
	if (!pChatSet)
	{
		return;
	}

	auto pLineList = pChatSet->m_ShowingChatLineList;
	int iHeight = 0;

	for(auto & pChatLine : boost::adaptors::reverse(pLineList))
	{
		iHeight -= pChatSet->m_iStep;
		pChatLine->Instance.SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight);
		pChatLine->Instance.SetColor(pChatLine->GetColorRef(dwID));
		pChatLine->Instance.Update();
		if (pChatLine->ImageInstance && pChatLine->iType == CHAT_TYPE_SHOUT)
			pChatLine->ImageInstance->SetPosition(pChatSet->m_ix, pChatSet->m_iy + iHeight + 2);
	}
}

void CPythonChat::Update(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	switch (pChatSet->m_iBoardState)
	{
		case BOARD_STATE_VIEW:
			UpdateViewMode(dwID);
			break;
		case BOARD_STATE_EDIT:
			UpdateEditMode(dwID);
			break;
		case BOARD_STATE_LOG:
			UpdateLogMode(dwID);
			break;
	}

	DWORD dwcurTime = CTimer::Instance().GetCurrentMillisecond();
	for (TWaitChatList::iterator itor = m_WaitChatList.begin(); itor != m_WaitChatList.end();)
	{
		TWaitChat & rWaitChat = *itor;

		if (rWaitChat.dwAppendingTime < dwcurTime)
		{
			AppendChat(rWaitChat.iType, rWaitChat.strChat.c_str());

			itor = m_WaitChatList.erase(itor);
		}
		else
		{
			++itor;
		}
	}
}

void CPythonChat::Render(DWORD dwID)
{
	TChatLineList * pLineList = GetChatLineListPtr(dwID);
	if (!pLineList)
		return;

	for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
	{
		TChatLine* pChatLine = (*itor);
		CGraphicTextInstance& rInstance = (*itor)->Instance;
		rInstance.Render();
		if (pChatLine->ImageInstance && pChatLine->iType == CHAT_TYPE_SHOUT)
		{
			CGraphicImageInstance*& imInstance = (*itor)->ImageInstance;
			if (imInstance)
				imInstance->Render();
		}
	}
}


void CPythonChat::SetBoardState(DWORD dwID, int iState)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iBoardState = iState;
	ArrangeShowingChat(dwID);
}
void CPythonChat::SetPosition(DWORD dwID, int ix, int iy)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_ix = ix;
	pChatSet->m_iy = iy;
}
void CPythonChat::SetHeight(DWORD dwID, int iHeight)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iHeight = iHeight;
}
void CPythonChat::SetStep(DWORD dwID, int iStep)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iStep = iStep;
}
void CPythonChat::ToggleChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = 1 - pChatSet->m_iMode[iMode];
// 	Tracef("ToggleChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::EnableChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = TRUE;
// 	Tracef("EnableChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::DisableChatMode(DWORD dwID, int iMode)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_iMode[iMode] = FALSE;
// 	Tracef("DisableChatMode : %d\n", iMode);
	ArrangeShowingChat(dwID);
}
void CPythonChat::SetEndPos(DWORD dwID, float fPos)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	fPos = max(0.0f, fPos);
	fPos = min(1.0f, fPos);
	if (pChatSet->m_fEndPos != fPos)
	{
		pChatSet->m_fEndPos = fPos;
		ArrangeShowingChat(dwID);
	}
}


int CPythonChat::GetVisibleLineCount(DWORD dwID)
{
	TChatLineList * pLineList = GetChatLineListPtr(dwID);
	if (!pLineList)
		return 0;

	return pLineList->size();
}

int CPythonChat::GetEditableLineCount(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;

	return pChatSet->m_iHeight / pChatSet->m_iStep + 1;
}

int CPythonChat::GetLineCount(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;

	int iCount = 0;
	for (DWORD i = 0; i < m_ChatLineDeque.size(); ++i)
	{
		if (!pChatSet->CheckMode(m_ChatLineDeque[i]->iType))
			continue;

		++iCount;
	}

	return iCount;
}

int CPythonChat::GetLineStep(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return 0;

	return pChatSet->m_iStep;
}

CPythonChat::TChatLineList * CPythonChat::GetChatLineListPtr(DWORD dwID)
{
	TChatSetMap::iterator itor = m_ChatSetMap.find(dwID);
	if (m_ChatSetMap.end() == itor)
		return nullptr;

	TChatSet & rChatSet = itor->second;
	return &(rChatSet.m_ShowingChatLineList);
}

CPythonChat::TChatSet * CPythonChat::GetChatSetPtr(DWORD dwID)
{
	TChatSetMap::iterator itor = m_ChatSetMap.find(dwID);
	if (m_ChatSetMap.end() == itor)
		return nullptr;

	TChatSet & rChatSet = itor->second;
	return &rChatSet;
}

void CPythonChat::ArrangeShowingChat(DWORD dwID)
{
	TChatSet * pChatSet = GetChatSetPtr(dwID);
	if (!pChatSet)
		return;

	pChatSet->m_ShowingChatLineList.clear();

	TChatLineDeque TempChatLineDeque;
	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
	{
		TChatLine * pChatLine = *itor;
		if (pChatSet->CheckMode(pChatLine->iType))
			TempChatLineDeque.push_back(pChatLine);
	}

	int icurLineCount = TempChatLineDeque.size();
	int iVisibleLineCount = min(icurLineCount, (pChatSet->m_iHeight + pChatSet->m_iStep) / pChatSet->m_iStep);
	int iEndLine = iVisibleLineCount + int(float(icurLineCount - iVisibleLineCount - 1) * pChatSet->m_fEndPos);

	/////

	int iHeight = 12;
	for (int i = min(icurLineCount-1, iEndLine); i >= 0; --i)
	{
		if (!pChatSet->CheckMode(TempChatLineDeque[i]->iType))
			continue;

		if (pChatSet->m_iHeight + pChatSet->m_iStep <= iHeight)
		{
			break;
		}

		pChatSet->m_ShowingChatLineList.push_front(TempChatLineDeque[i]);

		iHeight += pChatSet->m_iStep;
	}
}

void CPythonChat::AppendChat(int iType, const char * c_szChat)
{
	if (strstr(c_szChat, "#ebvs"))
		return;
	
#ifdef ENABLE_OX_RENEWAL
	if (strstr(c_szChat, "#start") || strstr(c_szChat, "#send") || strstr(c_szChat, "#end"))
		return;
#endif

	std::string str = c_szChat;

	//static CResource * s_pResource = CResourceManager::Instance().GetResourcePointer(g_strDefaultFontName.c_str());

	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
	if (!pkDefaultFont)
	{
		TraceError("CPythonChat::AppendChat - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	IAbstractApplication& rApp = IAbstractApplication::GetSingleton();
	SChatLine* pChatLine = SChatLine::New();
	pChatLine->iType = iType;
	pChatLine->Instance.SetValue(c_szChat);
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	pChatLine->fAppendedTime = rApp.GetGlobalTime();
	pChatLine->SetColorAll(GetChatColor(iType));

	m_ChatLineDeque.push_back(pChatLine);
	if (m_ChatLineDeque.size() > CHAT_LINE_MAX_NUM)
	{
		SChatLine* pkChatLine = m_ChatLineDeque.front();
		if(pkChatLine)
			SChatLine::Delete (pkChatLine);

		m_ChatLineDeque.pop_front();
	}

	for(auto& itor : m_ChatSetMap)
	{
		TChatSet* pChatSet = &(itor.second);

		if (BOARD_STATE_EDIT == pChatSet->m_iBoardState)
		{
			ArrangeShowingChat(itor.first);
		}
		else
		{
			pChatSet->m_ShowingChatLineList.push_back(pChatLine);
			if (pChatSet->m_ShowingChatLineList.size() > CHAT_LINE_MAX_NUM)
			{
				pChatSet->m_ShowingChatLineList.pop_front();
			}
		}
	}
	
	if (iType == CHAT_TYPE_SHOUT)
	{
		CGraphicImageInstance*& prFlag = pChatLine->ImageInstance;
		std::string const chat = c_szChat;
		const char* empireflag = "";
		if (chat.find("[1]") != std::string::npos)
			empireflag = "shinsoo";
		else if (chat.find("[2]") != std::string::npos)
			empireflag = "chunjo";
		else if (chat.find("[3]") != std::string::npos)
			empireflag = "jinno";
		else
			empireflag = "none";

		if (empireflag != "")
		{
			char szPath[256];
			sprintf(szPath, "d:/ymir work/ui/game/flag/%s.tga", empireflag);
			CGraphicImage* pFlagImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(szPath);
			prFlag = CGraphicImageInstance::New();
			prFlag->SetImagePointer(pFlagImage);
		}
	}
}

void CPythonChat::AppendChatWithDelay(int iType, const char * c_szChat, int iDelay)
{
	TWaitChat WaitChat;
	WaitChat.iType = iType;
	WaitChat.strChat = c_szChat;
	WaitChat.dwAppendingTime = CTimer::Instance().GetCurrentMillisecond() + iDelay;
	m_WaitChatList.push_back(WaitChat);
}

DWORD CPythonChat::GetChatColor(int iType)
{
	if (iType<CHAT_TYPE_MAX_NUM)
	{
		return m_akD3DXClrChat[iType];
	}

	return D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
}

void CPythonChat::IgnoreCharacter(const char * c_szName)
{
	TIgnoreCharacterSet::iterator itor = m_IgnoreCharacterSet.find(c_szName);
	// NOTE : 이미 차단 중이라면..
	if (m_IgnoreCharacterSet.end() != itor)
	{
		m_IgnoreCharacterSet.erase(itor);
	}
	// NOTE : 차단이 되지 않은 캐릭터라면..
	else
	{
		m_IgnoreCharacterSet.insert(c_szName);
	}
}

BOOL CPythonChat::IsIgnoreCharacter(const char * c_szName)
{
	TIgnoreCharacterSet::iterator itor = m_IgnoreCharacterSet.find(c_szName);

	if (m_IgnoreCharacterSet.end() == itor)
		return FALSE;

	return TRUE;
}

FloatingChat * CPythonChat::CreateWhisper(const char * c_szName)
{
	FloatingChat * pWhisper = FloatingChat::New();
	m_WhisperMap.insert(TWhisperMap::value_type(c_szName, pWhisper));
	return pWhisper;
}

void CPythonChat::AppendWhisper(int iType, const char * c_szName, const char * c_szChat)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	FloatingChat * pWhisper;
	if (itor == m_WhisperMap.end())
	{
		pWhisper = CreateWhisper(c_szName);
	}
	else
	{
		pWhisper = itor->second;
	}

	pWhisper->AppendChat(iType, c_szChat);
}

void CPythonChat::ClearWhisper(const char * c_szName)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	if (itor != m_WhisperMap.end())
	{
		FloatingChat * pWhisper = itor->second;
		FloatingChat::Delete(pWhisper);
		
		m_WhisperMap.erase(itor);
	}
}

BOOL CPythonChat::GetWhisper(const char * c_szName, FloatingChat ** ppWhisper)
{
	TWhisperMap::iterator itor = m_WhisperMap.find(c_szName);

	if (itor == m_WhisperMap.end())
		return FALSE;

	*ppWhisper = itor->second;

	return TRUE;
}

void CPythonChat::InitWhisper(PyObject * ppyObject)
{
	TWhisperMap::iterator itor = m_WhisperMap.begin();
	for (; itor != m_WhisperMap.end(); ++itor)
	{
		std::string strName = itor->first;
		PyCallClassMemberFunc(ppyObject, "MakeWhisperButton", Py_BuildValue("(s)", strName.c_str()));
	}
}

void CPythonChat::__DestroyWhisperMap()
{
	TWhisperMap::iterator itor = m_WhisperMap.begin();
	for (; itor != m_WhisperMap.end(); ++itor)
	{
		FloatingChat::Delete(itor->second);
	}
	m_WhisperMap.clear();
}

void CPythonChat::Close()
{
	TChatSetMap::iterator itor = m_ChatSetMap.begin();
	for (; itor != m_ChatSetMap.end(); ++itor)
	{
		TChatSet & rChatSet = itor->second;
		TChatLineList * pLineList = &(rChatSet.m_ShowingChatLineList);
		for (TChatLineList::iterator itor = pLineList->begin(); itor != pLineList->end(); ++itor)
		{
			TChatLine * pChatLine = (*itor);
			pChatLine->fAppendedTime = 0.0f;
		}
	}
}

void CPythonChat::Destroy()
{
	__DestroyWhisperMap();	

	m_ShowingChatLineList.clear();
	m_ChatSetMap.clear();
	m_ChatLineDeque.clear();

	SChatLine::DestroySystem();
	FloatingChat::DestroySystem();

	__Initialize();
}

void CPythonChat::__Initialize()
{
	m_akD3DXClrChat[CHAT_TYPE_TALKING]		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_INFO]			= D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_NOTICE]		= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_PARTY]		= D3DXCOLOR(0.542f, 1.0f, 0.949f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_GUILD]		= D3DXCOLOR(0.906f, 0.847f, 1.0f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_COMMAND]		= D3DXCOLOR(0.658f, 1.0f, 0.835f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_SHOUT]		= D3DXCOLOR(0.658f, 1.0f, 0.835f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_WHISPER]		= D3DXCOLOR(0xff4AE14A);
	m_akD3DXClrChat[CHAT_TYPE_BIG_NOTICE]	= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
	// m_akD3DXClrChat[CHAT_TYPE_UNK_10]		= D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
#ifdef ENABLE_DICE_SYSTEM
	m_akD3DXClrChat[CHAT_TYPE_DICE_INFO]	= D3DXCOLOR(0xFFcc00cc);
#endif
#ifdef ENABLE_OX_RENEWAL
	m_akD3DXClrChat[CHAT_TYPE_CONTROL_NOTICE] = D3DXCOLOR(1.0f, 0.902f, 0.730f, 1.0f);
#endif
#ifdef ENABLE_ZODIAC_TEMPLE_CHAT
	m_akD3DXClrChat[CHAT_TYPE_ZODIAC_NOTICE]	= D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_ZODIAC_SUB_NOTICE]	= D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
	m_akD3DXClrChat[CHAT_TYPE_ZODIAC_NOTICE_CLEAR]	= D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f);
#endif
}

CPythonChat::CPythonChat()
{
	__Initialize();
}

CPythonChat::~CPythonChat()
{
	assert(m_ChatLineDeque.empty());
	assert(m_ShowingChatLineList.empty());
	assert(m_ChatSetMap.empty());
	assert(m_WhisperMap.empty());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
CDynamicPool<FloatingChat> FloatingChat::ms_kPool;

FloatingChat* FloatingChat::New()
{
	return ms_kPool.Construct();
}

void FloatingChat::Delete(FloatingChat* pkWhisper)
{
	pkWhisper->Destroy();
	ms_kPool.Destruct(pkWhisper);
}

void FloatingChat::DestroySystem()
{
	ms_kPool.Destroy();	

	SChatLine::DestroySystem();	
}

void FloatingChat::SetPosition(float fPosition)
{
	m_fcurPosition = fPosition;
	__ArrangeChat();
}

void FloatingChat::SetBoxSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
	{
		TChatLine * pChatLine = *itor;
		pChatLine->Instance.SetLimitWidth(fWidth);
	}
}

void FloatingChat::AppendChat(int iType, const char * c_szChat)
{
	
	if (strstr(c_szChat, "#ebvs"))
    	return;
	
	std::string str = c_szChat;
	
	// DEFAULT_FONT
	//static CResource * s_pResource = CResourceManager::Instance().GetResourcePointer(g_strDefaultFontName.c_str());

#if defined(LOCALE_SERVICE_YMIR) || defined(LOCALE_SERVICE_JAPAN) || defined(LOCALE_SERVICE_HONGKONG) || defined(LOCALE_SERVICE_TAIWAN) || defined(LOCALE_SERVICE_NEWCIBN)
	CGraphicText* pkDefaultFont = static_cast<CGraphicText*>(DefaultFont_GetResource());
#else
	CGraphicText* pkDefaultFont = (iType == CPythonChat::WHISPER_TYPE_GM) ? static_cast<CGraphicText*>(DefaultItalicFont_GetResource()) : static_cast<CGraphicText*>(DefaultFont_GetResource());
#endif

	if (!pkDefaultFont)
	{
		TraceError("CWhisper::AppendChat - CANNOT_FIND_DEFAULT_FONT");
		return;
	}
	// END_OF_DEFAULT_FONT

	SChatLine * pChatLine = SChatLine::New();
	pChatLine->Instance.SetValue(c_szChat);

	// DEFAULT_FONT
	pChatLine->Instance.SetTextPointer(pkDefaultFont);
	// END_OF_DEFAULT_FONT
	
	pChatLine->Instance.SetLimitWidth(m_fWidth);
	pChatLine->Instance.SetMultiLine(TRUE);

	switch(iType)
	{
		case CPythonChat::WHISPER_TYPE_SYSTEM:
			pChatLine->Instance.SetColor(D3DXCOLOR(1.0f, 0.785f, 0.785f, 1.0f));
			break;
		case CPythonChat::WHISPER_TYPE_GM:
			pChatLine->Instance.SetColor(D3DXCOLOR(1.0f, 0.632f, 0.0f, 1.0f));
			break;
		case CPythonChat::WHISPER_TYPE_CHAT:
		default:
			pChatLine->Instance.SetColor(0xffffffff);
			break;
	}

	m_ChatLineDeque.push_back(pChatLine);

	__ArrangeChat();
}

void FloatingChat::__ArrangeChat()
{
	for (TChatLineDeque::iterator itor = m_ChatLineDeque.begin(); itor != m_ChatLineDeque.end(); ++itor)
	{
		TChatLine * pChatLine = *itor;
		pChatLine->Instance.Update();
	}
}

void FloatingChat::Render(float fx, float fy)
{
	float fHeight = fy;
	if (!m_drawDownwards)
		fHeight += m_fHeight; //We'll start at the bottom

	int iViewCount = int(m_fHeight / m_fLineStep) - 1;
	int iLineCount = int(m_ChatLineDeque.size());
	int iStartLine = -1;
	if (iLineCount > iViewCount)
	{
		iStartLine = int(float(iLineCount-iViewCount) * m_fcurPosition) + iViewCount - 1;
	}
	else if (!m_ChatLineDeque.empty())
	{
		iStartLine = iLineCount - 1;
	}

	RECT Rect = { fx, fy, fx+m_fWidth, fy+m_fHeight };

	for (int i = iStartLine; i >= 0; --i)
	{
		assert(i >= 0 && i < int(m_ChatLineDeque.size()));
		TChatLine * pChatLine = m_ChatLineDeque[i];

		WORD wLineCount = pChatLine->Instance.GetTextLineCount();
		if (!m_drawDownwards)
			fHeight -= wLineCount * m_fLineStep;
		else
			if (i != iStartLine)
				fHeight += wLineCount * m_fLineStep;

		pChatLine->Instance.SetPosition(fx, fHeight);
		pChatLine->Instance.Render(&Rect);

		if (m_drawDownwards)
			if(fHeight + (wLineCount+1) * m_fLineStep > fy + m_fHeight)
				break;
		else 
			if (fHeight < fy)
				break;
	}
}

void FloatingChat::__Initialize()
{
	m_fLineStep = 15.0f;
	m_fWidth = 300.0f;
	m_fHeight = 120.0f;
	m_fcurPosition = 1.0f;
	m_drawDownwards = false;
}

void FloatingChat::Destroy()
{
	std::for_each(m_ChatLineDeque.begin(), m_ChatLineDeque.end(), SChatLine::Delete);
	m_ChatLineDeque.clear();
	m_ShowingChatLineList.clear();
}

FloatingChat::FloatingChat()
{
	__Initialize();
}

FloatingChat::~FloatingChat()
{
	Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

CDynamicPool<FloatingChat::SChatLine> FloatingChat::SChatLine::ms_kPool;

FloatingChat::SChatLine* FloatingChat::SChatLine::New()
{
	return ms_kPool.Construct();
}

void FloatingChat::SChatLine::Delete(FloatingChat::SChatLine* pkChatLine)
{
	pkChatLine->Instance.Destroy();
	ms_kPool.Destruct(pkChatLine);
}

void FloatingChat::SChatLine::DestroySystem()
{
	ms_kPool.Destroy();
}