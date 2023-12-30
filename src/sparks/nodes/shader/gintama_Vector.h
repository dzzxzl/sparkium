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
    Height = 0, Normal = 1,
    GradHeight = 2
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
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
  void flush();

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
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
};

} // namespace sparks