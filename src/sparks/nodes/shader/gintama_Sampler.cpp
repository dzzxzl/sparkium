#include "sparks/nodes/shader/gintama_Sampler.h"



sparks::SpecularSampler::SpecularSampler(SceneInfo * scene_info): scene_info_(scene_info)
{
    out_slots_.resize(2);
    out_slots_[slotID(OutSlotName::Direction)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Weight)] = new FloatSlot(0.0f);
    out_slots_[slotID(OutSlotName::Direction)]->slotType_ = Slot::SlotType::output;
    out_slots_[slotID(OutSlotName::Weight)]->slotType_ = Slot::SlotType::output;
}

sparks::SpecularSampler::~SpecularSampler()
{
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void sparks::SpecularSampler::process()
{
    glm::vec3 out_direction = scene_info_->light_record_.reflected;
    glm::vec3 normal = scene_info_->hit_record_.geometry_normal;
    auto entity_id = scene_info_->hit_record_.hit_entity_id;
    auto material = scene_info_->scene_->GetEntity(entity_id).GetMaterial();
    
}

sparks::RoughGlassSampler::RoughGlassSampler(SceneInfo * scene_info): scene_info_(scene_info)
{
    in_slots_.resize(3);
    out_slots_.resize(2);
    in_slots_[slotID(InSlotName::Normal)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    in_slots_[slotID(InSlotName::IOR)] = new FloatSlot(1.0f);
    in_slots_[slotID(InSlotName::Roughness)] = new FloatSlot(0.0f);
    out_slots_[slotID(OutSlotName::Direction)] = new Vec3Slot(glm::vec3(0.0f, 0.0f, 0.0f));
    out_slots_[slotID(OutSlotName::Weight)] = new FloatSlot(0.0f);
    out_slots_[slotID(OutSlotName::Direction)]->slotType_ = Slot::SlotType::output;
    out_slots_[slotID(OutSlotName::Weight)]->slotType_ = Slot::SlotType::output;
}

sparks::RoughGlassSampler::~RoughGlassSampler()
{
    for (auto& slot : in_slots_) {
        delete slot;
    }
    for (auto& slot : out_slots_) {
        delete slot;
    }
}

void sparks::RoughGlassSampler::process()
{
    // process all input slots
    for(int i=0; i<3; i++){
        if(in_slots_[i]->lastNode_ != nullptr) {
            in_slots_[i]->lastNode_->process();
        }
    }
    // process current node
    float ret_weight;
    glm::vec3 ret_direction;
    float roughness = static_cast<FloatSlot*>(in_slots_[ slotID(InSlotName::Roughness) ])->value_;
    auto material = scene_info_->scene_->GetEntity(scene_info_->hit_record_.hit_entity_id).GetMaterial();
    auto hit_record = scene_info_->hit_record_;
    auto reflection = scene_info_->light_record_.reflected;
    auto normal = hit_record.geometry_normal;
    if(in_slots_[ slotID(InSlotName::Normal)]->lastNode_ != nullptr) {
        normal = static_cast<Vec3Slot*>(in_slots_[ slotID(InSlotName::Normal) ])->value_;
    }
    if (roughness < 1e-3f) {
        // treat as glass
        auto IOR = material.IOR;
        bool in_glass = ! ( hit_record.front_face ^ material.inverse_normal );
        glm::vec3 glass_normal = normal;
        // if(material.shade_smooth){
        // glass_normal = hit_record.normal;
        // }
        // bool in_glass = glm::dot(-reflection, hit_record.geometry_normal) > 0.0f;
        if(in_glass) {
            float cos_theta = glm::dot(reflection, glass_normal);
            float eta = IOR;
            float R_0 = (1 - eta) / (1 + eta);
            R_0 = R_0 * R_0;
            float R = R_0 + (1 - R_0) * glm::pow(1 - cos_theta, 5);
            if( 1 - eta * eta * (1 - cos_theta * cos_theta) < 0.0f ) {
                // return glm::vec3(0.0f);
                // return glm::vec4(glm::reflect(-reflection, glass_normal), 1.0f);
                ret_weight = 1.0f;
                ret_direction = glm::reflect(-reflection, glass_normal);
                goto push;
            }
            if(genRandFloat(0,1) < R) {
                // return glm::vec4(glm::reflect(-reflection, glass_normal),1.0f);
                ret_weight = 1.0f;
                ret_direction = glm::reflect(-reflection, glass_normal);
                goto push;
            }
            // return glm::vec4(glm::refract(-reflection, glass_normal, IOR),1.0f);
            ret_weight = 1.0f;
            ret_direction = glm::refract(-reflection, glass_normal, IOR);
            goto push;
        } else { // out glass
            float cos_theta = glm::dot(reflection, glass_normal);
            float eta = 1.0f / IOR;
            float R_0 = (1 - eta) / (1 + eta);
            R_0 = R_0 * R_0;
            float R = R_0 + (1 - R_0) * glm::pow(1 - cos_theta, 5);
            // R = 0.5;
            //     ret_weight = 1.0f;
            //     ret_direction = glm::reflect(-reflection, glass_normal);
            //     // ret_direction = glm::reflect(-reflection, hit_record.normal);
            //     goto push;
            if( 1 - eta * eta * (1 - cos_theta * cos_theta) < 0.0f ) {
                // return glm::vec3(0.0f);
                // return glm::vec4(glm::reflect(-reflection, glass_normal),1.0f);
                ret_weight = 1.0f;
                ret_direction = glm::reflect(-reflection, glass_normal);
                goto push;
            }
            if(genRandFloat(0,1) < R) {
                // return glm::vec4(glm::reflect(-reflection, glass_normal),1.0f);
                ret_weight = 1.0f;
                ret_direction = glm::reflect(-reflection, glass_normal);
                goto push;
            }
            //return glm::vec4(glm::refract(-reflection, glass_normal, 1.0f / IOR),1.0f);
            ret_weight = 1.0f;
            ret_direction = glm::refract(-reflection, glass_normal, 1.0f / IOR);
            goto push;
            }
    } else {
        // treat as rough glass
        glm::vec3 normal = hit_record.geometry_normal;
        if(material.shade_smooth) {
            normal = hit_record.normal;
        }
        glm::vec half = genRandVec3();
        while(true) {
            float rand_p = genRandFloat(0,1);
            float ggx_D = GGX_D(normal, half, material.roughness);
            if(rand_p < ggx_D) {
                break;
            }
            half = genRandVec3();
        }
        bool in_glass = ! ( hit_record.front_face ^ material.inverse_normal );
        float eta = 1.0f / material.IOR;
        if(in_glass) {
            eta = material.IOR;
        }
        float R = reflectFresnel(normal, -reflection, eta);
        float rand_p = genRandFloat(0,1);
        glm::vec3 new_direction = glm::reflect(-reflection, half);
        if(rand_p > R) {
            new_direction = glm::refract(-reflection, half, eta);
        }
        float cos_im = glm::dot(-reflection, half);
        float cos_in = glm::dot(-reflection, normal);
        float cos_mn = glm::dot(half, normal);
        float g = GGX_G(normal, half, -reflection, new_direction, material.roughness);
        float weight = glm::abs( cos_im * g / ( cos_in * cos_mn ) );
        // return glm::vec4( new_direction, weight );
        ret_weight = weight;
        ret_direction = new_direction;
        goto push;
    }
    push:
    // push to next node
    static_cast<Vec3Slot*>(out_slots_[ slotID(OutSlotName::Direction) ])->value_ = ret_direction;
    static_cast<FloatSlot*>(out_slots_[ slotID(OutSlotName::Weight) ])->value_ = ret_weight;
    if(out_slots_[ slotID(OutSlotName::Direction) ]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[ slotID(OutSlotName::Direction) ]->nextNode_->in_slots_[out_slots_[ slotID(OutSlotName::Direction) ]->nextSlotId_];
        static_cast<Vec3Slot*>(next_slot)->value_ = ret_direction;
    }
    if(out_slots_[ slotID(OutSlotName::Weight) ]->nextNode_ != nullptr) {
        auto next_slot = out_slots_[ slotID(OutSlotName::Weight) ]->nextNode_->in_slots_[out_slots_[ slotID(OutSlotName::Weight) ]->nextSlotId_];
        static_cast<FloatSlot*>(next_slot)->value_ = ret_weight;
    }
}

sparks::GlassSampler::GlassSampler(SceneInfo * scene_info)
{
}

sparks::GlassSampler::~GlassSampler()
{
}

void sparks::GlassSampler::process()
{
}

sparks::DiffuseSample::DiffuseSample(SceneInfo * scene_info)
{
}

sparks::DiffuseSample::~DiffuseSample()
{
}

void sparks::DiffuseSample::process()
{
}

sparks::MixSampler::MixSampler(SceneInfo * scene_info)
{
}

sparks::MixSampler::~MixSampler()
{
}

void sparks::MixSampler::process()
{
}
