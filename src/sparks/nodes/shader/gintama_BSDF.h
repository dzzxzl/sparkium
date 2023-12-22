#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"

namespace sparks {

class DiffuseBSDF: public Node {
 public:
  DiffuseBSDF() = delete;
  DiffuseBSDF(SceneInfo* scene_info);
  ~DiffuseBSDF() override;
  enum class InSlotName {
    Color = 0,
    Normal
  };
  enum class OutSlotName {
    Color = 0
  };
  void process() override;
  SceneInfo* scene_info_;
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
};


}