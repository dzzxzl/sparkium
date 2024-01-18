#include "sparks/assets/camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "sparks/util/util.h"

namespace sparks {

glm::mat4 Camera::GetProjectionMatrix(float aspect,
                                      float t_min,
                                      float t_max) const {
  return glm::scale(glm::mat4{1.0f}, glm::vec3{1.0f, -1.0f, 1.0f}) *
         glm::perspectiveZO(glm::radians(fov_), aspect, t_min, t_max);
}

bool Camera::ImGuiItems() {
  bool value_changed = false;
  value_changed |= ImGui::SliderFloat("FOV", &fov_, 1.0f, 160.0f, "%.0f", 0);
  value_changed |=
      ImGui::SliderFloat("Aperture", &aperture_, 0.0f, 1.0f, "%.2f");
  value_changed |=
      ImGui::SliderFloat("Focal Length", &focal_length_, 0.1f, 10000.0f, "%.2f",
                         ImGuiSliderFlags_Logarithmic);
  value_changed |= ImGui::SliderFloat("Clamp", &clamp_, 1.0f, 1000000.0f,
                                      "%.2f", ImGuiSliderFlags_Logarithmic);
  ImGui::SliderFloat("Gamma", &gamma_, 0.1f, 10.0f);
  ImGui::SliderFloat("Shutter Time", &shutter_time_, 0.0f, 1.0f);
//   value_changed |=
  ImGui::SliderFloat("Lens Radius", &lens_radius_, 0.0f, 10.0f);
    // value_changed |=
  ImGui::SliderFloat("Focus Distance", &focus_distance_, 0.0f, 10000.0f);
  ImGui::Checkbox("Enable Depth of Field", &enable_depth_of_field_);
  return value_changed;
}

void Camera::UpdateFov(float delta) {
  fov_ += delta;
  fov_ = glm::clamp(fov_, 1.0f, 160.0f);
}

void Camera::GenerateRay(float aspect,
                         glm::vec2 range_low,
                         glm::vec2 range_high,
                         glm::vec3 &origin,
                         glm::vec3 &direction,
                         float rand_u,
                         float rand_v,
                         float rand_w,
                         float rand_r,
                         float rand_t) const {
  auto pos = (range_high - range_low) * glm::vec2{rand_u, rand_v} + range_low;
  pos = pos * 2.0f - 1.0f;
  pos.y *= -1.0f;
  origin = glm::vec3{0.0f};
  auto tan_fov = std::tan(glm::radians(fov_ * 0.5f));
  float theta = 2.0f * PI * rand_w;
  float sin_theta = std::sin(theta);
  float cos_theta = std::cos(theta);
  origin =
      glm::vec3{glm::vec2{sin_theta, cos_theta} * rand_r * aperture_, 0.0f};
    if(true) {
        // TODO: if enable motion blur
        origin.x += shutter_time_ * rand_t;
    }

  direction = glm::normalize(
      glm::vec3{tan_fov * aspect * pos.x, tan_fov * pos.y, -1.0f} *
          focal_length_ -
      origin);

    // if(lens_radius_ > 0.0f) {
    //     float lens_x = lens_radius_ * cos_theta * sqrt(rand_r);
    //     float lens_y = lens_radius_ * sin_theta * sqrt(rand_r);
    //     glm::vec3 lens_offset = glm::vec3{glm::vec2{lens_x, lens_y}, 0.0f};
    //     origin += lens_offset;
    //     glm::vec3 focus_point = origin + direction * focus_distance_;
    //     direction = glm::normalize(focus_point - origin);
    // }

    // Vec3 focalPoint = camera.position + direction * camera.focalLength;

    // // Generate a jittered sample within the aperture
    // Vec3 jitteredSample = generateJitteredSample(camera.aperture * 0.5);

    // // Calculate the final ray direction with depth of field
    // Vec3 finalDirection = direction + right * u + up * v + jitteredSample;

    // // Normalize the direction vector
    // finalDirection = finalDirection.normalize();

    // // Calculate the final ray origin with depth of field
    // Vec3 finalOrigin = camera.position + jitteredSample;

    // return (focalPoint - finalOrigin).normalize();

}

Camera::Camera(float fov, float aperture, float focal_length)
    : fov_(fov), aperture_(aperture), focal_length_(focal_length) {
}
}  // namespace sparks
