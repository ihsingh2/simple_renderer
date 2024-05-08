#include "render.h"
#include "random.h"

Integrator::Integrator(Scene &scene)
{
    this->scene = scene;
    this->outputImage.allocate(TextureType::UNSIGNED_INTEGER_ALPHA, this->scene.imageResolution);
}

long long Integrator::render(int spp, int samplingMethod)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            Vector3f result(0.f, 0.f, 0.f);
            for (int s = 0; s < spp; s++) {
                Ray cameraRay = this->scene.camera.generateRay(x, y, next_float(), next_float());
                Interaction si = this->scene.rayIntersect(cameraRay);
                if (si.didIntersect) {
                    Vector3f radiance;
                    LightSample ls;
                    for (Light &light : this->scene.lights) {
                        std::tie(radiance, ls) = light.sample(&si, samplingMethod);
                        if (radiance == Vector3f(0.f, 0.f, 0.f))
                            continue;

                        Ray shadowRay(si.p + 1e-3f * si.n, ls.wo);
                        Interaction siShadow = this->scene.rayIntersect(shadowRay);

                        if (!siShadow.didIntersect || siShadow.t > ls.d) {
                            result += si.bsdf->eval(&si, si.toLocal(ls.wo)) * radiance;
                        }
                    }
                }
                Interaction siEmitter = this->scene.rayEmitterIntersect(cameraRay);
                if (siEmitter.didIntersect) {
                    result += siEmitter.emissiveColor;
                }
            }

            this->outputImage.writePixelColor(result / spp, x, y);
        }
    }
    auto finishTime = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
}

int main(int argc, char **argv)
{
    if (argc != 5) {
        std::cerr << "Usage: ./render <scene_config> <out_path> <num_samples> <sampling_strategy>";
        return 1;
    }
    Scene scene(argv[1]);

    Integrator rayTracer(scene);
    int spp = atoi(argv[3]);
    int samplingMethod = atoi(argv[4]);
    auto renderTime = rayTracer.render(spp, samplingMethod);
    
    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
