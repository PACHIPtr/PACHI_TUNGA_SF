#pragma once
#include "BandiVideoLibrary.h"

struct BV_DEVICE_DX8
{
	LPDIRECT3D8				m_d3d8;
	LPDIRECT3DDEVICE8		m_d3d8_device;
	HWND					m_hWnd;
	LPDIRECT3DTEXTURE8		m_texture;
	LPDIRECT3DVERTEXBUFFER8	m_vertex_buffer;
	D3DFORMAT				m_fmt_d3d;
	BVL_PIXEL_FORMAT		m_fmt_bvl;
};

class CBandiVideoPlayer : public CSingleton<CBandiVideoPlayer>, public BV_DEVICE_DX8
{

private:
	int m_vid_width;
	int	m_vid_height;
	int m_tex_width;
	int	m_tex_height;
	int m_pixel_size;
	int m_wsize_width;
	int m_wsize_height;

public:
	HRESULT BaseInit(void);
	BOOL	GetTextureFromat(void);
	HRESULT InitTexture(INT vid_width, INT vid_height);
	int CalcTexSize(int vid_size, BOOL pow2);
	HRESULT Draw(INT x, INT y, INT width, INT height);
	HRESULT SetVertices(float scr_x, float scr_y, float scr_width, float scr_height);
	void	CalMoveRect(int srcWidth, int srcHeight, RECT& movieRect);

public:
	void VideoPlay(const char* szFilename);

public:
	CBandiVideoPlayer(void);
	~CBandiVideoPlayer(void);
};
