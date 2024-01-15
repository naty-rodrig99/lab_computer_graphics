/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Friday, January 20, 2023 - 11:11:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once


#include <labgrids/labgridsmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

#include <vector>
using namespace std;
using namespace glm;

namespace inviwo
{

/** \docpage{org.inviwo.UniformBinning, Cube Animator}
    ![](org.inviwo.UniformBinning.png?classIdentifier=org.inviwo.UniformBinning)

    Processor to bin a list of points via a 2D uniform grid
    and show the number of points per grid cell.

    ### Inports
      * __InPoints__ Input mesh with points.

    ### Outports
      * __OutMesh__ Output mesh, result of binning.
      * __OutPoints__ A copy of the input points, but flattened into z=0, i.e., how they have been processed.

    ### Properties
      * __GridSize__ Number of vertices of the 2D uniform grid in either direction.
*/


/** \class UniformBinning
    \brief 2D Binning with Uniform Grid

    @author Tino Weinkauf
*/
class IVW_MODULE_LABGRIDS_API UniformBinning : public Processor
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    UniformBinning();
    virtual ~UniformBinning() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;

    void DoUniformBinning(const int nx, const int ny, const vector<vec2>& Points,
                          vector<vec2>& UniformGridVertices, vector<int>& Counts);

    ///Some tests to see whether the results of DoUniformBinning() are correct.
    ///@returns false, if one cannot savely work with the results.
    bool CheckResults(const int nx, const int ny, const int NumPoints,
                      const vector<vec2>& UniformGridVertices, const vector<int>& Counts);

    ///Visualize the point counts as skyscrapers
    shared_ptr<Mesh> BuildSkyscrapers(const bool bShowCounts, const int nx, const int ny,
                                      const float HeightFactor, const TransferFunction& TF,
                                      const vector<vec2>& UniformGridVertices, const vector<int>& Counts);

//Ports
public:
    ///Input points
    MeshMultiInport portInPoints;

    ///Output mesh with bins
    MeshOutport portOutMesh;

    ///Output mesh with flattened input points
    MeshOutport portOutPoints;

//Properties
public:
    ///Number of vertices of the 2D uniform grid in either direction.
    IntSize2Property propGridSize;

    ///Whether to show the counts, or just the uniform grid
    BoolProperty propShowCounts;

    ///Factor for the height
    FloatProperty propHeightFactor;

    ///Color for the height
    TransferFunctionProperty propColor;

//Attributes
private:
    ///Whether we announced x/y-fastest already
    bool bAnnouncedXYFastest;
};

} // namespace
