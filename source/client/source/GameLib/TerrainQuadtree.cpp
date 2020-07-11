// TerrainQuadtreeNode.cpp: implementation of the CTerrainQuadtreeNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TerrainQuadtree.h"

//////////////////////////////////////////////////////////////////////
// CTerrainQuadtree
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// CTerrainQuadtreeNode
//////////////////////////////////////////////////////////////////////
CTerrainQuadtreeNode::CTerrainQuadtreeNode():NW_Node(nullptr), NE_Node(nullptr), SW_Node(nullptr), SE_Node(nullptr), center(-1.0f, -1.0f, -1.0f)
{
	x0 = y0 = x1 = y1 = 0;
	Size = 0;
	PatchNum = 0;
	radius = 0.0f;
	m_byLODLevel = 0;
}

CTerrainQuadtreeNode::~CTerrainQuadtreeNode()
{
	if (NW_Node)
	{
		delete NW_Node;
		NW_Node = nullptr;
	}
	if (NE_Node)
	{
		delete NE_Node;
		NE_Node = nullptr;
	}
	if (SW_Node)
	{
		delete SW_Node;
		SW_Node = nullptr;
	}
	if (SE_Node)
	{
		delete SE_Node;
		SE_Node = nullptr;
	}
}
