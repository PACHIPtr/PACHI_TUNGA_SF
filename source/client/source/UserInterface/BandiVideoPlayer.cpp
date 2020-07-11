#include "stdafx.h"
#include "BandiVideoPlayer.h"
#include "PythonApplication.h"
#define BANDILIB_ID  "YMIRGAMES_METIN2_20130903"
#define BANDILIB_KEY "8ea04df0"

/// D3DVERTEX structure
typedef struct _D3DVERTEX {						// D3D Vertex
	D3DVECTOR pos;
	FLOAT rhw;
	FLOAT tu, tv;
} D3DVERTEX;

#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

CBandiVideoPlayer::CBandiVideoPlayer(void)
{
	m_d3d8			= nullptr;
	m_d3d8_device	= nullptr;
	m_hWnd			= nullptr;
	m_texture		= nullptr;
	m_vertex_buffer = nullptr;
	m_vid_width		= m_vid_height	 = 0;
	m_tex_width		= m_tex_height	 = 0;
	m_wsize_width	= m_wsize_height = 0;
}

CBandiVideoPlayer::~CBandiVideoPlayer(void)
{
}

void CBandiVideoPlayer::VideoPlay(const char* szFilename)
{
	BaseInit();
	//bandilib init
	CBandiVideoLibrary bandilib;	
	if(FAILED(bandilib.Create(BANDIVIDEO_DLL_FILE_NAME, nullptr, nullptr)))
	{
#ifdef _DEBUG
		MessageBox(nullptr, "Error createing BandiVideoLibrary.", "Error", MB_OK | MB_ICONSTOP);
#endif
		return;
	}

	//certification
	if(FAILED(bandilib.Verify(BANDILIB_ID, BANDILIB_KEY)))
	{
		TraceError(" bandilib not certification !! ");
		return;
	}
	
		
	//videoFile Read
	if(FAILED(bandilib.Open(szFilename, FALSE)))
	{
#ifdef _DEBUG
		MessageBox(nullptr, "Error Opening file.....", "Error", MB_OK | MB_ICONSTOP);
#endif
		return; 
	}
	//VIDEO_INFO 
	BVL_VIDEO_INFO info;
	if(FAILED(bandilib.GetVideoInfo(info)))
	{
#ifdef _DEBUG
		MessageBox(nullptr, "Error getting video info....", "Error", MB_OK | MB_ICONSTOP);
#endif
		return;
	}
	if(FAILED(InitTexture(info.width, info.height)))
	{
#ifdef _DEBUG
		MessageBox(nullptr, "Error opening texture...", "Error", MB_OK | MB_ICONSTOP);
#endif
		return;
	}
	bandilib.Play();
	do
	{
		BVL_STATUS status;
		bandilib.GetStatus(status);

		if(status == BVL_STATUS_PLAYEND)
		{
			bandilib.Close();
			break;
		}
		if(bandilib.IsNextFrame())
		{
			D3DLOCKED_RECT	rc;
			BYTE*			buf;
			INT				pitch;

			m_texture->LockRect(0, &rc, nullptr, 0);
			buf = (BYTE*)rc.pBits;
			pitch = rc.Pitch;

			if(buf)
			{
				BVL_FRAME frame;
				frame.frame_buf = buf;
				frame.frame_buf_size = info.height * pitch;
				frame.pitch = pitch;
				frame.width = info.width;
				frame.height = info.height;
				frame.pixel_format = m_fmt_bvl;

				bandilib.GetFrame(frame, FALSE);

				m_texture->UnlockRect(NULL);

				//Show frame
				m_d3d8_device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
				m_d3d8_device->BeginScene();

				Draw(0,0,info.width, info.height);

				m_d3d8_device->EndScene();
				m_d3d8_device->Present(nullptr, nullptr, nullptr, nullptr);
			}
		}
		if(GetAsyncKeyState(VK_ESCAPE) & 0x0001 || GetAsyncKeyState(VK_LBUTTON) & 0x0001 || GetAsyncKeyState(VK_RBUTTON) & 0x0001 || GetAsyncKeyState(VK_SPACE) & 0x0001)
				break;

	}while(1);

	bandilib.Destroy();
	m_texture->Release();
}
HRESULT CBandiVideoPlayer::Draw(INT x, INT y, INT width, INT height)
{
	HRESULT hr;
	hr = m_d3d8_device->SetTexture(0, m_texture);
	if(FAILED(hr)) return hr;

	m_d3d8_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_d3d8_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_d3d8_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	RECT moveRect;
	CalMoveRect(width, height, moveRect);

	hr = SetVertices((float)moveRect.left, (float)moveRect.top, 
		(float)(moveRect.right - moveRect.left), (float)(moveRect.bottom - moveRect.top));

	return hr;
}
//비율 맞게 영상 rect 구하기.
void CBandiVideoPlayer::CalMoveRect(int srcWidth, int srcHeight, RECT& movieRect)
{
	RECT windowRect;
	POINT p;
	p.x = 0;
	p.y = 0;
	ClientToScreen(m_hWnd, &p);
	GetClientRect(m_hWnd, &windowRect);
	OffsetRect(&windowRect, p.x, p.y);

	int nMovieWidth, nMovieHeight;
	if (srcWidth >= srcHeight)
	{
		nMovieWidth = (windowRect.right - windowRect.left);
		nMovieHeight =  srcHeight * nMovieWidth / srcWidth;
		if( nMovieHeight > windowRect.bottom - windowRect.top )
			nMovieHeight = windowRect.bottom - windowRect.top;
	}
	else
	{
		nMovieHeight = (windowRect.bottom - windowRect.top);
		nMovieWidth =  srcWidth * nMovieHeight / srcHeight;
		if( nMovieWidth > windowRect.right - windowRect.left )
			nMovieWidth = windowRect.right - windowRect.left;
	}
	movieRect.left = windowRect.left + ((windowRect.right - windowRect.left) - nMovieWidth) / 2;
	movieRect.top = windowRect.top + ((windowRect.bottom - windowRect.top) - nMovieHeight) / 2;
	movieRect.right = movieRect.left + nMovieWidth;
	movieRect.bottom = movieRect.top + nMovieHeight;
}


