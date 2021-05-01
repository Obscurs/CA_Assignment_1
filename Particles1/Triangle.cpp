#include "Shader.h"
#include "Triangle.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) : point1(p1), point2(p2), point3(p3)
{
	computeTriangleNormal();
	generateBuffer();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, buffer, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

Triangle::~Triangle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void Triangle::draw(Shader* ourShader)
{
	glBindVertexArray(VAO);
	ourShader->setVec3("objectColor", 0.0f, 0.5f, 0.31f);
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::translate(model, cubePositions[i]);
	//float angle = 20.0f * i;
	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	ourShader->setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
//void Plane::setPlaneFrom3Points(Point p1, Point p2, Point p3){
//	point = p1;
//	direc1 = p2.coord - p1.coord;
//	direc2 = p3.coord - p1.coord;
//	normal = glm::cross(direc1, direc2);
//	d = -glm::dot(point.coord, normal);
//}

float Triangle::distPlaneToPoint(glm::vec3 q)
{
	float dist = glm::dot(normal, q) - glm::dot(normal, point3);
	return dist = dist / glm::length(normal);
}

void Triangle::computeTriangleNormal()
{
	normal = glm::normalize(glm::cross(point1-point2, point2-point3));
	d = -glm::dot(point3, normal);
}
void Triangle::generateBuffer()
{
	buffer[0] = point1.x; buffer[1] = point1.y; buffer[2] = point1.z; buffer[3] = normal.x; buffer[4] = normal.y; buffer[5] = normal.z;
	buffer[6] = point2.x; buffer[7] = point2.y; buffer[8] = point2.z; buffer[9] = normal.x; buffer[10] = normal.y; buffer[11] = normal.z;
	buffer[12] = point3.x; buffer[13] = point3.y; buffer[14] = point3.z; buffer[15] = normal.x; buffer[16] = normal.y; buffer[17] = normal.z;
}

