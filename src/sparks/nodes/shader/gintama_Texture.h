#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "glm/glm.hpp"
#include "cmath"

namespace sparks {

class Checker: public Node {
 public:
  Checker() = delete;
  Checker(SceneInfo* scene_info);
  ~Checker() override;
  void process() override;
  enum class InSlotName {
    Color1 = 0,
    Color2,
    Scale
  };
  enum class OutSlotName {
    Color = 0,
    Gradient
  };
  SceneInfo* scene_info_;
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
};

} // namespace sparks