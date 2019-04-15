/**********************************************************
 实现有Xfile的任务模型
**********************************************************/

#pragma once
#include"D3dUtility.h"
#include"GameObject.h"
#include<vector>

class ObjectModel : public GameObject
{
public:
	ObjectModel(D3DXVECTOR3 pos, LPDIRECT3DDEVICE9 pDevice);
	~ObjectModel();

	virtual bool loadXFile(LPCWSTR filePath);
	virtual void init();
	virtual void render();

private:
	LPD3DXBUFFER ppAdjacency;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<LPDIRECT3DTEXTURE9> Textures;
	DWORD numMaterials;
};

