#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class DiffuseBSDF: public Node {
 public:
  DiffuseBSDF();
  ~DiffuseBSDF() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  void process() override;
  void process_recursive() override;
  // in slot 0 is color
  // in slot 1 is normal
  // out slot 0 is color
  glm::vec3 incident_{0.0f, 0.0f, 0.0f};
  glm::vec3 reflected_{0.0f, 0.0f, 0.0f};
};


}