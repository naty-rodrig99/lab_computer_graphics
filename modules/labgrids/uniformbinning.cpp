/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Friday, January 20, 2023 - 11:11:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labgrids/uniformbinning.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <modules/base/algorithm/meshutils.h>


namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo UniformBinning::processorInfo_
{
    "org.inviwo.UniformBinning",      // Class identifier
    "Uniform Binning",                // Display name
    "KTH Labs",                 // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo UniformBinning::getProcessorInfo() const
{
    return processorInfo_;
}


UniformBinning::UniformBinning()
    :Processor()
    ,portInPoints("InPoints")
    ,portOutMesh("OutMesh")
    ,portOutPoints("OutPoints")
    ,propGridSize("GridSize", "Grid Size", size2_t(100,100), size2_t(2,2), size2_t(1000,1000))
    ,propShowCounts("ShowCounts", "Show Counts", true)
    ,propHeightFactor("HeightFactor", "Height Factor", 1.0f, 0.1f, 10.0f)
    ,propColor("Color", "Color")
    ,bAnnouncedXYFastest(false)
{
    // Add ports
    addPort(portInPoints);
    addPort(portOutMesh);
    addPort(portOutPoints);

    // Add properties
    addProperty(propGridSize);
    addProperty(propShowCounts);
    addProperty(propHeightFactor);
    addProperty(propColor);
}

void UniformBinning::DoUniformBinning(const int nx, const int ny, const vector<vec2>& Points,
                                      vector<vec2>& UniformGridVertices, vector<int>& Counts)
{
    //TODO
 
    //Find the minimum and maximum elements along the x and y coordinates of the point in Points 
    auto xExtremes = std::minmax_element(Points.begin(), Points.end(), [](const vec2& lhs, const vec2& rhs) { return lhs.x < rhs.x; });
    auto yExtremes = std::minmax_element(Points.begin(), Points.end(), [](const vec2& lhs, const vec2& rhs) { return lhs.y < rhs.y; });
    vec2 lowerLeft(xExtremes.first->x, yExtremes.first->y); //x and y coordinates of the min x and y values
    vec2 upperRight(xExtremes.second->x, yExtremes.second->y); //x and y coordinates of the max x and y values

    //finding the width of each grid
    float unit_x = float(upperRight.x - lowerLeft.x) / float(nx-1); //width of bounding box / divisions along the x-axis
    //finding the height of each grid
    float unit_y = float(upperRight.y - lowerLeft.y) / float(ny-1); //width of bounding box / divisions along the y-axis

    const size_t NumVertices = nx * ny; //number of vertices
    UniformGridVertices.reserve(NumVertices); //Tell the machine how many points you expect to add;
    for (int i = 0; i < ny; i++)
    {
        for (int j = 0; j < nx; j++)
        {
            //distributes the vertices evenly along the x and y axis
            vec2 NewVertex(lowerLeft.x + j *unit_x, lowerLeft.y + i * unit_y);
            UniformGridVertices.push_back(NewVertex);
        }
    }

    Counts.resize((nx - 1) * (ny - 1));  //Make the array Counts as large as we have grid cells in the uniform grid.
    fill(Counts.begin(), Counts.end(), 0);//Fill the array with zeros
    for (vec2 p : Points) 
    {
        //calculate position relative to the lower-left corner of the bounding box, then divide by width and height
        int x_idx = (p.x - lowerLeft.x) / unit_x; 
        int y_idx = (p.y - lowerLeft.y) / unit_y;

        //adjusts the x and y-index if the point is exactly on the right boundary of the grid
        if (x_idx == nx - 1) x_idx--; 
        if (y_idx == ny - 1) y_idx--;

        //increment count of points in the corresponding grid
        Counts[y_idx * (nx - 1) + x_idx] += 1; 
    }

 
    /*
    //Example: add some vertices to UniformGridVertices.
    // this is not the solution
     const size_t NumVertices = ny;      //How many vertices will we have in the uniform grid?
    UniformGridVertices.reserve(NumVertices); //Tell the machine how many points you expect to add; much faster in real-world scenarios!
    for(int j=0;j<ny;j++)
    {
        vec2 NewVertex( float(j)/ny, float(j)/nx );
        UniformGridVertices.push_back(NewVertex);
    }

    //Example: add values to Counts
    // this is not the solution
    Counts.resize(nx + ny + 10); //Make the array Counts as large as we have grid cells in the uniform grid. Again, this is not the solution.
    fill(Counts.begin(), Counts.end(), 0); //Fill the array with zeros
    for(int i=0;i<nx;i++)
    {
        size_t idx = i + ny; //Which grid cell are we in?
        Counts[idx] += 1; //Increment the count in this grid cell by one.
    } */
}


bool UniformBinning::CheckResults(const int nx, const int ny, const int NumPoints,
                                  const vector<vec2>& UniformGridVertices, const vector<int>& Counts)
{
    const int NumVertices = nx * ny;
    const int NumCells = (nx-1) * (ny-1);

    bool bSuccess = true;

    if (UniformGridVertices.size() < 4)
    {
        LogErrorCustom("Tino says", "I expect to get at least 4 vertices in the 'UniformGridVertices' variable, but you only provided " << UniformGridVertices.size() << ".");
        bSuccess = false;
    }

    if (Counts.size() < 1)
    {
        LogErrorCustom("Tino says", "I expect to get at least 1 value in the 'Counts' variable, but you only provided " << Counts.size() << ".");
        bSuccess = false;
    }

    if (NumVertices != UniformGridVertices.size())
    {
        const string morefewer = NumVertices > UniformGridVertices.size() ? "more" : "fewer";
        LogErrorCustom("Tino says", "On a " << nx << "x" << ny << " grid, I expect to get " << morefewer << " than the " << UniformGridVertices.size() << " vertices you gave me in the 'UniformGridVertices' variable.");
        bSuccess = false;
    }

    if (NumCells != Counts.size())
    {
        const string morefewer = NumCells > Counts.size() ? "more" : "fewer";
        LogErrorCustom("Tino says", "On a " << nx << "x" << ny << " grid, I expect to get " << morefewer << " than the " << Counts.size() << " cells you gave me in the 'Counts' variable.");
        bSuccess = false;
    }

    int SumOfCounts(0);
    bool bPositive(true);
    for (int c : Counts)
    {
        SumOfCounts += c;
        bPositive = bPositive && (c >= 0);
    }
    if (SumOfCounts != NumPoints)
    {
        LogErrorCustom("Tino says", "Accumulating all values in the 'Counts' variable yields " << SumOfCounts << ", but it should be as much as the number of input points, which is " << NumPoints << ".");
        //bSuccess = false; //no need to abort further computations because of this error condition
    }
    if (!bPositive)
    {
        LogErrorCustom("Tino says", "All values in the 'Counts' variable should be positive or zero, but I found negative values.");
    }

    return bSuccess;
}


shared_ptr<Mesh> UniformBinning::BuildSkyscrapers(const bool bShowCounts, const int nx, const int ny,
                                      const float HeightFactor, const TransferFunction& TF,
                                      const vector<vec2>& UniformGridVertices, const vector<int>& Counts)
{
    //Prepare output data for skyscrapers
    auto OutMesh = std::make_shared<BasicMesh>();
    auto Indices = OutMesh->addIndexBuffer(DrawType::Triangles, ConnectivityType::None);

    int MaxCount(1);
    for (int c : Counts) MaxCount = std::max(c, MaxCount);

    //Figure out whether the arrays were build x-fastest or y-fastest.
    assert(UniformGridVertices.size() > 1);
    const vec2 Diff = UniformGridVertices[1] - UniformGridVertices[0];
    const int stride = (Diff.x > Diff.y) ? nx : ny;
    if (!bAnnouncedXYFastest)
    {
        const string xyfastest = (Diff.x > Diff.y) ? "x-fastest" : "y-fastest";
        LogInfoCustom("Tino says", "The vertices seem to be stored in " << xyfastest <<" manner and will be interpreted as such.");
        bAnnouncedXYFastest = true;
    }

    //Build skyscrapers
    for (int idx(0),vidx(-1);idx<(int)Counts.size();idx++,vidx++)
    {
        //Height of cuboid, z coordinate
        const float RelativeCountValue = Counts[idx] / float(MaxCount);
        const float z = HeightFactor * (0.1f + (bShowCounts ? RelativeCountValue : 0.f));
        // - Color
        vec4 Color = TF.sample(RelativeCountValue);
        if (!bShowCounts) Color = vec4(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX, 1);

        //Grid vertices making up that cuboid, x,y coordinates
        if (idx % (stride-1) == 0) vidx++;
        const vec2& v0 = UniformGridVertices[vidx];
        const vec2& v3 = UniformGridVertices[vidx+stride+1];
        vec3 posLLF = vec3(v0.x, v0.y, 0);
        vec3 posURB = vec3(v3.x, v3.y, z);

        //Create a transformation matrix describing position and size of cube
        mat4 m = glm::translate(mat4(1.0f), posLLF);
        m = glm::scale(m, posURB - posLLF);
        //Create cube
        auto NewCube = meshutil::cube(m, Color);
        //Append to existing mesh
        OutMesh->Mesh::append(*NewCube);
    }

    return OutMesh;
}


void UniformBinning::process()
{
    // We only do something, if we have input
    if (!portInPoints.hasData()) return;

    //Get the input data
    auto MultiInPoints = portInPoints.getVectorData();

    //Prepare output data for flattened points
    auto OutPoints = std::make_shared<Mesh>(DrawType::Points, ConnectivityType::None);
    auto OutVertexBuffer = std::make_shared<Buffer<vec3> >();
    auto OutVertices = OutVertexBuffer->getEditableRAMRepresentation();
    OutPoints->addBuffer(BufferType::PositionAttrib, OutVertexBuffer);

    //Get parameters
    const int nx = (int)propGridSize.get().x;
    const int ny = (int)propGridSize.get().y;

    //Collect all input points
    vector<vec2> AllInPoints;
    for(auto InPoints : MultiInPoints)
    {
        //Vertex data
        auto pit = util::find_if(InPoints->getBuffers(), [](const auto& buf)
        {
            return buf.first.type == BufferType::PositionAttrib;
        });
        if (pit == InPoints->getBuffers().end()) return; //could not find a position buffer
        // - in RAM
        const auto posRam = pit->second->getRepresentation<BufferRAM>();
        if (!posRam) return; //could not find a position buffer ram
        // - 3D
        if (posRam->getDataFormat()->getComponents() != 3) return; //Only 3 dimensional meshes are supported
        // - save into a reasonable format with transformed vertices
        Matrix<4, float> Trafo = InPoints->getWorldMatrix();
        const size_t NumInPoints = posRam->getSize();
        AllInPoints.reserve(AllInPoints.size() + NumInPoints);
        OutVertices->reserve(OutVertices->getSize() + NumInPoints);
        for(size_t i(0);i<NumInPoints;i++)
        {
            dvec3 Position = posRam->getAsDVec3(i);
            glm::vec4 HomogeneousPos(Position.x, Position.y, Position.z, 1.0f);
            glm::vec4 TransformedHomogeneousPos = Trafo * HomogeneousPos;

            AllInPoints.push_back(vec2(TransformedHomogeneousPos.x / TransformedHomogeneousPos.w,
                                       TransformedHomogeneousPos.y / TransformedHomogeneousPos.w//,
                                     //TransformedHomogeneousPos.z / TransformedHomogeneousPos.w
                                     ) );

            OutVertices->add(vec3(TransformedHomogeneousPos.x / TransformedHomogeneousPos.w,
                                  TransformedHomogeneousPos.y / TransformedHomogeneousPos.w,
                                  0
                                  ));
        }
    }

    //Show the input points in a flattened manner on one of our outports
    portOutPoints.setData(OutPoints);

    //Get the vertices of the uniform grid and the counts per grid cell
    vector<vec2> UniformGridVertices;
    vector<int> Counts;
    DoUniformBinning(nx, ny, AllInPoints, UniformGridVertices, Counts);

    //Check binning results. If ok, create and set binning output mesh.
    if (CheckResults(nx, ny, (int)AllInPoints.size(), UniformGridVertices, Counts))
    {
        //Visualize the results in a city/skyscraper metaphor
        auto OutMesh = BuildSkyscrapers(propShowCounts, nx, ny, propHeightFactor, propColor.get(), UniformGridVertices, Counts);

        portOutMesh.setData(OutMesh);
    }

}

} // namespace

