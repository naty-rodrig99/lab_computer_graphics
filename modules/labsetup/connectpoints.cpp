/*********************************************************************
 *  Author  : Tino Weinkauf, Jiahui Liu
 *  Init    : Monday, August 27, 2018 - 20:35:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */


#include <modules/labsetup/connectpoints.h>

namespace inviwo
{
namespace kth
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ConnectPoints::processorInfo_
{
    "org.inviwo.ConnectPoints",// Class identifier
    "Connect Points",          // Display name
    "KTH Labs",                // Category
    CodeState::Experimental,   // Code state
    Tags::None,                // Tags
};

const ProcessorInfo ConnectPoints::getProcessorInfo() const
{
    return processorInfo_;
}

ConnectPoints::ConnectPoints()
    : Processor()
    , portInLines("InLines")
    , portOutLines("OutLines")
#ifndef STUDENT_VIEW
    , propCheckMark("checkmark", "Checkmark", true)
#endif
{
    addPort(portInLines);
    addPort(portOutLines);

#ifndef STUDENT_VIEW
    addProperty(propCheckMark);
#endif
}

namespace
{
/// Little helper function for drawing a colored line.
void drawLineSegment(const vec2& v1, const vec2& v2, const vec4& color, IndexBufferRAM* indexBuffer,
                     Vec3BufferRAM* vertices)
{
    indexBuffer->add(static_cast<std::uint32_t>(vertices->getSize()));
    vertices->add({vec3(v1[0], v1[1], 0), vec3(0, 0, 1), vec3(v1[0], v1[1], 0), color});
    indexBuffer->add(static_cast<std::uint32_t>(vertices->getSize()));
    vertices->add({vec3(v2[0], v2[1], 0), vec3(0, 0, 1), vec3(v2[0], v2[1], 0), color});
}
}// namespace


#ifdef STUDENT_VIEW
void ConnectPoints::BoundingBox(const std::vector<vec3>& Points, IndexBufferRAM* OutIndices,
                                Vec3BufferRAM* OutVertices)
{
    // TODO: finish this function
    drawLineSegment({0, 0}, {0, 1}, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment({0, 1}, {1, 1}, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment({1, 1}, {1, 0}, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment({1, 0}, {0, 0}, {255, 0, 0, 1}, OutIndices, OutVertices);
}

#else

void ConnectPoints::BoundingBox(const std::vector<vec3>& Points, IndexBufferRAM* OutIndices,
                                Vec3BufferRAM* OutVertices)
{
    // Initialize values for the lowerleft corner (has minimum x and minimum y value)
    // by initializing the value for the minimum of the data with the maximum possible value.
    // Any actual data value will be smaller.
    vec2 lowerleftCorner = vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    // and upper right corner (has maximum x and maximum y value) initalized with minumum values.
    vec2 upperRightCorner = vec2(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

    /* Alternative for initializing values */
    vec2 pointA(0.0f, 1.0f);
    vec2 pointB = vec2(0.0f, 1.0f);
    auto pointC = vec2{0.0f, 1.0f};
    vec2 pointD{0.0f, 1.0f};
    vec2 pointE = {0.0f, 1.0f};
    // 0.0f means this value is a floating point value, just 0.0 is a double

    // A vec2 has x and y attributes, a vec3 had x, y and z attributes
    if (pointA.x < 1.f || pointB.y > 0.f)
    {
        // This is how you can print in a console
        LogProcessorInfo("Testing Logging");
    }

    for (auto point : Points)
    {
        // Is this points x-value smaller than the so far found minimum -> update it
        if (point.x < lowerleftCorner.x)
        {
            lowerleftCorner.x = point.x;
        }

        // Is this points x-value larger than the so far found maximum -> update it
        if (point.x > upperRightCorner.x)
        {
            upperRightCorner.x = point.x;
        }
        // Same for the y value
        if (point.y < lowerleftCorner.y)
        {
            lowerleftCorner.y = point.y;
        }
        if (point.y > upperRightCorner.y)
        {
            upperRightCorner.y = point.y;
        }
    }

    // Alternatives to iterate over a vector
    // with index
    for (size_t i = 0; i < Points.size(); i++)
    {
        vec3 point = Points[i];
        //...
    }
    // with iterater (note: here we need a const_iterator and cbegin, cend)
    for (std::vector<vec3>::const_iterator it = Points.cbegin(); it != Points.cend(); it++)
    {
        vec3 point = *it;
        ///
    }

    vec2 lowerRightCorner = {lowerleftCorner.x, upperRightCorner.y};
    vec2 upperLeftCorner = {upperRightCorner.x, lowerleftCorner.y};

    drawLineSegment(lowerleftCorner, lowerRightCorner, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment(lowerRightCorner, upperRightCorner, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment(upperRightCorner, upperLeftCorner, {255, 0, 0, 1}, OutIndices, OutVertices);
    drawLineSegment(upperLeftCorner, lowerleftCorner, {255, 0, 0, 1}, OutIndices, OutVertices);
}
#endif


void ConnectPoints::process()
{
    // Get the input data
    auto MultiInLines = portInLines.getVectorData();

    // Prepare output data
    auto OutLines = std::make_shared<Mesh>(DrawType::Lines, ConnectivityType::Strip);
    auto OutVertexBuffer = std::make_shared<Buffer<vec3>>();
    auto OutVertices = OutVertexBuffer->getEditableRAMRepresentation();
    OutLines->addBuffer(BufferType::PositionAttrib, OutVertexBuffer);

    for (auto InLines : MultiInLines)
    {
        // Vertex data
        auto pit = util::find_if(InLines->getBuffers(), [](const auto& buf) {
            return buf.first.type == BufferType::PositionAttrib;
        });
        if (pit == InLines->getBuffers().end()) return;// could not find a position buffer
        // - in RAM
        const auto posRam = pit->second->getRepresentation<BufferRAM>();
        if (!posRam) return;// could not find a position buffer ram
        // - 3D
        if (posRam->getDataFormat()->getComponents() != 3)
            return;// Only 3 dimensional meshes are supported
        // - save into a reasonable format with transformed vertices
        std::vector<glm::vec3> AllVertices;
        Matrix<4, float> Trafo = InLines->getWorldMatrix();
        const size_t NumInVertices = posRam->getSize();
        AllVertices.reserve(NumInVertices);
        for (size_t i(0); i < NumInVertices; i++)
        {
            dvec3 Position = posRam->getAsDVec3(i);
            glm::vec4 HomogeneousPos(Position.x, Position.y, Position.z, 1.0f);
            glm::vec4 TransformedHomogeneousPos = Trafo * HomogeneousPos;

            AllVertices.push_back(vec3(TransformedHomogeneousPos.x / TransformedHomogeneousPos.w,
                                       TransformedHomogeneousPos.y / TransformedHomogeneousPos.w,
                                       TransformedHomogeneousPos.z / TransformedHomogeneousPos.w));
        }

        // For each line buffer
        const auto& AllIndexBuffers = InLines->getIndexBuffers();
        for (const auto& IdxBuffer : AllIndexBuffers)
        {
            // Well, do we actually have lines? If not, next buffer!
            if (IdxBuffer.first.dt != DrawType::Lines) continue;

            // Get the indices of the lines
            const auto& Indices = IdxBuffer.second->getRAMRepresentation()->getDataContainer();

            // Create a simple vector of line vertices for the corner cutting
            std::vector<glm::vec3> LineVertices;
            LineVertices.reserve(Indices.size());
            for (const auto& idx : Indices)
            {
                if (LineVertices.empty() || LineVertices.back() != AllVertices[idx])
                {
                    LineVertices.push_back(AllVertices[idx]);
                }
            }

            // Remove a possibly duplicated first/last point (closed loop case),
            // since we assume a closed loop anyway.
            if (LineVertices.size() > 1 && LineVertices[0] == LineVertices[LineVertices.size() - 1])
            {
                LineVertices.pop_back();
            }

            // Draw the bounding box!
            auto OutIndexBuffer = std::make_shared<IndexBuffer>();
            auto OutIndices = OutIndexBuffer->getEditableRAMRepresentation();
            OutLines->addIndices(Mesh::MeshInfo(DrawType::Lines, ConnectivityType::Strip),
                                 OutIndexBuffer);
            BoundingBox(LineVertices, OutIndices, OutVertices);
        }
    }

    // Push it out!
    portOutLines.setData(OutLines);
}

}// namespace kth
}// namespace inviwo
