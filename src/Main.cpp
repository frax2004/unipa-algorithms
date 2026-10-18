#include <Common.hpp>

auto Main(vector<string_view> args) -> void;

auto main(int argc, const char** argv) -> int {
  srand(time(nullptr));

  set_terminate([]() { 
    println("Terminate called."); 
    exit(0);
  });
  
  signal(SIGSEGV, [](int signum) { 
    println("Segmentation Fault."); 
    exit(0);
  });

  try {
    Main(vector<string_view>{ argv+1, argv + argc });
  } catch(const exception& ex) {
    println("{}", ex.what());
    return 0;
  }

  return 0;
}
