#pragma once
#include "sparks/nodes/shader/gintama_Layout.h"
#include "glm/glm.hpp"


namespace sparks{

// struct ShadingCtx {
//     const Scene* scene;
//     HitRecord hit_record;
// };

struct ginRay {
    glm::vec3 origin;
    glm::vec3 direction;
    enum class RayType: int {
        Camera = 0,
        Shadow,
        Reflection,
        Refraction,
        Volume,
        NonCamera
    };
    RayType type;
    void enterMaterial( const Material* material );
    void exitMaterial( const Material* material );
    const Material* getMaterial() const { return materials_.back(); }
    ginRay(glm::vec3 p, glm::vec3 wi);
    ginRay() = default;
    private:
        std::vector<const Material*> materials_;
};

class ginShader {
    public:
        void generateSample( const Scene* scene, glm::vec3 *sample_direction, glm::vec3 *radiance, float *weight);
        void evaluateSample( const Scene* scene, glm::vec3 *sample_direction, glm::vec3 *radiance, float *weight);
    protected:
        std::vector<Node*> nodes_;
        std::vector<Node*> sampler_;
};

class ginVolume {
    public:
        virtual bool Integrate( const Scene* scene, glm::vec3 *p0, ginRay* wi, glm::vec3 *radiance, glm::vec3 *transmittance, glm::vec3 *weight, glm::vec3 *point, ginRay *wo, bool *is_hit_end, HitRecord *end_record ) = 0;
        virtual glm::vec3 Transmittance( const Scene* scene, glm::vec3 *p0, glm::vec3 *p1) = 0;
        glm::vec3 getExtinction() const { return extinction_; }
        glm::vec3 getScatterAlbedo() const { return scatter_albedo_; }
        glm::vec3 getEmission() const { return emission_; }
        float getMaxExtinction() const { return maxExtinction_; }
        void setYLow( float y_low ) { y_low_ = y_low; }
        void setYHigh( float y_high ) { y_high_ = y_high; }
        glm::vec3 computeCurrentExtinction( float y ) const {
            float t = (y - y_low_) / (y_high_ - y_low_);
            float ka = (1.0f - t);
            // ka = ka * ka;
            return extinction_ * ka + extinction_ * extinction_decay_rate_ * (1.0f - ka);
        }
    protected:
        friend ginVolume* createVolume( const Material* material );
        float maxExtinction_{0.5f};
        glm::vec3 extinction_{0.5f};
        glm::vec3 scatter_albedo_{0.6f};
        glm::vec3 emission_{0.1f};
        float extinction_decay_rate_{0.2f};
        // TODO: pointer to aabb box
        float y_low_{-1.0f};
        float y_high_{1.0f};
        float volume_scale_{100.0f};
};

class MultiScatterHeterogeneousVolume: public ginVolume {

    public:
        bool Integrate( const Scene* scene, glm::vec3 *p0, ginRay* wi, glm::vec3 *radiance, glm::vec3 *transmittance, glm::vec3 *weight, glm::vec3 *point, ginRay *wo, bool* is_hit_end, HitRecord *end_record ) override;
        glm::vec3 Transmittance( const Scene* scene, glm::vec3 *p0, glm::vec3 *p1) override;
};

class SingleScatterHeterogeneousVolume: public ginVolume {

    public:
        bool Integrate( const Scene* scene, glm::vec3 *p0, ginRay* wi, glm::vec3 *radiance, glm::vec3 *transmittance, glm::vec3 *weight, glm::vec3 *point, ginRay *wo, bool* is_hit_end, HitRecord *end_record ) override;
        glm::vec3 Transmittance( const Scene* scene, glm::vec3 *p0, glm::vec3 *p1) override;
        
};

ginShader* createBSDF( MaterialType material_type );
ginVolume* createVolume( const Material* material );



}