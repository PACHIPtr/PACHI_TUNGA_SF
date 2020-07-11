#pragma once
#pragma warning (disable : 4786)

///////////////////////////////////////////////////////////////////////  
//	Include files

#include "SpeedTreeMaterial.h"
#include <speedtree/SpeedTreeRT.h>

#include <directx/d3d8.h>
#include <directx/d3d8types.h>
#include <directx/d3dx8.h>
#include <vector>
#include <memory>
#include <list>

#include "../eterLib/GrpObjectInstance.h"
#include "../eterLib/GrpImageInstance.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p) = nullptr; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; } }
#endif

///////////////////////////////////////////////////////////////////////  
//	class CSpeedTreeWrapper declaration
#pragma warning(push)
#pragma warning(disable:4100)

class CSpeedTreeWrapper : public CGraphicObjectInstance , public std::enable_shared_from_this<CSpeedTreeWrapper>
{
	typedef std::shared_ptr<CSpeedTreeWrapper> SpeedTreeWrapperPtr;
	enum
	{
		ID = TREE_OBJECT
	};
	int GetType() const { return ID; }
	
	// Collision Data
protected:
	virtual void OnUpdateCollisionData(const CStaticCollisionDataVector * pscdVector);
	virtual void OnUpdateHeighInstance(CAttributeInstance * pAttributeInstance) {}
	virtual bool OnGetObjectHeight(float fX, float fY, float * pfHeight) { return false; }
	// Bounding Sphere
public:
	virtual bool GetBoundingSphere(D3DXVECTOR3 & v3Center, float & fRadius);
	
public:
	static bool					ms_bSelfShadowOn;

public:
	// methods from CGraphicObjectInstance
	virtual void				SetPosition(float x, float y, float z);
	virtual void				CalculateBBox();

	virtual void				OnRender(); // Render 시에 메소드, 그러나 프리뷰나 특수한 경우에만 직접 Render 콜을 부르며 
											// 그 이외에는 RenderBranches, RenderFronds 등의 메소드를 CSpeedTreeForest에서 호출한다.
	virtual void				OnBlendRender() {}
	virtual void				OnRenderToShadowMap() {}
	virtual void				OnRenderShadow() {}
	virtual void				OnRenderPCBlocker();

public:
	CSpeedTreeWrapper();
	virtual	~CSpeedTreeWrapper();
	
	const float *				GetPosition();
	static void					SetVertexShaders(DWORD dwBranchVertexShader, DWORD dwLeafVertexShader);

	// geometry 
	bool                        LoadTree(const char * pszSptFile, const BYTE * c_pbBlock = nullptr, unsigned int uiBlockSize = 0, unsigned int nSeed = 1, float fSize = -1.0f, float fSizeVariance = -1.0f);
	const float *				GetBoundingBox(void) const						{ return m_afBoundingBox; }
	void						GetTreeSize(float & r_fSize, float & r_fVariance);
	UINT						GetCollisionObjectCount();
	void						GetCollisionObject(unsigned int nIndex, CSpeedTreeRT::ECollisionObjectType& eType, float* pPosition, float* pDimensions);

	// rendering
	void						SetupBranchForTreeType(void) const;
	void						SetupFrondForTreeType(void) const;
	void						SetupLeafForTreeType(void) const;
	void						EndLeafForTreeType(void);
	
#ifdef WRAPPER_USE_GPU_LEAF_PLACEMENT
	void						UploadLeafTables(unsigned int uiLocation) const;
#endif
	
	void						RenderBranches(void) const;
	void                        RenderFronds(void) const;
	void						RenderLeaves(void) const;
	void						RenderBillboards(void) const;
	
	// instancing
	std::list<SpeedTreeWrapperPtr>		GetInstances(unsigned int& nCount);
	SpeedTreeWrapperPtr			InstanceOf(void) const							{ return m_pInstanceOf; }
	SpeedTreeWrapperPtr 		MakeInstance();								
	void						DeleteInstance(SpeedTreeWrapperPtr pInstance);
	CSpeedTreeRT *				GetSpeedTree(void) const						{ return m_pSpeedTree; }
	
