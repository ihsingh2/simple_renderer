#include "light.h"

Light::Light(LightType type, nlohmann::json config) {
    switch (type) {
        case LightType::POINT_LIGHT:
            this->position = Vector3f(config["location"][0], config["location"][1], config["location"][2]);
            break;
        case LightType::DIRECTIONAL_LIGHT:
            this->direction = Vector3f(config["direction"][0], config["direction"][1], config["direction"][2]);
            break;
        case LightType::AREA_LIGHT:
            this->center = Vector3f(config["center"][0], config["center"][1], config["center"][2]);
            this->vx = Vector3f(config["vx"][0], config["vx"][1], config["vx"][2]);
            this->vy = Vector3f(config["vy"][0], config["vy"][1], config["vy"][2]);
            this->normal = Vector3f(config["normal"][0], config["normal"][1], config["normal"][2]);
            break;
        default:
            std::cout << "WARNING: Invalid light type detected";
            break;
    }

    this->radiance = Vector3f(config["radiance"][0], config["radiance"][1], config["radiance"][2]);
    this->type = type;
}

std::pair<Vector3f, LightSample> Light::sample(Interaction *si, int samplingMethod) {
    LightSample ls;
    memset(&ls, 0, sizeof(ls));

    Vector3f radiance;
    switch (type) {
        case LightType::POINT_LIGHT:
            ls.wo = (position - si->p);
            ls.d = ls.wo.Length();
            ls.wo = Normalize(ls.wo);
            radiance = this->radiance;
            radiance *= std::abs(Dot(si->n, ls.wo));
            radiance *= (1.f / (ls.d * ls.d));
            break;
        case LightType::DIRECTIONAL_LIGHT:
            ls.wo = Normalize(direction);
            ls.d = 1e10;
            radiance = this->radiance;
            radiance *= std::abs(Dot(si->n, ls.wo));
            break;
        case LightType::AREA_LIGHT:
            float eps1 = next_float();
            float eps2 = next_float();
            switch (samplingMethod) {
                case 0:
                    return this->uniformHemisphereSampling(si, eps1, eps2);
                case 1:
                    return this->cosineWeightedSampling(si, eps1, eps2);
                default:
                    return this->areaLightSampling(si, eps1, eps2);
            }
            break;
    }
    return { radiance, ls };
}

std::pair<Vector3f, LightSample> Light::uniformHemisphereSampling(Interaction *si, float eps1, float eps2) {
    LightSample ls;
    memset(&ls, 0, sizeof(ls));
    Vector3f radiance;

    float theta = std::acos(eps1);
    float phi = 2 * M_PI * eps2;

    Vector3f local(
        std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta)
    );
    ls.wo = si->toWorld(local);
    
    Ray rayLight(si->p, ls.wo);
    Interaction siLight = this->intersectLight(&rayLight);
    if (siLight.didIntersect) {
        radiance = this->radiance;
        radiance *= std::abs(Dot(si->n, ls.wo));
        radiance *= 2 * M_PI;
        ls.d = siLight.t;
    }
    else {
        radiance = Vector3f(0.f, 0.f, 0.f);
        ls.d = 1e30;
    }
    
    return { radiance, ls };
}

std::pair<Vector3f, LightSample> Light::cosineWeightedSampling(Interaction *si, float eps1, float eps2) {
    LightSample ls;
    memset(&ls, 0, sizeof(ls));
    Vector3f radiance;

    float theta = std::acos(1.f - 2.f * eps1) / 2.f;
    float phi = 2 * M_PI * eps2;

    Vector3f local(
        std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta)
    );
    ls.wo = si->toWorld(local);

    Ray rayLight(si->p, ls.wo);
    Interaction siLight = this->intersectLight(&rayLight);
    if (siLight.didIntersect) {
        radiance = this->radiance;
        radiance *= M_PI;
        ls.d = siLight.t;
    }
    else {
        radiance = Vector3f(0.f, 0.f, 0.f);
        ls.d = 1e30;
    }
    
    return { radiance, ls };
}

std::pair<Vector3f, LightSample> Light::areaLightSampling(Interaction *si, float eps1, float eps2) {
    LightSample ls;
    memset(&ls, 0, sizeof(ls));
    Vector3f radiance;

    Vector3f area = this->center + (2 * (eps1 - 0.5) * this->vx) + (2 * (eps2 - 0.5) * this->vy);
    Vector3f dir = area - si->p;
    ls.wo = Normalize(dir);

    Ray rayLight(si->p, ls.wo);
    Interaction siLight = this->intersectLight(&rayLight);
    if (siLight.didIntersect) {
        radiance = this->radiance;
        radiance *= std::abs(Dot(si->n, ls.wo));
        radiance *= std::abs(Dot(this->normal, ls.wo));
        radiance /= dir.LengthSquared();
        radiance *= Cross(this->vx, this->vy).Length() * 4;
        ls.d = siLight.t;
    }
    else {
        radiance = Vector3f(0.f, 0.f, 0.f);
        ls.d = 1e30;
    }

    return { radiance, ls };
}

Interaction Light::intersectLight(Ray *ray) {
    Interaction si;
    memset(&si, 0, sizeof(si));

    if (type == LightType::AREA_LIGHT) {
        float dDotN = Dot(ray->d, this->normal);
        if (dDotN != 0.f) {
            float t = -Dot((ray->o - this->center), this->normal) / dDotN;
            if (t >= 0.f && dDotN < 0.f) {
                si.didIntersect = true;
                si.t = t;
                si.n = this->normal;
                si.p = ray->o + ray->d * si.t;

                Vector3f v1 = this->center - this->vx - this->vy;
                Vector3f v2 = this->center - this->vx + this->vy;
                Vector3f v3 = this->center + this->vx + this->vy;
                Vector3f v4 = this->center + this->vx - this->vy;
                bool edge1 = false, edge2 = false, edge3 = false, edge4 = false;

                {
                    Vector3f nIp = Cross((si.p - v1), (v2 - v1));
                    Vector3f nTri = Cross((v4 - v1), (v2 - v1));
                    edge1 = Dot(nIp, nTri) > 0;
                }

                {
                    Vector3f nIp = Cross((si.p - v2), (v3 - v2));
                    Vector3f nTri = Cross((v1 - v2), (v3 - v2));
                    edge2 = Dot(nIp, nTri) > 0;
                }

                {
                    Vector3f nIp = Cross((si.p - v3), (v4 - v3));
                    Vector3f nTri = Cross((v2 - v3), (v4 - v3));
                    edge3 = Dot(nIp, nTri) > 0;
                }

                {
                    Vector3f nIp = Cross((si.p - v4), (v1 - v4));
                    Vector3f nTri = Cross((v3 - v4), (v1 - v4));
                    edge4 = Dot(nIp, nTri) > 0;
                }

                if (edge1 && edge2 && edge3 && edge4 && si.t >= 0.f) {
                    si.didIntersect = true;
                    si.emissiveColor = this->radiance;
                }
                else {
                    si.didIntersect = false;
                }
            }    
        }
    }

    return si;
}