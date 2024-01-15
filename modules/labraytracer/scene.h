/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:37:19
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/ray.h>
#include <labraytracer/light.h>
#include <labraytracer/renderable.h>
#include <inviwo/core/datastructures/image/layerram.h>

namespace inviwo
{

/** \class Scene
    \brief Scene with a number of renderable objects (e.g. Spheres, Triangles), lights
    and properties of the camera.

    @author Himangshu Saikia
    @author Tino Weinkauf
*/
class IVW_MODULE_LABRAYTRACER_API Scene
{
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Scene();
    virtual ~Scene() = default;

//Methods
public:
    void init(const ivec2& imageSize);
    void setCameraProperties(const dvec3& lookFrom, const dvec3& lookTo, const dvec3& lookUp,
                             const double& fov);
    void drawCameraGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const;
    void drawAxesGeometry(std::shared_ptr<BasicMesh> mesh,
                          std::vector<BasicMesh::Vertex>& vertices) const;
    void drawLightGeometry(std::shared_ptr<BasicMesh> mesh,
                           std::vector<BasicMesh::Vertex>& vertices) const;
    void drawRenderablesGeometry(std::shared_ptr<BasicMesh> mesh,
                                 std::vector<BasicMesh::Vertex>& vertices) const;

    ///Computes the closest intersection of a ray and any object in scene.
    bool closestIntersection(const Ray& ray, RayIntersection& intersection,
                             const double maxLambda = std::numeric_limits<double>::infinity()) const;

    ///Checks for any intersection along the ray. Good for shadow rays.
    bool anyIntersection(const Ray& ray,
                         const double maxLambda = std::numeric_limits<double>::infinity()) const;

    ///Iterates over all pixels and shoots rays, recursively up to MaxRecursiveDepth levels.
    void render(LayerRAM* lr, const size_t maxRecursiveDepth) const;

    //Traces a ray. Used for recursive raytracing.
    dvec4 trace(const Ray& ray, const size_t depth, bool& bIntersectionFound) const;

    //Computes shading color for a ray. Used for recursive raytracing.
    dvec4 shade(const RayIntersection& intersection, const size_t depth) const;

    void prepareScene();
    Ray getRay(size2_t point) const;

    void addLight(std::shared_ptr<Light> light);
    void addRenderable(std::shared_ptr<Renderable> renderable);
    void clear();

//Attributes
public:
    ///Background color
    dvec4 backgroundColor;

    ///Overall light intensity.
    /// It helps to brighten or darken the image
    /// without needing to change the light balance in the scene.
    double lightIntensity;

    ///Whether to use adaptive anti-aliasing during rendering
    bool useAdaptiveAntiAliasing;

private:
    ///Counts number of intersections
    mutable long NumIntersections;

    ///Counts number of shading computations
    mutable long NumShading;

    ///Maximum depth for recursive raytracing
    mutable size_t maxDepth;

    dvec3 bottomLeft_;
    dvec3 topLeft_;
    dvec3 bottomRight_;
    dvec3 topRight_;
    dvec3 forward_;
    dvec3 up_;
    dvec3 right_;
    dvec3 bottomLeftPixelCenter_;
    dvec3 camPos_;
    ivec2 imageSize_;
    std::vector<std::shared_ptr<Light>> lights_;
    std::vector<std::shared_ptr<Renderable>> renderables_;
};

}// namespace inviwo
