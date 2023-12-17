#include "sparks/renderer/path_tracer.h"

#include "sparks/util/util.h"
#include "path_tracer.h"

#define GINTAMA_P_RR 0.5f

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
      auto &material =
          scene_->GetEntity(hit_record.hit_entity_id).GetMaterial();
      if (material.material_type == MATERIAL_TYPE_EMISSION) {
        radiance += throughput * material.emission * material.emission_strength;
        break;
      } 
      else if (material.material_type == MATERIAL_TYPE_GLASS) {
        // GLASS
        bool in_glass = glm::dot(hit_record.geometry_normal, hit_record.normal) > 0.0f;
        if(in_glass) {
          direction = glm::refract(direction, hit_record.normal, material.IOR);
          throughput *= material.albedo_color;
          origin = hit_record.position;
          // throughput *= std::max(glm::dot(direction, hit_record.normal), 0.0f) * 2.0f;
          in_glass = false;
        } else {
          direction = glm::refract(direction, -hit_record.normal, 1.0f / material.IOR);
          throughput *= material.albedo_color;
          origin = hit_record.position;
          // throughput *= std::max(glm::dot(direction, -hit_record.normal), 0.0f) * 2.0f;
          in_glass = true;
        }
      } else if (material.material_type == MATERIAL_TYPE_SPECULAR) {
        origin = hit_record.position;
        direction = glm::reflect(direction, -hit_record.normal);
      }
      else if (material.material_type == MATERIAL_TYPE_LAMBERTIAN) {
        glm::vec3 material_mutiplier = material.albedo_color *
            glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(
                hit_record.tex_coord)};
        origin = hit_record.position;
        // environment light
        // auto direction_env = scene_->GetEnvmapLightDirection();
        // radiance += throughput * material_mutiplier * scene_->GetEnvmapMinorColor();
        // if (scene_->TraceRay(origin, direction_env, 1e-3f, 1e4f, nullptr) < 0.0f) {
        //   radiance += throughput * material_mutiplier * 
        //     BSDF(direction, hit_record.normal, direction_env, MATERIAL_TYPE_LAMBERTIAN) 
        //     * scene_->GetEnvmapMajorColor();
        // }
        // direct light
        for(int eme_iter=0; eme_iter < scene_->GetEntityCount(); eme_iter++){
          auto &eme_material = scene_->GetEntity(eme_iter).GetMaterial();
          if(eme_material.material_type == MATERIAL_TYPE_EMISSION){
            HitRecord eme_hit_record;
            auto eme_origin = scene_->GetEntity(eme_iter).getCenter();
            auto crossSection = scene_->GetEntity(eme_iter).getCrossSection();
            auto eme_direction = glm::normalize(eme_origin - origin);
            auto distance = glm::distance(eme_origin, origin);
            auto eme_t = scene_->TraceRay(origin, eme_direction, 1e-3f, 1e4f, &eme_hit_record);
            if(eme_t > 0.0f){
              // blocked - yes
              if( eme_hit_record.hit_entity_id != eme_iter){
                continue;
              }
              // not blocked 
              radiance += throughput * eme_material.emission * eme_material.emission_strength / distance
                * crossSection / (distance * distance)
                * BSDF(-eme_direction, -eme_hit_record.normal, {}, MATERIAL_TYPE_EMISSION)
                * material_mutiplier;
            }
          }
        }
        break;
        auto old_direction = direction;
        direction = SampleHemisphere(-hit_record.normal);
        throughput *= material_mutiplier * BSDF(-old_direction, -hit_record.normal, -direction, 
         MATERIAL_TYPE_LAMBERTIAN) * 2.0f / GINTAMA_P_RR;
         if(genRanFloat() > GINTAMA_P_RR){
           break;
         }
      }
      else {
        throughput *=
            material.albedo_color *
            glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(
                hit_record.tex_coord)};
        origin = hit_record.position;
        direction = scene_->GetEnvmapLightDirection();
        radiance += throughput * scene_->GetEnvmapMinorColor();
        throughput *=
            std::max(glm::dot(direction, hit_record.normal), 0.0f) * 2.0f;
        if (scene_->TraceRay(origin, direction, 1e-3f, 1e4f, nullptr) < 0.0f) {
          radiance += throughput * scene_->GetEnvmapMajorColor();
        }
        break;
      }
    } else {
      radiance += throughput * glm::vec3{scene_->SampleEnvmap(direction)};
      break;
    }
  }
  return radiance;
}

glm::vec3 PathTracer::SampleHemisphere(glm::vec3 axis) const
{
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(0.0f, 1.0f);
  glm::vec3 sampleDirection(0.0f);
  while (glm::dot(sampleDirection, axis) == 0.0f) {
    float radius = 1.0f;
    float pi = glm::radians(180.0f);
    float theta = distr(eng) * 2.0f * pi;
    float phi = distr(eng) * 1.0f * pi;

    // Convert spherical coordinates to Cartesian coordinates
    float x = radius * glm::sin(phi) * glm::cos(theta);
    float y = radius * glm::sin(phi) * glm::sin(theta);
    float z = radius * glm::cos(phi);

    sampleDirection = glm::vec3(x, y, z);
    if (glm::dot(sampleDirection, axis) < 0.0f) {
      sampleDirection = -sampleDirection;
    }
  }
  return sampleDirection;
}

float PathTracer::genRanFloat() const
{
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<float> distr(0.0f, 1.0f);
  return distr(eng);
}



float PathTracer::BSDF(glm::vec3 reflection, glm::vec3 normal, glm::vec3 incidence, MaterialType material_type) const
{
  if( material_type == MATERIAL_TYPE_LAMBERTIAN ) {
    return std::max( glm::dot(reflection, normal), 0.0f) * std::max( glm::dot(-incidence, normal), 0.0f);
  } 
  else if (material_type == MATERIAL_TYPE_EMISSION) {
    return std::max( glm::dot(reflection, normal), 0.0f);
  }
  else {
    return 1.0f;
  }
}

}  // namespace sparks

