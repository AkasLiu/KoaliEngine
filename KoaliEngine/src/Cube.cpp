#include "../include/Cube.h"

//Cube::Cube()
//{
//}


Cube::Cube(D3DXVECTOR3 position, LPDIRECT3DDEVICE9 pDevice) : GameObject(MyTransform(position), pDevice)
{
}

Cube::Cube(D3DXVECTOR3 position, D3DXVECTOR3 size, LPDIRECT3DDEVICE9 pDevice) :
	size(size), GameObject(MyTransform(position), pDevice)
{

}

Cube::~Cube()
{
}

void Cube::init()
{
	// save a ptr to the device

	pDevice->CreateVertexBuffer(
		24 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box

	// fill in the front face vertex data
	v[0] = Vertex(-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = Vertex(size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = Vertex(size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	// fill in the back face vertex data
	v[4] = Vertex(-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = Vertex(size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = Vertex(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// fill in the top face vertex data
	v[8] = Vertex(-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9] = Vertex(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = Vertex(size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the bottom face vertex data
	v[12] = Vertex(-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = Vertex(size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = Vertex(-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	v[16] = Vertex(-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = Vertex(-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = Vertex(-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = Vertex(-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// fill in the right face vertex data
	v[20] = Vertex(size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = Vertex(size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = Vertex(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = Vertex(size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	_vb->Unlock();

	pDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	_ib->Unlock();
}

void Cube::defalutInit()
{
	D3DXCreateBox(pDevice, size.x, size.y, size.z, &pMesh, NULL);
}

void Cube::render()
{
	//pDevice->SetTexture(0, pTexture);
	////pDevice->SetMaterial(material->GetMaterial());	
	//pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//pDevice->SetTransform(D3DTS_WORLD, myTransform.getWorldTransform());
	///*pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);*/

	//pMesh->DrawSubset(0);	

	//pDevice->SetRenderState(D3DRS_LIGHTING, true);

	//pDevice->SetTexture(0, pTexture);
	//pDevice->SetRenderState(D3DRS_LIGHTING, false);

	////线性纹理过滤
	//pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	////多级渐进纹理
	//pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	////重复寻址模式
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//pDevice->SetTransform(D3DTS_WORLD, myTransform.getWorldTransform());

	//pMesh->DrawSubset(0);

	//pDevice->SetRenderState(D3DRS_LIGHTING, true);
	//pDevice->SetTexture(0, 0);

	pDevice->SetTexture(0, pTexture);
	pDevice->SetMaterial(material->GetMaterial());	
	pDevice->SetTransform(D3DTS_WORLD, myTransform.getWorldTransform());

	pDevice->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	pDevice->SetIndices(_ib);
	pDevice->SetFVF(Vertex::FVF);
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		24,
		0,
		12);

	pDevice->SetTexture(0, 0);
}

