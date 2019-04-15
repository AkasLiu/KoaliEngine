#pragma once
#include"D3dUtility.h"

class MyTransform
{
public:
	MyTransform(D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s);
	MyTransform(D3DXVECTOR3 p);
	MyTransform();
	~MyTransform();

	void move(D3DXVECTOR3 p);
	//�任ҲҪд
	D3DXMATRIX* getWorldTransform();

public:
	D3DXVECTOR3 position = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rotation = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f };
};

