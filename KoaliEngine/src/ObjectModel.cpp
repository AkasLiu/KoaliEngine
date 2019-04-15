#include "../include/ObjectModel.h"

ObjectModel::ObjectModel(D3DXVECTOR3 pos, LPDIRECT3DDEVICE9 pDevice) : GameObject(MyTransform(pos), pDevice)
{

}

ObjectModel::~ObjectModel()
{
}

bool ObjectModel::loadXFile(LPCWSTR filePath)
{
	LPD3DXBUFFER mtrlBuffer = 0;

	HRESULT hr = D3DXLoadMeshFromX(
		filePath,
		D3DXMESH_MANAGED,
		pDevice,
		&ppAdjacency,
		&mtrlBuffer,
		0,
		&numMaterials,
		&pMesh
	);

	if (FAILED(hr))
	{
		::MessageBox(0, L"ObjectModel - loadXFile - FAILED", 0, 0);
		return false;
	}

	if (mtrlBuffer != 0 && numMaterials != 0)
	{
		D3DXMATERIAL *materials = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (unsigned int i = 0; i < numMaterials; i++)
		{
			materials[i].MatD3D.Ambient = materials[i].MatD3D.Diffuse;
			Mtrls.push_back(materials[i].MatD3D);

			if (materials[i].pTextureFilename != 0)
			{
				LPDIRECT3DTEXTURE9 tex = 0;
				//LPCWSTR textureFilePath = materials[i].pTextureFilename;

				//路径问题暂时无法解决
				D3DXCreateTextureFromFileA(
					pDevice,
					materials[i].pTextureFilename,
					&tex);

				Textures.push_back(tex);
			}
			else
			{
				Textures.push_back(nullptr);
			}
		}
	}

	d3d::Release<LPD3DXBUFFER>(mtrlBuffer);

	return true;
}

void ObjectModel::init()
{
}

void ObjectModel::render()
{
	pDevice->SetTransform(D3DTS_WORLD, myTransform.getWorldTransform());

	for (unsigned int i = 0; i < Mtrls.size(); ++i)
	{
		pDevice->SetMaterial(&Mtrls[i]);
		pDevice->SetTexture(0, Textures[i]);
		pMesh->DrawSubset(i);
	}

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetTexture(0, 0);
}
