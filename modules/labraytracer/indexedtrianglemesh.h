#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/renderable.h>
#include <labraytracer/ray.h>
#include <labraytracer/boundingbox.h>

namespace inviwo
{

class IVW_MODULE_LABRAYTRACER_API IndexedTriangleMesh : public Renderable
{
public:
    int addVertex(const dvec3& v, const dvec3& n, const dvec3& uvw)
    {
        mVertexPosition.push_back(v);
        mVertexNormal.push_back(n);
        mVertexTextureCoordinate.push_back(uvw);

        BBox.expandByPoint(v);

        return int(mVertexPosition.size() - 1);
    }

    int addVertex(const dvec3& v, const dvec3& n)
    {
        mVertexPosition.push_back(v);
        mVertexNormal.push_back(n);
        //mVertexTextureCoordinate.push_back(uvw);

        BBox.expandByPoint(v);

        return int(mVertexPosition.size() - 1);
    }

    void addTriangle(const int i0, const int i1, const int i2)
    {
        mIndices.push_back(i0);
        mIndices.push_back(i1);
        mIndices.push_back(i2);
    }

    void reserveVertexPositions(const size_t NewCapacity)
    {
        mVertexPosition.reserve(NewCapacity);
    }

    void reserveVertexTextureCoordinates(const size_t NewCapacity)
    {
        mVertexTextureCoordinate.reserve(NewCapacity);
    }

    void reserveVertexNormals(const size_t NewCapacity)
    {
        mVertexNormal.reserve(NewCapacity);
    }

    void reserveTriangleIndices(const size_t NewCapacity)
    {
        mIndices.reserve(NewCapacity);
    }

    const std::vector<dvec3>& vertexPositions() const { return mVertexPosition; }
    const std::vector<dvec3>& vertexTextureCoordinates() const { return mVertexTextureCoordinate; }
    const std::vector<dvec3>& vertexNormals() const { return mVertexNormal; }
    const std::vector<int>& triangleIndices() const { return mIndices; }

    void drawGeometry(std::shared_ptr<BasicMesh> mesh, std::vector<BasicMesh::Vertex>& vertices) const
    {
        //We show the triangle mesh using a point cloud. Easier to code.
        auto indexBuffer = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);

        //Copy over the vertices.
        const size_t OldNum = vertices.size();
        vertices.reserve(OldNum + mVertexPosition.size());
        indexBuffer->reserve(indexBuffer->getSize() + mVertexPosition.size());
        for(size_t i(0);i<mVertexPosition.size();i++)
        {
            vertices.push_back({mVertexPosition[i], vec3(0, 0, 0), mVertexPosition[i], vec4(0.5, 0.5, 0.5, 1)});
            indexBuffer->add(uint32_t(OldNum + i));
        }
    }

private:
    std::vector<dvec3> mVertexPosition;
    std::vector<dvec3> mVertexTextureCoordinate;
    std::vector<dvec3> mVertexNormal;
    std::vector<int> mIndices;

    BoundingBox BBox;
};

}
