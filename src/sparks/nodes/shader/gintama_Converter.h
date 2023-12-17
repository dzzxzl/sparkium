#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class MixShader: public Node {
 public:
  MixShader();
  ~MixShader() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  Prober factor_; // in slot 0
  Prober in_map1; // in slot 1
  Prober in_map2; // in slot 2
  Prober out_map; // out slot 0
};


} // namespace sparks