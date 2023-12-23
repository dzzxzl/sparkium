#pragma once

#include "vector"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "sparks/assets/scene.h"

namespace sparks {

class AmbientOcclusion: public Node {
 public:
  AmbientOcclusion();
  ~AmbientOcclusion() override = default;
  float depth_{1.0f};
};

class TextureSample: public Node {
 public:
  TextureSample() = delete;
  TextureSample(SceneInfo* scene_info);
  ~TextureSample() override;
  void process() override;
  enum class OutSlotName {
    Color = 0
  };
  SceneInfo* scene_info_;
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
};

}