#pragma once
#include"D3dUtility.h"
#include"Colors.h"
#include"BoundingBox.h"
#include<list>

extern float GetRandomFloat(float lowBound, float highBound);
extern  void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);

float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound > highBound)
		return lowBound;
	//generate a randon float number between 0 and 1
	float fNum = (rand() % 10000) * 0.0001f;
	//get a number between lowBound and highBound
	return (highBound - lowBound) * fNum + lowBound;
}

void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

//struct BoundingBox
//{
//	BoundingBox();
//	BoundingBox(D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint);
//
//	bool isPointInside(D3DXVECTOR3& p);
//
//	D3DXVECTOR3 _minPoint;
//	D3DXVECTOR3 _maxPoint;
//};
//
//BoundingBox::BoundingBox()
//{
//	_minPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	_maxPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//}
//
//BoundingBox::BoundingBox(D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint)
//{
//	_minPoint = minPoint;
//	_maxPoint = maxPoint;
//}
//
//bool BoundingBox::isPointInside(D3DXVECTOR3 &p)
//{
//	if (p.x >= _minPoint.x && p.y >= _minPoint.y && p.z >= _minPoint.z &&
//		p.x <= _maxPoint.x && p.y <= _maxPoint.y && p.z <= _maxPoint.z)
//		return true;
//	else
//		return false;
//}

using namespace std;

//
struct ParticleVertex
{
	D3DXVECTOR3 _position;
	FLOAT       _size;     //size of the particle
	D3DCOLOR    _color;
	static const DWORD FVF;//vertex format
};

//the structure of a particle are specific to what kind of particle system you want to modeling, not all the members
//are useful to a particular system, and some additional infomation may not be listed here. for example, "_velocity" and
//"_acceleration" are useless when modeling clouds.
struct Particle
{
	Particle()
	{
		_age = 0.0f;
		_isAlive = true;
	}

	FLOAT       _size;
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float       _lifeTime;     // how long the particle lives for before dying  
	float       _age;          // current age of the particle  
	D3DXCOLOR   _color;        // current color of the particle   
	D3DXCOLOR   _colorFade;    // how the color fades with respect to time
	bool        _isAlive;
};



class ParticleSystem
{
public:
	ParticleSystem(void);
	~ParticleSystem(void);

public:

	virtual bool init(IDirect3DDevice9* device, LPCTSTR texFileName);
	virtual void reset();//reset all the particles in the system
	virtual void addParticle();

	virtual void render();
	virtual void update(float timeDelta) = 0;//abstract method

	int getParticleNum();
	bool isEmpty();
	bool isDead();

protected:
	virtual void removeDeadParticles();
	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual void resetParticle(Particle* particle) = 0;//reset or init a particle
	virtual void preRender();
	virtual void postRender();

protected:
	IDirect3DDevice9*       _device;
	//		D3DXVECTOR3             _origin;
	//		float                   _emitRate;   // rate new particles are added to system
	float                   _size;       // size of particles
	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;
	std::list<Particle>     _particleList;
	//		int                     _maxPointParticles; // max allowed particles system can have

			//
			// Following three data elements used for rendering the p-system efficiently
			// initialized in derived class's constructor

	DWORD _vbSize;      // size of vb
	DWORD _vbOffset;    // offset in vb to lock   
	DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset
};
