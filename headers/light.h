#pragma once

#include "common.h"

enum LightType {
    POINT_LIGHT=0,
    DIRECTIONAL_LIGHT=1,
    NUM_LIGHT_TYPES
};

struct Light {
    Vector3f location;
    Vector3f radiance;
    LightType type;
    
    Light(Vector3f loc, Vector3f rad, LightType type) : location(loc), radiance(rad), type(type) {  }
    
    Ray generateShadowRay(Vector3f p, Vector3f n);
    Vector3f shade(Vector3f p, Vector3f n, Vector3f color);
};

std::vector<Light> loadLights(nlohmann::json sceneConfig);