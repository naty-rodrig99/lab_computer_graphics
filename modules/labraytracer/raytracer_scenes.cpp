/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Monday, February 27, 2023 - 16:05:29
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */




#include <labraytracer/util.h>
#include <labraytracer/triangle.h>
#include <labraytracer/sphere.h>
#include <labraytracer/plane.h>
#include <labraytracer/bvhindexedtrianglemesh.h>
#include <labraytracer/constantmaterial.h>
#include <labraytracer/phongmaterial.h>
#include <labraytracer/cooktorrancematerial.h>
#include <modules/base/algorithm/meshutils.h>
#include <inviwo/core/datastructures/light/pointlight.h>

#include <labraytracer/raytracer.h>

#include <random>
#include <set>

namespace inviwo
{

void Raytracer::sceneIntersections()
{
    std::shared_ptr<Light> light1 =
        std::make_shared<Light>(dvec3(5, 2, 6), dvec3(0), dvec3(0), dvec3(0));
    scene_.addLight(light1);

    dvec3 a(2, 1, 0);
    dvec3 b(1, 2, 0);
    dvec3 c(1, 0, 2);

    std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(a, b, c);

    std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(dvec3(-5, -2, 1), 1);
    std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(dvec3(0, 1, 1), 1.5);
    std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(dvec3(-2, 0, 2), 1);

    std::shared_ptr<PlaneX> plane = std::make_shared<PlaneX>();

    std::shared_ptr<Material> materialTriangle =
        std::make_shared<ConstantMaterial>(dvec3(0.8, 0.0, 0.1));
    std::shared_ptr<Material> materialSpheres =
        std::make_shared<ConstantMaterial>(dvec3(1.0, 0.4, 0.1));
    std::shared_ptr<Material> materialPlane =
        std::make_shared<ConstantMaterial>(dvec3(0.0, 0.2, 0.7));

    triangle->setMaterial(materialTriangle);
    sphere1->setMaterial(materialSpheres);
    sphere2->setMaterial(materialSpheres);
    sphere3->setMaterial(materialSpheres);
    plane->setMaterial(materialPlane);

    scene_.addRenderable(triangle);
    scene_.addRenderable(sphere1);
    scene_.addRenderable(sphere2);
    scene_.addRenderable(sphere3);
    scene_.addRenderable(plane);
}

void Raytracer::sceneIllumination()
{
    std::shared_ptr<Material> material1 = std::make_shared<PhongMaterial>(
        dvec3(1.0, 0.4, 0.1), dvec3(1.0, 0.4, 0.1), dvec3(1.0, 0.4, 0.1), 0.8, 1000.0);
    std::shared_ptr<Material> material2 = std::make_shared<PhongMaterial>(
        dvec3(0.0, 0.0, 0.0), dvec3(0.2, 0.2, 0.2), dvec3(1.0, 1.0, 1.0), 0.2, 1000.0);
    std::shared_ptr<Material> material3 = std::make_shared<PhongMaterial>(
        dvec3(0.2, 0.3, 0.8), dvec3(0.2, 0.3, 0.8), dvec3(0.2, 0.3, 0.8), 0.1, 10.0);
    std::shared_ptr<Material> material4 = std::make_shared<PhongMaterial>(
        dvec3(0.5, 0.0, 0.0), dvec3(0.5, 0.0, 0.0), dvec3(0.5, 0.0, 0.0), 0.2, 50.0);
    std::shared_ptr<Material> material5 = std::make_shared<PhongMaterial>(
        dvec3(0.5, 0.5, 0.5), dvec3(0.5, 0.5, 0.5), dvec3(0.5, 0.5, 0.5), 0.1, 100.0);

    std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(dvec3(1.1, 1.1, 1.1));
    std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(dvec3(-1.1, 1.1, 1.1));
    std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(dvec3(0.0, -1.1, 1.1));
    std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(dvec3(0.0, 0.0, 2.0));

    std::shared_ptr<PlaneX> plane = std::make_shared<PlaneX>();

    sphere1->setMaterial(material1);
    sphere2->setMaterial(material2);
    sphere3->setMaterial(material3);
    sphere4->setMaterial(material4);
    plane->setMaterial(material5);

    scene_.addRenderable(sphere1);
    scene_.addRenderable(sphere2);
    scene_.addRenderable(sphere3);
    scene_.addRenderable(sphere4);
    scene_.addRenderable(plane);

    addInputLights();
}


void Raytracer::scenePhongTest()
{
    for(int i(0);i<4;i++)
    {
        const double shininess = pow(10, i);
        auto mat = std::make_shared<PhongMaterial>(dvec3(0.5, 0.5, 1), dvec3(0.5, 0.5, 1), dvec3(0.5, 0.5, 1),
                                                   0, shininess);
        auto S = std::make_shared<Sphere>(dvec3(i%2, floor(i/2), 0), 0.45);
        S->setMaterial(mat);
        scene_.addRenderable(S);
    }

    addInputLights();
}


///From https://github.com/rokuz/rt by Roman Kuznetsov aka rokuz
class Palette
{
public:
  static std::vector<uint32_t> const & Blue()
  {
    static std::vector<uint32_t> kColors = {0x011f4b, 0x03396c, 0x005b96, 0x6497b1, 0xb3cde0,
                                            0x2a4d69, 0x4b86b4, 0xadcbe3, 0xe7eff6, 0x63ace5,
                                            0x6e7f80, 0x536872, 0x708090, 0x536878, 0x36454f,
                                            0x3385c6, 0x4279a3, 0x476c8a, 0x49657b, 0x7f8e9e};
    return kColors;
  }

