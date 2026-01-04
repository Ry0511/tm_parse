//
// Date       : 31/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/gen/code_generator.h"
#include "tm_parse/gen/binary_file.h"

namespace tm_parse {
class ParserRule;

namespace rules {
class SetCommand;
class AssignmentExprList;
class AssignmentExpr;
class TupleExpr;
class LiteralExpr;
class ComposedExpr;
class MetaVarExpr;
class ClassObjectRef;
class UnquotedStrLiteral;
}  // namespace rules

}  // namespace tm_parse

namespace tm_parse::gen {

class AsmGenerator : public CodeGenerator {
   public:
    static constexpr int32_t FILE_MAGIC_NUMBER = 0x69626D74;
    static constexpr int32_t FILE_VERSION_NUMBER{1};

   private:
    BinaryFileWriter m_Writer;

   public:
    static str decompile(const std::vector<uint8_t>& ins);

   public:
    explicit AsmGenerator() = default;
    ~AsmGenerator() override = default;

   public:
    void evaluate(const rules::ProgramRule& program, const GeneratorContext& context) override;

   private:
    void write_file_header(void);
    void emit(const rules::SetCommand& cmd);
    void emit(const rules::TupleExpr& expr);
    void emit(const rules::ComposedExpr& expr);
    void emit(const rules::MetaVarExpr& expr);
    void emit(const rules::ClassObjectRef& expr);
    void emit(const rules::UnquotedStrLiteral& expr);
    void emit(const rules::AssignmentExpr& assign);
    void emit(const rules::AssignmentExprList& list);
    void emit(const rules::LiteralExpr& expr);

    void emit_expr(const ParserRule& expr);

    void emit_literal_value(const Int& value);
    void emit_literal_value(const Float& value);
    void emit_literal_value(Bool value);
    void emit_literal_value(const Str& value);
    void emit_literal_value(const NoneType& value);
};

}  // namespace tm_parse::gen
