#pragma once
#include"D3dUtility.h"
#include"Colors.h"

class KoaliMaterial
{
public:
	KoaliMaterial(void);
	KoaliMaterial(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);
	~KoaliMaterial(void);

protected:
	D3DMATERIAL9  m_Material;

public:
	D3DMATERIAL9 *GetMaterial();

	BOOL SetDiffuseColor(float Red, float Green, float Blue);
	BOOL GetDiffuseColor(float *Red, float *Green, float *Blue);

	BOOL SetAmbientColor(float Red, float Green, float Blue);
	BOOL GetAmbientColor(float *Red, float *Green, float *Blue);

	BOOL SetSpecularColor(float Red, float Green, float Blue);
	BOOL GetSpecularColor(float *Red, float *Green, float *Blue);

	BOOL SetEmissiveColor(float Red, float Green, float Blue);
	BOOL GetEmissiveColor(float *Red, float *Green, float *Blue);

	BOOL SetDiffuseColor(D3DXCOLOR d);
	BOOL GetDiffuseColor(D3DXCOLOR *d);

	BOOL SetAmbientColor(D3DXCOLOR a);
	BOOL GetAmbientColor(D3DXCOLOR *a);

	BOOL SetSpecularColor(D3DXCOLOR s);
	BOOL GetSpecularColor(D3DXCOLOR *s);

	BOOL SetEmissiveColor(D3DXCOLOR e);
	BOOL GetEmissiveColor(D3DXCOLOR *e);

	BOOL SetPower(float Power);
	float GetPower(float Power);
};

