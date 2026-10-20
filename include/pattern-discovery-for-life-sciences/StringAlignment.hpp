#pragma once
#include <Common.hpp>

namespace pdls {

  struct StringAlignment {
    private:
    string mFirst = "";
    string mSecond = "";

    public:
    StringAlignment(string first, string second);
    StringAlignment(StringAlignment const&) = default;

    public:
    auto matches() const -> size_t;
    auto mismatches() const -> size_t;
    auto getFirst() const -> string_view;
    auto getSecond() const -> string_view;

    auto toString() const -> string;
  };

}