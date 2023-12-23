#include "sparks/renderer/path_tracer.h"

#include "sparks/util/util.h"
#include "path_tracer.h"

#define GINTAMA_P_RR 0.7f

namespace sparks {
PathTracer::PathTracer(const RendererSettings *render_settings,
                       const Scene *scene) {
  render_settings_ = render_settings;
  scene_ = scene;
}

glm::vec3 PathTracer::SampleRay(glm::vec3 origin,
                                glm::vec3 direction,
                                int x,
                                int y,
                                int sample) const {
  glm::vec3 throughput{1.0f};
  glm::vec3 radiance{0.0f};
  HitRecord hit_record;
  const int max_bounce = render_settings_->num_bounces;
  std::mt19937 rd(sample ^ x ^ y);
  for (int i = 0; i < max_bounce; i++) {
    auto t = scene_->TraceRay(origin, direction, 1e-3f, 1e4f, &hit_record);
    if (t > 0.0f) {
      // hit an object
      // get material
      auto &material =
          scene_->GetEntity(hit_record.hit_entity_id).GetMaterial();
      // Emission material
      // some tests
      // if(hit_record.geometry_normal.x < 0.0f) {
      //   return glm::vec3(1.0f, 0.0f, 0.0f);
      // } else {
      //   return glm::vec3(0.0f);
      // }

      if (material.material_type == MATERIAL_TYPE_EMISSION) {
        radiance += throughput * material.emission * material.emission_strength;
        break;
      }
      // Glass or specular material
      else if (material.material_type == MATERIAL_TYPE_GLASS || material.material_type == MATERIAL_TYPE_SPECULAR) {
        origin = hit_record.position;
        direction = importanceSample(hit_record, -direction, material.material_type);
        if(glm::length(direction) < 1e-3f) {
          break;
        }
      }
      // Non-transmissive material
      else if (
        material.material_type == MATERIAL_TYPE_LAMBERTIAN || material.material_type == MATERIAL_TYPE_CHECKERBUMP) {
        origin = hit_record.position;
        auto shader_preset = getShaderPreset(material.material_type);
        sampleLight(scene_, hit_record, -direction, shader_preset, radiance, throughput);
        break;
        auto old_direction = direction;
        direction = importanceSample(hit_record, -old_direction, MATERIAL_TYPE_LAMBERTIAN);
        throughput *= surfaceBSDF(scene_, hit_record, {-direction, -old_direction}, shader_preset) * std::max(glm::dot(-old_direction, hit_record.geometry_normal), 0.0f) * importanceSampleFactor(hit_record, -old_direction, material.material_type) / GINTAMA_P_RR;
        if(genRandFloat(0,1) > GINTAMA_P_RR){
          break;
        }
      }
      else {
        // Default material
        // Shade from environment
        throughput *= material.albedo_color * glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(hit_record.tex_coord)};
        origin = hit_record.position;
        direction = scene_->GetEnvmapLightDirection();
        radiance += throughput * scene_->GetEnvmapMinorColor();
        throughput *= std::max(glm::dot(direction, hit_record.normal), 0.0f) * 2.0f;
        if (scene_->TraceRay(origin, direction, 1e-3f, 1e4f, nullptr) < 0.0f) {
          radiance += throughput * scene_->GetEnvmapMajorColor();
        }
        break;
      }
    } else {
      // hit environment
      radiance += throughput * glm::vec3{scene_->SampleEnvmap(direction)};
      break;
    }
  }
  return radiance;
}

glm::vec3 PathTracer::importanceSample(HitRecord hit_record, glm::vec3 reflection, MaterialType material_type) const {

  if(material_type == MATERIAL_TYPE_LAMBERTIAN) {
    float theta = genRandFloat(0, PI);
    float phi = genRandFloat(0, 2 * PI);
    glm::vec3 sample_direction = glm::vec3(
      glm::sin(theta) * glm::cos(phi),
      glm::sin(theta) * glm::sin(phi),
      glm::cos(theta)
    );
    glm::vec3 geo_normal = hit_record.geometry_normal;
    if(glm::dot(sample_direction, geo_normal) < 0.0f) {
      sample_direction = -sample_direction;
    }
    return sample_direction;
  }

  else if( material_type == MATERIAL_TYPE_GLASS ) {
    auto material = scene_->GetEntity(hit_record.hit_entity_id).GetMaterial();
    auto IOR = material.IOR;
    bool in_glass = ! ( hit_record.front_face ^ material.inverse_normal );
    // bool in_glass = glm::dot(-reflection, hit_record.geometry_normal) > 0.0f;
    if(in_glass) {
      float cos_theta = glm::dot(reflection, hit_record.geometry_normal);
      float eta = IOR;
      float R_0 = (1 - eta) / (1 + eta);
      R_0 = R_0 * R_0;
      float R = R_0 + (1 - R_0) * glm::pow(1 - cos_theta, 5);
      if( 1 - eta * eta * (1 - cos_theta * cos_theta) < 0.0f ) {
        // return glm::vec3(0.0f);
        return glm::reflect(-reflection, hit_record.geometry_normal);
      }
      if(genRandFloat(0,1) < R) {
        return glm::reflect(-reflection, hit_record.geometry_normal);
      }
      return glm::refract(-reflection, hit_record.geometry_normal, IOR);
    } else {
      float cos_theta = glm::dot(reflection, hit_record.geometry_normal);
      float eta = 1.0f / IOR;
      float R_0 = (1 - eta) / (1 + eta);
      R_0 = R_0 * R_0;
      float R = R_0 + (1 - R_0) * glm::pow(1 - cos_theta, 5);
      if( 1 - eta * eta * (1 - cos_theta * cos_theta) < 0.0f ) {
        // return glm::vec3(0.0f);
        return glm::reflect(-reflection, hit_record.geometry_normal);
      }
      if(genRandFloat(0,1) < R) {
        return glm::reflect(-reflection, hit_record.geometry_normal);
      }
      return glm::refract(-reflection, hit_record.geometry_normal, 1.0f / IOR);
    }
  }

  else if( material_type == MATERIAL_TYPE_SPECULAR ) {
    return glm::reflect(-reflection, hit_record.geometry_normal);
  }

}

