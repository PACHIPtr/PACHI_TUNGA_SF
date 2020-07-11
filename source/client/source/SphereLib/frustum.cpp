#include "Stdafx.h"
#include "frustum.h"

ViewState Frustum::ViewVolumeTest (const Vector3d& c_v3Center, const float c_fRadius) const
{
	if (m_bUsingSphere)
	{
		D3DXVECTOR3 v (
			c_v3Center.x - m_v3Center.x,
			c_v3Center.y - m_v3Center.y,
			c_v3Center.z - m_v3Center.z
		);

		if
		(
			(c_fRadius + m_fRadius) *
			(c_fRadius + m_fRadius) <
			D3DXVec3LengthSq (&v)
		)
		{
			return VS_OUTSIDE;
		}
	}

	const int count = 6;

	D3DXVECTOR3 center = c_v3Center;

	int i;

	float distance[count];
	for (i = 0; i < count; i++)
	{
		distance[i] = D3DXPlaneDotCoord (&m_plane[i], &center);
		if (distance[i] <= -c_fRadius)
		{
			return VS_OUTSIDE;
		}
	}

	for (i = 0; i < count; i++)
	{
		if (distance[i] <= c_fRadius)
		{
			return VS_PARTIAL;
		}
	}

	return VS_INSIDE;
}

void Frustum::BuildViewFrustum (D3DXMATRIX& mat)
{
	m_bUsingSphere = false;
	m_plane[0] = D3DXPLANE (mat._13,           mat._23,           mat._33,           mat._43);
	m_plane[1] = D3DXPLANE (mat._14 - mat._13, mat._24 - mat._23, mat._34 - mat._33, mat._44 - mat._43);
	m_plane[2] = D3DXPLANE (mat._14 + mat._11, mat._24 + mat._21, mat._34 + mat._31, mat._44 + mat._41);
	m_plane[3] = D3DXPLANE (mat._14 - mat._11, mat._24 - mat._21, mat._34 - mat._31, mat._44 - mat._41);
	m_plane[4] = D3DXPLANE (mat._14 + mat._12, mat._24 + mat._22, mat._34 + mat._32, mat._44 + mat._42);
	m_plane[5] = D3DXPLANE (mat._14 - mat._12, mat._24 - mat._22, mat._34 - mat._32, mat._44 - mat._42);

	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize (&m_plane[i], &m_plane[i]);
	}
}

void Frustum::BuildViewFrustum2 (D3DXMATRIX& mat, float fNear, float fFar, float fFov, float fAspect, const D3DXVECTOR3& vCamera, const D3DXVECTOR3& vLook)
{
	float fViewLen = fFar - fNear;
	float fH = fViewLen * tan (fFov * 0.5f);
	float fW = fH * fAspect;
	D3DXVECTOR3 P (0.0f, 0.0f, fNear + fViewLen * 0.5f);
	D3DXVECTOR3 Q (fW, fH, fViewLen);
	D3DXVECTOR3 PQ = P - Q;
	m_fRadius = D3DXVec3Length (&PQ);
	m_v3Center = vCamera + vLook * (fNear + fViewLen * 0.5f);
	BuildViewFrustum (mat);
	m_bUsingSphere = true;
}