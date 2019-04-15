#pragma once
#include"D3dUtility.h"

class KoaliTexture
{
public:
	KoaliTexture(void);
	~KoaliTexture(void);


protected:
	IDirect3DTexture9 *m_Texture;
	unsigned long      m_Width, m_Height;
	LPDIRECT3DDEVICE9  m_Device;
	ID3DXSprite *pSprite;

public:
	IDirect3DTexture9 *GetTextureCOM();

	BOOL Load(LPDIRECT3DDEVICE9  m_pDevice, LPCWSTR Filename, DWORD Transparent = 0, D3DFORMAT Format = D3DFMT_UNKNOWN);
	BOOL Create(LPDIRECT3DDEVICE9  m_Device, IDirect3DTexture9 *Texture);
	BOOL Free();

	BOOL      IsLoaded();

	long      GetWidth();
	long      GetHeight();
	D3DFORMAT GetFormat();

	BOOL Blit(long DestX, long DestY, \
		long SrcX = 0, long SrcY = 0, \
		long Width = 0, long Height = 0, \
		float XScale = 1.0f, float YScale = 1.0f, \
		D3DCOLOR Color = 0xFFFFFFFF);
};

