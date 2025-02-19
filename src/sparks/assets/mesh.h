#pragma once
#include "sparks/assets/model.h"
#include "sparks/assets/util.h"
#include "sparks/assets/vertex.h"
#include "vector"

namespace sparks {
class Mesh : public Model {
 public:
  Mesh() = default;
  Mesh(const Mesh &mesh);
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices);
  explicit Mesh(const tinyxml2::XMLElement *element);
  ~Mesh() override = default;
  [[nodiscard]] float TraceRay(const glm::vec3 &origin,
                               const glm::vec3 &direction,
                               float t_min,
                               HitRecord *hit_record, const glm::mat4& transform) const override;
  const char *GetDefaultEntityName() override;
  [[nodiscard]] AxisAlignedBoundingBox GetAABB(
      const glm::mat4 &transform) const override;
  [[nodiscard]] std::vector<Vertex> GetVertices() const override;
  [[nodiscard]] std::vector<uint32_t> GetIndices() const override;
  static Mesh Cube(const glm::vec3 &center, const glm::vec3 &size);
  static Mesh Sphere(const glm::vec3 &center = glm::vec3{0.0f},
                     float radius = 1.0f);
  static bool LoadObjFile(const std::string &obj_file_path, Mesh &mesh);
  void WriteObjFile(const std::string &file_path) const;
  void MergeVertices();
  glm::vec3 samplePoint() const override;
  float getSurfaceArea() const override;
  void setTransform(const glm::mat4& transform) override {transform_ = transform;}
  bool RayIntersectsBox(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir,
                      const glm::vec3 &boxMin, const glm::vec3 &boxMax) const;

 protected:
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
  float recompute_surface_area() const;
  glm::mat4 transform_{glm::mat4{1.0f}};
};
}  // namespace sparks
