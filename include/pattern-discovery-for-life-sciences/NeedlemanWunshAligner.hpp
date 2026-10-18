#pragma once
#include <StringAlignment.hpp>

namespace pdls {
  class NeedlemanWunshAligner {
    private:
    string mRow = "";
    string mColumn = "";

    private:
    enum class Direction : u8 {
      TOP,
      TOP_LEFT,
      LEFT,
      UNKNOWN,
    };

    public:
    NeedlemanWunshAligner(string lhs, string rhs);

    public:
    auto getMatchMismatchAlignment(i32 match, i32 mismatch, i32 gapPenalty) const -> StringAlignment;

    private:
    static auto getDirectionOf(i32 top, i32 topLeft, i32 left) -> Direction;
  };

}