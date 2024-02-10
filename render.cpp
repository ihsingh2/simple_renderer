#include "render.h"

Integrator::Integrator(Scene &scene)
{
    this->scene = scene;
    this->outputImage.allocate(TextureType::UNSIGNED_INTEGER_ALPHA, this->scene.imageResolution);
}

long long Integrator::render(int interpolationVariant)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            Vector3f color = Vector3f(0.0f, 0.0f, 0.0f);
            Ray cameraRay = this->scene.camera.generateRay(x, y);
            Interaction si = this->scene.rayIntersect(cameraRay);

            if (si.didIntersect) {
                for (auto light: this->scene.lights) {
                    Ray shadowRay = light.generateShadowRay(si.p + (1e-03 * si.n), si.n);
                    if (shadowRay.tmax >= 0.0f) {
                        Interaction siShadow = this->scene.rayIntersect(shadowRay);
                        if (!siShadow.didIntersect) {
                            Vector3f diffuse = this->scene.fetchTextureColor(si, interpolationVariant);
                            color += light.shade(si.p, si.n, diffuse);
                        }
                    }
                }
            }

            this->outputImage.writePixelColor(color, x, y);
        }
    }
    auto finishTime = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        std::cerr << "Usage: ./render <scene_config> <out_path> <interpolation_variant>" << std::endl;
        return 1;
    }
    Scene scene(argv[1]);

    Integrator rayTracer(scene);
    auto renderTime = rayTracer.render(atoi(argv[3]));
    
    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
