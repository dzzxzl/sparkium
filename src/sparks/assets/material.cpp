#include "sparks/assets/material.h"

#include "grassland/grassland.h"
#include "sparks/assets/scene.h"
#include "sparks/assets/texture.h"
#include "sparks/util/util.h"

namespace sparks {

namespace {
std::unordered_map<std::string, MaterialType> material_name_map{
    {"lambertian", MATERIAL_TYPE_LAMBERTIAN},
    {"specular", MATERIAL_TYPE_SPECULAR},
    {"transmissive", MATERIAL_TYPE_TRANSMISSIVE},
    {"principled", MATERIAL_TYPE_PRINCIPLED},
    {"emission", MATERIAL_TYPE_EMISSION},
    {"glass", MATERIAL_TYPE_GLASS},
    {"checkerBump", MATERIAL_TYPE_CHECKERBUMP},
    {"checker_A", MATERIAL_TYPE_CHECKER_A},
    {"roughGlass", MATERIAL_TYPE_ROUGHGLASS},
    {"volumeA", MATERIAL_TYPE_VOLUME_A},
    {"glossy", MATERIAL_TYPE_GLOSSY},
    {"noise_A", MATERIAL_TYPE_NOISE_A},
    {"rust", MATERIAL_TYPE_RUST},
    {"roughGlassNode", MATERIAL_TYPE_ROUGHGLASS_NODE},
    {"water", MATERIAL_TYPE_WATER},
    {"glassDispersion", MATERIAL_TYPE_GLASS_DISPERSION},};
}

Material::Material(Scene *scene, const tinyxml2::XMLElement *material_element)
    : Material() {
  if (!material_element) {
    return;
  }

  albedo_color = glm::vec3{1.0f};

  auto child_element = material_element->FirstChildElement("albedo");
  if (child_element) {
    albedo_color = StringToVec3(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("albedo_texture");
  if (child_element) {
    std::string path = child_element->FindAttribute("value")->Value();
    Texture albedo_texture(1, 1);
    if (Texture::Load(path, albedo_texture)) {
      albedo_texture_id =
          scene->AddTexture(albedo_texture, PathToFilename(path));
    }
  }

  child_element = material_element->FirstChildElement("emission");
  if (child_element) {
    emission = StringToVec3(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("emission_strength");
  if (child_element) {
    emission_strength =
        std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("alpha");
  if (child_element) {
    alpha = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("IOR");
  if (child_element) {
    IOR = std::stof(child_element->FindAttribute("value")->Value());
  }
  
  child_element = material_element->FirstChildElement("IOR_R");
  if (child_element) {
    IOR_R_ = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("IOR_G");
  if (child_element) {
    IOR_G_ = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("IOR_B");
  if (child_element) {
    IOR_B_ = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("inverter_invert");
  if (child_element) {
    inverter_invert =
        std::stoi(child_element->FindAttribute("value")->Value()) != 0;
  }

  child_element = material_element->FirstChildElement("rough_glass_ramp_pos");
  if (child_element) {
    rough_glass_ramp_pos_ =
        std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("max_extinction");
  if (child_element) {
    max_extinction =
        std::stof(child_element->FindAttribute("value")->Value());
  }


  child_element = material_element->FirstChildElement("inverse_normal");
  if (child_element) {
    inverse_normal =
        std::stoi(child_element->FindAttribute("value")->Value()) != 0;
  }

  child_element = material_element->FirstChildElement("shade_smooth");
  if (child_element) {
    shade_smooth =
        std::stoi(child_element->FindAttribute("value")->Value()) != 0;
  }
  
  child_element = material_element->FirstChildElement("roughness");
  if (child_element) {
    roughness = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("has_volume");
  if (child_element) {
    has_volume = std::stoi(child_element->FindAttribute("value")->Value()) != 0;
  }

  child_element = material_element->FirstChildElement("scatter_albedo");
  if (child_element) {
    scatter_albedo =
        StringToVec3(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("volume_scale");
  if (child_element) {
    volume_scale_ = std::stof(child_element->FindAttribute("value")->Value());
  }

  child_element = material_element->FirstChildElement("volume_emission_color");
  if (child_element) {
    volume_emission_color =
        StringToVec3(child_element->FindAttribute("value")->Value());
  }

  material_type =
      material_name_map[material_element->FindAttribute("type")->Value()];
}

Material::Material(const glm::vec3 &albedo) : Material() {
  albedo_color = albedo;
}
}  // namespace sparks
