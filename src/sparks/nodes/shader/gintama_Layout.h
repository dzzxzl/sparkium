#pragma once
#include "vector"

namespace sparks {

class Slot {
 public:
  Slot() = default;
  enum SlotType { input = 0, output = 1 };
  Slot(SlotType slotType): slotType_(slotType){};
  virtual ~Slot() = default;
  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void DrawImGui() = 0;
  SlotType slotType_{input};
  Slot* lastSlot_{nullptr};
  Slot* nextSlot_{nullptr};
  void link(Slot* nextSlot);
};

class FloatSlot: public Slot {
 public:
  FloatSlot() = default;
  ~FloatSlot() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  float value_{0.0f};
};

class Vec3Slot: public Slot {
 public:
  Vec3Slot() = default;
  ~Vec3Slot() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  float value_[3]{0.0f, 0.0f, 0.0f};
};

class Prober {
    public:
    Prober() = default;
    ~Prober() = default;
};

class ProberSlot: public Slot {
 public:
  ProberSlot() = default;
  ~ProberSlot() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  Prober prober_;
};

class EnumSlot: public Slot {
 public:
  EnumSlot() = default;
  ~EnumSlot() override = default;
  void Update() override;
  void Draw() override;
  void DrawImGui() override;
  int value_{0};
};

class Node {
 public:
  static int next_id_;
  Node(){
    id_ = next_id_++;
  };
  virtual ~Node(){
    next_id_--;
  };
  virtual void Update() = 0;
  virtual void Draw() = 0;
  virtual void DrawImGui() = 0;
  int id_;
  std::vector<Slot*> in_slots_;
  std::vector<Slot*> out_slots_;
};


} // namespace sparks
