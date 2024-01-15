/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 24, 2017 - 17:17:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/material.h>

namespace inviwo
{

/** \class PhongMaterial
    \brief Material that can be lit with the Phong illumination model

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API PhongMaterial : public Material
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    PhongMaterial(const dvec3& ambientColor = dvec3(0, 0.4, 0.8),
                  const dvec3& diffuseColor = dvec3(0, 0.4, 0.8),
                  const dvec3& specularColor = dvec3(0, 0.4, 0.8),
                  const double reflectance = 0.5, const double shininess = 10.0);
    virtual ~PhongMaterial() = default;

//Methods
public:
    virtual dvec3 getAbsorptionSpectrum() const override;

    dvec4 shade(const RayIntersection& intersection, const Light& light) const override;

    dvec3 getDiffuseMaterialColor() const
    {return diffuseMaterialColor;}

    dvec3 getSpecularMaterialColor() const
    {return specularMaterialColor;}

    double getShininess() const
    {return shininessExponent;}

//Attributes
protected:
    double shininessExponent;
    //dvec3 ambientMaterialColor; inherited from Material
    dvec3 diffuseMaterialColor;
    dvec3 specularMaterialColor;
};

}// namespace inviwo
