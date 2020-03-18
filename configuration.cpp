// Copyright 2020 Michael Ung

#include "configuration.h"

#include <iostream>
#include <string>
#include <sstream>

#include "glad/glad.h"

Configuration::Configuration() {
  graph_ = NULL;
  path_vertices_ = NULL;
  path_normals_ = NULL;
  path_distance_ = 0;
  path_size_ = 0;
  path_exists_ = false;
}

Configuration::~Configuration() {

}

void Configuration::create_graph(glm::vec3 start_pos, glm::vec3 goal_pos, int samples) {
  graph_ = new Graph(start_pos, goal_pos);

  graph_->generate(20.f, 20.f, samples);
  graph_->connect(5);
}

void Configuration::find_path() {
  std::cout << "Finding path using UCS..." << std::endl;
}

void Configuration::find_path_astar() {
  std::cout << "Finding path using A*..." << std::endl;
}

void Configuration::update() {

}

// Requires context bound to correct buffer
void Configuration::draw_points() {
  glEnable(GL_PROGRAM_POINT_SIZE);
  glDrawArrays(GL_POINTS, 0, graph_->size_);
}

// Requires context bound to next buffer
void Configuration::draw() {
  draw_points();

  glDrawArrays(GL_LINES, 0, path_size_);
}

void Configuration::info() {
  std::stringstream ss;
  std::stringstream sst;

  Milestone * goal = graph_->goal_;
  Milestone * start = graph_->start_;
  Milestone * next = goal->previous_;

  // Building a string
  if (path_exists_) {
    ss << "Path distance: " << path_distance_ << std::endl;

    while (next != NULL) {
      sst << next->position_.x << ", " << next->position_.y << ", " << next->position_.z;
      sst << "\n" << ss.rdbuf();
      ss = std::move(sst);
    }

    sst << "Path:\n" << ss.rdbuf();
    ss = std::move(sst);

    std::cout << ss.rdbuf();
  }
}