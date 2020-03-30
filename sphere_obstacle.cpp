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
    vertices_[8 * i + 2] = 0;
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

bool SphereObstacle::line_intersecting(glm::vec3 p1, glm::vec3 p2, float agent_radius) {
  float a, b, c, discrim;
  a = 1.f;
  b = 2.f * glm::dot((p1 - position_), glm::normalize(p2 - p1));
  c = glm::dot(p1 - position_, p1 - position_) - pow(radius_ + agent_radius, 2);
  discrim = pow(b, 2) - 4.f * a * c;

  if (pow(b, 2) - 4.f * a * c >= 0) {
    float t = -b - sqrt(discrim) / (2.f * a);
    glm::vec3 intersect = glm::normalize(p2 - p1) * t;
    if (glm::length(intersect) - glm::length(p2 - p1) >= 0.001) {
      return false;
    } return true;
  } return false;
}