  static std::vector<uint32_t> const & Warm()
  {
    static std::vector<uint32_t> kColors = {0xeee3e7, 0xead5dc, 0xeec9d2, 0xf4b6c2, 0xf6abb6,
                                            0xf6cd61, 0xfe8a71, 0xfe9c8f, 0xfeb2a8, 0xfec8c1,
                                            0xfad9c1, 0xf9caa7, 0xee4035, 0xf37736, 0xfdf498,
                                            0xff77aa, 0xff99cc, 0xffbbee, 0xff5588, 0xff3377};
    return kColors;
  }

  static std::vector<uint32_t> const & Green()
  {
    static std::vector<uint32_t> kColors = {0x0e9aa7, 0x3da4ab, 0x009688, 0x35a79c, 0x54b2a9,
                                            0x65c3ba, 0x83d0c9, 0x7bc043, 0x96ceb4, 0x88d8b0,
                                            0xa8e6cf, 0xdcedc1, 0x00b159, 0xE5FCC2, 0x9DE0AD,
                                            0x45ADA8, 0xC8C8A9, 0x83AF9B, 0xA8E6CE, 0xDCEDC2};
    return kColors;
  }

  static std::vector<uint32_t> const & Yellow()
  {
    static std::vector<uint32_t> kColors = {0xfed766, 0xf6cd61, 0xfdf498, 0xffeead, 0xffcc5c,
                                            0xffa700, 0xffc425, 0xedc951, 0xf1c27d, 0xffdbac};
    return kColors;
  }

  static std::vector<uint32_t> const & Red()
  {
    static std::vector<uint32_t> kColors = {0xfe4a49, 0x851e3e, 0xee4035, 0xff6f69, 0xd62d20,
                                            0xff8b94, 0xd11141, 0xcc2a36, 0xEC2049, 0xFE4365};
    return kColors;
  }

  template<typename Generator>
  static glm::dvec3 Random(Generator & generator, std::vector<uint32_t> const & palette)
  {
    std::uniform_int_distribution<> rnd(0, static_cast<int>(palette.size() - 1));
    return Convert(palette[rnd(generator)]);
  }

