#pragma once

#ifndef GINTAMA_ASSETS_H
#define GINTAMA_ASSETS_H

#define GINTAMA_B 256
#define GINTAMA_BM 256
#define GINTAMA_N 4096
#define GINTAMA_NP 12
#define GINTAMA_NM 4096

#include <cmath>

namespace sparks {

static unsigned char perm[GINTAMA_B + GINTAMA_B + 2];
static double grad3[GINTAMA_B + GINTAMA_B + 2][3];
static bool init_1 = false;
static unsigned char perm_2[GINTAMA_B + GINTAMA_B + 2];
static double grad3_2[GINTAMA_B + GINTAMA_B + 2][3];
static bool init_2 = false;

void initPerlinNoise(unsigned char perm[], double grad3[][3]);

double fade(double t);

double lerp(double t, double a, double b);

double grad(int hash, double x, double y);

double perlinNoise(double x, double y, unsigned char perm[], double grad3[][3]);

double perlinNoise(double x, double y, double scale, int detail, double roughness, unsigned char perm[], double grad3[][3]);

double perlinNoiseWithGradient(double x, double y, double scale, int detail, double roughness, double * gradient, unsigned char perm[], double grad3[][3]);

}



#endif