glm::vec3 PathTracer::surfaceBSDF(const Scene* scene, HitRecord hit_record, LightRecord light_record, ShaderPreset shader_preset) const {

  if(shader_preset == ShaderPreset::Lambertian){
    auto material = scene_->GetEntity(hit_record.hit_entity_id).GetMaterial();
    auto albedo = material.albedo_color;
    auto texture = glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(hit_record.tex_coord)};
    return albedo * texture / PI;
  }

  else if(shader_preset == ShaderPreset::CheckerBump) {
    SceneInfo* scene_info = new SceneInfo{scene, hit_record, light_record};
    return Presets::checkerBump(scene_info);
  }

}

void PathTracer::sampleLight(const Scene* scene, HitRecord hit_record, glm::vec3 reflection, ShaderPreset shader_preset, glm::vec3 &radiance, glm::vec3 throughput) const {

  for(int eme_iter=0; eme_iter < scene->GetEntityCount(); eme_iter++){
    auto &eme_material = scene->GetEntity(eme_iter).GetMaterial();
    if(eme_material.material_type == MATERIAL_TYPE_EMISSION) {
      HitRecord eme_hit_record;
      auto eme_origin = scene->GetEntity(eme_iter).getSamplePoint();
      auto pdf_area = scene->GetEntity(eme_iter).getSurfaceArea();
      auto eme_direction = glm::normalize(eme_origin - hit_record.position);
      auto distance = glm::distance(eme_origin, hit_record.position);
      auto eme_t = scene->TraceRay(hit_record.position, eme_direction, 1e-3f, 1e4f, &eme_hit_record);
      if(eme_t > 0.0f) {
        // blocked 
        if(eme_hit_record.hit_entity_id != eme_iter) {
          continue;
        } else if(eme_hit_record.position != eme_origin) {
          // continue; ?
        }
        radiance += throughput * eme_material.emission * eme_material.emission_strength 
        * pdf_area / (distance * distance)
        * std::max(glm::dot(-eme_direction, eme_hit_record.geometry_normal), 0.0f)
        * std::max(glm::dot(eme_direction, hit_record.geometry_normal), 0.0f)
        * surfaceBSDF(scene, hit_record, {-eme_direction, reflection}, shader_preset)
        ;
      }
    }
  }

}

void PathTracer::sampleEnv(const Scene* scene, HitRecord hit_record, glm::vec3 reflection, ShaderPreset shader_preset, glm::vec3 &radiance, glm::vec3 throughput) const {

  auto direction_env = scene_->GetEnvmapLightDirection();
  radiance += throughput * scene_->GetEnvmapMinorColor();
  if (scene_->TraceRay(hit_record.position, direction_env, 1e-3f, 1e4f, nullptr) < 0.0f) {
    radiance += throughput * scene_->GetEnvmapMajorColor() * 
      surfaceBSDF(scene, hit_record, {-direction_env, reflection}, shader_preset);
  }

}

float PathTracer::importanceSampleFactor(HitRecord hit_record, glm::vec3 reflection, MaterialType material_type) const {

  if(material_type == MATERIAL_TYPE_LAMBERTIAN || material_type == MATERIAL_TYPE_CHECKERBUMP) {
    return 2.0f * PI;
  }

}

PathTracer::ShaderPreset PathTracer::getShaderPreset(MaterialType material_type) {
  if(material_type == MATERIAL_TYPE_LAMBERTIAN) {
    return ShaderPreset::Lambertian;
  }
  else if(material_type == MATERIAL_TYPE_GLASS) {
    return ShaderPreset::Glass;
  }
  else if(material_type == MATERIAL_TYPE_SPECULAR) {
    return ShaderPreset::Specular;
  }
  else if(material_type == MATERIAL_TYPE_EMISSION) {
    return ShaderPreset::Emission;
  }
  else if(material_type == MATERIAL_TYPE_CHECKERBUMP) {
    return ShaderPreset::CheckerBump;
  }
}


}  // namespace sparks

