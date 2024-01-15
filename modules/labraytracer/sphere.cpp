/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/sphere.h>
#include <labraytracer/util.h>


namespace inviwo
{

Sphere::Sphere(const dvec3& center, const double& radius)
{
    center_ = center;
    radius_ = radius;
}


bool Sphere::closestIntersection(const Ray& ray, double maxLambda,
                                 RayIntersection& intersection) const
{
    /*
    TODO: implement this method
    Your code should compute the intersection between a ray and a sphere.

    Return false, if there is no intersection.

    If you detect an intersection,
    you return true and fill-in the information in the RayIntersection object:
    if(rayIntersectsSphere)
    {
        intersection = RayIntersection(ray, shared_from_this(), lambda, normal, dvec3(0, 0, 0));
        return true;
    }
    
    Hints :
      Ray origin p_r : ray.getOrigin()
      Ray direction t_r : ray.getDirection()
      Compute the intersection point using ray.pointOnRay(lambda)
    */

    return false;
}


bool Sphere::anyIntersection(const Ray& ray, double maxLambda) const
{
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Sphere::drawGeometryLonLat(std::shared_ptr<BasicMesh> mesh,
                          std::vector<BasicMesh::Vertex>& vertices,
                          const int lon, const int lat, const dvec4& color) const
{
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    for (int i = 0; i < lat - 1; i++)
    {
        float theta1 = float(i * M_PI) / (lat - 1);
        float theta2 = float((i + 1) * M_PI) / (lat - 1);

        for (int j = 0; j < lon - 1; j++)
        {
            float phi1 = float(j * 2 * M_PI) / (lon - 1);
            float phi2 = float((j + 1) * 2 * M_PI) / (lon - 1);

            dvec3 v1 = dvec3(radius_ * sin(theta1) * cos(phi1), radius_ * sin(theta1) * sin(phi1),
                           radius_ * cos(theta1)) +
                      center_;
            dvec3 v2 = dvec3(radius_ * sin(theta2) * cos(phi1), radius_ * sin(theta2) * sin(phi1),
                           radius_ * cos(theta2)) +
                      center_;
            dvec3 v3 = dvec3(radius_ * sin(theta2) * cos(phi2), radius_ * sin(theta2) * sin(phi2),
                           radius_ * cos(theta2)) +
                      center_;

            Util::drawLineSegment(v1, v2, color, indexBuffer.get(), vertices);
            Util::drawLineSegment(v2, v3, color, indexBuffer.get(), vertices);
        }
    }
}

}// namespace inviwo
