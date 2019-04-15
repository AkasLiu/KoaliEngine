#include "../include/Quad.h"

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

inline Quad::Quad(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	GameObject(MyTransform(position), pDevice)
{
}

Quad::Quad(float l, float w, D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) :
	length(l), width(w), GameObject(MyTransform(position), pDevice)
{
}

Quad::~Quad()
{
	d3d::Release<IDirect3DVertexBuffer9*>(vb);
}

void Quad::init()
{
	//�������㻺��
	pDevice->CreateVertexBuffer(
		6 * sizeof(Vertex),
		0,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&vb,
		0);
	vb->Lock(0, 0, (void**)&vertices, 0);

	//�������������Σ���Ϊ�������պ���������
	vertices[0] = Vertex(-length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[1] = Vertex(-length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex(length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	vertices[3] = Vertex(-length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[4] = Vertex(length / 2, 0.0f, width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[5] = Vertex(length / 2, 0.0f, -width / 2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	vb->Unlock();
}

void Quad::render()
{
	pDevice->SetTexture(0, pTexture);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	//�����������
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//�༶��������
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//�ظ�Ѱַģʽ
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	pDevice->SetTransform(D3DTS_WORLD, myTransform.getWorldTransform());

	pDevice->SetStreamSource(0, vb, 0, sizeof(Vertex));
	pDevice->SetFVF(Vertex::FVF);
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetTexture(0, 0);
}
