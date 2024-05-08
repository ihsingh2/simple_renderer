#include "light.h"
#include "common.h"

std::vector<Light> loadLights(nlohmann::json sceneConfig)
{
    std::vector<Light> lights;

    try {
        auto directionalLights = sceneConfig["directionalLights"];
        try {
            for (auto light : directionalLights) {
                if (light["direction"].size() == 3 && light["radiance"].size() == 3) {
                    Vector3f direction = Vector3f(light["direction"][0], light["direction"][1], light["direction"][2]);
                    Vector3f radiance = Vector3f(light["radiance"][0], light["radiance"][1], light["radiance"][2]);
                    Light var = Light(direction, radiance, DIRECTIONAL_LIGHT);
                    lights.push_back(var);
                }
                else {
                    std::cerr << "One of the lights has incorrectly specified attributes, may cause unexpected behaviour." << std::endl;
                }
            }
        }
        catch (nlohmann::json::exception e) {
            std::cerr << "One of the lights has missing attributes, may cause unexpected behaviour." << std::endl;
        }
    }
    catch (nlohmann::json::exception e) {
        std::cout << "No directional lights defined." << std::endl;
    }

    try {
        auto pointLights = sceneConfig["pointLights"];
        try {
            for (auto light : pointLights) {
                if (light["location"].size() == 3 && light["radiance"].size() == 3) {
                    Vector3f location = Vector3f(light["location"][0], light["location"][1], light["location"][2]);
                    Vector3f radiance = Vector3f(light["radiance"][0], light["radiance"][1], light["radiance"][2]);
                    Light var = Light(location, radiance, POINT_LIGHT);
                    lights.push_back(var);
                }
                else {
                    std::cerr << "One of the lights has incorrectly specified attributes, may cause unexpected behaviour." << std::endl;
                }
            }
        }
        catch (nlohmann::json::exception e) {
            std::cerr << "One of the lights has missing attributes, may cause unexpected behaviour." << std::endl;
        }
    }
    catch (nlohmann::json::exception e) {
        std::cout << "No point lights defined." << std::endl;
    }

    return lights;
}

Ray Light::generateShadowRay(Vector3f p, Vector3f n)
{
    Ray shadowRay;

    switch (this->type)
    {
        case POINT_LIGHT:
        {
            Vector3f direction = this->location - p;
            if (Dot(n, direction) >= 0.0f) {
                float distance = direction.Length();
                shadowRay = Ray(p, Normalize(direction), distance, distance);
            }
            else
                shadowRay.tmax = -1;
            break;
        }

        case DIRECTIONAL_LIGHT:
        {
            if (Dot(n, this->location) >= 0.0f)
                shadowRay = Ray(p, this->location);
            else
                shadowRay.tmax = -1;
            break;
        }
    }

    return shadowRay;
}

Vector3f Light::shade(Vector3f p, Vector3f n, Vector3f color)
{
    Vector3f rad;

    switch (this->type)
    {
        case POINT_LIGHT:
        {
            Vector3f direction = this->location - p;
            float r2 = direction.LengthSquared();
            Vector3f brdf = color / M_PI;
            float cosTheta = Dot(Normalize(direction), n);
            rad = (this->radiance * brdf) / r2 * cosTheta;
            break;
        }

        case DIRECTIONAL_LIGHT:
        {
            Vector3f brdf = color / M_PI;
            float cosTheta = Dot(this->location, n);
            rad = this->radiance * brdf * cosTheta;
            break;
        }
    }

    return rad;
}