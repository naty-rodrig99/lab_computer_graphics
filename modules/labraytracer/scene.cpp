/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:37:19
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */


#include <labraytracer/sphere.h>
#include <labraytracer/scene.h>
#include <labraytracer/util.h>
#include <labraytracer/performancetimer.h>

#ifdef IVW_USE_OPENMP
#include <omp.h>
#endif

namespace inviwo
{

Scene::Scene()
    :backgroundColor(0, 0, 0, 1)
    ,NumIntersections(0)
    ,NumShading(0)
    ,maxDepth(0)
{}

void Scene::init(const ivec2& imageSize)
{
    imageSize_ = imageSize;
    NumIntersections = 0;
    NumShading = 0;
}

void Scene::setCameraProperties(const dvec3& lookFrom, const dvec3& lookTo, const dvec3& lookUp,
                                const double& fov)
{
    camPos_ = lookFrom;

    forward_ = Util::normalize(lookTo - lookFrom);
    right_ = Util::normalize(cross(forward_, lookUp));
    up_ = Util::normalize(cross(right_, forward_));

    right_ = Util::scalarMult(tan(fov * M_PI / 360.0), right_);
    up_ = Util::scalarMult(tan(fov * M_PI / 360.0), up_);

    bottomLeft_ = lookFrom + forward_ - right_ - up_;
    topLeft_ = lookFrom + forward_ - right_ + up_;
    bottomRight_ = lookFrom + forward_ + right_ - up_;
    topRight_ = lookFrom + forward_ + right_ + up_;

    right_ = Util::scalarMult(2.0 / imageSize_.x, right_);
    up_ = Util::scalarMult(2.0 / imageSize_.y, up_);
    bottomLeftPixelCenter_ = bottomLeft_ + 0.5 * (right_ + up_);
}

void Scene::drawCameraGeometry(std::shared_ptr<BasicMesh> mesh,
                               std::vector<BasicMesh::Vertex>& vertices) const
{
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(camPos_, bottomLeft_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
    Util::drawLineSegment(camPos_, topLeft_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
    Util::drawLineSegment(camPos_, bottomRight_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
    Util::drawLineSegment(camPos_, topRight_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);

    Util::drawLineSegment(topLeft_, bottomLeft_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
    Util::drawLineSegment(bottomLeft_, bottomRight_, dvec4(1, 0.5, 0, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(bottomRight_, topRight_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
    Util::drawLineSegment(topRight_, topLeft_, dvec4(1, 0.5, 0, 1), indexBuffer.get(), vertices);
}

void Scene::drawAxesGeometry(std::shared_ptr<BasicMesh> mesh,
                             std::vector<BasicMesh::Vertex>& vertices) const
{
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(dvec3(0, 0, 0), dvec3(1, 0, 0), dvec4(1, 0, 0, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(dvec3(0, 0, 0), dvec3(0, 1, 0), dvec4(0, 1, 0, 1), indexBuffer.get(),
                          vertices);
    Util::drawLineSegment(dvec3(0, 0, 0), dvec3(0, 0, 1), dvec4(0, 0, 1, 1), indexBuffer.get(),
                          vertices);
}

void Scene::drawLightGeometry(std::shared_ptr<BasicMesh> mesh,
                              std::vector<BasicMesh::Vertex>& vertices) const
{
    //Render them as little spheres
    for(auto& L : lights_)
    {
        Sphere S(L->getPosition(), 0.3);
        S.drawGeometryLonLat(mesh, vertices, 10, 8, dvec4(1, 0.9, 0.0, 1));
    }
}

void Scene::drawRenderablesGeometry(std::shared_ptr<BasicMesh> mesh,
                                    std::vector<BasicMesh::Vertex>& vertices) const
{
    for (auto& R : renderables_)
    {
        R->drawGeometry(mesh, vertices);
    }
}


void Scene::prepareScene()
{
    for (auto& R : renderables_)
    {
        //R->updateBoundingBox();
        R->initialize();
        //R->updateTransforms();
    }
}


bool Scene::closestIntersection(const Ray& ray, RayIntersection& intersection,
                                const double maxLambda) const
{
    //Find the closest renderable that this ray hits
    double closestLambda = maxLambda;
    RayIntersection currInter, closestInter;
    bool bHit(false);

    for (auto& R : renderables_)
    {
        if (R->closestIntersection(ray, closestLambda, currInter))
        {
            if (currInter.getLambda() < closestLambda) //sanity check, should be ok from above, but _some_ code may not check
            {
                closestLambda = currInter.getLambda();
                closestInter = currInter;
                bHit = true;
            }
        }
    }

    if (bHit) intersection = closestInter;
    return bHit;
}


bool Scene::anyIntersection(const Ray& ray, const double maxLambda) const
{
    //Faster when you write it in one line...
    for (auto& R : renderables_) if (R->anyIntersection(ray, maxLambda)) return true;

    return false;
}


dvec4 Scene::trace(const Ray& ray, const size_t depth, bool& bIntersectionFound) const
{
    RayIntersection intersection;
    bIntersectionFound = false;

    //If we find an intersection, we gotta throw some color at it, recursively.
    if (closestIntersection(ray, intersection))
    {
        NumIntersections++;
        bIntersectionFound = true;
        return shade(intersection, depth);
    }

    //Set to background color, since no intersection was found.
    return backgroundColor;
}


dvec4 Scene::shade(const RayIntersection& intersection, const size_t depth) const
{
    // This offset must be added to intersection points for further
    // traced rays to avoid noise in the image
    const dvec3 offset = Util::scalarMult(Util::epsilon, intersection.getNormal());
    const dvec3 IntersectionSafePoint = intersection.getPosition() + offset;

    //Shortcuts for the intersection point that we are shading
    std::shared_ptr<const Renderable> renderable = intersection.getRenderable();
    std::shared_ptr<const Material> material = renderable->getMaterial();

    //Run over all lights and add their contributions. Light is linear.
    dvec4 retColor(0, 0, 0, 1);
    for (auto& light : lights_)
    {
        //Shade ambient part, always
        retColor += material->shadeAmbient(intersection, *light);

        //Define the shadow ray from the light to the intersection point.
        const dvec3 L = IntersectionSafePoint - light->getPosition();
        const Ray shadowRay(light->getPosition(), L);

        //Shade diffuse and specular parts only if light is visible from intersection point.
        if (!anyIntersection(shadowRay, length(L)))
        {
            retColor += material->shade(intersection, *light);
            NumShading++;
        }
    }

    //Recursive Raytracing:
    //We are now bouncing off the intersection point
    //into the direction of the reflection vector.
    //We do this up to a certain number of times and only if the material reflects.
    if (depth < maxDepth)
    {
        //Are we even reflective?
        const double t = material->getReflectance();
        if (t > 0)
        {
            //Get out-going viewing direction (reflection direction)
            // - normal
            const dvec3& N(intersection.getNormal());
            // - incident view vector
            const dvec3& I = intersection.getRay().getDirection();
            // - reflect the view vector around the normal
            const dvec3 D = normalize(reflect(I, N));

            // calculate incident radiance by recursive ray tracing
            const Ray r(IntersectionSafePoint, D);
            bool bIntersectionFound;
            dvec4 incident_radiance = trace(r, depth + 1, bIntersectionFound);

            //How much of the incident radiance is reflected toward the viewer?
            // - do not mix in much of the background color (we did not find an intersection in those cases)
            if (!bIntersectionFound) incident_radiance /= lightIntensity;
            retColor = retColor * (1.0 - t) + incident_radiance * dvec4(material->getAbsorptionSpectrum(), 1) * t;
            retColor[3] = 1.0;
        }
    }

    return retColor;
}


void Scene::render(LayerRAM* lr, const size_t maxRecursiveDepth) const
{
    //Statistics!
    NumIntersections = 0;
    NumShading = 0;

    //How deep do we go?
    maxDepth = maxRecursiveDepth;

    //Measure performance
    PerformanceTimer Timer;
    int NumThreads(1);

    //For every pixel, raytrace.
    #ifdef IVW_USE_OPENMP
        omp_set_num_threads(std::thread::hardware_concurrency());
        NumThreads = omp_get_max_threads();
        #pragma omp parallel for schedule(dynamic,16)
    #endif
    for(int j=0;j<imageSize_[1];j++)
    {
        for(int i=0;i<imageSize_[0];i++)
        {
            const size2_t P = size2_t(i, j);
            Ray ray = getRay(P);
            bool bIntersectionFound;
            dvec4 pixelcolor = trace(ray, 0, bIntersectionFound);
            //Only apply light intensity correction to non-background pixels
            if (bIntersectionFound) pixelcolor *= lightIntensity;
            pixelcolor[3] = 1.0;
            lr->setFromDVec4(P, pixelcolor);
        }
    }

    LogInfo("Scene raytraced with " << NumIntersections << " intersections and "
                                << NumShading << " shading computations."
                                << " Needed " << Timer.ElapsedTime() << " seconds with "
                                << NumThreads << " parallel threads.");
}

Ray Scene::getRay(size2_t point) const
{
    const dvec3 PixelCenter = bottomLeftPixelCenter_ + double(point.x) * right_ + double(point.y) * up_; 
    return Ray(camPos_, PixelCenter - camPos_);
}




void Scene::addLight(std::shared_ptr<Light> light)
{
    lights_.push_back(light);
}

void Scene::addRenderable(std::shared_ptr<Renderable> renderable)
{
    renderables_.push_back(renderable);
}

void Scene::clear()
{
    renderables_.clear();
    lights_.clear();
}

}// namespace inviwo
