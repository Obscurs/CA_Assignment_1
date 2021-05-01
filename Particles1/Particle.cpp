#include "Particle.h"
#include <iostream>
#include <math.h> 
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Particle::Particle()
//{
//}
const float OFFSET_COL_SPHERE = 0.025;
const float VERLET_K = 0.99;

Particle::Particle() :
m_currentPosition(0,0,0), m_previousPosition(0, 0, 0), m_previousPreviousPosition(0,0,0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false), m_mass(1), m_firstVerlet(true), m_hasColided(false)
{

}

Particle::Particle(const float& x, const float& y, const float& z) :
m_previousPosition(0, 0, 0), m_previousPreviousPosition(0, 0, 0), m_velocity(0.3, 0, 0.2), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false), m_mass(1), m_firstVerlet(true), m_hasColided(false)
{

	m_currentPosition.x = x;
	m_currentPosition.y = y;
	m_currentPosition.z = z;


}

void Particle::Init(float sizeBall)
{

	m_sphere.init(1, sizeBall);
	m_sphere.setNewPos(-0.25, -0.25, -0.25);
}
/*
Particle::Particle(glm::vec3 pos, glm::vec3 vel, float bouncing, bool fixed, int lifetime, glm::vec3 force) :
m_currentPosition(pos), m_previousPosition(pos), m_force(force), m_velocity(vel), m_bouncing(bouncing), m_lifetime(lifetime), m_fixed(fixed)
{
}
*/

Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_currentPosition = pos;
}
void Particle::setPosition(glm::vec3 pos)
{
	m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x = x; force.y = y; force.z = z;
	m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
	m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x += x; force.y += y; force.z += z;
	m_force = force;
}

void Particle::addForce(glm::vec3 force)
{
	m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel;
	vel.x = x; vel.y = y; vel.z = z;
	m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
	m_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
	m_fixed = fixed;
}

void Particle::ProcessKeyboard(int dir, float dt)
{
	switch (dir)
	{
	case 0:
		m_currentPosition.z -= dt;
		break;
	case 1:
		m_currentPosition.z += dt;
		break;
	case 2:
		m_currentPosition.x -= dt;
		break;
	case 3:
		m_currentPosition.x += dt;
		break;
	}
}
//getters
glm::vec3 Particle::getCurrentPosition()
{
	return m_currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
	return m_previousPosition;
}

glm::vec3 Particle::getForce()
{
	return m_force;
}

glm::vec3 Particle::getVelocity()
{
	return m_velocity;
}

float Particle::getBouncing()
{
	return m_bouncing;
}

float Particle::getLifetime()
{
	return m_lifetime;
}

const Sphere& Particle::getSphere()
{
	return m_sphere;
}








bool Particle::isFixed()
{
	return m_fixed;
}
void Particle::updateForce(const float& dt)
{
	m_force.x = 0.0;
	m_force.y = GRAVITY;
	m_force.z = 0.0;

}
void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	/*m_currentPosition.x = 0.142505;
	m_currentPosition.y = 0.949384;
	m_currentPosition.z = 0.0950038;
	return;*/
	updateForce(dt);
	//m_lifetime -= dt;
	if (m_fixed)
	{
		//m_currentPosition.x += sin(totalTime) *0.03;
		//m_currentPosition.z += cos(totalTime)*0.03;
	}
	if (!m_fixed)
	{
		switch (method)
		{
		case UpdateMethod::EulerOrig:
		{
			m_previousPosition = m_currentPosition;
			m_currentPosition += m_velocity*dt;
			m_velocity += m_force*dt;
		}
			break;
		case UpdateMethod::EulerSemi:
		{
			m_previousPosition = m_currentPosition;
			m_velocity += m_force*dt;
			m_currentPosition += m_velocity*dt;
		}
			break;
		case UpdateMethod::Verlet:
		{
			if (m_hasColided)
			{
				m_previousPreviousPosition = m_currentPosition - m_velocity*dt;
				m_hasColided = false;
			}
			else
			{
				m_previousPreviousPosition = m_previousPosition;
			}

			m_previousPosition = m_currentPosition;
			if (m_firstVerlet)
			{
				//m_previousPreviousPosition = (m_velocity * dt + m_force * dt) * (-1.0f) + m_currentPosition;
				m_previousPreviousPosition = m_currentPosition -(m_velocity * dt);
				m_firstVerlet = false;
			}
			m_currentPosition = m_previousPosition + VERLET_K * (m_previousPosition - m_previousPreviousPosition) + ((dt*dt) * (m_force)) / m_mass;
			m_velocity = (m_currentPosition - m_previousPosition) / dt;
		}
			break;
		}
	}
	return;
}




