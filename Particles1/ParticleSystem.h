#pragma once
#include "Particle.h"
#include <vector>
#include "Plane.h"
#include "Triangle.h"
#include "Sphere.h"
class ParticleSystem
{
public:
	enum class ParticleSystemType : std::int8_t { Fountain, Waterfall };
	ParticleSystem();
	~ParticleSystem();
	void setParticleSystem(int numParticles);
	Particle getParticle(int i);
	void iniParticleSystem(ParticleSystemType systemType, float sizeBall);
	void updateParticleSystem(const float& dt, Particle::UpdateMethod method);
	void checkColsPlane(Plane& p);
	void checkColsTriangle(Triangle& p);
	void checkColsSphere(const Sphere& p);
	void drawParticles(Shader* shader);
	void checkColsParticles();
private:
	int m_numParticles;
	ParticleSystemType m_currType;
	std::vector<Particle> m_particleSystem;
};

