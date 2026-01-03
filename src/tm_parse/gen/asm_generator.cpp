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

namespace {

template <CodeType Type>
uint8_t code_int() noexcept {
    return static_cast<uint8_t>(Type);
}

// pedantic
static_assert(std::endian::native == std::endian::little);

template <class T>
void write_int_t(std::vector<uint8_t>& buf, T val) {
    constexpr auto max_shifts = sizeof(T);
    constexpr auto shift_size = 8;
    using UnsignedType = std::make_unsigned_t<T>;

    auto uval = static_cast<UnsignedType>(val);
    for (size_t i = 0; i < max_shifts; ++i) {
        buf.emplace_back(
            static_cast<uint8_t>((uval >> (shift_size * i)) & 0xFF)
        );
    }
}

void write_int(std::vector<uint8_t>& buf, int64_t val) {
    constexpr auto fits_into = [](int64_t value, const auto& bounds) {
        return std::cmp_greater_equal(value, bounds.min())
               && std::cmp_less_equal(value, bounds.max());
    };

    if (fits_into(val, std::numeric_limits<int8_t>{})) {
        buf.emplace_back(code_int<CodeType::Int8>());
        write_int_t<int8_t>(buf, static_cast<int8_t>(val));

    } else if (fits_into(val, std::numeric_limits<int16_t>{})) {
        buf.emplace_back(code_int<CodeType::Int16>());
        write_int_t<int16_t>(buf, static_cast<int16_t>(val));

    } else if (fits_into(val, std::numeric_limits<int32_t>{})) {
        buf.emplace_back(code_int<CodeType::Int32>());
        write_int_t<int32_t>(buf, static_cast<int32_t>(val));

    } else if (fits_into(val, std::numeric_limits<int64_t>{})) {
        buf.emplace_back(code_int<CodeType::Int64>());
        write_int_t<int64_t>(buf, val);

    } else {
        throw std::overflow_error{"int value can't be written"};
    }
}

void write_f64(std::vector<uint8_t>& buf, double val) {
    static_assert(sizeof(double) == sizeof(uint64_t));
    uint64_t bits{0};
    std::memcpy(&bits, &val, sizeof(bits));
    write_int_t<uint64_t>(buf, bits);
}

void write_str(std::vector<uint8_t>& buf, const str_view& val) {
    static_assert(sizeof(str_view::value_type) == sizeof(uint8_t));
    buf.emplace_back(code_int<CodeType::Str>());
    write_int(buf, static_cast<int64_t>(val.size()));
    for (const str_char elem : val) {
        buf.emplace_back(static_cast<uint8_t>(elem));
    }
}

void emit(std::vector<uint8_t>& ins, const SetCommand& cmd);
void emit(std::vector<uint8_t>& ins, const AssignmentExprList& list);
void emit(std::vector<uint8_t>& ins, const AssignmentExpr& assign);
void emit(std::vector<uint8_t>& ins, const TupleExpr& expr);
void emit(std::vector<uint8_t>& ins, const LiteralExpr& expr);
void emit(std::vector<uint8_t>& ins, const ComposedExpr& expr);

void write(std::vector<uint8_t>& ins, const Int& val);
void write(std::vector<uint8_t>& ins, const Float& val);
void write(std::vector<uint8_t>& ins, const Bool& val);
void write(std::vector<uint8_t>& ins, const Str& val);
void write(std::vector<uint8_t>& ins, const NoneType& val);
void write(std::vector<uint8_t>& ins, const std::monostate& val);

void emit_expr(std::vector<uint8_t>& ins, const ParserRule& expr);

}  // namespace

void AsmGenerator::evaluate(
    const rules::ProgramRule& program,
    const GeneratorContext& /*context*/
) {
    constexpr static size_t PRE_ALLOC_SIZE = (1024 * 512);
    m_Instructions.clear();
    m_Instructions.reserve(PRE_ALLOC_SIZE);

    write_file_header();

    if (const auto* def = program.mod_definition()) {
        m_Instructions.emplace_back(code_int<CodeType::BeginMetadata>());
        emit(m_Instructions, def->expr_list());
        m_Instructions.emplace_back(code_int<CodeType::End>());
    }

    m_Instructions.shrink_to_fit();

    LOG_INFO("[Program Assembly]\n{}", AsmGenerator::decompile(m_Instructions));

    // for (const auto& rule : program.child_rules()) {
    //     rkind::ParserRuleKind kind = rule->rule_kind();
    //
    //     if (kind == rkind::SetCommand) {
    //         emit(m_Instructions, rule->as_ref<SetCommand>());
    //     } else if (kind == rkind::ObjectDefinition) {
    //         // TODO: although support for this does exist, i am not sure how this one should be
    //         //  implemented. i.e., is it creating an object or mutating one?
    //         //  only way we can know is at runtime in the game
    //     }
    // }
}

