
#pragma once
#include <vector>
#include "body.hpp"

class Solver {
public:
    Solver(double timestep);
    void addBody(const Body& body);
    void update();  // advances the simulation by one timestep
    const std::vector<Body>& getBodies() const;

private:
    double G = 6.67430e-11; // gravitational constant
    double dt;
    std::vector<Body> bodies;

    void computeAccelerations();
};
