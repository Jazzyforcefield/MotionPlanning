// Copyright 2020 Michael Ung

#include "agent.h"


Agent::Agent(glm::vec3 start, std::vector<Milestone *> path) {
  position_ = start;
  direction_ = glm::normalize(path[1]->position_ - position_);
  force_ = direction_ * 1.f;
  //speed_ = 5.f;
  radius_ = 0.5f;
  path_ = path;
  next_ = path[0];
  index_= 0;
}

Agent::~Agent() {

}

void Agent::update(float dt, const std::vector<Agent *> & agents) {
  glm::vec3 diff = next_->position_ - position_;
  direction_ = glm::normalize(diff);

  // Checks if at next milestone
  if (diff.x < 0.05 && diff.y < 0.05) {
    if (index_ == path_.size() - 1) {
      speed_ = 0;
      velocity_ = glm::vec3();
      acceleration_ = glm::vec3();
      force_ = glm::vec3();
      return;
    }
    
    index_++;
    next_ = path_[index_];
    direction_ = glm::normalize(next_->position_ - position_);
    velocity_ = 0.8f * glm::length(velocity_) * direction_;
  }

  position_ += velocity_ * dt;
  velocity_ += force_ * dt;
  force_ = direction_;
  calculate_forces(agents);
  //position_ = position_ + speed_ * direction_ * dt;
}

void Agent::calculate_forces(const std::vector<Agent *> & agents) {
  int size = agents.size();
  std::vector<Agent *> nearby;

  for (int i = 0; i < size; i++) {
    if (glm::length(agents[i]->position_ - position_) < 1.f) {
      nearby.push_back(agents[i]);
    }
  }

  glm::vec3 alignment_force = direction_;
  glm::vec3 cohesion_force = glm::vec3();
  glm::vec3 separation_force = glm::vec3();

  for (int i = 0; i < nearby.size(); i++) {
    alignment_force += nearby[i]->direction_;
  }

  alignment_force = alignment_force / ((float)nearby.size() + 1);

  for (int i = 0; i < nearby.size(); i++) {
    if (glm::length(nearby[i]->position_ - position_) > 2.f) {
      cohesion_force += nearby[i]->position_ - position_;
    } else {
      cohesion_force += nearby[i]->position_ - position_;
    }
  }

  //cohesion_force = cohesion_force / ((float)nearby.size());

  for (int i = 0; i < nearby.size(); i++) {
    if (1.f / glm::length(position_ - nearby[i]->position_) > 1.f) {
      separation_force += (position_ - nearby[i]->position_) - alignment_force / 2.f;
    } else {
      separation_force += 1.f / (position_ - nearby[i]->position_);
    }
  }

  //separation_force = separation_force / ((float)nearby.size());

  force_ = alignment_force + cohesion_force + separation_force;

}