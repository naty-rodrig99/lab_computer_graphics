#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/ray.h>
#include <labraytracer/indexedtrianglemesh.h>
#include <labraytracer/bvtree.h>

namespace inviwo
{
class IVW_MODULE_LABRAYTRACER_API BVHIndexedTriangleMesh : public IndexedTriangleMesh
{
public:
    BVHIndexedTriangleMesh();

    void initialize() override;

    bool closestIntersection(const Ray& ray, double maxLambda,
                             RayIntersection& intersection) const override;

    bool anyIntersection(const Ray& ray, double maxLambda) const override;

private:
    BVTree mTree;
};
}
