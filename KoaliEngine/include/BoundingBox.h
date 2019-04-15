#pragma once
#include"D3dUtility.h"

struct BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();
	BoundingBox(D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);

	bool isPointInside(D3DXVECTOR3& p);
	bool isCollided(BoundingBox* collision);		//collision �Ǳ���ײ������	
	bool ComputeBoundingBox(ID3DXMesh * XFileMesh, const D3DXVECTOR3 * p = nullptr);
	void initBoudingBox(LPDIRECT3DDEVICE9 pDevice);

	LPD3DXMESH getBoxMesh();
	void boudingBoxMove(D3DXVECTOR3 vec);
	D3DXVECTOR3* getBoudingBoxMin();
	D3DXVECTOR3* getBoudingBoxMax();

	float length;
	float width;
	float height;
	D3DXVECTOR3 _minPoint;
	D3DXVECTOR3 _maxPoint;
	D3DXVECTOR3 _posPoint;			//������λ�ã�һ����gameObjectλ����ͬ
	LPD3DXMESH pBoxMesh = 0;
	bool isShow = false;
};

