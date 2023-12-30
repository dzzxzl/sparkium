#pragma once
#include "cstdint"
#include "glm/glm.hpp"
#include "sparks/assets/util.h"

namespace sparks {

enum MaterialType : int {
  MATERIAL_TYPE_LAMBERTIAN = 0,
  MATERIAL_TYPE_SPECULAR = 1,
  MATERIAL_TYPE_TRANSMISSIVE = 2,
  MATERIAL_TYPE_PRINCIPLED = 3,
  MATERIAL_TYPE_EMISSION = 4,
  MATERIAL_TYPE_GLASS = 5,
  MATERIAL_TYPE_CHECKERBUMP = 6,
  MATERIAL_TYPE_CHECKER_A = 7,
  MATERIAL_TYPE_ROUGHGLASS = 8,
  MATERIAL_TYPE_VOLUME_A = 9,
  MATERIAL_TYPE_GLOSSY = 10,
};

class Scene;

struct Material {
  glm::vec3 albedo_color{0.8f};
  int albedo_texture_id{0};
  glm::vec3 emission{0.0f};
  float emission_strength{1.0f};
  float alpha{1.0f};
  MaterialType material_type{MATERIAL_TYPE_LAMBERTIAN};
  float IOR{1.5f};
  bool inverse_normal{false};
  bool shade_smooth{false};
  bool has_volume{false};
  bool reserve[1]{};
  float roughness{0.1f};
  float pad[3]{};
  glm::vec3 extinction{1.0f};
  float max_extinction{0.1f};
  glm::vec3 scatter_albedo{1.0f};
  float volume_scale_{1.0f};
  glm::vec3 volume_emission_color{0.0f};
  float pad3[1]{};
  Material() = default;
  explicit Material(const glm::vec3 &albedo);
  Material(Scene *scene, const tinyxml2::XMLElement *material_element);
};


}  // namespace sparks