  template<typename Generator>
  static glm::dvec3 RandomFromAll(Generator & generator)
  {
    static std::vector<std::vector<uint32_t>> kAll = {Blue(), Warm(), Green(), Yellow(), Red()};

    std::uniform_int_distribution<> rnd(0, static_cast<int>(kAll.size() - 1));
    return Random(generator, kAll[rnd(generator)]);
  }

private:
  static glm::dvec3 Convert(uint32_t color)
  {
    return glm::dvec3(static_cast<uint8_t>(color >> 16 & 0xFF) / 255.0f,
                      static_cast<uint8_t>(color >> 8 & 0xFF) / 255.0f,
                      static_cast<uint8_t>(color & 0xFF) / 255.0f);
  }
};


///From https://github.com/rokuz/rt by Roman Kuznetsov aka rokuz
void Raytracer::scenePrettySpheres()
{
    //m_frame->SetCameraPosition(glm::vec3(7.5f, 1.0f, -7.5f));
    //m_frame->SetCameraDirection(glm::vec3(-2.0f, -0.5f, 2.0f));

    //scene_.backgroundColor = dvec4(0.9, 0.9, 0.9, 1);

    std::random_device random;
    // - seed
    auto seed = random();
    if (useSpecificSeedPrettySpheres_.get())
    {
        seed = seedPrettySpheres_.get();
    }
    else
    {
        seedPrettySpheres_.set(seed);
    }

    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> randomDouble(0.0, 1.0);

    std::set<std::pair<int, int>> s;
    std::uniform_int_distribution<> randomPos(-5, 5);
    auto generatePosition = [this, &s, &randomPos, &generator]()
    {
        std::pair<int, int> p;
        int i = 0;
        do
        {
            p = std::make_pair(randomPos(generator), randomPos(generator));
            ++i;
        } while (s.find(p) != s.end() && i < 5000);
        s.insert(p);
        return p;
    };

    for (size_t i = 0; i < 40; ++i)
    {
        const auto p = generatePosition();
        const auto c = Palette::RandomFromAll(generator);

        const double radius = glm::mix(0.25, 0.5, randomDouble(generator));
        auto S = std::make_shared<Sphere>(dvec3(p.first, p.second, radius), radius);

        //Cook-Torrance kinda works, but the overall light distribution is not balanced well with it.
        //auto mat = std::make_shared<CookTorranceMaterial>(c, glm::mix(0.0, 0.2, randomDouble(generator)),
        //                                                  glm::mix(0.0, 0.2, randomDouble(generator)),
        //                                                  dvec3(0.2), glm::mix(0.0, 0.2, randomDouble(generator)));

        auto mat = std::make_shared<PhongMaterial>(dvec3(0.2, 0.2, 0.2) * c, c, dvec3(0.3, 0.3, 0.2),
                                                   glm::mix(0.0, 1.0, randomDouble(generator)),
                                                   glm::mix(0.1, 100., randomDouble(generator)));
        S->setMaterial(mat);
        scene_.addRenderable(S);
    }

    //Ground plane, slightly blue and reflective
    auto Ground = std::make_shared<PlaneX>();
    //std::shared_ptr<Material> GroundMat = std::make_shared<CookTorranceMaterial>(
    //    dvec3(0.7, 0.7, 1.0), 0, 0, dvec3(1), 0);
    std::shared_ptr<Material> GroundMat = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.3, 0.6), dvec3(0.7, 0.7, 0.9), dvec3(0.5, 0.5, 0.5), 0.25, 10);
    Ground->setMaterial(GroundMat);
    scene_.addRenderable(Ground);

    addInputLights();
}


void Raytracer::sceneSpikedSphereSubtraction()
{

    //Random colors for the small spheres
    std::random_device random;
    std::mt19937 generator(random());

    //The big sphere hosting all the smaller ones
    const double BigRadius(4);
    const dvec3 BigOrigin(0,0,0);
    const dvec3 BigOffset(0,0,4.2);
    auto BigSphere = std::make_shared<Sphere>(BigOrigin + BigOffset, BigRadius);
    auto BigMat = std::make_shared<PhongMaterial>(dvec3(0.5, 0.5, 1), dvec3(0.5, 0.5, 1), dvec3(0.5, 0.5, 1), 0.9, 2);
    BigSphere->setMaterial(BigMat);
    scene_.addRenderable(BigSphere);


    //Regular, equidistant sampling of the sphere
    // Deserno, Markus. "How to generate equidistributed points on the surface of a sphere." If Polymerforshung (Ed.) 99, no. 2 (2004).
    // https://www.cmu.edu/biolphys/deserno/pdf/sphere_equi.pdf
    //
    // an alternative that I did not try: http://extremelearning.com.au/evenly-distributing-points-on-a-sphere/
    const int N = 100;
    //int Ncount = 0;
    const double a = 4.0 * M_PI /** pow(BigRadius, 2)*/ / double(N);
    const double d = sqrt(a);
    const double Mtheta = round(M_PI / d);
    const double dtheta = M_PI / Mtheta;
    const double dphi = a / dtheta;
    for(int m(0);m<int(Mtheta);m++)
    {
        const double theta = M_PI * (double(m) + 0.5) / Mtheta;
        const double Mphi = round(2 * M_PI * sin(theta) / dphi);
        for(int n(0);n<int(Mphi);n++)
        {
            const double phi = 2 * M_PI * double(n) / Mphi;
            const dvec3 SmallOrigin(BigRadius * 1.15 * sin(theta) * cos(phi),
                                    BigRadius * 1.15 * sin(theta) * sin(phi),
                                    BigRadius * 1.15 * cos(theta));

            // TODO:
            // These small spheres should subtract volume.
            //Create small sphere and color it randomly
            auto SmallSphere = std::make_shared<Sphere>(SmallOrigin + BigOffset, 2.5 * dtheta);
            const auto SmallColor = Palette::Random(generator, Palette::Yellow());
            auto SmallMat = std::make_shared<PhongMaterial>(SmallColor, SmallColor, SmallColor, 0, 2);
            SmallSphere->setMaterial(SmallMat);
            scene_.addRenderable(SmallSphere);

            //Ncount++;
        }
    }

    //Ground plane, slightly blue and reflective
    auto Ground = std::make_shared<PlaneX>();
    std::shared_ptr<Material> GroundMat = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.3, 0.6), dvec3(0.7, 0.7, 0.9), dvec3(0.5, 0.5, 0.5), 0.25, 10);
    Ground->setMaterial(GroundMat);
    scene_.addRenderable(Ground);

    addInputLights();
}


