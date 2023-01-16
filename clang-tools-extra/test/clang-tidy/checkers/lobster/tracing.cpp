// RUN: %check_clang_tidy -std=c++11-or-later %s lobster-tracing %t

namespace kitten {

bool test1() { return false; }
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function kitten::test1 has no tracing tags [lobster-tracing]

bool test2() {
  // lobster-trace: cb-12345, tex-potato
  return false;
}
// CHECK-MESSAGES: :[[@LINE-4]]:6: warning: function kitten::test2 traces to  cb-12345, tex-potato [lobster-tracing]

}

class test3 {
  test3() {}
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: method test3::test3 has no tracing tags [lobster-tracing]

  bool test4() { return false; }
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: method test3::test4 has no tracing tags [lobster-tracing]
};

template <typename T>
T test5(T input) { return input; }
// CHECK-MESSAGES: :[[@LINE-1]]:3: warning: function test5 has no tracing tags [lobster-tracing]

int main(int argc, char **argv) { return 0; }
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: main function main has no tracing tags [lobster-tracing]
