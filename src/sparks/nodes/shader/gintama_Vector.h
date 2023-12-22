#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class Bump: public Node {
 public:
  Bump() = delete;
  Bump(SceneInfo* scene_info);
  ~Bump() override;
  enum class BumpType {
    Height = 0, Normal = 1
  };
  enum class InSlotName {
    Scale = 0,
    BumpType,
    Height,
    Gradient
  };
  enum class OutSlotName {
    Normal = 0
  };
  void process() override;
  SceneInfo* scene_info_;
};

class Color: public Node {
 public:
  Color() = delete;
  Color(glm::vec3 color);
  ~Color() override;
  enum class OutSlotName {
    Color = 0
  };
  void process() override;
  glm::vec3 color_{0.0f, 0.0f, 0.0f};
};


} // namespace sparks