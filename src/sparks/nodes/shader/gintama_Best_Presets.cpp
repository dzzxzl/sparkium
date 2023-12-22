#include "sparks/nodes/shader/gintama_Best_Presets.h"

namespace sparks {

glm::vec3 Presets::checkerBump(SceneInfo* scene_info) {
    // auto hit_record = scene_info->hit_record_;
    // auto scene = scene_info->scene_;
    // int entity_id = hit_record.hit_entity_id;
    // int texture_id = scene->GetEntity(entity_id).GetMaterial().albedo_texture_id;
    // glm::vec3 normal = hit_record.normal;
    // glm::vec3 tangent = hit_record.tangent;
    // float u = hit_record.tex_coord.x;
    // float v = hit_record.tex_coord.y;
    // glm::vec3 incident = light_record.incident;
    // glm::vec3 reflected = light_record.reflected;
    TextureSample texSamp(scene_info);
    // TextureSample texSamp(scene, entity_id, texture_id, u, v);
    Checker checker(scene_info);
    // setFloat(checker.getInput(2), 2.0f);
    // float texture_width = scene->GetTexture(texture_id).GetWidth();
    // float texture_height = scene->GetTexture(texture_id).GetHeight();
    // checker.u_ = u / texture_width;
    // checker.v_ = v / texture_height;
    Bump bump(scene_info);
    // bump.normal_ = normal;
    // bump.tangent_ = tangent;
    // bump.reflection_ = reflected;
    setEnum(bump.in_slots_[1], 0);
    DiffuseBSDF diffuse(scene_info);
    // diffuse.incident_ = incident;
    // diffuse.reflected_ = reflected;
    auto entity_id = scene_info->hit_record_.hit_entity_id;
    Color color(scene_info->scene_->GetEntity(entity_id).GetMaterial().albedo_color);
    Multiplier multiplier;
    checker.link(&bump, 0, 2);
    checker.link(&bump, 1, 3);
    bump.link(&diffuse, 0, 1);
    texSamp.link(&multiplier, 0, 0);
    color.link(&multiplier, 0, 1);
    multiplier.link(&diffuse, 0, 0);
    // setVec3(diffuse.getInput(1), normal);
    diffuse.incident_ = incident;
    diffuse.reflected_ = reflected;
    diffuse.process();
    glm::vec3 res = static_cast<Vec3Slot*>( diffuse.getOutput(0) )->value_;
    return res;
}

// bool Presets::checkgood() {
//     return true;
// }

}