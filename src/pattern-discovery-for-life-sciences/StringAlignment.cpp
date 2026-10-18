#include <pattern-discovery-for-life-sciences/StringAlignment.hpp>

namespace pdls {
  StringAlignment::StringAlignment(string first, string second)
    : mFirst{ first }
    , mSecond{ second }
  {
    if(this->mFirst.empty() or this->mSecond.empty()) {
      throw invalid_argument{ "Alignment string cannot be empty." };
    }
  }

  auto StringAlignment::matches() const -> size_t {
    i64 l12 = max(0LL, static_cast<i64>(this->mFirst.length()) - static_cast<i64>(this->mSecond.length()));
    i64 l21 = max(0LL, static_cast<i64>(this->mSecond.length()) - static_cast<i64>(this->mFirst.length()));
    auto w1 = views::concat(this->mFirst, views::repeat(' ', l12));
    auto w2 = views::concat(this->mSecond, views::repeat(' ', l21));

    auto isMatch = [](auto pair) -> bool {
      auto [a, b] = pair;
      return a == b and a != ' ' and a != '-';
    };

    return ranges::distance(
      views::zip(w1, w2) 
      | views::filter(isMatch)
    );
  }


  auto StringAlignment::mismatches() const -> size_t {
    i64 l12 = max(0LL, static_cast<i64>(this->mFirst.length()) - static_cast<i64>(this->mSecond.length()));
    i64 l21 = max(0LL, static_cast<i64>(this->mSecond.length()) - static_cast<i64>(this->mFirst.length()));
    auto w1 = views::concat(this->mFirst, views::repeat(' ', l12));
    auto w2 = views::concat(this->mSecond, views::repeat(' ', l21));

    auto isMismatch = [](auto pair) -> bool {
      auto [a, b] = pair;
      return a != b and a != ' ' and a != '-' and b != ' ' and b != '-';
    };

    return ranges::distance(
      views::zip(w1, w2) 
      | views::filter(isMismatch)
    );
  }

  auto StringAlignment::getFirst() const -> string_view {
    return this->mFirst;
  }

  auto StringAlignment::getSecond() const -> string_view {
    return this->mSecond;
  }

  auto StringAlignment::toString() const -> string {
    i64 l12 = max(0LL, static_cast<i64>(this->mSecond.length()) - static_cast<i64>(this->mFirst.length()));
    i64 l21 = max(0LL, static_cast<i64>(this->mFirst.length()) - static_cast<i64>(this->mSecond.length()));
    auto w1 = views::concat(this->mFirst, views::repeat(' ', l12));
    auto w2 = views::concat(this->mSecond, views::repeat(' ', l21));

    auto toMatch = [](auto pair) -> char {
      auto [a, b] = pair;
      return a == b and a != ' ' and a != '-' ? '|' : ' ';
    };

    return format(
      "{:s}\n{:s}\n{:s}", 
      w1,
      views::zip(w1, w2)
      | views::transform(toMatch),
      w2
    );
  }

}