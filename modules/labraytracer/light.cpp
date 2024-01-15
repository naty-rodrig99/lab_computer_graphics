/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:37:00
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/light.h>

namespace inviwo
{

Light::Light(const dvec3& position, const dvec3& ambientColor, const dvec3& diffuseColor,
             const dvec3& specularColor)
{
    mPosition = position;
    mAmbientColor = ambientColor;
    mDiffuseColor = diffuseColor;
    mSpecularColor = specularColor;
}

}// namespace inviwo
