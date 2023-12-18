#include "sparks/nodes/shader/gintama_Inputs.h"
#include "gintama_Inputs.h"


namespace sparks {

AmbientOcclusion::AmbientOcclusion() {

    in_slots_.resize(1);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;

}

TextureSample::TextureSample(const Scene *scene,
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
}

void TextureSample::process() {
    // just sample
    glm::vec3 sample( scene_->GetTextures()[texture_id_].Sample({u_,v_}) );
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = sample;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = sample;
    }
}

}  // namespace sparks