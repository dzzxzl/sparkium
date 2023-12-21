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
        // return glm::vec3(1.0f, 0.0f, 0.0f);
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
      else if (material.material_type == MATERIAL_TYPE_NODE) {
        origin = hit_record.position;
        // if(hit_record.tex_coord.x != 0.0f || hit_record.tex_coord.y != 0.0f) {
        //   LAND_INFO("u, v: {:.2f}, {:.2f}", hit_record.tex_coord.x, hit_record.tex_coord.y);
        // }
        for(int eme_iter=0; eme_iter < scene_->GetEntityCount(); eme_iter++){
          auto &eme_material = scene_->GetEntity(eme_iter).GetMaterial();
          if(eme_material.material_type == MATERIAL_TYPE_EMISSION){
            HitRecord eme_hit_record;
            auto eme_origin = scene_->GetEntity(eme_iter).getSamplePoint();
            auto crossSection = scene_->GetEntity(eme_iter).getSurfaceArea();
            auto eme_direction = glm::normalize(eme_origin - origin);
            auto eme_t = scene_->TraceRay(origin, eme_direction, 1e-3f, 1e4f, &eme_hit_record);
            auto distance = glm::distance(eme_hit_record.position, origin);
            if(eme_t > 0.0f){
              // blocked - yes
              if( eme_hit_record.hit_entity_id != eme_iter){
                continue;
              }
              // not blocked 
              // return glm::vec3(0.0f,0.0f,1.0f);
              auto computed_bsdf = nodeBSDF(
                ShaderPreset::CheckerBump,
                scene_, hit_record.hit_entity_id, material.albedo_texture_id,
                hit_record.tex_coord.x, hit_record.tex_coord.y,
                -eme_direction, -direction, hit_record.geometry_normal, hit_record.tangent
              );
              //LAND_INFO("crossSection is {}", crossSection);
              radiance += 
                throughput *
                eme_material.emission * eme_material.emission_strength
                * crossSection / (distance * distance)
                * BSDF(-eme_direction, -eme_hit_record.normal, {}, MATERIAL_TYPE_EMISSION)
                * computed_bsdf
                ;
            }
          }
        }
        break;
        auto old_direction = direction;
        direction = SampleHemisphere(-hit_record.normal);
        auto computed_bsdf = nodeBSDF(
          ShaderPreset::CheckerBump,
          scene_, hit_record.hit_entity_id, material.albedo_texture_id,
          hit_record.tex_coord.x, hit_record.tex_coord.y,
          -direction, -old_direction, -hit_record.normal, hit_record.tangent
        );
        throughput *= computed_bsdf * 2.0f / GINTAMA_P_RR;
         if(genRandFloat(0,1) > GINTAMA_P_RR){
           break;
         }
      }
      else if (material.material_type == MATERIAL_TYPE_LAMBERTIAN) {
        // return glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 material_multiplier = material.albedo_color *
            glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(
                hit_record.tex_coord)};
        origin = hit_record.position;
        // LAND_INFO("texture id: {}", material.albedo_texture_id);
        // if(hit_record.tex_coord.x != 0.0f || hit_record.tex_coord.y != 0.0f) {
        //   LAND_INFO("u, v: {:.2f}, {:.2f}", hit_record.tex_coord.x, hit_record.tex_coord.y);
        // }
        // environment light
        // auto direction_env = scene_->GetEnvmapLightDirection();
        // radiance += throughput * material_multiplier * scene_->GetEnvmapMinorColor();
        // if (scene_->TraceRay(origin, direction_env, 1e-3f, 1e4f, nullptr) < 0.0f) {
        //   radiance += throughput * material_multiplier * 
        //     BSDF(direction, hit_record.normal, direction_env, MATERIAL_TYPE_LAMBERTIAN) 
        //     * scene_->GetEnvmapMajorColor();
        // }
        // direct light
        for(int eme_iter=0; eme_iter < scene_->GetEntityCount(); eme_iter++){
          auto &eme_material = scene_->GetEntity(eme_iter).GetMaterial();
          if(eme_material.material_type == MATERIAL_TYPE_EMISSION){
            HitRecord eme_hit_record;
            auto eme_origin = scene_->GetEntity(eme_iter).getSamplePoint();
            auto pdf_area = scene_->GetEntity(eme_iter).getSurfaceArea();
            auto eme_direction = glm::normalize(eme_origin - origin);
            auto distance = glm::distance(eme_origin, origin);
            auto eme_t = scene_->TraceRay(origin, eme_direction, 1e-3f, 1e4f, &eme_hit_record);
            if(eme_t > 0.0f){
              // blocked - yes
              if( eme_hit_record.hit_entity_id != eme_iter){
                continue;
              } else if( eme_hit_record.position != eme_origin ) {
                // continue;
              }
              // not blocked 
              radiance += throughput * eme_material.emission * eme_material.emission_strength
                * pdf_area / (distance * distance)
                // * BSDF(-eme_direction, -eme_hit_record.normal, {}, MATERIAL_TYPE_EMISSION)
                * std::max(glm::dot(-eme_direction, eme_hit_record.geometry_normal), 0.0f)
                * BSDF( -direction, hit_record.geometry_normal, -eme_direction, MATERIAL_TYPE_LAMBERTIAN )

                * material_multiplier;
            }
          }
        }
        break;
        auto old_direction = direction;
        direction = SampleHemisphere(-hit_record.normal);
        throughput *= material_multiplier * BSDF(-old_direction, -hit_record.normal, -direction, 
         MATERIAL_TYPE_LAMBERTIAN) * 2.0f / GINTAMA_P_RR;
         if(genRandFloat(0,1) > GINTAMA_P_RR){
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

// glm::vec3 PathTracer::SampleHemisphere(glm::vec3 axis) const
// {
//   std::random_device rd;
//   std::default_random_engine eng(rd());
//   std::uniform_real_distribution<float> distr(0.0f, 1.0f);
//   glm::vec3 sampleDirection(0.0f);
//   while (glm::dot(sampleDirection, axis) == 0.0f) {
//     float radius = 1.0f;
//     float pi = glm::radians(180.0f);
//     float theta = distr(eng) * 2.0f * pi;
//     float phi = distr(eng) * 1.0f * pi;

//     // Convert spherical coordinates to Cartesian coordinates
//     float x = radius * glm::sin(phi) * glm::cos(theta);
//     float y = radius * glm::sin(phi) * glm::sin(theta);
//     float z = radius * glm::cos(phi);

//     sampleDirection = glm::vec3(x, y, z);
//     if (glm::dot(sampleDirection, axis) < 0.0f) {
//       sampleDirection = -sampleDirection;
//     }
//   }
//   return sampleDirection;
// }

// float PathTracer::genRanFloat() const
// {
//   std::random_device rd;
//   std::default_random_engine eng(rd());
//   std::uniform_real_distribution<float> distr(0.0f, 1.0f);
//   return distr(eng);
// }



// glm::vec3 PathTracer::BSDF(glm::vec3 reflection, glm::vec3 normal, glm::vec3 incidence, MaterialType material_type) const
// {
//   if( material_type == MATERIAL_TYPE_LAMBERTIAN ) {
//     // return glm::vec3(std::max( glm::dot(reflection, normal), 0.0f) * std::max( glm::dot(-incidence, normal), 0.0f));
//     return glm::vec3( 1.0f / glm::radians(180.0f) );
//   } 
//   else if (material_type == MATERIAL_TYPE_EMISSION) {
//     // return glm::vec3(std::max( glm::dot(reflection, normal), 0.0f));
//     return glm::vec3(1.0f);
//   }
//   else {
//     return glm::vec3(1.0f);
//   }
// }

// glm::vec3 PathTracer::nodeBSDF(
//     ShaderPreset shader_preset,
//     const Scene* scene, int entity_id, int texture_id, 
//     float u, float v, glm::vec3 incident, glm::vec3 reflected, 
//     glm::vec3 normal, glm::vec3 tangent, glm::vec3 position
// ) const {
//   if(shader_preset == CheckerBump){
//     return Presets::checkerBump(scene, entity_id, texture_id, u, v, incident, reflected, normal, tangent);
//   }
//   else {
//     return glm::vec3(0.5f);
//   }
// }

glm::vec3 PathTracer::importanceSample(HitRecord hit_record, 
      glm::vec3 reflection, MaterialType material_type) const {

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
    bool in_glass = glm::dot(hit_record.geometry_normal, hit_record.normal) > 0.0f;
    if(in_glass) {
      return glm::refract(-reflection, -hit_record.geometry_normal, IOR);
    } else {
      return glm::refract(-reflection, hit_record.geometry_normal, 1.0f / IOR);
    }
  }

  else if( material_type == MATERIAL_TYPE_SPECULAR ) {
    return glm::reflect(-reflection, hit_record.geometry_normal);
  }

}

glm::vec3 PathTracer::surfaceBSDF(const Scene* scene, HitRecord hit_record, 
      LightRecord light_record, ShaderPreset shader_preset) const {

  if(shader_preset == ShaderPreset::Lambertian){
    auto material = scene_->GetEntity(hit_record.hit_entity_id).GetMaterial();
    auto albedo = material.albedo_color;
    auto texture = glm::vec3{scene_->GetTextures()[material.albedo_texture_id].Sample(hit_record.tex_coord)};
    return albedo * texture / PI;
  }

  else if(shader_preset == ShaderPreset::CheckerBump) {
    return Presets::checkerBump(scene, hit_record, light_record);
  }

}

}  // namespace sparks

