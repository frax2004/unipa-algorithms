#pragma once
#include <pattern-discovery-for-life-sciences/StringAlignment.hpp>



namespace pdls {

class NeedlemanWunshAligner final {
private:
  string mRow = "";
  string mColumn = "";

public:
  NeedlemanWunshAligner(string lhs, string rhs);

  auto getFirst() const -> string_view;
  auto getSecond() const -> string_view;
  auto getMatchMismatchAlignment(i32 match, i32 mismatch, i32 gapPenalty) const -> StringAlignment;

private:
  enum class Direction : u8 {
    TOP,
    TOP_LEFT,
    LEFT,
    UNKNOWN,
  };

  static auto getDirectionOf(i32 top, i32 topLeft, i32 left) -> Direction;

}; // class NeedlemanWunshAligner

} // namespace pdls