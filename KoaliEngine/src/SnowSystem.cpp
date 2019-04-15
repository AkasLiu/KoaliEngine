#include "../include/SnowSystem.h"

float GetRandomFloat(float lowBound, float highBound);
void GetRandomVector(D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);
SnowSystem::SnowSystem(int numParticles) :ParticleSystem()
{
	_size = 0.15f;

	m_yVelocity = -0.25f;
	m_xzVel_maxPoint = 0.5f;
	m_yAcceleration = -0.98f;
	m_xzAcc_maxPoint = 5.0f;

	_boundingBox._maxPoint = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	_boundingBox._minPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

SnowSystem::SnowSystem(BoundingBox* pBoundingBox, int numParticles) :ParticleSystem()
{
	_size = 0.15f;

	m_yVelocity = -0.25f;
	m_xzVel_maxPoint = 0.5f;
	m_yAcceleration = -0.98f;
	m_xzAcc_maxPoint = 5.0f;

	_boundingBox._maxPoint = pBoundingBox->_maxPoint;
	_boundingBox._minPoint = pBoundingBox->_minPoint;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

SnowSystem::~SnowSystem()
{

}

void SnowSystem::resetBoundingBox(BoundingBox& boundingBox)
{
	_boundingBox = boundingBox;
}

void SnowSystem::resetParticle(Particle* pParticle)
{
	pParticle->_isAlive = true;
	//init snow's position
	GetRandomVector(&pParticle->_position, &_boundingBox._minPoint, &_boundingBox._maxPoint);
	pParticle->_position.y = _boundingBox._maxPoint.y;

	//init snow's size and color
	pParticle->_size = _size;
	pParticle->_color = d3d::WHITE;

	//init snow's velocity
	pParticle->_velocity.x = GetRandomFloat(-m_xzVel_maxPoint, m_xzVel_maxPoint);
	pParticle->_velocity.z = GetRandomFloat(-m_xzVel_maxPoint, m_xzVel_maxPoint);
	pParticle->_velocity.y = m_yVelocity;

	//init snow's acceleration
	pParticle->_acceleration.x = GetRandomFloat(-m_xzAcc_maxPoint, m_xzAcc_maxPoint);
	pParticle->_acceleration.z = GetRandomFloat(-m_xzAcc_maxPoint, m_xzAcc_maxPoint);
	pParticle->_acceleration.y = m_yAcceleration;
}

void SnowSystem::update(float timeDelta)
{
	std::list<Particle>::iterator i;
	//update every snow particle in the particleList
	for (i = _particleList.begin(); i != _particleList.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;
		i->_velocity += i->_acceleration * timeDelta;

		//update the snow particle's acceleration
		i->_acceleration.x = GetRandomFloat(-m_xzAcc_maxPoint, m_xzAcc_maxPoint);
		i->_acceleration.z = GetRandomFloat(-m_xzAcc_maxPoint, m_xzAcc_maxPoint);
		i->_acceleration.y = m_yAcceleration;

		//update the snow particle's _isAlive attribute
//		i->_isAlive = _boundingBox.isPointInside( i->_position );

		//reset particle that's dead
		if (!_boundingBox.isPointInside(i->_position))
			resetParticle(&(*i));
	}

}
