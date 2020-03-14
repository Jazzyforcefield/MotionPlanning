// Copyright 2020 Michael Ung

#ifndef GRAPH_H_
#define GRAPH_H_

#include "milestone.h"

class Graph {
 public:
  // Method declarations
  Graph();
  Graph(glm::vec3 start_pos, glm::vec3 goal_pos);
  ~Graph();

  // Randomly generates milestones centered at 0, 0
  void generate(float width, float height, int k);

  // Populates all milestones with up to k-neighbors
  void connect(int k);

  // Updates milestones
  void update();

  // Prints out data
  void info();

  // Member variables
  Milestone * start_;
  Milestone * goal_;

  // Vector of all milestones
  // Will likely change to a spatial data structure later
  std::vector<Milestone *> milestones_;
  float * graph_vertices_;
  float * graph_normals_;

  // Number of milestones in the graph
  int size_;
};

#endif	// GRAPH_H_