void Raytracer::sceneSphereSubtraction()
{
    std::shared_ptr<Material> material1 = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.15, 0.), dvec3(0.9, 0.4, 0.1), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material2 = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.2, 0.2), dvec3(0.9, 0.3, 0.3), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material3 = std::make_shared<PhongMaterial>(
        dvec3(0.2, 0.3, 0.2), dvec3(0.3, 0.9, 0.3), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material4 = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.2, 0.3), dvec3(0.8, 0.3, 0.8), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material5 = std::make_shared<PhongMaterial>(
        dvec3(0.2, 0.2, 0.3), dvec3(0.3, 0.3, 0.9), dvec3(0.3, 0.3, 0.3), 0, 1000);

    //Ground plane, slightly blue and reflective
    auto Ground = std::make_shared<PlaneX>();
    std::shared_ptr<Material> GroundMat = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.3, 0.6), dvec3(0.7, 0.7, 0.9), dvec3(0.5, 0.5, 0.5), 0.25, 10);
    Ground->setMaterial(GroundMat);
    scene_.addRenderable(Ground);

    addInputLights();

    //Normal spheres
    std::shared_ptr<Sphere> normalsphere1 = std::make_shared<Sphere>(dvec3(0, 0, 4), 4);

    // TODO:
    // These spheres should subtract volume.
    std::shared_ptr<Sphere> subsphere1 = std::make_shared<Sphere>(dvec3(4, 0, 4), 4);
    std::shared_ptr<Sphere> subsphere2 = std::make_shared<Sphere>(dvec3(0, 4, 4), 4);
    std::shared_ptr<Sphere> subsphere3 = std::make_shared<Sphere>(dvec3(-4, 0, 4), 4);
    std::shared_ptr<Sphere> subsphere4 = std::make_shared<Sphere>(dvec3(0, -4, 4), 4);
    std::shared_ptr<Sphere> subsphere5 = std::make_shared<Sphere>(dvec3(0, 0, 8), 4);

    normalsphere1->setMaterial(material1);
    subsphere1->setMaterial(material2);
    subsphere2->setMaterial(material3);
    subsphere3->setMaterial(material4);
    subsphere4->setMaterial(material5);
    subsphere5->setMaterial(material2);

    scene_.addRenderable(normalsphere1);
    scene_.addRenderable(subsphere1);
    scene_.addRenderable(subsphere2);
    scene_.addRenderable(subsphere3);
    scene_.addRenderable(subsphere4);
    scene_.addRenderable(subsphere5);
}


