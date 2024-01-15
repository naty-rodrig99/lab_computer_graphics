/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Friday, January 20, 2023 - 11:10:04
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labgrids/labgridsmodule.h>
#include <labgrids/uniformbinning.h>

namespace inviwo
{

LabGridsModule::LabGridsModule(InviwoApplication* app) : InviwoModule(app, "LabGrids")

{
	registerProcessor<UniformBinning>();
}

} // namespace
