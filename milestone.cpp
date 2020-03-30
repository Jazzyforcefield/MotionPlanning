// Copyright 2020 Michael Ung

#include "milestone.h"

#define NODEBUG

constexpr float AGENT_RADIUS = 0.5f;

#include <iostream>

static int num_milestones_ = 0;

Milestone::Milestone() {
  position_ = glm::vec3(0, 0, 0);
  neighbors_size_ = 0;
  previous_ = NULL;
  connections_ = NULL;
  id_ = num_milestones_++;
}

Milestone::Milestone(glm::vec3 pos) {
  position_ = pos;
  neighbors_size_ = 0;
  previous_ = NULL;
  connections_ = NULL;
  id_ = num_milestones_++;
}

Milestone::~Milestone() {
  num_milestones_--;
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
                                   const std::vector<Obstacle *>& obstacles,
                                   int k) {
  int neighbor_count = 0;
  int target_neighbors = k;
  int size = milestones.size();
  int osize = obstacles.size();
  bool intersected = false;
  std::vector<float> distances;

  float min_val = INFINITY;
  int min_index = -1;
  std::vector<Milestone *> smallest_neighbors;
  std::vector<int> added_indices;

  // Add the calculated distances to the vector
  for (int i = 0; i < size; i++) {
    distances.push_back(distance_between(milestones[i]));
  }


  // PROBLEM: Just because one milestone is a neighbor, doesn't meant that neighbor
  // has that milestone as a neighbor...
  // Loop through up to k-times or until graph size to find minimum
  for (int j = 0; neighbors_size_ < target_neighbors && j < size - 1; j++) {
    for (int i = 0; i < size; i++) {
      if (distances[i] < min_val &&
          distances[i] != 0 &&
          std::find(neighbors_.begin(), neighbors_.end(), milestones[i]) ==
              neighbors_.end() &&
          std::find(added_indices.begin(), added_indices.end(), i) ==
              added_indices.end()) {
        min_val = distances[i];
        min_index = i;
      }
    }

    // Adds the neighbor with the smallest distance to the neighbors
    // And add it to the vector of added indices

    if (min_index > -1) {
      for (int o = 0; o < osize; o++) {
        intersected = obstacles[o]->line_intersecting(position_,
                                                      milestones[min_index]->position_,
                                                      AGENT_RADIUS);
        if (intersected) {
          break;
        }
      }

      if (intersected) {
        // Make sure not to check this one again
        added_indices.push_back(min_index);

        // Reset values
        intersected = false;
        min_val = INFINITY;
        min_index = -1;
        continue;
      }

      // Add as neighbor
      neighbors_.push_back(milestones[min_index]);
      milestones[min_index]->neighbors_.push_back(this);
      neighbors_size_++;
      milestones[min_index]->neighbors_size_++;

      #ifndef NODEBUG
      std::cout << "    Added milestone " << min_index
                << " as neighbor and vice versa." << std::endl;
      #endif

      // Reset values
      min_val = INFINITY;
      min_index = -1;
    }
  }
}

void Milestone::connect_neighbors() {
  // Vertices for each line
  connections_ = new float[16 * neighbors_size_];

  for (int i = 0; i < neighbors_size_; i++) {
    connections_[16 * i] = position_.x;
    connections_[16 * i + 1] = position_.y;
    connections_[16 * i + 2] = position_.z + 0.1;
    connections_[16 * i + 3] = 0;
    connections_[16 * i + 4] = 0;
    connections_[16 * i + 5] = 0;
    connections_[16 * i + 6] = 0;
    connections_[16 * i + 7] = 0;

    connections_[16 * i + 8] = neighbors_[i]->position_.x;
    connections_[16 * i + 9] = neighbors_[i]->position_.y;
    connections_[16 * i + 10] = neighbors_[i]->position_.z + 0.1;
    connections_[16 * i + 11] = 0;
    connections_[16 * i + 12] = 0;
    connections_[16 * i + 13] = 0;
    connections_[16 * i + 14] = 0;
    connections_[16 * i + 15] = 0;
  }
}

void Milestone::info() {
  std::cout << "  Current milestone position: (" << position_.x << ", "
            << position_.y << ", " << position_.z << ")" << std::endl;

  if (previous_ != NULL) {
    std::cout << "  Previous milestone position: (" << previous_->position_.x
              << ", " << previous_->position_.y << ", " << previous_->position_.z
              << ")" << std::endl;
  } else {
    std::cout << "  Previous milestone position: N/A" << std::endl;
  }

  std::cout << "  This milestone has " << neighbors_size_ << " neighbors"
            << std::endl;
}