// Copyright 2020 Michael Ung

#include "graph.h"

#include <iostream>

Graph::Graph() {

}

Graph::Graph(glm::vec3 start_pos, glm::vec3 goal_pos) {

}

Graph::~Graph() {

}

void Graph::generate(float width, float height, int k) {

}

void Graph::connect(int k) {

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

  std::cout << "This graph has " << size_ << " neighbors" << std::endl;
}