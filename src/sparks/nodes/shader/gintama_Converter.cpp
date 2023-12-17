#include "sparks/nodes/shader/gintama_Converter.h"

namespace sparks{

MixShader::MixShader()
{
    in_slots_.resize(3);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    in_slots_[0] = new FloatSlot(0.5f);
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[2] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}
MixShader::~MixShader() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}
Multiplier::Multiplier()
{
    in_slots_.resize(2);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    in_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}
Multiplier::~Multiplier() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

} // namespace sparks