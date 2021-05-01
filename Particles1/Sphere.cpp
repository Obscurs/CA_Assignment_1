#include "sphere.h"
#include "Shader.h"
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Sphere::Sphere()
{
    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;

    lats = 40;
    longs = 40;
}

Sphere::~Sphere()
{

}

void Sphere::init(int c, float size)
{
    sphere_size = size;
    color = c;
    int i, j;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int indicator = 0;
    for (i = 0; i <= lats; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for (j = 0; j <= longs; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back(y * zr0 * size);
            vertices.push_back(x * zr0* size);
            vertices.push_back(z0 * size);

            glm::vec3 normal1(x * zr0, y * zr0, z0);
            //normal1 = glm::normalize(normal1);
            vertices.push_back(normal1.y);
            vertices.push_back(normal1.x);
            vertices.push_back(normal1.z);

            indices.push_back(indicator);
            indicator++;

            vertices.push_back(y * zr1 * size);
            vertices.push_back(x * zr1 * size);
            vertices.push_back(z1 * size);

            glm::vec3 normal2(x * zr1, y * zr1, z1);
            //normal2 = glm::normalize(normal2);
            vertices.push_back(normal2.y);
            vertices.push_back(normal2.x);
            vertices.push_back(normal2.z);
            
            indices.push_back(indicator);
            indicator++;
        }
        indices.push_back(1000000);
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vboVertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_vboIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    numsToDraw = indices.size();

    isInited = true;
}

void Sphere::cleanup()
{
    if (!isInited) {
        return;
    }
    if (m_vboVertex) {
        glDeleteBuffers(1, &m_vboVertex);
    }
    if (m_vboIndex) {
        glDeleteBuffers(1, &m_vboIndex);
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
    }

    isInited = false;
    m_vao = 0;
    m_vboVertex = 0;
    m_vboIndex = 0;
}

void Sphere::draw(Shader* ourShader)
{
    if (!isInited) {
        std::cout << "please call init() before draw()" << std::endl;
    }
    if(color==0)
        ourShader->setVec3("objectColor", 5.0f, 0.8f, 0.81f);
    else
        ourShader->setVec3("objectColor", 0.0f, 0.9f, 0.0f);
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, glm::vec3(posx, posy, posz));
    //float angle = 20.0f * i;
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    ourShader->setMat4("model", model);
    // draw sphere
    glBindVertexArray(m_vao);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(1000000);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
    glDrawElements(GL_TRIANGLE_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);

    
}
void Sphere::setNewPos(float newx, float newy, float newz)
{
    posx = newx;
    posy = newy;
    posz = newz;
}
glm::vec3 Sphere::getPosition() const
{
    return glm::vec3(posx, posy, posz);
}