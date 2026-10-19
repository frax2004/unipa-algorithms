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
    return x == y ? match : mismatch;
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

  i64 i = iMax;
  i64 j = jMax;

  vector<StringAlignment> bestAlignments { };

  for(auto row : values | views::chunk(N+1)) {
    println("{}", row);
  }

  string s1 = first.substr(iMax) 
  | views::reverse 
  | ranges::to<string>();
  string s2 = second.substr(jMax) 
  | views::reverse 
  | ranges::to<string>();

  // Perform traceback
  while(i > 0 and j > 0) {

    Direction cameFrom;
    if(i > 0 and j > 0) {
      cameFrom = NeedlemanWunshAligner::getDirectionOf(
        table[i-1, j] - gapPenalty,
        table[i-1, j-1] + score(first[i-1], second[j-1]),
        table[i, j-1] - gapPenalty
      );
    } else if(i > 0) {
      cameFrom = Direction::TOP;
    } else if(j > 0) {
      cameFrom = Direction::LEFT;
    }

    switch(cameFrom) {
      case Direction::TOP: {
        s2.push_back(0 <= j or j < N ? '-' : ' ');
        s1.push_back(first[--i]);
        break;
      } case Direction::LEFT: {
        s2.push_back(second[--j]);
        s1.push_back(0 <= i or i < M ? '-' : ' ');
        break;
      } case Direction::TOP_LEFT: {
        s2.push_back(second[--j]);
        s1.push_back(first[--i]);
        break;
      } default: break;
    }

  }


  return bestAlignments;
  // return StringAlignment{
  //   s1 | views::reverse | ranges::to<string>(),
  //   s2 | views::reverse | ranges::to<string>()
  // };
}

}
