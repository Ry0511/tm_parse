//
// Date       : 03/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::gen {

enum class MarkerByte : uint8_t {
    Nothing = 0,
    Invalid,
    Int8,
    Int16,
    Int32,
    Int64,
    Float,
    Bool,
    Str,
    BeginMetadata,
    BeginOptions,
    BeginKeyValue,
    BeginArray,
    End,
    Object,
    BeginSetCommand,
    BeginExpr,
    END_OF_ENUM
};

constexpr size_t marker_bytes_count = static_cast<size_t>(MarkerByte::END_OF_ENUM);

constexpr std::array<str_view, marker_bytes_count> marker_byte_names_vec{
    TXT("Nothing"),
    TXT("Invalid"),
    TXT("Int8"),
    TXT("Int16"),
    TXT("Int32"),
    TXT("Int64"),
    TXT("Float"),
    TXT("Bool"),
    TXT("Str"),
    TXT("BeginMetadata"),
    TXT("BeginOptions"),
    TXT("BeginKeyValue"),
    TXT("BeginArray"),
    TXT("End"),
    TXT("Object"),
    TXT("BeginSetCommand"),
    TXT("BeginExpr"),
};

constexpr str_view marker_byte_name(const MarkerByte value) {
    return marker_byte_names_vec.at(static_cast<size_t>(value));
}

constexpr str_view marker_byte_name(uint8_t value) {
    return marker_byte_names_vec.at(static_cast<size_t>(value));
}

constexpr bool is_valid_marker_byte(uint8_t value) {
    return value >= static_cast<uint8_t>(MarkerByte::Nothing)
           && value < static_cast<uint8_t>(MarkerByte::END_OF_ENUM);
}

constexpr int marker_byte_int_size(MarkerByte value) {
    // clang-format off
    switch (value) {
        case MarkerByte::Int8:  return sizeof(int8_t);
        case MarkerByte::Int16: return sizeof(int16_t);
        case MarkerByte::Int32: return sizeof(int32_t);
        case MarkerByte::Int64: return sizeof(int64_t);
        default: break;
    }
    // clang-format on
    throw std::out_of_range{"invalid integer MarkerByte"};
}

}  // namespace tm_parse::gen
