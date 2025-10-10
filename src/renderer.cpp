// src/renderer.cpp
#include "render/renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>


static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len; glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetShaderInfoLog(s, len, nullptr, &log[0]);
        std::cerr << "Shader compile error: " << log << std::endl;
    }
    return s;
}


static std::string readFile(const char* path) {
    std::ifstream in(path);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

static GLuint loadShaderProgramFromFiles(const char* vertPath, const char* fragPath) {
    std::string v = readFile(vertPath);
    std::string f = readFile(fragPath);
    GLuint vs = compileShader(GL_VERTEX_SHADER, v.c_str());
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, f.c_str());
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint ok; glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len; glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetProgramInfoLog(prog, len, nullptr, &log[0]);
        std::cerr << "Program link error: " << log << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

Renderer::Renderer(int width, int height)
    : m_width(width), m_height(height) {}

Renderer::~Renderer() {
    if (m_vboPoints) glDeleteBuffers(1, &m_vboPoints);
    if (m_vaoPoints) glDeleteVertexArrays(1, &m_vaoPoints);
    for (auto v : m_trailVBOs) glDeleteBuffers(1, &v);
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
}

void Renderer::init() {
    m_shaderProgram = loadShaderProgramFromFiles("shaders/orbit.vert", "shaders/orbit.frag");
    glGenVertexArrays(1, &m_vaoPoints);
    glGenBuffers(1, &m_vboPoints);

 
    glBindVertexArray(m_vaoPoints);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPoints);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 1024, nullptr, GL_DYNAMIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    
    glEnable(GL_PROGRAM_POINT_SIZE);
    float pointSize;
for (size_t i = 0; i < pts.size(); ++i) {
    if (i == 0)      pointSize = 20.0f;   // Sun
    else if (i == 1) pointSize = 10.0f;   // Earth
    else             pointSize = 6.0f;    // Moon
    glPointSize(pointSize);

    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
    glUniform3f(colorLoc, cols[i].r, cols[i].g, cols[i].b);
    glDrawArrays(GL_POINTS, (GLint)i, 1);
}
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   
    glViewport(0, 0, m_width, m_height);
}

void Renderer::setScale(double metersPerUnit) {
    m_scaleMetersPerUnit = metersPerUnit;
}

void Renderer::ensureTrailBuffers(size_t nBodies) {
    if (m_trails.size() == nBodies) return;

    for (auto v : m_trailVBOs) glDeleteBuffers(1, &v);
    m_trails.assign(nBodies, {});
    m_trailVBOs.assign(nBodies, 0);
    for (size_t i = 0; i < nBodies; ++i) {
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        m_trailVBOs[i] = vbo;
    }
}

void Renderer::uploadPoints(const std::vector<glm::vec2>& pts, GLuint vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec2), pts.data(), GL_DYNAMIC_DRAW);
}


static glm::vec2 worldToNDC(const glm::dvec2& worldPos, double scaleMetersPerUnit, int width, int height) {
    
    double x = worldPos.x / scaleMetersPerUnit;
    double y = worldPos.y / scaleMetersPerUnit;
    
    double ndcX = x / (double(width) * 0.5);
    double ndcY = y / (double(height) * 0.5);
    return glm::vec2((float)ndcX, (float)ndcY);
}

void Renderer::draw(const std::vector<Body>& solverBodies) {
    if (solverBodies.empty()) return;
    
    ensureTrailBuffers(solverBodies.size());

    for (size_t i = 0; i < solverBodies.size(); ++i) {
        glm::dvec2 wp = { solverBodies[i].position.x, solverBodies[i].position.y };
        glm::vec2 ndc = worldToNDC(wp, m_scaleMetersPerUnit, m_width, m_height);
        auto &trail = m_trails[i];
        trail.emplace_back(ndc);
        if (trail.size() > m_maxTrailLength) trail.erase(trail.begin());
    }


    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

 
    for (size_t i = 0; i < solverBodies.size(); ++i) {
        const auto &trail = m_trails[i];
        if (trail.size() < 2) continue;
      
        uploadPoints(trail, m_trailVBOs[i]);
  
        glBindBuffer(GL_ARRAY_BUFFER, m_trailVBOs[i]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

     
        GLint colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
        const auto &c = solverBodies[i].color;
        glUniform3f(colorLoc, c.r, c.g, c.b);

       
        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)trail.size());
    }

   
    std::vector<glm::vec2> pts;
    std::vector<glm::vec3> cols;
    pts.reserve(solverBodies.size());
    cols.reserve(solverBodies.size());
    for (const auto &b : solverBodies) {
        pts.emplace_back(worldToNDC({b.position.x, b.position.y}, m_scaleMetersPerUnit, m_width, m_height));
        cols.emplace_back(b.color);
    }


    glBindVertexArray(m_vaoPoints);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPoints);
    glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(glm::vec2), pts.data(), GL_DYNAMIC_DRAW);

   
    for (size_t i = 0; i < pts.size(); ++i) {
        GLint colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
        glUniform3f(colorLoc, cols[i].r, cols[i].g, cols[i].b);
        glDrawArrays(GL_POINTS, (GLint)i, 1);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}
