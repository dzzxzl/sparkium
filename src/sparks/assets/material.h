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
  MATERIAL_TYPE_NOISE_A = 11,
  MATERIAL_TYPE_RUST = 12,
  MATERIAL_TYPE_ROUGHGLASS_NODE = 13,
  MATERIAL_TYPE_WATER = 14,
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
  // some more bools
  bool inverter_invert{true};
  bool pad3[3]{};
  // here begin interpolation
  float inter_pos_1_{0.432f};
  float inter_pos_2_{0.532f};
  float pad4[2]{};
  // here begin texture moves
  float text_scale_x_{1.0f};
  float text_scale_y_{1.0f};
  float text_offset_x_{0.0f};
  float text_offset_y_{0.0f};

  // here begins glossy components
  glm::vec3 glossy_color_{1.0f};
  float roughness_x_{0.59f};

  float roughness_anisotropic_{0.0f};
  float pad5[3]{};

  // here begins color ramp components
  glm::vec3 ramp_color_1_{0.57f, 0.215f, 0.077f};
  float ramp_pos_1_{0.368f};
  glm::vec3 ramp_color_2_{0.139f};
  float ramp_pos_2_{0.505f};
  glm::vec3 ramp_color_3_{0.482f};
  float ramp_pos_3_{0.605f};

  // here is for rough glass globe
  glm::vec3 rough_glass_ramp_color_{1.0f};
  float rough_glass_ramp_pos_{0.059f};

  // here is water
  float water_ramp_pos_1_{0.1f};
  float water_ramp_pos_2_{0.9f};
  float water_bump_strength_{0.9f};
  float pad6[1]{};

  Material() = default;
  explicit Material(const glm::vec3 &albedo);
  Material(Scene *scene, const tinyxml2::XMLElement *material_element);
};


}  // namespace sparks
