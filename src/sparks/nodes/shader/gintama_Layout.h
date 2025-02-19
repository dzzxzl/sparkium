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
   ~SceneInfo() = default;
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
};

void setFloat(Slot* slot, float value);
void setVec3(Slot* slot, glm::vec3 value);
void setEnum(Slot* slot, int value);
float getFloat(Slot* slot);
glm::vec3 getVec3(Slot* slot);
int getEnum(Slot* slot);

// class BSDFShader {
//   public:
//     BSDFShader() = default;
//     BSDFShader(glm::vec3 *throughput, glm::vec3 *radiance, SceneInfo* scene_info);
//     ~BSDFShader();
//     void sampleLight();
//     void sampleEnv();
//     glm::vec3 sampleScatter();
//     glm::vec3 getFr();
//     void setShaderByPreset(MaterialType material_type);
//     glm::vec3 *throughput_{};
//     glm::vec3 *radiance_{};
//     float cur_weight_{};
//     SceneInfo* scene_info_{};
//     std::vector<Node*> nodes_;
// };


} // namespace sparks
