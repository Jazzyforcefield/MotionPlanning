// Copyright 2020 Michael Ung

#include "graph.h"

#define NODEBUG

constexpr float AGENT_RADIUS = 1.f;

#include <iostream>
#include <random>
#include <ctime>

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
  for (int i = 0; i < size_; i++) {
    delete milestones_[i];
  }

  for (int i = 0; i < obstacles_.size(); i++) {
    delete obstacles_[i];
  }

  delete[] graph_vertices_;
  delete[] graph_normals_;
}

void Graph::generate(float width, float height, int k, int connections) {
  #ifndef NODEBUG
  std::cout << "Generating graph..." << std::endl;
  #endif

  srand(time(NULL));

  // Allocate memory for vertices and normals
  graph_vertices_ = new float[8 * (k + 2)];
  graph_normals_ = new float[3 * (k + 2)];

  // Add start and goal to milestone vector
  graph_vertices_[0] = start_->position_.x;
  graph_vertices_[1] = start_->position_.y;
  graph_vertices_[2] = start_->position_.z;
  graph_vertices_[3] = 0;
  graph_vertices_[4] = 0;
  graph_vertices_[5] = 0;
  graph_vertices_[6] = 0;
  graph_vertices_[7] = 0;

  graph_vertices_[8] = goal_->position_.x;
  graph_vertices_[9] = goal_->position_.y;
  graph_vertices_[10] = goal_->position_.z;
  graph_vertices_[11] = 0;
  graph_vertices_[12] = 0;
  graph_vertices_[13] = 0;
  graph_vertices_[14] = 0;
  graph_vertices_[15] = 0;

  graph_normals_[0] = 0;
  graph_normals_[1] = 0;
  graph_normals_[2] = 1;

  graph_normals_[3] = 0;
  graph_normals_[4] = 0;
  graph_normals_[5] = 1;

  milestones_.push_back(start_);
  milestones_.push_back(goal_);
  size_ += 2;

  #ifndef NODEBUG
  std::cout << "  Added milestone " << 0 << " to milestones."
            << std::endl;

  std::cout << "  Added milestone " << 1 << " to milestones."
            << std::endl;
  #endif

  for (int i = 2; i < k + 2; i++) {
    // Create and add to milestones
    float randfx = (rand() % 1000) * (width / 1000) - width / 2;
    float randfy = (rand() % 1000) * (height / 1000) - height / 2;
    float randfz = 0; // (rand() % 1000) * (length / 1000) - length / 2;

    while (!in_cspace(randfx, randfy, randfz, AGENT_RADIUS)) {
      randfx = (rand() % 1000) * (width / 1000) - width / 2;
      randfy = (rand() % 1000) * (height / 1000) - height / 2;
      randfz = 0;  // (rand() % 1000) * (length / 1000) - length / 2;
    }

    // Setting x, y, z, r, g, b, u, v
    graph_vertices_[8 * i] = randfx;
    graph_vertices_[8 * i + 1] = randfy;
    graph_vertices_[8 * i + 2] = randfz;
    graph_vertices_[8 * i + 3] = 0;
    graph_vertices_[8 * i + 4] = 0;
    graph_vertices_[8 * i + 5] = 0;
    graph_vertices_[8 * i + 6] = 0;
    graph_vertices_[8 * i + 7] = 0;

    graph_normals_[3 * i] = 0;
    graph_normals_[3 * i + 1] = 0;
    graph_normals_[3 * i + 2] = 1;

    Milestone * new_milestone = new Milestone(glm::vec3(randfx, randfy, randfz));
    milestones_.push_back(new_milestone);
    size_++;

    #ifndef NODEBUG
    std::cout << "  Added milestone " << (size_ - 1)
              << " to milestones." << std::endl;
    #endif

  }

  connect(connections);

  #ifndef NODEBUG
  std::cout << "Finished generation." << std::endl;
  #endif
}

bool Graph::in_cspace(float x, float y, float z, float radius) {
  int size = obstacles_.size();

  for (int i = 0; i < size; i++) {
    if (obstacles_[i]->point_inside(glm::vec3(x, y, z), radius)) {
      return false;
    }
  } return true;
}

void Graph::connect(int k) {
  #ifndef NODEBUG
  std::cout << "Connecting graph..." << std::endl;
  #endif

  for (int i = 0; i < size_; i++) {
    #ifndef NODEBUG
    std::cout << "  Populating neighbors for milestone "
              << i << "..." << std::endl; 
    #endif

    milestones_[i]->populate_neighbors(milestones_, obstacles_, k);
  }

  #ifndef NODEBUG
  std::cout << "Graph connected." << std::endl;
  #endif
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