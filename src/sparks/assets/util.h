#pragma once
#include "glm/glm.hpp"
#include "string"
#include "tinyxml2.h"
#include "sparks/util/util.h"

namespace sparks {
glm::vec3 DecomposeRotation(glm::mat3 R);

glm::mat4 ComposeRotation(glm::vec3 pitch_yaw_roll);

glm::vec2 StringToVec2(const std::string &s);

glm::vec3 StringToVec3(const std::string &s);

glm::vec4 StringToVec4(const std::string &s);

glm::mat4 XmlTransformMatrix(tinyxml2::XMLElement *transform_element);

glm::mat4 XmlComposeTransformMatrix(tinyxml2::XMLElement *object_element);

float genRandFloat(float low, float high);

int genRandInt(int low, int high);

float reflectFresnel(glm::vec3 normal, glm::vec3 incident, float eta);

float GGX_D(glm::vec3 normal, glm::vec3 half, float roughness);

float GGX_G1(glm::vec3 normal, glm::vec3 half, glm::vec3 v, float roughness);

float GGX_G(glm::vec3 normal, glm::vec3 half, glm::vec3 i, glm::vec3 o, float roughness);

float genRandFloat(float low, float high);

int genRandInt(int low, int high);

glm::vec3 genRandVec3();

float rgbtoGray(glm::vec3 rgb);

// glm::vec3 good_refraction(glm::vec3 incident, glm::vec3 normal, float ior);

}  // namespace sparks
