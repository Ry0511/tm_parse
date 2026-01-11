//
// Date       : 03/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/gen/binary_file.h"

namespace tm_parse::gen {

void BinaryFileWriter::write_marker_byte(MarkerByte marker) {
    m_Data.emplace_back(static_cast<uint8_t>(marker));
}

void BinaryFileWriter::write_float(double value) {
    static_assert(sizeof(double) == sizeof(uint64_t));
    uint64_t bits{0};
    std::memcpy(&bits, &value, sizeof(bits));
    write_marker_byte(MarkerByte::Float);
    write_int_fixed<uint64_t>(bits, false);
}

void BinaryFileWriter::write_str(str_view value) {
    static_assert(sizeof(str_view::value_type) == sizeof(uint8_t));
    write_marker_byte(MarkerByte::Str);
    write_int(value.size());
    m_Data.insert(m_Data.end(), value.begin(), value.end());
}

void BinaryFileWriter::write_bool(bool value) {
    write_marker_byte(MarkerByte::Bool);
    m_Data.emplace_back(value ? TRUE_VALUE : FALSE_VALUE);
}

void BinaryFileWriter::write_to_file(const fs::path& dest_file) {
    // TODO: This is untested, just assuming it works atm since it is a low priority
    std::ofstream out{dest_file, std::ios::binary | std::ios::trunc};

    if (m_Data.empty()) {
        throw std::runtime_error{"attempted to write empty binary data to file"};
    }

    if (!out) {
        throw std::runtime_error{
            std::format("failed to open file '{}'", dest_file.string())
        };
    }

    out.write(
        reinterpret_cast<const char*>(m_Data.data()),
        static_cast<std::streamsize>(m_Data.size())
    );

    if (!out) {
        throw std::runtime_error{
            std::format("failed to write binary data to file '{}'", dest_file.string())
        };
    }
}

}  // namespace tm_parse::gen
