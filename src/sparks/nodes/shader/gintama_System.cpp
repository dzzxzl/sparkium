#include "gintama_System.h"



bool sparks::MultiScatterHeterogeneousVolume::Integrate(const Scene * scene, glm::vec3 *p0, ginRay * wi, glm::vec3 * radiance, glm::vec3 * transmittance, glm::vec3 * weight, glm::vec3 * point, ginRay * wo, bool *is_hit_end, HitRecord* end_record)
{

    // glm::vec3 p0; // TODO

    glm::vec3 p1 = *p0;
    HitRecord hit_record;

    auto distance = scene->TraceRay(wi->origin, wi->direction, 1e-3f, 1e4f, &hit_record);

    if (distance < 0.0f) {
        return false;
    }

    bool terminated = false;
    float t = 0.0f;
    do {
        float zeta = genRandFloat(0,1);
        t = t - log(1-zeta) / maxExtinction_;
        if (t > distance) {
            *is_hit_end = true;
            p1 = *p0 + t * wi->direction;
            break;
        }
        p1 = *p0 + t * wi->direction;
        // recompute inputs
        // TODO
        float xi = genRandFloat(0,1);
        float channel_avg = (extinction_.x + extinction_.y + extinction_.z) / 3.0f;
        if (xi < (channel_avg / maxExtinction_)) {
            terminated = true;
        }
    } while (!terminated);

    if(terminated) {
        *transmittance = glm::vec3{1.0f};
        *weight = scatter_albedo_  / 1.0f;
    } else {
        *transmittance = glm::vec3{1.0f};
        *weight = glm::vec3{1.0f};
    }
    *radiance = glm::vec3{0.0f};
    *wo = ginRay( p1, wi->direction );
    return true;
}

glm::vec3 sparks::MultiScatterHeterogeneousVolume::Transmittance(const Scene * scene, glm::vec3 * p0, glm::vec3 * p1)
{
    return glm::vec3(1.0f);
}

void sparks::ginRay::enterMaterial(const Material * material)
{
    materials_.push_back(material);
}

void sparks::ginRay::exitMaterial(const Material * material)
{
    materials_.pop_back();
}

sparks::ginRay::ginRay(glm::vec3 p, glm::vec3 wi)
{
    origin = p;
    direction = wi;
    type = RayType::Camera;
}

sparks::ginShader * sparks::createBSDF(MaterialType material_type)
{
return nullptr;
}

sparks::ginVolume * sparks::createVolume(const Material* material)
{
    auto volume = new SingleScatterHeterogeneousVolume();
    volume->extinction_ = material->extinction;
    volume->maxExtinction_ = material->max_extinction;
    volume->scatter_albedo_ = material->scatter_albedo;
    volume->emission_ = material->volume_emission_color;
    volume->volume_scale_ = material->volume_scale_;
    volume->extinction_decay_rate_ = material->extinction_decay_rate_;
    return volume;
}

void sparks::ginShader::generateSample(const Scene * scene, glm::vec3 * sample_direction, glm::vec3 * radiance, float * weight)
{
}

void sparks::ginShader::evaluateSample(const Scene * scene, glm::vec3 * sample_direction, glm::vec3 * radiance, float * weight)
{
}

bool sparks::SingleScatterHeterogeneousVolume::Integrate(const Scene * scene, glm::vec3 * p0, ginRay * wi, glm::vec3 * radiance, glm::vec3 * transmittance, glm::vec3 * weight, glm::vec3 * point, ginRay * wo, bool * is_hit_end, HitRecord* end_record)
{
    glm::vec3 p1 = *p0;
    HitRecord hit_record;
    auto distance = scene->TraceRay(wi->origin, wi->direction, 1e-3f, 1e4f, &hit_record);
    *end_record = hit_record;

    if (distance < 0.0f) {
        return false;
    }

    bool terminated = false;
    float t = 0.0f;
    do {
        float zeta = genRandFloat(0,1);
        t = t - log(1-zeta) / maxExtinction_;
        if (t > distance / volume_scale_) {
            *is_hit_end = true;
            // p1 = *p0 + t * wi->direction * volume_scale_;
            break;
        }
        p1 = *p0 + t * wi->direction * volume_scale_;
        // recompute inputs
        auto current_extinction = computeCurrentExtinction(p1.y);
        // TODO
        float xi = genRandFloat(0,1);
        // float channel_avg = (extinction_.x + extinction_.y + extinction_.z) / 3.0f;
        float channel_avg = (current_extinction.x + current_extinction.y + current_extinction.z) / 3.0f;
        if (xi < (channel_avg)) {
            terminated = true;
        }
    } while (!terminated);

    if(terminated) {
        auto current_extinction = computeCurrentExtinction(p1.y);
        *transmittance = glm::vec3{0.0f}; // <- check
        // *weight = 1.0f / extinction_ / maxExtinction_;
        *weight = 1.0f / current_extinction / maxExtinction_;
        *p0 = p1;
        *is_hit_end = false;
    } else {
        *transmittance = glm::vec3{1.0f};
        *weight = glm::vec3{1.0f};
        *p0 = hit_record.position;
        *is_hit_end = true;
    }
    *radiance = glm::vec3{0.0f};
    *wo = ginRay( hit_record.position, wi->direction );
    return true;
}

glm::vec3 sparks::SingleScatterHeterogeneousVolume::Transmittance(const Scene * scene, glm::vec3 * p0, glm::vec3 * p1)
{
    // float distance = glm::distance(*p0, *p1);
    // auto current_extinction = computeCurrentExtinction(p0->y);
    // auto end_extinction = computeCurrentExtinction(p1->y);
    // auto effective_extinction = (current_extinction + end_extinction) / 2.0f;
    // return glm::vec3{exp(-distance * effective_extinction.x * maxExtinction_ / volume_scale_), exp(-distance * effective_extinction.y * maxExtinction_ / volume_scale_), exp(-distance * effective_extinction.z * maxExtinction_ / volume_scale_)};

    // return glm::vec3{exp(-distance * extinction_.x * maxExtinction_ / volume_scale_), exp(-distance * extinction_.y * maxExtinction_ / volume_scale_), exp(-distance * extinction_.z * maxExtinction_ / volume_scale_)};
    // return glm::vec3{exp(-distance * extinction_.x * maxExtinction_ ), exp(-distance * extinction_.y * maxExtinction_ ), exp(-distance * extinction_.z * maxExtinction_ )};
    // return glm::vec3{0.0f};
    // return glm::vec3(0.0f);

    float distance = glm::distance(*p0, *p1);
    glm::vec3 dir = glm::normalize(*p1 - *p0);
    bool terminated = false;
    float t = 0.0f;
    do {
        float zeta = genRandFloat(0,1);
        t = t - log(1-zeta) / maxExtinction_;
        if( t > distance / volume_scale_ ) {
            break;
        }
        auto p = *p0 + t * dir * volume_scale_;
        auto current_extinction = computeCurrentExtinction(p.y);
        float xi = genRandFloat(0,1);
        float channel_avg = (current_extinction.x + current_extinction.y + current_extinction.z) / 3.0f;
        if (xi < (channel_avg)) {
            terminated = true;
        }
    } while (!terminated);

    if( terminated ) {
        return glm::vec3(0.0f);
    } else {
        return glm::vec3(1.0f);
    }
}
