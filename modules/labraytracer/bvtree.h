#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/boundingbox.h>

#include <vector>
#include <stack>

namespace inviwo
{

class IVW_MODULE_LABRAYTRACER_API BVTree
{
public:
    BVTree();

    //build from indexed triangle set
    void build(const std::vector<dvec3>& vertexPositions,
               const std::vector<ivec3>& triangleIndices);

    //returns a set of triangle indices as candidates for ray-triangle intersection
    const std::vector<int>& intersectBoundingBoxes(const Ray& ray, const double maxLambda) const;

private:
    struct Node
    {
        Node()
        {
            left = 0;
            right = 0;
        }
        int left;
        int right;
        BoundingBox bbox;
    };
    void sortTriangles();
    void createNodes(const std::vector<dvec3>& vertexPositions,
                     const std::vector<ivec3>& triangleIndices);

    void printSortedIndicesStatus(unsigned int offset, unsigned int numTriangles);
    void buildHierarchy(unsigned int rootNodeIndex, unsigned int numTriangles, unsigned int offset);

    void computeBoundingBoxAreas(unsigned int offset, unsigned int numTriangles);

    std::vector<Node> mNodes;

    std::vector<bool> mTempMarker;
    std::vector<BoundingBox> mTempTriangleBoxes;
    std::vector<ivec3> mTempSortedTriangleIndices;
    std::vector<ivec3> mTempBufferTriangleIndices;
    std::vector<dvec3> mTempAreasLeft;
    std::vector<dvec3> mTempAreasRight;
    mutable std::vector<std::vector<int>> mTempCandidates;
    mutable std::vector<std::stack<int>> mTempTraversalJobs;
};
}
