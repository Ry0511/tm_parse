//
// Date       : 31/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/git.inl"
#include "tm_parse/gen/asm_generator.h"

#include "tm_parse/parser/rules.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::gen {

using namespace tm_parse::rules;

void AsmGenerator::evaluate(
    const ProgramRule& program,
    const GeneratorContext& /*context*/
) {
    constexpr static size_t PRE_ALLOC_SIZE = (1024 * 512);
    m_Writer = BinaryFileWriter{};
    m_Writer.reserve(PRE_ALLOC_SIZE);

    write_file_header();

    if (const auto* def = program.mod_definition()) {
        m_Writer.write_marker_byte(MarkerByte::BeginMetadata);
        emit(def->expr_list());
        m_Writer.write_marker_byte(MarkerByte::End);
    }

    m_Writer.data().shrink_to_fit();

    LOG_INFO("[Program Assembly]\n{}", AsmGenerator::decompile(m_Writer.data()));
}

void AsmGenerator::write_file_header(void) {
    m_Writer.write_str(FILE_MAGIC_BYTES);
    m_Writer.write_int_fixed<int32_t>(FILE_VERSION_NUMBER);

    constexpr str_view git_head_sha1{TM_PARSE_GIT_HEAD_SHA1};
    m_Writer.write_str(git_head_sha1);
    m_Writer.write_str(txt::iso_date_now_str());

    // hash of the current content bytes
    uint32_t hash = txt::hash_data(m_Writer.data());
    m_Writer.write_int_fixed<uint32_t>(hash);
}

void AsmGenerator::emit(const SetCommand& /*cmd*/) {
    m_Writer.write_str("SetCommand not implemented");
}

void AsmGenerator::emit(const AssignmentExprList& list) {
    m_Writer.write_marker_byte(MarkerByte::BeginKeyValue);
    m_Writer.write_int(list.assignments().size());
    for (const auto& inner : list.assignments()) {
        emit(*inner);
    }
    m_Writer.write_marker_byte(MarkerByte::End);
}

void AsmGenerator::emit(const AssignmentExpr& assign) {
    // TODO: Property access should be a separate entity
    m_Writer.write_str(assign.property().full_text());
    emit_expr(assign.expr());
}

void AsmGenerator::emit(const TupleExpr& expr) {
    m_Writer.write_marker_byte(MarkerByte::BeginArray);
    m_Writer.write_int(expr.elements().size());
    for (const auto& inner : expr.elements()) {
        emit_expr(*inner);
    }
    m_Writer.write_marker_byte(MarkerByte::End);
}

void AsmGenerator::emit(const LiteralExpr& expr) {
    std::visit(
        [this](const auto& value) -> void {
            using T = std::remove_cvref_t<decltype(value)>;
            if constexpr (!std::is_same_v<T, std::monostate>) {
                this->emit_literal_value(value);
            } else {
                this->m_Writer.write_marker_byte(MarkerByte::Nothing);
            }
        },
        expr.value()
    );
}

void AsmGenerator::emit(const rules::MetaVarExpr& /*expr*/) {
    // TODO: This is the SrcObject, well, MetaVar ( $Primary ) ( Dot Id ArrayAccess? )*
    m_Writer.write_str("MetaVarExpr not implemented");
}

void AsmGenerator::emit(const rules::ClassObjectRef& /*expr*/) {
    m_Writer.write_str("ClassObjectRef not implemented");
}

void AsmGenerator::emit(const rules::UnquotedStrLiteral& expr) {
    // Not sure if this is enough
    m_Writer.write_str(expr.full_text());
}

void AsmGenerator::emit(const ComposedExpr& /*expr*/) {
    m_Writer.write_str("ComposedExpr not implemented");
}

void AsmGenerator::emit_expr(const ParserRule& expr) {
    const auto kind = expr.rule_kind();

    // clang-format off
    switch (kind) {
        case rkind::LiteralExpr:         ( emit(expr.as_ref<LiteralExpr>())        ); break;
        case rkind::TupleExpr:           ( emit(expr.as_ref<TupleExpr>())          ); break;
        case rkind::ComposedExpr:        ( emit(expr.as_ref<ComposedExpr>())       ); break;
        case rkind::MetaVarExpr:         ( emit(expr.as_ref<MetaVarExpr>())        ); break;
        case rkind::ClassObjectRef:      ( emit(expr.as_ref<ClassObjectRef>())     ); break;
        case rkind::UnquotedStrLiteral:  ( emit(expr.as_ref<UnquotedStrLiteral>()) ); break;
        case rkind::AssignmentExpr:      ( emit(expr.as_ref<AssignmentExpr>())     ); break;
        case rkind::AssignmentExprList:  ( emit(expr.as_ref<AssignmentExprList>()) ); break;

        // clang-format on
        default:
            throw std::runtime_error{
                std::format("unhandled rule in emit_expr '{}'", expr.rule_name())
            };
    }
}

void AsmGenerator::emit_literal_value(const Number& value) {
    if (value.is_float()) {
        m_Writer.write_float(value.as_float());
    } else {
        m_Writer.write_int(value.as_int());
    }
}

void AsmGenerator::emit_literal_value(Bool value) {
    m_Writer.write_bool(value);
}

void AsmGenerator::emit_literal_value(const Str& value) {
    m_Writer.write_str(value);
}

void AsmGenerator::emit_literal_value(const NoneType& /*value*/) {
    m_Writer.write_marker_byte(MarkerByte::Object);
    m_Writer.write_marker_byte(MarkerByte::Nothing);
}

}  // namespace tm_parse::gen
