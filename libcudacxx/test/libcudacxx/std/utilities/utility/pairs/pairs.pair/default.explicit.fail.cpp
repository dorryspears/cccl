//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// Before GCC 6, aggregate initialization kicks in.
// See https://stackoverflow.com/q/41799015/627587.
// UNSUPPORTED: gcc-5

// <utility>

// template <class T1, class T2> struct pair

// explicit(see-below) constexpr pair();

// This test checks the conditional explicitness of cuda::std::pair's default
// constructor as introduced by the resolution of LWG 2510.

#include <cuda/std/utility>

struct ImplicitlyDefaultConstructible
{
  ImplicitlyDefaultConstructible() = default;
};

struct ExplicitlyDefaultConstructible
{
  explicit ExplicitlyDefaultConstructible() = default;
};

cuda::std::pair<ImplicitlyDefaultConstructible, ExplicitlyDefaultConstructible> test1()
{
  return {};
} // expected-error 1 {{chosen constructor is explicit in copy-initialization}}
cuda::std::pair<ExplicitlyDefaultConstructible, ImplicitlyDefaultConstructible> test2()
{
  return {};
} // expected-error 1 {{chosen constructor is explicit in copy-initialization}}
cuda::std::pair<ExplicitlyDefaultConstructible, ExplicitlyDefaultConstructible> test3()
{
  return {};
} // expected-error 1 {{chosen constructor is explicit in copy-initialization}}
cuda::std::pair<ImplicitlyDefaultConstructible, ImplicitlyDefaultConstructible> test4()
{
  return {};
}

int main(int, char**)
{
  return 0;
}
