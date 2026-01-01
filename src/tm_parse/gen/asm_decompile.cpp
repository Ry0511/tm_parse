//
// Date       : 31/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/gen/asm_generator.h"

namespace tm_parse::gen {

namespace {

constexpr std::array<str_view, 16> op_codes{
    TXT("Nothing"),
    TXT("BeginMetadata"),
    TXT("BeginOptions"),
    TXT("BeginKeyValue"),
    TXT("BeginArray"),
    TXT("End"),
    TXT("Int8"),
    TXT("Int16"),
    TXT("Int32"),
    TXT("Int64"),
    TXT("Float"),
    TXT("Bool"),
    TXT("Str"),
    TXT("Object"),
    TXT("BeginSetCommand"),
    TXT("BeginExpr"),
};

int64_t read_int(str_stream& out, const std::vector<uint8_t>& ins, size_t& index);
void read_f64(str_stream& out, const std::vector<uint8_t>& ins, size_t& index);
void read_bool(str_stream& out, const std::vector<uint8_t>& ins, size_t& index);
void read_str(str_stream& out, const std::vector<uint8_t>& ins, size_t& index);
void read_obj(str_stream& out, const std::vector<uint8_t>& ins, size_t& index);

}  // namespace

str AsmGenerator::decompile(const std::vector<uint8_t>& ins) {
    str_stream out{};

    const char* data = reinterpret_cast<const char*>(ins.data());

    for (size_t i = 0; i < ins.size();) {
        uint8_t code = ins.at(i);

        switch (static_cast<CodeType>(code)) {
            case CodeType::Nothing:
            case CodeType::BeginMetadata:
            case CodeType::BeginOptions:
            case CodeType::BeginKeyValue:
            case CodeType::BeginArray:
            case CodeType::End:
            case CodeType::BeginSetCommand:
            case CodeType::BeginExpr:
                out << op_codes.at(code);
                ++i;
                break;
            case CodeType::Int8:
            case CodeType::Int16:
            case CodeType::Int32:
            case CodeType::Int64:
                read_int(out, ins, i);
                break;
            case CodeType::Float:
                read_f64(out, ins, i);
                break;
            case CodeType::Bool:
                read_bool(out, ins, i);
                break;
            case CodeType::Str:
                read_str(out, ins, i);
                break;
            case CodeType::Object:
                read_obj(out, ins, i);
                break;
            default: {
                throw std::runtime_error{"unhandled code type"};
            }
        }

        out << TXT('\n');
    }

    return out.str();
}

namespace {

int64_t read_int(str_stream& out, const std::vector<uint8_t>& ins, size_t& index) {
    uint8_t int_code = ins.at(index++);
    out << op_codes.at(int_code);

    int bytes_to_read{0};
    int64_t value{0};

    switch (static_cast<CodeType>(int_code)) {
        case CodeType::Int8:
            bytes_to_read = sizeof(int8_t);
            break;
        case CodeType::Int16:
            bytes_to_read = sizeof(int16_t);
            break;
        case CodeType::Int32:
            bytes_to_read = sizeof(int32_t);
            break;
        case CodeType::Int64:
            bytes_to_read = sizeof(int64_t);
            break;
        default:
            throw std::runtime_error{"expecting int op-code"};
    }

    std::memcpy(&value, &ins.at(index), bytes_to_read);
    out << TXT(' ') << value;
    index += bytes_to_read;
    return value;
}

void read_f64(str_stream& out, const std::vector<uint8_t>& ins, size_t& index) {
    uint8_t f64_code = ins.at(index++);
    if (f64_code != static_cast<uint8_t>(CodeType::Float)) {
        throw std::runtime_error{"expecting floating point op-code"};
    }
    out << op_codes.at(f64_code);
    constexpr auto bytes_to_read = sizeof(double);
    static_assert(bytes_to_read == sizeof(int64_t));
    double value{0.0};
    std::memcpy(&value, &ins.at(index), sizeof(value));
    out << TXT(' ') << std::format("{:.9f}", value);
    index += bytes_to_read;
}

void read_bool(str_stream& out, const std::vector<uint8_t>& ins, size_t& index) {
    uint8_t bool_code = ins.at(index++);
    if (bool_code != static_cast<uint8_t>(CodeType::Bool)) {
        throw std::runtime_error{"expecting bool op-code"};
    }
    out << op_codes.at(bool_code);
    bool value{ins.at(index++) != 0};
    out << TXT(' ') << (value ? TXT("True") : TXT("False"));
}

void read_str(str_stream& out, const std::vector<uint8_t>& ins, size_t& index) {
    uint8_t str_code = ins.at(index++);
    if (str_code != static_cast<uint8_t>(CodeType::Str)) {
        throw std::runtime_error{"expecting string op-code"};
    }
    out << op_codes.at(str_code) << TXT(' ');
    int64_t len = read_int(out, ins, index);

    out << TXT(" \"");
    for (int64_t i = 0; i < len; ++i) {
        out << static_cast<char>(ins.at(index + i));
    }
    out << TXT('\"');

    index += len;
}

void read_obj(str_stream& out, const std::vector<uint8_t>& ins, size_t& index) {
    out << op_codes.at(static_cast<uint8_t>(ins.at(index++)));
    out << op_codes.at(static_cast<uint8_t>(ins.at(index++)));
}

}  // namespace

}  // namespace tm_parse::gen