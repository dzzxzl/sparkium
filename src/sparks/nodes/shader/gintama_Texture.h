#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "glm/glm.hpp"

namespace sparks {

class Checker: public Node {
 public:
  Checker();
  ~Checker() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  glm::vec3 color1_{0.0f, 0.0f, 0.0f}; // in slot 0
  glm::vec3 color2_{1.0f, 1.0f, 1.0f}; // in slot 1
  float scale_{10.0f}; // in slot 2
 private:
  Prober prober_; // built in
};

} // namespace sparks