#pragma once
#include "vector"
#include "glm/glm.hpp"
#include "grassland/grassland.h"

namespace sparks {

class Node;

class Slot {
 public:
  Slot() = default;
  enum SlotType { input = 0, output = 1 };
  Slot(SlotType slotType): slotType_(slotType){};
  virtual ~Slot() = default;
  // virtual void Update() = 0;
  // virtual void Draw() = 0;
  // virtual void DrawImGui() = 0;
  SlotType slotType_{input};
  Node* lastNode_{nullptr};
  int lastSlotId_{-1};
  Node* nextNode_{nullptr};
  int nextSlotId_{-1};
};

class FloatSlot: public Slot {
 public:
  FloatSlot() = default;
  FloatSlot(float value): value_(value){};
  ~FloatSlot() override = default;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  float value_{0.0f};
};

class Vec3Slot: public Slot {
 public:
  Vec3Slot() = default;
  Vec3Slot(glm::vec3 value): value_(value){};
  ~Vec3Slot() override = default;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
  glm::vec3 value_{0.0f, 0.0f, 0.0f};
};

// class Prober {
//     public:
//     Prober() = default;
//     Prober(Scene* scene, int entity_id, int texture_id): scene_(scene), entity_id_(entity_id), texture_id_(texture_id){};
//     ~Prober() = default;
//     Scene* scene_;
//     int entity_id_{-1};
//     int texture_id_{-1};
// };

// class ProberSlot: public Slot {
//  public:
//   ProberSlot() = default;
//   ~ProberSlot() override = default;
//   void Update() override;
//   void Draw() override;
//   void DrawImGui() override;
//   Prober prober_;
// };

class EnumSlot: public Slot {
 public:
  EnumSlot() = default;
  EnumSlot(int value): value_(value){};
  ~EnumSlot() override = default;
  // void Update() override;
  // void Draw() override;
  // void DrawImGui() override;
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
  // virtual void Update() = 0;
  // virtual void Draw() = 0;
  // virtual void DrawImGui() = 0;
  virtual void process() = 0;
  // virtual void process_recursive();
  int id_;
  std::vector<Slot*> in_slots_;
  std::vector<Slot*> out_slots_;
  void link(Node* next_node, int out_slot_id, int in_slot_id);
  Slot* getOutput(int out_slot_id);
  Slot* getInput(int in_slot_id);
};

// class EndNode: public Node {
//  public:
//   EndNode() = default;
//   ~EndNode() override = default;
//   void Update() override;
//   void Draw() override;
//   void DrawImGui() override;
// };

void setFloat(Slot* slot, float value);
void setVec3(Slot* slot, glm::vec3 value);
void setEnum(Slot* slot, int value);
void getFloat(Slot* slot);
void getVec3(Slot* slot);
void getEnum(Slot* slot);


} // namespace sparks
