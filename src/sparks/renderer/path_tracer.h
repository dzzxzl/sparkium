#pragma once
#include "random"
#include "sparks/assets/scene.h"
#include "sparks/renderer/renderer_settings.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "sparks/assets/material.h"
#include "sparks/assets/util.h"
#include "sparks/nodes/shader/gintama_Layout.h"
#include "sparks/nodes/shader/gintama_Best_Presets.h"
#include "sparks/nodes/shader/gintama_System.h"

namespace sparks {
class PathTracer {
 public:
  enum ShaderPreset {
    Lambertian=0,
    Specular,
    Glass,
    Emission,
    CheckerBump,
    Checker_A,
    RoughGlass,
    VolumeA,
    Glossy,
    NoiseA,
    Rust,
    RoughGlassNode,
    Water
  };
  PathTracer(const RendererSettings *render_settings, const Scene *scene);
  [[nodiscard]] glm::vec3 SampleRay(glm::vec3 origin,
                                    glm::vec3 direction,
                                    int x,
                                    int y,
                                    int sample) const;
  [[nodiscard]] glm::vec4 importanceSample(HitRecord hit_record, glm::vec3 reflection, MaterialType material_type) const;
  [[nodiscard]] glm::vec4 importanceSample_use_node(HitRecord hit_record, glm::vec3 reflection, MaterialType material_type) const;
  [[nodiscard]] glm::vec3 surfaceBSDF(const Scene* scene, HitRecord hit_record, LightRecord light_record, ShaderPreset shader_preset) const;
  void sampleLight(const Scene* scene, HitRecord hit_record, glm::vec3 reflection, ShaderPreset shader_preset, glm::vec3 &radiance, glm::vec3 throughput) const;
  void sampleEnv(const Scene* scene, HitRecord hit_record, glm::vec3 reflection, ShaderPreset shader_preset, glm::vec3 &radiance, glm::vec3 throughput) const;
  static ShaderPreset getShaderPreset(MaterialType material_type);
  void volumeSampleLight(const Scene* scene, HitRecord hit_record, glm::vec3 &radiance, glm::vec3 throughput) const;
 private:
  const RendererSettings *render_settings_{};
  const Scene *scene_{};
};
}  // namespace sparks
