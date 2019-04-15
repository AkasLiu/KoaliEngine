#pragma once
#include"GameObject.h"
#include"Vertex.h"

class Quad : public GameObject
{
public:
	Quad(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice);
	Quad(float l, float w, D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice);
	~Quad();

	void init();
	void render();

private:
	float length = 1.0f;
	float width = 1.0f;
	Vertex* vertices;
	IDirect3DVertexBuffer9* vb;
};

