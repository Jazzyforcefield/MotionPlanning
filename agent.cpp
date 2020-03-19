// Copyright 2020 Michael Ung

#include "agent.h"


Agent::Agent(glm::vec3 start, std::vector<Milestone *> path) {
  position_ = start;
  direction_ = glm::normalize(path[1]->position_ - path[0]->position_);
  speed_ = 5.f;
  radius_ = 0.5f;
  path_ = path;
  next_ = path[0];
  index_= 0;
}

Agent::~Agent() {

}

void Agent::update(float dt) {
  glm::vec3 diff = next_->position_ - position_;

  // Checks if at next milestone
  if (diff.x < 0.05 && diff.y < 0.05) {
    if (index_ == path_.size() - 1) {
      speed_ = 0;
      return;
    }
    
    index_++;
    next_ = path_[index_];
    direction_ = glm::normalize(next_->position_ - position_);
  }

  position_ = position_ + speed_ * direction_ * dt;
}