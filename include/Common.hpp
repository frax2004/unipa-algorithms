#pragma once

////////////////////////
// STL 
////////////////////////
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
#include <generator>
////////////////////////

using namespace std;
using namespace string_literals;
using namespace string_view_literals;

using i8 = char;
using i16 = short int;
using i32 = int;
using i64 = long long int;
using u8 = unsigned char;
using u16 = unsigned short int;
using u32 = unsigned int;
using u64 = unsigned long long int;
using f32 = float;
using f64 = double;

namespace std {
  auto demangle(const i8* name) noexcept -> string;
}