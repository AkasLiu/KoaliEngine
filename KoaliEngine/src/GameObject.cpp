#include "../include/GameObject.h"

GameObject::GameObject(MyTransform mytf, LPDIRECT3DDEVICE9 device)
{
	myTransform = mytf;
	pDevice = device;
	material = new KoaliMaterial(d3d::WHITE, d3d::WHITE, d3d::WHITE, d3d::BLACK, 2.0f);
}


GameObject::~GameObject()
{
}

void GameObject::GetTransform(MyTransform * mytf)
{
	*mytf = myTransform;
}

void GameObject::init()
{
}

void GameObject::render()
{
}

void GameObject::setMaterial(KoaliMaterial mtrl)
{
	*material = mtrl;
}

void GameObject::loadTexture(LPCWSTR filePath)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath, &pTexture);
	if (FAILED(hr))
	{
		::MessageBox(0, L"GameObject - LoadTexture - FAILED", 0, 0);
	}
}

void GameObject::move(D3DXVECTOR3 dis)
{
	myTransform.position += dis;
}
