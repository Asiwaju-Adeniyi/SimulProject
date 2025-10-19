#pragma once
#include <vector>
#include "body.hpp"
#include <glm/glm.hpp>

class Solver {
public:
    Solver(double timestep);

    void computeAccelerations();

    void addBody(const Body& body);
    void update();

    std::vector<Body>& getBodies();            
    const std::vector<Body>& getBodies() const;  

    glm::dvec3 getBarycenter() const;
    double totalEnergy() const;
    glm::dvec3 totalMomentum() const;

private:
    double G = 6.67430e-11;
    double dt;
    std::vector<Body> bodies;

   
};
