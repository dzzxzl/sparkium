
struct Material {
  vec3 albedo_color;
  int albedo_texture_id;
  vec3 emission;
  float emission_strength;
  float alpha;
  uint material_type;
  float pad1;
  float pad2;
  vec4 pad3;
  // starting from pad4 is extinction
  vec4 pad4;
  vec4 pad5;
  vec4 pad6;
  // here begins interpolation pad
  vec4 pad7;
  // here begins texture move
  vec4 pad8;
  // here begins glossy components
  vec4 pad9;
  vec4 pad10;
  // here begins color ramp components pad
  vec4 pad11;
  vec4 pad12;
  vec4 pad13;
  // here rough glass globe
  vec4 pad14;
  // here is water
  vec4 pad15;
  // here is chromatic dispersion
  vec4 pad16;
};

#define MATERIAL_TYPE_LAMBERTIAN 0
#define MATERIAL_TYPE_SPECULAR 1
#define MATERIAL_TYPE_TRANSMISSIVE 2
#define MATERIAL_TYPE_PRINCIPLED 3
#define MATERIAL_TYPE_EMISSION 4
#define MATERIAL_TYPE_GLASS 5
#define MATERIAL_TYPE_CHECKERBUMP 6
#define MATERIAL_TYPE_CHECKER_A 7