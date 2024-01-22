#pragma once

#include "scene.h"

struct Integrator {
    Integrator(Scene& scene);

    long long render(int intersectionVariant);

    Scene scene;
    Texture outputImage;
};