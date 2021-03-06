// Copyright 2020 Michael Ung

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "graph.h"
#include "agent.h"

class Configuration {
 public:
  // Method declarations
  Configuration();
  ~Configuration();

  // Create a graph 
  void create_graph(glm::vec3 start_pos, glm::vec3 goal_pos,
                    int samples, int connections);

  // Will probably use Djikstra's algorithm for UCS
  void find_path();

  // Will probably use A* search algorithm
  void find_path_astar();
  void update();
  void draw_points();
  void draw();
  void info();

  // Member variables
  Graph * graph_;
  std::vector<Milestone *> path_;
  std::vector<Agent *> agents_;
  float * path_vertices_;
  float * path_normals_;
  float path_distance_;
  int path_size_;
  bool path_exists_;
};

#endif  // CONFIGURATION_H_