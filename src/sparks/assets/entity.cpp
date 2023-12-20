#include "sparks/assets/entity.h"
#include "entity.h"

namespace sparks {

const Model *Entity::GetModel() const {
  return model_.get();
}

glm::mat4 &Entity::GetTransformMatrix() {
  return transform_;
}

const glm::mat4 &Entity::GetTransformMatrix() const {
  return transform_;
}

Material &Entity::GetMaterial() {
  return material_;
}

const Material &Entity::GetMaterial() const {
  return material_;
}

const std::string &Entity::GetName() const {
  return name_;
}

// glm::vec3 Entity::getCenter() const
// {
//   auto aabb_box = GetModel()->GetAABB(transform_);
//   float x = (aabb_box.x_low + aabb_box.x_high) / 2;
//   float y = (aabb_box.y_low + aabb_box.y_high) / 2;
//   float z = (aabb_box.z_low + aabb_box.z_high) / 2;
//   return glm::vec3(x, y, z);
// }
// float Entity::getCrossSection() const
// {
//   auto aabb_box = GetModel()->GetAABB(transform_);
//   float x = (-aabb_box.x_low + aabb_box.x_high) / 2;
//   float y = (-aabb_box.y_low + aabb_box.y_high) / 2;
//   float z = (-aabb_box.z_low + aabb_box.z_high) / 2;
//   return (x*y+y*z+z*x)/3.0f;
// }

// glm::vec3 Entity::getSamplePoint() const {
//   auto aabb_box = GetModel()->GetAABB(transform_);
//   auto x = genRandFloat(aabb_box.x_low, aabb_box.x_high);
//   auto y = genRandFloat(aabb_box.y_low, aabb_box.y_high);
//   auto z = genRandFloat(aabb_box.z_low, aabb_box.z_high);
//   return glm::vec3(x, y, z);
// }

// float Entity::getArea() const {
//   auto aabb_box = GetModel()->GetAABB(transform_);
//   float x = (-aabb_box.x_low + aabb_box.x_high);
//   float y = (-aabb_box.y_low + aabb_box.y_high);
//   float z = (-aabb_box.z_low + aabb_box.z_high);
//   return 2.0f * (x * y + y * z + z * x);
// }

float Entity::getSurfaceArea() const {
  return model_->getSurfaceArea();
}

glm::vec3 Entity::getSamplePoint() const {
  auto sample_point = model_->samplePoint();
  return glm::vec3(transform_ * glm::vec4(sample_point, 1.0f));
}

}  // namespace sparks