void Raytracer::sceneSeeThroughSphereSubtraction()
{
    std::shared_ptr<Material> material1 = std::make_shared<PhongMaterial>(
        dvec3(0.6, 0.25, 0.), dvec3(0.9, 0.4, 0.1), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material2 = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.2, 0.2), dvec3(0.9, 0.3, 0.3), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material3 = std::make_shared<PhongMaterial>(
        dvec3(0.2, 0.3, 0.2), dvec3(0.3, 0.9, 0.3), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material4 = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.2, 0.3), dvec3(0.8, 0.3, 0.8), dvec3(0.3, 0.3, 0.3), 0, 1000);
    std::shared_ptr<Material> material5 = std::make_shared<PhongMaterial>(
        dvec3(0.2, 0.2, 0.3), dvec3(0.3, 0.3, 0.9), dvec3(0.3, 0.3, 0.3), 0, 1000);

    //Ground plane, slightly blue and reflective
    auto Ground = std::make_shared<PlaneX>();
    std::shared_ptr<Material> GroundMat = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.3, 0.6), dvec3(0.7, 0.7, 0.9), dvec3(0.5, 0.5, 0.5), 0.25, 10);
    Ground->setMaterial(GroundMat);
    scene_.addRenderable(Ground);

    addInputLights();

    //Normal spheres
    std::shared_ptr<Sphere> normalsphere1 = std::make_shared<Sphere>(dvec3(0, 0, 4), 4);

    //Params
    const double r = 30;
    const dvec3 o1 = dvec3(r * cos(0), r * sin(0), 4);
    const dvec3 o2 = dvec3(r * cos(2 * M_PI / 3), r * sin(2 * M_PI / 3), 4);
    const dvec3 o3 = dvec3(r * cos(4 * M_PI / 3), r * sin(4 * M_PI / 3), 4);

    // TODO:
    // These spheres should subtract volume.
    std::shared_ptr<Sphere> subsphere1 = std::make_shared<Sphere>(o1, r-2);
    std::shared_ptr<Sphere> subsphere2 = std::make_shared<Sphere>(o2, r-2);
    std::shared_ptr<Sphere> subsphere3 = std::make_shared<Sphere>(o3, r-2);
    std::shared_ptr<Sphere> subsphere4 = std::make_shared<Sphere>(dvec3(0, 0, 4), 3);

    normalsphere1->setMaterial(material1);
    subsphere1->setMaterial(material2);
    subsphere2->setMaterial(material3);
    subsphere3->setMaterial(material4);
    subsphere4->setMaterial(material5);

    scene_.addRenderable(normalsphere1);
    scene_.addRenderable(subsphere1);
    scene_.addRenderable(subsphere2);
    scene_.addRenderable(subsphere3);
    scene_.addRenderable(subsphere4);
}

