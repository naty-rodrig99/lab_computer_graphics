/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/renderable.h>

namespace inviwo
{

/** \class Sphere
    \brief Sphere defined by a center point and a radius.

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API Sphere : public Renderable
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Sphere(const dvec3& center = dvec3(0, 0, 0), const double& radius = 1.0);
    virtual ~Sphere() = default;

//Methods
public:
    bool closestIntersection(const Ray& ray, double maxLambda,
                             RayIntersection& intersection) const override;

    bool anyIntersection(const Ray& ray, double maxLambda) const override;

    void drawGeometry(std::shared_ptr<BasicMesh> mesh,
                      std::vector<BasicMesh::Vertex>& vertices) const override
    {
        drawGeometryLonLat(mesh, vertices, 10, 8, dvec4(0.2, 0.2, 0.2, 1));
    }

    void drawGeometryLonLat(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices,
                            const int lon, const int lat, const dvec4& color) const;

//Attributes
public:
    dvec3 center_;
    double radius_;
};

}// namespace inviwo
