#include "sparks/nodes/shader/gintama_Best_Presets.h"

namespace sparks {

glm::vec3 Presets::checkerBump(const Scene* scene, int entity_id, int texture_id, 
    float u, float v, glm::vec3 incident, glm::vec3 reflected, glm::vec3 normal, glm::vec3 tangent) { // geo normal
    TextureSample texSamp(scene, entity_id, texture_id, u, v);
    Checker checker;
    // setFloat(checker.getInput(2), 2.0f);
    float texture_width = scene->GetTexture(texture_id).GetWidth();
    float texture_height = scene->GetTexture(texture_id).GetHeight();
    checker.u_ = u / texture_width;
    checker.v_ = v / texture_height;
    Bump bump;
    bump.normal_ = normal;
    bump.tangent_ = tangent;
    bump.reflection_ = reflected;
    setEnum(bump.in_slots_[1], 0);
    DiffuseBSDF diffuse;
    diffuse.incident_ = incident;
    diffuse.reflected_ = reflected;
    Color color(scene->GetEntity(entity_id).GetMaterial().albedo_color);
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