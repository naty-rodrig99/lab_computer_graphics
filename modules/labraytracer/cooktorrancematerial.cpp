/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Wednesday, February 22, 2023 - 20:57:11
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/cooktorrancematerial.h>
#include <labraytracer/util.h>
#include <algorithm>

namespace inviwo
{

CookTorranceMaterial::CookTorranceMaterial(const dvec3& albedo, const double& roughness, const double& refraction,
                                           const dvec3& ambientColor, const double& reflection)
    :Material(ambientColor, reflection)
    ,albedo_(albedo)
    ,roughness_(roughness)
    ,refraction_(refraction)
{}

dvec4 CookTorranceMaterial::shade(const RayIntersection& intersection, const Light& light) const
{
    const dvec3 rayDir = Util::normalize(intersection.getRay().getDirection());
    const dvec3 normal = Util::normalize(intersection.getNormal());
    const double lightDist2 = glm::length2(intersection.getPosition() - light.getPosition());
    const dvec3 lightDir = Util::normalize(intersection.getPosition() - light.getPosition());

    double const vdn = glm::clamp(dot(-rayDir, normal), 0.0, 1.0);
    double const ndl = glm::clamp(dot(normal, -lightDir), 0.0, 1.0);
    auto const h = -Util::normalize(lightDir + rayDir);
    double const ndh = glm::clamp(dot(normal, h), Util::epsilon, 1.0);
    double const vdh = glm::clamp(dot(-rayDir, h), 0.0, 1.0);

    double const G = (fabs(vdh) >= Util::epsilon) ? std::min(1.0, 2.0 * ndh * std::min(vdn, ndl) / vdh) : 1.0;
    double const ndh2 = ndh * ndh;

    double const roughness = std::max(roughness_, 0.03);
    double const r2 = roughness * roughness;
    double D = exp((ndh2 - 1.0) / (r2 * ndh2)) / (4.0 * r2 * ndh2 * ndh2);

    double const f0 = (1.0f - refraction_) / (1.0f + refraction_);
    double const f2 = f0 * f0;
    double const F = f2 + (1.0 - f2) * pow(1.0 - vdn, 5.0f);

    //Light color with quadratic fall-off
    const dvec3 lightColor = light.getDiffuseColor();// / lightDist2;

    //Specular part
    double const spec = glm::clamp(G * D * F, 0.0, 1.0);
    const dvec3 specular = glm::mix(lightColor, albedo_, 0.5) * spec;

    //Diffuse part
    const dvec3 L = -lightDir;
    double cosNL = std::max(double(dot(normal, L)), double(0));
    const dvec3 diffuse = Util::scalarMult(cosNL, albedo_) * lightColor;

    //Final
    return dvec4(diffuse + specular, 1.0);
}

dvec4 CookTorranceMaterial::shadeAmbient(const RayIntersection& intersection, const Light& light) const
{
    const double dist2 = glm::length2(light.getPosition() - intersection.getPosition());
    const dvec3 c_a = light.getAmbientColor();// / dist2;
    const dvec3 c_ambient = ambientMaterialColor * c_a;

    return dvec4(c_ambient, 1.0);
}


dvec3 CookTorranceMaterial::getAbsorptionSpectrum() const
{
    dvec3 ret = albedo_ + ambientMaterialColor;
    double maxComp = ret[0];
    for(int i(1);i<3;i++)
    {
        if (ret[i] > maxComp) maxComp = ret[i];
    }
    if (maxComp > 1) ret /= maxComp;
    
    return ret;
}

}// namespace inviwo
