//
// Date       : 30/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse {

namespace rules {
class ProgramRule;
}

struct GeneratorContext {
    fs::path BasePath{fs::current_path()};  // The path to generate relatively from
    bool AllowFileCreation{true};           // Whether generation is allowed to create files
    bool EmitDebugInfo{true};               // The generated output will have debug information
};

class CodeGenerator {
   public:
    virtual ~CodeGenerator() = default;

   public:
    virtual void evaluate(
        const rules::ProgramRule& program,
        const GeneratorContext& context
    ) = 0;
};

}  // namespace tm_parse
