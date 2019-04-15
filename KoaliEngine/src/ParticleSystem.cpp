#include "../include/ParticleSystem.h"

const DWORD ParticleVertex::FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE;

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

ParticleSystem::ParticleSystem()
{
	_vbSize = 2000;
	_vbOffset = 0;
	_vbBatchSize = 500;
}

ParticleSystem::~ParticleSystem()
{
	d3d::Release<IDirect3DTexture9 *>(_tex);
	d3d::Release<IDirect3DVertexBuffer9 *>(_vb);
}

bool ParticleSystem::init(IDirect3DDevice9* device, LPCTSTR texFileName)
{
	_device = device;
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(_device, texFileName, &_tex);
	if (FAILED(hr))
	{
		::MessageBoxA(NULL, "Create Texture Failed!", "ParticleSystem", 0);
		return false;
	}
	hr = _device->CreateVertexBuffer(_vbSize * sizeof(ParticleVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		ParticleVertex::FVF, D3DPOOL_DEFAULT, &_vb, NULL);
	if (FAILED(hr))
	{
		::MessageBoxA(NULL, "Create VertexBuffer Failed!", "ParticleSystem", 0);
		return false;
	}
	return true;
}

void ParticleSystem::reset()
{
	std::list<Particle>::iterator particleIter;
	for (particleIter = _particleList.begin(); particleIter != _particleList.end(); particleIter++)
	{
		resetParticle(&(*particleIter));
	}
}

void ParticleSystem::addParticle()
{
	Particle newParticle;
	resetParticle(&newParticle);
	_particleList.push_back(newParticle);
}

void ParticleSystem::preRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSIZE, FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	_device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// use alpha from texture
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
}

void ParticleSystem::render()
{
	//
	// Remarks:  The render method works by filling a section of the vertex buffer with data,
	//           then we render that section.  While that section is rendering we lock a new
	//           section and begin to fill that section.  Once that sections filled we render it.
	//           This process continues until all the particles have been drawn.  The benifit
	//           of this method is that we keep the video card and the CPU busy.  

	if (!_particleList.empty())
	{
		// set render states
		preRender();

		_device->SetTexture(0, _tex);
		_device->SetFVF(ParticleVertex::FVF);
		_device->SetStreamSource(0, _vb, 0, sizeof(ParticleVertex));

		// render batches one by one

		// start at beginning if we're at the end of the vb
		if (_vbOffset >= _vbSize)
			_vbOffset = 0;

		ParticleVertex* v = 0;

		_vb->Lock(
			_vbOffset * sizeof(ParticleVertex),
			_vbBatchSize * sizeof(ParticleVertex),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//
		// Until all particles have been rendered.
		//
		std::list<Particle>::iterator i;
		for (i = _particleList.begin(); i != _particleList.end(); i++)
		{
			if (i->_isAlive)
			{
				//
				// Copy a batch of the living particles to the
				// next vertex buffer segment
				//
				v->_position = i->_position;
				v->_size = i->_size;
				v->_color = (D3DCOLOR)i->_color;
				v++; // next element;

				numParticlesInBatch++; //increase batch counter

				// if this batch full?
				if (numParticlesInBatch == _vbBatchSize)
				{
					//
					// Draw the last batch of particles that was
					// copied to the vertex buffer. 
					//
					_vb->Unlock();

					_device->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					//
					// While that batch is drawing, start filling the
					// next batch with particles.
					//

					// move the offset to the start of the next batch
					_vbOffset += _vbBatchSize;

					// don't offset into memory thats outside the vb's range.
					// If we're at the end, start at the beginning.
					if (_vbOffset >= _vbSize)
						_vbOffset = 0;

					_vb->Lock(
						_vbOffset * sizeof(ParticleVertex),
						_vbBatchSize * sizeof(ParticleVertex),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // reset for new batch
				}
			}
		}

		_vb->Unlock();

		// its possible that the LAST batch being filled never 
		// got rendered because the condition 
		// (numParticlesInBatch == _vbBatchSize) would not have
		// been satisfied.  We draw the last partially filled batch now.

		if (numParticlesInBatch)
		{
			_device->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticlesInBatch);
		}

		// next block
		_vbOffset += _vbBatchSize;

		// reset render states
		postRender();
	}
}

void ParticleSystem::postRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

int ParticleSystem::getParticleNum()
{
	return (int)_particleList.size();
}
bool ParticleSystem::isDead()
{
	if (isEmpty())
		return true;

	std::list<Particle>::iterator i;
	for (i = _particleList.begin(); i != _particleList.end(); i++)
	{
		if (i->_isAlive)
			return false;
	}
	return true;
}

bool ParticleSystem::isEmpty()
{
	return _particleList.empty();
}

void ParticleSystem::removeDeadParticles()
{
	if (isEmpty())
		return;

	std::list<Particle>::iterator i;
	for (i = _particleList.begin(); i != _particleList.end(); i++)
	{
		if (i->_isAlive == false)
			_particleList.erase(i);
	}
}
