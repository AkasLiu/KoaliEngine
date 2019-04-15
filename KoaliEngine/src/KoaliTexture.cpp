#include "../include/KoaliTexture.h"

KoaliTexture::KoaliTexture()
{
	m_Device = NULL;
	m_Texture = NULL;
	m_Width = m_Height = 0;
}

KoaliTexture::~KoaliTexture()
{
	Free();
}

BOOL KoaliTexture::Load(LPDIRECT3DDEVICE9  m_pDevice, LPCWSTR Filename, DWORD Transparent, D3DFORMAT Format)
{
	Free();

	if ((m_Device = m_pDevice) == NULL)
		return FALSE;

	if (Filename == NULL)
		return FALSE;

	if (FAILED(D3DXCreateTextureFromFileEx(m_Device,
		Filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, Format, D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE,
		Transparent, NULL, NULL, &m_Texture)))
		return FALSE;

	m_Width = GetWidth();
	m_Height = GetHeight();

	return TRUE;
}

BOOL KoaliTexture::Create(LPDIRECT3DDEVICE9 m_pDevice, IDirect3DTexture9 *Texture)
{
	D3DLOCKED_RECT SrcRect, DestRect;
	D3DSURFACE_DESC Desc;

	Free();

	if ((m_Device = m_pDevice) == NULL)
		return FALSE;

	if (Texture == NULL)
		return TRUE;

	// Copy texture over
	Texture->GetLevelDesc(0, &Desc);
	m_Width = Desc.Width;
	m_Height = Desc.Height;
	m_Device->CreateTexture(m_Width, m_Height, 0, 0, Desc.Format, D3DPOOL_MANAGED, &m_Texture, NULL);

	Texture->LockRect(0, &SrcRect, NULL, D3DLOCK_READONLY);
	m_Texture->LockRect(0, &DestRect, NULL, 0);

	memcpy(DestRect.pBits, SrcRect.pBits, SrcRect.Pitch * m_Height);

	m_Texture->UnlockRect(0);
	Texture->UnlockRect(0);

	return TRUE;
}

BOOL KoaliTexture::Free()
{
	m_Texture = NULL;
	m_Device = NULL;
	m_Width = m_Height = 0;

	return TRUE;
}

BOOL KoaliTexture::IsLoaded()
{
	if (m_Texture == NULL)
		return FALSE;
	return TRUE;
}

IDirect3DTexture9 *KoaliTexture::GetTextureCOM()
{
	return m_Texture;
}

long KoaliTexture::GetWidth()
{
	D3DSURFACE_DESC d3dsd;

	if (m_Texture == NULL)
		return 0;

	if (FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
		return 0;

	return d3dsd.Width;
}

long KoaliTexture::GetHeight()
{
	D3DSURFACE_DESC d3dsd;

	if (m_Texture == NULL)
		return 0;

	if (FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
		return 0;

	return d3dsd.Height;
}

D3DFORMAT KoaliTexture::GetFormat()
{
	D3DSURFACE_DESC d3dsd;

	if (m_Texture == NULL)
		return D3DFMT_UNKNOWN;

	if (FAILED(m_Texture->GetLevelDesc(0, &d3dsd)))
		return D3DFMT_UNKNOWN;

	return d3dsd.Format;
}

BOOL KoaliTexture::Blit(long DestX, long DestY, \
	long SrcX, long SrcY, \
	long Width, long Height, \
	float XScale, float YScale, \
	D3DCOLOR Color)
{
	RECT Rect;

	if (m_Texture == NULL)
		return FALSE;
	if (m_Device == NULL)
		return FALSE;
	if (FAILED(D3DXCreateSprite(m_Device, &pSprite)))
		return FALSE;


	if (!Width)
		Width = m_Width;
	if (!Height)
		Height = m_Height;

	Rect.left = SrcX;
	Rect.top = SrcY;
	Rect.right = Rect.left + Width;
	Rect.bottom = Rect.top + Height;

	//if(FAILED(pSprite->Draw(m_Texture,                          \
    //       &Rect, &D3DXVECTOR2(XScale, YScale),                 \
    //       NULL, 0.0f,                                          \
    //       &D3DXVECTOR2((float)DestX, (float)DestY),            \
    //       Color)))
	return FALSE;
	return TRUE;
}
