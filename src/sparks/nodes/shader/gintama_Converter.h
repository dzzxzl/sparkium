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

class ColorClamp: public Node {
  public:
    ColorClamp();
    ~ColorClamp() override;
    void process() override;
    enum class InSlotName {
      Color = 0
    };
    enum class OutSlotName {
      Color = 0
    };
    std::vector<glm::vec3> colors_;
    std::vector<float> values_;
    glm::vec3 start_color_{0.0f, 0.0f, 0.0f};
    glm::vec3 end_color_{1.0f, 1.0f, 1.0f};
    bool grayscale_{true};
    void setColor( glm::vec3 color, float value );
    int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
    int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
    Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
    Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class ColorRamp: public Node {
  public:
    ColorRamp();
    ~ColorRamp() override;
    void process() override;
    enum class InSlotName {
      Color = 0
    };
    enum class OutSlotName {
      Color = 0
    };
    enum class Interpolation {
      Linear = 0,
      Constant = 1,
      Ease = 2,
      Bounce = 3,
      Bezier = 4
    };
    std::vector<glm::vec3> colors_;
    std::vector<float> values_;
    glm::vec3 start_color_{0.0f, 0.0f, 0.0f};
    glm::vec3 end_color_{1.0f, 1.0f, 1.0f};
    bool grayscale_{true};
    Interpolation interpolation_{Interpolation::Linear};
    void setColor( glm::vec3 color, float value );
    int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
    int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
    Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
    Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class PerturbNormal: public Node {
  public:
    PerturbNormal(SceneInfo* scene_info);
    ~PerturbNormal() override;
    void process() override;
    enum class InSlotName {
      Normal = 0,
      PerturbDirection,
      Scale,
      Fac
    };
    enum class OutSlotName {
      Normal = 0
    };
    int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
    int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
    Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
    Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
    SceneInfo* scene_info_;
};

class Inverter: public Node {
  public:
    Inverter();
    ~Inverter() override;
    void process() override;
    enum class InSlotName {
      Fac = 0
    };
    enum class OutSlotName {
      Fac = 0
    };
    bool invert_{true};
    int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
    int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
    Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
    Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

class Color2Fac: public Node {
  public:
    Color2Fac();
    ~Color2Fac() override;
    void process() override;
    enum class InSlotName {
      Color = 0
    };
    enum class OutSlotName {
      Fac = 0
    };
    int slotID(OutSlotName slot_name) { return static_cast<int>(slot_name); }
    int slotID(InSlotName slot_name) { return static_cast<int>(slot_name); }
    Slot * getOutSlot(OutSlotName slot_name) { return out_slots_[slotID(slot_name)]; }
    Slot * getInSlot(InSlotName slot_name) { return in_slots_[slotID(slot_name)]; }
};

} // namespace sparks