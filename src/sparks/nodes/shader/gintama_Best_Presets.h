#pragma once
#include "vector"
#include "glm/glm.hpp"
#include "sparks/nodes/shader/gintama_Texture.h"
#include "sparks/nodes/shader/gintama_Vector.h"
#include "sparks/nodes/shader/gintama_BSDF.h"
#include "sparks/nodes/shader/gintama_Inputs.h"
#include "sparks/nodes/shader/gintama_Converter.h"
#include "sparks/assets/scene.h"

namespace sparks {

class Presets {
    public:
    Presets() = default;
    ~Presets() = default;
    int test = 0;
    bool checkgood();
    // glm::vec3 checkerBump(const Scene* scene, int entity_id, int texture_id, 
    //     float u, float v, glm::vec3 incident, glm::vec3 reflected, glm::vec3 normal, glm::vec3 tangent);
};


}