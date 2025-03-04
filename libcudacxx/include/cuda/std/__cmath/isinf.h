//===----------------------------------------------------------------------===//
//
// Part of libcu++, the C++ Standard Library for your entire system,
// under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___CMATH_ISINF_H
#define _LIBCUDACXX___CMATH_ISINF_H

#include <cuda/std/detail/__config>

#if defined(_CCCL_IMPLICIT_SYSTEM_HEADER_GCC)
#  pragma GCC system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_CLANG)
#  pragma clang system_header
#elif defined(_CCCL_IMPLICIT_SYSTEM_HEADER_MSVC)
#  pragma system_header
#endif // no system header

#include <cuda/std/__cmath/fp_utils.h>
#include <cuda/std/__cmath/isnan.h>
#include <cuda/std/__concepts/concept_macros.h>
#include <cuda/std/__type_traits/is_constant_evaluated.h>
#include <cuda/std/__type_traits/is_floating_point.h>
#include <cuda/std/__type_traits/is_integral.h>
#include <cuda/std/limits>

// MSVC and clang cuda need the host side functions included
#if _CCCL_COMPILER(MSVC) || _CCCL_CUDA_COMPILER(CLANG)
#  include <math.h>
#endif // _CCCL_COMPILER(MSVC) || _CCCL_CUDA_COMPILER(CLANG)

_LIBCUDACXX_BEGIN_NAMESPACE_STD

template <class _Tp>
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool __isinf_impl(_Tp __x) noexcept
{
  static_assert(_CCCL_TRAIT(is_floating_point, _Tp), "Only standard floating-point types are supported");
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
    return ::isinf(__x);
  }
  if (_CUDA_VSTD::isnan(__x))
  {
    return false;
  }
  return __x > numeric_limits<_Tp>::max() || __x < numeric_limits<_Tp>::lowest();
}

_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(float __x) noexcept
{
#if defined(_CCCL_BUILTIN_ISINF) && !_CCCL_CUDA_COMPILER(NVCC) && !_CCCL_CUDA_COMPILER(NVRTC)
  return _CCCL_BUILTIN_ISINF(__x);
#elif defined(_CCCL_BUILTIN_ISINF)
  // Workaround for nvbug 5120680
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
    return _CCCL_BUILTIN_ISINF(__x);
  }
  return _CCCL_BUILTIN_ISINF(__x) && !_CCCL_BUILTIN_ISNAN(__x);
#elif _LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST()
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
    return ::isinf(__x);
  }
  return (_CUDA_VSTD::__cccl_fp_get_storage(__x) & __cccl_fp32_exp_mant_mask) == __cccl_fp32_exp_mask;
#else // ^^^ _LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST() ^^^ / vvv !_LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST() vvv
  return _CUDA_VSTD::__isinf_impl(__x);
#endif // ^^^ !_CCCL_BUILTIN_ISINF ^^^
}

_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(double __x) noexcept
{
#if defined(_CCCL_BUILTIN_ISINF) && !_CCCL_CUDA_COMPILER(NVCC) && !_CCCL_CUDA_COMPILER(NVRTC)
  return _CCCL_BUILTIN_ISINF(__x);
#elif defined(_CCCL_BUILTIN_ISINF)
  // Workaround for nvbug 5120680
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
    return _CCCL_BUILTIN_ISINF(__x);
  }
  return _CCCL_BUILTIN_ISINF(__x) && !_CCCL_BUILTIN_ISNAN(__x);
#elif _LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST()
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
    return ::isinf(__x);
  }
  return (_CUDA_VSTD::__cccl_fp_get_storage(__x) & __cccl_fp64_exp_mant_mask) == __cccl_fp64_exp_mask;
#else // ^^^ _LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST() ^^^ / vvv !_LIBCUDACXX_HAS_CONSTEXPR_BIT_CAST() vvv
  return _CUDA_VSTD::__isinf_impl(__x);
#endif // ^^^ !_CCCL_BUILTIN_ISINF ^^^
}

