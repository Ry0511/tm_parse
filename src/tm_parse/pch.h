//
// Date       : 27/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_NO_STATUS
#define NOGDI
#define NOMINMAX
#include <windows.h>
#include <winternl.h>

#undef WIN32_NO_STATUS
#include <ntstatus.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <charconv>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <cwctype>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::int8_t;

using std::uint16_t;
using std::uint32_t;
using std::uint8_t;

////////////////////////////////////////////////////////////////////////////////
// | TM_PARSE |
////////////////////////////////////////////////////////////////////////////////

#include "tm_parse/lexer/token_type.h"
#include "tm_parse/util/logging.h"

namespace tm_parse {

namespace fs = std::filesystem;

constexpr size_t invalid_index_v = std::numeric_limits<size_t>::max();

using str_char = char;
using str = std::string;
using str_view = std::string_view;

using str_istream = std::basic_istream<str_char>;
using str_fstream = std::basic_fstream<str_char>;
using str_ifstream = std::basic_ifstream<str_char>;
using str_stream = std::basic_stringstream<str_char>;

#define TXT(S) S

}  // namespace tm_parse
