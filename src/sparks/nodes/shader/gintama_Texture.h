#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "glm/glm.hpp"
#include "cmath"

namespace sparks {

class Checker: public Node {
 public:
  Checker();
  ~Checker() override;
  void process() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  // in slot 0 color1_
  // in slot 1 color2_
  // in slot 2 scale_
  // out slot 0 color_
  // out slot 1 gradient_
  float u_{0.0f};
  float v_{0.0f};
};

} // namespace sparks