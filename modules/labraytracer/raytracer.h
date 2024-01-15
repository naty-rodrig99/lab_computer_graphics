/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Friday, October 13, 2017 - 16:05:29
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/datastructures/light/baselightsource.h>

#include <labraytracer/light.h>
#include <labraytracer/renderable.h>
#include <labraytracer/scene.h>

namespace inviwo
{

/** \docpage{org.inviwo.Raytracer, Raytracer}
    ![](org.inviwo.Raytracer.png?classIdentifier=org.inviwo.Raytracer)

    CPU Raytracer. The rendering has to be explicitly triggered with the Render button.

    ### Outports
      * __<imageOutport>__ Rendered image.
      * __<meshOutport>__ Geometry of the scene with wireframes for the camera and the
            objects of the scene

    ### Properties
      * __<Render Button>__ Button triggering raytracing.
      * __<Image Size>__ Size of the rendering image (one Ray per pixel)
*/

/** \class Raytracer
    \brief A CPU Raytracer

    A CPU Raytracer.

    @author Himangshu Saikia
    @author Tino Weinkauf
    @author The Visualization Group at Magdeburg University
*/
class IVW_MODULE_LABRAYTRACER_API Raytracer : public Processor
{
    // Friends
    // Types
public:
    // Construction / Deconstruction
public:
    Raytracer();
    virtual ~Raytracer() = default;

    // Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    /// Our main computation function
    virtual void process() override;
    void render();
    void makeAScene();
    void showProperties();

    /* Functions creating different scenes. See raytracer_scenes.cpp for their implementation. */
    void sceneIntersections();
    void sceneIllumination();
    void scenePrettySpheres();
    void sceneInputMesh();
    void scenePhongTest();
    void sceneSpikedSphereSubtraction();
    void sceneSphereSubtraction();
    void sceneSeeThroughSphereSubtraction();
    void addDefaultLights();
    void addInputLights();
    
// Ports
public:
    MeshMultiInport triangleInput_;
    DataInport<LightSource, 0> lightInput_;

    ImageOutport image_;
    MeshOutport sceneGeometry_;

// Properties
public:
    CameraProperty camera_;

    enum class SceneCreationMethod
    {
        Intersections,
        Illumination,
        PhongTest,
        PrettySpheres,
        SpikedSphereSubtraction,
        SphereSubtraction,
        SeeThroughSphereSubtraction,
        InputMesh
    };
    TemplateOptionProperty<SceneCreationMethod> sceneSelection_;

    DoubleVec3Property backgroundColor_;

    DoubleVec3Property ambientLight_;
    DoubleVec3Property diffuseLight_;
    DoubleVec3Property specularLight_;

    DoubleVec3Property inputMeshColor_;

    BoolProperty useSpecificSeedPrettySpheres_;
    IntProperty seedPrettySpheres_;

    IntVec2Property imageSize_;
    BoolProperty adaptiveAntiAliasing_;
    DoubleProperty lightIntensity_;
    IntSizeTProperty maxRecursiveDepth_;
    ButtonProperty render_;

// Attributes
private:
    Scene scene_;
};

}// namespace inviwo
