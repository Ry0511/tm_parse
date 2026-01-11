//
// Date       : 03/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/gen/marker_bytes.h"

namespace tm_parse::gen {

static_assert(std::endian::native == std::endian::little);

class BinaryFileWriter {
   public:
    static constexpr uint8_t TRUE_VALUE = 1;
    static constexpr uint8_t FALSE_VALUE = 0;

   private:
    std::vector<uint8_t> m_Data;

   public:
    BinaryFileWriter() noexcept = default;
    ~BinaryFileWriter() noexcept = default;

   public:
    const auto& data() const noexcept { return m_Data; }
    auto size() const noexcept { return m_Data.size(); }
    auto& data() noexcept { return m_Data; }

   public:
    void clear() noexcept { m_Data.clear(); }
    void reserve(size_t length) { m_Data.reserve(length); }

   public:
    BinaryFileWriter(const BinaryFileWriter&) = default;
    BinaryFileWriter& operator=(const BinaryFileWriter&) = default;
    BinaryFileWriter(BinaryFileWriter&&) noexcept = default;
    BinaryFileWriter& operator=(BinaryFileWriter&&) noexcept = default;

   public:
    template <class T>
        requires std::is_integral_v<T>
    static constexpr MarkerByte marker_byte_for_int() {
        // clang-format off
        switch (sizeof(T)) {
            case sizeof(uint8_t):  return MarkerByte::Int8;
            case sizeof(uint16_t): return MarkerByte::Int16;
            case sizeof(uint32_t): return MarkerByte::Int32;
            case sizeof(uint64_t): return MarkerByte::Int64;
            default: throw std::logic_error{"invalid integer size provided"};
        }
        // clang-format on
    }

    /**
     * @tparam T The type of the integer to write
     * @param value The value to write
     */
    template <class T>
        requires std::is_integral_v<T>
    void write_int_fixed(T value, bool write_int_marker = true) {
        using UnsignedType = std::make_unsigned_t<T>;
        if (write_int_marker) {
            write_marker_byte(marker_byte_for_int<T>());
        }
        auto uval = static_cast<UnsignedType>(value);
        for (size_t i = 0; i < sizeof(T); ++i) {
            m_Data.emplace_back(
                static_cast<uint8_t>((uval >> (8 * i)) & 0xFF)
            );
        }
    }

    /**
     * Writes an int value as the smallest signed integral type that fits it.
     * @tparam T The integer type to write
     * @param value The value to write
     * @exception std::runtime_error if the value does not fit into any signed integral type
     */
    template <class T>
        requires std::is_integral_v<T>
    void write_int(T value) {
        const auto fits_into = []<class U>(auto value) -> bool {
            using Limits = std::numeric_limits<U>;
            return std::cmp_greater_equal(value, Limits::min())
                   && std::cmp_less_equal(value, Limits::max());
        };

        if (fits_into.template operator()<int8_t>(value)) {
            write_int_fixed<int8_t>(static_cast<int8_t>(value));

        } else if (fits_into.template operator()<int16_t>(value)) {
            write_int_fixed<int16_t>(static_cast<int16_t>(value));

        } else if (fits_into.template operator()<int32_t>(value)) {
            write_int_fixed<int32_t>(static_cast<int32_t>(value));

        } else if (fits_into.template operator()<int64_t>(value)) {
            write_int_fixed<int64_t>(static_cast<int64_t>(value));

        } else {
            throw std::runtime_error{"integer could not be written"};
        }
    }

   public:
    void write_marker_byte(MarkerByte marker);
    void write_float(double value);
    void write_str(str_view value);
    void write_bool(bool value);

   public:
    void write_to_file(const fs::path& dest_file);
};

}  // namespace tm_parse::gen
