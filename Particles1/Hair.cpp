#include "Hair.h"
#include <math.h> 
Hair::Hair(): m_anchorX(0), m_anchorY(0), m_anchorZ(0)
{
	

}
void Hair::init(int numParticles, float xAnchor, float yAnchor, float zAnchor, bool isRobe, float spacing)
{
	m_isRobe = isRobe;
	m_anchorX = xAnchor;
	m_anchorY = yAnchor;
	m_anchorZ = zAnchor;
	
	if (isRobe)
	{
		m_particles.resize(numParticles* numParticles);
		Particle* p_prev = nullptr;
		for (int i = 0; i < numParticles; i++)
		{
			for (int j = 0; j < numParticles; ++j)
			{
				m_particles[i*numParticles + j].Init();
				m_particles[i * numParticles + j].setPosition(glm::vec3(m_anchorX + spacing * i, m_anchorY, m_anchorZ + spacing * j));
				m_particles[i * numParticles + j].setVelocity(0, 0, 0);

				if (i == 0)
				{
					m_particles[i * numParticles + j].setFixed(true);
				}
				else
				{
					m_particles[(i - 1) * numParticles + j].setP2d(&m_particles[i * numParticles + j]);
					m_particles[i*numParticles + j].setP1u(&m_particles[(i - 1) * numParticles + j]);
					if (i > 1)
					{
						m_particles[(i - 2) * numParticles + j].setBendd(&m_particles[i * numParticles + j]);
						m_particles[i * numParticles + j].setBendu(&m_particles[(i - 2) * numParticles + j]);
					}
					/*m_particles[(i - 1) * numParticles + j].setCurl1(&m_particles[i * numParticles + j]);
					m_particles[i * numParticles + j].setCurl3(&m_particles[(i - 1) * numParticles + j]);
					if (i > 1)
					{
						m_particles[(i - 2) * numParticles + j].setCurl2(&m_particles[i * numParticles + j]);
						m_particles[i * numParticles + j].setCurl4(&m_particles[(i - 2) * numParticles + j]);
					}*/
						
					

				}
				if (j == 0)
				{

				}
				else
				{
					m_particles[i * numParticles + j-1].setP4r(&m_particles[i * numParticles + j]);
					m_particles[i * numParticles + j].setP3l(&m_particles[i * numParticles + j - 1]);
					if (j > 1)
					{
						m_particles[i * numParticles + j - 2].setBendr(&m_particles[i * numParticles + j]);
						m_particles[i * numParticles + j].setBendl(&m_particles[i * numParticles + j - 2]);
					}
				}
				if (i > 0 && j > 0)
				{
					m_particles[(i - 1) * numParticles + j-1].setShear_ul(&m_particles[i * numParticles + j]);
					m_particles[i * numParticles + j].setShear_dr(&m_particles[(i - 1) * numParticles + j - 1]);
				}
				if (i < numParticles - 1 && j < numParticles - 1)
				{
					m_particles[(i + 1) * numParticles + j + 1].setShear_ur(&m_particles[i * numParticles + j]);
					m_particles[i * numParticles + j].setShear_dl(&m_particles[(i + 1) * numParticles + j +1]);
				}
				
			}
			
			
		}
	}
	else
	{
		m_particles.resize(numParticles);
		Particle* p_prev = nullptr;
		for (int i = 0; i < m_particles.size(); i++)
		{
			m_particles[i].Init();
			m_particles[i].setPosition(glm::vec3(m_anchorX, m_anchorY, m_anchorZ));
			m_particles[i].setVelocity(0, 0, 0);
			if (i == 0)
			{
				m_particles[i].setFixed(true);
			}
			else
			{
				m_particles[i - 1].setP2d(&m_particles[i]);
				m_particles[i].setP1u(&m_particles[i - 1]);
				/*m_particles[i - 1].setCurl1(&m_particles[i]);
				m_particles[i].setCurl3(&m_particles[i - 1]);
				if (i > 1)
				{
					m_particles[i - 2].setCurl2(&m_particles[i]);
					m_particles[i].setCurl4(&m_particles[i - 2]);
				}*/
			}
		}
	}
	
}
void Hair::update(float dt, Particle::UpdateMethod method)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		//m_particles[i].setForce(0.0f, -9.8f, 0.0f);
		m_particles[i].updateParticle(dt, method);

		
	}
}
void Hair::draw(Shader* ourShader)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles[i].draw(ourShader, true);
	}
}

void Hair::checkColsPlane(Plane& p)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i].collisionParticlePlane(p))
			m_particles[i].correctCollisionParticlePlain(p);
	}

}

void Hair::checkColsTriangle(Triangle& p)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i].collisionParticleTriangle(p))
			m_particles[i].correctCollisionParticleTriangle(p);
	}
}
void Hair::checkColsSphere(Sphere& p)
{
	
	for (int i = 0; i < m_particles.size(); i++)
	{
		if (m_particles[i].collisionParticleSphere(p))
			m_particles[i].correctCollisionParticleSphere(p);
	}
	
}