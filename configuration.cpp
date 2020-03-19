// Copyright 2020 Michael Ung

#include "configuration.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>

#include "glad/glad.h"

Configuration::Configuration() {
  graph_ = NULL;
  path_vertices_ = NULL;
  path_normals_ = NULL;
  path_distance_ = 0;
  path_size_ = 0;
  path_exists_ = false;
  circle_vertices_ = new float[8 * 362];

  circle_vertices_[0] = 0;
  circle_vertices_[1] = 0;
  circle_vertices_[2] = 0;
  circle_vertices_[3] = 0;
  circle_vertices_[4] = 0;
  circle_vertices_[5] = 0;
  circle_vertices_[6] = 0;
  circle_vertices_[7] = 0;

  for (int i = 1; i < 362; i++) {
    circle_vertices_[8 * i] = 2.f * cos((i - 1) * 3.1415 / 180);
    circle_vertices_[8 * i + 1] = 2.f * sin((i - 1) * 3.1415 / 180);
    circle_vertices_[8 * i + 2] = 0;
    circle_vertices_[8 * i + 3] = 0;
    circle_vertices_[8 * i + 4] = 0;
    circle_vertices_[8 * i + 5] = 0;
    circle_vertices_[8 * i + 6] = 0;
    circle_vertices_[8 * i + 7] = 0;
  }
}

Configuration::~Configuration() {

}

void Configuration::create_graph(glm::vec3 start_pos, glm::vec3 goal_pos, int samples) {
  graph_ = new Graph(start_pos, goal_pos);

  graph_->generate(20.f, 20.f, samples);
  graph_->connect(10);
}

void Configuration::find_path() {
  std::cout << "Finding path using UCS..." << std::endl;
  std::map<Milestone *, float> start_dist;
  std::list<Milestone *> next;
  Milestone * next_milestone;
  float min_value = INFINITY;
  Milestone * min_index = NULL;

  start_dist.insert(std::pair<Milestone *, float>(graph_->start_, 0));
  next.push_back(graph_->start_);

  while (next.size() != 0) {
  // Find minimum distance of next milestones
    for (std::list<Milestone *>::iterator it = next.begin();
         it != next.end(); it++) {
      if (start_dist[*it] < min_value) {
        min_value = start_dist[*it];
        min_index = *it;
      }
    }

    if (min_index != NULL) {
      std::cout << min_index->position_.x << " " << min_index->position_.y << std::endl;
      next_milestone = min_index;
    } else {
      std::cerr << "Problem... something isn't less than infinity."
                << std::endl;
      exit(1);
    }

    if (next_milestone == graph_->goal_) {
      return;
    }

    next.remove(min_index);

    for (int i = 0; i < next_milestone->neighbors_size_; i++) {
      float distance =
          next_milestone->distance_between(next_milestone->neighbors_[i]) +
          start_dist[next_milestone];

      if (start_dist.find(next_milestone->neighbors_[i]) == start_dist.end()) {
        start_dist.insert(std::pair<Milestone *, float>(
            next_milestone->neighbors_[i], distance));
        next.push_back(next_milestone->neighbors_[i]);
        next_milestone->neighbors_[i]->previous_ = next_milestone;
      }
    }

    min_value = INFINITY;
    min_index = NULL;
  }

  Milestone * prev = graph_->goal_;
  while (prev->previous_ != NULL) {
    std::cout << prev->position_.x << " " << prev->position_.y << std::endl;
    prev = prev->previous_;
  }

  if (prev == graph_->start_) {
    path_exists_ = true;
    path_distance_ = start_dist[graph_->goal_];
  }
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