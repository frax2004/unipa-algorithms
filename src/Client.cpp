#include <pattern-discovery-for-life-sciences/NeedlemanWunshAligner.hpp>


auto Main(vector<string_view> args) -> void {
  if(args.size() != 2) {
    println("Usage: testing <seq1> <seq2>");
    return;
  }

  string s1 { args[0] };
  string s2 { args[1] };

  pdls::NeedlemanWunshAligner nwa { s1, s2 };
  vector<pdls::StringAlignment> alignments = nwa.align(1, 0, 1);

  for(auto x: alignments | views::transform(pdls::StringAlignment::toString))
    println("MatchMismatchAlignment(\"{}\", \"{}\") =\n{}", s1, s2, x);

  vector m1 = {
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


}
