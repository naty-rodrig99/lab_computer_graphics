#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/util.h>
#include <labraytracer/bvhindexedtrianglemesh.h>
#include <labraytracer/triangle.h>


namespace inviwo
{

BVHIndexedTriangleMesh::BVHIndexedTriangleMesh()
    : IndexedTriangleMesh()
{
}

void BVHIndexedTriangleMesh::initialize()
{
    mTree.build(this->vertexPositions(), *((const std::vector<ivec3>*)(&this->triangleIndices())));
}

bool BVHIndexedTriangleMesh::closestIntersection(const Ray& ray, double maxLambda,
                                                 RayIntersection& intersection) const
{
    const std::vector<int>& intersectionCandidates = mTree.intersectBoundingBoxes(ray, maxLambda);

    double closestLambda = maxLambda;
    int closestTri = -1;
    dvec3 closestbary(0);
    for (size_t i = 0; i < intersectionCandidates.size(); ++i)
    {
        const int triangleIndex = intersectionCandidates[i];
        const int idx0 = this->triangleIndices()[3 * triangleIndex + 0];
        const int idx1 = this->triangleIndices()[3 * triangleIndex + 1];
        const int idx2 = this->triangleIndices()[3 * triangleIndex + 2];

        const dvec3& p0 = this->vertexPositions()[idx0];
        const dvec3& p1 = this->vertexPositions()[idx1];
        const dvec3& p2 = this->vertexPositions()[idx2];

        std::shared_ptr<Triangle> T = std::make_shared<Triangle>(p0, p1, p2);
        RayIntersection currIntersection;
        const bool bIntersect = T->closestIntersection(ray, closestLambda, currIntersection);
        if (bIntersect && currIntersection.getLambda() > 0 && currIntersection.getLambda() < closestLambda)
        {
            closestLambda = currIntersection.getLambda();
            closestTri = triangleIndex;
            closestbary = currIntersection.getUVW();
        }
    }

    if (closestTri >= 0)
    {
        const int i0 = this->triangleIndices()[3 * closestTri + 0];
        const int i1 = this->triangleIndices()[3 * closestTri + 1];
        const int i2 = this->triangleIndices()[3 * closestTri + 2];

        //const dvec3& p0 = this->vertexPositions()[i0];
        //const dvec3& p1 = this->vertexPositions()[i1];
        //const dvec3& p2 = this->vertexPositions()[i2];

        assert(!vertexNormals().empty());
        dvec3 n(vertexNormals()[i0] * closestbary[0] +
                vertexNormals()[i1] * closestbary[1] +
                vertexNormals()[i2] * closestbary[2]);

        dvec3 uvw(0, 0, 0);
        //if (!this->vertexTextureCoordinates().empty())
        //    uvw = this->vertexTextureCoordinates()[i0] * closestbary[0] +
        //          this->vertexTextureCoordinates()[i1] * closestbary[1] +
        //          this->vertexTextureCoordinates()[i2] * closestbary[2];

        intersection = RayIntersection(ray, shared_from_this(), closestLambda, normalize(n), uvw);
        return true;
    }

    return false;
}


bool BVHIndexedTriangleMesh::anyIntersection(const Ray& ray, double maxLambda) const
{
    std::vector<int> intersectionCandidates = mTree.intersectBoundingBoxes(ray, maxLambda);

    for (size_t i = 0; i < intersectionCandidates.size(); ++i)
    {
        const int triangleIndex = intersectionCandidates[i];
        const int idx0 = this->triangleIndices()[3 * triangleIndex + 0];
        const int idx1 = this->triangleIndices()[3 * triangleIndex + 1];
        const int idx2 = this->triangleIndices()[3 * triangleIndex + 2];

        const dvec3& p0 = this->vertexPositions()[idx0];
        const dvec3& p1 = this->vertexPositions()[idx1];
        const dvec3& p2 = this->vertexPositions()[idx2];

        std::shared_ptr<Triangle> T = std::make_shared<Triangle>(p0, p1, p2);
        if (T->anyIntersection(ray, maxLambda)) return true;
    }

    return false;
}

}

