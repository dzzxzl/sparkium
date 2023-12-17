#include "sparks/nodes/shader/gintama_Inputs.h"
#include "gintama_Inputs.h"


namespace sparks {

AmbientOcclusion::AmbientOcclusion() {

    in_slots_.resize(1);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;

}

TextureSample::TextureSample(Scene *scene,
                            int entity_id,
                            int texture_id, float u, float v):
                            scene_(scene), entity_id_(entity_id), texture_id_(texture_id), u_(u), v_(v) {
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
}

TextureSample::~TextureSample() {
    for (auto& slot : out_slots_) {
        delete slot;
    }
}  // namespace sparks