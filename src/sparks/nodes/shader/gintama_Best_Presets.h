#pragma once
#include "vector"
#include "glm/glm.hpp"
#include "sparks/nodes/shader/gintama_Texture.h"
#include "sparks/nodes/shader/gintama_Vector.h"
#include "sparks/nodes/shader/gintama_BSDF.h"
#include "sparks/nodes/shader/gintama_Inputs.h"
#include "sparks/nodes/shader/gintama_Converter.h"
#include "sparks/nodes/shader/gintama_Sampler.h"
#include "sparks/assets/scene.h"

namespace sparks {

class Presets {
    public:
    Presets() = default;
    ~Presets() = default;
    static glm::vec3 checkerBump(SceneInfo* scene_info);
    static glm::vec3 checkerA(SceneInfo* scene_info);
    static glm::vec3 noiseA(SceneInfo* scene_info);
    static glm::vec3 glossy(SceneInfo* scene_info);
    static glm::vec3 rust(SceneInfo* scene_info);
    static glm::vec4 sampleRoughGlass(SceneInfo* scene_info);
    static glm::vec4 sampleWater(SceneInfo* scene_info);
};


}