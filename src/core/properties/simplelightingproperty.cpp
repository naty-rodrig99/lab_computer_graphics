/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2021 Inviwo Foundation
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

#include <inviwo/core/properties/simplelightingproperty.h>
#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

const std::string SimpleLightingProperty::classIdentifier = "org.inviwo.SimpleLightingProperty";
std::string SimpleLightingProperty::getClassIdentifier() const { return classIdentifier; }

SimpleLightingProperty::SimpleLightingProperty(std::string identifier, std::string displayName,
                                               CameraProperty* camera,
                                               InvalidationLevel invalidationLevel,
                                               PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics)
    , shadingMode_("shadingMode", "Shading",
                   {{"none", "No Shading", ShadingMode::None},
                    {"ambient", "Ambient", ShadingMode::Ambient},
                    {"diffuse", "Diffuse", ShadingMode::Diffuse},
                    {"specular", "Specular", ShadingMode::Specular},
                    {"blinnphong", "Blinn Phong", ShadingMode::BlinnPhong},
                    {"phong", "Phong", ShadingMode::Phong}},
                   5, InvalidationLevel::InvalidResources)
    , referenceFrame_("referenceFrame", "Space")
    , lightPosition_("lightPosition", "Position", util::ordinalLight(vec3(0.0f, 5.0f, 5.0f)))
    , ambientColor_("lightColorAmbient", "Ambient color", util::ordinalColor(vec3(0.15f)))
    , diffuseColor_("lightColorDiffuse", "Diffuse color", util::ordinalColor(vec3(0.6f)))
    , specularColor_("lightColorSpecular", "Specular color", util::ordinalColor(vec3(0.4f)))
    , specularExponent_("materialShininess", "Shininess", 60.0f, 1.0f, 180.0f)
    , camera_(camera) {

    referenceFrame_.addOption("world", "World", static_cast<int>(Space::WORLD));
    referenceFrame_.setSelectedValue(static_cast<int>(Space::WORLD));
    if (camera_) {
        referenceFrame_.addOption("view", "View", static_cast<int>(Space::VIEW));
        referenceFrame_.setSelectedValue(static_cast<int>(Space::VIEW));
    }
    referenceFrame_.setCurrentStateAsDefault();

    addProperties(shadingMode_, referenceFrame_, lightPosition_, ambientColor_, diffuseColor_,
                  specularColor_, specularExponent_);
}

SimpleLightingProperty::SimpleLightingProperty(const SimpleLightingProperty& rhs)
    : CompositeProperty(rhs)
    , shadingMode_(rhs.shadingMode_)
    , referenceFrame_(rhs.referenceFrame_)
    , lightPosition_(rhs.lightPosition_)
    , ambientColor_(rhs.ambientColor_)
    , diffuseColor_(rhs.diffuseColor_)
    , specularColor_(rhs.specularColor_)
    , specularExponent_(rhs.specularExponent_)
    , camera_(rhs.camera_) {

    addProperties(shadingMode_, referenceFrame_, lightPosition_, ambientColor_, diffuseColor_,
                  specularColor_, specularExponent_);
}

SimpleLightingProperty* SimpleLightingProperty::clone() const {
    return new SimpleLightingProperty(*this);
}

SimpleLightingProperty::~SimpleLightingProperty() = default;

vec3 SimpleLightingProperty::getTransformedPosition() const {
    switch (static_cast<Space>(referenceFrame_.getSelectedValue())) {
        case Space::VIEW:
            return camera_ ? vec3(camera_->inverseViewMatrix() * vec4(lightPosition_.get(), 1.0f))
                           : lightPosition_.get();
        case Space::WORLD:
        default:
            return lightPosition_.get();
    }
}

}  // namespace inviwo
