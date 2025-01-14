/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Monday, October 30, 2017 - 13:13:06
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraycasting/compositor.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo Compositor::processorInfo_
{
    "org.inviwo.Compositor",  // Class identifier
    "Compositor",             // Display name
    "KTH Labs",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo Compositor::getProcessorInfo() const
{
    return processorInfo_;
}

Compositor::Compositor()
    :Processor()
    , volIn_("volin")
    , image_("img")
    , method_("method", "Compositing Method")
    , tf_("tf", "Transfer Function", &volIn_)
    , firstHitIsoValue_("iso", "Iso Value", 0.65f, 0.0f, 1.0f, 0.01f)
{
    // Add ports
    addPort(volIn_);
    addPort(image_);

    // Add Properties
    method_.addOption("average", "Average", 0);
    method_.addOption("first", "First Hit", 1);
    method_.addOption("max", "Maximum", 2);
    method_.addOption("ftb", "Front to Back", 3);
    method_.addOption("btf", "Back to Front", 4);
    addProperty(method_);

    method_.onChange([&]()
    {
        if (method_.get() == 1)
        {
            firstHitIsoValue_.setVisible(true);
        }
        else
        {
            firstHitIsoValue_.setVisible(false);
        }
    });

    addProperty(tf_);

    tf_.get().clearPoints();
    // Set up a gray ramp transfer function as default
    // (from fully transparent black to fully opaque white)
    tf_.get().addPoint(vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f));
    tf_.get().addPoint(vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
    tf_.setCurrentStateAsDefault();

    addProperty(firstHitIsoValue_);

}


void Compositor::process()
{

    // Get the input volume
    if (!volIn_.hasData())
    {
        return;
    }
    auto vol = volIn_.getData();

    if (!vol)
    {
        LogError("Read null volume! Exiting");
        return;
    }

    // Get information about the input volume
    const VolumeRAM* vr = vol->getRepresentation< VolumeRAM >();
    dims_ = vol->getDimensions();

    // Prepare the output
    auto outImage = std::make_shared<Image>();
    auto outLayer = outImage->getColorLayer();
    outLayer->setDimensions(size2_t(dims_.x, dims_.y));
    outLayer->setDataFormat(DataVec4Float32::get());
    auto lr = outLayer->getEditableRepresentation<LayerRAM>();

    // Actually compute and set the output
    renderImage(vr, lr);
    image_.setData(outImage);

}

vec4 Compositor::getAverage(const VolumeRAM * vr, const size_t x, const size_t y)
{
    double sum = 0;

    // Go through all z values for the x/y values the function was called for
    for (size_t z = 0; z < dims_.z; z++)
    {
        // Volume values are queried with getAsDouble()
        double val = vr->getAsDouble(size3_t(x, y, z));
        sum += val;
    }
    double average = sum / dims_.z;
    // The transfer function is sampled as follows
    return tf_.get().sample(average);
}

vec4 Compositor::getMaximum(const VolumeRAM * vr, const size_t x, const size_t y)
{
    double Mx = std::numeric_limits<double>::lowest();

    for (int z = 0; z < dims_.z; z++)
    {
        double val = vr->getAsDouble(size3_t(x, y, z));
        Mx = std::max(Mx, val);
    }
    return tf_.get().sample(Mx);
}

vec4 Compositor::getFirstHit(const VolumeRAM * vr, const size_t x, const size_t y, double isovalue)
{
    for (int z = 0; z < dims_.z; z++)
    {
        double val = vr->getAsDouble(size3_t(x, y, z));
        if (val > isovalue)
        {
            return tf_.get().sample(val);
        }
    }
    return vec4(0, 0, 0, 0);
}

vec4 Compositor::accumulateFrontToBack(const VolumeRAM * vr, const size_t x, const size_t y)
{
    vec3 col(0, 0, 0);
    double alpha = 0;

    for (int z = 0; z < dims_.z; z++)
    {

        if (alpha >= 1.0)
            break;

        double val = vr->getAsDouble(size3_t(x, y, z));

        vec4 currCol = tf_.get().sample(val);

        col += vec3(currCol.x * (1 - alpha) * currCol.w, currCol.y * (1 - alpha) * currCol.w, currCol.z * (1 - alpha) * currCol.w);

        alpha += (1 - alpha) * currCol.w;
    }

    return vec4(col, alpha);
}

vec4 Compositor::accumulateBackToFront(const VolumeRAM * vr, const size_t x, const size_t y)
{
    vec3 col(0, 0, 0);
    double alpha = 0;

    for (int z = dims_.z - 1; z >= 0; z--)
    {
        double val = vr->getAsDouble(size3_t(x, y, z));
        vec4 currCol = tf_.get().sample(val);
        col = (1 - currCol[3]) * col + vec3(currCol.x * currCol[3], currCol.y * currCol[3], currCol.z * currCol[3]);
        alpha = alpha * (1 - currCol[3]) + currCol[3];
    }

    return vec4(col, alpha);
}

void Compositor::renderImage(const VolumeRAM * vr, LayerRAM * lr)
{
    for (size_t i = 0; i < dims_.x; i++)
    {
        for (size_t j = 0; j < dims_.y; j++)
        {

            vec4 color;

            switch (method_.get())
            {
            case 0: color = getAverage(vr, i, j); break;
            case 1: color = getFirstHit(vr, i, j, firstHitIsoValue_.get()); break;
            case 2: color = getMaximum(vr, i, j); break;
            case 3: color = accumulateFrontToBack(vr, i, j); break;
            case 4: color = accumulateBackToFront(vr, i, j); break;
            default: break;
            }

            lr->setFromDVec4(size2_t(i, j), color);

        }
    }
}

} // namespace

