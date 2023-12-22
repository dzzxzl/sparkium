#pragma once
#include "vector"
#include "glm/glm.hpp"
#include "grassland/grassland.h"
#include "sparks/assets/hit_record.h"
#include "sparks/assets/scene.h"

namespace sparks {

class Node;

class SceneInfo {
  public:
   SceneInfo(const Scene* scene, HitRecord hit_record, LightRecord light_record);
   const Scene* scene_;
   HitRecord hit_record_;
   LightRecord light_record_;
};

class Slot {
 public:
  Slot() = default;
  enum SlotType { input = 0, output = 1 };
  Slot(SlotType slotType): slotType_(slotType){};
  virtual ~Slot() = default;
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
  float value_{0.0f};
};

class Vec3Slot: public Slot {
 public:
  Vec3Slot() = default;
  Vec3Slot(glm::vec3 value): value_(value){};
  ~Vec3Slot() override = default;
  glm::vec3 value_{0.0f, 0.0f, 0.0f};
};

class EnumSlot: public Slot {
 public:
  EnumSlot() = default;
  EnumSlot(int value): value_(value){};
  ~EnumSlot() override = default;
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
  virtual void process() = 0;
  int id_;
  std::vector<Slot*> in_slots_;
  std::vector<Slot*> out_slots_;
  void link(Node* next_node, int out_slot_id, int in_slot_id);
  Slot* getOutput(int out_slot_id);
  Slot* getInput(int in_slot_id);
};

void setFloat(Slot* slot, float value);
void setVec3(Slot* slot, glm::vec3 value);
void setEnum(Slot* slot, int value);
void getFloat(Slot* slot);
void getVec3(Slot* slot);
void getEnum(Slot* slot);


} // namespace sparks
