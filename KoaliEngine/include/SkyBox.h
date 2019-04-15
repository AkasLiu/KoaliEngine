#pragma once
#include"D3dUtility.h"

//����һ����պе�FVF�����ʽ
struct SkyBoxVertex
{
	SkyBoxVertex() {}
	SkyBoxVertex(float x, float y, float z, float u, float v)
	{
		_x = x; _y = y; _z = z; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _u, _v;

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};

class SkyBox
{
public:
	SkyBox(float l, LPDIRECT3DDEVICE9 device);
	SkyBox(LPDIRECT3DDEVICE9 device);
	~SkyBox();
	BOOL init();
	BOOL LoadSkyTextureFromFile(LPCWSTR pFrontTextureFile, LPCWSTR pBackTextureFile, LPCWSTR pLeftTextureFile, LPCWSTR pRightTextureFile, LPCWSTR pTopTextureFile);
	void render(BOOL bRenderFrame);

private:
	LPDIRECT3DDEVICE9			pd3dDevice;				//D3D�豸����
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer;			//���㻺�����
	LPDIRECT3DTEXTURE9			pTexture[5];			//5���������ӿ�
	float						skyBoxLength;			//��պб߳�
	D3DXVECTOR3					position;				//��պ�λ�� Ĭ�ϣ�0,0,0��
	D3DXMATRIX					m_world;				
};

