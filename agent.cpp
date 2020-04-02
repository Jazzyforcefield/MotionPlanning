// Copyright 2020 Michael Ung

#include <algorithm>

#include "agent.h"


Agent::Agent(glm::vec3 start, std::vector<Milestone *> path) {
  position_ = start;
  velocity_ = glm::normalize(path[1]->position_ - position_);
  force_ = direction_ * 1.f;
  radius_ = 0.5f;
  speed_ = 5.f;
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
  if (glm::length(diff) < 2.05f) {   // Naive smoothing
    if (index_ == path_.size() - 1) {
      velocity_ = glm::vec3();
      acceleration_ = glm::vec3();
      force_ = glm::vec3();
      return;
    }
    
    index_++;
    next_ = path_[index_];
    direction_ = glm::normalize(next_->position_ - position_);
    velocity_ = 1.f * glm::length(velocity_) * direction_;
  }

  position_ += velocity_ * dt;
  velocity_ += force_ * dt;
  if (glm::length(velocity_) < 0.05f && glm::length(force_) - glm::length(direction_) < 0.15f) {
    velocity_ = glm::vec3();
  }
  //force_ = direction_;
  calculate_forces(agents);
  if (glm::length(velocity_) > 5.f) {
    velocity_ = glm::normalize(velocity_) * 5.f;
  }
}

void Agent::calculate_forces(const std::vector<Agent *> & agents) {
  int size = agents.size();
  float boid_dist = 1.5f;
  std::vector<Agent *> nearby;

  for (int i = 0; i < size; i++) {
    float dist = glm::length(agents[i]->position_ - position_);
    if (dist < boid_dist && dist > 0) {
      nearby.push_back(agents[i]);
    }
  }

  int count = 0;
  glm::vec3 alignment_force = glm::vec3();//direction_;
  glm::vec3 cohesion_force = glm::vec3();
  glm::vec3 separation_force = glm::vec3();

  for (int i = 0; i < nearby.size(); i++) {
    alignment_force += nearby[i]->velocity_;
    count++;
  }

  if (count > 0) {
    alignment_force = alignment_force / ((float)count);
  } else {
    alignment_force = direction_;
  }

  if (glm::length(alignment_force) > 0 ) {
    alignment_force = glm::normalize(alignment_force);
    alignment_force = speed_ * alignment_force;
    alignment_force = alignment_force - velocity_;
  }

  count = 0;
  for (int i = 0; i < nearby.size(); i++) {
    cohesion_force += (nearby[i]->position_);
    count++;
  }

  if (count > 0) {
    cohesion_force = cohesion_force / ((float)count);
    cohesion_force = glm::normalize(cohesion_force - position_) * speed_ - velocity_;
  } else {
    cohesion_force = glm::vec3();
  }

  count = 0;
  for (int i = 0; i < nearby.size(); i++) {
    float dist = glm::length(position_ - nearby[i]->position_);
    if (dist > 0 && dist < 2.f * boid_dist - radius_) {
      separation_force += glm::normalize(position_ - nearby[i]->position_) / dist;
      count++;
    }
  }

  if (count > 0) {
    separation_force = separation_force / ((float)count);
  }

  if (glm::length(separation_force) > 0) {
    separation_force = glm::normalize(separation_force) * speed_ - velocity_;
  }

  std::cout << glm::length(alignment_force) << " " << glm::length(cohesion_force) << " "
            << glm::length(separation_force) << std::endl;

  force_ = direction_ + alignment_force + cohesion_force + separation_force;
}