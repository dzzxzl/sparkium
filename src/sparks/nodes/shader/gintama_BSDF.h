#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class DiffuseBSDF: public Node {
 public:
  DiffuseBSDF();
  ~DiffuseBSDF() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  Prober in_color_; // in slot 0
  Prober out_color_; // out slot 0
  Prober deform_; // in slot 1
  enum DeformType {
    Geometry = 0, Normal = 1
  };
  DeformType deformType_{Geometry}; // in slot 2
};


}