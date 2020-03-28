// Copyright 2020 Michael Ung

#include "configuration.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>

#include "glad/glad.h"
#include "sphere_obstacle.h"

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

void Configuration::create_graph(glm::vec3 start_pos, glm::vec3 goal_pos,
                                 int samples, int connections) {
  graph_ = new Graph(start_pos, goal_pos);

  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(0, 0, 0), 2.f));

  graph_->generate(20.f, 20.f, samples, connections);
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

    // Assign next milestone to the min index
    if (min_index != NULL) {
      next_milestone = min_index;
    } else {
      std::cerr << "Problem... something isn't less than infinity."
                << std::endl;
      exit(1);
    }

    // Check if goal is found
    if (next_milestone == graph_->goal_) {
      path_exists_ = true;
      path_distance_ = start_dist[next_milestone];
      break;
    }

    next.remove(min_index);

    // Loop through neighbors
    for (int i = 0; i < next_milestone->neighbors_size_; i++) {
      float distance =
          next_milestone->distance_between(next_milestone->neighbors_[i]) +
          start_dist[next_milestone];

      // Double check this next time I look at it
      if (start_dist.find(next_milestone->neighbors_[i]) == start_dist.end()) {
        start_dist.insert(std::pair<Milestone *, float>(
            next_milestone->neighbors_[i], distance));
        next.push_back(next_milestone->neighbors_[i]);
        next_milestone->neighbors_[i]->previous_ = next_milestone;
      } else {
        if (distance < start_dist[next_milestone->neighbors_[i]]) {
          start_dist[next_milestone->neighbors_[i]] = distance;
        }
      }
    }

    // Reset min values
    min_value = INFINITY;
    min_index = NULL;
  }

  if (!path_exists_) {
    std::cout << "No path found, try again or"
              << " use more samples and connections." << std::endl;
    exit(0);
  }

  Milestone * next_path = graph_->goal_;

  while (next_path != NULL) {
    path_.push_back(next_path);
    next_path = next_path->previous_;
  }

  std::reverse(path_.begin(), path_.end());
}

void Configuration::find_path_astar() {
  std::cout << "Finding path using A*..." << std::endl;
  std::map<Milestone *, float> start_dist;
  std::list<Milestone *> next;
  Milestone *next_milestone;
  float min_value = INFINITY;
  Milestone *min_index = NULL;

  start_dist.insert(std::pair<Milestone *, float>(graph_->start_, 0));
  next.push_back(graph_->start_);

  while (next.size() != 0) {
    // Find minimum distance of next milestones
    for (std::list<Milestone *>::iterator it = next.begin(); it != next.end();
         it++) {
      if (start_dist[*it] < min_value) {  // Add heuristic here for A*, + goal - (*it)->position
        min_value = start_dist[*it];
        min_index = *it;
      }
    }

    // Assign next milestone to the min index
    if (min_index != NULL) {
      next_milestone = min_index;
    } else {
      std::cerr << "Problem... something isn't less than infinity."
                << std::endl;
      exit(1);
    }

    // Check if goal is found
    if (next_milestone == graph_->goal_) {
      path_exists_ = true;
      path_distance_ = start_dist[next_milestone];
      break;
    }

    next.remove(min_index);

    // Loop through neighbors
    for (int i = 0; i < next_milestone->neighbors_size_; i++) {
      float distance =
          next_milestone->distance_between(next_milestone->neighbors_[i]) +
          start_dist[next_milestone];

      // Double check this next time I look at it
      if (start_dist.find(next_milestone->neighbors_[i]) == start_dist.end()) {
        start_dist.insert(std::pair<Milestone *, float>(
            next_milestone->neighbors_[i], distance));
        next.push_back(next_milestone->neighbors_[i]);
        next_milestone->neighbors_[i]->previous_ = next_milestone;
      } else {
        if (distance < start_dist[next_milestone->neighbors_[i]]) {
          start_dist[next_milestone->neighbors_[i]] = distance;
        }
      }
    }

    // Reset min values
    min_value = INFINITY;
    min_index = NULL;
  }

  Milestone *next_path = graph_->goal_;

  while (next_path != NULL) {
    path_.push_back(next_path);
    next_path = next_path->previous_;
  }

  std::reverse(path_.begin(), path_.end());
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
      sst << "  " << next->position_.x << ", " << next->position_.y << ", "
          << next->position_.z << "\n" << ss.rdbuf();
      ss = std::move(sst);
      next = next->previous_;
    }

    sst << "Path:\n" << ss.rdbuf();
    ss = std::move(sst);

    std::cout << ss.rdbuf();
  }
}