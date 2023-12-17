#include "sparks/nodes/shader/gintama_BSDF.h"

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
}
