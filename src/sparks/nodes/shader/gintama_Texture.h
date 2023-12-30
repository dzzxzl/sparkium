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
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class Stripes: public Node {
 public:
  Stripes() = delete;
  Stripes(SceneInfo* scene_info);
  ~Stripes() override;
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
  bool inverse_direction_{false};
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class Noise: public Node {
 public:
  Noise() = delete;
  Noise(SceneInfo* scene_info);
  ~Noise() override;
  void process() override;
  enum class InSlotName {
    Scale,
    Detail,
    Roughness
  };
  enum class OutSlotName {
    Color = 0,
    Gradient
  };
  SceneInfo* scene_info_;
  bool inverse_direction_{false};
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

} // namespace sparks