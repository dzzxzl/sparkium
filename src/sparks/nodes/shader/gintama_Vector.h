#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class Bump: public Node {
 public:
  Bump();
  ~Bump() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  enum BumpType {
    Height = 0, Normal = 1
  };
  // in slot 0 is scale
  // in slot 1 is bump type
  // in slot 2 is height
  // in slot 3 is gradient
  // out slot 0 is out normal
  void process() override;
  glm::vec3 normal_{0.0f, 0.0f, 0.0f};
  glm::vec3 tangent_{0.0f, 0.0f, 0.0f};
  glm::vec3 reflection_{0.0f, 0.0f, 0.0f};
};

class Color: public Node {
 public:
  Color() = delete;
  Color(glm::vec3 color);
  ~Color() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  // out slot 0 is color
  void process() override;
  glm::vec3 color_{0.0f, 0.0f, 0.0f};
};




} // namespace sparks