/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/triangle.h>
#include <labraytracer/util.h>
#include <memory>


namespace inviwo
{

Triangle::Triangle() {}

Triangle::Triangle(const dvec3& v0, const dvec3& v1, const dvec3& v2, const dvec3& uvw0,
                   const dvec3& uvw1, const dvec3& uvw2)
{
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    mUVW[0] = uvw0;
    mUVW[1] = uvw1;
    mUVW[2] = uvw2;
}


bool Triangle::closestIntersection(const Ray& ray, double maxLambda,
                                   RayIntersection& intersection) const
{
    /*
    TODO: Implement this method
    Your code should compute the intersection between a ray and a triangle.
    You should accept the intersection only in the interval 0 < lambda < maxLambda.

    Return false, if there is no intersection.

    If you detect an intersection,
    you return true and fill-in the information in the RayIntersection object:
    if(rayIntersectsTriangle)
    {
        intersection = RayIntersection(ray, shared_from_this(), lambda, normal, dvec3(0, 0, 0));
        return true;
    }
    
    Hints:
      Ray origin p_r : ray.getOrigin()
      Ray direction t_r : ray.getDirection()
      Compute the intersection point using ray.pointOnRay(lambda)
    */

    return false;
}


bool Triangle::anyIntersection(const Ray& ray, double maxLambda) const
{
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Triangle::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const
{
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(mVertices[0], mVertices[1], dvec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[1], mVertices[2], dvec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(mVertices[2], mVertices[0], dvec4(0.2, 0.2, 0.2, 1), indexBuffer.get(),
                          vertices);
}

}// namespace inviwo
