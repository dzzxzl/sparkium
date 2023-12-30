#include "sparks/nodes/shader/gintama_Best_Presets.h"
#include "gintama_Best_Presets.h"

namespace sparks {

glm::vec3 Presets::checkerBump(SceneInfo* scene_info) {
    TextureSample texSamp(scene_info);
    Checker checker(scene_info);
    Bump bump(scene_info);
    Multiplier multiplier;
    auto entity_id = scene_info->hit_record_.hit_entity_id;
    Color color(scene_info->scene_->GetEntity(entity_id).GetMaterial().albedo_color);
    DiffuseBSDF diffuse(scene_info);
    setEnum(bump.in_slots_[1], 0);
    checker.link(&bump, 0, 2);
    checker.link(&bump, 1, 3);
    bump.link(&diffuse, 0, 1);
    texSamp.link(&multiplier, 0, 0);
    color.link(&multiplier, 0, 1);
    multiplier.link(&diffuse, 0, 0);
    color.link(&diffuse, 0, 0);
    diffuse.process();
    glm::vec3 res = getVec3(diffuse.out_slots_[0]);
    return res;
}

glm::vec3 Presets::checkerA(SceneInfo* scene_info) {
    Stripes stripes(scene_info);
    setVec3(stripes.in_slots_[0], glm::vec3(0.8f, 0.0f, 0.0f));
    setVec3(stripes.in_slots_[1], glm::vec3(0.0f, 0.8f, 0.0f));
    stripes.inverse_direction_ = true;
    stripes.process();
    glm::vec3 res = getVec3(stripes.out_slots_[0]);
    return res;
}

glm::vec3 Presets::noiseA(SceneInfo * scene_info)
{
    // NoiseTexture noise(scene_info);
    TextureSampleAbsolute texSamp(scene_info);
    DiffuseBSDF diffuse(scene_info);
    Bump bump(scene_info);
    ColorRamp colorRamp;
    auto material = scene_info->scene_->GetEntity(scene_info->hit_record_.hit_entity_id).GetMaterial();
    auto pos_1 = material.inter_pos_1_;
    auto pos_2 = material.inter_pos_2_;
    colorRamp.setColor(glm::vec3(0.0f), pos_1);
    colorRamp.setColor(glm::vec3(1.0f), pos_2);
    texSamp.offset_x = material.text_offset_x_;
    texSamp.offset_y = material.text_offset_y_;
    texSamp.scale_x = material.text_scale_x_;
    texSamp.scale_y = material.text_scale_y_;
    auto entity_id = scene_info->hit_record_.hit_entity_id;
    Color color(scene_info->scene_->GetEntity(entity_id).GetMaterial().albedo_color);
    setEnum(bump.in_slots_[1], 2);
    color.link(&diffuse, 0, 0);
    bump.link(&diffuse, 0, 1);
    // noise.link(&bump, 0, 2);
    texSamp.link(&colorRamp, 0, 0);
    // texSamp.link(&diffuse, 0, 0);
    colorRamp.link(&bump, 0, 2);    
    // texSamp.link(&bump, 1, 3);
    // noise.link(&bump, 1, 3);
    diffuse.process();
    glm::vec3 res = getVec3(diffuse.out_slots_[0]);
    return res;
}

glm::vec3 Presets::glossy(SceneInfo * scene_info)
{
    GlossyBSDF glossy(scene_info);
    // LAND_INFO("here");
    auto material = scene_info->scene_->GetEntity(scene_info->hit_record_.hit_entity_id).GetMaterial();
    setVec3(glossy.in_slots_[0], material.glossy_color_);
    setVec3(glossy.in_slots_[1], scene_info->hit_record_.geometry_normal);
    setFloat(glossy.in_slots_[2], material.roughness_x_);
    setFloat(glossy.in_slots_[3], material.roughness_anisotropic_);
    glossy.process();
    glm::vec3 res = getVec3(glossy.out_slots_[0]);
    return res;
}

glm::vec3 Presets::rust(SceneInfo * scene_info)
{
    TextureSampleAbsolute texSamp(scene_info);
    GlossyBSDF glossy(scene_info);
    Bump bump(scene_info);
    ColorRamp colorRamp;
    auto material = scene_info->scene_->GetEntity(scene_info->hit_record_.hit_entity_id).GetMaterial();
    auto pos_1 = material.inter_pos_1_;
    auto pos_2 = material.inter_pos_2_;
    colorRamp.setColor(glm::vec3{0.0f}, pos_1);
    colorRamp.setColor(glm::vec3{1.0f}, pos_2);
    texSamp.offset_x = material.text_offset_x_;
    texSamp.offset_y = material.text_offset_y_;
    texSamp.scale_x = material.text_scale_x_;
    texSamp.scale_y = material.text_scale_y_;
    auto entity_id = scene_info->hit_record_.hit_entity_id;
    // Color color(scene_info->scene_->GetEntity(entity_id).GetMaterial().albedo_color);
    setEnum(bump.in_slots_[1], 2);
    // color.link(&glossy, 0, 0);
    bump.link(&glossy, 0, 1);
    texSamp.link(&colorRamp, 0, 0); 
    colorRamp.link(&bump, 0, 2);
    // setVec3(glossy.in_slots_[0], material.glossy_color_);
    setFloat(glossy.in_slots_[2], material.roughness_x_);
    setFloat(glossy.in_slots_[3], material.roughness_anisotropic_);

    // color ramp 2
    ColorRamp colorRamp2;
    auto pos_3 = material.ramp_pos_1_;
    auto pos_4 = material.ramp_pos_2_;
    auto pos_5 = material.ramp_pos_3_;
    colorRamp2.start_color_ = material.ramp_color_1_;
    colorRamp2.end_color_ = material.ramp_color_3_;
    colorRamp2.setColor(material.ramp_color_1_, pos_3);
    colorRamp2.setColor(material.ramp_color_2_, pos_4);
    colorRamp2.setColor(material.ramp_color_3_, pos_5);

    Inverter inverter;
    inverter.invert_ = material.inverter_invert;

    texSamp.link(&inverter, 0, 0);
    inverter.link(&colorRamp2, 0, 0);
    colorRamp2.link(&glossy, 0, 0);

    glossy.process();
    glm::vec3 res = getVec3(glossy.out_slots_[0]);
    return res;
}

}