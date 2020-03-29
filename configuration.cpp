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
}

Configuration::~Configuration() {
  delete graph_;
  delete[] path_vertices_;
}

void Configuration::create_graph(glm::vec3 start_pos, glm::vec3 goal_pos,
                                 int samples, int connections) {
  graph_ = new Graph(start_pos, goal_pos);

  // Adding obstacles to scene
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(-8.f, 8.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(-6.f, 6.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(-4.f, 4.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(-2.f, 2.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(0, 0, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(2.f, -2.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(4.f, -4.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(6.f, -6.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(-4.f, -4.f, 0), 2.f));
  graph_->obstacles_.push_back(new SphereObstacle(glm::vec3(0.f, -8.f, 0), 2.f));

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
      } else if (distance <= start_dist[next_milestone->neighbors_[i]]) {
        start_dist[next_milestone->neighbors_[i]] = distance;
        next_milestone->neighbors_[i]->previous_ = next_milestone;
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
  Milestone * trailing_path = graph_->goal_;

  while (next_path != NULL) {
    path_.push_back(next_path);
    
    // Highlighting path
    graph_->graph_vertices_[8 * next_path->id_ + 3] = 1.f;
    int size = next_path->neighbors_size_;
    for (int i = 0; i < size; i++) {
      if (next_path->previous_ == next_path->neighbors_[i] ||
          trailing_path == next_path->neighbors_[i]) {
        next_path->connections_[16 * i + 3] = 1.f;
        next_path->connections_[16 * i + 11] = 1.f;
      }
    }

    trailing_path = next_path;
    next_path = next_path->previous_;
  }

  std::reverse(path_.begin(), path_.end());
}

void Configuration::find_path_astar() {
  std::cout << "Finding path using A*..." << std::endl;
  std::map<Milestone *, float> start_dist;
  std::list<Milestone *> next;
  Milestone * next_milestone;
  float min_value = INFINITY;
  Milestone * min_index = NULL;

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
      } else if (distance < start_dist[next_milestone->neighbors_[i]]) {
        start_dist[next_milestone->neighbors_[i]] = distance;
        next_milestone->neighbors_[i]->previous_ = next_milestone;
      }
    }

    // Reset min values
    min_value = INFINITY;
    min_index = NULL;
  }

  Milestone * next_path = graph_->goal_;

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

  Milestone * next = graph_->goal_;

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