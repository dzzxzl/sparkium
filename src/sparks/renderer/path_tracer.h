#pragma once
#include "random"
#include "sparks/assets/scene.h"
#include "sparks/renderer/renderer_settings.h"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "sparks/assets/material.h"

namespace sparks {
class PathTracer {
 public:
  PathTracer(const RendererSettings *render_settings, const Scene *scene);
  [[nodiscard]] glm::vec3 SampleRay(glm::vec3 origin,
                                    glm::vec3 direction,
                                    int x,
                                    int y,
                                    int sample) const;
  [[nodiscard]] glm::vec3 SampleHemisphere(glm::vec3 axis) const;
  [[nodiscard]] float genRanFloat() const;
  [[nodiscard]] float BSDF(glm::vec3 reflection, glm::vec3 normal, glm::vec3 incidence, MaterialType material_type) const;

 private:
  const RendererSettings *render_settings_{};
  const Scene *scene_{};
};
}  // namespace sparks
