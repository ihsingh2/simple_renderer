#pragma once

#include "vec.h"

// Forward declaration of BSDF class
class BSDF;

struct Interaction {
    // Position of interaction
    Vector3f p;
    // Normal of the surface at interaction
    Vector3f n;
    // The uv co-ordinates at the intersection point
    Vector2f uv;
    // The viewing direction in local shading frame
    Vector3f wi; 
    // Distance of intersection point from origin of the ray
    float t = 1e30f; 
    // Used for light intersection, holds the radiance emitted by the emitter.
    Vector3f emissiveColor = Vector3f(0.f, 0.f, 0.f);
    // BSDF at the shading point
    BSDF* bsdf;
    // Vectors defining the orthonormal basis
    Vector3f a, b, c;

    bool didIntersect = false;

    Vector3f toWorld(Vector3f w) {
        return Vector3f(
            (this->a.x * w.x) + (this->b.x * w.y) + (this->c.x * w.z),
            (this->a.y * w.x) + (this->b.y * w.y) + (this->c.y * w.z),
            (this->a.z * w.x) + (this->b.z * w.y) + (this->c.z * w.z)
        );
    }

    Vector3f toLocal(Vector3f w) {
        return Vector3f(
            Dot(this->a, w),
            Dot(this->b, w),
            Dot(this->c, w)
        );
    }
};