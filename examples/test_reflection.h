//
// Created by YorkDow Co on 2021/12/22.
//

#ifndef MAGIC_EXAMPLES_TEST_REFLECTION_H_
#define MAGIC_EXAMPLES_TEST_REFLECTION_H_

#include "reflection.h"

class A {
 public:
  A() : f(0.5),
        pf(new float{14.5}),
        vi({1, 2, 3}),
        vd({1.1, 2.2, 3.3}),
        s("123_f3") {}

 private:
  friend class magic::TypeFieldsScheme<A>;
  float f;
  float *pf;
  std::vector<int> vi;
  std::vector<double> vd;
  std::string s;
};

REGISTER_FIELDS(A,
                FIELD(vi, vi tag),
                FIELD(vd, vd tag),
                FIELD(s, s tag),
                FIELD_NO_TAG(f),
                FIELD_NO_TAG(pf));

void TestReflection();

#endif//MAGIC_EXAMPLES_TEST_REFLECTION_H_
