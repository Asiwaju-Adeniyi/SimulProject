// src/solver.cpp
#include "physics/solver.hpp"
#include <cmath>

Solver::Solver(double timestep) : dt(timestep) {}

void Solver::addBody(const Body& body) {
    bodies.push_back(body);
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

std::vector<glm::dvec3> oldAccels(bodies.size());
for (size_t i=0; i<bodies.size(); ++i) oldAccels[i] = bodies[i].acceleration;


for (auto &b : bodies) {
    b.position += b.velocity * dt + 0.5 * b.acceleration * dt * dt;
}

    computeAccelerations();
    
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i].velocity += 0.5 * (oldAccels[i] + bodies[i].acceleration) * dt;
    }
}

glm::dvec3 Solver::getBarycenter() const {
    glm::dvec3 totalPos(0.0);
    double totalMass = 0.0;

    for (const auto& body : bodies) {
        totalPos += body.mass * body.position;
        totalMass += body.mass;
    }

    if (totalMass == 0.0) return {0.0, 0.0, 0.0};
    return totalPos / totalMass;
}

double Solver::totalEnergy() const {
    double KE = 0.0;
    double PE = 0.0;
    for (size_t i = 0; i < bodies.size(); ++i) {
        KE += 0.5 * bodies[i].mass * glm::dot(bodies[i].velocity, bodies[i].velocity);
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            double r = glm::length(bodies[i].position - bodies[j].position);
            PE -= G * bodies[i].mass * bodies[j].mass / r;
        }
    }
    return KE + PE;
}

glm::dvec3 Solver::totalMomentum() const {
    glm::dvec3 P(0.0);
    for (const auto& b : bodies)
        P += b.mass * b.velocity;
    return P;
}
std::vector<Body>& Solver::getBodies() {
    return bodies;
}

const std::vector<Body>& Solver::getBodies() const {
    return bodies;
}


