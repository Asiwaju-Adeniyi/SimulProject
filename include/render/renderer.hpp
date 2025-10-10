#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "physics/body.hpp"

class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

 
    void init();

 
    void setScale(double metersPerUnit);


    void draw(const std::vector<Body>& solverBodies);


    void resize(int width, int height);

private:
    void uploadPoints(const std::vector<glm::vec2>& pts, GLuint vbo);
    void ensureTrailBuffers(size_t nBodies);

    int m_width, m_height;
    double m_scaleMetersPerUnit = 1.0e9; 
    GLuint m_shaderProgram = 0;

    GLuint m_vaoPoints = 0;
    GLuint m_vboPoints = 0;

    std::vector<std::vector<glm::vec2>> m_trails;
    std::vector<GLuint> m_trailVBOs;
    size_t m_maxTrailLength = 1024;
};
