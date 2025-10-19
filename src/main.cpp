// src/main.cpp
#define GLM_ENABLE_EXPERIMENTAL  

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "physics/solver.hpp"
#include "physics/body.hpp"
#include "utils/constants.hpp"
#include "render/renderer.hpp"
#include <glm/gtx/string_cast.hpp>


int main() {
    if (!glfwInit()) return -1;
    // Request core profile if needed:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const int width = 1000, height = 1000;
    GLFWwindow* window = glfwCreateWindow(width, height, "N-Body (Vel Verlet)", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }


    Solver solver(3600.0);

    // Sun
    Body sun; sun.mass = Constants::massSun;
    sun.position = {0,0,0}; sun.velocity = {0,0,0}; sun.color = {1.0f,0.9f,0.3f};
    solver.addBody(sun);

    // Earth

    Body earth;
    earth.mass = Constants::massEarth;
    earth.position = {Constants::earthOrbitRadius, 0, 0};
    earth.velocity = {0, Constants::earthOrbitVelocity, 0};
    earth.color = {0.2f, 0.4f, 1.0f};
    solver.addBody(earth);
    
   

    // Moon
    Body moon;
    moon.mass = Constants::massMoon;
    moon.position = {earth.position.x + Constants::moonOrbitRadius, 0, 0};
    moon.velocity = {0, Constants::earthOrbitVelocity + Constants::moonOrbitVelocity, 0};
    moon.color = {0.6f, 0.6f, 0.6f};
    solver.addBody(moon);


    solver.computeAccelerations();   // Initialize accelerations


    // Renderer
    Renderer renderer(width, height);
    renderer.init();
    renderer.setScale(5.0e11);  

    // Timing
    double lastTime = glfwGetTime();
    const double physicsStepsPerFrame = 1.0; 

    glm::dvec3 totalMom(0.0);
    double totalMass = 0.0;
       for (auto& b : solver.getBodies()) {
    totalMom += b.mass * b.velocity;
    totalMass += b.mass;
     }
        glm::dvec3 vCOM = totalMom / totalMass;
        for (auto& b : solver.getBodies()) {
        b.velocity -= vCOM; 
        }

int frameCount = 0; 

while (!glfwWindowShouldClose(window)) {
    double now = glfwGetTime();
    double dtSec = now - lastTime;
    lastTime = now;

    int steps = (int)physicsStepsPerFrame;
    for (int s = 0; s < steps; ++s) {
        solver.update();
    }

    // 🔹 Log energy and momentum every 500 frames
    frameCount++;
    if (frameCount % 500 == 0) {
        std::cout << "Energy: " << solver.totalEnergy()
                  << " | Momentum: " << glm::to_string(solver.totalMomentum())
                  << std::endl;
    }

    // 🔹 Zoom toggle (Z key)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        renderer.setScale(2e8);   // close-up Earth–Moon
    } else {
        renderer.setScale(1.5e9); // full Solar view
    }

    renderer.draw(solver.getBodies());

    glfwSwapBuffers(window);
    glfwPollEvents();
}

glfwTerminate();
return 0;
}