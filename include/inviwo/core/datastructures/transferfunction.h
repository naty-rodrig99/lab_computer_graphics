/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2021 Inviwo Foundation
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

#include <inviwo/core/datastructures/datatraits.h>
#include <inviwo/core/datastructures/tfprimitiveset.h>
#include <inviwo/core/util/fileextension.h>

namespace inviwo {

class Layer;

template <typename T>
class LayerRAMPrecision;

/**
 * \ingroup datastructures
 * \class TransferFunction
 * \brief data structure for holding 1D transfer function data
 */
class IVW_CORE_API TransferFunction : public TFPrimitiveSet {
public:
    TransferFunction(size_t textureSize = 1024);
    TransferFunction(const std::vector<TFPrimitiveData>& values, size_t textureSize = 1024);
    TransferFunction(const std::vector<TFPrimitiveData>& values, TFPrimitiveSetType type,
                     size_t textureSize = 1024);
    TransferFunction(const TransferFunction& rhs);
    TransferFunction& operator=(const TransferFunction& rhs);

    virtual ~TransferFunction();

    const Layer* getData() const;
    size_t getTextureSize() const;

    void setMaskMin(double maskMin);
    double getMaskMin() const;
    void setMaskMax(double maskMax);
    double getMaskMax() const;

    void clearMask();

    /**
     * Notify that the layer data (texture) needs to be updated next time it is requested.
     */
    virtual void invalidate() override;

    virtual void serialize(Serializer& s) const override;
    virtual void deserialize(Deserializer& d) override;

    /**
     * Sample the transfer function at position v and return the respective color and
     * opacity (rgba). The range of the transfer function is [0,1].
     *
     * @param v   sampling position, if v is outside the range [0,1] it is clamped to [0,1]
     * @return color and opacity at position v
     */
    vec4 sample(double v) const;
    /**
     * Sample the transfer function at position v and return the respective color and
     * opacity (rgba). The range of the transfer function is [0,1].
     *
     * @param v   sampling position, if v is outside the range [0,1] it is clamped to [0,1]
     * @return color and opacity at position v
     */
    vec4 sample(float v) const;

    friend bool operator==(const TransferFunction& lhs, const TransferFunction& rhs);

    virtual std::vector<FileExtension> getSupportedExtensions() const override;
    virtual void save(const std::string& filename,
                      const FileExtension& ext = FileExtension()) const override;
    virtual void load(const std::string& filename,
                      const FileExtension& ext = FileExtension()) override;

    virtual std::string_view getTitle() const override;

    /**
     * Simplify a vector of TF points by removing points that can be replaced by interpolating
     * between the previous and the next point, as long as the resulting error is less then delta.
     * the error is the absolute maximal component wise error at the point to be removed.
     */
    static std::vector<TFPrimitiveData> simplify(const std::vector<TFPrimitiveData>& points,
                                                 double delta = 0.01);

protected:
    void calcTransferValues() const;

    virtual std::string_view serializationKey() const override;
    virtual std::string_view serializationItemKey() const override;

private:
    double maskMin_;
    double maskMax_;

    mutable bool invalidData_;
    std::shared_ptr<LayerRAMPrecision<vec4>> dataRepr_;
    std::unique_ptr<Layer> data_;
};

bool operator==(const TransferFunction& lhs, const TransferFunction& rhs);
bool operator!=(const TransferFunction& lhs, const TransferFunction& rhs);

template <>
struct DataTraits<TransferFunction> {
    static const std::string& classIdentifier() {
        static const std::string id{"org.inviwo.transferfunction"};
        return id;
    }
    static const std::string& dataName() {
        static const std::string name{"Transfer function"};
        return name;
    }
    static uvec3 colorCode() { return uvec3{55, 66, 77}; }
    static Document info(const TransferFunction& data) {
        using H = utildoc::TableBuilder::Header;
        using P = Document::PathComponent;
        Document doc;
        doc.append("b", "TransferFunction", {{"style", "color:white;"}});
        utildoc::TableBuilder tb(doc.handle(), P::end());
        tb(H("Number of primitives: "), data.size());
        utildoc::TableBuilder tb2(doc.handle(), P::end());
        tb2(H("Index"), H("Pos"), H("Color"));
        // abbreviate list if there are more than 20 items
        const size_t ncols = (data.size() > 20) ? 10 : data.size();
        for (size_t i = 0; i < ncols; i++) {
            tb2(std::to_string(i + 1), data[i].getPosition(), data[i].getColor());
        }
        if (ncols != data.size()) {
            doc.append("span",
                       "... (" + std::to_string(data.size() - ncols) + " additional primitives)");
        }
        return doc;
    }
};

}  // namespace inviwo
