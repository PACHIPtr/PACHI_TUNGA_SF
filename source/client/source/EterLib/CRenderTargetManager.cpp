#include "StdAfx.h"
#include "CRenderTargetManager.h"
#include "../EterBase/Stl.h"
//#include "GrpBase.h"
#include <memory>


CRenderTargetManager::CRenderTargetManager()
{
}

CRenderTargetManager::~CRenderTargetManager()
{
	Destroy();
}

void CRenderTargetManager::Destroy()
{
	m_renderTargets.clear();
}

void CRenderTargetManager::DeformModels()
{
	for (const auto& elem : m_renderTargets)
		elem.second->DeformModel();
}

void CRenderTargetManager::UpdateModels()
{
	for (auto& elem : m_renderTargets)
		elem.second->UpdateModel();
}

void CRenderTargetManager::RenderBackgrounds()
{
	for (const auto& elem : m_renderTargets)
		elem.second->RenderBackground();
}

void CRenderTargetManager::RenderModels()
{
	for (const auto& elem : m_renderTargets)
		elem.second->RenderModel();
}

void CRenderTargetManager::CreateRenderTargetTextures()
{
	for (const auto& elem : m_renderTargets)
		elem.second->CreateTextures();
}

void CRenderTargetManager::ReleaseRenderTargetTextures()
{
	for (const auto& elem : m_renderTargets)
		elem.second->ReleaseTextures();
}

bool CRenderTargetManager::CreateRenderTarget(const uint8_t index, const int width, const int height)
{
	if (index < 1 || GetRenderTarget(index))
		return false;

	m_renderTargets.emplace(index, std::make_shared<CRenderTarget>(width, height));
	return true;
}

std::shared_ptr<CRenderTarget> CRenderTargetManager::GetRenderTarget(const uint8_t index)
{
	const auto it = m_renderTargets.find(index);
	if (it != m_renderTargets.end())
		return it->second;

	return nullptr;
}
