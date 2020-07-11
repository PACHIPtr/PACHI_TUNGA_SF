#include "StdAfx.h"
#include "../EterBase/Stl.h"
#include "GrpRenderTargetTexture.h"
#include "StateManager.h"

CGraphicRenderTargetTexture::CGraphicRenderTargetTexture()
	: m_d3dFormat{D3DFMT_UNKNOWN}, m_depthStencilFormat{D3DFMT_UNKNOWN}
{
	Initialize();
	m_renderRect.left = 0;
	m_renderRect.top = 0;
	m_renderRect.right = 0;
	m_renderRect.bottom = 0;
}

CGraphicRenderTargetTexture::~CGraphicRenderTargetTexture()
{
	Reset();
}

void CGraphicRenderTargetTexture::ReleaseTextures()
{
	safe_release(m_lpd3dRenderTexture);
	safe_release(m_lpd3dRenderTargetSurface);
	safe_release(m_lpd3dDepthSurface);
	safe_release(m_lpd3dDepthSurface);
	safe_release(m_lpd3dOriginalRenderTarget);
	safe_release(m_lpd3dOldDepthBufferSurface);
	m_renderRect.left = 0;
	m_renderRect.top = 0;
	m_renderRect.right = 0;
	m_renderRect.bottom = 0;
}

bool CGraphicRenderTargetTexture::Create(const int width, const int height, const D3DFORMAT texFormat, const D3DFORMAT depthFormat)
{
	Reset();

	m_height = height;
	m_width = width;

	if (!CreateRenderTexture(width, height, texFormat))
		return false;


	if (!CreateRenderDepthStencil(width, height, depthFormat))
		return false;

	return true;
}

void CGraphicRenderTargetTexture::CreateTextures()
{
	if (CreateRenderTexture(m_width, m_height, m_d3dFormat))
	{
		CreateRenderDepthStencil(m_width, m_height, m_depthStencilFormat);
	}
}

bool CGraphicRenderTargetTexture::CreateRenderTexture(const int width, const int height, const D3DFORMAT format)
{
	m_d3dFormat = format;

	if (FAILED(ms_lpd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, m_d3dFormat, D3DPOOL_DEFAULT, &m_lpd3dRenderTexture)))
		return false;


	if (FAILED(m_lpd3dRenderTexture->GetSurfaceLevel(0, &m_lpd3dRenderTargetSurface)))
		return false;

	return true;
}

bool CGraphicRenderTargetTexture::CreateRenderDepthStencil(const int width, const int height, const D3DFORMAT format)
{
	m_depthStencilFormat = format;

	return (ms_lpd3dDevice->CreateDepthStencilSurface(width, height, m_depthStencilFormat, D3DMULTISAMPLE_NONE, &m_lpd3dDepthSurface)) == D3D_OK;
}

void CGraphicRenderTargetTexture::SetRenderTarget()
{
	ms_lpd3dDevice->GetRenderTarget(&m_lpd3dOriginalRenderTarget);
	ms_lpd3dDevice->GetDepthStencilSurface(&m_lpd3dOldDepthBufferSurface);
	ms_lpd3dDevice->SetRenderTarget(m_lpd3dRenderTargetSurface, m_lpd3dDepthSurface);
}

void CGraphicRenderTargetTexture::ResetRenderTarget()
{
	ms_lpd3dDevice->SetRenderTarget(m_lpd3dOriginalRenderTarget, m_lpd3dOldDepthBufferSurface);

	safe_release(m_lpd3dOriginalRenderTarget);
	safe_release(m_lpd3dOldDepthBufferSurface);
}

void CGraphicRenderTargetTexture::Clear()
{
	ms_lpd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
}

void CGraphicRenderTargetTexture::SetRenderingRect(RECT* rect)
{
	m_renderRect = *rect;
}

RECT* CGraphicRenderTargetTexture::GetRenderingRect()
{
	return &m_renderRect;
}

LPDIRECT3DTEXTURE8 CGraphicRenderTargetTexture::GetRenderTargetTexture() const
{
	return m_lpd3dRenderTexture;
}

void CGraphicRenderTargetTexture::Render() const
{
	D3DSURFACE_DESC desc;
	m_lpd3dRenderTargetSurface->GetDesc(&desc);


	const auto imgWidth = static_cast<float>(m_renderRect.right - m_renderRect.left);
	const auto imgHeight = static_cast<float>(m_renderRect.bottom - m_renderRect.top);

	const auto su = 0.0f;
	const auto sv = 0.0f;
	const auto eu = imgWidth / static_cast<float>(desc.Width);
	const auto ev = imgHeight / static_cast<float>(desc.Height);


	TPDTVertex pVertices[4];

	pVertices[0].position = TPosition(m_renderRect.left - 0.5f, m_renderRect.top - 0.5f, 0.0f);
	pVertices[0].texCoord = TTextureCoordinate(su, sv);
	pVertices[0].diffuse = 0xffffffff;


	pVertices[1].position = TPosition(m_renderRect.right - 0.5f, m_renderRect.top - 0.5f, 0.0f);
	pVertices[1].texCoord = TTextureCoordinate(eu, sv);
	pVertices[1].diffuse = 0xffffffff;

	pVertices[2].position = TPosition(m_renderRect.left - 0.5f, m_renderRect.bottom - 0.5f, 0.0f);
	pVertices[2].texCoord = TTextureCoordinate(su, ev);
	pVertices[2].diffuse = 0xffffffff;

	pVertices[3].position = TPosition(m_renderRect.right - 0.5f, m_renderRect.bottom - 0.5f, 0.0f);
	pVertices[3].texCoord = TTextureCoordinate(eu, ev);
	pVertices[3].diffuse = 0xffffffff;

	if (SetPDTStream(pVertices, 4))
	{
		SetDefaultIndexBuffer(DEFAULT_IB_FILL_RECT);

		STATEMANAGER.SetTexture(0, GetRenderTargetTexture());
		STATEMANAGER.SetTexture(1, nullptr);
		STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
		STATEMANAGER.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);
	}

}

void CGraphicRenderTargetTexture::Reset()
{
	Destroy();
	ReleaseTextures();

	m_d3dFormat = D3DFMT_UNKNOWN;
	m_depthStencilFormat = D3DFMT_UNKNOWN;
}

