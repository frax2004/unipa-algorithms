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
  auto align(i32 match, i32 mismatch, i32 gapPenalty) const -> vector<StringAlignment>;
  auto align(i32 gapPenalty, vector<i32> substitutionMatrix) const -> vector<StringAlignment>;

}; // class NeedlemanWunshAligner

} // namespace pdls