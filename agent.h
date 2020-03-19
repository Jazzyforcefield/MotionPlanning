// Copyright 2020 Michael Ung

#include <vector>
#include <iostream>

#include "glm/glm.hpp"
#include "milestone.h"

#ifndef AGENT_H_
#define AGENT_H_

class Agent {
 public:
  Agent(glm::vec3 start, std::vector<Milestone *> path);
  ~Agent();
  void update(float dt);

  glm::vec3 position_;
  glm::vec3 direction_;
  float speed_;
  float radius_;
  std::vector<Milestone *> path_;
  int index_;
  Milestone * next_;
};

#endif  // AGENT_H_