HRESULT CBandiVideoPlayer::SetVertices(float scr_x, float scr_y, float scr_width, float scr_height)
{
	D3DVERTEX	vertices[4];

	// Set vertices
	vertices[0].pos.x = scr_x;
	vertices[0].pos.y = scr_y;
	vertices[0].tu    = 0.0f;
	vertices[0].tv    = 0.0f;

	vertices[1].pos.x = scr_x + scr_width;
	vertices[1].pos.y = scr_y;
	vertices[1].tu    = 1.0f * ((float)m_vid_width / m_tex_width);
	vertices[1].tv    = 0.0f;

	vertices[2].pos.x = scr_x + scr_width;
	vertices[2].pos.y = scr_y + scr_height;
	vertices[2].tu    = 1.0f * ((float)m_vid_width / m_tex_width);
	vertices[2].tv    = 1.0f * ((float)m_vid_height / m_tex_height);

	vertices[3].pos.x = scr_x;
	vertices[3].pos.y = scr_y + scr_height;
	vertices[3].tu    = 0.0f ;
	vertices[3].tv    = 1.0f * ((float)m_vid_height / m_tex_height);


	vertices[0].pos.z = vertices[1].pos.z = vertices[2].pos.z = vertices[3].pos.z = 0.0f;
	vertices[0].rhw   = vertices[1].rhw   = vertices[2].rhw   = vertices[3].rhw   = 1.0f;

	m_d3d8_device->SetVertexShader(D3DFVF_VERTEX);
	
	return m_d3d8_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(D3DVERTEX));
}

