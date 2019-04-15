#pragma once
#include"D3dUtility.h"


class Light
{
public:

	D3DLIGHT9 *GetLight();

	BOOL SetType(D3DLIGHTTYPE Type);

	BOOL Move(float XPos, float YPos, float ZPos);
	BOOL MoveRel(float XPos, float YPos, float ZPos);
	BOOL GetPos(float *XPos, float *YPos, float *ZPos);

	BOOL Point(float XFrom, float YFrom, float ZFrom,
		float XAt, float YAt, float ZAt);
	BOOL GetDirection(float *XDir, float *YDir, float *ZDir);

	BOOL SetDiffuseColor(float Red, float Green, float Blue);
	BOOL GetDiffuseColor(float *Red, float *Green, float *Blue);

	BOOL SetSpecularColor(float Red, float Green, float Blue);
	BOOL GetSpecularColor(float *Red, float *Green, float *Blue);

	BOOL SetAmbientColor(float Red, float Green, float Blue);
	BOOL GetAmbientColor(float *Red, float *Green, float *Blue);

	BOOL SetRange(float Range);
	float GetRange();

	BOOL SetFalloff(float Falloff);
	float GetFalloff();

	BOOL SetAttenuation0(float Attenuation);
	float GetAttenuation0();

	BOOL SetAttenuation1(float Attenuation);
	float GetAttenuation1();

	BOOL SetAttenuation2(float Attenuation);
	float GetAttenuation2();

	BOOL SetTheta(float Theta);
	float GetTheta();

	BOOL SetPhi(float Phi);
	float GetPhi();
public:
	Light(void);
	~Light(void);

protected:
	D3DLIGHT9 m_Light;
};

