#include <NeedlemanWunshAligner.hpp>

using namespace pdls;

auto Main(vector<string_view> args) -> void {
  auto m1 = vector{
    3, 1 , 4 , 6, 5, 1, 2, 2 , 2, 9, 3, 3 , 2 , 3, 6, 1 , 2, 3,
    8, 2 , 3 , 3, 3, 3, 3, 2 , 4, 3, 8, 2 , 2 , 4, 3, 3 , 3, 4,
    2, 10, 1 , 1, 1, 4, 4, 1 , 3, 1, 2, 7 , 5 , 2, 1, 2 , 4, 2,
    3, 1 , 0 , 1, 1, 2, 2, 2 , 1, 2, 2, 6 , 14, 1, 1, 2 , 2, 1,
    2, 5 , 6 , 9, 4, 1, 1, 2 , 2, 6, 3, 2 , 1 , 3, 9, 1 , 1, 3,
    3, 2 , 1 , 1, 1, 3, 3, 2 , 3, 1, 3, 2 , 2 , 4, 1, 13, 3, 4,
    3, 4 , 1 , 1, 1, 8, 6, 1 , 7, 1, 3, 3 , 2 , 3, 1, 3 , 6, 3,
    3, 1 , 3 , 5, 6, 1, 2, 3 , 2, 5, 3, 2 , 1 , 3, 5, 1 , 2, 3,
    2, 1 , 3 , 2, 2, 1, 0, 10, 1, 2, 2, 1 , 2 , 4, 2, 2 , 0, 4,
    3, 1 , 10, 6, 3, 1, 1, 3 , 1, 4, 3, 1 , 0 , 2, 6, 1 , 1, 2,
    4, 1 , 5 , 4, 4, 1, 1, 4 , 1, 4, 4, 2 , 0 , 3, 4, 2 , 1, 3,
    4, 2 , 2 , 3, 3, 3, 3, 4 , 4, 3, 4, 2 , 1 , 8, 3, 4 , 3, 8,
    8, 2 , 3 , 3, 3, 3, 3, 2 , 4, 3, 8, 2 , 2 , 4, 3, 3 , 3, 4,
    2, 7 , 1 , 2, 2, 3, 3, 1 , 3, 3, 2, 10, 6 , 2, 2, 2 , 3, 2
  };

  if(args.size() != 2) {
    println("Usage: testing <seq1> <seq2>");
    return;
  }

  auto s1 = string{ args[0] };
  auto s2 = string{ args[1] };

  println("Align({}, {}) = ", s1, s2);

  auto nwa = NeedlemanWunshAligner{ s1, s2 };
  StringAlignment alignment = nwa.getMatchMismatchAlignment(1, 0, 1);

  println("{}", alignment.toString());
}
