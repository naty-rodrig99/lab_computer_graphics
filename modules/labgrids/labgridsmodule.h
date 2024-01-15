/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Friday, January 20, 2023 - 11:10:04
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <labgrids/labgridsmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo
{

class IVW_MODULE_LABGRIDS_API LabGridsModule : public InviwoModule
{
public:
    LabGridsModule(InviwoApplication* app);
    virtual ~LabGridsModule() = default;
};

} // namespace
