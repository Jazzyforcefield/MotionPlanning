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

void Agent::update(float dt, const std::vector<Agent *> & agents,
                   const std::vector<Obstacle *> & obstacles) {
  glm::vec3 diff = next_->position_ - position_;
  direction_ = glm::normalize(diff);

  int osize = obstacles.size();
  bool broken = false;
  for (int i = 0; i < osize; i++) {
    if (index_ < path_.size() - 1)
      broken = obstacles[i]->line_intersecting(position_, path_[index_ + 1]->position_, radius_);

    if (broken) break;
  }

  // Hooke's Law constants
  int k = 3;
  int kv = 5;

  // Checks if at next milestone
  if (!broken) {
    if (index_ >= path_.size() - 1 && glm::length(diff) < 1.15f) {
      // Hacky way to slow down and still take into account cohesion/separation
      force_ = (float)-k * -diff + (float)-kv * velocity_ + force_ - direction_;

      // Stops if at goal
      if (index_ >= path_.size() - 1 && glm::length(diff) < 0.05f) {
        return;
      } 
    } else if (index_ < path_.size()) {
      next_ = path_[index_++];
      direction_ = glm::normalize(next_->position_ - position_);
      //velocity_ = 1.f * glm::length(velocity_) * direction_;
    }
  }

  position_ += velocity_ * dt;
  velocity_ += force_ * dt;

  calculate_forces(agents, obstacles);


  // Stops if too slow
  if (glm::length(velocity_) < 0.05f) {
    velocity_ = glm::vec3();
  }

  // Limit velocity
  if (glm::length(velocity_) > speed_) {
    velocity_ = glm::normalize(velocity_) * speed_;
  }
}

void Agent::calculate_forces(const std::vector<Agent *> & agents,
                             const std::vector<Obstacle *> & obstacles) {
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

  glm::vec3 alignment_force = glm::vec3();
  glm::vec3 cohesion_force = glm::vec3();
  glm::vec3 separation_force = glm::vec3();

  // Alignment force
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

  // Cohesion force
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
  
  // Separation force
  for (int i = 0; i < nearby.size(); i++) {
    float dist = glm::length(position_ - nearby[i]->position_);
    if (dist > 0 && dist < boid_dist - radius_) {
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

  
  //std::vector<Obstacle *> nearby;
  int osize = obstacles.size();
  float sphere_influence = 0.5f;
  glm::vec3 obstacle_force = glm::vec3();

  count = 0;

  for (int i = 0; i < osize; i++) {
    glm::vec3 dir_to_self = position_ - obstacles[i]->position_;
    float dist = glm::length(position_ - obstacles[i]->position_);
    if (dist > 0 &&
        dist < static_cast<SphereObstacle *>(obstacles[i])->radius_ + radius_ +
                   sphere_influence) {
      obstacle_force += (glm::normalize(dir_to_self) / dist);
      count++;
    }
  }

  if (count > 0) {
    obstacle_force /= count;
  }

  // Sum the forces and add multiplier to separation to keep separated
  force_ = 10.f * direction_ + alignment_force + cohesion_force + 5.f * separation_force +
           35.f * obstacle_force;
}