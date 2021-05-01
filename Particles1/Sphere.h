#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader;
class Sphere
{
public:
    Sphere();
    ~Sphere();
    void init(int color, float size);
    void cleanup();
    void draw(Shader* ourShader);
    void setNewPos(float newx, float newy, float newz);
    glm::vec3 getPosition() const;
    float sphere_size;
private:
    int color;
    int lats, longs;
    float posx, posy, posz;
    bool isInited;
    unsigned int m_vao, m_vboVertex, m_vboIndex;
    int numsToDraw;
};

#endif // SPHERE_H