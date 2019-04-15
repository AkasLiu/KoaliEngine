#pragma once
#include"GameObject.h"
#include"Vertex.h"

class Cube : public GameObject
{
public:
	Cube(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice);
	Cube(D3DXVECTOR3 position, D3DXVECTOR3 size, LPDIRECT3DDEVICE9 pDevice);
	~Cube();

public:
	void init();
	void defalutInit();
	void render();

private:
	D3DXVECTOR3 size = { 1.0f,1.0f,1.0f };

	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;
};

