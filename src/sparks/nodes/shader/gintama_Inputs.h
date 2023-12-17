#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "sparks/assets/scene.h"

namespace sparks {

class AmbientOcclusion: public Node {
 public:
  AmbientOcclusion();
  ~AmbientOcclusion() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  float depth_{1.0f};
};

class TextureSample: public Node {
 public:
  TextureSample() = delete;
  TextureSample(Scene* scene, int entity_id, int texture_id, float u, float v);
  ~TextureSample() override;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  // out slot 0 color_
  Scene* scene_;
  int entity_id_{-1};
  int texture_id_{-1};
  float u_{0.0f};
  float v_{0.0f};
};


}