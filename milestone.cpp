// Copyright 2020 Michael Ung

#include "milestone.h"

#include <iostream>

Milestone::Milestone() {

}

Milestone::Milestone(glm::vec3 pos) {
  position_ = pos;
}

Milestone::~Milestone() {

}

void Milestone::update() {
  // What was I gonna use this for again?
  // Maybe moving milestones? Dynamic paths?
}

float Milestone::distance_between(Milestone * other) {
  return sqrt(pow(other->position_.x - position_.x, 2) +
              pow(other->position_.y - position_.y, 2) +
              pow(other->position_.z - position_.z, 2));
}

void Milestone::populate_neighbors(const std::vector<Milestone *>& milestones,
                                   int k) {
  int neighbor_count = 0;
  int size = milestones.size();
  std::vector<float> distances;

  float min_val = INFINITY;
  int min_index = -1;
  std::vector<Milestone *> smallest_neighbors;
  std::vector<int> added_indices;

  // Add the calculated distances to the vector
  for (int i = 0; i < size; i++) {
    distances.push_back(distance_between(milestones[i]));
  }

  // Loop through up to k-times or until graph size to find minimum
  for (int j = 0; j < k && j < size; j++) {
    for (int i = 0; i < size; i++) {
      if (distances[i] < min_val &&
          distances[i] != 0 &&
          std::find(added_indices.begin(), added_indices.end(), i) ==
          added_indices.end()) {
        min_val = distances[i];
        min_index = i;
      }
    }

    // Adds the neighbor with the smallest distance to the neighbors
    // And add it to the vector of added indices
    smallest_neighbors.push_back(milestones[min_index]);
    added_indices.push_back(min_index);
    neighbor_count++;

    std::cout << "    Added milestone " << min_index
              << " as neighbor." << std::endl;

    // Reset values
    min_val = INFINITY;
    min_index = -1;
  }

  // Assign new/updated neighbor vector
  neighbors_size_ = neighbor_count;
  neighbors_ = smallest_neighbors;
}

void Milestone::info() {
  std::cout << "Current milestone position: (" << position_.x << ", "
            << position_.y << ", " << position_.z << ")" << std::endl;

  if (previous_ != NULL) {
    std::cout << "Previous milestone position: (" << previous_->position_.x
              << ", " << previous_->position_.y << ", " << previous_->position_.z
              << ")" << std::endl;
  } else {
    std::cout << "Previous milestone position: N/A" << std::endl;
  }

  std::cout << "This milestone has " << neighbors_size_ << " neighbors"
            << std::endl;
}