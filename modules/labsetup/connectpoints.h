/*********************************************************************
 *  Author  : Tino Weinkauf, Jiahui Liu
 *  Init    : Monday, August 27, 2018 - 20:35:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

//#define STUDENT_VIEW 1


#include <modules/labsetup/labsetupmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/meshport.h>
//#include <inviwo/core/ports/volumeport.h>
//#include <inviwo/core/ports/meshport.h>
//#include <inviwo/core/properties/boolcompositeproperty.h>
#include <inviwo/core/properties/boolproperty.h>
//#include <inviwo/core/properties/buttonproperty.h>
//#include <inviwo/core/properties/compositeproperty.h>
//#include <inviwo/core/properties/fileproperty.h>
//#include <inviwo/core/properties/minmaxproperty.h>
//#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
//#include <inviwo/core/properties/stringproperty.h>
//#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo
{
namespace kth
{

/** Processor to explain basic Inviwo programming.

    @author Tino Weinkauf
*/
class IVW_MODULE_LABSETUP_API ConnectPoints : public Processor
{
// Friends
// Types
public:
// Construction / Deconstruction
public:
    ConnectPoints();
    virtual ~ConnectPoints() = default;

// Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    /// Our main computation function
    virtual void process() override;

    /// Draws a bounding box
    void BoundingBox(const std::vector<vec3>& Points, IndexBufferRAM* OutIndices,
                     Vec3BufferRAM* OutVertices);

// Ports
public:
    /// Line input
    MeshMultiInport portInLines;

    /// Line output
    MeshOutport portOutLines;

// Properties
public:
#ifndef STUDENT_VIEW
    BoolProperty propCheckMark;
#endif

// Attributes
private:
};

}// namespace kth
}// namespace inviwo
