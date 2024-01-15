/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 19, 2017 - 12:14:16
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

/** \class ConstantMaterial
    \brief Material with constant color 
    (shaded according to the color irrespective of lighting)

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API ConstantMaterial : public Material
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    ConstantMaterial(const dvec3& color = dvec3(0, 0.4, 0.8));
    virtual ~ConstantMaterial() = default;

//Methods
public:
    dvec4 shade(const RayIntersection& intersection, const Light& light) const override;
    dvec4 shadeAmbient(const RayIntersection&, const Light&) const override
    {return dvec4(0);}

//Attributes
public:
};

}// namespace inviwo
