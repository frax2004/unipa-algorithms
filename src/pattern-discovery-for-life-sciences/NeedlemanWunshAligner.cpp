#include <pattern-discovery-for-life-sciences/NeedlemanWunshAligner.hpp>


namespace pdls {


NeedlemanWunshAligner::NeedlemanWunshAligner(string column, string row)
  : mRow { row }
  , mColumn { column }
{
  if(this->mColumn.empty() or this->mRow.empty()) {
    throw invalid_argument{ "Alignment strings cannot be empty." };
  }
}

auto NeedlemanWunshAligner::getFirst() const -> string_view {
  return this->mColumn;
}

auto NeedlemanWunshAligner::getSecond() const -> string_view {
  return this->mRow;
}

auto NeedlemanWunshAligner::getDirectionOf(i32 top, i32 topLeft, i32 left) -> Direction {
  array values = { top, topLeft, left };

  return static_cast<Direction>(
    ranges::distance(
      values.begin(), 
      ranges::max_element(values)
    )
  );
}


auto NeedlemanWunshAligner::align(i32 match, i32 mismatch, i32 gapPenalty) const -> vector<StringAlignment> {
  string_view first = this->getFirst();
  string_view second = this->getSecond();
  u64 M = first.length();
  u64 N = second.length();

  vector<i32> values((M + 1) * (N + 1), 0);

  auto table = mdspan{ values.data(), M+1, N+1 };

  auto score = [match, mismatch](char x, char y) -> i32 {
    return x == y and x != '-' and x != ' ' ? match : mismatch;
  };

  u64 iMax = M;
  u64 jMax = N;

  // Fill the matrix
  for(i64 i : views::iota(1ULL, M+1)) {
    for(i64 j : views::iota(1ULL, N+1)) {
      table[i, j] = ranges::max(
        array{
          table[i-1, j] - gapPenalty,
          table[i-1, j-1] + score(first[i-1], second[j-1]),
          table[i, j-1] - gapPenalty
        }
      );

      bool isLastRow = i == M;
      bool isLastColumn = j == N;

      if((isLastRow or isLastColumn) and table[i, j] > table[iMax, jMax]) {
        iMax = i;
        jMax = j;
      }
    }
  }

  auto containsMax = [&](tuple<u64, u64> index) -> bool {
    auto [i, j] = index;
    return table[i, j] == table[iMax, jMax];
  };

  auto traceback = [&](tuple<u64, u64> index) -> StringAlignment {
    auto [iMax, jMax] = index;

    string s1 = "";
    string s2 = "";

    u64 i = iMax;
    u64 j = jMax;

    // Perform traceback
    while(i > 0 and j > 0) {

      Direction cameFrom = NeedlemanWunshAligner::getDirectionOf(
        table[i-1, j] - gapPenalty,
        table[i-1, j-1] + score(first[i-1], second[j-1]),
        table[i, j-1] - gapPenalty
      );

      switch(cameFrom) {
        case Direction::TOP: {
          s2.push_back(0 < j or j < N ? '-' : ' ');
          s1.push_back(first[--i]);
          break;
        } case Direction::LEFT: {
          s2.push_back(second[--j]);
          s1.push_back(0 < i or i < M ? '-' : ' ');
          break;
        } case Direction::TOP_LEFT: {
          s2.push_back(second[--j]);
          s1.push_back(first[--i]);
          break;
        } default: break;
      }

    }

    return StringAlignment{
      views::concat(
        first.subview(iMax) | views::reverse,
        s1,
        first.subview(0, i) | views::reverse,
        views::repeat(' ', j)
      )
      | views::reverse
      | ranges::to<string>(),
      views::concat(
        second.subview(jMax) | views::reverse,
        s2,
        second.subview(0, j) | views::reverse,
        views::repeat(' ', i)
      )
      | views::reverse
      | ranges::to<string>(),
    };
  };

  return views::concat(
    views::cartesian_product(views::single(M), views::iota(1ULL, N)),
    views::cartesian_product(views::iota(1ULL, M), views::single(N)),
    views::single(tuple{ M, N })
  ) 
  | views::filter(containsMax)
  | views::transform(traceback)
  | ranges::to<vector>();

}

}
