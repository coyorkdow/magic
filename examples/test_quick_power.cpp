//
// Created by YorkDow Co on 2021/12/22.
//

#include "test_quick_power.h"

using namespace magic;

void TestPower() {
  static_assert(Power<2, 10, 998244353>::Calculate() == 1024, "");
  static_assert(Power<3, 20, 998244353>::Calculate() == 492051342, "");
}
