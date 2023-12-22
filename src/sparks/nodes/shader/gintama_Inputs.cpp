#include "sparks/nodes/shader/gintama_Inputs.h"
#include "gintama_Inputs.h"


namespace sparks {

AmbientOcclusion::AmbientOcclusion() {

    in_slots_.resize(1);
    out_slots_.resize(1);
    out_slots_[0]->slotType_ = Slot::SlotType::output;

}

TextureSample::TextureSample( SceneInfo* scene_info): scene_info_(scene_info) {
    out_slots_.resize(1);
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}

TextureSample::~TextureSample() {
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void TextureSample::process() {
    // just sample
    auto scene_ = scene_info_->scene_;
    auto entity_id = scene_info_->hit_record_.hit_entity_id;
    auto texture_id_ = scene_->GetEntity(entity_id).GetMaterial().albedo_texture_id;
    auto u_ = scene_info_->hit_record_.tex_coord.x;
    auto v_ = scene_info_->hit_record_.tex_coord.y;
    glm::vec3 sample( scene_->GetTextures()[texture_id_].Sample({u_,v_}) );
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = sample;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = sample;
    }
}

}  // namespace sparks