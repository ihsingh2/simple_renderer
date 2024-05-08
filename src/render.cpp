#include "render.h"

Integrator::Integrator(Scene &scene)
{
    this->scene = scene;
    this->outputImage.allocate(TextureType::UNSIGNED_INTEGER_ALPHA, this->scene.imageResolution);
}

long long Integrator::render(int intersectionVariant)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int x = 0; x < this->scene.imageResolution.x; x++) {
        for (int y = 0; y < this->scene.imageResolution.y; y++) {
            Ray cameraRay = this->scene.camera.generateRay(x, y);
            Interaction si;

            switch (intersectionVariant)
            {
                case 0:
                    si = this->scene.rayIntersect(cameraRay);
                    break;
                case 1:
                    si = this->scene.AABBIntersect(cameraRay);
                    break;
                case 2:
                    si = this->scene.BVHIntersect(cameraRay, 0);
                    break;
                default:
                    si = this->scene.twoLevelBVHIntersect(cameraRay, 0);
            }

            if (si.didIntersect)
                this->outputImage.writePixelColor(0.5f * (si.n + Vector3f(1.f, 1.f, 1.f)), x, y);
            else
                this->outputImage.writePixelColor(Vector3f(0.f, 0.f, 0.f), x, y);
        }
    }
    auto finishTime = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        std::cerr << "Usage: ./render <scene_config> <out_path> <intersection_variant>" << std::endl;
        return 1;
    }
    int intersectionVariant = atoi(argv[3]);

    Scene scene(argv[1]);
    if (intersectionVariant > 0)
        scene.createBoundingBoxes();
    if (intersectionVariant > 1)
        scene.createBVH();
    if (intersectionVariant > 2)
        scene.createTriangleBVH();

    Integrator rayTracer(scene);
    auto renderTime = rayTracer.render(intersectionVariant);

    std::cout << "Render Time: " << std::to_string(renderTime / 1000.f) << " ms" << std::endl;
    rayTracer.outputImage.save(argv[2]);

    return 0;
}
