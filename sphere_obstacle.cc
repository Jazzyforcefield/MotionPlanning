// Copyright 2020 Michael Ung

#include "sphere_obstacle.h"

SphereObstacle::SphereObstacle() : Obstacle() {
  radius_ = 1.f;
}

SphereObstacle::SphereObstacle(glm::vec3 center, float radius) : Obstacle(center) {
  radius_ = radius;
}

SphereObstacle::~SphereObstacle(){

}

bool SphereObstacle::point_inside(glm::vec3 point, float agent_radius) {
  if (glm::length(point - position_) > radius_ + agent_radius) {
    return false;
  } return true;
}

bool SphereObstacle::line_intersecting(glm::vec3 p1, glm::vec3 p2) {
  float a, b, c;
  a = 1.f;
  b = 2.f * glm::dot((p1 - position_), (p1 - p2));
  c = pow(glm::dot(p1 - position_, p1 - position_), 2) - pow(radius_, 2);

  std::cout << pow(b, 2) - 4.f * a *
                               c << std::endl;

  if (pow(b, 2) - 4.f * a * c >= 0) {
    return true;
  } return false;
}