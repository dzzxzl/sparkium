#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class Bump: public Node {
 public:
  Bump();
  ~Bump() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  enum BumpType {
    Height = 0, Normal = 1
  };
  Prober in_map; // in slot 0
  float scale_{1.0f}; // in slot 1
  BumpType bumpType_{Height}; // in slot 2
  Prober out_map; // out slot 0
};






} // namespace sparks