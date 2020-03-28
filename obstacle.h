// Copyright 2020 Michael Ung

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include <iostream>

#include "glm/glm.hpp"

class Obstacle {
 public:
  Obstacle() {};
  Obstacle(glm::vec3 center) { position_ = center; };
  virtual ~Obstacle() {}

  // Checks if the given point is inside the obstacle
  virtual bool point_inside(glm::vec3 point, float agent_radius) = 0;

  // Checks if the line between the two given points insersects the obstacle
  virtual bool line_intersecting(glm::vec3 p1, glm::vec3 p2) = 0;

  glm::vec3 position_;
  glm::vec3 velocity_;
};

#endif  // OBSTACLE_H_