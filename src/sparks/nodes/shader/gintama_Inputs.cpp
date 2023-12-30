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

TextureSampleAbsolute::TextureSampleAbsolute(SceneInfo * scene_info): scene_info_(scene_info)
{
    out_slots_.resize(2);
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    out_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[1]->slotType_ = Slot::SlotType::output;
}

TextureSampleAbsolute::~TextureSampleAbsolute()
{
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void TextureSampleAbsolute::process()
{
    // just sample
    auto scene_ = scene_info_->scene_;
    auto entity_id = scene_info_->hit_record_.hit_entity_id;
    auto texture_id_ = scene_->GetEntity(entity_id).GetMaterial().albedo_texture_id;
    auto u_ = scene_info_->hit_record_.tex_coord.x;
    auto v_ = scene_info_->hit_record_.tex_coord.y;
    // LAND_INFO("u: {}, v: {}", u_, v_);
    float sample_u = offset_x + ( u_ / scale_x );
    float sample_v = offset_y + ( v_ / scale_y );
    if(sample_u > 1.0f) {
        sample_u -= 1.0f;
    }
    if(sample_v > 1.0f) {
        sample_v -= 1.0f;
    }
    glm::vec3 sample( scene_->GetTextures()[texture_id_].Sample({sample_u, sample_v}) );
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = sample;
    // evaluate gradient
    // float dx = 1e-3f;
    // float sample_u_dx = offset_x + ( (u_ + dx) / scale_x );
    // glm::vec3 sample_dx( scene_->GetTextures()[texture_id_].Sample({sample_u_dx, sample_v}) );
    // float diffx = glm::length( (sample_dx - sample) / dx ) / sqrt(3.0f);
    // float dy = 1e-3f;
    // float sample_v_dy = offset_y + ( (v_ + dy) / scale_y );
    // glm::vec3 sample_dy( scene_->GetTextures()[texture_id_].Sample({sample_u, sample_v_dy}) );
    // float diffy = glm::length( (sample_dy - sample) / dy ) / sqrt(3.0f);
    // glm::vec3 grad_result(diffx, diffy, 1.0f);
    // grad_result = glm::normalize(grad_result);
    // static_cast<Vec3Slot*>(out_slots_[1])->value_ = grad_result;
    // push to next node
    for(int i=0; i<2; i++){
        if(out_slots_[i]->nextNode_ != nullptr) {
            auto next_slot = out_slots_[i]->nextNode_->in_slots_[out_slots_[i]->nextSlotId_];
            static_cast<Vec3Slot*>(next_slot)->value_ = static_cast<Vec3Slot*>(out_slots_[i])->value_;
        }
    }
}

}  // namespace sparks