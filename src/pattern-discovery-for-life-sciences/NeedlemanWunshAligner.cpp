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


  auto traceback = [&](this auto&& self, vector<StringAlignment>& alignments, u64 i, u64 j, string s1, string s2) -> void {
    if(i == 0 or j == 0) {
      // base case
      alignments.push_back({
        views::concat(
          s1,
          first.subview(0, i) | views::reverse,
          views::repeat(' ', j)
        )
        | views::reverse
        | ranges::to<string>(),
        views::concat(
          s2,
          second.subview(0, j) | views::reverse,
          views::repeat(' ', i)
        )
        | views::reverse
        | ranges::to<string>(),
      });
    } else {
      i64 max = ranges::max(
        array{
          table[i-1, j] - gapPenalty,
          table[i-1, j-1] + score(first[i-1], second[j-1]),
          table[i, j-1] - gapPenalty
        }
      );
      
      struct direction_info {
        u64 i, j;
        i64 value;
        Direction direction;
      };

      auto dinfo = array{
        direction_info{ i-1, j, table[i-1, j] - gapPenalty, Direction::TOP },
        direction_info{ i-1, j-1, table[i-1, j-1] + score(first[i-1], second[j-1]), Direction::TOP_LEFT },
        direction_info{ i, j-1, table[i, j-1] - gapPenalty, Direction::LEFT }
      };

      for(auto info : dinfo) {
        if(info.value != max) continue;

        switch(info.direction) {
          case Direction::TOP: {
            self(alignments, info.i, info.j, s1 + first[i-1], s2 + (0 < j or j < N ? '-' : ' '));
            break;
          } case Direction::LEFT: {
            self(alignments, info.i, info.j, s1 + (0 < i or i < M ? '-' : ' '), s2 + second[j-1]);
            break;
          } case Direction::TOP_LEFT: {
            self(alignments, info.i, info.j, s1 + first[i-1], s2 + second[j-1]);
            break;
          } default: break;
        }
      }
    }
  };

  vector<StringAlignment> bestAlignments { };

  auto indexes = views::concat(
    views::cartesian_product(views::single(M), views::iota(1ULL, N)),
    views::cartesian_product(views::iota(1ULL, M), views::single(N)),
    views::single(tuple{ M, N })
  ) 
  | views::filter(containsMax);


  for(auto [i, j] : indexes) {
    traceback(
      bestAlignments, 
      i, 
      j, 
      first.substr(i-1) 
      | views::reverse 
      | ranges::to<string>(), 
      second.substr(j-1) 
      | views::reverse 
      | ranges::to<string>()
    );
  }

  return bestAlignments;
}

}
