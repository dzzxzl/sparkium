#include "sparks/nodes/shader/gintama_Layout.h"
#include "gintama_Layout.h"

namespace sparks{

int Node::next_id_ = 0;

void Node::link(Node* next_node, int out_slot_id, int in_slot_id){
    out_slots_[out_slot_id]->nextNode_ = next_node;
    out_slots_[out_slot_id]->nextSlotId_ = in_slot_id;
    next_node->in_slots_[in_slot_id]->lastNode_ = this;
    next_node->in_slots_[in_slot_id]->lastSlotId_ = out_slot_id;
}

Slot* Node::getOutput(int out_slot_id){
    return out_slots_[out_slot_id];
}

Slot* Node::getInput(int in_slot_id){
    return in_slots_[in_slot_id];
}

void setFloat(Slot* slot, float value){
    static_cast<FloatSlot*>(slot)->value_ = value;
}

void setVec3(Slot* slot, glm::vec3 value){
    static_cast<Vec3Slot*>(slot)->value_ = value;
}

void setEnum(Slot* slot, int value){
    static_cast<EnumSlot*>(slot)->value_ = value;
}

float getFloat(Slot* slot){
    return static_cast<FloatSlot*>(slot)->value_;
}

glm::vec3 getVec3(Slot* slot){
    return static_cast<Vec3Slot*>(slot)->value_;
}

int getEnum(Slot* slot){
    return static_cast<EnumSlot*>(slot)->value_;
}

SceneInfo::SceneInfo(const Scene* scene, HitRecord hit_record, LightRecord light_record): scene_(scene), hit_record_(hit_record), light_record_(light_record){
}


} // namespace sparks