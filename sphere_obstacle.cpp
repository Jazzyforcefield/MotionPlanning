// Copyright 2020 Michael Ung

#include "sphere_obstacle.h"

SphereObstacle::SphereObstacle() : Obstacle() {
  radius_ = 1.f;
  vertices_ = new float[8 * 361];

  for (int i = 0; i < 361; i++) {
    vertices_[8 * i] = radius_ * cos(i * 3.14159265 / 180);
    vertices_[8 * i + 1] = radius_ * sin(i * 3.14159265 / 180);
    vertices_[8 * i + 2] = 0;
    vertices_[8 * i + 3] = 1.f;
    vertices_[8 * i + 4] = 0;
    vertices_[8 * i + 5] = 1.f;
    vertices_[8 * i + 6] = 0;
    vertices_[8 * i + 7] = 0;
  }
}

SphereObstacle::SphereObstacle(glm::vec3 center, float radius) : Obstacle(center) {
  radius_ = radius;
  vertices_ = new float[8 * 361];

  for (int i = 0; i < 361; i++) {
    vertices_[8 * i] = radius_ * cos(i * 3.14159265 / 180) + center.x;
    vertices_[8 * i + 1] = radius_ * sin(i * 3.14159265 / 180) + center.y;
    vertices_[8 * i + 2] = 0 + center.z;
    vertices_[8 * i + 3] = 1.f;
    vertices_[8 * i + 4] = 0;
    vertices_[8 * i + 5] = 1.f;
    vertices_[8 * i + 6] = 0;
    vertices_[8 * i + 7] = 0;
  }
}

SphereObstacle::~SphereObstacle(){
  delete[] vertices_;
}

bool SphereObstacle::point_inside(glm::vec3 point, float agent_radius) {
  if (glm::length(point - position_) > radius_ + agent_radius) {
    return false;
  } return true;
}

bool SphereObstacle::line_intersecting(glm::vec3 p1, glm::vec3 p2) {
  float a, b, c;
  a = 1.f;
  b = 2.f * glm::dot((p1 - position_), (p1 - p2));
  c = pow(glm::dot(p1 - position_, p1 - position_), 2) - pow(radius_, 2);

  if (pow(b, 2) - 4.f * a * c >= 0) {
    return true;
  } return false;
}