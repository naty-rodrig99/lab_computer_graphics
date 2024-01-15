/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 24, 2017 - 17:17:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/phongmaterial.h>
#include <labraytracer/util.h>


namespace inviwo
{

PhongMaterial::PhongMaterial(const dvec3& ambientColor, const dvec3& diffuseColor, const dvec3& specularColor,
                             const double reflectance, const double shininess)
    :Material(ambientColor, reflectance)
    ,shininessExponent(shininess)
    //,ambientMaterialColor(ambientColor) <-- base class takes care of this
    ,diffuseMaterialColor(diffuseColor)
    ,specularMaterialColor(specularColor)
{}


dvec4 PhongMaterial::shade(const RayIntersection& intersection, const Light& light) const
{
    // get normal and light direction
    dvec3 N = intersection.getNormal();
    N = Util::normalize(N);
    dvec3 L = (light.getPosition() - intersection.getPosition());
    const double dist2 = dot(L, L);
    L = Util::normalize(L);
    dvec3 V = intersection.getRay().getDirection();// from intersection to camera
    V = Util::normalize(V);
    dvec3 R = L - Util::scalarMult(dot(N, L) * 2.0, N);// reflect(L, N); from intersection to reflected light source

    R = Util::normalize(R);

    double cosNL = std::max(double(dot(N, L)), double(0));
    double cosRV = std::max(dot(R, V), 0.0);
    //double cosRV = dot(R, V); // <-- incorrect, leads to haze around spheres

    //const dvec3 c_a = Util::scalarMult(1.0 / dist2, light.getAmbientColor());
    const dvec3 c_d = Util::scalarMult(1.0 / dist2, light.getDiffuseColor());
    const dvec3 c_s = Util::scalarMult(1.0 / dist2, light.getSpecularColor());
    //const dvec3 c_ambient = ambientMaterialColor * c_a;
    const dvec3 c_diffuse = Util::scalarMult(cosNL, diffuseMaterialColor) * c_d;
    const dvec3 c_specular = Util::scalarMult(pow(cosRV, shininessExponent), specularMaterialColor) * c_s;

    dvec3 tot = c_diffuse + c_specular;

    return dvec4(tot, 1);
}


dvec3 PhongMaterial::getAbsorptionSpectrum() const
{
    dvec3 ret = diffuseMaterialColor + ambientMaterialColor;
    double maxComp = ret[0];
    for(int i(1);i<3;i++)
    {
        if (ret[i] > maxComp) maxComp = ret[i];
    }
    if (maxComp > 1) ret /= maxComp;
    
    return ret;
}

}// namespace inviwo