#if !defined(_LIBCUDACXX_HAS_NO_LONG_DOUBLE)
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(long double __x) noexcept
{
#  if defined(_CCCL_BUILTIN_ISINF)
  return _CCCL_BUILTIN_ISINF(__x);
#  else // ^^^ _CCCL_BUILTIN_ISINF ^^^ / vvv !_CCCL_BUILTIN_ISINF vvv
  return _CUDA_VSTD::__isinf_impl(__x);
#  endif // defined(_CCCL_BUILTIN_ISINF)
}
#endif // !_LIBCUDACXX_HAS_NO_LONG_DOUBLE

#if defined(_LIBCUDACXX_HAS_NVFP16)
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__half __x) noexcept
{
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
#  if _CCCL_STD_VER >= 2020 && _CCCL_CUDACC_BELOW(12, 3)
    // this is a workaround for nvbug 4362808
    return !::__hisnan(__x) && ::__hisnan(__x - __x);
#  else // ^^^ C++20 && below 12.3 ^^^ / vvv C++17 or 12.3+ vvv
    return ::__hisinf(__x) != 0;
#  endif // _CCCL_STD_VER <= 2017 || _CCCL_CUDACC_BELOW(12, 3)
  }
  return (_CUDA_VSTD::__cccl_fp_get_storage(__x) & __cccl_nvfp16_exp_mant_mask) == __cccl_nvfp16_exp_mask;
}
#endif // _LIBCUDACXX_HAS_NVFP16

#if defined(_LIBCUDACXX_HAS_NVBF16)
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_bfloat16 __x) noexcept
{
  if (!_CUDA_VSTD::__cccl_default_is_constant_evaluated())
  {
#  if _CCCL_STD_VER >= 2020 && _CCCL_CUDACC_BELOW(12, 3)
    // this is a workaround for nvbug 4362808
    return !::__hisnan(__x) && ::__hisnan(__x - __x);
#  else // ^^^ C++20 && below 12.3 ^^^ / vvv C++17 or 12.3+ vvv
    return ::__hisinf(__x) != 0;
#  endif // _CCCL_STD_VER <= 2017 || _CCCL_CUDACC_BELOW(12, 3)
  }
  return (_CUDA_VSTD::__cccl_fp_get_storage(__x) & __cccl_nvbf16_exp_mant_mask) == __cccl_nvbf16_exp_mask;
}
#endif // _LIBCUDACXX_HAS_NVBF16

#if _CCCL_HAS_NVFP8_E4M3()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp8_e4m3) noexcept
{
  return false;
}
#endif // _CCCL_HAS_NVFP8_E4M3()

#if _CCCL_HAS_NVFP8_E5M2()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp8_e5m2 __x) noexcept
{
  return (__x.__x & __cccl_nvfp8_e5m2_exp_mant_mask) == __cccl_nvfp8_e5m2_exp_mask;
}
#endif // _CCCL_HAS_NVFP8_E5M2()

#if _CCCL_HAS_NVFP8_E8M0()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp8_e8m0) noexcept
{
  return false;
}
#endif // _CCCL_HAS_NVFP8_E8M0()

#if _CCCL_HAS_NVFP6_E2M3()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp6_e2m3) noexcept
{
  return false;
}
#endif // _CCCL_HAS_NVFP6_E2M3()

#if _CCCL_HAS_NVFP6_E3M2()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp6_e3m2) noexcept
{
  return false;
}
#endif // _CCCL_HAS_NVFP6_E3M2()

#if _CCCL_HAS_NVFP4_E2M1()
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(__nv_fp4_e2m1) noexcept
{
  return false;
}
#endif // _CCCL_HAS_NVFP4_E2M1()

_CCCL_TEMPLATE(class _Tp)
_CCCL_REQUIRES(_CCCL_TRAIT(is_integral, _Tp))
_CCCL_NODISCARD _LIBCUDACXX_HIDE_FROM_ABI constexpr bool isinf(_Tp) noexcept
{
  return false;
}

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___CMATH_ISINF_H
