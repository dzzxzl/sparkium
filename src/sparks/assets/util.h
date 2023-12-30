#pragma once
#include "glm/glm.hpp"
#include "string"
#include "tinyxml2.h"
#include "sparks/util/util.h"
#include <cmath>

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

float MISWeight(int nsamp1, float pdf1, int nsamp2, float pdf2);

void genThetaPhih(float u, float v, glm::vec3 * h, float rough_x, float rough_y, glm::vec3 n, glm::vec3 i, glm::vec3 ex, glm::vec3 ey);

void genThetaPhihWeight( float rho_s, glm::vec3 h, glm::vec3 i, glm::vec3 o, glm::vec3 n, float *weight);

glm::vec3 frGlossy(glm::vec3 rho_s, glm::vec3 n, glm::vec3 h, glm::vec3 i, glm::vec3 o, float rough_x, float rough_y, glm::vec3 ex, glm::vec3 ey);

// glm::vec3 good_refraction(glm::vec3 incident, glm::vec3 normal, float ior);

}  // namespace sparks
