#include "../include/KoaliText.h"

KoaliText::KoaliText(void)
{
	Height = 50;
	Width = 30;
	Weight = FW_BOLD;
	SetRect(&rc, 100, 300, 0, 0);
	ppFont = NULL;

	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
}

KoaliText::~KoaliText(void)
{


}


void KoaliText::SetFont(INT iHeight, UINT iWidth, UINT iWeight)
{

	Height = iHeight;
	Width = iWidth;
	Weight = iWeight;

}


void KoaliText::SetColor(D3DXCOLOR iColor)
{
	Color = iColor;
}
void KoaliText::SetDrawRect(LPRECT pRect)
{
	rc = *pRect;
}

void KoaliText::init(LPDIRECT3DDEVICE9 pDevice)
{
	Device = pDevice;
	D3DXCreateFont(Device, Height, Width, Weight, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"System", &ppFont);
}

int KoaliText::DrawTheText(LPCTSTR pString)
{
	ppFont->DrawText(NULL, pString, -1, &rc, DT_NOCLIP, Color);

	return 0;
}

