// src/solver.cpp
#include "physics/solver.hpp"
#include <cmath>

Solver::Solver(double timestep) : dt(timestep) {}

void Solver::addBody(const Body& body) {
    bodies.push_back(body);
}

const std::vector<Body>& Solver::getBodies() const {
    return bodies;
}

void Solver::computeAccelerations() {
    for (auto& body : bodies) body.acceleration = {0.0, 0.0, 0.0};

    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = 0; j < bodies.size(); ++j) {
            if (i == j) continue;

            glm::dvec3 r = bodies[j].position - bodies[i].position;
            double distSqr = glm::dot(r, r); 
            double dist = sqrt(distSqr);
            glm::dvec3 forceDir = r / dist;

            bodies[i].acceleration += G * bodies[j].mass / distSqr * forceDir;
        }
    }
}

void Solver::update() {

    for (auto& body : bodies) {
        body.position += body.velocity * dt + 0.5 * body.acceleration * dt * dt;
    }

    std::vector<glm::dvec3> oldAccels;
    oldAccels.reserve(bodies.size());
    for (auto& body : bodies) oldAccels.push_back(body.acceleration);

    computeAccelerations();
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i].velocity += 0.5 * (oldAccels[i] + bodies[i].acceleration) * dt;
    }
}
