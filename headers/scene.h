#pragma once

#include "camera.h"
#include "surface.h"

struct AABB {
    Vector3f pmin = Vector3f(1e30f, 1e30f, 1e30f);
    Vector3f pmax = Vector3f(-1e30f, -1e30f, -1e30f);
    Vector3f centroid;

    bool slabTest(Ray& ray);
};

struct BVHNode {
    AABB aabb;
    int left, count;
        /*  for internal nodes:
                count = 0
                left = {index of left child} = {index of right child} - 1
            for leaf nodes:
                count = {number of surfaces/triangles** enclosed}
                left = {index of the leftmost surface/triangle**}
            **each surface/triangle being consecutive in their respective vector */
};

struct Scene {
    std::vector<Surface> surfaces;
    std::vector<AABB> boxes; // around each surface
    std::vector<BVHNode> bvh; // hierarchy over each box
    std::vector<std::vector<BVHNode>> trianglebvh; // hierarchy over each triangle, for each box

    Camera camera;
    Vector2i imageResolution;

    Scene() {};
    Scene(std::string sceneDirectory, std::string sceneJson);
    Scene(std::string pathToJson);
    
    void parse(std::string sceneDirectory, nlohmann::json sceneConfig);

    void createBoundingBoxes();
    
    void createBVH();
    void updateBVHNodeBounds(int idx);
    void subdivideBVHNode(int idx);

    void createTriangleBVH();
    void updateTriangleBVHNodeBounds(int surface, int idx);
    void subdivideTriangleBVHNode(int surface, int idx);

    Interaction rayIntersect(Ray& ray);
    // iterates Scene::surfaces, invokes Surface::rayIntersect

    Interaction AABBIntersect(Ray& ray);
    // iterates Scene::boxes, invokes AABB::slabTest

    Interaction BVHIntersect(Ray& ray, int idx);
    // traverses Scene::bvh, invokes AABB::slabTest and Surface::rayIntersect

    Interaction twoLevelBVHIntersect(Ray& ray, int idx);
    // traverses Scene::bvh, invokes AABB::slabTest and Surface::triangleBVHIntersect

    Interaction triangleBVHIntersect(Ray& ray, int surface, int idx);
    // traverses Scene::trianglebvh, AABB::slabTest and invokes Surface::rayTriangleIntersect
};
