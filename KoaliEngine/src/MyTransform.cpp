#include "../include/MyTransform.h"

MyTransform::MyTransform(D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s): 
	position(p), rotation(r), scale(s){ }

MyTransform::MyTransform(D3DXVECTOR3 p) : position(p) { }

MyTransform::MyTransform()
{
}

MyTransform::~MyTransform()
{

}

void MyTransform::move(D3DXVECTOR3 p)
{
	position = p;
}

D3DXMATRIX* MyTransform::getWorldTransform()
{
	D3DXMATRIX *V, T, R, S;
	V = new D3DXMATRIX();
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	*V = T * S;
	return V;
}
