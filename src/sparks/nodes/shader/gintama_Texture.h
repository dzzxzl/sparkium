#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "glm/glm.hpp"
#include "cmath"
#include "sparks/nodes/shader/gintama_assets.h"

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

class NoiseTexture: public Node {
 public:
  NoiseTexture() = delete;
  NoiseTexture(SceneInfo* scene_info);
  ~NoiseTexture() override;
  void process() override;
  enum class InSlotName {
    Scale_Grid,
    Scale_Noise,
    Detail,
    Roughness,
    PerlinID
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