/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2020-2021 Inviwo Foundation
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

#include <inviwo/core/util/introspection.h>

#include <string_view>

namespace inviwo {

namespace {

struct ClassWithClassIdentifierLower {
    static const std::string classIdentifier;
};

const std::string ClassWithClassIdentifierLower::classIdentifier = "id";

struct ClassWithClassIdentifierUpper {
    static const std::string CLASS_IDENTIFIER;
};

const std::string ClassWithClassIdentifierUpper::CLASS_IDENTIFIER = "id";

struct Empty {};

template <typename T>
struct [[deprecated("Type in warning")]] printType{};

}  // namespace

// printType<util::detail::lowerClassIdentifierType<ClassWithClassIdentifierLower>> tmp{};

static_assert(util::HasClassIdentifierLower<ClassWithClassIdentifierLower>::value);
static_assert(util::HasClassIdentifierUpper<ClassWithClassIdentifierUpper>::value);
static_assert(util::HasClassIdentifier<ClassWithClassIdentifierLower>::value);
static_assert(util::HasClassIdentifier<ClassWithClassIdentifierUpper>::value);
static_assert(!util::HasClassIdentifierLower<Empty>::value);
static_assert(!util::HasClassIdentifier<Empty>::value);

}  // namespace inviwo
