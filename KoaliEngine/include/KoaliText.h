#pragma once
#include"D3dUtility.h"

class KoaliText
{
public:
	KoaliText(void);
	~KoaliText(void);
	LPDIRECT3DDEVICE9  Device;
	INT Height;
	UINT Width;
	UINT Weight;
	RECT rc;
	D3DXCOLOR Color;

	LPD3DXFONT ppFont;

	void SetFont(INT iHeight, UINT iWidth, UINT iWeight);
	void SetColor(D3DXCOLOR iColor);
	void SetDrawRect(LPRECT pRect);

	void init(LPDIRECT3DDEVICE9 pDevice);
	int DrawTheText(LPCTSTR pString);
};

