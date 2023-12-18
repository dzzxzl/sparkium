#include "sparks/nodes/shader/gintama_BSDF.h"
#include "gintama_BSDF.h"

namespace sparks {

DiffuseBSDF::DiffuseBSDF()
{
    in_slots_.resize(2);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}
DiffuseBSDF::~DiffuseBSDF() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}
void DiffuseBSDF::process() {
    // process all input slots
    for(int i=0; i<2; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 normal = static_cast<Vec3Slot*>(in_slots_[1])->value_;
    glm::vec3 in_color = static_cast<Vec3Slot*>(in_slots_[0])->value_;

    float diffusive_factor = std::max( glm::dot(reflected_, normal), 0.0f) * std::max( glm::dot(-incident_, normal), 0.0f);
    glm::vec3 res_color = diffusive_factor * in_color;
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_color;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_color;
    }
}

}