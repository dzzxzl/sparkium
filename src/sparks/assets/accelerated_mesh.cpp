#include "sparks/assets/accelerated_mesh.h"

#include "algorithm"

namespace sparks {
AcceleratedMesh::AcceleratedMesh(const Mesh &mesh) : Mesh(mesh) {
  BuildAccelerationStructure();
}

AcceleratedMesh::AcceleratedMesh(const std::vector<Vertex> &vertices,
                                 const std::vector<uint32_t> &indices)
    : Mesh(vertices, indices) {
  BuildAccelerationStructure();
}

float AcceleratedMesh::TraceRay(const glm::vec3 &origin,
                                const glm::vec3 &direction,
                                float t_min,
                                HitRecord *hit_record, const glm::mat4& transform) const {
                                  // LAND_INFO("called through accelerated mesh");
  return Mesh::TraceRay(origin, direction, t_min, hit_record, transform);
}

void AcceleratedMesh::BuildAccelerationStructure() {

  
}

}  // namespace sparks
