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
  // int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
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
  // int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
};

}