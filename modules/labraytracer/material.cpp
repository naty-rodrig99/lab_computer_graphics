/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:25:10
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/material.h>

namespace inviwo
{

dvec4 Material::shadeAmbient(const RayIntersection& intersection, const Light& light) const
{
    const double dist2 = glm::length2(light.getPosition() - intersection.getPosition());
    const dvec3 c_a = light.getAmbientColor() / dist2;
    const dvec3 c_ambient = ambientMaterialColor * c_a;

    return dvec4(c_ambient, 1.0);
}

}// namespace inviwo
