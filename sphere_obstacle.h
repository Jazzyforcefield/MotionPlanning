// Copyright 2020 Michael Ung

#ifndef SPHERE_OBSTACLE_H_
#define SPHERE_OBSTACLE_H_

#include "obstacle.h"

class SphereObstacle : public Obstacle {
 public:
  SphereObstacle();
  SphereObstacle(glm::vec3 center, float radius);
  ~SphereObstacle();

  bool point_inside(glm::vec3 point, float agent_radius) override;
  bool line_intersecting(glm::vec3 p1, glm::vec3 p2) override;

  float radius_;
  float * vertices_;  // Unused atm, but will use later
};

#endif  // SPHERE_OBSTACLE_H_