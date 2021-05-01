#pragma once
#pragma once
#include <glm\glm.hpp>
class Shader;
class Triangle
{
public:
	Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~Triangle();
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
	glm::vec3 direc1;
	glm::vec3 direc2;
	glm::vec3 normal;
	float d;
	unsigned int VBO;
	unsigned int VAO;
	float buffer[18];
	void draw(Shader* ourShader);
	float distPlaneToPoint(glm::vec3 q);
private:
	void computeTriangleNormal();
	void generateBuffer();

};