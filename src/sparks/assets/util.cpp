#include "sparks/assets/util.h"

#include "glm/gtc/matrix_transform.hpp"
#include "grassland/grassland.h"
#include "iostream"
#include "sstream"
#include "unordered_map"
#include "vector"

// clang-format off
#include "imgui.h"
#include "ImGuizmo.h"
#include "util.h"
// clang-format on

namespace sparks {
glm::vec3 DecomposeRotation(glm::mat3 R) {
  return {
      std::atan2(-R[2][1], std::sqrt(R[0][1] * R[0][1] + R[1][1] * R[1][1])),
      std::atan2(R[2][0], R[2][2]), std::atan2(R[0][1], R[1][1])};
}

glm::mat4 ComposeRotation(glm::vec3 pitch_yaw_roll) {
  return glm::rotate(glm::mat4{1.0f}, pitch_yaw_roll.y,
                     glm::vec3{0.0f, 1.0f, 0.0f}) *
         glm::rotate(glm::mat4{1.0f}, pitch_yaw_roll.x,
                     glm::vec3{1.0f, 0.0f, 0.0f}) *
         glm::rotate(glm::mat4{1.0f}, pitch_yaw_roll.z,
                     glm::vec3{0.0f, 0.0f, 1.0f});
}

glm::vec2 StringToVec2(const std::string &s) {
  std::istringstream ss(s);
  std::vector<float> v;
  std::string word;
  while (ss >> word)
    v.push_back(std::stof(word));
  return {v[0], v[1]};
}

glm::vec3 StringToVec3(const std::string &s) {
  std::istringstream ss(s);
  std::vector<float> v;
  std::string word;
  while (ss >> word)
    v.push_back(std::stof(word));
  return {v[0], v[1], v[2]};
}

glm::vec4 StringToVec4(const std::string &s) {
  std::istringstream ss(s);
  std::vector<float> v;
  std::string word;
  while (ss >> word)
    v.push_back(std::stof(word));
  return {v[0], v[1], v[2], v[3]};
}

glm::mat4 XmlTransformMatrix(tinyxml2::XMLElement *transform_element) {
  if (!transform_element)
    return glm::mat4{1.0f};
  std::string transform_type =
      transform_element->FindAttribute("type")->Value();
  if (transform_type == "lookat") {
    glm::vec3 eye{0.0f, 0.0f, 1.0f};
    glm::vec3 center{0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    auto child_element = transform_element->FirstChildElement("eye");
    if (child_element) {
      eye = StringToVec3(child_element->FindAttribute("value")->Value());
    }
    child_element = transform_element->FirstChildElement("center");
    if (child_element) {
      center = StringToVec3(child_element->FindAttribute("value")->Value());
    }
    child_element = transform_element->FirstChildElement("up");
    if (child_element) {
      up = StringToVec3(child_element->FindAttribute("value")->Value());
    }
    return glm::inverse(glm::lookAt(eye, center, up));
  } else if (transform_type == "translate") {
    glm::vec3 translation =
        StringToVec3(transform_element->FindAttribute("value")->Value());
    return glm::translate(glm::mat4{1.0f}, translation);
  } else if (transform_type == "rotate") {
    float angle = glm::radians(
        std::stof(transform_element->FindAttribute("angle")->Value()));
    glm::vec3 v =
        StringToVec3(transform_element->FindAttribute("axis")->Value());
    return glm::rotate(glm::mat4{1.0f}, angle, v);
  } else if (transform_type == "world") {
    glm::vec3 scale{1.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 translation(0.0f);
    auto child_element = transform_element->FirstChildElement("scale");
    if (child_element) {
      scale = StringToVec3(child_element->FindAttribute("value")->Value());
    }
    child_element = transform_element->FirstChildElement("rotation");
    if (child_element) {
      rotation = StringToVec3(child_element->FindAttribute("value")->Value());
    }
    child_element = transform_element->FirstChildElement("translation");
    if (child_element) {
      translation =
          StringToVec3(child_element->FindAttribute("value")->Value());
    }

    glm::mat4 matrix;
    ImGuizmo::RecomposeMatrixFromComponents(
        reinterpret_cast<float *>(&translation),
        reinterpret_cast<float *>(&rotation), reinterpret_cast<float *>(&scale),
        reinterpret_cast<float *>(&matrix));
    return matrix;
  } else {
    LAND_ERROR("Unknown Transformation Type: {}", transform_type);
    return glm::mat4{1.0f};
  }
}

glm::mat4 XmlComposeTransformMatrix(tinyxml2::XMLElement *object_element) {
  glm::mat4 result{1.0f};
  for (auto child_element = object_element->FirstChildElement("transform");
       child_element;
       child_element = child_element->NextSiblingElement("transform")) {
    result *= XmlTransformMatrix(child_element);
  }
  return result;
}

float genRandFloat(float low, float high) {
  return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
}

int genRandInt(int low, int high) {
  return low + rand() % (high - low);
}

float reflectFresnel(glm::vec3 normal, glm::vec3 incident, float eta) {
    float cos_i = glm::dot( -incident, normal );
    float R_0 = (1.0f - eta) / (1.0f + eta);
    R_0 = R_0 * R_0;
    float R = R_0 + (1.0f - R_0) * std::pow(1.0f - cos_i, 5.0f);
    if( 1 - eta * eta * (1 - cos_i * cos_i) < 0.0f ) {
        return 1.0f;
    }
    return R;
}

float GGX_D(glm::vec3 normal, glm::vec3 half, float roughness) {
    float cos_h = glm::dot(half, normal);
    float x_ = cos_h > 0.0f ? 1.0f : 0.0f;
    float cos_h2 = cos_h * cos_h;
    float tan_h2 = (1.0f - cos_h2) / cos_h2;
    float alpha2 = roughness * roughness;
    float alpha2tan2 = alpha2 * tan_h2;
    float denom = cos_h2 * (alpha2 + tan_h2);
    return alpha2 * x_ / (PI * denom * denom);
}

float GGX_G1(glm::vec3 normal, glm::vec3 half, glm::vec3 v, float roughness) {
    float cos_vm = glm::dot(v, half);
    float cos_vn = glm::dot(v, normal);
    if (std::abs(cos_vn) < 1e-3f) {
        return 0.0f;
    }
    float x_ = cos_vm / cos_vn > 0.0f ? 1.0f : 0.0f;
    float tan_vn2 = (1.0f - cos_vn * cos_vn) / (cos_vn * cos_vn);
    float alpha2 = roughness * roughness;
    float alpha2tan2 = alpha2 * tan_vn2;
    return 2.0f * x_ / (1.0f + std::sqrt(1.0f + alpha2tan2));
}

float GGX_G(glm::vec3 normal, glm::vec3 half, glm::vec3 i, glm::vec3 o, float roughness) {
    return GGX_G1(normal, half, i, roughness) * GGX_G1(normal, half, o, roughness);
}

glm::vec3 genRandVec3() {
    float theta = genRandFloat(0, PI);
    float phi = genRandFloat(0, 2 * PI);
    glm::vec3 sample_direction = glm::vec3(
        glm::sin(theta) * glm::cos(phi),
        glm::sin(theta) * glm::sin(phi),
        glm::cos(theta)
    );
    return sample_direction;
}

float rgbtoGray(glm::vec3 rgb) {
    return 0.299f * rgb.r + 0.587f * rgb.g + 0.114f * rgb.b;
}

float MISWeight(int nsamp1, float pdf1, int nsamp2, float pdf2)
{
    float w1 = nsamp1 * pdf1;
    float w2 = nsamp2 * pdf2;
    return 1.0f / (w1 + w2);
}
void genThetaPhih(float u, float v, glm::vec3 *h, float rough_x, float rough_y, glm::vec3 n, glm::vec3 i, glm::vec3 ex, glm::vec3 ey)
{
    float tan_2v = tan(2 * PI * v);
    float phi = atan(rough_y / rough_x * tan_2v);
    if ( v > 0.25f && v <= 0.5f) {
        phi += PI;
    }
    if ( v > 0.75f) {
        phi += 2 * PI;
    }
    if ( v > 0.5f && v <= 0.75f) {
        phi += PI;
    }
    float cos_phi_x = cos(phi) / rough_x;
    float sin_phi_y = sin(phi) / rough_y;
    float cos_phi_x2 = cos_phi_x * cos_phi_x;
    float sin_phi_y2 = sin_phi_y * sin_phi_y;
    float theta = atan(sqrt(-log(u) / (cos_phi_x2 + sin_phi_y2)));
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    float cos_phi = cos(phi);
    float sin_phi = sin(phi);
    *h = 
        sin_theta * cos_phi * ex +
        sin_theta * sin_phi * ey +
        cos_theta * (n);
}

void genThetaPhihWeight(float rho_s, glm::vec3 h, glm::vec3 i, glm::vec3 o, glm::vec3 n, float * weight)
{
  float h_n = glm::dot(h, n);
  float h_i = glm::dot(h, i);
  float i_n = glm::dot(i, n);
  float o_n = glm::dot(o, n);
  float h_n_3 = h_n * h_n * h_n;
  *weight = rho_s * h_n_3 * h_i * sqrt( o_n / i_n );
}

glm::vec3 frGlossy(glm::vec3 rho_s, glm::vec3 n, glm::vec3 h, glm::vec3 i, glm::vec3 o, float rough_x, float rough_y, glm::vec3 ex, glm::vec3 ey)
{
  float hx_x = glm::dot( h, ex ) / rough_x;
  float hy_y = glm::dot( h, ey ) / rough_y;
  float hx_x2 = hx_x * hx_x;
  float hy_y2 = hy_y * hy_y;
  float h_n = glm::dot(h, n);
  float h_n_2 = h_n * h_n;
  float i_n = glm::dot(i, n);
  float o_n = glm::dot(o, n);
  float pre_a = (hx_x2 + hy_y2) / (h_n_2);
  float a = 1.0f / ( 4.0f * PI * rough_x * rough_y * sqrt( i_n * o_n ) ) * exp( - ( hx_x2 + hy_y2 ) / ( h_n_2 ) );
  // float a = 1.0f / ( 4.0f * PI * rough_x * rough_y * sqrt( i_n * o_n ) ) ;
  LAND_INFO("exp term: {} -> {} -> {} -> {}", glm::dot( ex, ex ), rough_x, - ( hx_x2 + hy_y2 ) / ( h_n_2 ), exp( - ( hx_x2 + hy_y2 ) / ( h_n_2 ) ));
  return rho_s * a;
}

}  // namespace sparks
