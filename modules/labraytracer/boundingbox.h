#pragma once

//#include "math.h"
#include <labraytracer/labraytracermoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <labraytracer/ray.h>

namespace inviwo
{

/**
 * Represents an axis-aligned bounding box.
 */
class IVW_MODULE_LABRAYTRACER_API BoundingBox
{
public:
    BoundingBox()
        : mMin(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
               std::numeric_limits<double>::infinity())
        , mMax(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
               -std::numeric_limits<double>::infinity())
    {
    }
    BoundingBox(const dvec3& min, const dvec3& max)
        : mMin(min)
        , mMax(max)
    {
    }

    // Returns true in case of any intersection
    bool anyIntersection(const Ray& ray, double maxLambda) const;

    void merge(const BoundingBox& box);// merge with a given bounding box

    const dvec3& min() const { return mMin; }
    const dvec3& max() const { return mMax; }

    void setMin(const dvec3& min) { mMin = min; }
    void setMax(const dvec3& max) { mMax = max; }

    void expandByPoint(const dvec3& point);

    double computeArea() const;

private:
    dvec3 mMin;// Coordinate of min corner
    dvec3 mMax;// Coordinate of max corner
};

}// namespace inviwo
