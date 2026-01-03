//
// Date       : 31/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/gen/code_generator.h"

namespace tm_parse::gen {

enum class CodeType : uint8_t {
    Nothing = 0,
    BeginMetadata,
    BeginOptions,
    BeginKeyValue,
    BeginArray,
    End,
    Int8,
    Int16,
    Int32,
    Int64,
    Float,
    Bool,
    Str,
    Object,
    BeginSetCommand,
    BeginExpr,
};

class AsmGenerator : public CodeGenerator {
   public:
    static constexpr int32_t FILE_MAGIC_NUMBER = 0x69626D74;
    static constexpr int32_t FILE_VERSION_NUMBER{1};

   private:
    std::vector<uint8_t> m_Instructions;

   public:
    static str decompile(const std::vector<uint8_t>& ins);

   public:
    explicit AsmGenerator() = default;
    ~AsmGenerator() override = default;

   public:
    void evaluate(const rules::ProgramRule& program, const GeneratorContext& context) override;

   private:
    void write_file_header(void);
};

}  // namespace tm_parse::gen
