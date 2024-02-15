/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 12, 2017 - 11:11:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labtransformations/cubeanimator.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo CubeAnimator::processorInfo_
{
    "org.inviwo.CubeAnimator",      // Class identifier
    "Cube Animator",                // Display name
    "KTH Labs",                 // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo CubeAnimator::getProcessorInfo() const
{
    return processorInfo_;
}


CubeAnimator::CubeAnimator()
    :Processor()
    // Ports
    , meshIn_("meshIn")
    , meshOut_("meshOut")
    // Properties 
    // For a FloatProperty 
    // variablename(identifier, display name, init value, minvalue, maxvalue)
    , radius_("radius", "Radius", 0, 0, 4)
    , rotate_("angle", "Angle", 0, 0, 180)
    , scale_("scale", "Scale", 1, 0, 3)
{
    // Add ports
    addPort(meshIn_);
    addPort(meshOut_);
    
    // Add properties
    addProperty(radius_);
    addProperty(rotate_);
    addProperty(scale_);
}


void CubeAnimator::process()
{
    // Clone the input mesh
    if (!meshIn_.getData()) return;
    auto mesh = meshIn_.getData()->clone();

    // Get the matrix that defines where the mesh is currently
    auto matrix = mesh->getWorldMatrix();

    /** Transform the mesh (TODO)
    * 
    * You create a translation matrix using:
    *     glm::translate(vec3(x, y, z))
    * where x, y, z are the components of the translation vector.
    * 
    * You create a rotation matrix using:
    *     glm::rotate(alpha, vec3(x, y, z))
    * where alpha is the angle of rotation,
    * and x, y, z are the components of the rotation axis.
    * 
    * You create a scaling matrix using:
    *     glm::scale(vec3(sx, sy, sz))
    * where sx, sy, sz are the scaling factors in x, y, z direction.
    * 
    */

    //translates the matrix by a certain distance along the z-axis, where the distance is determined by radius_.get()
    matrix = glm::translate(vec3(0, 0, - radius_.get())) * matrix;
    //rotates the matrix around the z-axis by an angle obtained from rotate_.get()
    matrix = glm::rotate(rotate_.get(), vec3(0, 0, 1)) * matrix; 
    //scales the matrix uniformly along all three axes by a scaling factor obtained from scale_.get()
    matrix = glm::scale(vec3(scale_.get(), scale_.get(), scale_.get())) * matrix;

    // Update
    mesh->setWorldMatrix(matrix);
	
    // Set output
    meshOut_.setData(mesh);
}

} // namespace

