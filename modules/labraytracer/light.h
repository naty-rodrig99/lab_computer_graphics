/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:37:00
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo
{

/** \class Light
    \brief Light defined by position and light colors

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API Light
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Light(const dvec3& position = dvec3(0, 0, 0), const dvec3& mAmbientColor = dvec3(1, 1, 1),
          const dvec3& mDiffuseColor = dvec3(1, 1, 1), const dvec3& mSpecularColor = dvec3(1, 1, 1));
    virtual ~Light() = default;

//Methods
public:
    const dvec3& getPosition() const { return mPosition; }
    const dvec3& getAmbientColor() const { return mAmbientColor; }
    const dvec3& getDiffuseColor() const { return mDiffuseColor; }
    const dvec3& getSpecularColor() const { return mSpecularColor; }

//Attributes
private:
    dvec3 mPosition;
    dvec3 mAmbientColor;
    dvec3 mDiffuseColor;
    dvec3 mSpecularColor;
};

}// namespace inviwo
