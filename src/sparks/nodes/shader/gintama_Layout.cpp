#include "sparks/nodes/shader/gintama_Layout.h"
#include "gintama_Layout.h"

namespace sparks{

int Node::next_id_ = 0;

void Node::link(Node* next_node, int out_slot_id, int in_slot_id){
    // in_slots_[in_slot_id]->nextNode_ = next_node;
    // in_slots_[in_slot_id]->nextSlotId_ = out_slot_id;
    // next_node->in_slots_[out_slot_id]->lastNode_ = this;
    // next_node->in_slots_[out_slot_id]->lastSlotId_ = in_slot_id;
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

} // namespace sparks