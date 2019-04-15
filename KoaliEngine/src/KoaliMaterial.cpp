#include "../include/KoaliMaterial.h"

KoaliMaterial::~KoaliMaterial(void)
{
}
KoaliMaterial::KoaliMaterial()
{
	// Set a default material (white)
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	SetDiffuseColor(255, 255, 0);
	SetAmbientColor(255, 255, 0);
	//SetSpecularColor(255,255,255);
	//SetEmissiveColor(0,0,0);
	//SetPower(10.0f);


	   //ZeroMemory( &m_Material, sizeof(D3DMATERIAL9) );
	  //m_Material.Diffuse.r = m_Material.Ambient.r = 1.0f;
	  //m_Material.Diffuse.g = m_Material.Ambient.g = 1.0f;
	  //m_Material.Diffuse.b = m_Material.Ambient.b =0.0f;
	  //m_Material.Diffuse.a = m_Material.Ambient.a = 1.0f;
}

KoaliMaterial::KoaliMaterial(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	SetDiffuseColor(d);
	SetAmbientColor(a);
	SetSpecularColor(s);
	SetEmissiveColor(e);
	SetPower(p);
}

BOOL KoaliMaterial::SetDiffuseColor(float Red, float Green, float Blue)
{
	m_Material.Diffuse.r = 1.0f / 255.0f * (float)Red;
	m_Material.Diffuse.g = 1.0f / 255.0f * (float)Green;
	m_Material.Diffuse.b = 1.0f / 255.0f * (float)Blue;

	return TRUE;
}

BOOL KoaliMaterial::GetDiffuseColor(float *Red, float *Green, float *Blue)
{
	if (Red != NULL)
		*Red = (float)(255.0f * m_Material.Diffuse.r);

	if (Green != NULL)
		*Green = (float)(255.0f * m_Material.Diffuse.g);

	if (Blue != NULL)
		*Blue = (float)(255.0f * m_Material.Diffuse.b);

	return TRUE;
}

BOOL KoaliMaterial::SetAmbientColor(float Red, float Green, float Blue)
{
	float hg = (float)Red;
	m_Material.Ambient.r = 1.0f / 255.0f * (float)Red;
	m_Material.Ambient.g = 1.0f / 255.0f * (float)Green;
	m_Material.Ambient.b = 1.0f / 255.0f * (float)Blue;

	return TRUE;
}

BOOL KoaliMaterial::GetAmbientColor(float *Red, float *Green, float *Blue)
{
	if (Red != NULL)
		*Red = (float)(255.0f * m_Material.Ambient.r);

	if (Green != NULL)
		*Green = (float)(255.0f * m_Material.Ambient.g);

	if (Blue != NULL)
		*Blue = (float)(255.0f * m_Material.Ambient.b);

	return TRUE;
}

BOOL KoaliMaterial::SetSpecularColor(float Red, float Green, float Blue)
{
	m_Material.Specular.r = 1.0f / 255.0f * (float)Red;
	m_Material.Specular.g = 1.0f / 255.0f * (float)Green;
	m_Material.Specular.b = 1.0f / 255.0f * (float)Blue;

	return TRUE;
}

BOOL KoaliMaterial::GetSpecularColor(float *Red, float *Green, float *Blue)
{
	if (Red != NULL)
		*Red = (float)(255.0f * m_Material.Specular.r);

	if (Green != NULL)
		*Green = (float)(255.0f * m_Material.Specular.g);

	if (Blue != NULL)
		*Blue = (float)(255.0f * m_Material.Specular.b);

	return TRUE;
}

BOOL KoaliMaterial::SetEmissiveColor(float Red, float Green, float Blue)
{
	m_Material.Emissive.r = 1.0f / 255.0f * (float)Red;
	m_Material.Emissive.g = 1.0f / 255.0f * (float)Green;
	m_Material.Emissive.b = 1.0f / 255.0f * (float)Blue;

	return TRUE;
}

BOOL KoaliMaterial::GetEmissiveColor(float *Red, float *Green, float *Blue)
{
	if (Red != NULL)
		*Red = (float)(255.0f * m_Material.Emissive.r);

	if (Green != NULL)
		*Green = (float)(255.0f * m_Material.Emissive.g);

	if (Blue != NULL)
		*Blue = (float)(255.0f * m_Material.Emissive.b);

	return TRUE;
}

BOOL KoaliMaterial::SetDiffuseColor(D3DXCOLOR d)
{
	m_Material.Diffuse = d;
	return true;
}

BOOL KoaliMaterial::GetDiffuseColor(D3DXCOLOR *d)
{
	*d = m_Material.Diffuse;
	return true;
}

BOOL KoaliMaterial::SetAmbientColor(D3DXCOLOR a)
{
	m_Material.Ambient = a;
	return true;
}

BOOL KoaliMaterial::GetAmbientColor(D3DXCOLOR * a)
{
	return 0;
}

BOOL KoaliMaterial::SetSpecularColor(D3DXCOLOR s)
{
	m_Material.Specular = s;
	return true;
}

BOOL KoaliMaterial::GetSpecularColor(D3DXCOLOR * s)
{
	return 0;
}

BOOL KoaliMaterial::SetEmissiveColor(D3DXCOLOR e)
{
	m_Material.Emissive = e;
	return true;
}

BOOL KoaliMaterial::GetEmissiveColor(D3DXCOLOR * e)
{
	return 0;
}

BOOL KoaliMaterial::SetPower(float Power)
{
	m_Material.Power = Power;
	return TRUE;
}

float KoaliMaterial::GetPower(float Power)
{
	return m_Material.Power;
}

D3DMATERIAL9 *KoaliMaterial::GetMaterial()
{
	return &m_Material;
}

