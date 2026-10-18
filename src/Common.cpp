#include <Common.hpp>
#include <cxxabi.h>

namespace std {
  auto demangle(const char* name) noexcept -> string {
    int status = -4;
  
    unique_ptr<char, void(*)(void*)> result {
      abi::__cxa_demangle(name, nullptr, nullptr, &status),
      free
    };
  
    return status == 0 ? result.get() : name;
  }
}