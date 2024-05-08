#include "scene.h"

Scene::Scene(std::string sceneDirectory, std::string sceneJson)
{
    nlohmann::json sceneConfig;
    try {
        sceneConfig = nlohmann::json::parse(sceneJson);
    }
    catch (std::runtime_error e) {
        std::cerr << "Could not parse json." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

Scene::Scene(std::string pathToJson)
{
    std::string sceneDirectory;

#ifdef _WIN32
    const size_t last_slash_idx = pathToJson.rfind('\\');
#else
    const size_t last_slash_idx = pathToJson.rfind('/');
#endif

    if (std::string::npos != last_slash_idx) {
        sceneDirectory = pathToJson.substr(0, last_slash_idx);
    }

    nlohmann::json sceneConfig;
    try {
        std::ifstream sceneStream(pathToJson.c_str());
        sceneStream >> sceneConfig;
    }
    catch (std::runtime_error e) {
        std::cerr << "Could not load scene .json file." << std::endl;
        exit(1);
    }

    this->parse(sceneDirectory, sceneConfig);
}

void Scene::parse(std::string sceneDirectory, nlohmann::json sceneConfig)
{
    Vector3f from, to, up;

    // Output
    try {
        auto res = sceneConfig["output"]["resolution"];
        this->imageResolution = Vector2i(res[0], res[1]);
    }
    catch (nlohmann::json::exception e) {
        std::cerr << "\"output\" field with resolution, filename & spp should be defined in the scene file." << std::endl;
        exit(1);
    }

    // Cameras
    try {
        auto cam = sceneConfig["camera"];
        from = Vector3f(cam["from"][0], cam["from"][1], cam["from"][2]);
        to = Vector3f(cam["to"][0], cam["to"][1], cam["to"][2]);
        up = Vector3f(cam["up"][0], cam["up"][1], cam["up"][2]);

        this->camera = Camera(
            Vector3f(0, 0, 0),
            to - from,
            up,
            float(cam["fieldOfView"]),
            this->imageResolution
        );
    }
    catch (nlohmann::json::exception e) {
        std::cerr << "No camera(s) defined. Atleast one camera should be defined." << std::endl;
        exit(1);
    }

    // Surface
    try {
        auto surfacePaths = sceneConfig["surface"];

        uint32_t surfaceIdx = 0;
        for (std::string surfacePath : surfacePaths) {
            surfacePath = sceneDirectory + "/" + surfacePath;

            auto surf = createSurfaces(surfacePath, /*isLight=*/false, /*idx=*/surfaceIdx, /*cameraCoords=*/ from);
            this->surfaces.insert(this->surfaces.end(), surf.begin(), surf.end());

            surfaceIdx = surfaceIdx + surf.size();
        }
    }
    catch (nlohmann::json::exception e) {
        std::cout << "No surfaces defined." << std::endl;
    }
}

void Scene::createBoundingBoxes()
{
    for (auto surf : this->surfaces) {
        AABB box;
        for (auto vertex : surf.vertices) {
            box.pmin = Min(box.pmin, vertex);
            box.pmax = Max(box.pmax, vertex);
        }
        box.centroid = (box.pmin + box.pmax) * 0.5f;
        this->boxes.push_back(box);
    }
}

void Scene::createBVH()
{
    BVHNode root;
    root.left = 0;
    root.count = this->surfaces.size();
    this->bvh.push_back(root);
    this->updateBVHNodeBounds(0);
    this->subdivideBVHNode(0);
}

void Scene::updateBVHNodeBounds(int idx)
{
    BVHNode& node = this->bvh[idx];
    AABB& bound = node.aabb;
    for (std::size_t idx = node.left; idx < node.left + node.count; idx++) {
        AABB& box = this->boxes[idx];
        bound.pmax = Max(bound.pmax, box.pmax);
        bound.pmin = Min(bound.pmin, box.pmin);
    }
}

void Scene::subdivideBVHNode(int idx)
{
    BVHNode& node = this->bvh[idx];
    if (node.count <= 2) return;

    Vector3f extent = node.aabb.pmax - node.aabb.pmin;
    int axis = extent.MaxAxis();
    float split = node.aabb.pmin[axis] + extent[axis] * 0.5f;

    int i = node.left, j = node.left + node.count - 1;
    while (i <= j) {
        if (this->boxes[i].centroid[axis] < split)
            i++;
        else {
            std::swap(this->boxes[i], this->boxes[j]);
            std::swap(this->surfaces[i], this->surfaces[j]);
            j--;
        }
    }

    int leftCount = i - node.left;
    if (leftCount == 0 || leftCount == node.count) return;

    int leftChildIdx = this->bvh.size();
    int rightChildIdx = this->bvh.size() + 1;

    BVHNode leftChild, rightChild;
    leftChild.left = node.left;
    leftChild.count = leftCount;
    rightChild.left = i;
    rightChild.count = node.count - leftCount;
    this->bvh.push_back(leftChild);
    this->bvh.push_back(rightChild);

    this->bvh[idx].left = leftChildIdx;
    this->bvh[idx].count = 0;
    updateBVHNodeBounds(leftChildIdx);
    updateBVHNodeBounds(rightChildIdx);
    subdivideBVHNode(leftChildIdx);
    subdivideBVHNode(rightChildIdx);
}

void Scene::createTriangleBVH()
{
    this->trianglebvh.resize(this->surfaces.size());
    for (std::size_t idx = 0; idx < this->surfaces.size(); idx++) {
        BVHNode root;
        root.aabb = this->boxes[idx];
        root.left = 0;
        root.count = this->surfaces[idx].centroids.size();
        this->trianglebvh[idx].push_back(root);
        this->subdivideTriangleBVHNode(idx, 0);
    }
}

void Scene::updateTriangleBVHNodeBounds(int surface, int idx)
{
    BVHNode& node = this->trianglebvh[surface][idx];
    AABB& bound = node.aabb;
    for (std::size_t idx = node.left; idx < node.left + node.count; idx++) {
        bound.pmax = Max(bound.pmax, this->surfaces[surface].vertices[idx * 3]);
        bound.pmax = Max(bound.pmax, this->surfaces[surface].vertices[idx * 3 + 1]);
        bound.pmax = Max(bound.pmax, this->surfaces[surface].vertices[idx * 3 + 2]);
        bound.pmin = Min(bound.pmin, this->surfaces[surface].vertices[idx * 3]);
        bound.pmin = Min(bound.pmin, this->surfaces[surface].vertices[idx * 3 + 1]);
        bound.pmin = Min(bound.pmin, this->surfaces[surface].vertices[idx * 3 + 2]);
    }
}

void Scene::subdivideTriangleBVHNode(int surface, int idx)
{
    BVHNode& node = this->trianglebvh[surface][idx];
    if (node.count <= 2) return;

    Vector3f extent = node.aabb.pmax - node.aabb.pmin;
    int axis = extent.MaxAxis();
    float split = node.aabb.pmin[axis] + extent[axis] * 0.5f;

    int i = node.left, j = node.left + node.count - 1;
    while (i <= j) {
        if (this->surfaces[surface].centroids[i][axis] < split)
            i++;
        else {
            std::swap(this->surfaces[surface].vertices[i * 3], this->surfaces[surface].vertices[j * 3]);
            std::swap(this->surfaces[surface].vertices[i * 3 + 1], this->surfaces[surface].vertices[j * 3 + 1]);
            std::swap(this->surfaces[surface].vertices[i * 3 + 2], this->surfaces[surface].vertices[j * 3 + 2]);
            std::swap(this->surfaces[surface].normals[i * 3], this->surfaces[surface].normals[j * 3]);
            std::swap(this->surfaces[surface].normals[i * 3 + 1], this->surfaces[surface].normals[j * 3 + 1]);
            std::swap(this->surfaces[surface].normals[i * 3 + 2], this->surfaces[surface].normals[j * 3 + 2]);
            std::swap(this->surfaces[surface].centroids[i], this->surfaces[surface].centroids[j]);
            std::swap(this->surfaces[surface].avgnorms[i], this->surfaces[surface].avgnorms[j]);
            j--;
        }
    }

    int leftCount = i - node.left;
    if (leftCount == 0 || leftCount == node.count) return;

    int leftChildIdx = this->trianglebvh[surface].size();
    int rightChildIdx = this->trianglebvh[surface].size() + 1;

    BVHNode leftChild, rightChild;
    leftChild.left = node.left;
    leftChild.count = leftCount;
    rightChild.left = i;
    rightChild.count = node.count - leftCount;
    this->trianglebvh[surface].push_back(leftChild);
    this->trianglebvh[surface].push_back(rightChild);

    this->trianglebvh[surface][idx].left = leftChildIdx;
    this->trianglebvh[surface][idx].count = 0;
    updateTriangleBVHNodeBounds(surface, leftChildIdx);
    updateTriangleBVHNodeBounds(surface, rightChildIdx);
    subdivideTriangleBVHNode(surface, leftChildIdx);
    subdivideTriangleBVHNode(surface, rightChildIdx);
}

Interaction Scene::rayIntersect(Ray& ray)
{
    Interaction siFinal;

    for (auto& surface : this->surfaces) {
        Interaction si = surface.rayIntersect(ray);
        if (si.t <= ray.t) {    
            siFinal = si;
            ray.t = si.t;
        }
    }

    return siFinal;
}

Interaction Scene::AABBIntersect(Ray& ray)
{
    Interaction siFinal;

    for (std::size_t idx = 0; idx < this->boxes.size(); idx++) {
        if (this->boxes[idx].slabTest(ray)) {
            Interaction si = this->surfaces[idx].rayIntersect(ray);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }
        }
    }

    return siFinal;
}

Interaction Scene::BVHIntersect(Ray& ray, int idx)
{
    Interaction si, siFinal;
    BVHNode& node = this->bvh[idx];

    if (node.aabb.slabTest(ray)) {
        if (node.count == 0) {
            si = this->BVHIntersect(ray, node.left);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }

            si = this->BVHIntersect(ray, node.left + 1);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }
        }
        else {
            for (std::size_t i = node.left; i < node.left + node.count; i++) {
                if (this->boxes[i].slabTest(ray)) {
                    si = this->surfaces[i].rayIntersect(ray);
                    if (si.t <= ray.t) {
                        siFinal = si;
                        ray.t = si.t;
                    }
                }
            }
        }
    }

    return siFinal;
}

