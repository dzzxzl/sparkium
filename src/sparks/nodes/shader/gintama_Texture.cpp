#include "sparks/nodes/shader/gintama_Texture.h"
#include "gintama_Texture.h"

namespace sparks{

Checker::Checker( SceneInfo* scene_info): scene_info_(scene_info)
{
    in_slots_.resize(3);
    out_slots_.resize(2);
    in_slots_[0] = new Vec3Slot(glm::vec3(1.0f));
    in_slots_[1] = new Vec3Slot(glm::vec3(0.0f));
    in_slots_[2] = new FloatSlot(10.0f);
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[1] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
    out_slots_[1]->slotType_ = Slot::SlotType::output;
}

Checker::~Checker() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void Checker::process() {
    // process all input slots
    for(int i=0; i<3; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    auto scale = static_cast<FloatSlot*>(in_slots_[2])->value_;
    auto color1 = static_cast<Vec3Slot*>(in_slots_[0])->value_;
    auto color2 = static_cast<Vec3Slot*>(in_slots_[1])->value_;

    auto color1_len = glm::length(color1);
    auto color2_len = glm::length(color2);

    float grid_w = 1.0f / scale;

    float dx = grid_w / 20.0f;

    auto u_ = scene_info_->hit_record_.tex_coord.x;
    auto v_ = scene_info_->hit_record_.tex_coord.y;
    auto entity_id = scene_info_->hit_record_.hit_entity_id;
    auto texture_id_ = scene_info_->scene_->GetEntity(entity_id).GetMaterial().albedo_texture_id;
    auto texture_width = scene_info_->scene_->GetTexture(texture_id_).GetWidth();
    auto texture_height = scene_info_->scene_->GetTexture(texture_id_).GetHeight();
    u_ = u_ / texture_width;
    v_ = v_ / texture_height;

    int grid_x = std::floor(u_ / grid_w);
    int grid_xdx = std::floor((u_ + dx) / grid_w);
    int grid_y = std::floor(v_ / grid_w);
    int grid_ydy = std::floor((v_ + dx) / grid_w);

    auto curColor = (grid_x + grid_y) % 2 == 0 ? color1_len : color2_len;
    auto curColorDx = (grid_xdx + grid_y) % 2 == 0 ? color1_len : color2_len;
    auto curColorDy = (grid_x + grid_ydy) % 2 == 0 ? color1_len : color2_len;
    auto diffx = 0.0f;
    auto diffy = 0.0f;
    auto diffz = 1.0f;
    if(curColor > curColorDx){
        diffx = 1.0f;
        diffz = 0.0f;
    }
    if(curColor > curColorDy){
        diffy = 1.0f;
        diffz = 0.0f;
    }
    if(curColor < curColorDx){
        diffx = -1.0f;
        diffz = 0.0f;
    }
    if(curColor < curColorDy){
        diffy = -1.0f;
        diffz = 0.0f;
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = (grid_x + grid_y) % 2 == 0 ? color1 : color2;
    static_cast<Vec3Slot*>(out_slots_[1])->value_ = glm::vec3(diffx, diffy, diffz);
    // push to next node
    for(int i=0; i<2; i++){
        if(out_slots_[i]->nextNode_ != nullptr) {
            auto next_slot = out_slots_[i]->nextNode_->in_slots_[out_slots_[i]->nextSlotId_];
            static_cast<Vec3Slot*>(next_slot)->value_ = static_cast<Vec3Slot*>(out_slots_[i])->value_;
        }
    }

}

}