void AsmGenerator::write_file_header(void) {
    m_Instructions.emplace_back(code_int<CodeType::Int32>());
    write_int_t<int32_t>(m_Instructions, FILE_MAGIC_NUMBER);

    m_Instructions.emplace_back(code_int<CodeType::Int32>());
    write_int_t<int32_t>(m_Instructions, FILE_VERSION_NUMBER);

    constexpr std::string_view git_head_sha1{TM_PARSE_GIT_HEAD_SHA1};
    write_str(m_Instructions, git_head_sha1);
    write_str(m_Instructions, txt::iso_date_now_str());

    // hash of the current content bytes
    uint32_t hash = txt::hash_data({m_Instructions.data(), m_Instructions.size()});
    m_Instructions.emplace_back(code_int<CodeType::Int32>());
    write_int_t<uint32_t>(m_Instructions, hash);
}

////////////////////////////////////////////////////////////////////////////////
// | IMPL |
////////////////////////////////////////////////////////////////////////////////

namespace {

void emit(std::vector<uint8_t>& /*ins*/, const SetCommand& /*cmd*/) {
    // TODO: Implement
}

void emit(std::vector<uint8_t>& ins, const AssignmentExprList& list) {
    ins.emplace_back(code_int<CodeType::BeginKeyValue>());
    write_int(ins, static_cast<int64_t>(list.assignments().size()));

    for (const auto& inner : list.assignments()) {
        emit(ins, *inner);
    }
    ins.emplace_back(code_int<CodeType::End>());
}

void emit(std::vector<uint8_t>& ins, const AssignmentExpr& assign) {
    write_str(ins, assign.property().full_text());
    emit_expr(ins, assign.expr());
}

void emit(std::vector<uint8_t>& ins, const TupleExpr& expr) {
    ins.emplace_back(code_int<CodeType::BeginArray>());
    write_int(ins, static_cast<int64_t>(expr.elements().size()));
    for (const auto& inner : expr.elements()) {
        emit_expr(ins, *inner);
    }
    ins.emplace_back(code_int<CodeType::End>());
}

void emit(std::vector<uint8_t>& ins, const LiteralExpr& expr) {
    std::visit(
        [&ins](const auto& val) { write(ins, val); },
        expr.value()
    );
}

void emit(std::vector<uint8_t>& ins, const ComposedExpr& /*expr*/) {
    ins.emplace_back(code_int<CodeType::BeginExpr>());
    write_int(ins, int64_t{0});
}

void write(std::vector<uint8_t>& ins, const Int& val) {
    if (!val.has_value()) {
        ins.emplace_back(code_int<CodeType::Nothing>());
    } else {
        write_int(ins, static_cast<int64_t>(*val));
    }
}

void write(std::vector<uint8_t>& ins, const Float& val) {
    if (!val.has_value()) {
        ins.emplace_back(code_int<CodeType::Nothing>());
    } else {
        ins.emplace_back(code_int<CodeType::Float>());
        write_f64(ins, *val);
    }
}

void write(std::vector<uint8_t>& ins, const Bool& val) {
    constexpr uint8_t true_value{1};
    constexpr uint8_t false_value{0};
    ins.emplace_back(code_int<CodeType::Bool>());
    ins.emplace_back(val ? true_value : false_value);
}

void write(std::vector<uint8_t>& ins, const Str& val) {
    write_str(ins, val);
}

void write(std::vector<uint8_t>& ins, const NoneType& /*val*/) {
    ins.emplace_back(code_int<CodeType::Object>());
    ins.emplace_back(code_int<CodeType::Nothing>());
}

void write(std::vector<uint8_t>& ins, const std::monostate& /*val*/) {
    ins.emplace_back(code_int<CodeType::Nothing>());
}

void emit_expr(std::vector<uint8_t>& ins, const ParserRule& expr) {
    const rkind::ParserRuleKind kind = expr.rule_kind();

#define TM_PARSE_EMIT(ord, cls)        \
    case rkind::ord:                   \
        emit(ins, expr.as_ref<cls>()); \
        break

    switch (kind) {
        TM_PARSE_EMIT(AssignmentExprList, AssignmentExprList);
        TM_PARSE_EMIT(AssignmentExpr, AssignmentExpr);
        TM_PARSE_EMIT(TupleExpr, TupleExpr);
        TM_PARSE_EMIT(LiteralExpr, LiteralExpr);

        default: {
            break;
        }
    }

#undef TM_PARSE_EMIT
}

}  // namespace

}  // namespace tm_parse::gen
