#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class MixShader: public Node {
 public:
  MixShader();
  ~MixShader() override;
  // void process() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  // in slot 0 is factor
  // in slot 1 is color1
  // in slot 2 is color2
  // out slot 0 is color
};

class Multiplier: public Node {
 public:
  Multiplier();
  ~Multiplier() override;
  void process() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  // in slot 0 is color1
  // in slot 1 is color2
  // out slot 0 is color
};

} // namespace sparks