#include"../include/BoundingBox.h"

BoundingBox::BoundingBox()
{
	// infinite small 
	_minPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_maxPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	_posPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

BoundingBox::BoundingBox(D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
{
	_minPoint = minPoint;
	_maxPoint = maxPoint;
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::isPointInside(D3DXVECTOR3 &p)
{
	if (p.x >= _minPoint.x && p.y >= _minPoint.y && p.z >= _minPoint.z &&
		p.x <= _maxPoint.x && p.y <= _maxPoint.y && p.z <= _maxPoint.z)
		return true;
	else
		return false;
}

//
//碰撞检测
//
bool BoundingBox::isCollided(BoundingBox* collision)
{
	if (_minPoint.x <= collision->_maxPoint.x && _maxPoint.x >= collision->_minPoint.x &&
		//_minPoint.y <= collision->_maxPoint.y && _maxPoint.y >= collision->_minPoint.y &&
		_minPoint.z <= collision->_maxPoint.z && _maxPoint.z >= collision->_minPoint.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//计算出该物体的外接体
bool BoundingBox::ComputeBoundingBox(ID3DXMesh * XFileMesh, const D3DXVECTOR3 * p)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	XFileMesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		XFileMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(XFileMesh->GetFVF()),
		&this->_minPoint,
		&this->_maxPoint);

	XFileMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	_posPoint = *p;
	_minPoint += _posPoint;
	_maxPoint += _posPoint;

	return true;
}

void BoundingBox::initBoudingBox(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXCreateBox(
		pDevice,
		this->_maxPoint.x - this->_minPoint.x,
		this->_maxPoint.y - this->_minPoint.y,
		this->_maxPoint.z - this->_minPoint.z,
		&pBoxMesh,
		0);
}

LPD3DXMESH BoundingBox::getBoxMesh()
{
	return pBoxMesh;
}

//即时修改BoudingBox的_minPoint和_maxPoint
void BoundingBox::boudingBoxMove(D3DXVECTOR3 vec)
{
	_minPoint += vec;
	_maxPoint += vec;
	_posPoint += vec;
}

D3DXVECTOR3* BoundingBox::getBoudingBoxMin()
{
	return &_minPoint;
}

D3DXVECTOR3* BoundingBox::getBoudingBoxMax()
{
	return &_maxPoint;
}
