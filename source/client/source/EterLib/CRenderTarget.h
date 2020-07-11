#pragma once

#include <memory>
#include "GrpRenderTargetTexture.h"

class CInstanceBase;
class CGraphicImageInstance;

class CRenderTarget
{
	using TCharacterInstanceMap = std::map<DWORD, CInstanceBase*>;
	
	public:
		CRenderTarget(DWORD width, DWORD height);
		~CRenderTarget();


		void SetVisibility(bool isShow);
		void RenderTexture() const;
		void SetRenderingRect(RECT* rect) const;

		void SelectModel(DWORD index);
		bool CreateBackground(const char* imgPath, DWORD width, DWORD height);
		void RenderBackground() const;
		void UpdateModel();
		void DeformModel() const;
		void RenderModel() const;
		
		void SetScale(float x, float y, float z);
		
		void ChangeArmor(DWORD dwVnum);
		void ChangeWeapon(DWORD dwVnum);
		void ChangeHair(DWORD dwVnum);
		void ChangeSash(DWORD dwVnum);

		void CreateTextures() const;
		void ReleaseTextures() const;
	
	private:
		std::unique_ptr<CInstanceBase> m_pModel; 
		std::unique_ptr<CGraphicImageInstance> m_background;
		std::unique_ptr<CGraphicRenderTargetTexture> m_renderTargetTexture;
		float m_modelRotation;
		bool m_visible;
};