bool Particle::collisionParticlePlane(Plane &p){
	//float sign;
	//sign = glm::dot(m_currentPosition, p.normal) + p.d;
	//sign *= glm::dot(m_previousPosition, p.normal) + p.d;
	float dist_to_plane = p.distPlaneToPoint(m_currentPosition);
	return dist_to_plane < m_sphere.sphere_size;
	//std::cout << "D = " << p.d << std::endl;
	//if ( sign <= 0){
	//	return true;
	//}
	//return false;
}



void Particle::correctCollisionParticlePlain(Plane& p) {
	m_hasColided = true;
	//glm::vec3 ray(m_currentPosition - m_previousPosition);
	//float x = (glm::dot(p.normal, p.point) - glm::dot(p.normal, m_previousPosition)) / glm::dot(p.normal, normalize(ray));
	//glm::vec3 intersectPointPlane(m_previousPosition + normalize(ray) * x);
	//m_correctedPrev = intersectPointPlane;
	glm::vec3 direction = glm::normalize(m_currentPosition - m_previousPosition);
	float dist_traveled = glm::length(m_currentPosition - m_previousPosition);
	float dist_prev = p.distPlaneToPoint(m_previousPosition);
	float dist_curr = p.distPlaneToPoint(m_currentPosition);
	float dist_to_col = (dist_prev - m_sphere.sphere_size) / ((dist_prev - dist_curr) / dist_traveled);
	glm::vec3 sph_pos_col = m_previousPosition + direction * dist_to_col;
	glm::vec3 reflected = glm::reflect(direction, p.normal);
	m_currentPosition = sph_pos_col + m_bouncing * reflected * (dist_traveled - dist_to_col);
	//m_currentPosition = m_currentPosition - (1 + m_bouncing) * /*(glm::dot(m_currentPosition, p.normal) + p.d)*/ (m_sphere.sphere_size-p.distPlaneToPoint(m_currentPosition)) * p.normal;
	m_velocity = m_velocity - (1 + m_bouncing) * (glm::dot(m_velocity, p.normal)) * p.normal;

	//glm::vec3 rayToPrev(glm::normalize(intersectPointPlane - m_currentPosition));
	//m_correctedPrev = rayToPrev * (glm::length(ray)) + m_currentPosition;

}

bool Particle::collisionParticleTriangle(Triangle& p) {




	float dist_to_plane = p.distPlaneToPoint(m_currentPosition);
//Intersects plane
	if (dist_to_plane < m_sphere.sphere_size) {
		glm::vec3 ray(glm::normalize(m_currentPosition - p.normal * m_sphere.sphere_size - m_previousPosition- p.normal * m_sphere.sphere_size));


		if (glm::dot(p.normal, ray) == 0) {
			return false; // No intersection, the line is parallel to the plane
		}
		//float d = glm::dot(p.normal, p.point3);
		//float x = (d - glm::dot(p.normal, m_previousPosition)) / glm::dot(p.normal, ray);
		float x = (glm::dot(p.normal, p.point3) - glm::dot(p.normal, m_previousPosition - p.normal * m_sphere.sphere_size)) / glm::dot(p.normal, ray);
		glm::vec3 intersectPointPlane(m_previousPosition - p.normal * m_sphere.sphere_size + normalize(ray) * x);
		
		float areaTriangle = 0.5 * glm::length(glm::cross(p.point2- p.point1 ,p.point3- p.point1));
		float area1 = 0.5 * glm::length(glm::cross(p.point2- intersectPointPlane, p.point3 - intersectPointPlane));
		float area2 = 0.5 * glm::length(glm::cross(intersectPointPlane - p.point1, p.point3 - p.point1));
		float area3 = 0.5 * glm::length(glm::cross(p.point2 - p.point1, intersectPointPlane - p.point1));
		float res = area1 + area2 + area3 - areaTriangle;
		//std::cout << "intersect = " << intersectPointPlane.x << " " << intersectPointPlane.y << " " << intersectPointPlane.z << std::endl;
		//std::cout << "areas = " << area1 + area2 + area3 << " " << areaTriangle << std::endl;
		if (res <= 0.0001) return true;
	}
	return false;
}
void Particle::correctCollisionParticleTriangle(Triangle& p) {
	m_hasColided = true;

	glm::vec3 direction = glm::normalize(m_currentPosition - m_previousPosition);
	float dist_traveled = glm::length(m_currentPosition - m_previousPosition);
	float dist_prev = p.distPlaneToPoint(m_previousPosition);
	float dist_curr = p.distPlaneToPoint(m_currentPosition);
	float dist_to_col = (dist_prev - m_sphere.sphere_size) / ((dist_prev - dist_curr) / dist_traveled);
	glm::vec3 sph_pos_col = m_previousPosition + direction * dist_to_col;
	glm::vec3 reflected = glm::reflect(direction, p.normal);
	m_currentPosition = sph_pos_col + m_bouncing * reflected * (dist_traveled - dist_to_col);
	//m_currentPosition = m_currentPosition - (1 + m_bouncing) * /*(glm::dot(m_currentPosition, p.normal) + p.d)*/ (m_sphere.sphere_size-p.distPlaneToPoint(m_currentPosition)) * p.normal;
	m_velocity = m_velocity - (1 + m_bouncing) * (glm::dot(m_velocity, p.normal)) * p.normal;
}


