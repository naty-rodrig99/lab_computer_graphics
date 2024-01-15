/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#pragma once

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/glmvec.h>

namespace inviwo {

/**
 * \class TrackballObject
 */
class IVW_CORE_API TrackballObject {
public:
    virtual ~TrackballObject() = default;
    virtual const vec3& getLookTo() const = 0;
    virtual const vec3& getLookFrom() const = 0;
    virtual const vec3& getLookUp() const = 0;

    virtual TrackballObject& setLookTo(vec3 lookTo) = 0;
    virtual TrackballObject& setLookFrom(vec3 lookFrom) = 0;
    virtual TrackballObject& setLookUp(vec3 lookUp) = 0;

    virtual TrackballObject& setLook(vec3 lookFrom, vec3 lookTo, vec3 lookUp) = 0;

    virtual vec3 getLookFromMinValue() const = 0;
    virtual vec3 getLookFromMaxValue() const = 0;

    virtual vec3 getLookToMinValue() const = 0;
    virtual vec3 getLookToMaxValue() const = 0;

    virtual float getNearPlaneDist() const = 0;
    virtual float getFarPlaneDist() const = 0;

    virtual vec3 getWorldPosFromNormalizedDeviceCoords(const vec3& ndcCoords) const = 0;
    virtual vec3 getNormalizedDeviceFromNormalizedScreenAtFocusPointDepth(
        const vec2& normalizedScreenCoord) const = 0;

    enum class Bounded { Yes, No };
    virtual void zoom(float factor, Bounded bounded) = 0;
};

}  // namespace inviwo
