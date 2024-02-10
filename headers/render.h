#pragma once

#include "scene.h"

struct Integrator {
    Integrator(Scene& scene);

    long long render(int interpolationVariant);

    Scene scene;
    Texture outputImage;
};