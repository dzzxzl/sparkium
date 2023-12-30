#include "sparks/nodes/shader/gintama_assets.h"
#include "gintama_assets.h"

namespace sparks {

void initPerlinNoise( unsigned char perm [] , double grad3[][3]) {
    int i, j, k;

    for (i = 0; i < GINTAMA_B; i++) {
        perm[i] = i;
        for (j = 0; j < 3; j++)
            grad3[i][j] = (double)((rand() % (GINTAMA_B + GINTAMA_B)) - GINTAMA_B) / GINTAMA_B;
    }

    while (--i) {
        k = perm[i];
        perm[i] = perm[j = rand() % GINTAMA_B];
        perm[j] = k;
    }

    for (i = 0; i < GINTAMA_B + 2; i++) {
        perm[GINTAMA_B + i] = perm[i];
        for (j = 0; j < 3; j++)
            grad3[GINTAMA_B + i][j] = grad3[i][j];
    }
}

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double grad(int hash, double x, double y) {
    int h = hash & 15;
    double grad = 1 + (h & 7); // Gradient value 1-8
    if (h & 8) grad = -grad;   // Randomly invert half of the gradients
    return (h & 1 ? x : y) * grad;
}

double perlinNoise(double x, double y, unsigned char perm[], double grad3[][3]) {
    auto BM = GINTAMA_BM;
    int X = (int)floor(x) & BM;
    int Y = (int)floor(y) & BM;
    x -= floor(x);
    y -= floor(y);
    double u = fade(x);
    double v = fade(y);
    int A = perm[X] + Y, AA = perm[A], AB = perm[A + 1];
    int B = perm[X + 1] + Y, BA = perm[B], BB = perm[B + 1];

    return lerp(v, lerp(u, grad(perm[AA], x, y), grad(perm[BA], x - 1, y)),
                lerp(u, grad(perm[AB], x, y - 1), grad(perm[BB], x - 1, y - 1)));
}

double perlinNoise(double x, double y, double scale, int detail, double roughness, unsigned char perm[], double grad3[][3]) {
    double total = 0.0;
    double frequency = scale;
    double amplitude = 1.0;

    for (int i = 0; i < detail; i++) {
        total += perlinNoise(x * frequency, y * frequency, perm, grad3) * amplitude;
        frequency *= 2.0;
        amplitude *= roughness;
    }

    return total;
}


double perlinNoiseWithGradient(double x, double y, double scale, int detail, double roughness, double *gradient, unsigned char perm[], double grad3[][3]) {
    double total = 0.0;
    double frequency = scale;
    double amplitude = 1.0;
    double dx = 0.0, dy = 0.0;

    for (int i = 0; i < detail; i++) {
        double n = perlinNoise(x * frequency, y * frequency, perm, grad3);
        total += n * amplitude;

        // Calculate gradient contributions
        double gx = grad(x * frequency, y * frequency, 0);
        double gy = grad(x * frequency, y * frequency, 1);

        dx += gx * amplitude;
        dy += gy * amplitude;

        frequency *= 2.0;
        amplitude *= roughness;
    }

    gradient[0] = dx;
    gradient[1] = dy;

    return total;
}



}