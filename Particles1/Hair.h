#pragma once
#include <glm\glm.hpp>
#include "Particle.h"
#include "Plane.h"
#include "Triangle.h"
#include "Sphere.h"
#include <iostream>
#include <vector>
class Shader;

class Hair
{
public:
	Hair();
	void update(float dt, Particle::UpdateMethod method);
	void draw(Shader* ourShader);
	void init(int numParticles, float xAnchor, float yAnchor, float zAnchor, bool isRobe , float spacing);
	void checkColsPlane(Plane& p);
	void checkColsTriangle(Triangle& p);
	void checkColsSphere(Sphere& p);

private:

	std::vector<Particle> m_particles;
	float m_anchorX;
	float m_anchorY;
	float m_anchorZ;
	bool m_isRobe;
	
};