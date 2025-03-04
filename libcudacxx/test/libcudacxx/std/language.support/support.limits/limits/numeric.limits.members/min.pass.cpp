//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test numeric_limits

// min()

#include <cuda/std/cassert>
#include <cuda/std/cfloat>
#include <cuda/std/climits>
#include <cuda/std/cstdint>
#include <cuda/std/limits>

#include "common.h"
#include "test_macros.h"

template <class T>
__host__ __device__ void test(T expected)
{
  assert(float_eq(cuda::std::numeric_limits<T>::min(), expected));
  assert(cuda::std::numeric_limits<T>::is_bounded || !cuda::std::numeric_limits<T>::is_signed);
  assert(float_eq(cuda::std::numeric_limits<const T>::min(), expected));
  assert(cuda::std::numeric_limits<const T>::is_bounded || !cuda::std::numeric_limits<const T>::is_signed);
  assert(float_eq(cuda::std::numeric_limits<volatile T>::min(), expected));
  assert(cuda::std::numeric_limits<volatile T>::is_bounded || !cuda::std::numeric_limits<volatile T>::is_signed);
  assert(float_eq(cuda::std::numeric_limits<const volatile T>::min(), expected));
  assert(cuda::std::numeric_limits<const volatile T>::is_bounded
         || !cuda::std::numeric_limits<const volatile T>::is_signed);
}

int main(int, char**)
{
#ifndef TEST_COMPILER_NVRTC
  test<wchar_t>(WCHAR_MIN);
#endif
  test<bool>(false);
  test<char>(CHAR_MIN);
  test<signed char>(SCHAR_MIN);
  test<unsigned char>(0);
#if TEST_STD_VER > 2017 && defined(__cpp_char8_t)
  test<char8_t>(0);
#endif
#ifndef _LIBCUDACXX_HAS_NO_UNICODE_CHARS
  test<char16_t>(0);
  test<char32_t>(0);
#endif // _LIBCUDACXX_HAS_NO_UNICODE_CHARS
  test<short>(SHRT_MIN);
  test<unsigned short>(0);
  test<int>(INT_MIN);
  test<unsigned int>(0);
  test<long>(LONG_MIN);
  test<unsigned long>(0);
  test<long long>(LLONG_MIN);
  test<unsigned long long>(0);
#if _CCCL_HAS_INT128()
  test<__int128_t>(-__int128_t(__uint128_t(-1) / 2) - 1);
  test<__uint128_t>(0);
#endif // _CCCL_HAS_INT128()
  test<float>(FLT_MIN);
  test<double>(DBL_MIN);
#ifndef _LIBCUDACXX_HAS_NO_LONG_DOUBLE
  test<long double>(LDBL_MIN);
#endif
#if _CCCL_HAS_NVFP16()
  test<__half>(__double2half(6.103515625e-05));
#endif // _CCCL_HAS_NVFP16
#if _CCCL_HAS_NVBF16()
  test<__nv_bfloat16>(__double2bfloat16(1.17549435082228750796873653722e-38));
#endif // _CCCL_HAS_NVBF16
#if _CCCL_HAS_NVFP8_E4M3()
  test<__nv_fp8_e4m3>(make_fp8_e4m3(0.015625));
#endif // _CCCL_HAS_NVFP8_E4M3()
#if _CCCL_HAS_NVFP8_E5M2()
  test<__nv_fp8_e5m2>(make_fp8_e5m2(0.000061035));
#endif // _CCCL_HAS_NVFP8_E5M2()
#if _CCCL_HAS_NVFP8_E8M0()
  test<__nv_fp8_e8m0>(make_fp8_e8m0(5.8774717541114375398436826861112283890933277838604376075437585313920e-39));
#endif // _CCCL_HAS_NVFP8_E8M0()
#if _CCCL_HAS_NVFP6_E2M3()
  test<__nv_fp6_e2m3>(make_fp6_e2m3(1.0));
#endif // _CCCL_HAS_NVFP6_E2M3()
#if _CCCL_HAS_NVFP6_E3M2()
  test<__nv_fp6_e3m2>(make_fp6_e3m2(0.25));
#endif // _CCCL_HAS_NVFP6_E3M2()
#if _CCCL_HAS_NVFP4_E2M1()
  test<__nv_fp4_e2m1>(make_fp4_e2m1(1.0));
#endif // _CCCL_HAS_NVFP4_E2M1()

  return 0;
}
