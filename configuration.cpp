// Copyright 2020 Michael Ung

#include "configuration.h"

#include <iostream>
#include <string>
#include <sstream>

#include "glad/glad.h"

Configuration::Configuration() {
  
}

Configuration::~Configuration() {

}

void Configuration::create_graph(glm::vec3 start_pos, glm::vec3 goal_pos, int samples) {

}

void Configuration::find_path() {

}

void Configuration::find_path_astar() {

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