#pragma once
#include "memory"
#include "sparks/assets/material.h"
#include "sparks/assets/mesh.h"
#include "sparks/assets/model.h"
#include "sparks/assets/util.h"

namespace sparks {
class Entity {
 public:
  template <class ModelType>
  Entity(const ModelType &model,
         const Material &material,
         const glm::mat4 &transform = glm::mat4{1.0f}) {
    model_ = std::make_unique<ModelType>(model);
    material_ = material;
    transform_ = transform;
    name_ = model_->GetDefaultEntityName();
  }

  template <class ModelType>
  Entity(const ModelType &model,
         const Material &material,
         const glm::mat4 &transform,
         const std::string &name) {
    model_ = std::make_unique<ModelType>(model);
    material_ = material;
    transform_ = transform;
    name_ = name;
  }
  [[nodiscard]] const Model *GetModel() const;
  [[nodiscard]] glm::mat4 &GetTransformMatrix();
  [[nodiscard]] const glm::mat4 &GetTransformMatrix() const;
  [[nodiscard]] Material &GetMaterial();
  [[nodiscard]] const Material &GetMaterial() const;
  [[nodiscard]] const std::string &GetName() const;
  [[nodiscard]] float getSurfaceArea() const;
  [[nodiscard]] glm::vec3 getSamplePoint() const;
  void resetaabb();
  bool checkaabb(glm::vec3 origin, glm::vec3 direction) {
    // LAND_INFO("checking aabb");
    auto diff = transform_ - last_transform_;
    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
        if(std::abs(diff[i][j]) > 1e-3f) {
          LAND_INFO("transform changed");
          resetaabb();
          break;
        }
      }
    }
    return aabb_.IsIntersect(origin, direction, 1e-3f, 1e4f);
  };

 private:
  std::unique_ptr<Model> model_;
  Material material_{};
  glm::mat4 transform_{1.0f};
  glm::mat4 last_transform_{1.0f};
  std::string name_;
  AxisAlignedBoundingBox aabb_{};
};
}  // namespace sparks