void Raytracer::sceneInputMesh()
{
    //Material for the triangle inputs
    const dvec3& DesiredTriangleColor = inputMeshColor_.get();
    std::shared_ptr<Material> TriangleMat = std::make_shared<PhongMaterial>(
        DesiredTriangleColor * 0.2, DesiredTriangleColor, dvec3(0.4, 0.4, 0.4), 0.0, 10.0);

    //Ground plane, slightly blue and reflective
    auto Ground = std::make_shared<PlaneX>();
    std::shared_ptr<Material> GroundMat = std::make_shared<PhongMaterial>(
        dvec3(0.3, 0.3, 0.6), dvec3(0.7, 0.7, 0.9), dvec3(0.5, 0.5, 0.5), 0.25, 10);
    Ground->setMaterial(GroundMat);
    scene_.addRenderable(Ground);

    addInputLights();

    //Get the input mesh data
    auto MultiInMeshes = triangleInput_.getVectorData();
    if (MultiInMeshes.empty()) return;

    for(auto InMesh : MultiInMeshes)
    {
        //Vertex data
        auto pit = util::find_if(InMesh->getBuffers(), [](const auto& buf)
        {
            return buf.first.type == BufferType::PositionAttrib;
        });
        if (pit == InMesh->getBuffers().end()) return; //could not find a position buffer
        // - in RAM
        const auto posRam = pit->second->getRepresentation<BufferRAM>();
        if (!posRam) return; //could not find a position buffer ram
        // - 3D
        if (posRam->getDataFormat()->getComponents() != 3) return; //Only 3 dimensional meshes are supported

        //Vertex Normals data
        auto nit = util::find_if(InMesh->getBuffers(), [](const auto& buf)
        {
            return buf.first.type == BufferType::NormalAttrib;
        });
        if (nit == InMesh->getBuffers().end()) return; //could not find a position buffer
        // - in RAM
        const auto norRam = nit->second->getRepresentation<BufferRAM>();
        if (!norRam) return; //could not find a position buffer ram
        // - 3D
        if (norRam->getDataFormat()->getComponents() != 3) return; //We need 3D normals

        //We need the same amount of normals as we have vertices
        if (posRam->getSize() != norRam->getSize()) return;

        //Convert to raytracer triangles with transformed vertices and normals
        // - get the trafo as well as the inverted, transposed trafo for the normals
        Matrix<4, double> Trafo = InMesh->getWorldMatrix();
        Matrix<3, double> TrafoInvTransp = glm::inverseTranspose(Matrix<3, double>(Trafo));
        // - get a triangle mesh with bounding volume hierarchy
        std::shared_ptr<BVHIndexedTriangleMesh> RayTriMesh = std::make_shared<BVHIndexedTriangleMesh>();
        const size_t NumInVertices = posRam->getSize();
        RayTriMesh->reserveVertexPositions(NumInVertices);
        RayTriMesh->reserveVertexNormals(NumInVertices);
        for(size_t i(0);i<NumInVertices;i++)
        {
            //Get and transform position
            dvec3 Position = posRam->getAsDVec3(i);
            dvec4 HomogeneousPos(Position.x, Position.y, Position.z, 1.0f);
            dvec4 TransformedHomogeneousPos = Trafo * HomogeneousPos;

            //Get and transform normal
            const dvec3 NewNormal = glm::normalize(TrafoInvTransp * norRam->getAsDVec3(i));

            //Set position and normal
            RayTriMesh->addVertex(dvec3(TransformedHomogeneousPos.x / TransformedHomogeneousPos.w,
                                        TransformedHomogeneousPos.y / TransformedHomogeneousPos.w,
                                        TransformedHomogeneousPos.z / TransformedHomogeneousPos.w),
                                        NewNormal);
        }

        //Go over all index buffers and copy over the triangles.
        for (auto& ib : InMesh->getIndexBuffers())
        {
            if (ib.first.dt != DrawType::Triangles) continue;

            //Memory considerations
            RayTriMesh->reserveTriangleIndices(RayTriMesh->triangleIndices().size() + ib.second->getSize());

            //Copy; takes care of triangle strips and such
            meshutil::forEachTriangle(ib.first, *ib.second,
                [&](uint32_t i0, uint32_t i1, uint32_t i2)
                {
                    RayTriMesh->addTriangle(i0, i1, i2);
                });
        }

        RayTriMesh->setMaterial(TriangleMat);
        scene_.addRenderable(RayTriMesh);
    }
}


void Raytracer::addInputLights()
{
    //Get the input light sources
    auto MultiInLights = lightInput_.getVectorData();
    if (MultiInLights.empty())
    {
        addDefaultLights();
    }

    for(auto InLight : MultiInLights)
    {
        if (InLight->getLightSourceType() != LightSourceType::point)
        {
            LogWarn("Raytracer supports only point light sources. Ignored the input of type " << InLight->classIdentifier << ".");
        }

        //Get the point light and its data
        auto PLight = dynamic_cast<const PointLight*>(InLight.get());
        if (!PLight) continue; //should not happen

        const vec3 DiffuseColor = PLight->getIntensity() / 100;
        std::shared_ptr<Light> NewLight = std::make_shared<Light>(PLight->getPosition(),
                                          DiffuseColor * 0.2, DiffuseColor, DiffuseColor * 0.4);
        scene_.addLight(NewLight);
    }
}


void Raytracer::addDefaultLights()
{
    dvec3 RedShift(200, 170, 150);
    RedShift = Util::normalize(RedShift);
    dvec3 BlueShift(130, 160, 200);
    BlueShift = Util::normalize(BlueShift);

    std::shared_ptr<Light> light1 = std::make_shared<Light>(
        dvec3(5.0, 2.0, 6.0), ambientLight_, diffuseLight_.get() * RedShift, specularLight_);
    std::shared_ptr<Light> light2 = std::make_shared<Light>(
        dvec3(5.0, -7.0, 3.0), ambientLight_, diffuseLight_.get() * RedShift, specularLight_);
    std::shared_ptr<Light> light3 = std::make_shared<Light>(
        dvec3(-10.0, 4.0, 5.0), ambientLight_, diffuseLight_.get() * BlueShift, specularLight_);
    scene_.addLight(light1);
    scene_.addLight(light2);
    scene_.addLight(light3);
}

}// namespace inviwo
