// src/main.cpp
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "physics/solver.hpp"
#include "physics/body.hpp"
#include "utils/constants.hpp"
#include "render/renderer.hpp"

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


    Solver solver(8640.0);

    // Sun
    Body sun; sun.mass = Constants::massSun;
    sun.position = {0,0,0}; sun.velocity = {0,0,0}; sun.color = {1.0f,0.9f,0.3f};
    solver.addBody(sun);

    // Earth
    Body earth; earth.mass = Constants::massEarth;
    earth.position = {Constants::earthOrbitRadius / 100.0, 0, 0}; earth.velocity = {0,Constants::earthOrbitVelocity,0};
    earth.color = {0.2f,0.4f,1.0f}; solver.addBody(earth);

    // Moon
    Body moon; moon.mass = Constants::massMoon;
    moon.position = {earth.position.x + Constants::moonOrbitRadius / 100.0, 0, 0};
    moon.velocity = {0, Constants::earthOrbitVelocity + Constants::moonOrbitVelocity, 0};
    moon.color = {0.6f,0.6f,0.6f}; solver.addBody(moon);

    // Renderer
    Renderer renderer(width, height);
    renderer.init();
    renderer.setScale(5e8);  

    // Timing
    double lastTime = glfwGetTime();
    const double physicsStepsPerFrame = 1.0; 
   

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double dtSec = now - lastTime;
        lastTime = now;

    
        int steps = (int)physicsStepsPerFrame;
        for (int s = 0; s < steps; ++s) {
            solver.update();
        }

        
        renderer.draw(solver.getBodies());

       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
