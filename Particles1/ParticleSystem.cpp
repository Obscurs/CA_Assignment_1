#include "ParticleSystem.h"
#include <random>
#include <iostream>

#define rand01() ((float)std::rand()/RAND_MAX)


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::setParticleSystem(int numParticles){
	m_particleSystem.clear();
	m_particleSystem.resize(numParticles);
	m_numParticles = numParticles;
}


Particle ParticleSystem::getParticle(int i){
	return m_particleSystem[i];
}

void ParticleSystem::iniParticleSystem(ParticleSystemType systemType, float sizeBall){
	m_currType = systemType;
	switch (systemType)
	{
		case ParticleSystemType::Waterfall:
		{							
 		for (int i = 0; i < m_numParticles; i++)
		   {
				m_particleSystem[i].Init(sizeBall);
			   m_particleSystem[i].setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
			   m_particleSystem[i].setVelocity((rand01() - 0.5), 0, 0.5*rand01());
			   m_particleSystem[i].setLifetime(rand01() * 10 + 5);
		   }
		}
		break;
		case ParticleSystemType::Fountain:
		{
           for (int i = 0; i < m_numParticles; i++)
		   {
			   m_particleSystem[i].Init(sizeBall);
			   m_particleSystem[i].setPosition(0, 0, 0);
			   m_particleSystem[i].setVelocity(2 * (rand01() - 0.5), 5, 2 * rand01());
			   m_particleSystem[i].setLifetime(rand01() * 10 + 5);
		   }
		}
		break;
	}

}




void ParticleSystem::updateParticleSystem(const float& dt, Particle::UpdateMethod method){

	for (int i = 0; i < m_numParticles; i++)
	{
		m_particleSystem[i].setForce(0.0f, -9.8f, 0.0f);
		m_particleSystem[i].updateParticle(dt, method);

		if (m_particleSystem[i].getLifetime() <0)
		{
			switch (m_currType)
			{
			case ParticleSystemType::Waterfall:
			{
				m_particleSystem[i].setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
				m_particleSystem[i].setVelocity((rand01() - 0.5), 0, 0.5 * rand01());
				m_particleSystem[i].setLifetime(rand01() * 10 + 5);
			}
			break;
			case ParticleSystemType::Fountain:
			{
				m_particleSystem[i].setPosition(0, 0, 0);
				m_particleSystem[i].setVelocity(2 * (rand01() - 0.5), 5, 2 * rand01());
				m_particleSystem[i].setLifetime(rand01() * 10 + 5);
			}
			break;
			}
		}
	}
}

void ParticleSystem::drawParticles(Shader* shader)
{
	for (int i = 0; i < m_numParticles; i++)
	{
		m_particleSystem[i].draw(shader, true);
	}
}


void ParticleSystem::checkColsPlane(Plane& p)
{
	for (int i = 0; i < m_numParticles; i++)
	{
		if(m_particleSystem[i].collisionParticlePlane(p))
			m_particleSystem[i].correctCollisionParticlePlain(p);
	}

}

void ParticleSystem::checkColsTriangle(Triangle& p)
{
	for (int i = 0; i < m_numParticles; i++)
	{
		if (m_particleSystem[i].collisionParticleTriangle(p))
			m_particleSystem[i].correctCollisionParticleTriangle(p);
	}
}
void ParticleSystem::checkColsSphere(const Sphere& p)
{
	for (int i = 0; i < m_numParticles; i++)
	{
		if (m_particleSystem[i].collisionParticleSphere(p))
			m_particleSystem[i].correctCollisionParticleSphere(p);
	}
}

void ParticleSystem::checkColsParticles()
{
	for (int i = 0; i < m_numParticles; i++)
	{
		bool been_corrected = true;
		//while (been_corrected)
		{
			//been_corrected = false;
			for (int j = 0; j < m_numParticles; ++j)
			{
				if (j != i)
				{
					if (m_particleSystem[i].collisionParticleSphere(m_particleSystem[j].getSphere()))
					{
						m_particleSystem[i].correctCollisionParticleSphere(m_particleSystem[j].getSphere());
						//been_corrected = true;
					}
						
				}
			}
		}
		
		
	}
}