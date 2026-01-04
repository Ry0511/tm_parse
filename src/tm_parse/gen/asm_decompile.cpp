//
// Date       : 31/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/gen/asm_generator.h"

#include "tm_parse/util/text_helpers.h"

namespace tm_parse::gen {

namespace {
using Iter = std::vector<uint8_t>::const_iterator;

void read_file_header(str_stream& out, Iter& it);
int64_t read_int(str_stream& out, Iter& it);
double read_float(str_stream& out, Iter& it);
bool read_bool(str_stream& out, Iter& it);
str_view read_str(str_stream& out, Iter& it);
void read_obj(str_stream& out, Iter& it);

}  // namespace

str AsmGenerator::decompile(const std::vector<uint8_t>& ins) {
    str_stream out{};

    auto it = ins.begin();
    read_file_header(out, it);

    while (it != ins.end()) {
        if (!is_valid_marker_byte(*it)) {
            throw std::runtime_error{
                std::format("invalid marker byte in data: {}", static_cast<int>(*it))
            };
        }

        const auto marker = static_cast<MarkerByte>(*it);

        switch (marker) {
            case MarkerByte::Int8:
            case MarkerByte::Int16:
            case MarkerByte::Int32:
            case MarkerByte::Int64:
                read_int(out, it);
                break;
            case MarkerByte::Float:
                read_float(out, it);
                break;
            case MarkerByte::Bool:
                read_bool(out, it);
                break;
            case MarkerByte::Str:
                read_str(out, it);
                break;
            case MarkerByte::Object:
                read_obj(out, it);
                break;
            case MarkerByte::Nothing:
            case MarkerByte::Invalid:
            case MarkerByte::BeginSetCommand:
            case MarkerByte::BeginMetadata:
            case MarkerByte::BeginKeyValue:
            case MarkerByte::BeginOptions:
            case MarkerByte::BeginArray:
            case MarkerByte::BeginExpr:
            case MarkerByte::End:
                ++it;
                out << marker_byte_name(marker);
                break;

            default: {
                throw std::runtime_error{
                    std::format("unhandled marker byte - {}", marker_byte_name(marker))
                };
            }
        }
        out << TXT('\n');
    }

    return out.str();
}

namespace {

void read_file_header(str_stream& out, Iter& it) {
    out << TXT("[File Header]\n");
    auto start = it;
    // clang-format off
    out << TXT("Magic   => "); read_int(out, it); out << TXT('\n');
    out << TXT("Version => "); read_int(out, it); out << TXT('\n');
    out << TXT("SHA-1   => "); read_str(out, it); out << TXT('\n');
    out << TXT("Date    => "); read_str(out, it); out << TXT('\n');
    // clang-format on
    uint32_t computed_hash = txt::hash_data(std::span<const uint8_t>{start, it});
    out << TXT("Hash    => ");
    auto actual_hash = static_cast<uint32_t>(read_int(out, it));
    out << TXT(" == ") << computed_hash << TXT('\n');
    if (computed_hash != actual_hash) {
        throw std::runtime_error{
            "potentially malformed data as the header hash does not match the computed"
        };
    }
    out << str(80, TXT('-')) << TXT('\n');
}

int64_t read_int(str_stream& out, Iter& it) {
    const auto marker = static_cast<MarkerByte>(*it++);
    const int bytes_to_read = marker_byte_int_size(marker);
    int64_t value{0};
    std::memcpy(&value, &it[0], bytes_to_read);
    it += bytes_to_read;
    out << std::format(TXT("{} {}"), marker_byte_name(marker), value);
    return value;
}

double read_float(str_stream& out, Iter& it) {
    const auto marker = static_cast<MarkerByte>(*it++);
    if (marker != MarkerByte::Float) {
        throw std::runtime_error{"expecting float marker byte"};
    }
    uint64_t int_value = read_int(out, it);
    double value{};
    std::memcpy(&value, &int_value, sizeof(double));
    out << std::format(TXT(" {} {:.15g}"), marker_byte_name(marker), value);
    return value;
}

bool read_bool(str_stream& out, Iter& it) {
    const auto marker = static_cast<MarkerByte>(*it++);
    if (marker != MarkerByte::Bool) {
        throw std::runtime_error{"expecting bool marker byte"};
    }
    bool value{(*it++ == BinaryFileWriter::TRUE_VALUE)};
    out << std::format(TXT("{} {}"), marker_byte_name(marker), value ? TXT("True") : TXT("False"));
    return value;
}

str_view read_str(str_stream& out, Iter& it) {
    const auto marker = static_cast<MarkerByte>(*it++);
    if (marker != MarkerByte::Str) {
        throw std::runtime_error{"expecting str marker byte"};
    }
    out << marker_byte_name(marker) << TXT(' ');
    int64_t len = read_int(out, it);
    str_view text{
        reinterpret_cast<str_view::const_pointer>(&it[0]),
        static_cast<size_t>(len)
    };
    out << TXT(" \"") << text << TXT('"');
    it += static_cast<int32_t>(len);
    return text;
}

void read_obj(str_stream& out, Iter& it) {
    const auto marker = static_cast<MarkerByte>(*it++);
    if (marker != MarkerByte::Object) {
        throw std::runtime_error{"expecting object marker byte"};
    }
    out << std::format(TXT("{} {}"), marker_byte_name(marker), marker_byte_name(*it++));
}

}  // namespace

}  // namespace tm_parse::gen