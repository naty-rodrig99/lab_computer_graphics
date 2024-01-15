/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 19, 2017 - 12:14:16
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/constantmaterial.h>

namespace inviwo
{

ConstantMaterial::ConstantMaterial(const dvec3& color)
    : Material(color)
{
}

dvec4 ConstantMaterial::shade(const RayIntersection&, const Light&) const
{
    return dvec4(this->getAmbientMaterialColor(), 1.0);
}

}// namespace inviwo
