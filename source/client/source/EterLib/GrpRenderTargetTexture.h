#pragma once
#include "GrpBase.h"
#include "GrpTexture.h"

class CGraphicRenderTargetTexture : public CGraphicTexture
{
	public:
		CGraphicRenderTargetTexture();
		virtual	~CGraphicRenderTargetTexture();
	public:
		bool Create(int width, int height, D3DFORMAT texFormat, D3DFORMAT depthFormat);
		void CreateTextures();
		bool CreateRenderTexture(int width, int height, D3DFORMAT format);
		bool CreateRenderDepthStencil(int width, int height, D3DFORMAT format);
		void SetRenderTarget();
		void ResetRenderTarget();
		void SetRenderingRect(RECT* rect);
		RECT* GetRenderingRect();
		LPDIRECT3DTEXTURE8 GetRenderTargetTexture() const;
		void ReleaseTextures();
		static void Clear();
	
		void Render() const;
	
	protected:
		void Reset();
		LPDIRECT3DTEXTURE8 m_lpd3dRenderTexture{};
		LPDIRECT3DSURFACE8 m_lpd3dRenderTargetSurface{};
		LPDIRECT3DSURFACE8 m_lpd3dDepthSurface{};
	
		LPDIRECT3DSURFACE8 m_lpd3dOriginalRenderTarget{};
		LPDIRECT3DSURFACE8 m_lpd3dOldDepthBufferSurface{};
		D3DFORMAT m_d3dFormat;
		D3DFORMAT m_depthStencilFormat;
		RECT m_renderRect{};
};
