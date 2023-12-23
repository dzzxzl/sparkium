#include "sparks/nodes/shader/gintama_Best_Presets.h"

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

}