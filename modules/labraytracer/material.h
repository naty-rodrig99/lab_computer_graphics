/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:25:10
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/rayintersection.h>
#include <labraytracer/light.h>

namespace inviwo
{
class RayIntersection;
class Light;

/** \class Material
    \brief Material defined by color and reflectence

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API Material
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Material(const dvec3& ambientColor = dvec3(0.5, 0.5, 0.5), double reflectance = 0.0)
        : ambientMaterialColor(ambientColor)
        , mReflectance(reflectance)
    {
    }

    virtual ~Material() = default;

//Methods
public:
    const dvec3& getAmbientMaterialColor() const { return ambientMaterialColor; }
    const double getReflectance() const { return mReflectance; }
    virtual dvec3 getAbsorptionSpectrum() const { return ambientMaterialColor; }
    
    virtual dvec4 shade(const RayIntersection& intersection, const Light& light) const = 0;
    virtual dvec4 shadeAmbient(const RayIntersection& intersection, const Light& light) const;

//Attributes
protected:
    dvec3 ambientMaterialColor;
    double mReflectance;
};

}// namespace inviwo
