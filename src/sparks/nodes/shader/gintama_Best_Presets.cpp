#include "sparks/nodes/shader/gintama_Best_Presets.h"

namespace sparks {

// glm::vec3 Presets::checkerBump(const Scene* scene, int entity_id, int texture_id, 
//     float u, float v, glm::vec3 incident, glm::vec3 reflected, glm::vec3 normal, glm::vec3 tangent) { // geo normal
//     glm::vec3 res(0.0f);
//     return res;
//     TextureSample texSamp(scene, entity_id, texture_id, u, v);
//     Checker checker;
//     checker.u_ = u;
//     checker.v_ = v;
//     Bump bump;
//     bump.normal_ = normal;
//     bump.tangent_ = tangent;
//     DiffuseBSDF diffuse;
//     diffuse.incident_ = incident;
//     diffuse.reflected_ = reflected;
//     Color color(scene->GetEntity(entity_id).GetMaterial().albedo_color);
//     Multiplier multiplier;
//     checker.link(&bump, 0, 2);
//     checker.link(&bump, 1, 3);
//     bump.link(&diffuse, 0, 1);
//     texSamp.link(&multiplier, 0, 0);
//     color.link(&multiplier, 0, 1);
//     multiplier.link(&diffuse, 0, 0);
//     diffuse.process();
//     res = static_cast<Vec3Slot*>( diffuse.getOutput(0) )->value_;
//     return res;
// }

bool Presets::checkgood() {
    return true;
}

}