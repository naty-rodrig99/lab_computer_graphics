/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Friday, October 13, 2017 - 16:05:29
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/raytracer.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/camera/perspectivecamera.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo Raytracer::processorInfo_
{
    "org.inviwo.Raytracer", // Class identifier
    "Raytracer",            // Display name
    "KTH Labs",               // Category
    CodeState::Experimental,// Code state
    Tags::None,             // Tags
};

const ProcessorInfo Raytracer::getProcessorInfo() const
{
    return processorInfo_;
}

Raytracer::Raytracer()
    :Processor()
    ,triangleInput_("triangleInput")
    ,lightInput_("lightInput")
    ,image_("image")
    ,sceneGeometry_("sg")
    ,camera_("camera", "Camera")
    ,sceneSelection_("scene", "Scene",
                    {{"sceneIntersections", "Intersections", SceneCreationMethod::Intersections}
                    ,{"sceneIllumination", "Illumination", SceneCreationMethod::Illumination}
                    ,{"scenePhongTest", "Phong Test", SceneCreationMethod::PhongTest}
                    ,{"scenePrettySpheres", "Pretty Spheres", SceneCreationMethod::PrettySpheres}
                    ,{"sceneSpikedSphereSubtraction", "Spiked Sphere Subtraction", SceneCreationMethod::SpikedSphereSubtraction}
                    ,{"sceneSphereSubtraction", "Sphere Subtraction", SceneCreationMethod::SphereSubtraction}
                    ,{"sceneSeeThroughSphereSubtraction", "See Through Sphere Subtraction", SceneCreationMethod::SeeThroughSphereSubtraction}
                    ,{"sceneInputMesh", "Input Mesh", SceneCreationMethod::InputMesh}},
                    0)
    ,backgroundColor_("backgroundColor", "Background", dvec3(0, 0, 0), dvec3(0), dvec3(1),
                   dvec3(.1), InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    ,ambientLight_("ambientlight", "Ambient Light", dvec3(0.8, 0.65, 0.6), dvec3(0), dvec3(1),
                   dvec3(.1), InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    ,diffuseLight_("diffuseLight", "Diffuse Light", dvec3(0.8, 0.65, 0.6), dvec3(0), dvec3(1),
                   dvec3(.1), InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    ,specularLight_("specularLight", "Specular Light", dvec3(0.8, 0.65, 0.6), dvec3(0), dvec3(1),
                    dvec3(.1), InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    ,inputMeshColor_("inputMeshColor", "Mesh Color", dvec3(0.5, 0.9, 0.5), dvec3(0), dvec3(1),
                   dvec3(.1), InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    ,useSpecificSeedPrettySpheres_("useSpecificSeedPrettySpheres", "Use Seed", false)
    ,seedPrettySpheres_("seedPrettySpheres", "Seed", 0, INT_MIN, INT_MAX, 1,
                    InvalidationLevel::InvalidOutput, PropertySemantics::Text)
    ,imageSize_("imSize", "Image Size", ivec2(100, 100), ivec2(10, 10), ivec2(1000, 1000), ivec2(1, 1))
    ,adaptiveAntiAliasing_("adaptiveAntiAliasing", "Adaptive Anti-Aliasing")
    ,lightIntensity_("lightIntensity", "Brightness", 100, 0, 255, 1)
    ,maxRecursiveDepth_("maxRecursiveDepth", "Depth", 1, 0, 3)
    ,render_("render", "Render")
{
    triangleInput_.setOptional(true);
    addPort(triangleInput_);
    lightInput_.setOptional(true);
    addPort(lightInput_);
    addPort(image_);
    addPort(sceneGeometry_);

    camera_.setLookFrom(dvec3(5, 0, 5))
           .setLookTo(dvec3(0, 0, 0))
           .setLookUp(dvec3(0, 0, 1))
           .setCollapsed(true);
    camera_.getPropertyByIdentifier("cameraType")->setReadOnly(true);
    camera_.getPropertyByIdentifier("aspectRatio")->setReadOnly(true);
    camera_.setCurrentStateAsDefault();
    addProperty(camera_);

    addProperty(sceneSelection_);

    addProperty(imageSize_);
    addProperty(backgroundColor_);
    addProperty(ambientLight_);
    addProperty(diffuseLight_);
    addProperty(specularLight_);

    addProperty(inputMeshColor_);
    addProperty(useSpecificSeedPrettySpheres_);
    addProperty(seedPrettySpheres_);

    addProperty(adaptiveAntiAliasing_);
    addProperty(lightIntensity_);
    addProperty(maxRecursiveDepth_);

    render_.onChange([&]() { render(); });
    addProperty(render_);

    sceneSelection_.onChange([&]() { showProperties(); });
    lightInput_.onConnect([&]() { showProperties(); });
    lightInput_.onDisconnect([&]() { showProperties(); });
    useSpecificSeedPrettySpheres_.onChange([&]() { showProperties(); });
    showProperties();
}

void Raytracer::showProperties()
{
    const bool bFirstScene = (sceneSelection_.get() == SceneCreationMethod::Intersections);
    const bool bPrettySpheres = (sceneSelection_.get() == SceneCreationMethod::PrettySpheres);
    const bool bUseSeed = useSpecificSeedPrettySpheres_.get();
    const bool bHaveInLights = lightInput_.hasData();
    const bool bHaveInTriangles = triangleInput_.hasData();

    ambientLight_.setVisible(!bFirstScene && !bHaveInLights);
    diffuseLight_.setVisible(!bFirstScene && !bHaveInLights);
    specularLight_.setVisible(!bFirstScene && !bHaveInLights);
    inputMeshColor_.setVisible(bHaveInTriangles);
    useSpecificSeedPrettySpheres_.setVisible(bPrettySpheres);
    seedPrettySpheres_.setVisible(bPrettySpheres);
    seedPrettySpheres_.setReadOnly(!bUseSeed); 
}

void Raytracer::makeAScene()
{
    //Make a scene
    scene_.clear();
    switch (sceneSelection_.get())
    {
        case SceneCreationMethod::Intersections:
            sceneIntersections();
            break;

        case SceneCreationMethod::Illumination:
            sceneIllumination();
            break;

        case SceneCreationMethod::PhongTest:
            scenePhongTest();
            break;

        case SceneCreationMethod::SpikedSphereSubtraction:
            sceneSpikedSphereSubtraction();
            break;

        case SceneCreationMethod::PrettySpheres:
            scenePrettySpheres();
            break;

        case SceneCreationMethod::SphereSubtraction:
            sceneSphereSubtraction();
            break;

        case SceneCreationMethod::SeeThroughSphereSubtraction:
            sceneSeeThroughSphereSubtraction();
            break;

        case SceneCreationMethod::InputMesh:
            sceneInputMesh();
            break;

        default:
            break;
    }
}


void Raytracer::process()
{
    auto PCam = dynamic_cast<PerspectiveCamera*>(&camera_.get());
    if (!PCam) return;

    //Init scene; avoid regenerating when unimportant changes have been made.
    if (sceneSelection_.isModified() || lightInput_.isChanged() || triangleInput_.isChanged()
        || ambientLight_.isModified() || diffuseLight_.isModified() || specularLight_.isModified()
        || inputMeshColor_.isModified()
        || useSpecificSeedPrettySpheres_.isModified() || seedPrettySpheres_.isModified())
    {
        makeAScene();
    }
    // - image size, camera, and some other params always get updated
    scene_.init(imageSize_.get());
    scene_.setCameraProperties(PCam->getLookFrom(), PCam->getLookTo(), PCam->getLookUp(), PCam->getFovy());
    scene_.backgroundColor = dvec4(backgroundColor_.get(), 1);
    scene_.lightIntensity = lightIntensity_.get();
    scene_.useAdaptiveAntiAliasing = adaptiveAntiAliasing_.get();

    //Create a representation of the scene to be rendered interactively outside of the raytracer
    auto mesh = std::make_shared<BasicMesh>();
    std::vector<BasicMesh::Vertex> vertices;
    // - draw stuff
    scene_.drawCameraGeometry(mesh, vertices);
    scene_.drawAxesGeometry(mesh, vertices);
    scene_.drawLightGeometry(mesh, vertices);
    scene_.drawRenderablesGeometry(mesh, vertices);
    // - finalize
    mesh->addVertices(vertices);
    sceneGeometry_.setData(mesh);
}


void Raytracer::render()
{
    auto outImage = std::make_shared<Image>(imageSize_.get(), DataVec4Float32::get());
    auto outLayer = outImage->getColorLayer();
    auto lr = outLayer->getEditableRepresentation<LayerRAM>();

    scene_.prepareScene();
    scene_.render(lr, maxRecursiveDepth_.get());

    image_.setData(outImage);
}


}// namespace inviwo
