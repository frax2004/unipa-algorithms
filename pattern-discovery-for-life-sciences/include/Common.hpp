#pragma once

#include <print>
#include <iostream>
#include <variant>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <ranges>
#include <typeindex>
#include <csignal>
#include <cmath>
#include <algorithm>
#include <mdspan>
#include <stdexcept>

using namespace std;
using namespace string_literals;
using namespace string_view_literals;

namespace std {
  auto demangle(const char* name) noexcept -> string;
}

using i8 = signed char;
using i16 = signed short int;
using i32 = signed int;
using i64 = signed long long int;
using u8 = unsigned char;
using u16 = unsigned short int;
using u32 = unsigned int;
using u64 = unsigned long long int;
using f32 = float;
using f64 = double;