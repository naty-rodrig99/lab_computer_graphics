/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Friday, October 20, 2017 - 12:20:21
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo
{

/** \class Util
    \brief Utilities for raytracing

    @author Himangshu Saikia
*/
class IVW_MODULE_LABRAYTRACER_API Util
{
    //Friends
    //Types
public:
    //Construction / Deconstruction
public:
    Util();
    virtual ~Util() = default;

    //Methods
public:
    static dvec3 normalize(const dvec3& v);
    static dvec3 scalarMult(const double& s, const dvec3& v);
    static void drawLineSegment(const dvec3& v1, const dvec3& v2, const dvec4& color,
                                IndexBufferRAM* indexBuffer,
                                std::vector<BasicMesh::Vertex>& vertices);
    //Attributes
public:
    static const double epsilon;
};

}// namespace inviwo
