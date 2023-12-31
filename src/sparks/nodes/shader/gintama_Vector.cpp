#include "sparks/nodes/shader/gintama_Vector.h"
#include "gintama_Vector.h"

namespace sparks {

Bump::Bump( SceneInfo* scene_info ): scene_info_(scene_info)
{
    in_slots_.resize(4);
    out_slots_.resize(1);
    in_slots_[0] = new FloatSlot(1.0f);
    in_slots_[1] = new EnumSlot(0);
    in_slots_[2] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[3] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}
Bump::~Bump() {
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}
void Bump::process() {
    // process all input slots
    for(int i=0; i<4; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    float scale = static_cast<FloatSlot*>(in_slots_[0])->value_;
    int type = static_cast<EnumSlot*>(in_slots_[1])->value_;
    glm::vec3 height = static_cast<Vec3Slot*>(in_slots_[2])->value_;
    glm::vec3 grad = static_cast<Vec3Slot*>(in_slots_[3])->value_;
    auto tangent_ = scene_info_->hit_record_.tangent;
    auto normal_ = scene_info_->hit_record_.geometry_normal;
    auto material = scene_info_->scene_->GetEntity(scene_info_->hit_record_.hit_entity_id).GetMaterial();
    auto shade_smooth = material.shade_smooth;
    if(shade_smooth) {
        normal_ = scene_info_->hit_record_.normal;
    }
    auto reflection_ = scene_info_->light_record_.reflected;
    glm::vec3 e_v = glm::normalize( glm::cross(tangent_, normal_) );
    glm::vec3 e_u = glm::normalize( glm::cross( normal_, e_v ) );
    glm::vec3 out_normal = normal_;
    if(type == 0) {
        // height displacement
        out_normal = - grad[0] * e_u + grad[1] * e_v + grad[2] * normal_;
        // out_normal = normal_;
        // out_normal = grad[0] * e_v + grad[1] * e_u + grad[2] * normal_;
        out_normal = glm::normalize(out_normal);
        if (glm::dot(out_normal, reflection_) < 0.0f) {
            out_normal = normal_;
        }
    } else if(type == 1) {
        // normal rectification
        bool outward = true;
        if( height[0] < 0.5f ) {
            outward = false;
        }
        if(!outward) {
            out_normal = -out_normal;
        }
    } else if(type == 2) {
        float dx = 1e-2f;
        scene_info_->hit_record_.tex_coord.x += dx;
        flush();
        glm::vec3 height_dx = static_cast<Vec3Slot*>(in_slots_[2])->value_;
        scene_info_->hit_record_.tex_coord.y += dx;
        scene_info_->hit_record_.tex_coord.x -= dx;
        flush();
        glm::vec3 height_dy = static_cast<Vec3Slot*>(in_slots_[2])->value_;
        scene_info_->hit_record_.tex_coord.y -= dx;
        float diffx = glm::length( (height_dx - height) / dx ) / sqrt(3.0f);
        float diffy = glm::length( (height_dy - height) / dx ) / sqrt(3.0f);
        auto bump_strength = getFloat(in_slots_[0]);
        out_normal = (- diffx * e_u + diffy * e_v) * bump_strength + 1.0f * normal_;
        out_normal = glm::normalize(out_normal);
        if (glm::dot(out_normal, reflection_) < 0.0f) {
            out_normal = normal_;
        }
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = out_normal;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = out_normal;
    }
}

void Bump::flush()
{
    for(int i=0; i<4; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
}

Color::Color(glm::vec3 color): color_(color)
{
    out_slots_.resize(1);
    out_slots_[0] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[0]->slotType_ = Slot::SlotType::output;
}


Color::~Color() {
    for (auto& slot : out_slots_) {
        delete slot;
    }
}
void Color::process() {
    // process current node
    glm::vec3 res_color = color_;
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_color;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_color;
    }
}

}  // namespace sparks
