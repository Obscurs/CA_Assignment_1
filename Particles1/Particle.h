#pragma once
#include <glm\glm.hpp>
#include "Plane.h"
#include "Triangle.h"
#include "Sphere.h"

//#define PARTICLE_SYS_ENABLED
#define HAIR_ENABLED
const float SPACING = 0.5;
class Particle
{
public:

	const float GRAVITY = -9.8f;
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z);
	void Init(float sizeBall);
	//Particle(glm::vec3 pos, glm::vec3 vel, float bouncing = 1.0f, bool fixed = false, int lifetime = -1, glm::vec3 force = glm::vec3(0, 0, 0));
	~Particle();
	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
	void setBouncing(float bouncing);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);
	void ProcessKeyboard(int dir, float dt);


	//getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();
	glm::vec3 getForce();
	glm::vec3 getVelocity();
	float getBouncing();
	float getLifetime();
	bool isFixed();

	//other
	void addForce(glm::vec3 force);
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::EulerOrig);
	bool collisionParticlePlane(Plane &p);
	void correctCollisionParticlePlain(Plane &p);
	bool collisionParticleTriangle(Triangle& p);
	void correctCollisionParticleTriangle(Triangle& p);
	bool collisionParticleSphere(const Sphere& p);
	void correctCollisionParticleSphere(const Sphere& p);
	void draw(Shader* ourShader, bool wireframed);
	const Sphere& getSphere();
	glm::vec3 m_velocity;

private:
	void updateForce(const float& dt);
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_previousPreviousPosition;
	glm::vec3 m_force;
	glm::vec3 m_correctedPrev;
	bool m_hasColided;
	bool m_firstVerlet;
	float m_mass;
	Sphere m_sphere;
	float m_bouncing;
	float m_lifetime;
	bool  m_fixed;

};

