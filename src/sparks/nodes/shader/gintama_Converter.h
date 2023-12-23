#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class MixShader: public Node {
 public:
  MixShader();
  ~MixShader() override;
  enum class InSlotName {
    Factor = 0,
    Color1,
    Color2
  };
  enum class OutSlotName {
    Color = 0
  };
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class Multiplier: public Node {
 public:
  Multiplier();
  ~Multiplier() override;
  void process() override;
  enum class InSlotName {
    Color1 = 0,
    Color2
  };
  enum class OutSlotName {
    Color = 0
  };
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
  Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

} // namespace sparks