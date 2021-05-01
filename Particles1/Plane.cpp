#include "Shader.h"
#include "Plane.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
		-5.0f, -5.0f, -5.0f,  0.0f,  0.0f, 1.0f,
		 5.0f, -5.0f, -5.0f,  0.0f,  0.0f, 1.0f,
		 5.0f,  5.0f, -5.0f,  0.0f,  0.0f, 1.0f,
		 5.0f,  5.0f, -5.0f,  0.0f,  0.0f, 1.0f,
		-5.0f,  5.0f, -5.0f,  0.0f,  0.0f, 1.0f,
		-5.0f, -5.0f, -5.0f,  0.0f,  0.0f, 1.0f,

		-5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,
		 5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,
		 5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,
		 5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,
		-5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,
		-5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  -1.0f,

		-5.0f,  5.0f,  5.0f, 1.0f,  0.0f,  0.0f,
		-5.0f, -5.0f, -5.0f, 1.0f,  0.0f,  0.0f,
		-5.0f,  5.0f, -5.0f, 1.0f,  0.0f,  0.0f,
		-5.0f, -5.0f, -5.0f, 1.0f,  0.0f,  0.0f,
		-5.0f,  5.0f,  5.0f, 1.0f,  0.0f,  0.0f,
		-5.0f, -5.0f,  5.0f, 1.0f,  0.0f,  0.0f,

		 5.0f,  5.0f,  5.0f,  -1.0f,  0.0f,  0.0f,
		 5.0f,  5.0f, -5.0f,  -1.0f,  0.0f,  0.0f,
		 5.0f, -5.0f, -5.0f,  -1.0f,  0.0f,  0.0f,
		 5.0f, -5.0f, -5.0f,  -1.0f,  0.0f,  0.0f,
		 5.0f, -5.0f,  5.0f,  -1.0f,  0.0f,  0.0f,
		 5.0f,  5.0f,  5.0f,  -1.0f,  0.0f,  0.0f,

		-5.0f, -5.0f, -5.0f,  0.0f, 1.0f,  0.0f,
		 5.0f, -5.0f,  5.0f,  0.0f, 1.0f,  0.0f,
		 5.0f, -5.0f, -5.0f,  0.0f, 1.0f,  0.0f,
		 5.0f, -5.0f,  5.0f,  0.0f, 1.0f,  0.0f,
		-5.0f, -5.0f, -5.0f,  0.0f, 1.0f,  0.0f,
		-5.0f, -5.0f,  5.0f,  0.0f, 1.0f,  0.0f,

		-5.0f,  5.0f, -5.0f,  0.0f,  -1.0f,  0.0f,
		 5.0f,  5.0f, -5.0f,  0.0f,  -1.0f,  0.0f,
		 5.0f,  5.0f,  5.0f,  0.0f,  -1.0f,  0.0f,
		 5.0f,  5.0f,  5.0f,  0.0f,  -1.0f,  0.0f,
		-5.0f,  5.0f,  5.0f,  0.0f,  -1.0f,  0.0f,
		-5.0f,  5.0f, -5.0f,  0.0f,  -1.0f,  0.0f
};

Plane::Plane(int normalCode) : normal(0, 0, 0), d(0), point(0,0,0), size(10)
{
	switch (normalCode)
	{
	case(0):
		indexVertices = 0;
		break;
	case(1):
		indexVertices = 1;
		break;
	case(2):
		indexVertices = 4;
		break;
	case(3):
		indexVertices = 5;
		break;
	case(4):
		indexVertices = 2;
		break;
	case(5):
		indexVertices = 3;
		break;
	}

	computePlanePointAndNormal(indexVertices);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, &vertices[indexVertices * 6 * 6], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

Plane::~Plane()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}



void Plane::draw(Shader *ourShader)
{
	glBindVertexArray(VAO);
	ourShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
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


	
void Plane::computePlanePointAndNormal(int index)
{
	normal = glm::vec3(vertices[index * 6 * 6+3], vertices[index * 6 * 6 + 4], vertices[index * 6 * 6 + 5]);
	point = glm::vec3(vertices[index * 6 * 6], vertices[index * 6 * 6 + 1], vertices[index * 6 * 6 + 2]);
	d = -glm::dot(point, normal);

}

glm::vec3 Plane::closestPointInPlane(glm::vec3 q)
{
	float dist = distPlaneToPoint(q);
		return q - normal*dist;
}

float Plane::distPlaneToPoint(glm::vec3 q)
{
	float dist = glm::dot(normal, q) - glm::dot(normal, point);
	return dist = dist / glm::length(normal);
}


glm::vec3 Plane::entryPointSegmentPlane(glm::vec3 p, glm::vec3 q)
{
	float alfa;
	glm::vec3 r;
	alfa = (-glm::dot(normal, p) + glm::dot(normal, q - p) + glm::dot(normal, point)) / glm::dot(normal, q - p);
	r = p + (alfa-1)*(q - p);
	return r;
}
