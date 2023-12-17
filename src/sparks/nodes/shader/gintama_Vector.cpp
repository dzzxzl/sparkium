#include "sparks/nodes/shader/gintama_Vector.h"
#include "gintama_Vector.h"

namespace sparks {

Bump::Bump()
{
    in_slots_.resize(4);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    in_slots_[0] = new FloatSlot(1.0f);
    in_slots_[1] = new EnumSlot(0);
    in_slots_[2] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[3] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}
Bump::~Bump() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

Color::Color(glm::vec3 color): color_(color)
{
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}

Color::~Color() {
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

}  // namespace sparks
