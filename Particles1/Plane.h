#pragma once
#include <glm\glm.hpp>
class Shader;
class Plane
{
public:
	Plane(int normalCode);
	~Plane();
	float size;
	int indexVertices;
	glm::vec3 point;
	glm::vec3 normal;
	unsigned int VBO;
	unsigned int VAO;
	float d;
	//setter
//	void setPlaneFrom3Points(Point p1, Point p2, Point p3);
	//functions
	float distPlaneToPoint(glm::vec3 q);
	void computePlanePointAndNormal(int index);
	void draw(Shader* ourShader);
	glm::vec3 closestPointInPlane(glm::vec3 q);
	glm::vec3 entryPointSegmentPlane(glm::vec3 p, glm::vec3 q);
};

