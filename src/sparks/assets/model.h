#pragma once
#include "glm/glm.hpp"
#include "iostream"
#include "sparks/assets/aabb.h"
#include "sparks/assets/hit_record.h"
#include "sparks/assets/vertex.h"
#include "sparks/util/util.h"
#include "vector"

namespace sparks {
class Model {
 public:
  virtual ~Model() = default;
  [[nodiscard]] virtual float TraceRay(const glm::vec3 &origin,
                                       const glm::vec3 &direction,
                                       float t_min,
                                       HitRecord *hit_record, const glm::mat4& transform) const = 0;
  [[nodiscard]] virtual AxisAlignedBoundingBox GetAABB(
      const glm::mat4 &transform) const = 0;
  [[nodiscard]] virtual std::vector<Vertex> GetVertices() const = 0;
  [[nodiscard]] virtual std::vector<uint32_t> GetIndices() const = 0;
  virtual void setTransform(const glm::mat4& transform) = 0;
  virtual const char *GetDefaultEntityName();
  virtual glm::vec3 samplePoint() const = 0;
  virtual float getSurfaceArea() const = 0;
};
}  // namespace sparks
