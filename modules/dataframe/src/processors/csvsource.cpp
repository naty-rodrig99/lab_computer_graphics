/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2021 Inviwo Foundation
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

#include <inviwo/dataframe/processors/csvsource.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

#include <inviwo/dataframe/io/csvreader.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo CSVSource::processorInfo_{
    "org.inviwo.CSVSource",                   // Class identifier
    "CSV Source",                             // Display name
    "Data Input",                             // Category
    CodeState::Stable,                        // Code state
    "CPU, Plotting, Source, CSV, DataFrame",  // Tags
};
const ProcessorInfo CSVSource::getProcessorInfo() const { return processorInfo_; }

CSVSource::CSVSource(const std::string& file)
    : Processor()
    , data_("data")
    , inputFile_("inputFile_", "CSV File", file, "dataframe")
    , firstRowIsHeaders_("firstRowIsHeaders", "First Row Contains Column Headers", true)
    , delimiters_("delimiters", "Delimiters", ",")
    , doublePrecision_("doublePrecision", "Double Precision", false)
    , reloadData_("reloadData", "Reload Data") {

    addPort(data_);

    addProperties(inputFile_, firstRowIsHeaders_, delimiters_, doublePrecision_, reloadData_);

    isReady_.setUpdate([this]() { return filesystem::fileExists(inputFile_.get()); });
    inputFile_.onChange([this]() { isReady_.update(); });
}

void CSVSource::process() {
    if (inputFile_.get().empty()) return;

    CSVReader reader(delimiters_, firstRowIsHeaders_, doublePrecision_);

    data_.setData(reader.readData(inputFile_.get()));
}

}  // namespace inviwo
