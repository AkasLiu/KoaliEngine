#pragma once
#include"ParticleSystem.h"

class SnowSystem : public ParticleSystem
{
public:
	SnowSystem(int numParticles);//will initialize _boundingBox with a minPoint = (0,0,0), maxPoint = (1,1,1)
	SnowSystem(BoundingBox* pBoundingBox, int numParticles);
	~SnowSystem();

	void resetBoundingBox(BoundingBox& boundingBox);
	void update(float timeDelta);
protected:
	void resetParticle(Particle* pParticle);
	BoundingBox _boundingBox;
private:
	float m_yVelocity;
	float m_xzVel_maxPoint;
	float m_yAcceleration;
	float m_xzAcc_maxPoint;
};

