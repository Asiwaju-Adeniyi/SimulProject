
#pragma once
#include <glm/glm.hpp>  

struct Body {
    double mass;
    glm::dvec3 position;
    glm::dvec3 velocity;
    glm::dvec3 acceleration;
    glm::vec3 color;     
};
