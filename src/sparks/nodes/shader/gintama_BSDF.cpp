#include "sparks/nodes/shader/gintama_BSDF.h"
#include "gintama_BSDF.h"

namespace sparks {

DiffuseBSDF::DiffuseBSDF( SceneInfo* scene_info): scene_info_(scene_info)
{
    in_slots_.resize(2);
    out_slots_.resize(1);
    in_slots_[slotID(InSlotName::Color)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[slotID(InSlotName::Normal)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Color)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Color)]->slotType_ = Slot::SlotType::output;
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
    glm::vec3 normal = scene_info_->hit_record_.geometry_normal;
    auto material = scene_info_->scene_->GetEntity(scene_info_->hit_record_.hit_entity_id).GetMaterial();
    auto shade_smooth = material.shade_smooth;
    if(shade_smooth) {
        normal = scene_info_->hit_record_.normal;
    }
    if(in_slots_[ slotID(InSlotName::Normal) ]->lastNode_ != nullptr) {
        normal = static_cast<Vec3Slot*>(in_slots_[ slotID(InSlotName::Normal) ])->value_;
    }
    glm::vec3 in_color = getVec3(in_slots_[ slotID(InSlotName::Color) ]);
    glm::vec3 incident_ = scene_info_->light_record_.incident;
    glm::vec3 reflected_ = scene_info_->light_record_.reflected;

    glm::vec3 res_color = in_color / PI;

    float diffusive_factor = std::max(glm::dot(normal, -incident_), 0.0f) * std::max(glm::dot(normal, reflected_), 0.0f);

    if (diffusive_factor == 0.0f) {
        res_color = glm::vec3(0.0f);
    } else {
        float geo_fac = glm::dot( -incident_, scene_info_->hit_record_.geometry_normal );
        float act_fac = glm::dot( -incident_, normal );
        res_color *= act_fac / geo_fac;
    }
    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_color;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_color;
    }
}

GlossyBSDF::GlossyBSDF(SceneInfo * scene_info)
{
    scene_info_ = scene_info;
    in_slots_.resize(4);
    out_slots_.resize(1);
    in_slots_[slotID(InSlotName::Color)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[slotID(InSlotName::Normal)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[slotID(InSlotName::Roughness)] = new FloatSlot(0.0f);
    in_slots_[slotID(InSlotName::Anisotropy)] = new FloatSlot(0.0f);
    out_slots_[slotID(OutSlotName::Color)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Color)]->slotType_ = Slot::SlotType::output;
}

GlossyBSDF::~GlossyBSDF()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void GlossyBSDF::process()
{
    // process all input slots
    for(int i=0; i<4; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    glm::vec3 normal = scene_info_->hit_record_.geometry_normal;
    auto material = scene_info_->scene_->GetEntity(scene_info_->hit_record_.hit_entity_id).GetMaterial();
    auto shade_smooth = material.shade_smooth;
    if(shade_smooth) {
        normal = scene_info_->hit_record_.normal;
    }
    if(in_slots_[ slotID(InSlotName::Normal) ]->lastNode_ != nullptr) {
        normal = static_cast<Vec3Slot*>(in_slots_[ slotID(InSlotName::Normal) ])->value_;
    }
    glm::vec3 in_color = getVec3(in_slots_[ slotID(InSlotName::Color) ]);
    glm::vec3 incident_ = scene_info_->light_record_.incident;
    glm::vec3 reflected_ = scene_info_->light_record_.reflected;
    glm::vec3 half = glm::normalize(-incident_ + reflected_);
    auto rough_x = getFloat(in_slots_[ slotID(InSlotName::Roughness) ]);
    auto rough_y = (getFloat(in_slots_[ slotID(InSlotName::Anisotropy) ]) + 1.0f) * rough_x;
    glm::vec3 ey = scene_info_->hit_record_.tangent;
    glm::vec3 interm = glm::cross(ey, normal);
    // if(glm::length(interm) < 0.0001f) {
    //     ey = glm::normalize(glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f)));
    // }
    glm::vec3 ex = glm::normalize(glm::cross(ey, normal));
    ey = glm::normalize(glm::cross(normal, ex));

    glm::vec3 res_color = frGlossy( in_color, normal, half, reflected_, -incident_, rough_x, rough_y, ex, ey );
    // LAND_INFO("in color: {}", in_color.x);
    // LAND_INFO("res color: {}", res_color.x);
    float diffusive_factor = std::max(glm::dot(normal, -incident_), 0.0f) * std::max(glm::dot(normal, reflected_), 0.0f);

    if (diffusive_factor == 0.0f) {
        res_color = glm::vec3(0.0f);
    } else {
        float geo_fac = glm::dot( -incident_, scene_info_->hit_record_.geometry_normal );
        float act_fac = glm::dot( -incident_, normal );
        res_color *= act_fac / geo_fac;
    }

    static_cast<Vec3Slot*>(out_slots_[0])->value_ = res_color;
    // push to next node
    if(out_slots_[0]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[0]->nextNode_->in_slots_[out_slots_[0]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = res_color;
    }
}

}