Interaction Scene::twoLevelBVHIntersect(Ray& ray, int idx)
{
    Interaction si, siFinal;
    BVHNode& node = this->bvh[idx];

    if (node.aabb.slabTest(ray)) {
        if (node.count == 0) {
            si = this->twoLevelBVHIntersect(ray, node.left);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }

            si = this->twoLevelBVHIntersect(ray, node.left + 1);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }
        }
        else {
            for (std::size_t i = node.left; i < node.left + node.count; i++) {
                si = this->triangleBVHIntersect(ray, i, 0);
                if (si.t <= ray.t) {
                    siFinal = si;
                    ray.t = si.t;
                }
            }
        }
    }

    return siFinal;
}

Interaction Scene::triangleBVHIntersect(Ray& ray, int surface, int idx)
{
    Interaction si, siFinal;
    BVHNode& node = this->trianglebvh[surface][idx];

    if (node.aabb.slabTest(ray)) {
        if (node.count == 0) {
            si = this->triangleBVHIntersect(ray, surface, node.left);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }

            si = this->triangleBVHIntersect(ray, surface, node.left + 1);
            if (si.t <= ray.t) {
                siFinal = si;
                ray.t = si.t;
            }
        }
        else {
            for (std::size_t i = node.left; i < node.left + node.count; i++) {
                Vector3f p1 = this->surfaces[surface].vertices[i * 3];
                Vector3f p2 = this->surfaces[surface].vertices[i * 3 + 1];
                Vector3f p3 = this->surfaces[surface].vertices[i * 3 + 2];
                Vector3f n = this->surfaces[surface].avgnorms[i];

                si = this->surfaces[surface].rayTriangleIntersect(ray, p1, p2, p3, n);
                if (si.t <= ray.t && si.didIntersect) {
                    siFinal = si;
                    ray.t = si.t;
                }
            }
        }
    }

    return siFinal;
}

bool AABB::slabTest(Ray& ray)
{
    float tx1, tx2, ty1, ty2, tz1, tz2, tmin, tmax;

    tx1 = (this->pmin.x - ray.o.x) / ray.d.x;
    tx2 = (this->pmax.x - ray.o.x) / ray.d.x;
    tmin = std::min(tx1, tx2);
    tmax = std::max(tx1, tx2);
    
    ty1 = (this->pmin.y - ray.o.y) / ray.d.y;
    ty2 = (this->pmax.y - ray.o.y) / ray.d.y;
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    tz1 = (this->pmin.z - ray.o.z) / ray.d.z;
    tz2 = (this->pmax.z - ray.o.z) / ray.d.z;
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin && tmin < ray.t && tmax > 0;
}
