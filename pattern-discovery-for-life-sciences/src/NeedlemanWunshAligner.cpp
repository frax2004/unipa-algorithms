#include <NeedlemanWunshAligner.hpp>

namespace pdls {
  NeedlemanWunshAligner::NeedlemanWunshAligner(string column, string row)
    : mRow { row }
    , mColumn { column }
  {
    if(this->mColumn.empty() or this->mRow.empty()) {
      throw invalid_argument{ "Alignment strings cannot be empty." };
    }
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


  auto NeedlemanWunshAligner::getMatchMismatchAlignment(i32 match, i32 mismatch, i32 gapPenalty) const -> StringAlignment {
    auto values = vector<i32>(this->mRow.length() * this->mColumn.length());
    auto original = values;

    auto score = [match, mismatch](char x, char y) -> i32 {
      return x == y ? match : mismatch;
    };

    auto table = mdspan{ 
      values.data(), 
      this->mRow.length(), 
      this->mColumn.length() 
    };

    auto $ = [table](i64 i, i64 j) -> i32 {
      return i < 0 or j < 0 ? 0 : table[i, j];
    };

    // Pre-process
    for(i64 i : views::indices(table.extent(0))) {
      for(i64 j : views::indices(table.extent(1))) {
        table[i, j] = score(this->mRow[i], this->mColumn[j]);
      }
    }

    // Rest of the matrix
    for(i64 i : views::indices(table.extent(0))) {
      for(i64 j : views::indices(table.extent(1))) {
        table[i, j] = ranges::max(
          array{
            $(i-1, j) - gapPenalty,
            $(i-1, j-1) + score(this->mRow[i], this->mColumn[j]),
            $(i, j-1) - gapPenalty
          }
        );
      }
    }

    string s1 = "";
    string s2 = "";

    i64 i = static_cast<i64>(table.extent(0))-1;
    i64 j = static_cast<i64>(table.extent(1))-1;

    // Perform traceback
    while(i > 0 or j > 0) {
      println("({}, {})", i, j);
      Direction cameFrom = NeedlemanWunshAligner::getDirectionOf(
        $(i-1, j) - gapPenalty,
        $(i-1, j-1) + score(this->mRow[i], this->mColumn[j]),
        $(i, j-1) - gapPenalty
      );

      switch(cameFrom) {
        case Direction::TOP: {
          s1.push_back('-');
          s2.push_back(this->mRow[i--]);
          break;
        } case Direction::LEFT: {
          s1.push_back(this->mColumn[j--]);
          s2.push_back('-');
          break;
        } case Direction::TOP_LEFT: {
          s1.push_back(this->mColumn[j--]);
          s2.push_back(this->mRow[i--]);
          break;
        } default: break;
      }
    }

    s1.push_back(this->mColumn[0]);
    s2.push_back(this->mRow[0]);

    return StringAlignment{
      s1
      | views::reverse
      | ranges::to<string>(),
      s2
      | views::reverse
      | ranges::to<string>()
    };
  }
}
