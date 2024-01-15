/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Wednesday, February 22, 2023 - 20:57:11
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

/** \class CookTorranceMaterial
    \brief Material that can be lit with the Cook-Torrance illumination model

    Strongly inspired by https://github.com/rokuz/rt by Roman Kuznetsov aka rokuz

    @author Tino Weinkauf
*/
class IVW_MODULE_LABRAYTRACER_API CookTorranceMaterial : public Material
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    CookTorranceMaterial(const dvec3& albedo, const double& roughness, const double& refraction,
                         const dvec3& ambientColor, const double& reflection);
    virtual ~CookTorranceMaterial() = default;

//Methods
public:
    virtual dvec3 getAbsorptionSpectrum() const override;
    dvec4 shade(const RayIntersection& intersection, const Light& light) const override;
    dvec4 shadeAmbient(const RayIntersection& intersection, const Light& light) const override;

//Attributes
protected:
    dvec3 albedo_;
    double roughness_;
    double refraction_;
};

}// namespace inviwo
