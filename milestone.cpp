// Copyright 2020 Michael Ung

#include "milestone.h"

#include <iostream>

Milestone::Milestone() {

}

Milestone::~Milestone() {

}

void Milestone::update() {

}

float Milestone::distance_between(Milestone * other) {
  return 0.f;
}

void Milestone::populate_neighbors(const std::vector<Milestone *>& milestones,
                                   int k) {
  return;
}

void Milestone::info() {
  std::cout << "Current milestone position: (" << position_.x << ", "
            << position_.y << ", " << position_.z << ")" << std::endl;

  std::cout << "Previous milestone position: (" << previous_->position_.x
            << ", " << previous_->position_.y << ", " << previous_->position_.z
            << ")" << std::endl;

  std::cout << "This milestone has " << neighbors_size_ << " neighbors"
            << std::endl;
}