#pragma once
#include"MyTransform.h"
#include"MaterialsManager.h"
#include"BoundingBox.h"
#include<string>

class GameObject
{
public:
	GameObject(MyTransform mytf,LPDIRECT3DDEVICE9 device);
	~GameObject();

	void GetTransform(MyTransform* mytf);

	virtual void init();
	virtual void render();
	virtual void setMaterial(KoaliMaterial mtrl);
	virtual void loadTexture(LPCWSTR filePath);

	virtual void move(D3DXVECTOR3 dis);

protected:
	std::string objectName;
	MyTransform myTransform;
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXMESH pMesh;
	KoaliMaterial *material;
	LPDIRECT3DTEXTURE9 pTexture;
	BoundingBox *boundingBox;
};