	// lighting																	
	const CSpeedTreeMaterial &	GetBranchMaterial(void) const					{ return m_cBranchMaterial; }
	const CSpeedTreeMaterial &	GetFrondMaterial(void) const					{ return m_cFrondMaterial; }
	const CSpeedTreeMaterial &	GetLeafMaterial(void) const						{ return m_cLeafMaterial; }
	float                       GetLeafLightingAdjustment(void) const			{ return m_pSpeedTree->GetLeafLightingAdjustment( ); }
	
	// wind																		
	void						SetWindStrength(float fStrength)				{ m_pSpeedTree->SetWindStrength(fStrength); }
	void						Advance(void);
	
	// utility
	LPDIRECT3DTEXTURE8			GetBranchTexture(void) const;
	void						CleanUpMemory(void);
	
private:
	void						SetupBuffers(void);
	void						SetupBranchBuffers(void);
	void						SetupFrondBuffers(void);
	void						SetupLeafBuffers(void);
	void						PositionTree(void) const;
	static bool					LoadTexture(const char* pFilename, CGraphicImageInstance & rImage);
	void						SetShaderConstants(const float* pMaterial) const;
	
	
private:
	// SpeedTreeRT data
	CSpeedTreeRT*					m_pSpeedTree;					// the SpeedTree object
	CSpeedTreeRT::STextures*		m_pTextureInfo;					// texture info cache
	bool							m_bIsInstance;					// is this an instance?
	std::vector<SpeedTreeWrapperPtr>	m_vInstances;					// what is an instance of us
	SpeedTreeWrapperPtr				m_pInstanceOf;					// which tree is this an instance of
	
	// geometry cache
	CSpeedTreeRT::SGeometry*		m_pGeometryCache;				// cache for pulling geometry from SpeedTree avoids lots of reallocation

	// branch buffers
	LPDIRECT3DVERTEXBUFFER8			m_pBranchVertexBuffer;			// branch vertex buffer
	unsigned int					m_unBranchVertexCount;			// number of vertices in branches
	LPDIRECT3DINDEXBUFFER8			m_pBranchIndexBuffer;			// branch index buffer
	unsigned short*					m_pBranchIndexCounts;			// number of indexes per branch LOD level
	
	// frond buffers
	LPDIRECT3DVERTEXBUFFER8			m_pFrondVertexBuffer;			// frond vertex buffer
	unsigned int					m_unFrondVertexCount;			// number of vertices in frond
	LPDIRECT3DINDEXBUFFER8			m_pFrondIndexBuffer;			// frond index buffer
	unsigned short*					m_pFrondIndexCounts;			// number of indexes per frond LOD level
	
	// leaf buffers
	unsigned short					m_usNumLeafLods;				// the number of leaf LODs
	LPDIRECT3DVERTEXBUFFER8*		m_pLeafVertexBuffer;			// leaf vertex buffer
	bool*							m_pLeavesUpdatedByCpu;			// stores which LOD's have been updated already per frame
	
	// tree properties
	float							m_afPos[3];						// tree position
	float							m_afBoundingBox[6];				// tree bounding box
	
	// materials
	CSpeedTreeMaterial				m_cBranchMaterial;				// branch material
	CSpeedTreeMaterial				m_cLeafMaterial;				// leaf material
	CSpeedTreeMaterial				m_cFrondMaterial;				// frond material
	
	// branch texture
	CGraphicImageInstance			m_BranchImageInstance;
	CGraphicImageInstance			m_ShadowImageInstance;			// shadow texture object (used if shadows are enabled)
	CGraphicImageInstance			m_CompositeImageInstance;

	static DWORD					ms_dwBranchVertexShader;
	static DWORD					ms_dwLeafVertexShader;
};

#pragma warning(pop)
