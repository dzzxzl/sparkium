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

class TextureSampleAbsolute: public Node {
 public:
  TextureSampleAbsolute() = delete;
  TextureSampleAbsolute(SceneInfo* scene_info);
  ~TextureSampleAbsolute() override;
  void process() override;
  enum class OutSlotName {
    Color = 0,
    Gradient
  };
  SceneInfo* scene_info_;
  float scale_x{2.0f};
  float scale_y{2.0f};
  float offset_x{0.0f};
  float offset_y{0.0f};
  float detail_{1.0f};
  float roughness_{1.0f};
  int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
  Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
};

}