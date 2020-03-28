// Copyright 2020 Michael Ung

#ifndef MILESTONE_H_
#define MILESTONE_H_

#include <vector>

#include "glm/vec3.hpp"
#include "sphere_obstacle.h"

class Milestone {
 public:
  // Method declarations
  Milestone();
  Milestone(glm::vec3 pos);
  ~Milestone();
  void update();
  float distance_between(Milestone * other);

  // Populates the neighbors_ vector with up to k-many neighbors when called
  // Will likely be changed to a spatial data structure instead of a vector later
  void populate_neighbors(const std::vector<Milestone *>& milestones,
                          const std::vector<Obstacle *>& obstacles,
                          int k = 1);

  // Prints out data
  void info();

  // Member variables
  std::vector<Milestone *> neighbors_;
  Milestone * previous_;
  float * connections_;
  int neighbors_size_;
  glm::vec3 position_;
};

#endif  // MILESTONE_H_