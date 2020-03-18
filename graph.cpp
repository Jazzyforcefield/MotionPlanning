// Copyright 2020 Michael Ung

#include "graph.h"

#include <iostream>
#include <random>

Graph::Graph() {
  start_ = NULL;
  goal_ = NULL;
  graph_vertices_ = NULL;
  graph_normals_ = NULL;
  size_ = 0;
}

Graph::Graph(glm::vec3 start_pos, glm::vec3 goal_pos) {
  start_ = new Milestone(start_pos);
  goal_ = new Milestone(goal_pos);
  graph_vertices_ = NULL;
  graph_normals_ = NULL;
  size_ = 0;
}

Graph::~Graph() {
}

void Graph::generate(float width, float height, int k) {
  std::cout << "Generating graph..." << std::endl;

  // Add start and goal to milestone vector
  milestones_.push_back(start_);
  milestones_.push_back(goal_);
  size_ += 2;

  for (int i = 0; i < k; i++) {
    // Create and add to milestones
    float randfx = (rand() % 1000) * (width / 1000) - width / 2;
    float randfy = (rand() % 1000) * (height / 1000) - height / 2;
    float randfz = 0; // (rand() % 1000) * (length / 1000) - length / 2;

    Milestone * new_milestone = new Milestone(glm::vec3(randfx, randfy, randfz));
    milestones_.push_back(new_milestone);
    size_++;

    std::cout << "  Added milestone " << (size_ - 1)
              << " to milestones." << std::endl;
  }

  std::cout << "Finished generation." << std::endl;
}

void Graph::connect(int k) {
  std::cout << "Connecting graph..." << std::endl;

  for (int i = 0; i < size_; i++) {
    std::cout << "  Populating neighbors for milestone "
              << i << "..." << std::endl; 
    milestones_[i]->populate_neighbors(milestones_, k);

  }
}

void Graph::update() {

}

void Graph::info() {
  std::cout << "Start milestone position: (" << start_->position_.x
            << ", " << start_->position_.y << ", " << start_->position_.z
            << ")" << std::endl;

  std::cout << "Goal milestone position: (" << goal_->position_.x
            << ", " << goal_->position_.y << ", " << goal_->position_.z
            << ")" << std::endl;

  std::cout << "This graph has " << size_ << " milestones." << std::endl;

  for (int i = 0; i < size_; i++) {
    std::cout << "Milestone " << i << ": " << std::endl;
    milestones_[i]->info();
  }
}