HRESULT CBandiVideoPlayer::BaseInit(void)
{
	m_hWnd			= CPythonApplication::Instance().GetWindowHandle();
	m_d3d8			= CPythonApplication::Instance().GetDirectx8();
	m_d3d8_device	= CPythonApplication::Instance().GetDevice();
	m_wsize_width	= CPythonApplication::Instance().GetWidth();
	m_wsize_height	= CPythonApplication::Instance().GetHeight();

	GetTextureFromat();

	return S_OK;
}
//cla texture size
int CBandiVideoPlayer::CalcTexSize(int vid_size, BOOL pow2)
{
	// Power-of-2 texture dimensions are required.
	D3DCAPS8 caps;
	m_d3d8_device->GetDeviceCaps(&caps);
	if(pow2 && caps.TextureCaps & D3DPTEXTURECAPS_POW2)
	{
		int tex_size = 16;
		while (tex_size < vid_size)
			tex_size = tex_size<<1;

		return tex_size;
	}
	
	return vid_size;
}
//create texture
HRESULT CBandiVideoPlayer::InitTexture(INT vid_width, INT vid_height)
{
	
	HRESULT hr;
	LPDIRECT3DTEXTURE8 texture = nullptr;
	BOOL pow2 = TRUE;
	
	D3DCAPS8 Caps;
	m_d3d8_device->GetDeviceCaps(&Caps);

	if(!!(Caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL))
	{
		pow2 = FALSE;
	}

	int tex_width  = CalcTexSize(vid_width, pow2);
	int tex_height = CalcTexSize(vid_height, pow2);


	hr = m_d3d8_device->CreateTexture(tex_width+1, tex_height+1, 1,D3DUSAGE_DYNAMIC, 
									m_fmt_d3d, D3DPOOL_DEFAULT, &texture);
	if(FAILED(hr))
	{
		hr = m_d3d8_device->CreateTexture(tex_width+1, tex_height+1, 1, 0, 
									m_fmt_d3d, D3DPOOL_MANAGED, &texture);
		if(FAILED(hr))
		{
			Beep(1000, 1000);
			ASSERT(0);
			return hr;
		}
	}

	D3DLOCKED_RECT locked_rect;

	if(SUCCEEDED(texture->LockRect(0, &locked_rect, nullptr, 0)))
	{
		BYTE* dest = (BYTE*) locked_rect.pBits;
		int byte = tex_width * m_pixel_size;

		for(int y = 0 ; y < tex_height; ++y)
		{
			memset(dest, 0, byte);
			dest += locked_rect.Pitch;
		}

		texture->UnlockRect(NULL);
	}

	m_texture    = texture;
	m_tex_width  = tex_width;
	m_tex_height = tex_height;
	m_vid_width  = vid_width;
	m_vid_height = vid_height;

	return S_OK;
}

//Get textureformat
BOOL CBandiVideoPlayer::GetTextureFromat(void)
{
	struct
	{
		D3DFORMAT			d3d_fmt;
		BVL_PIXEL_FORMAT	bvl_fmt;
		int					pixel_size;
	} fmt_list[] =
	{
		{ D3DFMT_X8R8G8B8,  BVLPF_X8R8G8B8,	4},
		{ D3DFMT_R8G8B8,    BVLPF_R8G8B8,	3},
		{ D3DFMT_R5G6B5,    BVLPF_R5G6B5,	2},
		{ D3DFMT_X1R5G5B5,  BVLPF_R5G5B5,	2},
	};

	D3DDISPLAYMODE d3ddm;
	HRESULT hr = m_d3d8->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, & d3ddm);
	if(SUCCEEDED(hr))
	{
		int fmt_count = sizeof(fmt_list)/sizeof(fmt_list[0]);
		for(int i = 0; i < fmt_count; i ++)
		{
			if(SUCCEEDED(m_d3d8->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, 0, D3DRTYPE_TEXTURE, fmt_list[i].d3d_fmt)))
			{
				m_fmt_d3d		= fmt_list[i].d3d_fmt;
				m_fmt_bvl		= fmt_list[i].bvl_fmt;
				m_pixel_size	= fmt_list[i].pixel_size;
				return TRUE;
			}
		}
	}

	m_fmt_d3d	 = D3DFMT_UNKNOWN;
	m_fmt_bvl	 = BVLPF_NULL;
	m_pixel_size = NULL;
 
	return FALSE;
}