bool Particle::collisionParticleSphere(const Sphere& p) {
	glm::vec3 spherePos = p.getPosition();
	glm::vec3 vecToSphere(spherePos - m_currentPosition);



	float magnitude = sqrt(vecToSphere.x* vecToSphere.x+ vecToSphere.y * vecToSphere.y+ vecToSphere.z * vecToSphere.z);
	
	return magnitude < p.sphere_size+ m_sphere.sphere_size;
}



void Particle::correctCollisionParticleSphere(const Sphere& p) {
	m_hasColided = true;
	glm::vec3 spherePos = p.getPosition();
	glm::vec3 vecToSphere(spherePos - m_currentPosition);
	glm::vec3 intersectionPoint;


	double cx = spherePos.x;
	double cy = spherePos.y;
	double cz = spherePos.z;

	double px = m_previousPosition.x;
	double py = m_previousPosition.y;
	double pz = m_previousPosition.z;

	double vx = m_currentPosition.x - px;
	double vy = m_currentPosition.y - py;
	double vz = m_currentPosition.z - pz;

	double A = vx * vx + vy * vy + vz * vz;
	double B = 2.0 * (px * vx + py * vy + pz * vz - vx * cx - vy * cy - vz * cz);
	double C = px * px - 2 * px * cx + cx * cx + py * py - 2 * py * cy + cy * cy +
		pz * pz - 2 * pz * cz + cz * cz - (p.sphere_size+ m_sphere.sphere_size) * (p.sphere_size+ m_sphere.sphere_size);

	// discriminant
	double D = B * B - 4 * A * C;

	if (D < 0)
	{
		std::cout << "Something bad has happened" << std::endl;
	}

	double t1 = (-B - sqrt(D)) / (2.0 * A);

	glm::vec3 solution1(m_previousPosition.x * (1 - t1) + t1 * m_currentPosition.x,
		m_previousPosition.y * (1 - t1) + t1 * m_currentPosition.y,
		m_previousPosition.z * (1 - t1) + t1 * m_currentPosition.z);
	if (D == 0)
	{
		intersectionPoint = solution1;
	}
	else
	{
		double t2 = (-B + sqrt(D)) / (2.0 * A);
		glm::vec3 solution2(m_previousPosition.x * (1 - t2) + t2 * m_currentPosition.x,
			m_previousPosition.y * (1 - t2) + t2 * m_currentPosition.y,
			m_previousPosition.z * (1 - t2) + t2 * m_currentPosition.z);

		// prefer a solution that's on the line segment itself

		if (abs(t1 - 0.5) < abs(t2 - 0.5))
		{
			intersectionPoint = solution1;
		}
		else
		{
			intersectionPoint = solution2;
		}
	}
	glm::vec3 sphereNormal = glm::normalize(intersectionPoint - spherePos);
	float circleD = -glm::dot(intersectionPoint, sphereNormal);

	m_currentPosition = m_currentPosition - (1 + m_bouncing) * (glm::dot(m_currentPosition, sphereNormal) + circleD) * sphereNormal;
	m_velocity = m_velocity - (1 + m_bouncing) * (glm::dot(m_velocity, sphereNormal)) * sphereNormal;
}
void Particle::draw(Shader* ourShader, bool wireframed)
{

	
	//m_sphere.setNewPos(-0.25, -0.25, -0.25);
	m_sphere.setNewPos(m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	m_sphere.draw(ourShader);

}
